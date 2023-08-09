/******************************************************************************
*  Filename:       phy_manager.h
*
*  Description:    Header file for PHY manager functionality
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
#ifndef PHY_MANAGER_H
#define PHY_MANAGER_H

#include <stdint.h>
// TI-RTOS radio driver
#include <ti/drivers/rf/RF.h>

#include "phy_rf_api.h"


//! \brief Initialize PHY manager
//!
void PhyManager_init(void);


//! \brief Set PHY (RF API and PHY setting)
//!
//! \param[in] phyNumber
//!            The number of the PHY to use. This number decodes into an RF API and a PHY setting 
//!
//! \return true if phyNumber is valid, or false otherwise
extern bool PhyManager_setPhy(uint8_t phyNumber);



//! \brief Set frequency value for currently used PHY
//!
//! \param[in] frequency
//!            Frequency in MHz
//!
//! \param[in] fractFrequency
//!            Fractional frequency in 1 MHz/65536 fractions.
//!
//! \return None
extern void PhyManager_setFrequency(uint16_t frequency, uint16_t fractFrequency);


//! \brief Configure RF command used for TX
//!
//! \param[in] packet
//!            Pointer to packet
//!
//! \param[in] packetLength
//!            Length of the packet
//!
//! \return None
extern void PhyManager_configureTxCommand(uint8_t* packet, uint16_t packetLength);


//! \brief Get pointer to the currently used RF Core Setup Command (for example: CMD_PROP_RADIO_DIV_SETUP)
//!
//! \return Pointer to RF Core Setup command
extern RF_RadioSetup* PhyManager_getSetupCmd(void);


//! \brief Get pointer to the currently used RF Core FS Command (CMD_FS)
//!
//! \return Pointer to RF Core FS command
extern RF_Op* PhyManager_getFsCmd(void);


//! \brief Get pointer to the currently used RF Core RX Command (for example: CMD_PROP_RX)
//!
//! \return Pointer to RF Core RX command
extern RF_Op* PhyManager_getRxCmd(void);


//! \brief Get pointer to the currently used RF Core TX Command (for example: CMD_PROP_TX)
//!
//! \return Pointer to RF Core TX command
extern RF_Op* PhyManager_getTxCmd(void);


//! \brief Get pointer to the currently used RF mode object
//!
//! \return Pointer to RF mode object
extern RF_Mode* PhyManager_getRfMode(void);


//! \brief Get currently used RF API type
//!        (With RF API it is meant the RF Core API type, for example Proprietary, 
//!         proprietary in IEEE 802.15.4g mode, or IEEE 802.15.4)
//!
//! \return Pointer to RF mode object
extern Phy_RfApi PhyManager_getRfApi(void);


//! \brief Get currently used PHY index 
//!        (PHY index for the currently used RF API)
//!
//! \return Pointer to RF mode object
extern uint8_t PhyManager_getPhyIndex(void);


//! \brief Check if BLE phy is supported
//!
//! \return true if BLE phy is supported, false otherwise
extern bool PhyManager_isBlePhySupported();


#endif

