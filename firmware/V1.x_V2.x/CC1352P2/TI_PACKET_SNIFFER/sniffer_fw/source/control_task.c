/******************************************************************************
*  Filename:       control_task.c
*
*  Description:    Source file for the Control Task. This task controls the state
*                  of application.
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
#include "host_if.h"
#include "radio_if.h"
#include "ble/ll_ble.h"
#include "control_task.h"
#include "command_packet.h"
#include "command_handler.h"
#include "test_command_handler.h"
#include "timestamp.h"
#include "task_event.h"

#include <stdbool.h>

// Sniffer control state. WAIT_FOR_COMMAND is initial state.
static SnifferState ControlTask_state = STATE_WAIT_FOR_COMMAND;

// Command handling related
static CommandPacket_Obj ControlTask_command;
static uint8_t ControlTask_handleCommand(CommandPacket_Obj* command);
static uint8_t ControlTask_handleCommandStart(void);
static uint8_t ControlTask_handleCommandStop(void);
static uint8_t ControlTask_handleCommandCfgFrequency(uint8_t* commandData, uint16_t length);
static uint8_t ControlTask_handleCommandCfgPhy(uint8_t* commandData, uint16_t length);
static uint8_t ControlTask_handleCommandCfgBleInitiatorAddr(uint8_t* commandData, uint16_t length);
static bool ControlTask_isCmdPing(CommandPacket_Obj* cmdPacket);

// This function is not static to make it accessible from test task
void ControlTask_setControlState(SnifferState state);

// Other local functions
static void ControlTask_notifyUserIfTask(void);


//! \brief Control task function
//!        The arguments are not used.
//!
//!        This task controls the state of the application. It waits for
//!        commands from host. At reception of a host command it will handle
//!        the command and change state accordingly. This task is blocked in the
//!        function CommandHandler_getCommand until a host command is received.
//!        After the first command from the host interface is received, it will
//!        notify the user interface task by sending the event EVENT_ID_USER_IF_TASK_END
//!        to it. This event causes the user interface task to exit itself.
//!
void controlTask(UArg a0, UArg a1)
{
    // Initialize the modules to be used by this task
    HostIF_init();
    RadioIF_init();
    TaskEvent_init();

    while (true)
    {
        // Wait for command from host
        uint8_t status = CommandHandler_getCommand(&ControlTask_command);
        if(status == COMMAND_OK)
        {
            // After first command is received successfully, state is changed
            // to INIT and User Interface task is notified.
            if(ControlTask_state == STATE_WAIT_FOR_COMMAND)
            {
                ControlTask_notifyUserIfTask();
                ControlTask_state = STATE_INIT;
            }

            status = ControlTask_handleCommand(&ControlTask_command);
        }
        // Command is completed, respond to host
        if(ControlTask_isCmdPing(&ControlTask_command))
        {
            CommandHandler_respondPingCommand(status);
        }
        else
        {
            CommandHandler_respondCommand(status);
        }
    }
}


//! \brief handle a host command
//!
//! \param[in] cmdPacket
//!            Pointer to the received command packet
//!
//! \return status that indicates successful or failed handling of the command.
//!         COMMAND_OK - The command was handled successfully.
//!         COMMAND_INVALID - The command is invalid (e.g. payload does not match
//!         the command type.)
//!         COMMAND_INVALID_STATE: The received command is not valid for the
//!         current state of the sniffer.
uint8_t ControlTask_handleCommand(CommandPacket_Obj* command)
{
    uint8_t status = COMMAND_OK;

    switch(command->genPacketHdr.packetType)
    {
        case PACKET_TYPE_COMMAND_START:
            if(ControlTask_state != STATE_STARTED)
            {
                status = ControlTask_handleCommandStart();
                ControlTask_state = STATE_STARTED;
            }
            break;
        case PACKET_TYPE_COMMAND_STOP:
            if(ControlTask_state != STATE_STOPPED)
            {
                status = ControlTask_handleCommandStop();
                ControlTask_state = STATE_STOPPED;
            }
            break;
        case PACKET_TYPE_COMMAND_CFG_FREQUENCY:
            status = ControlTask_handleCommandCfgFrequency((uint8_t*)&command->payload, command->genPacketHdr.packetLen);
            break;
        case PACKET_TYPE_COMMAND_CFG_PHY:
            status = ControlTask_handleCommandCfgPhy((uint8_t*)&command->payload, command->genPacketHdr.packetLen);
            break;
        case PACKET_TYPE_COMMAND_CGF_BLE_INITIATOR_ADDR:
            status = ControlTask_handleCommandCfgBleInitiatorAddr((uint8_t*)&command->payload, command->genPacketHdr.packetLen);
            break;
        case PACKET_TYPE_COMMAND_PING:
            status = COMMAND_OK;
            break;
        case PACKET_TYPE_TEST_COMMAND_TRANSMIT_SEQUENCE:
            if(ControlTask_state != STATE_STARTED)
            {
                status = TestCommandHandler_transmitSequence((uint8_t*)&command->payload, command->genPacketHdr.packetLen);
            }
            break;
        default:
            status = COMMAND_INVALID;
            break;
    }

    return status;
}


//! \brief handle CMD_START
//!
//! \return status that indicates successful or failed handling of the command.
uint8_t ControlTask_handleCommandStart(void)
{
    RadioIF_startRx();

    return COMMAND_OK;
}


//! \brief handle CMD_STOP
//!
//! \return status that indicates successful or failed handling of the command.
uint8_t ControlTask_handleCommandStop(void)
{
    RadioIF_stopRx();

    // Reset timestamp module
    Timestamp_init();

    return COMMAND_OK;
}


//! \brief handle CMD_CFG_FREQUENCY
//!
//! \param[in] commandData
//!            payload data for the CMD_CFG_FREQUENCY command.
//!
//! \param[in] length
//!            length of payload data
//!
//! \return status that indicates successful or failed handling of the command.
//!         COMMAND_OK - The command was handled successfully.
//!         COMMAND_INVALID - The command is invalid (e.g. payload does not match
//!         the command type.)
//!         COMMAND_INVALID_STATE: The received command is not valid for the
//!         current state of the sniffer.
uint8_t ControlTask_handleCommandCfgFrequency(uint8_t* commandData, uint16_t length)
{
    if(length != COMMAND_CFG_FREQUENCY_DATA_LENGTH)
    {
        // Length of data field is invalid for this command
        // The command is ignored
        return COMMAND_INVALID;
    }

    if( (ControlTask_state != STATE_STOPPED) && (ControlTask_state != STATE_INIT) )
    {
        // The command is only valid in STOPPED and INIT state
        // The command is ignored in other states.
        return COMMAND_INVALID_STATE;
    }

    uint16_t* frequency = (uint16_t*)(commandData + COMMAND_CFG_FREQUENCY_FREQ_OFFSET);
    uint16_t* fractFrequency = (uint16_t*)(commandData + COMMAND_CFG_FREQUENCY_FRACT_FREQ_OFFSET);
    RadioIF_setFrequency(*frequency, *fractFrequency);

    return COMMAND_OK;
}


//! \brief handle CMD_CFG_PHY
//!
//! \param[in] commandData
//!            payload data for the CMD_CFG_PHY command.
//!
//! \param[in] length
//!            length of payload data (a length of 1 byte is expected)
//!
//! \return status that indicates successful or failed handling of the command.
//!         COMMAND_OK - The command was handled successfully.
//!         COMMAND_INVALID - The command is invalid (e.g. payload does not match
//!         the command type.)
//!         COMMAND_INVALID_STATE: The received command is not valid for the
//!         current state of the sniffer.
uint8_t ControlTask_handleCommandCfgPhy(uint8_t* commandData, uint16_t length)
{
    if(length != COMMAND_CFG_PHY_DATA_LENGTH)
    {
        // Length of data field is invalid for this command
        // The command is ignored
        return COMMAND_INVALID;
    }
    
    if( (ControlTask_state != STATE_STOPPED) && (ControlTask_state != STATE_INIT) )
    {
        // The command is only valid in STOPPED and INIT state
        // The command is ignored in other states.
        return COMMAND_INVALID_STATE;
    }
    
    uint8_t phyNumber = *(commandData + COMMAND_CFG_PHY_DATA_OFFSET);
    
    uint8_t status = COMMAND_OK;
    if(!RadioIf_setPhy(phyNumber))
    {   
        status = COMMAND_INVALID_PARAMETER;
    }
       
    return status;
}


//! \brief handle CMD_CFG_BLE_INITIATOR_ADDR
//!
//! \param[in] commandData
//!            payload data for the CMD_CFG_BLE_INITIATOR_ADDR command.
//!
//! \param[in] length
//!            length of payload data (a length of 6 bytes is expected)
//!
//! \return status that indicates successful or failed handling of the command.
//!         COMMAND_OK - The command was handled successfully.
//!         COMMAND_INVALID - The command is invalid (e.g. payload does not match
//!         the command type.)
//!         COMMAND_INVALID_STATE: The received command is not valid for the
//!         current state of the sniffer.
uint8_t ControlTask_handleCommandCfgBleInitiatorAddr(uint8_t* commandData, uint16_t length)
{
    if(length != COMMAND_CFG_BLE_INITIATOR_ADDR_LENGTH)
    {
        // Length of data field is invalid for this command
        // The command is ignored
        return COMMAND_INVALID;
    }
    
    if( (ControlTask_state != STATE_STOPPED) && (ControlTask_state != STATE_INIT) )
    {
        // The command is only valid in STOPPED and INIT state
        // The command is ignored in other states.
        return COMMAND_INVALID_STATE;
    }
    
    uint8_t* initiatorAddr = (commandData + COMMAND_CFG_BLE_INITIATOR_ADDR_OFFSET);
    
    LLBle_setInitiatorAddressFilterValue(initiatorAddr);
       
    return COMMAND_OK;
}


//! \brief Notify user interface task.
//!
//!       Upon reception of this notification the user interface shall exit
//!       itself.
void ControlTask_notifyUserIfTask(void)
{
    Event_post(TaskEvent_Handle, EVENT_ID_USER_IF_TASK_END);
}


//! \brief Set application control state
//!
//!        This function is used from test FW to set control state of the
//!        application, for example to check that the correct command responses
//!        are given dependent on control state.
void ControlTask_setControlState(SnifferState controlState)
{
    ControlTask_state = controlState;
}


//! \brief Check if command is CMD_PING
bool ControlTask_isCmdPing(CommandPacket_Obj* cmdPacket)
{
    return (cmdPacket->genPacketHdr.packetType == PACKET_TYPE_COMMAND_PING);
}


