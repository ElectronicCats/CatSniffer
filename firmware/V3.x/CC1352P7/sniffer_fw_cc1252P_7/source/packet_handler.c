/******************************************************************************
*  Filename:       packet_handler.c
*
*  Description:    Source file for packet handler. This module handles data and
*                  and error packet, and forwarding of these packets to host.
*
* Copyright (C) 2017-2018 Texas Instruments Incorporated - http://www.ti.com/
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

#include "packet_handler.h"
#include "host_if.h"
#include "phy_manager.h"
#include "phy_if_ble_5.h"
#include "ble/ll_ble.h"
#include "common.h"
#include <string.h>


// Local functions
static uint16_t PacketHandler_constructDataPacket(DataPacket_Obj* dataPacket, uint16_t payloadLength);
static void PacketHandler_constructErrorPacket(ErrorReportPacket_Obj* errorReportPacket, uint8_t error);
static void PacketHandler_convertStatusToCc1200FormatIeee154Api(uint8_t* status);
static void PacketHandler_convertStatusToCc1200FormatPropApi(uint8_t* status);
static void PacketHandler_convertStatusToCc1200FormatBle5Api(uint8_t* status);


extern uint16_t PacketHandler_processPacket(DataPacket_Obj* dataPacket, uint16_t payloadLength)
{
    return PacketHandler_constructDataPacket(dataPacket, payloadLength);
}


//! \brief Construct data packet
//!
//!       The caller of this function is responsible for allocating the memory
//!       large enough for a data packet, and for writing the payload of the data
//!       packet before this function is called. This function will populate the
//!       the rest of the datapacket.
//!
//! \param[in] dataPacket
//!            Pointer to buffer for the data packet
//!
//! \param[in] payloadLength
//!            Length of the payload field of the data packet
//!
//! \return Updated payload length
uint16_t PacketHandler_constructDataPacket(DataPacket_Obj* dataPacket, uint16_t payloadLength)
{
    dataPacket->genPacketHeader.sof = START_OF_FRAME_DELIMITER;
    dataPacket->genPacketHeader.packetType = PACKET_TYPE_DATA;
    dataPacket->genPacketHeader.packetLen = TIMESTAMP_LENGHT + payloadLength;

    if(PhyManager_getRfApi() == IEEE_802_15_4)
    {
        uint8_t* status = (uint8_t*)(dataPacket->payload + payloadLength - 1);
        PacketHandler_convertStatusToCc1200FormatIeee154Api(status);
    }
    if(PhyManager_getRfApi() == BLE_5_1M)
    {
        // There are two status bytes added for BLE 5, while the Host Interface 
        // packet format has only one status byte. 
        // Chop off the last status byte by subtracting one from payload length.
        payloadLength -= 1;
        dataPacket->genPacketHeader.packetLen -= 1;
        
        uint8_t* status = (uint8_t*)(dataPacket->payload + payloadLength - 1);
        PacketHandler_convertStatusToCc1200FormatBle5Api(status);
    }
    // For WBMS the status byte is followed by an 'extra sniffer info' byte at the end
    else if(PhyManager_getRfApi() == WBMS)
    {
        uint8_t* status = (uint8_t*)(dataPacket->payload + payloadLength - 2);
        PacketHandler_convertStatusToCc1200FormatPropApi(status);
    }
    // For other RF APIs
    else 
    {
        uint8_t* status = (uint8_t*)(dataPacket->payload + payloadLength - 1);
        PacketHandler_convertStatusToCc1200FormatPropApi(status);
    }

    uint8_t* eof = (uint8_t*)(dataPacket->payload + payloadLength);
    Common_set16BitValueLE(eof, END_OF_FRAME_DELIMITER);
    return payloadLength;
}


extern void PacketHandler_processError(uint8_t* errorPacket, uint8_t error)
{
    PacketHandler_constructErrorPacket((ErrorReportPacket_Obj*)errorPacket, error);
}


//! \brief Construct error packet
//!
//!       The caller of this function is responsible for allocating the memory
//!       large enough for an error packet. This function will populate the
//!       the error packet.
//!
//! \param[in] errorReportPacket
//!            Pointer to buffer for the error packet.
//!
//! \param[in] error
//!            Error code to set in the error packet.
//!
//! \return None
void PacketHandler_constructErrorPacket(ErrorReportPacket_Obj* errorReportPacket, uint8_t error)
{
    errorReportPacket->genPacketHeader.sof = START_OF_FRAME_DELIMITER;
    errorReportPacket->genPacketHeader.packetType = PACKET_TYPE_ERROR;
    errorReportPacket->genPacketHeader.packetLen  = ERROR_REPORT_PAYLOAD_LENGTH;
    *errorReportPacket->error = error;
    errorReportPacket->genPacketFooter.eof = END_OF_FRAME_DELIMITER;
}


//! \brief Convert the status byte to CC1200 format (CC26xx IEEE 15.4 API version
//!        of this function). 
//!
//! \param[in] status
//!            Pointer to the status byte added by RF Core
//!
//! \return None
void PacketHandler_convertStatusToCc1200FormatIeee154Api(uint8_t* status)
{
    // Get value of the result bit field of the status byte (bit 7)
    uint8_t result = (*status & 0x80) >> 7;

    // Initialize status byte in CC1200 format.
    // Set CRC_OK false and LQI to invalid value (0)
    *status = 0;

    // The following values shall give CRC_OK true in new format:
    // - Result = 0 -> 'Packet received withour CRC error'
    // Other values keeps CRC_OK false.
    if(result == 0)
    {
        // Set CRC_OK -> 1
        *status |= 0x80;
    }
}


//! \brief Convert the status byte to CC1200 format (CC13xx PROP API version
//!        of this function). 
//!
//! \param[in] status
//!            Pointer to the status byte added by RF Core
//!
//! \return None
void PacketHandler_convertStatusToCc1200FormatPropApi(uint8_t* status)
{
    // Get value of the result bit field of the status byte (bits 6-7)
    uint8_t result = (*status & 0xC0) >> 6;

    // Initialize status byte in CC1200 format.
    // Set CRC_OK false and LQI to invalid value (0)
    *status = 0;

    // The following values shall give CRC_OK true in new format:
    // - Result = 0 -> 'Packet Received correctly, not ingored'
    // - Result = 2 -> 'Packet received correct, but can be ignored'
    // Other values keeps CRC_OK false.
    if((result == 0) || (result == 0x2))
    {
        // Set CRC_OK -> 1
        *status |= 0x80;
    }
}


//! \brief Convert the status byte to CC1200 format (BLE 5 API version
//!        of this function). 
//!
//! \param[in] status
//!            Pointer to the status byte added by RF Core
//!
//! \return None
void PacketHandler_convertStatusToCc1200FormatBle5Api(uint8_t* status)
{
     // Get value of the result bit field of the status byte (bit 7)
    uint8_t result = (*status & 0x80) >> 7;

    // Initialize status byte in CC1200 format.
    // Set CRC_OK false and LQI to invalid value (0)
    *status = 0;
    
    // The following values shall give CRC_OK true in new format:
    // - Result = 0 -> 'Packet received withour CRC error'
    // Other values keeps CRC_OK false.
    if(result == 0)
    {
        // Set CRC_OK -> 1
        *status |= 0x80;
    }
}




