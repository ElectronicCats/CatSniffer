/*
 *  ======== ti_radio_config.c ========
 *  Configured RadioConfig module definitions
 *
 *  DO NOT EDIT - This file is generated for the CC1352P1F3RGZ
 *  by the SysConfig tool.
 *
 *  Radio Config module version : 1.17.2
 *  SmartRF Studio data version : 2.29.0
 */

#include "ti_radio_config.h"
#include DeviceFamily_constructPath(rf_patches/rf_patch_cpe_bt5.h)

// Custom overrides
#include <ti/ble5stack/icall/inc/ble_overrides.h>


// *********************************************************************************
//   RF Frontend configuration
// *********************************************************************************
// RF design based on: LAUNCHXL-CC1352P-2

// TX Power tables
// The RF_TxPowerTable_DEFAULT_PA_ENTRY and RF_TxPowerTable_HIGH_PA_ENTRY macros are defined in RF.h.
// The following arguments are required:
// RF_TxPowerTable_DEFAULT_PA_ENTRY(bias, gain, boost, coefficient)
// RF_TxPowerTable_HIGH_PA_ENTRY(bias, ibboost, boost, coefficient, ldoTrim)
// See the Technical Reference Manual for further details about the "txPower" Command field.
// The PA settings require the CCFG_FORCE_VDDR_HH = 0 unless stated otherwise.

// 2400 MHz, 5 dBm
RF_TxPowerTable_Entry txPowerTable_2400_pa5[TXPOWERTABLE_2400_PA5_SIZE] =
{
    {-20, RF_TxPowerTable_DEFAULT_PA_ENTRY(6, 3, 0, 2) }, // 0x04C6
    {-18, RF_TxPowerTable_DEFAULT_PA_ENTRY(8, 3, 0, 3) }, // 0x06C8
    {-15, RF_TxPowerTable_DEFAULT_PA_ENTRY(10, 3, 0, 3) }, // 0x06CA
    {-12, RF_TxPowerTable_DEFAULT_PA_ENTRY(12, 3, 0, 5) }, // 0x0ACC
    {-10, RF_TxPowerTable_DEFAULT_PA_ENTRY(15, 3, 0, 5) }, // 0x0ACF
    {-9, RF_TxPowerTable_DEFAULT_PA_ENTRY(16, 3, 0, 5) }, // 0x0AD0
    {-6, RF_TxPowerTable_DEFAULT_PA_ENTRY(20, 3, 0, 8) }, // 0x10D4
    {-5, RF_TxPowerTable_DEFAULT_PA_ENTRY(22, 3, 0, 9) }, // 0x12D6
    {-3, RF_TxPowerTable_DEFAULT_PA_ENTRY(19, 2, 0, 12) }, // 0x1893
    {0, RF_TxPowerTable_DEFAULT_PA_ENTRY(19, 1, 0, 20) }, // 0x2853
    {1, RF_TxPowerTable_DEFAULT_PA_ENTRY(22, 1, 0, 20) }, // 0x2856
    {2, RF_TxPowerTable_DEFAULT_PA_ENTRY(25, 1, 0, 25) }, // 0x3259
    {3, RF_TxPowerTable_DEFAULT_PA_ENTRY(29, 1, 0, 28) }, // 0x385D
    {4, RF_TxPowerTable_DEFAULT_PA_ENTRY(35, 1, 0, 39) }, // 0x4E63
    {5, RF_TxPowerTable_DEFAULT_PA_ENTRY(23, 0, 0, 57) }, // 0x7217
    RF_TxPowerTable_TERMINATION_ENTRY
};

