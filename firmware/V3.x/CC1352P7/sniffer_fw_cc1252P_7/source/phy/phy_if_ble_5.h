/******************************************************************************
*  Filename:       phy_if_ble_5.h
*
*  Description:    Header file for PHY interface BLE 5 mode
*
* Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
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
#ifndef PHY_IF_BLE_5_H
#define PHY_IF_BLE_5_H

#include <stdint.h>

//! \brief Configure RF Core Setup command for the BLE 5 PHYs
//!
//! \param[in] phyIndex
//!            Index of the PHY setting in the table of BLE 5 PHY settings 
//!
//! \return None
void PhyIf_configureSetupCmdBle_5(uint8_t phyIndex);


//! \brief Configure RF Core RX command for the BLE 5  PHYs
//!
//! \param[in] phyIndex
//!            Index of the PHY setting in the table of BLE 5 PHY settings 
//!
//! \return None
void PhyIf_configureRxCmdBle_5(uint8_t phyIndex);


//! \brief Configure RF Core TX command for the BLE 5 PHYs
//!
//! \param[in] packet
//!            Pointer to packet
//!
//! \param[in] packetLength
//!            Length of the packet
//!
//! \param[in] phyIndex
//!            Index of the PHY setting in the table of BLE 5 PHY settings 
//!
//! \return None
void PhyIf_configureTxCmdBle_5(uint8_t* packet, uint16_t packetLength, uint8_t phyIndex);


//! \brief Set frequency value 
//!
//! \param[in] phyIndex
//!            Index of the PHY setting in the table of BLE 5 PHY settings 
//!
//! \param[in] frequency
//!            Frequency in MHz
//!
//! \param[in] fractFrequency
//!            Fractional frequency in 1 MHz/65536 fractions.
//!
//! \return None
void PhyIf_setFrequencyBle_5(uint8_t phyIndex, uint16_t frequency, uint16_t fractFrequency);


//! \brief Get currently used access address value 
//!
//!
//! \return 32 bits Access Address value
uint32_t PhyIf_getAccessAddressBle_5(uint8_t phyIndex);


void PhyIf_setAccessAddressBle_5(uint8_t phyIndex, uint32_t accessAddress);

void PhyIf_setCrcInitValueBle_5(uint8_t phyIndex, uint32_t crcInit);

void PhyIf_setChannelBle_5(uint8_t phyIndex, uint8_t channel);

uint8_t PhyIf_getChannelBle_5(uint8_t phyIndex);

#endif

