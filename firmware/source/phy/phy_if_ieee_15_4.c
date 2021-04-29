/******************************************************************************
*  Filename:       phy_if_ieee_15_4.c
*
*  Description:    Source file for PHY interface IEEE 802.15.4
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

#include "phy_if_ieee_15_4.h"
#include "phy_tables.h"
#include "../radio_if_dataqueue.h"


void PhyIf_setFrequencyIeee_15_4(uint8_t phyIndex, uint16_t frequency, uint16_t fractFrequency)
{
    rfc_CMD_FS_t* pFsCmd = Phy_phyTableIeee_15_4[phyIndex].pFsCmd;
    
    pFsCmd->frequency = frequency;
    pFsCmd->fractFreq = fractFrequency;
}


void PhyIf_configureSetupCmdIeee_15_4(uint8_t phyIndex)
{
    // No configuration required for the setup command in IEEE 802.15.4 mode
}


void PhyIf_configureRxCmdIeee_15_4(uint8_t phyIndex)
{
// IEEE 15.4 mode is not supported for CC13X0 and rfc_CMD_IEEE_RX_t is not
// defined for this device
#ifndef DeviceFamily_CC13X0 
    rfc_CMD_IEEE_RX_t* pIeeeRxCmd = Phy_phyTableIeee_15_4[phyIndex].pRxCmd;
  
    // Modify CMD_IEEE_RX command
    pIeeeRxCmd->pRxQ = RadioIF_dataQueueGet();
    pIeeeRxCmd->rxConfig.bAutoFlushCrc = 0;
    pIeeeRxCmd->rxConfig.bAutoFlushIgn = 0;
    pIeeeRxCmd->rxConfig.bIncludePhyHdr = 1;
    pIeeeRxCmd->rxConfig.bIncludeCrc = 1;
    pIeeeRxCmd->rxConfig.bAppendRssi = 1;
    pIeeeRxCmd->rxConfig.bAppendCorrCrc = 1;
    pIeeeRxCmd->rxConfig.bAppendSrcInd = 0;
    pIeeeRxCmd->rxConfig.bAppendTimestamp = 1;
#endif
}


void PhyIf_configureTxCmdIeee_15_4(uint8_t* packet, uint16_t packetLength, uint8_t phyIndex)
{
// IEEE 15.4 mode is not supported for CC13X0 and rfc_CMD_IEEE_TX_t is not
// defined for this device
#ifndef DeviceFamily_CC13X0
    rfc_CMD_IEEE_TX_t* pIeeeTxCmd = Phy_phyTableIeee_15_4[phyIndex].pTxCmd;
  
    pIeeeTxCmd->payloadLen = packetLength;
    pIeeeTxCmd->pPayload = packet;
    pIeeeTxCmd->startTrigger.triggerType = TRIG_NOW;
#endif
}





