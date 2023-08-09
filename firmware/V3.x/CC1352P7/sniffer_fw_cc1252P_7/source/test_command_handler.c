/******************************************************************************
*  Filename:       test_command_handler.c
*
*  Description:    Source file for test command handler. This module handles test
*                  commands.
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

#include "test_command_handler.h"
#include "command_packet.h"
#include "command_handler.h"
#include "phy_manager.h"
#include "radio_if.h"

#define TRANSMIT_SEQUENCE_MAX_PAYLOAD_LENGTH                            2047
#define PACKET_LENGHT_OFFSET_15_4_G                                     -4      // Length offset setting for packet header length used for 15.4g settings
#define HEADER_LENGTH_15_4_G                                            2       // Header for 15.4G support
#define HEADER_LENGTH                                                   0       // No header for Easylink support

static uint8_t packet[TRANSMIT_SEQUENCE_MAX_PAYLOAD_LENGTH];

void TestCommandHandle_initializePacketPayload(uint8_t* packet, uint16_t packetLength);
void TestCommandHandle_initializePacketHeader(uint8_t* packet, uint16_t packetLength);
uint8_t TestCommandHandler_transmitOneSequence(uint8_t* commandData, uint16_t length);
void TestCommandHandler_transmitSeries(uint16_t packetLen, uint16_t numPackets);

uint8_t TestCommandHandler_transmitSequence(uint8_t* commandData, uint16_t length)
{
    RadioIF_initTx();

    if( (length < TEST_COMMAND_TRANSMIT_SEQUENCE_REPETITIONS_SIZE + TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_LENGTH_SIZE + TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_NUMBER_SIZE)
         || (length % 2))
    {
        RadioIF_disableTx();
        return COMMAND_INVALID;
    }

    TestCommandHandle_initializePacketPayload(packet, TRANSMIT_SEQUENCE_MAX_PAYLOAD_LENGTH);

    uint8_t* pSequenceRepetitions = (commandData + TEST_COMMAND_TRANSMIT_SEQUENCE_REPETITIONS_OFFSET);
    uint16_t sequenceRepetitions = *(uint16_t*)pSequenceRepetitions;

    for(int i = 0; i < sequenceRepetitions; i++)
    {
        uint8_t status = TestCommandHandler_transmitOneSequence(commandData, length);
        if(status != COMMAND_OK)
        {
            RadioIF_disableTx();
            return status;
        }
    }
    
    RadioIF_disableTx();
    return COMMAND_OK;
}


//! \brief Transmit packet sequence
//!
//! \param[in] commandData
//!            payload data for the TEST_CMD_TRANSMIT_SEQUENCE.
//!
//! \param[in] length
//!            length of payload data
//!
//! \return status that indicates successful or failed handling of the command.
//!         COMMAND_OK - The command was handled successfully.
//!         COMMAND_INVALID - The command is invalid (e.g. payload does not match
//!         the command type.)
uint8_t TestCommandHandler_transmitOneSequence(uint8_t* commandData, uint16_t length)
{
    uint16_t numSeries = (length - TEST_COMMAND_TRANSMIT_SEQUENCE_REPETITIONS_SIZE) / (TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_LENGTH_SIZE + TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_NUMBER_SIZE);

    for(int i = 0; i < numSeries; i++)
    {
        uint8_t* serieStart = (commandData + TEST_COMMAND_TRANSMIT_SEQUENCE_REPETITIONS_SIZE);
        serieStart += i * (TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_LENGTH_SIZE + TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_NUMBER_SIZE);

        uint8_t* pPacketLen = serieStart + TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_LENGTH_OFFSET;
        uint16_t packetLen = *(uint16_t*)pPacketLen;

        uint8_t* pNumPackets = serieStart + TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_NUMBER_OFFSET;
        uint16_t numPackets = *(uint16_t*)pNumPackets;

        if(packetLen > MAX_PAYLOAD_LENGTH)
        {
            return COMMAND_INVALID;
        }

        TestCommandHandler_transmitSeries(packetLen, numPackets);
    }

    return COMMAND_OK;
}


//! \brief Transmit packet series
//!        A series is a number of packets with same packet length
//!
//! \param[in] packetLen
//!            Packet length
//!
//! \param[in] numPackets
//!            Number of packets
//!
//! \return None
void TestCommandHandler_transmitSeries(uint16_t packetLen, uint16_t numPackets)
{
    TestCommandHandle_initializePacketHeader(packet, packetLen);

    for(int i = 0; i < numPackets; i++)
    {
        RadioIF_sendPacket(packet, packetLen);
    }
}


//! \brief Initialize packet payload
//!        The payload is initialized as a 1 byte counter
//!
//! \param[in] packet
//!            Pointer to packet buffer
//!
//! \param[in] packetLength
//!            Length of the packet
//!
//! \return None
void TestCommandHandle_initializePacketPayload(uint8_t* packet, uint16_t packetLength)
{
    int headerLen = HEADER_LENGTH;
    if(PhyManager_getRfApi() == PROPRIETARY_15_4_G)
    {
        headerLen = HEADER_LENGTH_15_4_G;
    }
    
    for(int i = headerLen; i < packetLength; i++)
    {
        packet[i] = i;
    }
}


//! \brief Initialize packet header
//!        Set packet header for 802.15.4g frame format and whitenMode=7
//!
//! \param[in] packet
//!            Pointer to packet buffer
//!
//! \param[in] packetLength
//!            Length of the packet
//!
//! \return None
void TestCommandHandle_initializePacketHeader(uint8_t* packet, uint16_t packetLength)
{ 
    if(PhyManager_getRfApi() == PROPRIETARY_15_4_G)
    {
        // create the 15_4_g header (2 bytes used here)
        packet[0] = (packetLength-HEADER_LENGTH_15_4_G - (PACKET_LENGHT_OFFSET_15_4_G)) & 0xFF;
        packet[1] = ((packetLength-HEADER_LENGTH_15_4_G - (PACKET_LENGHT_OFFSET_15_4_G)) & 0xFF00) >> 8;

        // Set bit 11 in header to enable whitening for whitenMode=7
        packet[1] |= 1 << 3;
    }
}






