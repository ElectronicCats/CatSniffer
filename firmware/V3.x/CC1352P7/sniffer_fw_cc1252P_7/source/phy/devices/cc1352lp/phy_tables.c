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
#include <rf_prop/smartrf_settings_rf_prop_2.h>
#include <rf_prop/smartrf_settings_rf_prop_3.h>
#include <15.4g/smartrf_settings_15_4g_0.h>
#include <15.4g/smartrf_settings_15_4g_1.h>
#include <15.4g/smartrf_settings_15_4g_2.h>
#include <15.4g/smartrf_settings_15_4g_3.h>
#include <15.4g/smartrf_settings_15_4g_4.h>
#include <15.4g/smartrf_settings_15_4g_5.h>
#include <15.4g/smartrf_settings_15_4g_6.h>
#include <15.4g/smartrf_settings_15_4g_7.h>
#include <15.4g/smartrf_settings_15_4g_8.h>
#include <15.4g/smartrf_settings_15_4g_9.h>
#include <15.4g/smartrf_settings_15_4g_10.h>
#include <15.4g/smartrf_settings_15_4g_11.h>
#include <15.4g/smartrf_settings_15_4g_12.h>
#include <15.4g/smartrf_settings_15_4g_13.h>
#include <15.4/smartrf_settings_15_4_0.h>
#include <ble5/smartrf_settings_ble5_0.h>

