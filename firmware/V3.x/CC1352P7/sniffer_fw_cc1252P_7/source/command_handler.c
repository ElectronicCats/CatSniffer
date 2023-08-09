/******************************************************************************
*  Filename:       command_handler.c
*
*  Description:    Source file for the command handler. This module handles
*                  commands and command responses on the host interface.
*
* Copyright (C) 2017-2022 Texas Instruments Incorporated - http://www.ti.com/
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
#include "command_handler.h"
#include "host_if.h"
#include "version.h"
#include "common.h"

#define DEFAULT_FW_ID 100
#ifndef FW_ID
#define FW_ID DEFAULT_FW_ID
#endif

/* driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/chipinfo.h)

/* Since chip info is defined as enum, cant use a preprocessor MACRO to undef 
   if old SDK`s are used there is no clean way to ammend to the enum list, 
   make sure enum values match with latest SDK`s chipinfo.h*/
enum{   
    CHIP_TYPE_EX_CC1312R7 = 23,
    CHIP_TYPE_EX_CC1352P7 = 26,
    CHIP_TYPE_EX_CC2652R7 = 27,
}ChipType_ex_t;

uint16_t g_ConnectedChipId = 0;

// Storage for response packets
static CommandResponsePacket_Obj CommandHandler_commandResponse;
static CommandPingResponsePacket_Obj CommandHandler_commandPingResponse;

// Local functions
static void CommandHandler_constructResponsePacket(CommandResponsePacket_Obj* cmdResponse, uint8_t status);
static void CommandHandler_constructPingResponsePacket(CommandPingResponsePacket_Obj* cmdPingResponse, uint8_t status);
static void CommandHandler_forwardResponsePacket(uint8_t* cmdResponse, uint16_t packetLength);
static uint8_t CommandHandler_computeFcs(uint8_t* dataBuffer, uint16_t length);
static uint8_t CommandHandler_verifyPacket(CommandPacket_Obj* cmdPacket);
static uint8_t CommandHandler_searchForPacketHeader(CommandPacket_Obj* cmdPacket);
static void CommandHandler_flushHostInterface(uint16_t expectedLength);
static uint16_t CommandHandler_getChipId(void);
static uint8_t CommandHandler_getChipRevision(void);
static uint16_t CommandHandler_getFwRevision(void);
static uint8_t CommandHandler_getFwId(void);


uint8_t CommandHandler_getCommand(CommandPacket_Obj* cmdPacket)
{
    // Search for packet header
    uint8_t status = CommandHandler_searchForPacketHeader(cmdPacket);
    if(status != COMMAND_OK)
    {
        return status;
    }

    // Compute length of remaining packet content
    uint16_t remainingLen = cmdPacket->genPacketHdr.packetLen + sizeof(CommandPacketFooter_Obj);

    // Check if command length is valid
    if(remainingLen > (MAX_COMMAND_PAYLOAD_LENGTH + sizeof(CommandPacketFooter_Obj)))
    {
        // The length indicates that the packet is longer than maximum length of a
        // command packet.

        // The remaining bytes must be read/removed from host IF
        CommandHandler_flushHostInterface(remainingLen);
        return COMMAND_INVALID;
    }

    // Start timeout timer

    // Read the rest of the packet
    HostIF_readBuffer((uint8_t*) cmdPacket->payload, remainingLen);

    status = CommandHandler_verifyPacket(cmdPacket);
    return status;
}


void CommandHandler_respondCommand(uint8_t status)
{
    CommandHandler_constructResponsePacket(&CommandHandler_commandResponse, status);
    CommandHandler_forwardResponsePacket((uint8_t*)&CommandHandler_commandResponse, sizeof(CommandHandler_commandResponse));
}


void CommandHandler_respondPingCommand(uint8_t status)
{
    CommandHandler_constructPingResponsePacket(&CommandHandler_commandPingResponse, status);
    CommandHandler_forwardResponsePacket((uint8_t*)&CommandHandler_commandPingResponse, sizeof(CommandHandler_commandPingResponse));
}


