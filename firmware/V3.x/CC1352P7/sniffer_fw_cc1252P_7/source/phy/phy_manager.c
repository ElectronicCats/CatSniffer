/******************************************************************************
*  Filename:       phy_manager.c
*
*  Description:    Source file PHY manager functionality.
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

#include "phy_manager.h"
#include "phy_tables.h"
#include "phy_io_config.h"
#include "phy_if_prop.h"
#include "phy_if_prop_15_4g.h"
#include "phy_if_ieee_15_4.h"
#include "phy_if_ble_5.h"
#include "phy_if_wbms.h"
#include "ll_manager.h"

// Local functions
static bool PhyManager_isPhyNumberValid(uint8_t phyNumber);
static void PhyManager_configureCommands(void);

// The currently used RF API (Proprietary, Proprietary in IEEE 802.15.4g mode, or IEEE 802.15.4)
// The default RF API is PROPRIETARY
static Phy_RfApi PhyManager_currentRfApi = PROPRIETARY;

// The currently used PHY index (the index of the current setting in the table of settings for the currently used RF API)
// The default selected PHY index is 0 (that is the first PHY setting in the PHY table)
static uint8_t PhyManager_currentPhyIndex = 0;

// Function pointer definitions for PHY interface functions
typedef void (*setFreqFunc)(uint8_t tableIndex, uint16_t frequency, uint16_t fractFrequency);
typedef void (*cfgSetupCmdFunc)(uint8_t phyIndex);
typedef void (*cfgRxCmdFunc)(uint8_t phyIndex);
typedef void (*cfgTxCmdFunc)(uint8_t* packet, uint16_t packetLength, uint8_t phyIndex);

// A struct object with PHY interface function pointers
typedef struct Phy_Api_Obj
{
    setFreqFunc setFrequency;
    cfgSetupCmdFunc configureSetupCmd;
    cfgRxCmdFunc configureRxCmd;
    cfgTxCmdFunc configureTxCmd;
} Phy_Api_Obj;


// Table of PHY interface functions. There is one array element for each PHY type, where each element contains function pointer to PHY interface functions
//
// Note!: The order of these elements must be in exactly the same order as the enums in the RfApi enum type, since
// the value of the RfApi is used as index in the array. 
Phy_Api_Obj phyApis[] =
{
    {PhyIf_setFrequencyProp, PhyIf_configureSetupCmdProp, PhyIf_configureRxCmdProp, PhyIf_configureTxCmdProp},                          // PHY interface functions for RF Proprietary PHYs
    {PhyIf_setFrequencyProp_15_4g, PhyIf_configureSetupCmdProp_15_4g, PhyIf_configureRxCmdProp_15_4g, PhyIf_configureTxCmdProp_15_4g},  // PHY interface functions for PHYs using RF Proprietary in IEEE 802.15.4g mode
    {PhyIf_setFrequencyIeee_15_4, PhyIf_configureSetupCmdIeee_15_4, PhyIf_configureRxCmdIeee_15_4, PhyIf_configureTxCmdIeee_15_4},      // PHY interface functions for PHYs using IEEE 802.15.4 mode 
    {PhyIf_setFrequencyBle_5, PhyIf_configureSetupCmdBle_5, PhyIf_configureRxCmdBle_5, PhyIf_configureTxCmdBle_5},                      // PHY interface functions for PHYs using BLE 5
    {PhyIf_setFrequencyWbms, PhyIf_configureSetupCmdWbms, PhyIf_configureRxCmdWbms, PhyIf_configureTxCmdWbms},                          // PHY interface functions for PHYs using WBMS
};


void PhyManager_init(void)
{
    Phy_IoPinInit();
}


bool PhyManager_setPhy(uint8_t phyNumber)
{
    // Check that PHY number is valid
    if(!PhyManager_isPhyNumberValid(phyNumber))
    {
        return false;
    }
    
    // Set currently used RfApi and Phy index
    PhyManager_currentRfApi = Phy_supportedPhys[phyNumber].rfApi;
    PhyManager_currentPhyIndex = Phy_supportedPhys[phyNumber].phyIndex;
    
    // Configure RF commands for currently selected PHY setting 
    PhyManager_configureCommands();
    
    // Configure IO pins for currently selected RF API
    Phy_configureIoPins(PhyManager_currentRfApi);
    
    // Set Link Layer for the currently selected RF API
    // Use BLE link layer type for the BLE_5_1M RF API, WBMS link layer for WBMS RF API and the Default Link Layer type for other RF APIs. 
    switch(PhyManager_currentRfApi)
    {
        case BLE_5_1M:
            LL_setLLType(LL_BLE);
            break;
        case WBMS:
            LL_setLLType(LL_WBMS);
            break;
        default:
            LL_setLLType(LL_DEFAULT);
            break;
    }
    
    return true;
}


void PhyManager_setFrequency(uint16_t frequency, uint16_t fractFrequency)
{
    phyApis[PhyManager_currentRfApi].setFrequency(PhyManager_currentPhyIndex, frequency, fractFrequency);
}


RF_RadioSetup* PhyManager_getSetupCmd(void)
{
    if(PhyManager_currentRfApi == PROPRIETARY)
    {
        return (RF_RadioSetup*)Phy_phyTableProp[PhyManager_currentPhyIndex].pSetupCmd;
    }
    else if(PhyManager_currentRfApi == PROPRIETARY_15_4_G)
    {
        return (RF_RadioSetup*)Phy_phyTableProp_15_4_g[PhyManager_currentPhyIndex].pSetupCmd;
    }
    else if(PhyManager_currentRfApi == IEEE_802_15_4)
    {
        return (RF_RadioSetup*)Phy_phyTableIeee_15_4[PhyManager_currentPhyIndex].pSetupCmd;
    }
    else if(PhyManager_currentRfApi == BLE_5_1M)
    {
        return (RF_RadioSetup*)Phy_phyTableBle_5[PhyManager_currentPhyIndex].pSetupCmd;
    }
    else if(PhyManager_currentRfApi == WBMS)
    {
        return (RF_RadioSetup*)Phy_phyTableWbms[PhyManager_currentPhyIndex].pSetupCmd;
    }
    
    return NULL;
}


RF_Op* PhyManager_getFsCmd(void)
{
    if(PhyManager_currentRfApi == PROPRIETARY)
    {
        return (RF_Op*)Phy_phyTableProp[PhyManager_currentPhyIndex].pFsCmd;
    }
    else if(PhyManager_currentRfApi == PROPRIETARY_15_4_G)
    {
        return (RF_Op*)Phy_phyTableProp_15_4_g[PhyManager_currentPhyIndex].pFsCmd;
    }
    else if(PhyManager_currentRfApi == IEEE_802_15_4)
    {
        return (RF_Op*)Phy_phyTableIeee_15_4[PhyManager_currentPhyIndex].pFsCmd;
    }
    else if(PhyManager_currentRfApi == BLE_5_1M)
    {
        return (RF_Op*)Phy_phyTableBle_5[PhyManager_currentPhyIndex].pFsCmd;
    }
    else if(PhyManager_currentRfApi == WBMS)
    {
        return (RF_Op*)Phy_phyTableWbms[PhyManager_currentPhyIndex].pFsCmd;
    }
    
    return NULL;
}


RF_Op* PhyManager_getRxCmd(void)
{
    if(PhyManager_currentRfApi == PROPRIETARY)
    {
        return (RF_Op*)Phy_phyTableProp[PhyManager_currentPhyIndex].pRxCmd;
    }
    else if(PhyManager_currentRfApi == PROPRIETARY_15_4_G)
    {
        return (RF_Op*)Phy_phyTableProp_15_4_g[PhyManager_currentPhyIndex].pRxCmd;
    }
    else if(PhyManager_currentRfApi == IEEE_802_15_4)
    {
        return (RF_Op*)Phy_phyTableIeee_15_4[PhyManager_currentPhyIndex].pRxCmd;
    }
    else if(PhyManager_currentRfApi == BLE_5_1M)
    {
        return (RF_Op*)Phy_phyTableBle_5[PhyManager_currentPhyIndex].pRxCmd;
    }
    else if(PhyManager_currentRfApi == WBMS)
    {
        return (RF_Op*)Phy_phyTableWbms[PhyManager_currentPhyIndex].pRxCmd;
    }
    
    return NULL;
}


RF_Op* PhyManager_getTxCmd(void)
{
    if(PhyManager_currentRfApi == PROPRIETARY)
    {
        return (RF_Op*)Phy_phyTableProp[PhyManager_currentPhyIndex].pTxCmd;
    }
    else if(PhyManager_currentRfApi == PROPRIETARY_15_4_G)
    {
        return (RF_Op*)Phy_phyTableProp_15_4_g[PhyManager_currentPhyIndex].pTxCmd;
    }
    else if(PhyManager_currentRfApi == IEEE_802_15_4)
    {
        return (RF_Op*)Phy_phyTableIeee_15_4[PhyManager_currentPhyIndex].pTxCmd;
    }
    else if(PhyManager_currentRfApi == BLE_5_1M)
    {
        return (RF_Op*)Phy_phyTableBle_5[PhyManager_currentPhyIndex].pTxCmd;
    }
    else if(PhyManager_currentRfApi == WBMS)
    {
        return (RF_Op*)Phy_phyTableWbms[PhyManager_currentPhyIndex].pTxCmd;
    }
      
    return NULL;
}


RF_Mode* PhyManager_getRfMode(void)
{
    if(PhyManager_currentRfApi == PROPRIETARY)
    {
        return (RF_Mode*)Phy_phyTableProp[PhyManager_currentPhyIndex].pRfMode;
    }
    else if(PhyManager_currentRfApi == PROPRIETARY_15_4_G)
    {
        return (RF_Mode*)Phy_phyTableProp_15_4_g[PhyManager_currentPhyIndex].pRfMode;
    }
    else if(PhyManager_currentRfApi == IEEE_802_15_4)
    {
        return (RF_Mode*)Phy_phyTableIeee_15_4[PhyManager_currentPhyIndex].pRfMode;
    }
    else if(PhyManager_currentRfApi == BLE_5_1M)
    {
        return (RF_Mode*)Phy_phyTableBle_5[PhyManager_currentPhyIndex].pRfMode;
    }
    else if(PhyManager_currentRfApi == WBMS)
    {
        return (RF_Mode*)Phy_phyTableWbms[PhyManager_currentPhyIndex].pRfMode;
    }
    
    return NULL;
}


void PhyManager_configureTxCommand(uint8_t* packet, uint16_t packetLength)
{
    phyApis[PhyManager_currentRfApi].configureTxCmd(packet, packetLength, PhyManager_currentPhyIndex);
}


extern Phy_RfApi PhyManager_getRfApi(void)
{
    return PhyManager_currentRfApi;
}


extern uint8_t PhyManager_getPhyIndex(void)
{
    return PhyManager_currentPhyIndex;      
}


//! \brief Configure RF Core commands
//!        Note: The configuration of RF command used for TX is handled in a 
//!        separate function, and not in this.   
//!
//! \return None
void PhyManager_configureCommands(void)
{
    phyApis[PhyManager_currentRfApi].configureSetupCmd(PhyManager_currentPhyIndex);
    phyApis[PhyManager_currentRfApi].configureRxCmd(PhyManager_currentPhyIndex);
}


//! \brief Check if PHY number is valid
//!
//! \param[in] phyNumber
//!            PHY number
//!
//! \return true if PHY number is valid, and false otherwise
bool PhyManager_isPhyNumberValid(uint8_t phyNumber)
{ 
    if(phyNumber > (Phy_getNumSupportedPhys() - 1))
    {
        return false;
    }
    
    Phy_RfApi rfApi = Phy_supportedPhys[phyNumber].rfApi;
    uint8_t phyIndex = Phy_supportedPhys[phyNumber].phyIndex;
    
    bool status = false;
    if(rfApi == PROPRIETARY)
    {
        status = phyIndex > (Phy_getNumPropPhys() - 1) ? false : true;
    }
    else if(rfApi == PROPRIETARY_15_4_G)
    {
        status = phyIndex > (Phy_getNumProp_15_4_g_Phys() - 1) ? false : true;
    }
    else if(rfApi == IEEE_802_15_4)
    {
        status = phyIndex > (Phy_getNumIeee_15_4_Phys() - 1) ? false : true;
    }
    else if(rfApi == BLE_5_1M)
    {
        status = phyIndex > (Phy_getNumBle_5_Phys() - 1) ? false : true;
    }
    else if(rfApi == WBMS)
    {
        status = phyIndex > (Phy_getNumWbmsPhys() - 1) ? false : true;
    }
    
    return status;
}


//! \brief Check if BLE phy is supported
//!
//! \return true if BLE phy is supported, false otherwise
bool PhyManager_isBlePhySupported()
{
    return (Phy_getNumBle_5_Phys() != 0);
}