// Table of RF Proprietary PHY settings (These settings use Proprietary RF core commands)
const Phy_Prop_Obj Phy_phyTableProp[] = 
{
    // 50 kbps, 2-GFSK, 868 MHz band
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&RFProp0_cmdPropRadioDivSetup, &RFProp0_cmdFs, &RFProp0_cmdPropRx, &RFProp0_cmdPropTx, &RFProp0_mode},
    // 50 kbps, 2-GFSK, 433 MHz band
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&RFProp1_cmdPropRadioDivSetup, &RFProp1_cmdFs, &RFProp1_cmdPropRx, &RFProp1_cmdPropTx, &RFProp1_mode},
    // 5 kbps, SimpleLink Long Range, 868 Mhz band
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&RFProp2_cmdPropRadioDivSetup, &RFProp2_cmdFs, &RFProp2_cmdPropRx, &RFProp2_cmdPropTx, &RFProp2_mode},
    // 5 kbps, SimpleLink Long Range, 433 MHz band
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&RFProp3_cmdPropRadioDivSetup, &RFProp3_cmdFs, &RFProp3_cmdPropRx, &RFProp3_cmdPropTx, &RFProp3_mode},
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of IEEE 15.4g PHY settings (These settings use Proprietary RF core commands in IEEE 802.15.4g mode)
const Phy_Prop_15_4_g_Obj Phy_phyTableProp_15_4_g[] = 
{
    // 50 kbps, 2-GFSK, 868 Mhz band
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g0_cmdPropRadioDivSetup, &Ieee154g0_cmdFs, &Ieee154g0_cmdPropRxAdv, &Ieee154g0_cmdPropTxAdv, &Ieee154g0_mode},
    // 50 kbps, 2-GFSK, 433 Mhz band
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g1_cmdPropRadioDivSetup, &Ieee154g1_cmdFs, &Ieee154g1_cmdPropRxAdv, &Ieee154g1_cmdPropTxAdv, &Ieee154g1_mode},
    // 5 kbps, SimpleLink Long Range, 868 Mhz band
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g2_cmdPropRadioDivSetup, &Ieee154g2_cmdFs, &Ieee154g2_cmdPropRxAdv, &Ieee154g2_cmdPropTxAdv, &Ieee154g2_mode},
    // 5 kbps, SimpleLink Long Range, 433 Mhz band
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g3_cmdPropRadioDivSetup, &Ieee154g3_cmdFs, &Ieee154g3_cmdPropRxAdv, &Ieee154g3_cmdPropTxAdv, &Ieee154g3_mode},
    // 50 kbps, WiSUN mode #1a 
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g4_cmdPropRadioDivSetup, &Ieee154g4_cmdFs, &Ieee154g4_cmdPropRxAdv, &Ieee154g4_cmdPropTxAdv, &Ieee154g4_mode},
    // 50 kbps, WiSUN mode #1b 
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g5_cmdPropRadioDivSetup, &Ieee154g5_cmdFs, &Ieee154g5_cmdPropRxAdv, &Ieee154g5_cmdPropTxAdv, &Ieee154g5_mode},
    // 100 kbps, WiSUN mode #2a
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g6_cmdPropRadioDivSetup, &Ieee154g6_cmdFs, &Ieee154g6_cmdPropRxAdv, &Ieee154g6_cmdPropTxAdv, &Ieee154g6_mode},
    // 100 kbps, WiSUN mode #2b
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g7_cmdPropRadioDivSetup, &Ieee154g7_cmdFs, &Ieee154g7_cmdPropRxAdv, &Ieee154g7_cmdPropTxAdv, &Ieee154g7_mode},
    // 150 kbps, WiSUN mode #3
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g8_cmdPropRadioDivSetup, &Ieee154g8_cmdFs, &Ieee154g8_cmdPropRxAdv, &Ieee154g8_cmdPropTxAdv, &Ieee154g8_mode},
    // 200 kbps, WiSUN mode #4a
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g9_cmdPropRadioDivSetup, &Ieee154g9_cmdFs, &Ieee154g9_cmdPropRxAdv, &Ieee154g9_cmdPropTxAdv, &Ieee154g9_mode},
    // 200 kbps, WiSUN mode #4b
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g10_cmdPropRadioDivSetup, &Ieee154g10_cmdFs, &Ieee154g10_cmdPropRxAdv, &Ieee154g10_cmdPropTxAdv, &Ieee154g10_mode},
    // 100 kbps, ZigBee R23 non N.A. region   
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g11_cmdPropRadioDivSetup, &Ieee154g11_cmdFs, &Ieee154g11_cmdPropRxAdv, &Ieee154g11_cmdPropTxAdv, &Ieee154g11_mode},
    // 500 kbps, ZigBee R23 N.A. region   
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g12_cmdPropRadioDivSetup, &Ieee154g12_cmdFs, &Ieee154g12_cmdPropRxAdv, &Ieee154g12_cmdPropTxAdv, &Ieee154g12_mode},
     // 200 kbps, 915MHz
    {(rfc_CMD_PROP_RADIO_DIV_SETUP_t*)&Ieee154g13_cmdPropRadioDivSetup, &Ieee154g13_cmdFs, &Ieee154g13_cmdPropRxAdv, &Ieee154g13_cmdPropTxAdv, &Ieee154g13_mode},
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of IEEE 15.4 PHY settings (These settings use IEEE 802.15.4 RF core commands)
const Phy_Ieee_15_4_Obj Phy_phyTableIeee_15_4[] = 
{
    // IEEE 802.15.4 at 2.4GHz
    {(rfc_CMD_RADIO_SETUP_t*)&Ieee154_0_cmdRadioSetup, &Ieee154_0_cmdFs, &Ieee154_0_cmdIeeeRx, &Ieee154_0_cmdIeeeTx, &Ieee154_0_mode},
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of BLE 5 PHY settings (These settings use BLE 5 RF core commands)
const Phy_Ble_5_Obj Phy_phyTableBle_5[] = 
{
    // BLE 5 
    // The Radio Setup command has cast to type rfc_CMD_BLE5_RADIO_SETUP_t*. The difference between the types rfc_CMD_BLE5_RADIO_SETUP_t and rfc_CMD_BLE5_RADIO_SETUP_PA_t
    // is that the latter type has 2 more override pointers at the end. 
    {(rfc_CMD_BLE5_RADIO_SETUP_t*)&Ble5_0_cmdBle5RadioSetup, &Ble5_0_cmdFs, &Ble5_0_cmdBle5GenericRx, &Ble5_0_cmdBleAdvNc, &Ble5_0_mode},
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of WBMS PHY settings 
const Phy_Wbms_Obj Phy_phyTableWbms[] = 
{
    // WBMS PHY is not supported for this device 
    {NULL, NULL, NULL, NULL, NULL}
};


// Table of all supported Rf API and PHY setting combinations
// Note: This table must match the PHY tables for each RF API 
const Phy_Obj Phy_supportedPhys[] =
{
  {PROPRIETARY_15_4_G, 0},
  {PROPRIETARY_15_4_G, 1},
  {PROPRIETARY_15_4_G, 2},
  {PROPRIETARY_15_4_G, 3},
  {PROPRIETARY_15_4_G, 4},
  {PROPRIETARY_15_4_G, 5},
  {PROPRIETARY_15_4_G, 6},
  {PROPRIETARY_15_4_G, 7},
  {PROPRIETARY_15_4_G, 8},
  {PROPRIETARY_15_4_G, 9},
  {PROPRIETARY_15_4_G, 10},
  {PROPRIETARY_15_4_G, 11},
  {PROPRIETARY_15_4_G, 12},
  {PROPRIETARY_15_4_G, 13},
  {PROPRIETARY, 0},
  {PROPRIETARY, 1},
  {PROPRIETARY, 2},
  {PROPRIETARY, 3},
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


uint8_t Phy_getNumWbmsPhys(void)
{
    return (sizeof(Phy_phyTableWbms)/sizeof(Phy_phyTableWbms[0])) - 1;
}