//! \brief Construct command response packet
//!
//! \param[in] cmdResponse
//!            Pointer to buffer for the command response packet
//!
//! \param[in] status
//!            Status to set in the command response
//!
//! \return None
void CommandHandler_constructResponsePacket(CommandResponsePacket_Obj* cmdResponse, uint8_t status)
{
    cmdResponse->genPacketHdr.sof = START_OF_FRAME_DELIMITER;
    cmdResponse->genPacketHdr.packetType = PACKET_TYPE_COMMAND_RESPONSE;
    cmdResponse->genPacketHdr.packetLen = COMMAND_RESPONSE_PAYLOAD_LENGTH;
    cmdResponse->status = status;
    cmdResponse->cmdPacketFtr.fcs = CommandHandler_computeFcs(&cmdResponse->genPacketHdr.packetType, \
                                       sizeof(cmdResponse->genPacketHdr.packetType) + sizeof(cmdResponse->genPacketHdr.packetLen) + sizeof(cmdResponse->status));

    cmdResponse->cmdPacketFtr.genPacketFooter.eof = END_OF_FRAME_DELIMITER;
}


//! \brief Construct command response packet
//!
//! \param[in] cmdResponse
//!            Pointer to buffer for the command response packet
//!
//! \param[in] status
//!            Status to set in the command response
//!
//! \return None
void CommandHandler_constructPingResponsePacket(CommandPingResponsePacket_Obj* cmdPingResponse, uint8_t status)
{
    cmdPingResponse->genPacketHdr.sof = START_OF_FRAME_DELIMITER;

    cmdPingResponse->genPacketHdr.packetType = PACKET_TYPE_COMMAND_RESPONSE;
    cmdPingResponse->genPacketHdr.packetLen = COMMAND_PING_RESPONSE_PAYLOAD_LENGTH;

    cmdPingResponse->status = status;
    cmdPingResponse->chipId = g_ConnectedChipId = CommandHandler_getChipId();
    cmdPingResponse->chipRev = CommandHandler_getChipRevision();
    cmdPingResponse->fwId = CommandHandler_getFwId();
    cmdPingResponse->fwRev = CommandHandler_getFwRevision();

    cmdPingResponse->cmdPacketFtr.fcs = CommandHandler_computeFcs(&cmdPingResponse->genPacketHdr.packetType, \
                                       sizeof(cmdPingResponse->genPacketHdr.packetType) + sizeof(cmdPingResponse->genPacketHdr.packetLen) + COMMAND_PING_RESPONSE_PAYLOAD_LENGTH);

    cmdPingResponse->cmdPacketFtr.genPacketFooter.eof = END_OF_FRAME_DELIMITER;
}


//! \brief Forward command response packet to host
//!
//! \param[in] cmdResponse
//!            Pointer to command response packet
//!
//! \param[in] packetLength
//!            Length of command response packet
//!
//! \return None
void CommandHandler_forwardResponsePacket(uint8_t* cmdResponse, uint16_t packetLength)
{
    HostIF_writeBuffer((uint8_t*)cmdResponse, packetLength);
}


//! \brief Compute frame check sequence value from a data buffer
//!
//! \param[in] dataBuffer
//!            Pointer to start of the data buffer to compute FCS over.
//!
//! \param[in] length
//!            Length of the data buffer to compute FCS over.
//!
//! \return Computed frame sequence value
uint8_t CommandHandler_computeFcs(uint8_t* dataBuffer, uint16_t length)
{
    uint32_t result=0;
    for(int i=0; i<length; i++)
    {
        result += *((uint8_t*)(dataBuffer + i));
    }

    return (result & 0xFF);
}


//! \brief Verify command packet
//!
//! \param[in] cmdPacket
//!            Pointer to command packet
//!
//! \return COMMAND_OK or COMMAND_FCS_FAILED
uint8_t CommandHandler_verifyPacket(CommandPacket_Obj* cmdPacket)
{
    uint8_t status = COMMAND_OK;
    uint16_t fcsIndex = sizeof(cmdPacket->genPacketHdr) + cmdPacket->genPacketHdr.packetLen;

    uint8_t expectedFcs = CommandHandler_computeFcs(&cmdPacket->genPacketHdr.packetType,
                          PACKET_TYPE_LENGTH + PACKET_LEN_LENGTH + cmdPacket->genPacketHdr.packetLen);
    if(expectedFcs != *((uint8_t*)(cmdPacket) + fcsIndex))
    {
        status = COMMAND_FCS_FAILED;
    }

    return status;
}


