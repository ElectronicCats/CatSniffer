/******************************************************************************
*  Filename:       phy_if_prop.c
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

#include "phy_if_prop.h"
#include "phy_tables.h"
#include "../radio_if_dataqueue.h"


void PhyIf_setFrequencyProp(uint8_t phyIndex, uint16_t frequency, uint16_t fractFrequency)
{
// Proprietary mode is not supported for CC26X0 and CMD_PROP_RADIO_DIV_SETUP is not
// defined for this device
#ifndef DeviceFamily_CC26X0  
    rfc_CMD_PROP_RADIO_DIV_SETUP_t* pSetupCmd = Phy_phyTableProp[phyIndex].pSetupCmd;
    rfc_CMD_FS_t* pFsCmd = Phy_phyTableProp[phyIndex].pFsCmd;
    
    pSetupCmd->centerFreq = frequency;

    pFsCmd->frequency = frequency;
    pFsCmd->fractFreq = fractFrequency;
#endif
}


void PhyIf_configureSetupCmdProp(uint8_t phyIndex)
{
    // No configuration done for Setup command 
}


void PhyIf_configureRxCmdProp(uint8_t phyIndex)
{
    rfc_CMD_PROP_RX_t* pPropRxCmd = Phy_phyTableProp[phyIndex].pRxCmd;
  
    // Modify CMD_PROP_RX command
    pPropRxCmd->pQueue = RadioIF_dataQueueGet();
    pPropRxCmd->rxConf.bAutoFlushIgnored = 1;
    pPropRxCmd->rxConf.bAutoFlushCrcErr = 0;
    pPropRxCmd->maxPktLen = 0x80;
    pPropRxCmd->pktConf.bRepeatOk = 1;
    pPropRxCmd->pktConf.bRepeatNok = 1;

    // Append 1 status byte, 1 RSSI byte and 4 bytes timestamp
    pPropRxCmd->rxConf.bAppendStatus = 1;
    pPropRxCmd->rxConf.bAppendRssi = 1;
    pPropRxCmd->rxConf.bAppendTimestamp = 1;

    // Append CRC bytes
    pPropRxCmd->rxConf.bIncludeCrc = 0x1;
}


void PhyIf_configureTxCmdProp(uint8_t* packet, uint16_t packetLength, uint8_t phyIndex)
{       
    rfc_CMD_PROP_TX_t* pPropTxCmd = Phy_phyTableProp[phyIndex].pTxCmd;
  
    pPropTxCmd->pktLen = packetLength;
    pPropTxCmd->pPkt = packet;
    pPropTxCmd->startTrigger.triggerType = TRIG_NOW;
}








