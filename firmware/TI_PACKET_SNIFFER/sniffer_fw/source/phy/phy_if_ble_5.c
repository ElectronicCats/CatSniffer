/******************************************************************************
*  Filename:       phy_if_ble_5.c
*
*  Description:    Source file for PHY interface BLE 5
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

#include "phy_if_ble_5.h"
#include "phy_tables.h"
#include "../radio_if_dataqueue.h"

#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0)
static int8_t PhyIf_convertFrequencyToBleChannel(uint16_t frequency, uint16_t fractFrequency);
static uint8_t convertToBleChannel(uint8_t channel);
#endif

void PhyIf_setFrequencyBle_5(uint8_t phyIndex, uint16_t frequency, uint16_t fractFrequency)
{
// BLE 5 mode is not supported for CC13X0/CC26X0 and rfc_CMD_BLE5_GENERIC_RX_t is not
// defined for these devices
#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0)
    
    rfc_CMD_FS_t* pFsCmd = Phy_phyTableBle_5[phyIndex].pFsCmd;
    rfc_CMD_BLE5_GENERIC_RX_t* pBle5RxCmd = Phy_phyTableBle_5[phyIndex].pRxCmd;
    
    pFsCmd->frequency = frequency;
    pFsCmd->fractFreq = fractFrequency;
    
    int8_t bleChannel = PhyIf_convertFrequencyToBleChannel(frequency, fractFrequency);
    if(bleChannel != -1)
    {
        // Set channel as standard BLE channel
        PhyIf_setChannelBle_5(phyIndex, bleChannel);
    }
    else
    {
        // Set custom frequency
        pBle5RxCmd->channel = frequency - 2300;
    }
#endif
}


void PhyIf_configureSetupCmdBle_5(uint8_t phyIndex)
{
    // No configuration required for the setup command in BLE 5 mode
}


void PhyIf_configureRxCmdBle_5(uint8_t phyIndex)
{
// BLE 5 mode is not supported for CC13X0/CC26X0 and rfc_CMD_BLE5_GENERIC_RX_t is not
// defined for these devices
#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0) 
    rfc_CMD_BLE5_GENERIC_RX_t* pBle5RxCmd = Phy_phyTableBle_5[phyIndex].pRxCmd;
    
    // Modify CMD_BLE5_GENERIC_RX command
    pBle5RxCmd->pParams->pRxQ = RadioIF_dataQueueGet();
         
    pBle5RxCmd->pParams->rxConfig.bAutoFlushIgnored = 0;
    pBle5RxCmd->pParams->rxConfig.bAutoFlushCrcErr = 0;
    pBle5RxCmd->pParams->rxConfig.bAutoFlushEmpty = 0;
    pBle5RxCmd->pParams->rxConfig.bIncludeLenByte = 1;
    pBle5RxCmd->pParams->rxConfig.bIncludeCrc = 1;
    pBle5RxCmd->pParams->rxConfig.bAppendRssi = 1;
    pBle5RxCmd->pParams->rxConfig.bAppendStatus = 1;
    pBle5RxCmd->pParams->rxConfig.bAppendTimestamp = 1;
#endif
}


void PhyIf_configureTxCmdBle_5(uint8_t* packet, uint16_t packetLength, uint8_t phyIndex)
{
    // Not yet implemented
}


//! \brief Convert frequency value to BLE channel
//!
//! \param[in] frequency
//!            Frequency in MHz
//!
//! \param[in] fractFrequency
//!            Fractional frequency in 1 MHz/65536 fractions.
//!
//! \return BLE channel (0-39), or -1 if the input does not correspond to a BLE channel
#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0) 
int8_t PhyIf_convertFrequencyToBleChannel(uint16_t frequency, uint16_t fractFrequency)
{
    int8_t bleChannel = -1;
    
    if(fractFrequency != 0)
    {
        // If fractional frequency is not 0, this is not a BLE channel
        return -1;
    }
    
    if (frequency == 2402)  bleChannel = 37;
    else if (frequency == 2426) bleChannel = 38;
    else if (frequency == 2480) bleChannel = 39;
    else if( (frequency >= 2404) && (frequency <= 2424) )
    {
        bleChannel = (frequency - 2404)/2; 
    }
    else if(frequency >= 2428)
    {
        bleChannel = (frequency - 2406)/2; 
    }

    return bleChannel;
}
#endif


uint32_t PhyIf_getAccessAddressBle_5(uint8_t phyIndex)
{
// BLE 5 mode is not supported for CC13X0/CC26X0 and rfc_CMD_BLE5_GENERIC_RX_t is not
// defined for these devices
#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0) 
    rfc_CMD_BLE5_GENERIC_RX_t* pBle5RxCmd = Phy_phyTableBle_5[phyIndex].pRxCmd;
    
    return pBle5RxCmd->pParams->accessAddress;
#else
    return 0;
#endif
}


void PhyIf_setAccessAddressBle_5(uint8_t phyIndex, uint32_t accessAddress)
{
// BLE 5 mode is not supported for CC13X0/CC26X0 and rfc_CMD_BLE5_GENERIC_RX_t is not
// defined for these devices
#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0) 
    rfc_CMD_BLE5_GENERIC_RX_t* pBle5RxCmd = Phy_phyTableBle_5[phyIndex].pRxCmd;
    
    if(pBle5RxCmd != NULL)
    {
        pBle5RxCmd->pParams->accessAddress = accessAddress;
    }
#endif
}


void PhyIf_setCrcInitValueBle_5(uint8_t phyIndex, uint32_t crcInit)
{
// BLE 5 mode is not supported for CC13X0/CC26X0 and rfc_CMD_BLE5_GENERIC_RX_t is not
// defined for these devices
#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0) 
    rfc_CMD_BLE5_GENERIC_RX_t* pBle5RxCmd = Phy_phyTableBle_5[phyIndex].pRxCmd;
    pBle5RxCmd->pParams->crcInit0 = crcInit & 0xFF;
    pBle5RxCmd->pParams->crcInit1 = (crcInit & 0xFF00) >> 8;
    pBle5RxCmd->pParams->crcInit2 = (crcInit & 0xFF0000) >> 16;
#endif
}


void PhyIf_setChannelBle_5(uint8_t phyIndex, uint8_t channel)
{
// BLE 5 mode is not supported for CC13X0/CC26X0 and rfc_CMD_BLE5_GENERIC_RX_t is not
// defined for these devices
#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0) 
    rfc_CMD_BLE5_GENERIC_RX_t* pBle5RxCmd = Phy_phyTableBle_5[phyIndex].pRxCmd;
    
    // Convert to BLE channel
    channel = convertToBleChannel(channel);
    pBle5RxCmd->channel = channel;
   
    // If channel is valid BLE channel use default whitening 
    if(channel <= 39)
    {
        pBle5RxCmd->whitening.bOverride = 0;
    }
    else
    {
        pBle5RxCmd->whitening.init = 0x80 | 0x40 | channel;
        pBle5RxCmd->whitening.bOverride = 0;
    }
#endif
}



uint8_t PhyIf_getChannelBle_5(uint8_t phyIndex)
{
// BLE 5 mode is not supported for CC13X0/CC26X0 and rfc_CMD_BLE5_GENERIC_RX_t is not
// defined for these devices
#if !defined(DeviceFamily_CC26X0) && !defined(DeviceFamily_CC13X0) 
    rfc_CMD_BLE5_GENERIC_RX_t* pBle5RxCmd = Phy_phyTableBle_5[phyIndex].pRxCmd;
    
    return pBle5RxCmd->channel;
#else
    return 0;
#endif
}


//! \brief If channel setting corresponds to a BLE channel, convert it to BLE channel
//!        in the range 0 - 39
//!
//! \param[in] channel
//!            BLE channel (0-39) or custom frequency (60-207).
//! 
//! \return If valid BLE channel return BLE channel in range 0-39, otherwise return 
//!         channel as given in input. 
uint8_t convertToBleChannel(uint8_t channel)
{
    // Channel can either be specified as BLE channel 0-39 or custom frequency:
    // Frequency (in MHz) = Custom frequency + 2300
    
    if(channel <= 39) 
    {
        // Channel is already specified as BLE channel
        return channel;
    }
    // Custom frequency is specified. 
    else if(channel == 102)
    {
        return 37; // Frequency 2402 MHz
    }
    else if(channel == 126)
    {
        return 38; // Frequency 2426 MHz
    }
    else if(channel == 180)
    {
        return 39; // Frequency 2480 MHz
    }
    else if(channel >= 104 && channel <= 124)
    {
        // BLE data channel in the range 0-10
        return (channel - 104) / 2;
    }
    else if (channel >= 128 && channel <= 178)
    {
        // BLE data channel in the range 11-36
        return 11 + ((channel - 128)/2);
    }
    
    // Not a valid BLE channel, return channel unmodified
    return channel;
}


