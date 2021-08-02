/******************************************************************************
*  Filename:       ll_ble.c
*
*  Description:    Source code for the BLE Link Layer
*
*  Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
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

#include "ll_ble.h"
#include "../common.h"
#include "phy_manager.h"
#include "phy_if_ble_5.h"
#include "../radio_if.h"
#include "ll_ble_timer.h"

#define PDU_HEADER_TYPE_OFFSET                 0
#define PDU_HEADER_TYPE_MASK                   0xF
#define PDU_HEADER_LENGTH_OFFSET               1
#define PDU_HEADER_LENGTH_MASK                 0x3F
#define PDU_PAYLOAD_LENGTH_CONNECT_REQ         34
#define PDU_HEADER_LENGTH                      2
#define ACCESS_ADDRESS_LENGTH                  4 
#define TIMESTAMP_OFFSET                       6        // The offset from last byte of the PDU payload
#define STATUS_OFFSET                          4        // The offset from last byte of the PDU payload
#define STATUS_CHANNEL_FIELD_MASK              0x3F     // The bitmask of the channel field in status byte 0
    
// CONNECT_REQ PDU payload defines
#define PDU_PAYLOAD_AA_OFFSET                  12
#define PDU_PAYLOAD_CRCINIT_OFFSET             16
#define PDU_PAYLOAD_WINSIZE_OFFSET             19
#define PDU_PAYLOAD_WINOFFSET_OFFSET           20
#define PDU_PAYLOAD_INTERVAL_OFFSET            22
#define PDU_PAYLOAD_LATENCY_OFFSET             24
#define PDU_PAYLOAD_TIMEOUT_OFFSET             26
#define PDU_PAYLOAD_CHM_OFFSET                 28
#define PDU_PAYLOAD_HOP_OFFSET                 33
#define PDU_PAYLOAD_SCA_OFFSET                 33
#define PDU_PAYLOAD_HOP_MASK                   0x1F
#define PDU_PAYLOAD_SCA_MASK                   0xE0

// Advertising channel PDU types
#define PDU_TYPE_ADV_IND                        0
#define PDU_TYPE_ADV_DIRECT_IND                 1
#define PDU_TYPE_ADV_NONCONN_IND                2
#define PDU_TYPE_SCAN_REQ                       3
#define PDU_TYPE_SCAN_RSP                       4
#define PDU_TYPE_CONNECT_REQ                    5
#define PDU_TYPE_ADV_DISCOVER_IND               6

// LL Control PDU Opcodes
#define LL_CONNECTION_UPDATE_REQ                0
#define LL_CHANNEL_MAP_REQ                      1
#define LL_TERMINATE_IND                        2

// Data channel LLID
#define DATA_CHANNEL_PDU_LLID_MASK              0x3             // Mask of LLID field in the data channel PDU header
#define DATA_CHANNEL_LLID_CONTROL_PDU           0x3             // Value of LLID field for a LL Control PDU

// Advertising channel PDU header
#define ADV_PDU_HEADER_CHSEL_OFFSET             0               // Offset of ChSel bit in advertising channel PDU header
#define ADV_PDU_HEADER_CHSEL_MASK               0x20            // Mask of ChSel bit in advertising channel PDU header

// LL Control PDU 
#define LL_CONTROL_OPCODE_OFFSET                0               // Offset of LL Control Opcode field in data channel PDU payload
#define LL_CONTROL_OPCODE_MASK                  0xFF            // Mask of LL Control PDU Opcode field
#define LL_CONTROL_CTRDATA_OFFSET               1               // Offset of LL Control CtrData field in data channel PDU payload

#define INITIATOR_ADDRESS_LENGTH                6
#define BITRATE                                 1000000  // 1 Mbps bit rate
#define BITS_PER_BYTE                           8
#define MICRO_SECONDS_PER_SECOND                1000000
#define MICROSECONDS_PER_MILLISECOND            1000
#define RADIO_TIMER_TICKS_PER_MICROSECOND       4
#define BLE_TIME_UNIT                           1250 // 1.25 ms time unit used in connect_req packet

#define CHANNEL_SWITCH_TIME_MEASURED            290  // The time in us from RAT timer callback function is called until RX is on (RFC_GPO0 is high) is measured to ~290 us (on both LAUNCHXL-CC2652R PG1.1 and LAUNCHXL-CC1352R1 PG1.1 boards) .
#define CHANNEL_SWITCH_TIME_MARGIN              140  // Margin added to the measured channel switch time
#define SCA_DRIFT_FACTOR                        600  // Total Sleep Clock Accuracy drift factor. BLE 4.0 spec allows for up to +/- 500 ppm SCA drift for each device, plus 16 us deviation of anchor point from average. 
                                                     // 600 is set here to allow for 500 ppm on Master device, 50 ppm on the sniffer device (LAUNCHXL board) plus a margin of 50 ppm
#define MISSED_PACKET_TIME_MARGIN                50  // 50 us constant time margin for detection of missed first packet (M->S) in each connection event. This margin is added to a margin dependent on CI. 
#define MAX_WINDOW_WIDENING_FACTOR              833  // Maximum number of missed connection events that contributes to increased window widening. This gives a maximum of ~CI/2 in window widening (833 * 600/1000000 * CI)
#define NUMBER_OF_DATA_CHANNELS                 37

// Reset values
#define DEFAULT_ACCESS_ADDRESS                  0x8E89BED6
#define DEFAULT_CRC_INIT_VALUE                  0x00555555
#define DEFAULT_CHANNEL                         37

typedef enum LL_State
{
    State_Init = 0,
    State_Non_Connected = 1,
    State_Connected = 2
} LL_State;


// Enumerated values for direction info field in info byte appended to each packet
// The direction is only meaningful when LL is in connected state. 
// The value of direction info is set to Dir_Non_Connected when not in connected state.
typedef enum LL_DirInfo
{
    DirInfo_Non_Connected = 0,                      // Direction is unused. LL is not in connected state. 
    DirInfo_Master_Slave = 1,                       // Direction: Master to Slave
    DirInfo_Slave_Master = 2,                       // Direction: Slave to Master
    DirInfo_Unknown = 3,                            // Direction is unknown
} LL_DirInfo;

typedef struct LL_DirState_Obj
{
    LL_DirInfo prevDirInfo;                         // Direction info for previous packet
    LL_DirInfo currentDirInfo;                      // Direction info for current packet
    uint32_t prevPacketEndTime;                     // End time of previous packet
} LL_DirState_Obj;

static LL_State LLBle_state = State_Non_Connected;
static LL_DirState_Obj LLBle_dirState = { DirInfo_Non_Connected, DirInfo_Non_Connected, 0 };

// Connection parameters. These are content of the CONNECT_REQ packet
static uint16_t LLBle_connWinOffset = 0;                      // Connection window offset
static uint16_t LLBle_connInterval = 0;                       // Connection interval
static uint16_t LLBle_connTimeout = 0;                        // Connection timeout
static uint8_t LLBle_hopIncrement = 0;                        // Channel hop increment
static uint8_t LLBle_channelMap[5];                           // Channel map
static uint8_t LLBle_newChannelMap[5];                        // New channel map (after reception of a LL_CHANNEL_MAP_REQ)
static uint16_t LLBle_newChannelMapUpdateInstant = 0;         // Instant of new channel map (after reception of a LL_CHANNEL_MAP_REQ)
static bool LLBle_pendingChannelMapUpdate = false;

// Connection update parameters
static uint16_t LLBle_newConnWinOffset = 0;
static uint16_t LLBle_newConnInterval = 0;
static uint16_t LLBle_newConnTimeout = 0;
static uint16_t LLBle_newConnectionUpdateInstant  = 0;
static bool LLBle_pendingConnectionUpdate = false;

// Channel mapping
static uint8_t LLBle_lastUnmappedChannel = 0;                 // The last unmpapped channel
static uint8_t LLBle_numUsedChannels = 0;                     // Number of used channels
static uint8_t LLBle_remappingTable[NUMBER_OF_DATA_CHANNELS]; // Channel remapping table

static uint32_t LLBle_accessAddress = DEFAULT_ACCESS_ADDRESS; // Access Address

// Connection interval timing
static uint32_t LLBle_windowWideningUs = 0;                   // Calculated Window Widening in us
static uint32_t LLBle_missedConnEvents = 0;                   // Number of consecutive connection events where the first M->S packet was missed 

static uint64_t LLBle_connEventStart = 0;                     // Connection event start (in microseconds as absolute time from RAT timer was started)   
static uint32_t LLBle_connEventCount = 0;                     // Number of connection events
static uint32_t LLBle_anchorNext = 0;                         // Time for next anchor (in microseconds as absolute time from RAT timer was started)   
static uint8_t LLBle_anchorNotSet = 0;                        // True if anchor is not set for this connection event. 
                                                              // This variable will set to true when the first packet in the connection event is received
static uint8_t LLBle_anchorReschedule = 0;                    // If true, next anchor will be reschedule regardless of timing of the first packet received in the connection event

static bool LLBle_pendingConnectionTimeoutUpdate = false;
static uint8_t LLBle_startChannel = DEFAULT_CHANNEL;

static void LLBle_setupDataConnection(uint8_t* pduData, uint16_t length, uint8_t* pTimestamp);
static void LLBle_adjustAnchorPoint(uint8_t* pTimeStamp);
static uint32_t LLBle_calculatePacketEndTime(uint8_t* timeStamp, uint16_t pduLength);
static uint32_t LLBle_calculatePacketStartTime(uint8_t* timeStamp);
static uint32_t LLBle_convertRadioTimeToUs(uint32_t timerValue);
static void LLBle_connectionTimerEventHandler(uint32_t ratTime);
static void LLBle_scheduleNextConnectionEvent(uint32_t nextConnEvent);
static uint32_t LLBle_calculateMissedPacketTimeMargin(uint32_t missedConnEvents);
static void LLBle_disconnect(void);
bool LLBle_isDataChannel(uint8_t channel);
void LLBle_prepareFutureConnectionUpdate(uint8_t* ctrData);

// Channel mapping
static void LLBle_prepareFutureChannelMapUpdate(uint8_t* ctrData);
static void LLBle_channelMapUpdate(const uint8_t* pNewChannelMap);
static uint8_t LLBle_channelCheckIfUsed(uint8_t unmappedChannel);
static uint8_t LLBle_channelGetNext(void);
static uint8_t LLBle_channelCheckIfUsed(uint8_t unmappedChannel);

// Access Address handling
static void LLBle_handleAccessAddressUpdate(uint8_t channel);
static bool LLBle_bAccessAddressChanged;

// Initiator address filter functionality
static bool LLBle_filterDataConnection(uint8_t* pduHeader, uint8_t* payload);
static bool LLBle_filterIsValid = false;                     // True means that a valid initiator address value is set as filter
static uint8_t LLBle_filterValue[6] = {0};                   // Initiator address value for filter

// Supervision timer
static void LLBle_supervisionTimerStart(uint32_t currentTime, uint32_t connInterval);
static void LLBle_supervisionTimerReset(uint32_t currentTime, uint32_t supervisionTimeout);
static void LLBle_supervisionTimerEventHandler(uint32_t ratTime);

// BLE meta info 
static uint16_t LLBle_prependPacketInfo(uint8_t* packet, uint16_t packetLength, uint8_t channel);
static void LLBle_setDirInfo(uint16_t pduLength, uint8_t* pTimeStamp);

uint16_t LLBle_processPacket(uint8_t* packetData, uint16_t packetLength)
{
    uint8_t* pduHeader = packetData;
    uint8_t* pduPayload = &packetData[PDU_HEADER_LENGTH];
    uint8_t pduPayloadLength = pduHeader[PDU_HEADER_LENGTH_OFFSET] & PDU_HEADER_LENGTH_MASK;
    uint8_t* pTimestamp = &packetData[PDU_HEADER_LENGTH + pduPayloadLength + TIMESTAMP_OFFSET]; // Radio timer timestamp bytes 
    uint8_t* pStatus = &packetData[PDU_HEADER_LENGTH + pduPayloadLength + STATUS_OFFSET];      // Status byte 0
    uint8_t channel = *pStatus & STATUS_CHANNEL_FIELD_MASK;                                    // The BLE channel that this packet was received on
    
    LLBle_handleAccessAddressUpdate(channel);
    
    switch(LLBle_state)
    {
    case State_Non_Connected: 
        
        // Consider only CONNECT_REQ with the correct length
        if( ((pduHeader[PDU_HEADER_TYPE_OFFSET] & PDU_HEADER_TYPE_MASK) == PDU_TYPE_CONNECT_REQ) && (pduPayloadLength == PDU_PAYLOAD_LENGTH_CONNECT_REQ) )
        {   
            if(LLBle_filterDataConnection(pduHeader, pduPayload))
            {
                LLBle_setupDataConnection(pduPayload, pduPayloadLength, pTimestamp);
            }
        }
           
        break;
        
    case State_Connected:
        
        // Reset connection watchdog
        // For the first packet on the first connection event,
        // set the connection timeout according to spec.
        if (LLBle_pendingConnectionTimeoutUpdate)
        {
            LLBle_supervisionTimerStart(LLBle_calculatePacketStartTime(pTimestamp), LLBle_connInterval);
            LLBle_pendingConnectionTimeoutUpdate = false;
        }
        else
        {
            LLBle_supervisionTimerReset(LLBle_calculatePacketStartTime(pTimestamp), LLBle_connTimeout);
        }
        
        // Set packet direction info
        LLBle_setDirInfo(pduPayloadLength, pTimestamp);
        
        // If this is the first packet for the current connection
        // event, set the anchor point
        LLBle_adjustAnchorPoint(pTimestamp);
        
        // Handle LL Control PDUs
        if((pduHeader[PDU_HEADER_TYPE_OFFSET] & DATA_CHANNEL_PDU_LLID_MASK) == DATA_CHANNEL_LLID_CONTROL_PDU)
        {
            uint8_t opcode = pduPayload[LL_CONTROL_OPCODE_OFFSET & LL_CONTROL_OPCODE_MASK];
            uint8_t* ctrData = &pduPayload[LL_CONTROL_CTRDATA_OFFSET];
            
            switch (opcode) 
            {
            case LL_TERMINATE_IND:
                LLBle_disconnect();
                break;
            case LL_CHANNEL_MAP_REQ:
                LLBle_prepareFutureChannelMapUpdate(ctrData);
                break;
             case LL_CONNECTION_UPDATE_REQ:
                LLBle_prepareFutureConnectionUpdate(ctrData);
                break;
            }
        }

        break;
    }
    
    // Add access address in front of the packet
    uint16_t newPacketLength = LLBle_prependPacketInfo(packetData, packetLength, channel);
    
    return newPacketLength;
}


uint32_t LLBle_getAccessAddress(void)
{
    return LLBle_accessAddress;
}


//! \brief Setup data connection
//!
//!        This function shall be called when a CONNECT_REQ PDU is received. This function does the following: 
//!        - Retrieves connection parameters from the CONNECT_REQ PDU and store this information in file scope variables.
//!        - Stops RX and restarts RX on the channel for the first connection event.  
//!
//! \param[in] pduData
//!            PDU payload data
//!
//! \param[in] length
//!            Length of PDU payload
//!
//! \param[in] pTimeStamp
//!            Pointer to packet timestamp (4 bytes) as appended by Radio Timer
//!
//! \return none
void LLBle_setupDataConnection(uint8_t* pduData, uint16_t length, uint8_t* pTimeStamp)
{
    uint8_t channel = 0;
    
    // Stop RX 
    RadioIf_stopRxCmd();
    
    // Use the end of the CONNECT REQUEST packet as the initial anchor
    // for subsequent synchronization.
    LLBle_connEventStart = LLBle_calculatePacketEndTime(pTimeStamp, length);
    
    LLBle_pendingConnectionTimeoutUpdate = true;
    
    // Get currently used PHY index
    uint8_t phyIndex = PhyManager_getPhyIndex();
    
    // Set new Access Address for the current PHY Index
    uint32_t accessAddress = Common_get32BitValueLE(pduData + PDU_PAYLOAD_AA_OFFSET);
    PhyIf_setAccessAddressBle_5(phyIndex, accessAddress);
    LLBle_bAccessAddressChanged = true;
    
    // Set new init value for CRC
    uint32_t crcInit = Common_get32BitValueLE(pduData + PDU_PAYLOAD_CRCINIT_OFFSET);
    // CRC Init value is 3 bytes. 
    PhyIf_setCrcInitValueBle_5(phyIndex, crcInit & 0xFFFFFF);
    
    // Get window offset (multiple of 1.25 ms)
    LLBle_connWinOffset = Common_get16BitValueLE(pduData + PDU_PAYLOAD_WINOFFSET_OFFSET);
    
    // Get connection event timing info (multiple of 1.25 ms)
    LLBle_connInterval =  Common_get16BitValueLE(pduData + PDU_PAYLOAD_INTERVAL_OFFSET);
    
    // Get supervision Timeout Value (multiple of 10 ms)
    LLBle_connTimeout = Common_get16BitValueLE(pduData + PDU_PAYLOAD_TIMEOUT_OFFSET);
    
    // Get channel info
    LLBle_channelMapUpdate(&pduData[PDU_PAYLOAD_CHM_OFFSET]);
    LLBle_hopIncrement = pduData[PDU_PAYLOAD_HOP_OFFSET] & PDU_PAYLOAD_HOP_MASK;
    
    // Store the advertiser channel LL had in non connected mode and set new channel
    LLBle_startChannel = PhyIf_getChannelBle_5(phyIndex); 
    LLBle_lastUnmappedChannel = 0;
    channel = LLBle_channelGetNext();
    PhyIf_setChannelBle_5(phyIndex, channel);
    
    // Reset Connection Event Counter
    LLBle_connEventCount = 0;
    
    // Calculate allowed timing drift from anchor point in us for first M->S packet (Window Widening)
    // SCA drift factor in ppm / 1000000 * CI (in us) 
    LLBle_windowWideningUs = (((SCA_DRIFT_FACTOR * BLE_TIME_UNIT) / 1000) * LLBle_connInterval) / 1000;

    // Schedule next connection event, taking into account the possibility of
    // losing the packet(s) from the master (and slave) in the initial 
    // transmit window. The next connection event will then, worst case, start
    // sometime around
    //
    //       t_end_conn_req + 1.25ms + winOffset + connInterval
    //
    LLBle_anchorNext = LLBle_connEventStart + (1 + LLBle_connWinOffset + LLBle_connInterval) * BLE_TIME_UNIT;
    // Todo, subtract the elapsed time since packet was timestamped by radio
    
    LLBle_scheduleNextConnectionEvent(LLBle_anchorNext);
    LLBle_anchorReschedule = 1;
    LLBle_anchorNotSet = 1;
    
    // Start supervision timeout timer for first connection event.
    // For the first connection event use 6 * connInterval (as per BLE spec.)
    LLBle_supervisionTimerStart(LLBle_connEventStart, LLBle_connInterval);
    
    // Update state
    LLBle_state = State_Connected;
    
    // Restart RX 
    RadioIf_startRxCmd();
}


//! \brief Update channel map and recalculates channel remapping table
//!
//!        This function updates the following file scope variables: 
//!        - LLBle_channelMap[]
//!        - LLBle_remappingTable[]
//!        - LLBle_numUsedChannels
//!
//! \param[in] pNewChannelMap
//!            Pointer to buffer (5 bytes) with updated channel map
//!
//! \return none
static void LLBle_channelMapUpdate(const uint8_t* pNewChannelMap)
{
    uint8_t channel = 0;
    LLBle_numUsedChannels = 0;

    LLBle_channelMap[0] = pNewChannelMap[0];
    LLBle_channelMap[1] = pNewChannelMap[1];
    LLBle_channelMap[2] = pNewChannelMap[2];
    LLBle_channelMap[3] = pNewChannelMap[3];
    LLBle_channelMap[4] = pNewChannelMap[4];

    // Update the remapping table
    for(uint8_t i = 0; i < NUMBER_OF_DATA_CHANNELS; i++)
    {
        uint8_t mapIndex = i / 8;
        uint8_t bitIndex = i % 8;
        
        if(LLBle_channelMap[mapIndex] & (1 << bitIndex))
        {
            LLBle_remappingTable[LLBle_numUsedChannels++] = channel;
        }
        
        channel++;
    }
}


//! \brief Check if channel number is used 
//!
//! \return A non zero value if the channel is used, and 0 if channel is not in use
static uint8_t LLBle_channelCheckIfUsed(uint8_t unmappedChannel)
{
    uint8_t octet = unmappedChannel >> 3;
    uint8_t bit = 0x01 << (unmappedChannel - (octet << 3));
    
    return ((LLBle_channelMap[octet] & bit));
}


//! \brief Get next channel 
//!        
//!        This function updates the variable LLBle_lastUnmappedChannel
//!
//! \return Next channel to use 
static uint8_t LLBle_channelGetNext(void)
{
    uint8_t remappingIndex;
    uint8_t unmappedChannel;

    unmappedChannel = (LLBle_lastUnmappedChannel + LLBle_hopIncrement) % 37;
    LLBle_lastUnmappedChannel = unmappedChannel;

    if (LLBle_channelCheckIfUsed(unmappedChannel))
    {
        return unmappedChannel;
    }
    else
    {
        remappingIndex = unmappedChannel % LLBle_numUsedChannels;
        return LLBle_remappingTable[remappingIndex];
    }
}


//! \brief Add 8 bytes BLE meta information in front of the payload 
//!        The following information is added to the packet:
//!        - Channel (1 byte)
//!        - Connection event counter (2 bytes)
//!        - Info field (1 byte)
//!        - Access Address (4 bytes)
//!        
//! \param[in] payload
//!            Pointer to payload data
//!
//! \param[in] payloadLength
//!            Length of the payload
//! 
//! \param[in] channel
//!            BLE channel the packet was received on
//!            
//! \return Updated length of the payload
static uint16_t LLBle_prependPacketInfo(uint8_t* packet, uint16_t packetLength, uint8_t channel)
{
    uint8_t numberOfBytesToAdd = 8;
    uint8_t* pBufferAddr = packet;
    
    // Move each payload to numberOfBytesToAdd bytes higher address
    for(int i=0; i<packetLength; i++)
    {   
        *(packet + packetLength + numberOfBytesToAdd - i - 1) = *(packet + packetLength - i - 1);
    }
    
    // Add channel (1 byte)
    *pBufferAddr = channel;
    pBufferAddr += 1;
    
    // Add connection event counter (2 bytes)
    Common_set16BitValueLE(pBufferAddr, LLBle_connEventCount);
    pBufferAddr += 2;
    
    // Add info field (1 byte)
    *pBufferAddr = LLBle_dirState.currentDirInfo;
    pBufferAddr += 1;
    
    // Add 4 bytes access address
    Common_set32BitValueLE(pBufferAddr, LLBle_accessAddress);
    
    return packetLength + numberOfBytesToAdd;
}


//! \brief Handle change of access address
//!
//!       Stores a local copy of the access address to avoid having to read this for each packet.  
//!
//! \return none
static void LLBle_handleAccessAddressUpdate(uint8_t channel)
{
    // Check if access address has changed
    if(LLBle_bAccessAddressChanged)
    {
        // Update the local copy of access address, except for the situation where LL is in non connected state and the packet 
        // was received on a data channel. This can happen if the connection is terminated (e.g.
        // a TERMINATE_IND was received), the LL layer was reset and state changed back to non-connected state, 
        // but this current data packet was already received before RX channel was switched from data channel
        // to an advertiser channel. 
        if( !((LLBle_state == State_Non_Connected) && LLBle_isDataChannel(channel)) )
        {
            // Get currently used PHY index
            uint8_t phyIndex = PhyManager_getPhyIndex();
            LLBle_accessAddress = PhyIf_getAccessAddressBle_5(phyIndex);
            LLBle_bAccessAddressChanged = false;
        }
    }
}


//! \brief Check if a CONNECT_REQ packet is accepted by filter
//!
//! \param[in] pduHeader
//!            Pointer to the PDU header for the packet
//!
//! \param[in] payload
//!            Pointer to CONNECT_REQ PDU payload data
//!
//! \return true if packet accepted by filter, and false otherwise
static bool LLBle_filterDataConnection(uint8_t* pduHeader, uint8_t* payload)
{
    // CONNECT_REQ packets with channel selection algorithm 2 (ChSel bit in PDU header is 1)
    // shall be filtered out. Channel selection algorithm 2 is not supported. 
    if(pduHeader[ADV_PDU_HEADER_CHSEL_OFFSET] & ADV_PDU_HEADER_CHSEL_MASK)
    {
        return false;
    }
    
    if(LLBle_filterIsValid)
    {
        if( (payload[0] == LLBle_filterValue[0]) && 
            (payload[1] == LLBle_filterValue[1]) &&
            (payload[2] == LLBle_filterValue[2]) &&
            (payload[3] == LLBle_filterValue[3]) &&
            (payload[4] == LLBle_filterValue[4]) &&
            (payload[5] == LLBle_filterValue[5]) )
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}


//! \brief Set the iniatior address value for the connection filter
//!
//! \param[in] initiatorAddr
//!            Pointer to buffer that holds initiator address (6 bytes)
//!
//! \return none
void LLBle_setInitiatorAddressFilterValue(uint8_t* initiatorAddr)
{
    for(int i=0; i<INITIATOR_ADDRESS_LENGTH; i++)
    {
        LLBle_filterValue[i] = *(initiatorAddr + i);
    }
    
    LLBle_filterIsValid = true;
}


//! \brief Reset state of Link Layer
//!
//! \return none
extern void LLBle_reset(void)
{
    // Get currently used PHY index
    uint8_t phyIndex = PhyManager_getPhyIndex();
    
    LLBle_state = State_Non_Connected;
    LLBle_connWinOffset = 0;
    LLBle_connInterval = 0;
    LLBle_connTimeout = 0;
    LLBle_hopIncrement = 0;
    LLBle_lastUnmappedChannel = 0;
    LLBle_numUsedChannels = 0;
    LLBle_connEventStart = 0;
    LLBle_connEventCount = 0;
    LLBle_anchorNext = 0;
    LLBle_anchorNotSet = 0;
    LLBle_anchorReschedule = 0;
    LLBle_windowWideningUs = 0;
    LLBle_missedConnEvents = 0;
    LLBle_newChannelMapUpdateInstant = 0;
    LLBle_pendingChannelMapUpdate = false;
    
    LLBle_newConnWinOffset = 0;
    LLBle_newConnInterval = 0;
    LLBle_newConnTimeout = 0;
    LLBle_newConnectionUpdateInstant  = 0;
    LLBle_pendingConnectionUpdate = false;
    
    for(int i=0; i<5; i++)
    {
        LLBle_channelMap[i] = 0;
        LLBle_newChannelMap[i] = 0;
    }
    
    for(int i=0; i<36; i++)
    {
        LLBle_remappingTable[i] = 0;
    }
    
    // Set default Access address and CRC init values
    PhyIf_setAccessAddressBle_5(phyIndex, DEFAULT_ACCESS_ADDRESS);
    LLBle_bAccessAddressChanged = true;
    PhyIf_setCrcInitValueBle_5(phyIndex, DEFAULT_CRC_INIT_VALUE);
    PhyIf_setChannelBle_5(phyIndex, DEFAULT_CHANNEL);
    
    // Clear initiator address filter 
    LLBle_filterIsValid = false;
    LLBle_filterValue[0] = 0;
    LLBle_filterValue[1] = 0;
    LLBle_filterValue[2] = 0;
    LLBle_filterValue[3] = 0;
    LLBle_filterValue[4] = 0;
    LLBle_filterValue[5] = 0;
    
    // Reset direction state
    LLBle_dirState.currentDirInfo = DirInfo_Non_Connected;
    LLBle_dirState.prevDirInfo = DirInfo_Non_Connected;
    LLBle_dirState.prevPacketEndTime = 0;
}


//! \brief Initialize Link Layer
//!
//! \return none
void LLBle_init(void)
{
    LLBle_reset();
    
    // Connection event timer
    LLBleTimer_create(CONNECTION_EVENT_TIMER, LLBle_connectionTimerEventHandler);
    
    // Connection event timer
    LLBleTimer_create(SUPERVISION_TIMER, LLBle_supervisionTimerEventHandler);
}


//! \brief Calculate time of a packet given timestamp and packet length
//!        The time is an absolute value in microseconds from Radio timer start
//!
//! \param[in] timeStamp
//!            Radio timer timestamp for the packet
//! 
//! \param[in] pduLength
//!            Length of the PDU in bytes
//! 
//! \return none
uint32_t LLBle_calculatePacketEndTime(uint8_t* timeStamp, uint16_t pduLength)
{
    uint32_t packetDuration = (pduLength + PDU_HEADER_LENGTH + ACCESS_ADDRESS_LENGTH) * BITS_PER_BYTE * MICRO_SECONDS_PER_SECOND/BITRATE;
    
    // convert Radio Timer (RAT) timestamp to microseconds
    uint32_t ratTime = Common_get32BitValueLE(timeStamp);
    uint32_t time = ratTime/RADIO_TIMER_TICKS_PER_MICROSECOND;
    
    // calcalute end of packet time
    return time + packetDuration;
}


//! \brief Calculate start time of a packet given the timestamp.
//!        The time is an absolute value in microseconds from Radio timer start
//!
//! \param[in] timeStamp
//!            Radio timer timestamp for the packet
//! 
//! \return none
uint32_t LLBle_calculatePacketStartTime(uint8_t* timeStamp)
{   
    // convert Radio Timer (RAT) timestamp to microseconds
    uint32_t ratTime = Common_get32BitValueLE(timeStamp);
    uint32_t time = ratTime/RADIO_TIMER_TICKS_PER_MICROSECOND;
    
    return time;
}


//! \brief Convert value of radio timer to microseconds
//!
//! \param[in] timerValue
//!            Value of radio timer
//! 
//! \return time in microseconds
uint32_t LLBle_convertRadioTimeToUs(uint32_t timerValue)
{   
    // convert Radio Timer (RAT) timestamp to microseconds
    uint32_t time = timerValue/RADIO_TIMER_TICKS_PER_MICROSECOND;
    
    return time;
}


//! \brief Schedule next connection event
//!
//! \param[in] nextConnEvent
//!            Time of next connection event (absolute time in microseconds)
//! 
//! \return none
static void LLBle_scheduleNextConnectionEvent(uint32_t nextConnEvent)
{
    // Subtract the calculated time margin to do the actual channel switch to make sure channel switch happens early enough
    // For each missed connection event, the window widening contribution is increased
    uint32_t defaultChannelSwitchTimeMargin = CHANNEL_SWITCH_TIME_MEASURED + CHANNEL_SWITCH_TIME_MARGIN + LLBle_windowWideningUs;
    nextConnEvent -= defaultChannelSwitchTimeMargin + (LLBle_missedConnEvents * LLBle_windowWideningUs);
    
    LLBleTimer_stop(CONNECTION_EVENT_TIMER);
    LLBleTimer_setTimeout(CONNECTION_EVENT_TIMER, nextConnEvent);
    LLBleTimer_start(CONNECTION_EVENT_TIMER);
}


//! \brief Adjust anchor point for first packet in a connection event
//!
//! \param[in] pTimeStamp
//!            Timestamp of packet 
//! 
//! \return none
static void LLBle_adjustAnchorPoint(uint8_t* pTimeStamp)
{
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t anchorAdjusted = 0;
    
    if(LLBle_anchorNotSet)
    {
        // The baseline is the time we did the channel switch
        t0 = LLBle_connEventStart;
        
        // Get the captured timestamp for this packet
        anchorAdjusted = LLBle_calculatePacketStartTime(pTimeStamp);
        
        // Store current time for use later
        t1 = anchorAdjusted;
                
        // Set the adjusted anchor for the next connection event
        // based on the time of this packet + connection interval
        anchorAdjusted += LLBle_connInterval * BLE_TIME_UNIT;
        LLBle_anchorNotSet = 0;
        
        // Calculate the time margin for when to consider the first M->S packet lost
        uint32_t missedPacketTimeMargin = LLBle_calculateMissedPacketTimeMargin(LLBle_missedConnEvents);
        
        // Determine if we need to adjust the next expected anchor point:
        
        if (LLBle_anchorReschedule == 1)
        {
            LLBle_anchorReschedule = 0;
            LLBle_anchorNext = anchorAdjusted;
            LLBle_missedConnEvents = 0;
            LLBle_scheduleNextConnectionEvent(LLBle_anchorNext);
        }
        else if((t1 - t0) > missedPacketTimeMargin)
        {
            // Assume that first packet (from Master to Slave) has been lost. Do NOT
            // resynchronize anchor.
            
            // A note on wraparound: t1 is always larger (ahead in time) than t0.
            // If t1 has just wrapped while t0 has not wrapped the subtraction will still be correct since both 
            // variables are unsigned int and the result will be: t1 - to modulo (UINT32_MAX + 1)
            
            if(LLBle_missedConnEvents < MAX_WINDOW_WIDENING_FACTOR)
            {
                LLBle_missedConnEvents++;
            }
            
            // First packet in the connection event was lost. Set direction to Unknown. 
            LLBle_dirState.currentDirInfo = DirInfo_Unknown;
            LLBle_dirState.prevDirInfo = DirInfo_Unknown;
        }
        else if (anchorAdjusted != LLBle_anchorNext)
        {
            LLBle_anchorNext = anchorAdjusted;
            LLBle_missedConnEvents = 0;
            LLBle_scheduleNextConnectionEvent(LLBle_anchorNext);
        }
    }
}


//! \brief Set direction info
//!
//!        This function determines the direction of a packet when in connected state based on
//!        direction of the previous packet and time interval from the previous packet. 
//!        This function updates the LLBle_dirState variable.   
//!
//! \param[in] pduLength
//!            Length of PDU payload
//!
//! \param[in] pTimeStamp
//!            Pointer to packet timestamp (4 bytes) as appended by Radio Timer
//!
//! \return none
void LLBle_setDirInfo(uint16_t pduLength, uint8_t* pTimeStamp)
{
    uint32_t packetStartTime = LLBle_calculatePacketStartTime(pTimeStamp);
    uint32_t packetEndTime = LLBle_calculatePacketEndTime(pTimeStamp, pduLength);
    
    // If anchor is not yet set, this is the first packet received in this connection event, and  
    // it is assumed this packet is from Master->Slave. The function LLBle_adjustAnchorPoint() will detect
    // if the first packet in the connection event have been missed. If so the direction of the first packet
    // received will be set to Unknown (by LLBle_adjustAnchorPoint) and all subsequent packets in the connection
    // event will be marked with direction Unknown. 
    if(LLBle_state == State_Non_Connected)
    {
        // LL is not in connected state (hence direction is not meaningful)
        LLBle_dirState.currentDirInfo = DirInfo_Non_Connected;
    }
    else if(LLBle_anchorNotSet)
    {
        // This is the first packet in the connection event. Set direction to Master->Slave. 
        // If first packet is missed direction will be changed to Unknown by LLBle_adjustAnchorPoint().
        LLBle_dirState.currentDirInfo = DirInfo_Master_Slave;
    }
    else if(LLBle_dirState.prevDirInfo == DirInfo_Unknown)
    {
        // If the previous packet in a connection have unknown direction, this packet also have unknown direction
        LLBle_dirState.currentDirInfo = DirInfo_Unknown;
    }
    else if((packetStartTime - LLBle_dirState.prevPacketEndTime) > 250)
    {
        // If delta time between start of this packet and end of previous packet in the same connection event is more than 250 us, 
        // consider direction of this packet as unknown. 
        LLBle_dirState.currentDirInfo = DirInfo_Unknown;
    }
    else if(LLBle_dirState.prevDirInfo == DirInfo_Master_Slave)
    {
        LLBle_dirState.currentDirInfo = DirInfo_Slave_Master;
    }
    else if(LLBle_dirState.prevDirInfo == DirInfo_Slave_Master)
    {
        LLBle_dirState.currentDirInfo = DirInfo_Master_Slave;
    }
    
    LLBle_dirState.prevDirInfo = LLBle_dirState.currentDirInfo;
    LLBle_dirState.prevPacketEndTime = packetEndTime;
}


//! \brief Calculate time margin from connection event timer callback until the first packet from M->S is considered lost
//!
//! \param[in] missedConnEvents
//!            Number of consecutive missed connection events
//! 
//! \return none
uint32_t LLBle_calculateMissedPacketTimeMargin(uint32_t missedConnEvents)
{
    // Window widening is increased each time a connection event is missed (that is, the first M->S packet is lost)
    // The increased window widening causes the connection event callback to be scheduled earlier ahead of
    // expected anchor point and the missed packet time margin must take this into account. 
    
    // Channel switch time + 2 * window widening + a small constant margin 
    uint32_t defaultMissedPacketTimeMargin = CHANNEL_SWITCH_TIME_MEASURED + CHANNEL_SWITCH_TIME_MARGIN + (LLBle_windowWideningUs * 2) + MISSED_PACKET_TIME_MARGIN;
    
    // Compensate for the earlier connection event callback when connection events are missed, 
    // but keep the margin constant after the expected anchor point
    return defaultMissedPacketTimeMargin + (LLBle_windowWideningUs * missedConnEvents);
}


//! \brief Start supervision timer 
//!        This function shall be used to start supervision timer before connection is established
//!        That is, before at least 2 data packets are received. 
//!
//! param[in] currentTime
//!           current time in us from Radio timer was started
//!
//! \param[in] connInterval
//!            Connection interval value in 1,25ms steps as specified in CONNECT_REQ
//! 
//! \return none
static void LLBle_supervisionTimerStart(uint32_t currentTime, uint32_t connInterval)
{
    uint32_t nextTimeout = currentTime + 6 * connInterval * BLE_TIME_UNIT;
    
    LLBleTimer_stop(SUPERVISION_TIMER);
    LLBleTimer_setTimeout(SUPERVISION_TIMER, nextTimeout);
    LLBleTimer_start(SUPERVISION_TIMER);
}


//! \brief Reset supervision timer
//!        This function shall be used to reset supervision timer after the connection is established
//!
//! param[in] currentTime
//!           current time in us from Radio timer was started
//!
//! \param[in] supervisionTimeout
//!            Supervision timeout value in 10ms steps as specified in CONNECT_REQ
//! 
//! \return none
static void LLBle_supervisionTimerReset(uint32_t currentTime, uint32_t supervisionTimeout)
{
    // Add current time and supervision timeout (in multiple of 10ms)
    uint32_t nextTimeout = currentTime + supervisionTimeout * 10000;   
    
    LLBleTimer_stop(SUPERVISION_TIMER);
    LLBleTimer_setTimeout(SUPERVISION_TIMER, nextTimeout);
    LLBleTimer_start(SUPERVISION_TIMER);
}


//! \brief Callback for connection event timer expiration
//!
//!        This function:
//!        - Stops RX, 
//!        - Sets channel for next connection event, 
//!        - Restarts RX and schedules the next connection event.
//!
//! \param[in] ratTime
//!            Radio timer value at expiration
//! 
//! \return none
void LLBle_connectionTimerEventHandler(uint32_t ratTime)
{
    uint8_t channel = 0;
        
    if(LLBle_state == State_Connected)
    {
        LLBle_connEventStart = LLBle_convertRadioTimeToUs(ratTime);
            
        // Stop RX
        RadioIf_stopRxCmd();
       
        LLBle_connEventCount++;
        if(LLBle_pendingChannelMapUpdate && (LLBle_connEventCount == LLBle_newChannelMapUpdateInstant))
        {
            LLBle_channelMapUpdate(LLBle_newChannelMap);
            LLBle_pendingChannelMapUpdate = false;
        }
        if (LLBle_pendingConnectionUpdate && (LLBle_connEventCount == LLBle_newConnectionUpdateInstant))
        {
            LLBle_connInterval  = LLBle_newConnInterval;
            LLBle_connWinOffset = LLBle_newConnWinOffset;
            LLBle_connTimeout   = LLBle_newConnTimeout;
            LLBle_pendingConnectionUpdate = false;
            LLBle_pendingConnectionTimeoutUpdate = true;
            LLBle_supervisionTimerStart(LLBle_connEventStart, LLBle_connInterval);
            LLBle_anchorNext += LLBle_connWinOffset * BLE_TIME_UNIT;
            LLBle_anchorReschedule = 1;
        }
        
        // Get currently used PHY index
        uint8_t phyIndex = PhyManager_getPhyIndex();
        
        // Set new channel
        channel = LLBle_channelGetNext();
        PhyIf_setChannelBle_5(phyIndex, channel);
        
        // Restart RX 
        RadioIf_startRxCmd();
        
        if(LLBle_anchorNotSet && (LLBle_missedConnEvents < MAX_WINDOW_WIDENING_FACTOR))
        {
            // No packet was received in last connection event. Increase the missed connection event counter
            LLBle_missedConnEvents++;
        }
        
        LLBle_anchorNext += LLBle_connInterval * BLE_TIME_UNIT;
        LLBle_scheduleNextConnectionEvent(LLBle_anchorNext);
        
        // By default, the anchor is not set for the new connection event.
        // The anchor will be set by the first received packet.
        LLBle_anchorNotSet = 1;
    }
}


//! \brief Callback for supervision event timer expiration 
//!
//!        This function disconnect from current connection 
//!
//! \param[in] ratTime
//!            Radio timer value at expiration
//! 
//! \return none
void LLBle_supervisionTimerEventHandler(uint32_t ratTime)
{
    // Disconnect current connection
    LLBle_disconnect();
}


//! \brief Disconnect from current connection and restart RX 
//!
//!        This function:
//!        - Stops RX,
//!        - Reset Link Layer, 
//!        - Sets channel back to the advertiser channel sniffer was using 
//!          when in non connected mode,
//!        - Restores initiator address filter and
//!        - Restarts RX. 
//! 
//! \return none
void LLBle_disconnect(void)
{
    // Stop connection event timer
    LLBleTimer_stop(CONNECTION_EVENT_TIMER);
    
    // Stop RX
    RadioIf_stopRxCmd();
    
    // The reset function will reset initiator address filter value
    // Save the original filter address values so it can be 
    // restored after the reset. 
    uint8_t originalFilterValue[6] = {0};
    for(uint8_t i=0; i<6; i++)
    {
        originalFilterValue[i] = LLBle_filterValue[i];
    }
    
    // Reset LL
    LLBle_reset();
    
    // Restore the originally used BLE channel and initiator address filter
    uint8_t phyIndex = PhyManager_getPhyIndex();
    PhyIf_setChannelBle_5(phyIndex, LLBle_startChannel);
    for(uint8_t i=0; i<6; i++)
    {
        LLBle_filterValue[i] = originalFilterValue[i];
    }
    
    // Restart RX
    RadioIf_startRxCmd();
}


//! \brief Check if channel number is a data channel 
//! 
//! \param[in] channel
//!            BLE channel number 
//!
//! \return true if channel is data channel, and false otherwise
bool LLBle_isDataChannel(uint8_t channel)
{
    return (channel <= 36);
}


//! \brief Prepare for a future channel map update after reception of 
//!        LL_CHANNEL_MAP_REQ
//! 
//! \param[in] ctrData
//!            Pointer to the start of ctrData field of the LL_CHANNEL_MAP_REQ
//!
//! \return none
void LLBle_prepareFutureChannelMapUpdate(uint8_t* ctrData)
{
    LLBle_newChannelMap[0] = ctrData[0];
    LLBle_newChannelMap[1] = ctrData[1];
    LLBle_newChannelMap[2] = ctrData[2];
    LLBle_newChannelMap[3] = ctrData[3];
    LLBle_newChannelMap[4] = ctrData[4];

    LLBle_newChannelMapUpdateInstant  = ctrData[5];
    LLBle_newChannelMapUpdateInstant |= ctrData[6] << 8;

    LLBle_pendingChannelMapUpdate = true;
}


//! \brief Prepare for a update after reception of 
//!        LL_CONNECTION_UPDATE_REQ
//! 
//! \param[in] ctrData
//!            Pointer to the start of ctrData field of the LL_CONNECTION_UPDATE_REQ
//!
//! \return none
void LLBle_prepareFutureConnectionUpdate(uint8_t* ctrData)
{
    LLBle_newConnWinOffset = ctrData[1];
    LLBle_newConnWinOffset |= ctrData[2] << 8;
    LLBle_newConnInterval = ctrData[3];
    LLBle_newConnInterval |= ctrData[4] << 8;
    LLBle_newConnTimeout = ctrData[7];
    LLBle_newConnTimeout |= ctrData[8] << 8;

    LLBle_newConnectionUpdateInstant  = ctrData[9];
    LLBle_newConnectionUpdateInstant |= ctrData[10] << 8;

    LLBle_pendingConnectionUpdate = 1;
}