//! \brief Search for packet header in bytes read from host interface.
//!
//!       This function blocks until first byte of SOF is read from host.
//!       It then blocks until a second byte is read.
//!       - If the two bytes does not match SOF it returns with error.
//!       - If the two bytes matches SOF, it blocks until the complete packet
//!         header is read, and returns success.
//!
//! \param[in] cmdPacket
//!            Pointer to buffer for command packet. If a successful packet
//!            header is found, this function will populate the packet header values
//!            of the given packet.
//!
//! \return COMMAND_OK or COMMAND_INVALID
static uint8_t CommandHandler_searchForPacketHeader(CommandPacket_Obj* cmdPacket)
{
    uint8_t status = COMMAND_INVALID;
    uint8_t readValue = 0;
    uint8_t sofByte0Value = START_OF_FRAME_DELIMITER & 0xFF;
    uint8_t sofByte1Value = (START_OF_FRAME_DELIMITER & 0xFF00) >> 8;

    // Read bytes from host IF until 0xFD is found (first byte of Start of Frame)
    while(readValue != sofByte0Value)
    {
        HostIF_readBuffer(&readValue, 1);
    }

    // It is expected that next byte is second byte of SOF (0x50)
    HostIF_readBuffer(&readValue, 1);
    if(readValue != sofByte1Value)
    {
        // Packet is invalid
        status = COMMAND_INVALID;
    }
    else
    {
        // Correct SOF is found. Read rest of header
        cmdPacket->genPacketHdr.sof = START_OF_FRAME_DELIMITER;
        HostIF_readBuffer((uint8_t*)&(cmdPacket->genPacketHdr.packetType), sizeof(GeneralPacketHeader_Obj) - SOF_LENGTH);
        status = COMMAND_OK;
    }

    return status;
}


//! \brief Read and throw bytes from Host IF.
//!
//!        This function is used to flush the host interface. It can be used in
//!        the case where the received command is too long (no available buffer
//!        space for received command). The function blocks until the expected
//!        number of bytes are read.
//!
//! \param[in] expectedLength
//!            Expected number of bytes to receive and throw.
//!
//! \return None
static void CommandHandler_flushHostInterface(uint16_t expectedLength)
{
#define READ_CHUNK_SIZE         8
    uint8_t buffer[READ_CHUNK_SIZE];
    uint16_t remainingBytes = expectedLength;
    uint16_t readSize = 0;
    int32_t numReadBytes;

    while(remainingBytes)
    {
        readSize = (remainingBytes < READ_CHUNK_SIZE) ? remainingBytes : READ_CHUNK_SIZE;
        numReadBytes = HostIF_readBuffer(buffer, readSize);
        remainingBytes -= readSize;
        if(numReadBytes < READ_CHUNK_SIZE && remainingBytes)
        {
            // Number of bytes was less than the chunk size. A UART read timeout
            // has probably occured. Abort this loop to avoid be stuck here 
            // forever waiting for data that are not received. 
            break;
        }
    }
}


//! \brief Get chip id
//!
//! \return Chip id hex value
static uint16_t CommandHandler_getChipId(void)
{
    ChipType_t chipType = ChipInfo_GetChipType();
    switch(chipType)
    {
        case CHIP_TYPE_CC1310:
            return 0x1310;
        case CHIP_TYPE_CC1350:
            return 0x1350;
        case CHIP_TYPE_CC2620:
            return 0x2620;
        case CHIP_TYPE_CC2630:
            return 0x2630;
        case CHIP_TYPE_CC2640:
            return 0x2640;
        case CHIP_TYPE_CC2650:
            return 0x2650;
        case CHIP_TYPE_CC2652:
            return 0x2652;
        case CHIP_TYPE_CC1352:
            return 0x1352;
        case CHIP_TYPE_CC1352P:
            return 0x1353;
        case CHIP_TYPE_CC1312:
            return 0x1312;
        case CHIP_TYPE_EX_CC2652R7:
            return 0x2652;
        case CHIP_TYPE_EX_CC1352P7:
            return 0x1353;
        case CHIP_TYPE_EX_CC1312R7:
            return 0x1312;
        default:
            return 0;
    }
}


//! \brief Get chip revision
//!
//! \return Chip revision value
static uint8_t CommandHandler_getChipRevision(void)
{
    HwRevision_t hwRevision = ChipInfo_GetHwRevision();

    switch(hwRevision)
    {
        case HWREV_1_0:
            return 0x10;
        case HWREV_1_1:
            return 0x11;
        case HWREV_2_0:
            return 0x20;
        case HWREV_2_1:
            return 0x21;
        case HWREV_2_2:
            return 0x22;
        case HWREV_2_3:
            return 0x23;
        default:
            return 0;
    }
}


//! \brief Get firmware revision
//!
//! \return Firmware major revision (MSB) and minor revision (LSB)
static uint16_t CommandHandler_getFwRevision(void)
{
    return (MAJOR_VERSION << 8) | MINOR_VERSION;
}

//! \brief Get firmware ID
//!
//! \return Firmware Id
static uint8_t CommandHandler_getFwId(void)
{

    return FW_ID;
}

