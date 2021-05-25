/******************************************************************************
*  Filename:       phy_if_prop_15_4g.c
*
*  Description:    Source file for PHY interface proprietary
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

#include "phy_if_prop_15_4g.h"
#include "phy_tables.h"
#include "../radio_if_dataqueue.h"


void PhyIf_setFrequencyProp_15_4g(uint8_t phyIndex, uint16_t frequency, uint16_t fractFrequency)
{
// Proprietary 15.4g mode is not supported for CC26X0 and CMD_PROP_RADIO_DIV_SETUP is not
// defined for this device
#ifndef DeviceFamily_CC26X0
    rfc_CMD_PROP_RADIO_DIV_SETUP_t* pSetupCmd = Phy_phyTableProp_15_4_g[phyIndex].pSetupCmd;
    rfc_CMD_FS_t* pFsCmd = Phy_phyTableProp_15_4_g[phyIndex].pFsCmd;


    pSetupCmd->centerFreq = frequency;

    pFsCmd->frequency = frequency;
    pFsCmd->fractFreq = fractFrequency;
#endif
}


void PhyIf_configureSetupCmdProp_15_4g(uint8_t phyIndex)
{
    // No configuration required for the setup command in IEEE 802.15.4g mode
}


void PhyIf_configureRxCmdProp_15_4g(uint8_t phyIndex)
{
    rfc_CMD_PROP_RX_ADV_t* pPropRxAdvCmd = Phy_phyTableProp_15_4_g[phyIndex].pRxCmd;
  
    // Modify CMD_PROP_RX_ADV command
    pPropRxAdvCmd->pQueue = RadioIF_dataQueueGet();
    pPropRxAdvCmd->rxConf.bAutoFlushIgnored = 1;
    pPropRxAdvCmd->rxConf.bAutoFlushCrcErr = 0;
    pPropRxAdvCmd->maxPktLen = 2047;
    pPropRxAdvCmd->pktConf.bRepeatOk = 1;
    pPropRxAdvCmd->pktConf.bRepeatNok = 1;

    pPropRxAdvCmd->rxConf.bIncludeHdr = 0x1;
    pPropRxAdvCmd->rxConf.bAppendRssi = 0x1;
    pPropRxAdvCmd->rxConf.bAppendTimestamp = 0x1;
    pPropRxAdvCmd->rxConf.bAppendStatus = 0x1;

    // Append CRC bytes
    pPropRxAdvCmd->rxConf.bIncludeCrc = 0x1;
}


void PhyIf_configureTxCmdProp_15_4g(uint8_t* packet, uint16_t packetLength, uint8_t phyIndex)
{
    rfc_CMD_PROP_TX_ADV_t* pPropTxAdvCmd = Phy_phyTableProp_15_4_g[phyIndex].pTxCmd;
  
    // TRIG_ABSTIME with current time is used instaed of TRIG_NOW because of a
    // bug in RF Core with the combination of 15.4g and TRIG_NOW (?)

    pPropTxAdvCmd->pktLen = packetLength;
    pPropTxAdvCmd->pPkt = packet;
    pPropTxAdvCmd->startTrigger.triggerType = TRIG_ABSTIME;
    pPropTxAdvCmd->startTrigger.pastTrig = 1;
    pPropTxAdvCmd->startTime = RF_getCurrentTime();
}





