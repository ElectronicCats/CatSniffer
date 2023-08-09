/******************************************************************************
*  Filename:       phy_if_prop.h
*
*  Description:    Header file for PHY interface proprietary
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
#ifndef PHY_IF_PROP_H
#define PHY_IF_PROP_H

#include <stdint.h>

//! \brief Configure RF Core Setup command for the proprietary PHYs
//!
//! \param[in] phyIndex
//!            Index of the PHY setting in the table of proprietary PHY settings 
//!
//! \return None
void PhyIf_configureSetupCmdProp(uint8_t phyIndex);


//! \brief Configure RF Core RX command for the proprietary PHYs
//!
//! \param[in] phyIndex
//!            Index of the PHY setting in the table of proprietary PHY settings 
//!
//! \return None
void PhyIf_configureRxCmdProp(uint8_t phyIndex);


//! \brief Configure RF Core TX command for the proprietary PHYs
//!
//! \param[in] packet
//!            Pointer to packet
//!
//! \param[in] packetLength
//!            Length of the packet
//!
//! \param[in] phyIndex
//!            Index of the PHY setting in the table of proprietary PHY settings 
//!
//! \return None
void PhyIf_configureTxCmdProp(uint8_t* packet, uint16_t packetLength, uint8_t phyIndex);


//! \brief Set frequency value 
//!
//! \param[in] phyIndex
//!            Index of the PHY setting in the table of proprietary PHY settings 
//!
//! \param[in] frequency
//!            Frequency in MHz
//!
//! \param[in] fractFrequency
//!            Fractional frequency in 1 MHz/65536 fractions.
//!
//! \return None
void PhyIf_setFrequencyProp(uint8_t phyIndex, uint16_t frequency, uint16_t fractFrequency);

#endif

