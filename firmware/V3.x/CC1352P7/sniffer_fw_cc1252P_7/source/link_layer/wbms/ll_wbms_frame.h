/******************************************************************************
*  Filename:       ll_wbms_frame.h
*
*  Description:    Header file for the wBMS Link Layer Frame functionality
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
#ifndef LL_WBMS_FRAME_H
#define LL_WBMS_FRAME_H

#include <stdint.h>
#include <stdbool.h>


// Downlink Configuration Frame Header
// As defined in wBMS MAC specification v1.31 section 8.4.1
typedef struct DLConfigHeader_Obj
{
    uint8_t length;
    uint8_t frameControl[2];
    uint8_t networkId;
    uint8_t uniqueId[9];
    uint8_t fcExtension;
    uint8_t ack[4];
    uint8_t switchField;
    uint8_t securityHeader[5];
} DLConfigHeader_Obj;


// Scan Request Network info field
// As defined in wBMS MAC specification v1.31 section 8.4.1.1
typedef struct ScanReqNetworkInfo_Obj
{
    uint8_t sfPeriod;                   // Superframe period
    uint8_t packetSizeSecurity;         // Packet size and security field
    uint8_t ulAllocaction[6];           // Uplink Allocation field (This field has variable size >= 3)
    uint8_t dlAllocaction[6];           // Downlink Allocation field (This field has variable size >= 3)
    uint8_t ul2dlTime;                  // UL 2 DL interfame time
    uint8_t dl2ulTime;                  // DL 2 UL interfame time
    uint8_t ul2ulTime;                  // UL 2 UL interfame time
    uint8_t keepAlive;                  // Keep Alive PS Frequency field
    uint8_t wakeupFrameInterval;        // Wakeup Frame Interval
    uint8_t normalSf;                   // Normal SF between Wakeup Frame Intervals field
} ScanReqNetworkInfo_Obj;


// Information from scan request
typedef struct ScanRequestInfo_Obj
{
    uint8_t networkId;                  // Network Identifier
    uint16_t superFrameInterval;        // Superframe interval in us
    bool isBroadcast;                   // Is unique Id set to the broadcast Id?
    uint8_t switchCounter;              // Number of superframe periods until state switch (if switchIndicator is true)
    bool switchIndicator;               // True if switch indicator bits in SwitchField of Scan Request is non-zero
} ScanRequestInfo_Obj;


// Information from pairing request
typedef struct PairingRequestInfo_Obj
{
    uint8_t networkId;                  // Network Identifier
    uint8_t switchCounter;              // Number of superframe periods until state switch (if switchIndicator is true)
    bool switchIndicator;               // True if switch to connected mode is indicated 
    uint8_t trackN;                     // TrackN field from Pairing Request frame
    uint8_t slotN;                      // SlotN field from Pairing Request frame
    uint8_t shortId[2];                 // Short Id field from Pairing Request frame
    uint8_t tableLength;                // Channel hop table length
} PairingRequestInfo_Obj;


// Network paramaters. 
// Derived from information in the Configuration frames
typedef struct NetworkParam_Obj
{
    uint8_t ackLength;
} NetworkParam_Obj;

//! \brief Checks if the packet is a DownLink frame type
//!
//! \param[in] packetData
//!            PDU data. The expected format of the packet shall be as written to memory by RF Core:
//!            Header, Payload, 4 bytes CRC, RSSI, 4 bytes Timestamp, 1 byte status 
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return bool
//!         true if packet is a DownLink frame, and false otherwise
bool LLWbms_isDownLinkFrame(uint8_t* packetData, uint16_t packetLength);


//! \brief Checks if the packet is a DownLink data frame
//!
//! \param[in] packetData
//!            PDU data. The expected format of the packet shall be as written to memory by RF Core:
//!            Header, Payload, 4 bytes CRC, RSSI, 4 bytes Timestamp, 1 byte status 
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return bool
//!         true if packet is a DownLink data frame, and false otherwise
bool LLWbms_isDownLinkDataFrame(uint8_t* packetData, uint16_t packetLength);


//! \brief Checks if the packet is a Scan Request
//!
//! \param[in] packetData
//!            PDU data. The expected format of the packet shall be as written to memory by RF Core:
//!            Header, Payload, 4 bytes CRC, RSSI, 4 bytes Timestamp, 1 byte status 
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return bool
//!         true if packet is a Scan Request frame, and false otherwise
bool LLWbms_isScanRequest(uint8_t* packetData, uint16_t packetLength);


//! \brief Checks if the packet is a Pairing Request
//!
//! \param[in] packetData
//!            PDU data. The expected format of the packet shall be as written to memory by RF Core:
//!            Header, Payload, 4 bytes CRC, RSSI, 4 bytes Timestamp, 1 byte status 
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return bool
//!         true if packet is a Pairing Request frame, and false otherwise
bool LLWbms_isPairingRequest(uint8_t* packetData, uint16_t packetLength);


//! \brief Parse the Scan Request frame
//!
//! \param[out] ScanRequestInfo_Obj
//!            Pointer to Scan Request Information struct
//!
//! \param[in] packetData
//!            PDU data. The expected format of the packet shall be as written to memory by RF Core:
//!            Header, Payload, 4 bytes CRC, RSSI, 4 bytes Timestamp, 1 byte status 
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return bool
//!         true if is a valid Scan Request frame, and false otherwise
bool LLWbms_parseScanRequest(ScanRequestInfo_Obj* pScanInfo, uint8_t* packetData, uint16_t packetLength);


//! \brief Parse the Pairing Request frame
//!
//! \param[out] PairingRequestInfo_Obj
//!            Pointer to Pairing Requestion Information struct
//!
//! \param[in] packetData
//!            PDU data. The expected format of the packet shall be as written to memory by RF Core:
//!            Header, Payload, 4 bytes CRC, RSSI, 4 bytes Timestamp, 1 byte status 
//!
//! \param[in] packetLength
//!            Length of PDU data
//!
//! \return bool
//!         true if is a valid Pairing Request frame, and false otherwise
bool LLWbms_parsePairingRequest(PairingRequestInfo_Obj* pPairingInfo, uint8_t* packetData, uint16_t packetLength);

#endif

