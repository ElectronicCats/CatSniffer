/******************************************************************************
*  Filename:       phy_tables.h
*
*  Description:    Header file for PHY tables
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
#ifndef PHY_TABLES_H
#define PHY_TABLES_H

// TI-RTOS radio driver
#include <ti/drivers/rf/RF.h>



// There are no rf_ieee_cmd.h for DeviceFamily_CC13X0
#ifndef DeviceFamily_CC13X0 
// A separate include file is required for IEEE command definitions. 
// RF.h does not include the header file for this. 
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_ieee_cmd.h)
#else
// For DeviceFamily_CC13X0 define the IEEE 15.4 and BLE 5 RF commands as void
// These are not used for CC13X0, but required to build the project. 
#define rfc_CMD_IEEE_RX_t               void
#define rfc_CMD_IEEE_TX_t               void
#define rfc_CMD_BLE5_GENERIC_RX_t       void
#endif

#ifdef DeviceFamily_CC26X0
// For DeviceFamily_CC26X0 define the BLE 5 RF commands as void
// These are not used for CC26X0, but required to build the project. 
#define rfc_CMD_BLE5_GENERIC_RX_t       void
#endif

#include "phy_rf_api.h"


// Struct definition for RF Proprietary PHY settings
typedef struct Phy_Prop_Obj
{
    rfc_CMD_PROP_RADIO_DIV_SETUP_t* pSetupCmd;
    rfc_CMD_FS_t* pFsCmd;
    rfc_CMD_PROP_RX_t* pRxCmd;
    rfc_CMD_PROP_TX_t* pTxCmd;
    RF_Mode* pRfMode;
} Phy_Prop_Obj;


// Struct definition for IEEE 802.15.4g Proprietary PHY settings
typedef struct Phy_Prop_15_4_g_Obj
{
    rfc_CMD_PROP_RADIO_DIV_SETUP_t* pSetupCmd;
    rfc_CMD_FS_t* pFsCmd;
    rfc_CMD_PROP_RX_ADV_t* pRxCmd;
    rfc_CMD_PROP_TX_ADV_t* pTxCmd;
    RF_Mode* pRfMode;
} Phy_Prop_15_4_g_Obj;


// Struct definition for IEEE 802.15.4 PHY settings
typedef struct Phy_Ieee_15_4_Obj
{
    rfc_CMD_RADIO_SETUP_t* pSetupCmd;
    rfc_CMD_FS_t* pFsCmd;
    rfc_CMD_IEEE_RX_t* pRxCmd;
    rfc_CMD_IEEE_TX_t* pTxCmd;
    RF_Mode* pRfMode;
} Phy_Ieee_15_4_Obj;


// Struct definition for BLE 5 PHY settings
typedef struct Phy_Ble_5_Obj
{
    rfc_CMD_BLE5_RADIO_SETUP_t* pSetupCmd;
    rfc_CMD_FS_t* pFsCmd;
    rfc_CMD_BLE5_GENERIC_RX_t* pRxCmd;
    rfc_CMD_BLE_ADV_NC_t* pTxCmd;
    RF_Mode* pRfMode;
} Phy_Ble_5_Obj;


// Struct definition for WBMS PHY settings
typedef struct Phy_Wbms_Obj
{
    rfc_CMD_BLE5_RADIO_SETUP_t* pSetupCmd;
    rfc_CMD_FS_t* pFsCmd;
    rfc_CMD_PROP_RX_ADV_t* pRxCmd;
    rfc_CMD_PROP_TX_ADV_t* pTxCmd;
    RF_Mode* pRfMode;
} Phy_Wbms_Obj;


// Struct definition for a PHY (That is combination of RF API and PHY setting)
typedef struct Phy_Obj
{
    Phy_RfApi rfApi;
    uint8_t phyIndex;
} Phy_Obj;


// Table of proprietary PHY settings
extern const Phy_Prop_Obj Phy_phyTableProp[];

// Table of proprietary IEEE 802.15.4g PHY settings
extern const Phy_Prop_15_4_g_Obj Phy_phyTableProp_15_4_g[];

// Table of IEEE 802.15.4 PHY settings
extern const Phy_Ieee_15_4_Obj Phy_phyTableIeee_15_4[];

// Table of BLE 5 PHY settings
extern const Phy_Ble_5_Obj Phy_phyTableBle_5[];

// Table of WBMS PHY settings
extern const Phy_Wbms_Obj Phy_phyTableWbms[];

// Table of all supported PHYs (Combinations of RF API and setting)
extern const Phy_Obj Phy_supportedPhys[];


//! \brief Get number of supported PHYs (Combinations of RF API and setting)
//!
//! \return Number of supported PHYs
extern uint8_t Phy_getNumSupportedPhys(void);


//! \brief Get number of proprietary PHY settings
//!
//! \return Number of proprietary PHY settings
extern uint8_t Phy_getNumPropPhys(void);


//! \brief Get number of proprietary IEEE 802.15.4g PHY settings 
//!
//! \return Number of proprietary IEEE 802.15.4g PHY settings
extern uint8_t Phy_getNumProp_15_4_g_Phys(void);


//! \brief Get number of IEEE 802.15.4 PHY settings 
//!
//! \return Number of IEEE 802.15.4 PHY settings
extern uint8_t Phy_getNumIeee_15_4_Phys(void);


//! \brief Get number of BLE 5 PHY settings 
//!
//! \return Number of IEEE BLE 5 PHY settings
extern uint8_t Phy_getNumBle_5_Phys(void);

//! \brief Get number of WBMS PHY settings 
//!
//! \return Number of WBMS PHY settings
extern uint8_t Phy_getNumWbmsPhys(void);

#endif