// 2400 MHz, 5 + 20 dBm
RF_TxPowerTable_Entry txPowerTable_2400_pa5_20[TXPOWERTABLE_2400_PA5_20_SIZE] =
{
    {-20, RF_TxPowerTable_DEFAULT_PA_ENTRY(6, 3, 0, 2) }, // 0x04C6
    {-18, RF_TxPowerTable_DEFAULT_PA_ENTRY(8, 3, 0, 3) }, // 0x06C8
    {-15, RF_TxPowerTable_DEFAULT_PA_ENTRY(10, 3, 0, 3) }, // 0x06CA
    {-12, RF_TxPowerTable_DEFAULT_PA_ENTRY(12, 3, 0, 5) }, // 0x0ACC
    {-10, RF_TxPowerTable_DEFAULT_PA_ENTRY(15, 3, 0, 5) }, // 0x0ACF
    {-9, RF_TxPowerTable_DEFAULT_PA_ENTRY(16, 3, 0, 5) }, // 0x0AD0
    {-6, RF_TxPowerTable_DEFAULT_PA_ENTRY(20, 3, 0, 8) }, // 0x10D4
    {-5, RF_TxPowerTable_DEFAULT_PA_ENTRY(22, 3, 0, 9) }, // 0x12D6
    {-3, RF_TxPowerTable_DEFAULT_PA_ENTRY(19, 2, 0, 12) }, // 0x1893
    {0, RF_TxPowerTable_DEFAULT_PA_ENTRY(19, 1, 0, 20) }, // 0x2853
    {1, RF_TxPowerTable_DEFAULT_PA_ENTRY(22, 1, 0, 20) }, // 0x2856
    {2, RF_TxPowerTable_DEFAULT_PA_ENTRY(25, 1, 0, 25) }, // 0x3259
    {3, RF_TxPowerTable_DEFAULT_PA_ENTRY(29, 1, 0, 28) }, // 0x385D
    {4, RF_TxPowerTable_DEFAULT_PA_ENTRY(35, 1, 0, 39) }, // 0x4E63
    {5, RF_TxPowerTable_DEFAULT_PA_ENTRY(23, 0, 0, 57) }, // 0x7217
    {14, RF_TxPowerTable_HIGH_PA_ENTRY(22, 3, 1, 19, 27) }, // 0x1B27D6
    {15, RF_TxPowerTable_HIGH_PA_ENTRY(26, 3, 1, 23, 27) }, // 0x1B2FDA
    {16, RF_TxPowerTable_HIGH_PA_ENTRY(30, 3, 1, 28, 27) }, // 0x1B39DE
    {17, RF_TxPowerTable_HIGH_PA_ENTRY(37, 3, 1, 39, 27) }, // 0x1B4FE5
    {18, RF_TxPowerTable_HIGH_PA_ENTRY(32, 3, 1, 35, 48) }, // 0x3047E0
    {19, RF_TxPowerTable_HIGH_PA_ENTRY(34, 3, 1, 48, 63) }, // 0x3F61E2
    {20, RF_TxPowerTable_HIGH_PA_ENTRY(53, 3, 1, 58, 63) }, // 0x3F75F5
    RF_TxPowerTable_TERMINATION_ENTRY
};



//*********************************************************************************
//  RF Setting:   BLE, 2 Mbps, LE 2M
//
//  PHY:          bt5le2m
//  Setting file: setting_bt5_le_2m.json
//*********************************************************************************

// PARAMETER SUMMARY
// NB! Setting RF parameters in this design has no effect as no RF commands are selected.

// TI-RTOS RF Mode Object
RF_Mode RF_modeBle =
{
    .rfMode = RF_MODE_AUTO,
    .cpePatchFxn = &rf_patch_cpe_bt5,
    .mcePatchFxn = 0,
    .rfePatchFxn = 0
};

// Overrides for CMD_BLE5_RADIO_SETUP_PA
uint32_t pOverrides_bleCommon[] =
{
    // override_ble5_setup_override_common_hpa.json
    // Bluetooth 5: Reconfigure to 35 us pilot tone length for high output power PA
    HW_REG_OVERRIDE(0x6024,0x5B20),
    // Bluetooth 5: Compensate for 35 us pilot tone length
    (uint32_t)0x01640263,
    // Bluetooth 5: Set IPEAK = 3 and DCDC dither off for TX
    (uint32_t)0x00F388D3,
    // Bluetooth 5: Default to no CTE. 
    HW_REG_OVERRIDE(0x5328,0x0000),
    // Synth: Increase mid code calibration time to 5 us
    (uint32_t)0x00058683,
    // Synth: Increase mid code calibration time to 5 us
    HW32_ARRAY_OVERRIDE(0x4004,1),
    // Synth: Increase mid code calibration time to 5 us
    (uint32_t)0x38183C30,
    // Synth: Set calibration fine point code to 60 (default: 64)
    HW_REG_OVERRIDE(0x4064,0x003C),
    // Bluetooth 5: Set DTX gain -5% for 1 Mbps
    (uint32_t)0x00E787E3,
    // Bluetooth 5: Set DTX threshold 1 Mbps
    (uint32_t)0x00950803,
    // Bluetooth 5: Set DTX gain -2.5% for 2 Mbps
    (uint32_t)0x00F487F3,
    // Bluetooth 5: Set DTX threshold 2 Mbps
    (uint32_t)0x012A0823,
    // Bluetooth 5: Set synth fine code calibration interval
    HW32_ARRAY_OVERRIDE(0x4020,0x0001),
    // Bluetooth 5: Set synth fine code calibration interval
    (uint32_t)0x41005F00,
    // Bluetooth 5: Adapt to synth fine code calibration interval
    (uint32_t)0xC0040141,
    // Bluetooth 5: Adapt to synth fine code calibration interval
    (uint32_t)0x0007DD44,
    // ti/ble5stack/icall/inc/ble_overrides.h
    BLE_STACK_OVERRIDES(),
    (uint32_t)0xFFFFFFFF
};

