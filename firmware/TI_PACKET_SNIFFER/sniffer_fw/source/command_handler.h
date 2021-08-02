/******************************************************************************
*  Filename:       command_handler.h
*
*  Description:    Header file for the command handler. This module handles
*                  commands and command responses on the host interface.
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
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "command_packet.h"

// Status values
#define COMMAND_OK                      0
#define COMMAND_TIMED_OUT               1
#define COMMAND_FCS_FAILED              2
#define COMMAND_INVALID                 3
#define COMMAND_INVALID_STATE           4
#define COMMAND_INVALID_PARAMETER       5

//! \brief Get command from host interface.
//!
//!        This function will block until either:
//!        - A complete command is successfully received, or
//!        - A complete command or parts of it is received but there are errors
//!         in the command.
//!
//! \param[in] cmdPacket
//!            Pointer to buffer large enough for the received command packet.
//!            The buffer must be allocated by the caller.
//!
//! \return \li \c COMMAND_OK - successful reception of a command.
//!         \li \c COMMAND_TIMED_OUT - Parts of the command was received but not the
//!         complete command.
//!         \li \c COMMAND_INVALID - The command is invalid.
extern uint8_t CommandHandler_getCommand(CommandPacket_Obj* cmdPacket);


//! \brief Send command response on host interface.
//!
//! \param[in] status - Status of the received host command.
extern void CommandHandler_respondCommand(uint8_t status);


//! \brief Send command response for CMD_PING on host interface.
//!
//! \param[in] status - Status of the received ping command.
extern void CommandHandler_respondPingCommand(uint8_t status);

#endif

