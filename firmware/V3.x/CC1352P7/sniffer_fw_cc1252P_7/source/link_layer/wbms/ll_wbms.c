/******************************************************************************
*  Filename:       ll_wbms.c
*
*  Description:    Source code for the wBMS Link Layer
*
* Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

#include "ll_wbms.h"
#include "ll_wbms_frame.h"
#include "ll_wbms_channel.h"
#include "ll_timer.h"
#include "../ll_common.h"
#include "phy_manager.h"
#include "phy_if_wbms.h"
#include "../radio_if.h"

#define TIMESTAMP_OFFSET                       5                // The offset from last byte of the PDU payload
#define PDU_HEADER_LENGTH                      3
#define PDU_HEADER_LENGTH_OFFSET               0

#define EXTRA_SNIFFER_INFO_DL_MASK             0x80
#define EXTRA_SNIFFER_INFO_CHANNEL_MASK        0x7E
#define EXTRA_SNIFFER_INFO_CHANNEL_SHIFT       1

 #define WBMS_CHANNEL_SWITCH_TIME_MARGIN       410              // Margin added to the measured channel switch time. Measured margin (logic analyzer) with 210 is ~150 us
                                                                // Observed difference between programmed timer value and RAT timer value when callback happens is ~21 us
                                                                // Added 200us (210 -> 410) extra margin because of observed jitter in DL packet period (28.51ms -> 28.625ms)
                                                                // TODO: Discuss this jitter with WBMS team

#define WBMS_CHANNEL_SWITCH_TIME_MEASURED      290              // The time in us from RAT timer callback function is called until RX is on (RFC_GPO0 is high) is measured to ~290 us (LAUNCHXL-CC1352R Rev E) . 

#define WBMS_MISSED_PACKET_TIME_MARGIN         500              // 500 us constant time margin for detection of missed first packet (DownLink) in each superframe. 

// Connection interval timing
static uint32_t LLWbms_anchorNext = 0;                          // Time for next anchor (in microseconds as absolute time from RAT timer was started)
static uint32_t LLWbms_superframeInterval =    15400;           // Superframe interval (in microseconds). This is updated on reception of the first Scan Request
static uint32_t LLWbms_superframeStart = 0;                     // Superframe start (in microseconds as absolute time from RAT timer was started) 
static uint8_t LLWbms_firstAnchor = true;                       // Set to true until the first DL (first superframe)

static uint32_t LLWbms_superFrameCount = 0;                     // Number of superframes
static uint32_t LLWbms_ConnectionStartInstant = 0;              // Instant of first superframe in connected state 
static ScanRequestInfo_Obj LLWbms_scanInfo;
static PairingRequestInfo_Obj LLWbms_pairingInfo;


typedef enum LL_State
{
    State_Scanning = 0,
    State_Scan_Received = 1,
    State_Enter_Connected = 2,
    State_Connected = 3
} LL_State;


#ifdef JOIN_ENABLE
static LL_State LLWbms_state = State_Scanning;
#else
static LL_State LLWbms_state = State_Connected;          // Only connected state is supported for now
#endif


static uint16_t LLWbms_appendExtraSnifferInfo(uint8_t* packetData, uint16_t packetLength);
static void LLWbms_adjustAnchorPoint(uint8_t* pTimeStamp);
static void LLWbms_superframeTimerEventHandler(uint32_t ratTime);
static void LLWbms_scheduleNextSuperframe(uint32_t nextSuperframe);
static uint32_t LLWbms_calculateMissedPacketTimeMargin(void);
static bool LLWbms_filterPairingRequest(ScanRequestInfo_Obj* pScanReq, PairingRequestInfo_Obj* pPairingReq);
static bool LLWbms_scanReqIndicatesRejoin(ScanRequestInfo_Obj* pScanReq);


//! \brief Process Packet 
//!
//! \param[in] packetData
//!            PDU data. The expected format of the packet shall be as written to memory by RF Core:
//!            Header, Payload, 4 bytes CRC, RSSI, 4 bytes Timestamp, 1 byte status 
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return Length of the packet after packet processing
uint16_t LLWbms_processPacket(uint8_t* packetData, uint16_t packetLength)
{
    uint8_t* pduHeader = packetData;
    uint8_t pduLength = pduHeader[PDU_HEADER_LENGTH_OFFSET];
    uint8_t* pTimestamp = &packetData[pduLength + TIMESTAMP_OFFSET]; // Radio timer timestamp bytes
    bool bValidScanReq = false;
    
    switch(LLWbms_state)
    {
    case State_Scanning:
        
        // Search for scan request packets in scan state
        if(LLWbms_isScanRequest(packetData, packetLength))
        {
            bValidScanReq = LLWbms_parseScanRequest(&LLWbms_scanInfo, packetData, packetLength);
            if(bValidScanReq)
            {
                // Update superframe interval
#ifdef FIXED_SF_PERIOD
                LLWbms_superframeInterval = 3850;
#else
                LLWbms_superframeInterval = LLWbms_scanInfo.superFrameInterval;
#endif
                // Check if the scan request indicates a network rejoin. 
                // If so, pairing is skipped and next state shall be State_Enter_Connected.
                if(LLWbms_scanReqIndicatesRejoin(&LLWbms_scanInfo))
                {
                    // Calculate superframe instant of first connected state superframe
                    LLWbms_ConnectionStartInstant = LLWbms_superFrameCount + LLWbms_scanInfo.switchCounter;
                    // Move to State_Enter_Connected
                    LLWbms_state = State_Enter_Connected;
                }
                else
                {
                    // Rejoin is not indicated. Move to scan received state
                    LLWbms_state = State_Scan_Received;
                }
            }
        }
        
        break;
    
    case State_Scan_Received:
        
        // After a scan request is received, search for either: 
        // - pairing requests (these will be received when network is formed the first time), or
        // - scan requests indicating that a rejoin shall happen (network is restored from NV info, and pairing is bypassed)
        
        if(LLWbms_isPairingRequest(packetData, packetLength))
        {
            bool bValidPairingReq = LLWbms_parsePairingRequest(&LLWbms_pairingInfo, packetData, packetLength);
            // Proceed if the frame is valid and pass filter criterias
            if(bValidPairingReq && LLWbms_filterPairingRequest(&LLWbms_scanInfo, &LLWbms_pairingInfo))
            {
                // Calculate superframe instant of first connected state superframe
                LLWbms_ConnectionStartInstant = LLWbms_superFrameCount + LLWbms_pairingInfo.switchCounter;
                
                 // Move to enter connected state after a successful pairing request is received
                LLWbms_state = State_Enter_Connected;
            }
        }
        else if(LLWbms_isScanRequest(packetData, packetLength))
        {
            bValidScanReq = LLWbms_parseScanRequest(&LLWbms_scanInfo, packetData, packetLength);
            // Proceed if the frame is valid and indicates a rejoin
            if(bValidScanReq && LLWbms_scanReqIndicatesRejoin(&LLWbms_scanInfo))
            {
                // Calculate superframe instant of first connected state superframe
                LLWbms_ConnectionStartInstant = LLWbms_superFrameCount + LLWbms_scanInfo.switchCounter;
                
                 // Move to enter connected state after a successful pairing request is received
                LLWbms_state = State_Enter_Connected;
            }
        }
        
        break;
        
    case State_Enter_Connected:    
    
        break;
        
    case State_Connected:    
    
        break;
        
    }
    
    // Adjust anchor point for next superframe if this is a downlink frame
    if(LLWbms_isDownLinkFrame(packetData, packetLength))
    {
        LLWbms_adjustAnchorPoint(pTimestamp);
    }
    
    // Append extra sniffer info byte to the packet
    // This function will add one byte to the packet. This is safe to do here since the buffer holding
    // packetData (allocated in dataTask) is larger than any WBMS packet
    uint16_t newPacketLength = LLWbms_appendExtraSnifferInfo(packetData, packetLength);
        
    return newPacketLength;
}

//! \brief Reset Link Layer
//!
void LLWbms_reset(void)
{
    LLWbms_superframeStart = 0;
    LLWbms_firstAnchor = true;
    LLWbms_superFrameCount = 0;
    LLWbms_channelReset();
    
#ifdef JOIN_ENABLE
    LLWbms_state = State_Scanning;
    LLWbms_setActiveChannelTable(ChannelTable_Config);
#else
    LLWbms_state = State_Connected;
    LLWbms_setActiveChannelTable(ChannelTable_Data);
#endif
    
    // Register superframe timer callback
    LLTimer_registerCb(SUPERFRAME_TIMER, LLWbms_superframeTimerEventHandler);
}


//! \brief Force data channel table
//!
//!        The specified channel hop table will be used instead of the data channel table
//!        specified in the received Pairing Request packets. This is intended to be used
//!        for WBMS security level 1 and 2 where the channel table information in Pairing Request
//!        packets is encrypted. The specified channel table will be used until Link Layer is reset,
//!        that is LLWbms_reset is called. 
//!
//! \param[in] channelTable
//!            Pointer to the channel table
//!
//! \param[in] length
//!            Length of the channel table
//!
//! \return none 
void LLWbms_forceDataChannelTable(uint8_t* channelTable, uint8_t length)
{
    // Unlock data channel table for update
    LLWbms_channelUnlockDataTable();
    
    // Update data channel table
    LLWbms_channelUpdateDataTable(channelTable, length);
    
    // Lock data channel table for further updates
    LLWbms_channelLockDataTable();
}


//! \brief Append the 'Extra Sniffer Info' byte
//!
//!        This function appends an 'extra sniffer info' byte in the following format: 
//!        b7: Is Downlink? (1 means Yes, and 0 means No)
//!        b1-b6: Channel
//!        b0: Reserved
//!
//! \param[in] packetData
//!            PDU data
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return Updated packet length
uint16_t LLWbms_appendExtraSnifferInfo(uint8_t* packetData, uint16_t packetLength)
{
    uint8_t* pExtraSnifferInfo = packetData + packetLength;                // The last byte after status byte shall contain extra sniffer info
    *pExtraSnifferInfo = 0;
    
    // Set Downlink bit
    if(LLWbms_isDownLinkFrame(packetData, packetLength))
    {
        *pExtraSnifferInfo |= EXTRA_SNIFFER_INFO_DL_MASK; 
    }
    
    // Set channel bit field
    uint8_t currentChannel = LLWbms_channelGetCurrent();
    *pExtraSnifferInfo |= currentChannel << EXTRA_SNIFFER_INFO_CHANNEL_SHIFT;
    
    return packetLength + 1;
}


//! \brief Adjust anchor point for first packet in a superframe
//!
//! \param[in] pTimeStamp
//!            Timestamp of packet 
//! 
//! \return none
static void LLWbms_adjustAnchorPoint(uint8_t* pTimeStamp)
{
    uint32_t anchorAdjusted = 0;
    
    // Get the captured timestamp for this packet
    anchorAdjusted = LLCommon_calculatePacketStartTime(pTimeStamp);
    
    // If this is the first DL packet received set superframe start to the timestamp of this packet
    if(LLWbms_firstAnchor)
    {
        LLWbms_superframeStart = anchorAdjusted;
        LLWbms_firstAnchor = false;
    }
    
    // Calculate the time margin for when to consider the first M->S packet lost
    uint32_t missedPacketTimeMargin = LLWbms_calculateMissedPacketTimeMargin();
    
    // If time delta between DL packet and start of superframe is within allowed time margin, 
    // adjust anchor point and reschedule next expected superframe accordingly. 
    // If not, consider the DL packet as lost.    
    uint32_t timeDelta = LLCommon_subtractTimerValue(anchorAdjusted, LLWbms_superframeStart);
    if(timeDelta < missedPacketTimeMargin)
    {
        // set expected time for next anchor
        LLWbms_anchorNext = LLCommon_addTimerValue(anchorAdjusted, LLWbms_superframeInterval);
    
        // Schedule next superframe
        LLWbms_scheduleNextSuperframe(LLWbms_anchorNext);
    }
}


//! \brief Callback for superframe timer expiration
//!
//!        This function:
//!        - Stops RX, 
//!        - Sets channel for next superframe, 
//!        - Restarts RX and schedules the superframe.
//!
//! \param[in] ratTime
//!            Radio timer value at expiration
//! 
//! \return none
static void LLWbms_superframeTimerEventHandler(uint32_t ratTime)
{    
    LLWbms_superframeStart = LLCommon_convertRadioTimeToUs(ratTime);
    
    uint8_t channel = 0;
    
    // Check if the switch to connected state should happen
    if((LLWbms_state == State_Enter_Connected) && (LLWbms_ConnectionStartInstant == LLWbms_superFrameCount))
    {
        // Connected state starts in next superframe
        // Select the hopping table for data channels for next superframe
        LLWbms_setActiveChannelTable(ChannelTable_Data);
        // Do not advance to next channel for the first superframe in connected state
        // (use first channel in connected state hopping table)
        channel = LLWbms_channelGetCurrent();
        
        LLWbms_state = State_Connected; 
    }
    else
    {
        // Advance to next channel
        channel = LLWbms_channelGetNext();
    }
    
    LLWbms_superFrameCount++;
        
    // Stop RX
    RadioIf_stopRxCmd();
           
    // Set new channel on currently used PHY
    uint8_t phyIndex = PhyManager_getPhyIndex();
    PhyIf_setChannelWbms(phyIndex, channel);
    
    // Run the radio frequency command for new frequency to take effect 
    RadioIf_runFsCmd();
    
    // Restart RX 
    RadioIf_startRxCmd();
    
    // Schedule next superframe
    //LLWbms_anchorNext += LLWbms_superframeInterval;
    LLWbms_anchorNext = LLCommon_addTimerValue(LLWbms_anchorNext, LLWbms_superframeInterval);
    LLWbms_scheduleNextSuperframe(LLWbms_anchorNext);
}


//! \brief Schedule next superframe
//!
//! \param[in] nextSuperframe
//!            Time of next superframe (absolute time in microseconds)
//! 
//! \return none
static void LLWbms_scheduleNextSuperframe(uint32_t nextSuperframe)
{
    // Subtract the calculated time margin to do the actual channel switch to make sure channel switch happens early enough
    // Todo, check window widenign algoritm in BLE link layer and check if similar should be done here
    uint32_t defaultChannelSwitchTimeMargin = WBMS_CHANNEL_SWITCH_TIME_MEASURED + WBMS_CHANNEL_SWITCH_TIME_MARGIN;
    nextSuperframe = LLCommon_subtractTimerValue(nextSuperframe, defaultChannelSwitchTimeMargin);
    
    LLTimer_stop(SUPERFRAME_TIMER);
    LLTimer_setTimeout(SUPERFRAME_TIMER, nextSuperframe);
    LLTimer_start(SUPERFRAME_TIMER);
}


//! \brief Calculate time margin from connection event timer callback until the first packet in the superframe is considered lost
//! 
//! \return none
uint32_t LLWbms_calculateMissedPacketTimeMargin(void)
{
    // Constant packet time margin. If a DL packet is received after this time the DL packet is considered lost
    // and it shall not be used for resynchronization. 
    return WBMS_CHANNEL_SWITCH_TIME_MEASURED + WBMS_CHANNEL_SWITCH_TIME_MARGIN + WBMS_MISSED_PACKET_TIME_MARGIN;
}


//! \brief Filter a received pairing request based on information from first Scan Request and the 
//!        Pairing Request itself. 
//!
//! \param[in] ScanRequestInfo_Obj
//!            Pointer to Scan Request Information struct
//!
//! \param[in] PairingRequestInfo_Obj
//!            Pointer to Pairing Request Information struct
//! 
//! \return True if the pairing request passes the filter, and false otherwise
static bool LLWbms_filterPairingRequest(ScanRequestInfo_Obj* pScanReq, PairingRequestInfo_Obj* pPairingReq)
{
    // The Scan Request and Pairing Request must be from same Network ID, and
    // Switch Indicator must be set to true. 
    return (pScanReq->networkId == pPairingReq->networkId) && pPairingReq->switchIndicator;
}


//! \brief Check if the scan request indicates rejoin 
//!        (skip pairing and move directly to connected state)
//!
//! \param[in] ScanRequestInfo_Obj
//!            Pointer to Scan Request Information struct
//!
//! \return True if rejoin is indicated, and false otherwise
static bool LLWbms_scanReqIndicatesRejoin(ScanRequestInfo_Obj* pScanReq)
{
    // If the scan request is broadcast and the switch indicator is set,
    // a rejoin is indicated. 
    return (pScanReq->isBroadcast && pScanReq->switchIndicator);
}