// Overrides for CMD_BLE5_RADIO_SETUP_PA
uint32_t pOverrides_ble1Mbps[] =
{
    // override_ble5_setup_override_1mbps_hpa.json
    // Bluetooth 5: Reconfigure pilot tone length for high output power PA
    HW_REG_OVERRIDE(0x5320,0x0690),
    // Bluetooth 5: Compensate for modified pilot tone length
    (uint32_t)0x018F02A3,
    // override_ble5_symbol_error_tracking.json
    // Symbol tracking: timing correction
    HW_REG_OVERRIDE(0x50D4,0x00F9),
    // Symbol tracking: reduce sample delay
    HW_REG_OVERRIDE(0x50E0,0x0087),
    // Symbol tracking: demodulation order
    HW_REG_OVERRIDE(0x50F8,0x0014),
    (uint32_t)0xFFFFFFFF
};

// Overrides for CMD_BLE5_RADIO_SETUP_PA
uint32_t pOverrides_ble2Mbps[] =
{
    // override_ble5_setup_override_2mbps_hpa.json
    // Bluetooth 5: Reconfigure pilot tone length for high output power PA
    HW_REG_OVERRIDE(0x5320,0x0690),
    // Bluetooth 5: Compensate for modified pilot tone length
    (uint32_t)0x012D02A3,
    // Bluetooth 5: increase low gain AGC delay for 2 Mbps
    HW_REG_OVERRIDE(0x60A4,0x7D00),
    // override_ble5_symbol_error_tracking.json
    // Symbol tracking: timing correction
    HW_REG_OVERRIDE(0x50D4,0x00F9),
    // Symbol tracking: reduce sample delay
    HW_REG_OVERRIDE(0x50E0,0x0087),
    // Symbol tracking: demodulation order
    HW_REG_OVERRIDE(0x50F8,0x0014),
    (uint32_t)0xFFFFFFFF
};

// Overrides for CMD_BLE5_RADIO_SETUP_PA
uint32_t pOverrides_bleCoded[] =
{
    // override_ble5_setup_override_coded_hpa.json
    // Bluetooth 5: Set pilot tone length to 35 us
    HW_REG_OVERRIDE(0x5320,0x0690),
    // Bluetooth 5: Compensate for modified pilot tone length
    (uint32_t)0x07E502A3,
    // Bluetooth 5: Set AGC mangnitude target to 0x21.
    HW_REG_OVERRIDE(0x609C,0x0021),
    (uint32_t)0xFFFFFFFF
};

// Overrides for CMD_BLE5_RADIO_SETUP_PA
uint32_t pOverrides_bleTxStd[] =
{
    // override_txstd_placeholder.json
    // TX Standard power override
    TX_STD_POWER_OVERRIDE(0x7217),
    // The ANADIV radio parameter based on LO divider and front end settings
    (uint32_t)0x05320703,
    // override_txstd_settings.json
    // Bluetooth 5: Set RTIM offset to default for standard PA
    (uint32_t)0x00008783,
    // Bluetooth 5: Set synth mux to default value for standard PA
    (uint32_t)0x050206C3,
    // Set TXRX pin to 0 in RX and high impedance in idle/TX. 
    HW_REG_OVERRIDE(0x60A8,0x0401),
    // Bluetooth 5: Set DTX gain -5% for 1 Mbps
    (uint32_t)0x00E787E3,
    // Bluetooth 5: Set DTX gain -2.5% for 2 Mbps
    (uint32_t)0x00F487F3,
    // Bluetooth 5: Set enhanced TX shape
    (uint32_t)0x000D8C73,
    (uint32_t)0xFFFFFFFF
};

// Overrides for CMD_BLE5_RADIO_SETUP_PA
uint32_t pOverrides_bleTx20[] =
{
    // override_tx20_placeholder.json
    // TX HighPA power override
    TX20_POWER_OVERRIDE(0x003F75F5),
    // The ANADIV radio parameter based on LO divider and front end settings
    (uint32_t)0x01C20703,
    // override_tx20_settings.json
    // Bluetooth 5: Set RTIM offset to 3 for high power PA
    (uint32_t)0x00030783,
    // Bluetooth 5: Set synth mux for high power PA
    (uint32_t)0x010206C3,
    // Set TXRX pin to 0 in RX/TX and high impedance in idle. 
    HW_REG_OVERRIDE(0x60A8,0x0001),
    // Bluetooth 5: Turn off DTX gain adjustment
    (uint32_t)0x000007E3,
    // Bluetooth 5: Set default TX shape
    (uint32_t)0x00008C73,
    (uint32_t)0xFFFFFFFF
};




