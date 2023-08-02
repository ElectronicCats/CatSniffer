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

#include "phy_if_wbms.h"
#include "phy_tables.h"
#include "../radio_if_dataqueue.h"


void PhyIf_setFrequencyWbms(uint8_t phyIndex, uint16_t frequency, uint16_t fractFrequency)
{    
    rfc_CMD_FS_t* pFsCmd = Phy_phyTableWbms[phyIndex].pFsCmd;
    
    pFsCmd->frequency = frequency;
    pFsCmd->fractFreq = fractFrequency;
}


void PhyIf_configureSetupCmdWbms(uint8_t phyIndex)
{
    // No configuration done for Setup command 
}


void PhyIf_configureRxCmdWbms(uint8_t phyIndex)
{
    rfc_CMD_PROP_RX_ADV_t* pWbmsRxCmd = Phy_phyTableWbms[phyIndex].pRxCmd;
  
    // Modify CMD_PROP_RX command
    pWbmsRxCmd->pQueue = RadioIF_dataQueueGet();
    pWbmsRxCmd->rxConf.bAutoFlushIgnored = 1;
    pWbmsRxCmd->rxConf.bAutoFlushCrcErr = 0;
    pWbmsRxCmd->maxPktLen = 0xFF;
    pWbmsRxCmd->pktConf.bRepeatOk = 1;
    pWbmsRxCmd->pktConf.bRepeatNok = 1;
    
    pWbmsRxCmd->pktConf.bUseCrc = 1;
    pWbmsRxCmd->pktConf.bCrcIncHdr = 1;
    
    pWbmsRxCmd->lenOffset = -3;

    // Append 1 status byte, 1 RSSI byte and 4 bytes timestamp
    pWbmsRxCmd->rxConf.bAppendStatus = 1;
    pWbmsRxCmd->rxConf.bAppendRssi = 1;
    pWbmsRxCmd->rxConf.bAppendTimestamp = 1;

    // Append CRC bytes and header
    pWbmsRxCmd->rxConf.bIncludeCrc = 0x1;
    pWbmsRxCmd->rxConf.bIncludeHdr = 0x1;
}


void PhyIf_configureTxCmdWbms(uint8_t* packet, uint16_t packetLength, uint8_t phyIndex)
{       
    rfc_CMD_PROP_TX_ADV_t* pWbmsTxCmd = Phy_phyTableWbms[phyIndex].pTxCmd;
  
    pWbmsTxCmd->pktLen = packetLength;
    pWbmsTxCmd->pPkt = packet;
    pWbmsTxCmd->startTrigger.triggerType = TRIG_NOW;
}


void PhyIf_setChannelWbms(uint8_t phyIndex, uint8_t channel)
{
    uint16_t frequency = 0;
    bool bChannelValid = true;
    
    if(channel == 37)
    {
        frequency = 2402;
    }
    else if(channel == 38)
    {
        frequency = 2426;
    }
    else if(channel == 39)
    {
        frequency = 2480;
    }
    else if(channel <= 10)
    {
        frequency = 2404 + (channel * 2);
    }
    else if((channel >= 11) && (channel <= 36))
    {
        frequency = 2406 + (channel * 2);
    }
    else 
    {
        // Channel is not valid
        bChannelValid = false;
    }
    
   /* if(channel > 39)
    {
        bChannelValid = false;
    }
    // Todo, check if this is correct. Does WBMS not use BLE channel assignments?
    frequency = 2402 + channel * 2;
    */
    
    // Set frequency if channel is valid
    if(bChannelValid)
    {
        PhyIf_setFrequencyWbms(phyIndex, frequency, 0);
    }
    
    return;
}


/*uint8_t PhyIf_getChannelWbms(uint8_t phyIndex)
{
    
}*/








