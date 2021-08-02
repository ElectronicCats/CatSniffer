/******************************************************************************
*  Filename:       phy_tables.c
*
*  Description:    Source file PHY tables
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

#include "../../phy_tables.h"
#include <rf_prop/smartrf_settings_rf_prop_0.h>
#include <rf_prop/smartrf_settings_rf_prop_1.h>
#include <15.4g/smartrf_settings_15_4g_0.h>
#include <15.4g/smartrf_settings_15_4g_1.h>
#include <15.4/smartrf_settings_15_4_0.h>
#include <ble5/smartrf_settings_ble5_0.h>

// Table of RF Proprietary PHY settings (These settings use Proprietary RF core commands)
const Phy_Prop_Obj Phy_phyTableProp[] = 
{
    // 50 kbps, 2-GFSK
    {&RFProp0_cmdPropRadioDivSetup, &RFProp0_cmdFs, &RFProp0_cmdPropRx, &RFProp0_cmdPropTx, &RFProp0_mode},
    // 5 kbps, SimpleLink Long Range 
    {&RFProp1_cmdPropRadioDivSetup, &RFProp1_cmdFs, &RFProp1_cmdPropRx, &RFProp1_cmdPropTx, &RFProp1_mode}, 
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of IEEE 15.4g PHY settings (These settings use Proprietary RF core commands in IEEE 802.15.4g mode)
const Phy_Prop_15_4_g_Obj Phy_phyTableProp_15_4_g[] = 
{
    // 50 kbps, 2-GFSK
    {&Ieee154g0_cmdPropRadioDivSetup, &Ieee154g0_cmdFs, &Ieee154g0_cmdPropRxAdv, &Ieee154g0_cmdPropTxAdv, &Ieee154g0_mode},
    // 5 kbps, SimpleLink Long Range 
    {&Ieee154g1_cmdPropRadioDivSetup, &Ieee154g1_cmdFs, &Ieee154g1_cmdPropRxAdv, &Ieee154g1_cmdPropTxAdv, &Ieee154g1_mode},
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of IEEE 15.4 PHY settings (These settings use IEEE 802.15.4 RF core commands)
const Phy_Ieee_15_4_Obj Phy_phyTableIeee_15_4[] = 
{
    // IEEE 802.15.4 at 2.4GHz
    {&Ieee154_0_cmdRadioSetup, &Ieee154_0_cmdFs, &Ieee154_0_cmdIeeeRx, &Ieee154_0_cmdIeeeTx, &Ieee154_0_mode},
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of BLE 5 PHY settings (These settings use BLE 5 RF core commands)
const Phy_Ble_5_Obj Phy_phyTableBle_5[] = 
{
    // BLE 5 
    {&Ble5_0_cmdBle5RadioSetup, &Ble5_0_cmdFs, &Ble5_0_cmdBle5GenericRx, &Ble5_0_cmdBleAdvNc, &Ble5_0_mode},
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of all supported Rf API and PHY setting combinations
// Note: This table must match the PHY tables for each RF API 
const Phy_Obj Phy_supportedPhys[] =
{
  {PROPRIETARY_15_4_G, 0},
  {PROPRIETARY_15_4_G, 1},
  {PROPRIETARY, 0},
  {PROPRIETARY, 1},
  {IEEE_802_15_4, 0},
  {BLE_5_1M, 0},
};



uint8_t Phy_getNumSupportedPhys(void)
{
    return sizeof(Phy_supportedPhys)/sizeof(Phy_supportedPhys[0]);
}


uint8_t Phy_getNumPropPhys(void)
{
    return (sizeof(Phy_phyTableProp)/sizeof(Phy_phyTableProp[0])) - 1;
}


uint8_t Phy_getNumProp_15_4_g_Phys(void)
{
    return (sizeof(Phy_phyTableProp_15_4_g)/sizeof(Phy_phyTableProp_15_4_g[0])) - 1;
}


uint8_t Phy_getNumIeee_15_4_Phys(void)
{
    return (sizeof(Phy_phyTableIeee_15_4)/sizeof(Phy_phyTableIeee_15_4[0])) - 1;
}


uint8_t Phy_getNumBle_5_Phys(void)
{
    return (sizeof(Phy_phyTableBle_5)/sizeof(Phy_phyTableBle_5[0])) - 1;
}
