/******************************************************************************
*  Filename:       ll_wbms_frame.c
*
*  Description:    Source code for the wBMS Link Layer Frame functionality
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

#include "ll_wbms_frame.h"
#include "ll_wbms_channel.h"
#include <string.h>

// Common frame definitions
#define FRAME_HEADER_LENGTH_OFFSET              0               // Byte offset of length field from start of header
#define FRAME_HEADER_CONTROL_OFFSET             1               // Byte offset of frame control field from start of header
#define FRAME_HEADER_NETWORKID_OFFSET           3               // Byte offset of networkId field from start of header
#define FRAME_HEADER_UNIQUEID_OFFSET            4               // Byte offset of UniqueId field from start of header
#define FRAME_HEADER_UNIQUEID_LENGTH            9               // Length of UniqueId field
#define FRAME_HEADER_FCEXTENSION_OFFSET         13              // Byte offset of FC Extension field (if present) from start of header
#define FRAME_HEADER_SECURITY_HEADER_LENGTH     5               // 5 bytes security header (present when security mode is 1 or 2)

// FC Extension field
#define FC_EXTENSION_ACK_LENGTH_BITMASK         0x0F            // Ack length field within the FC Extension byte (first 4 bits)

#define EXPECTED_LENGTH_SCAN_REQUEST            138             // Expected length of scan request packet including bytes appended by RF Core
#define EXPECTED_LENGTH_PAIRING_REQUEST         83              // Expected length of pairing request packet including bytes appended by RF Core
#define DL_CONFIG_FRAME_HEADER_LENGTH_MIN       14              // Minimum length of a Downlink Config frame header
#define DL_CONFIG_FRAME_PAYLOAD_LENGTH_MIN      22              // Minimum payload of 6 bytes in a Downlink Config frame 
                                                                // (NetworkInfo 14 octets + MIC 4 octets + CRC 4 octets)
#define RFCORE_NUM_APPENDED_BYTES               10              // Number of bytes appended by RF Core to each PDU

// Frame Control field
#define FRAME_TYPE_BITMASK                      0x1E            // Bit mask of frame type field in frame control field
#define FRAME_TYPE_DL_DATA                      0x10            // Downlink data frame
#define FRAME_TYPE_UL_DATA                      0x12            // Uplink data frame
#define FRAME_TYPE_SCAN_REQUEST                 0x0
#define FRAME_TYPE_PAIRING_REQUEST              0x4
#define SECURITY_MODE_BITMASK                   0x60            // Security Mode field (Octet 0)          
#define SECURITY_MODE_BITSHIFT                  0x5             // Security Mode field (Octet 0)
#define FC_EXTENSION_BITMASK                    0x80            // FC Extension field (Octet 1)    
#define FC_EXTENSION_BITSHIFT                   0x7             // FC Extension field (Octet 1)


// Scan Request
#define NETWORKINFO_SFPERIOD_OFFSET                     0               // Byte offset of SF Period field within NetworkInfo field
#define NETWORKINFO_PACKETSIZE_SECURITY_OFFSET          1               // Byte offset of Packet Size and Security field within NetworkInfo field
#define NETWORKINFO_UL_ALLOCATION_OFFSET                2               // Byte offset of UL Allocation field within NetworkInfo field
#define NETWORKINFO_UL_ALLOCATION_NUMBER_OFFSET         0               // Byte offset of Uplink Allocation Number within UL Allocation field
#define NETWORKINFO_DL_ALLOCATION_NUMBER_OFFSET         0               // Byte offset of Downlink Allocation Number within DL Allocation field
#define NETWORKINFO_UL_ALLOCATION_SLOT_OFFSET           1               // Byte offset of Uplink Allocation Slot within UL Allocation field
#define NETWORKINFO_DL_ALLOCATION_SLOT_OFFSET           1               // Byte offset of Downlink Allocation Slot within DL Allocation field
#define NETWORKINFO_UL_DL_ALLOC_MAX_LENGTH              6               // Maximum supported length of the UL and DL alloc fields
#define NETWORKINFO_UL_DL_ALLOC_MIN_LENGTH              3               // Minimum supported length of the UL and DL alloc fields
#define ALLOCATION_SLOT_TIME_UNIT_US                    16              // Allocation slot time unit in us   

// Pairing Request
#define SWITCH_COUNTER_BITMASK                          0x3C             // Bit mask of switch counter bitfield in SwitchField (b2b3b4b5)
#define SWITCH_COUNTER_BITSHIFT                         2                // Bit shift of switch counter
#define SWITCH_INDICATOR_BITMASK                        0x3              // Bit mask of switch indicator bitfield in SwitchField (b0b1)
#define SWITCH_INDICATOR_BITSHIFT                       0                // Bit shift of switch indicator 
// The following offsets are defined from start of payload in Pairing Request packet. 
// Payload start is defined as TrackN field
#define PAIRING_REQ_PAYLOAD_TRACKN_OFFSET               0                
#define PAIRING_REQ_PAYLOAD_SLOTN_OFFSET                1
#define PAIRING_REQ_PAYLOAD_SHORTID_OFFSET              2
#define PAIRING_REQ_PAYLOAD_SHORTID_LENGTH              2
#define PAIRING_REQ_PAYLOAD_TABLE_LENGTH_OFFSET         4
#define PAIRING_REQ_PAYLOAD_HOP_TABLE_OFFSET            5 


static uint8_t LLWbms_parseDlConfigHeader(DLConfigHeader_Obj* pDlCfgHeader, uint8_t* packetData, uint16_t packetLength);
static uint8_t LLWbms_parseScanReqNetworkInfo(ScanReqNetworkInfo_Obj* pNetworkInfo, uint8_t* pPayload, uint16_t length);
static bool LLWbms_isBroadcastId(uint8_t* pUniqueId);

// Derived network parameters
// The ack length part of this data structure is set when receiving a Scan Request
// With FC Extension included. 
// For all other packets, this ack length is used (e.g. in calculation of 
// offsets for the fields after ack fields)
static NetworkParam_Obj LLWbms_networkParams = {1};    // 1 ACK byte as default

bool LLWbms_isDownLinkFrame(uint8_t* packetData, uint16_t packetLength)
{
    uint8_t frameType = (packetData[FRAME_HEADER_CONTROL_OFFSET] & FRAME_TYPE_BITMASK);
    
    return ((frameType == FRAME_TYPE_SCAN_REQUEST) || 
            (frameType == FRAME_TYPE_PAIRING_REQUEST) || 
            (frameType == FRAME_TYPE_DL_DATA));
}


bool LLWbms_isDownLinkDataFrame(uint8_t* packetData, uint16_t packetLength)
{
    uint8_t frameType = (packetData[FRAME_HEADER_CONTROL_OFFSET] & FRAME_TYPE_BITMASK);
    
    return (frameType == FRAME_TYPE_DL_DATA);
}


bool LLWbms_isScanRequest(uint8_t* packetData, uint16_t packetLength)
{
    uint8_t frameType = (packetData[FRAME_HEADER_CONTROL_OFFSET] & FRAME_TYPE_BITMASK);
    
    return (frameType == FRAME_TYPE_SCAN_REQUEST);
}


bool LLWbms_isPairingRequest(uint8_t* packetData, uint16_t packetLength)
{
    uint8_t frameType = (packetData[FRAME_HEADER_CONTROL_OFFSET] & FRAME_TYPE_BITMASK);
    
    return (frameType == FRAME_TYPE_PAIRING_REQUEST);
}


bool LLWbms_parseScanRequest(ScanRequestInfo_Obj* pScanInfo, uint8_t* packetData, uint16_t packetLength)
{
    bool bValid = false;
    DLConfigHeader_Obj frameHeader;
    
    uint8_t payloadOffset = LLWbms_parseDlConfigHeader(&frameHeader, packetData, packetLength);
    
    if(payloadOffset != 0)
    {
        if(packetLength >= (payloadOffset + DL_CONFIG_FRAME_PAYLOAD_LENGTH_MIN))
        {
            // Accept all 3 security levels (0,1 and 2) packets with sufficient length.
            // The payload of scan requests will not be encrypted for any of the security levels. 
            bValid = true;
        }
    }
    
    if(bValid)
    {
        // parse network info field
        uint8_t* pNetworkInfo = &packetData[payloadOffset];
        ScanReqNetworkInfo_Obj networkInfo;
        uint8_t numBytes = LLWbms_parseScanReqNetworkInfo(&networkInfo, pNetworkInfo, packetLength - payloadOffset);
    
        if(numBytes == 0) 
        {
            bValid = false;
        }
        else 
        {
            uint8_t numUlSlots = networkInfo.ulAllocaction[NETWORKINFO_UL_ALLOCATION_NUMBER_OFFSET];
            uint8_t numDlSlots = networkInfo.dlAllocaction[NETWORKINFO_DL_ALLOCATION_NUMBER_OFFSET];
            uint8_t ulAllocationSlot = networkInfo.ulAllocaction[NETWORKINFO_UL_ALLOCATION_SLOT_OFFSET];
            uint8_t dlAllocationSlot = networkInfo.dlAllocaction[NETWORKINFO_DL_ALLOCATION_SLOT_OFFSET];
         
            // Calculate the superframe interval and convert to microseconds
            pScanInfo->superFrameInterval = (numDlSlots * dlAllocationSlot) + (numUlSlots * ulAllocationSlot);
            pScanInfo->superFrameInterval *= ALLOCATION_SLOT_TIME_UNIT_US;
            
            pScanInfo->networkId = frameHeader.networkId;
            pScanInfo->switchCounter = (frameHeader.switchField & SWITCH_COUNTER_BITMASK) >> SWITCH_COUNTER_BITSHIFT;
            pScanInfo->switchIndicator = (frameHeader.switchField & SWITCH_INDICATOR_BITMASK) >> SWITCH_INDICATOR_BITSHIFT;
            pScanInfo->isBroadcast = LLWbms_isBroadcastId(frameHeader.uniqueId);
            
            if( pScanInfo->superFrameInterval == 0) 
            {
                bValid = false;
            }
        }
    }
    
    return bValid;
}


bool LLWbms_parsePairingRequest(PairingRequestInfo_Obj* pPairingInfo, uint8_t* packetData, uint16_t packetLength)
{
    bool bValid = false;
    DLConfigHeader_Obj frameHeader;
    
    uint8_t payloadOffset = LLWbms_parseDlConfigHeader(&frameHeader, packetData, packetLength);
      
    if(payloadOffset != 0)
    {
        if(packetLength >= (payloadOffset + DL_CONFIG_FRAME_PAYLOAD_LENGTH_MIN))
        {
            // Only packets with sufficient length are accepted
            bValid = true;
        }
    }
      
    if(bValid)
    {
        
        pPairingInfo->networkId = frameHeader.networkId;
        pPairingInfo->switchCounter = (frameHeader.switchField & SWITCH_COUNTER_BITMASK) >> SWITCH_COUNTER_BITSHIFT;
        pPairingInfo->switchIndicator = (frameHeader.switchField & SWITCH_INDICATOR_BITMASK) >> SWITCH_INDICATOR_BITSHIFT;
        uint8_t securityMode = (frameHeader.frameControl[0] & SECURITY_MODE_BITMASK) >> SECURITY_MODE_BITSHIFT;
        
        // For pairing requests with security mode 0 parse the payload fields, starting from TrackN field, 
        // and update the channel table. 
        // For security mode 1 and 2 the payload is encrypted and shall be ignored. Also, the channel table shall
        // NOT be updated for security mode 1 and 2. 
        if(securityMode == 0)
        {
            uint8_t* pPayLoad = packetData + payloadOffset;
            pPairingInfo->trackN = pPayLoad[PAIRING_REQ_PAYLOAD_TRACKN_OFFSET];
            pPairingInfo->slotN = pPayLoad[PAIRING_REQ_PAYLOAD_SLOTN_OFFSET];
            memcpy(&pPairingInfo->shortId, &pPayLoad[PAIRING_REQ_PAYLOAD_SHORTID_OFFSET], PAIRING_REQ_PAYLOAD_SHORTID_LENGTH);
            pPairingInfo->tableLength = pPayLoad[PAIRING_REQ_PAYLOAD_TABLE_LENGTH_OFFSET];
            uint8_t* pHoptable = &pPayLoad[PAIRING_REQ_PAYLOAD_HOP_TABLE_OFFSET];
            
            // Update the channel table
            if(pPairingInfo->tableLength <= MAX_DATA_CHANNELS)
            {
                LLWbms_channelUpdateDataTable(pHoptable, pPairingInfo->tableLength);
            }
            else
            {
                bValid = false;
            }
        }
    }
    
    return bValid;
}


//! \brief Parse the frame header of DownLink configuration frames
//!        The frame header consists for the fields as in the DLConfigHeader_Obj struct. 
//!        The packet content after those fields is defined as payload, where the payload content depends on frame type. 
//!
//! \param[out] DLConfigHeader_Obj
//!            Pointer to Downlink Config frame header struct
//!
//! \param[in] packetData
//!            PDU data. The expected format of the packet shall be as written to memory by RF Core:
//!            Header, Payload, 4 bytes CRC, RSSI, 4 bytes Timestamp, 1 byte status 
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return uint8_t
//!         offset of payload in packetData if header is parsed correct, or 0 otherwise
uint8_t LLWbms_parseDlConfigHeader(DLConfigHeader_Obj* pDlCfgHeader, uint8_t* packetData, uint16_t packetLength)
{
    bool bValid = true;
    uint8_t offset = 0;
    
    if(packetLength < (DL_CONFIG_FRAME_HEADER_LENGTH_MIN + DL_CONFIG_FRAME_PAYLOAD_LENGTH_MIN + RFCORE_NUM_APPENDED_BYTES))
    {
        // Length is too short
        bValid = false;
    }
    
    if(bValid)
    {
        pDlCfgHeader->length = packetData[FRAME_HEADER_LENGTH_OFFSET];
        pDlCfgHeader->frameControl[0] = packetData[FRAME_HEADER_CONTROL_OFFSET];
        pDlCfgHeader->frameControl[1] = packetData[FRAME_HEADER_CONTROL_OFFSET + 1];
        pDlCfgHeader->networkId = packetData[FRAME_HEADER_NETWORKID_OFFSET];
        uint8_t frameType = (packetData[FRAME_HEADER_CONTROL_OFFSET] & FRAME_TYPE_BITMASK);
        
        memcpy(pDlCfgHeader->uniqueId, &packetData[FRAME_HEADER_UNIQUEID_OFFSET], FRAME_HEADER_UNIQUEID_LENGTH);
        
        // Check if FC Extension field is present
        bool bfcExtension = (pDlCfgHeader->frameControl[1] & FC_EXTENSION_BITMASK) >> FC_EXTENSION_BITSHIFT;
        offset = FRAME_HEADER_FCEXTENSION_OFFSET;
        uint8_t ackLength = 0;
        if(bfcExtension)
        {
            pDlCfgHeader->fcExtension = packetData[FRAME_HEADER_FCEXTENSION_OFFSET];
            offset++;
            ackLength = pDlCfgHeader->fcExtension & FC_EXTENSION_ACK_LENGTH_BITMASK;
            // Store ack length in network parameter struct. This is used to determine ack length
            // of all sub sequent packets without FC Extension field. 
            LLWbms_networkParams.ackLength = ackLength;
        }
        else if(frameType == FRAME_TYPE_PAIRING_REQUEST)
        {
            // For pairing Request frame use ack length set by a previous packet with FC Extension
            ackLength = LLWbms_networkParams.ackLength;
        }
        
        pDlCfgHeader->switchField = packetData[offset++];
        
        if(ackLength > 0)
        {
            memcpy(pDlCfgHeader->ack, &packetData[offset], ackLength);
            offset += ackLength;
        }
        
        // Check if security header is present
        uint8_t securityMode = (pDlCfgHeader->frameControl[0] & SECURITY_MODE_BITMASK) >> SECURITY_MODE_BITSHIFT;
        if((securityMode == 1) || (securityMode == 2))
        {
            memcpy(pDlCfgHeader->securityHeader, &packetData[offset], FRAME_HEADER_SECURITY_HEADER_LENGTH);
            offset += FRAME_HEADER_SECURITY_HEADER_LENGTH;
        }
    }
    
    if(bValid)
    {
        return offset;
    }
    else
    {
        return 0;
    }
}


//! \brief Parse the network info field in scan request frames
//!
//! \param[out] ScanReqNetworkInfo_Obj
//!            Pointer to Network Info struct
//!
//! \param[in] pPayload
//!            Pointer to the data containing network info field in the packet
//!
//! \param[in] length
//!            Length of payload
//!
//! \return uint8_t
//!         number of bytes parsed if parsed correctly, or 0 otherwise
uint8_t LLWbms_parseScanReqNetworkInfo(ScanReqNetworkInfo_Obj* pNetworkInfo, uint8_t* pPayload, uint16_t length)
{
    bool bValid = true;
    uint8_t offset = 0;
    
    pNetworkInfo->sfPeriod = pPayload[NETWORKINFO_SFPERIOD_OFFSET];
    pNetworkInfo->packetSizeSecurity = pPayload[NETWORKINFO_PACKETSIZE_SECURITY_OFFSET];
    
    // Parse UL Allocation fields (this field has 3 subfields where the last has variable length)
    // Check UL allocation number to calculate correct field length of UL Allocation field
    uint8_t ulAllocNumber = pPayload[NETWORKINFO_UL_ALLOCATION_OFFSET];
    // The field length is UL Allocation Number (1 byte) + UL Allocation Slot (1 byte) + UL Allocation Occupancy (N_UPLINK_SLOTS + 8 - 1)/8 bytes)
    uint8_t ulAllocFieldLength = 1 + 1 + ((ulAllocNumber + 8 - 1)/8);
    
    if((ulAllocFieldLength <= NETWORKINFO_UL_DL_ALLOC_MAX_LENGTH) && (ulAllocFieldLength >= NETWORKINFO_UL_DL_ALLOC_MIN_LENGTH))
    {
        // Find DL Allocation number, and calculate DL allocation field length
        offset = NETWORKINFO_UL_ALLOCATION_OFFSET + ulAllocFieldLength;
        uint8_t dlAllocNumber = pPayload[offset];
        // The field length is DL Allocation Number (1 byte) + DL Allocation Slot (1 byte) + DL Allocation Occupancy (N_UPLINK_SLOTS + 8 - 1)/8 bytes)
        uint8_t dlAllocFieldLength = 1 + 1 + ((dlAllocNumber + 8 - 1)/8);
        
        if((dlAllocFieldLength <= NETWORKINFO_UL_DL_ALLOC_MAX_LENGTH) && (dlAllocFieldLength >= NETWORKINFO_UL_DL_ALLOC_MIN_LENGTH))
        {
            // Both DL and UL allocation are within correct size limits
            memcpy(pNetworkInfo->ulAllocaction, &pPayload[NETWORKINFO_UL_ALLOCATION_OFFSET], ulAllocFieldLength);
            memcpy(pNetworkInfo->dlAllocaction, &pPayload[offset], dlAllocFieldLength);
            offset += dlAllocFieldLength;
        }
        else
        {
            bValid = false;
        }
    }
    else
    {
        bValid = false;
    }
    
    if(bValid)
    {
        // Parse rest of the fields
        pNetworkInfo->ul2dlTime = pPayload[offset++];
        pNetworkInfo->dl2ulTime = pPayload[offset++];
        pNetworkInfo->ul2ulTime = pPayload[offset++];
        pNetworkInfo->keepAlive = pPayload[offset++];
        pNetworkInfo->wakeupFrameInterval = pPayload[offset++];
        pNetworkInfo->normalSf = pPayload[offset++];
    }
    
    if(bValid)
    {
        return offset;
    }
    else
    {
        return 0;
    }
}


//! \brief Check if unique id is the broadcast id 
//!        (The broadcast id has all bytes to 0xFF)
//!
//! \param[in] pUniqueId 
//!            Pointer to unique id
//!
//! \return bool
//!         True if the unique id is the broadcast id, and false otherwise
static bool LLWbms_isBroadcastId(uint8_t* pUniqueId)
{
    for(int i=0; i<FRAME_HEADER_UNIQUEID_LENGTH; i++)
    {
        if(pUniqueId[i] != 0xFF)
        {
            return false;
        }
    }
    
    return true;
}

