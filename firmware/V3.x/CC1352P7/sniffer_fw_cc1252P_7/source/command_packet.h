/******************************************************************************
*  Filename:       command_packet.h
*
*  Description:    Defines for the command packet format.
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
#ifndef COMMAND_PACKET_H
#define COMMAND_PACKET_H

#include "general_packet.h"

// Command packet format
#define MAX_COMMAND_PAYLOAD_LENGTH                                      255
#define FCS_LENGTH                                                      1
#define COMMAND_RESPONSE_PAYLOAD_LENGTH                                 1
#define COMMAND_PING_RESPONSE_PAYLOAD_LENGTH                            7

// Command packet types
#define PACKET_TYPE_COMMAND_PING                                        0x40
#define PACKET_TYPE_COMMAND_START                                       0x41
#define PACKET_TYPE_COMMAND_STOP                                        0x42
#define PACKET_TYPE_COMMAND_PAUSE                                       0x43
#define PACKET_TYPE_COMMAND_RESUME                                      0x44
#define PACKET_TYPE_COMMAND_CFG_FREQUENCY                               0x45
#define PACKET_TYPE_COMMAND_CFG_PHY                                     0x47
#define PACKET_TYPE_COMMAND_CFG_WBMS_CHANNEL_TABLE                      0x50
#define PACKET_TYPE_COMMAND_CFG_BLE_INITIATOR_ADDR                      0x70

// Test command packet types
#define PACKET_TYPE_TEST_COMMAND_TRANSMIT_SEQUENCE                      0x60

// Command response packet types
#define PACKET_TYPE_COMMAND_RESPONSE                                    0x80

// Command packet payload fields
#define COMMAND_CFG_FREQUENCY_DATA_LENGTH                               4
#define COMMAND_CFG_FREQUENCY_FREQ_OFFSET                               0       // Offset of frequency field in command data
#define COMMAND_CFG_FREQUENCY_FRACT_FREQ_OFFSET                         2       // Offset of fractional frequency field in command data
#define COMMAND_CFG_PHY_DATA_LENGTH                                     1
#define COMMAND_CFG_PHY_DATA_OFFSET                                     0       // Offset of PHY number field in command data
#define COMMAND_CFG_BLE_INITIATOR_ADDR_LENGTH                           6
#define COMMAND_CFG_BLE_INITIATOR_ADDR_OFFSET                           0       // Offset of initiator address in command data
#define COMMAND_CFG_WBMS_CHANNEL_TABLE_LENGTH_FIELD_LENGHT              1
#define COMMAND_CFG_WBMS_CHANNEL_TABLE_LENGTH_FIELD_OFFSET              0
#define COMMAND_CFG_WBMS_CHANNEL_TABLE_FIELD_LENGHT                     37
#define COMMAND_CFG_WBMS_CHANNEL_TABLE_FIELD_OFFSET                     1

// Test Command packet payload fields
#define TEST_COMMAND_TRANSMIT_SEQUENCE_REPETITIONS_SIZE                 2
#define TEST_COMMAND_TRANSMIT_SEQUENCE_REPETITIONS_OFFSET               0       // offset from start of command data
#define TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_LENGTH_SIZE               2
#define TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_LENGTH_OFFSET             0       // Offset from start of serie
#define TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_NUMBER_SIZE               2
#define TEST_COMMAND_TRANSMIT_SEQUENCE_PACKET_NUMBER_OFFSET             2       // Offset from start of serie


// Command Response Status values
#define STATUS_OK                                                       0
#define STATUS_TIMEOUT                                                  1
#define STATUS_FCS_FAILED                                               2

// Command packet footer
#pragma pack(1)
typedef struct CommandPacketFooter_Obj
{
  uint8_t fcs;
  GeneralPacketFooter_Obj genPacketFooter;
} CommandPacketFooter_Obj;

// Command packet
#pragma pack(1)
typedef struct CommandPacket_Obj
{
    GeneralPacketHeader_Obj genPacketHdr;
    uint8_t payload[MAX_COMMAND_PAYLOAD_LENGTH + sizeof(CommandPacketFooter_Obj)];     // space for payload and general packet footer
} CommandPacket_Obj;

// Command response packet
#pragma pack(1)
typedef struct CommandResponsePacket_Obj
{
    GeneralPacketHeader_Obj genPacketHdr;
    uint8_t status;
    CommandPacketFooter_Obj cmdPacketFtr;
} CommandResponsePacket_Obj;


// Command response packet
#pragma pack(1)
typedef struct CommandPingResponsePacket_Obj
{
    GeneralPacketHeader_Obj genPacketHdr;
    uint8_t status;
    uint16_t chipId;
    uint8_t chipRev;
    uint8_t fwId;
    uint16_t fwRev;
    CommandPacketFooter_Obj cmdPacketFtr;
} CommandPingResponsePacket_Obj;



#endif

