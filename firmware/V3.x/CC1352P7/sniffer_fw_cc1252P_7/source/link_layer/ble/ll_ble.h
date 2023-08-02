/******************************************************************************
*  Filename:       ll_ble.h
*
*  Description:    Header file for the BLE Link Layer
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
#ifndef LL_BLE_H
#define LL_BLE_H

#include <stdint.h>

//! \brief Process packet by Link Layer
//!
//! \param[in] packetData
//!            Pointer to packet data including data added by RF Core (Timestamp, status, RSSI etc.)
//!
//! \param[in] packetLength
//!            Length of the buffer holding packet data. The actual packet data might be shorter than this. 
//!
//! \return Length of PDU 
extern uint16_t LLBle_processPacket(uint8_t* packetData, uint16_t packetLength);


//! \brief Get Access Address
//!
//! \return Access Address 
extern uint32_t LLBle_getAccessAddress(void);


//! \brief Set initiator address filter value
//!
//! \param[in] initiatorAddr
//!            Pointer to buffer holding 6 bytes initiator address 
//!
//! \return none 
extern void LLBle_setInitiatorAddressFilterValue(uint8_t* initiatorAddr);


//! \brief Reset Link Layer
//!
//! \return none 
extern void LLBle_reset(void);


#endif

