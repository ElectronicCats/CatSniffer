

//*********************************************************************************
// Generated by SmartRF Studio version 2.7.0 (build #21)
// Tested for SimpleLink SDK version: CC13x0 SDK 1.50.xx.xx
// Device: CC1350 Rev. 2.1
// 
//*********************************************************************************


//*********************************************************************************
// Parameter summary
// Address: off 
// Address0: 0xAA 
// Address1: 0xBB 
// Frequency: 433.92000 MHz
// Data Format: Serial mode disable 
// Deviation: 5.000 kHz
// Packet Length Config: Variable 
// Max Packet Length: 128 
// Packet Length: 20 
// RX Filter BW: 49 kHz
// Symbol Rate: 19.99969 kBaud
// Sync Word Length: 32 Bits 
// TX Power: 15 dBm (requires define CCFG_FORCE_VDDR_HH = 1 in ccfg.c, see CC13xx/CC26xx Technical Reference Manual)
// Whitening: No whitening 

/* driverlib header files */


#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_common_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_prop_cmd.h)
#include <ti/drivers/rf/RF.h>
#include DeviceFamily_constructPath(rf_patches/rf_patch_cpe_sl_longrange.h)
#include DeviceFamily_constructPath(rf_patches/rf_patch_rfe_sl_longrange.h)
#include DeviceFamily_constructPath(rf_patches/rf_patch_mce_sl_longrange.h)
#include "smartrf_settings_15_4g_3.h"


// TI-RTOS RF Mode Object
RF_Mode Ieee154g3_mode =
{
    .rfMode = RF_MODE_PROPRIETARY_SUB_1,
    .cpePatchFxn = &rf_patch_cpe_sl_longrange,
    .mcePatchFxn = &rf_patch_mce_sl_longrange,
    .rfePatchFxn = &rf_patch_rfe_sl_longrange,
};

// Overrides for CMD_PROP_RADIO_DIV_SETUP
static uint32_t pOverrides[] =
{
    // override_use_patch_simplelink_long_range.xml
    // PHY: Use MCE RAM patch, RFE RAM patch
    MCE_RFE_OVERRIDE(1,0,0,1,0,0),
    // override_synth_prop_430_510_div10_lbw60k.xml
    // Synth: Set recommended RTRIM to 7
    HW_REG_OVERRIDE(0x4038,0x0037),
    // Synth: Set Fref to 4 MHz
    (uint32_t)0x000684A3,
    // Synth: Configure fine calibration setting
    HW_REG_OVERRIDE(0x4020,0x7F00),
    // Synth: Configure fine calibration setting
    HW_REG_OVERRIDE(0x4064,0x0040),
    // Synth: Configure fine calibration setting
    (uint32_t)0xB1070503,
    // Synth: Configure fine calibration setting
    (uint32_t)0x05330523,
    // Synth: Set loop bandwidth after lock to 60 kHz
    (uint32_t)0x40410583,
    // Synth: Set loop bandwidth after lock to 60 kHz
    (uint32_t)0x32CC0603,
    // Synth: Set loop bandwidth after lock to 60 kHz
    (uint32_t)0x00010623,
    // Synth: Configure VCO LDO (in ADI1, set VCOLDOCFG=0x9F to use voltage input reference)
    ADI_REG_OVERRIDE(1,4,0x9F),
    // Synth: Configure synth LDO (in ADI1, set SLDOCTL0.COMP_CAP=1)
    ADI_HALFREG_OVERRIDE(1,7,0x4,0x4),
    // Synth: Use 24 MHz XOSC as synth clock, enable extra PLL filtering
    (uint32_t)0x02010403,
    // Synth: Configure extra PLL filtering
    (uint32_t)0x00108463,
    // Synth: Increase synth programming timeout (0x04B0 RAT ticks = 300 us)
    (uint32_t)0x04B00243,
    // override_synth_disable_bias_div10.xml
    // Synth: Set divider bias to disabled
    HW32_ARRAY_OVERRIDE(0x405C,1),
    // Synth: Set divider bias to disabled (specific for loDivider=10)
    (uint32_t)0x18000280,
    // override_phy_rx_aaf_bw_0xd.xml
    // Rx: Set anti-aliasing filter bandwidth to 0xD (in ADI0, set IFAMPCTL3[7:4]=0xD)
    ADI_HALFREG_OVERRIDE(0,61,0xF,0xD),
    // override_phy_gfsk_rx.xml
    // Rx: Set LNA bias current trim offset to 3
    (uint32_t)0x00038883,
    // Rx: Freeze RSSI on sync found event
    HW_REG_OVERRIDE(0x6084,0x35F1),
    // override_phy_gfsk_pa_ramp_agc_reflevel_0x16.xml
    // Tx: Configure PA ramping setting (0x41). Rx: Set AGC reference level to 0x16.
    HW_REG_OVERRIDE(0x6088,0x4116),
    // Tx: Configure PA ramping setting
    HW_REG_OVERRIDE(0x608C,0x8213),
    // override_phy_long_range_dsss2.xml
    // PHY: Configure DSSS SF=2
    HW_REG_OVERRIDE(0x505C,0x0100),
    // override_phy_rx_rssi_offset_neg2db.xml
    // Rx: Set RSSI offset to adjust reported RSSI by -2 dB
    (uint32_t)0x000288A3,
    // TX power override
    // Tx: Set PA trim to max (in ADI0, set PACTL0=0xF8)
    ADI_REG_OVERRIDE(0,12,0xF8),
    // Overrides required for correct crc-16 calculation (see TRM section 23.7.5.2.1)
    (uint32_t)0x943, 
    (uint32_t)0x963,
    // Disable pointer check in RF Core to allow for using GPRAM for buffers
    (uint32_t)0x00018063,
    (uint32_t)0xFFFFFFFF,
};


// CMD_PROP_RADIO_DIV_SETUP
rfc_CMD_PROP_RADIO_DIV_SETUP_t Ieee154g3_cmdPropRadioDivSetup =
{
    .commandNo = 0x3807,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .modulation.modType = 0x1,
    .modulation.deviation = 0x14,
    .symbolRate.preScale = 0xF,
    .symbolRate.rateWord = 0x3333,
    .rxBw = 0x21,
    .preamConf.nPreamBytes = 0x2,
    .preamConf.preamMode = 0x0,
    .formatConf.nSwBits = 0x20,
    .formatConf.bBitReversal = 0x0,
    .formatConf.bMsbFirst = 0x1,
    .formatConf.fecMode = 0x0,
    //.formatConf.whitenMode = 0x0,
    .formatConf.whitenMode = 0x7,
    .config.frontEndMode = 0x0,
    .config.biasMode = 0x1,
    .config.analogCfgMode = 0x0,
    .config.bNoFsPowerUp = 0x0,
    .txPower = 0x003F,
    .pRegOverride = pOverrides,
    .centerFreq = 0x01B2,
    .intFreq = 0x8000,
    .loDivider = 0x0A,
};

// CMD_FS
rfc_CMD_FS_t Ieee154g3_cmdFs =
{
    .commandNo = 0x0803,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .frequency = 0x01B1,
    .fractFreq = 0xEB85,
    .synthConf.bTxMode = 0x0,
    .synthConf.refFreq = 0x0,
    .__dummy0 = 0x00,
    .__dummy1 = 0x00,
    .__dummy2 = 0x00,
    .__dummy3 = 0x0000,
};

// CMD_PROP_TX
rfc_CMD_PROP_TX_t Ieee154g3_cmdPropTx =
{
    .commandNo = 0x3801,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bVarLen = 0x1,
    .pktLen = 0x14, // SET APPLICATION PAYLOAD LENGTH
    .syncWord = 0x00000000,
    .pPkt = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
};

// CMD_PROP_RX
rfc_CMD_PROP_RX_t Ieee154g3_cmdPropRx =
{
    .commandNo = 0x3802,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bRepeatOk = 0x0,
    .pktConf.bRepeatNok = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bVarLen = 0x1,
    .pktConf.bChkAddress = 0x0,
    .pktConf.endType = 0x0,
    .pktConf.filterOp = 0x0,
    .rxConf.bAutoFlushIgnored = 0x0,
    .rxConf.bAutoFlushCrcErr = 0x0,
    .rxConf.bIncludeHdr = 0x1,
    .rxConf.bIncludeCrc = 0x0,
    .rxConf.bAppendRssi = 0x0,
    .rxConf.bAppendTimestamp = 0x0,
    .rxConf.bAppendStatus = 0x1,
    .syncWord = 0x00000000,
    .maxPktLen = 0x80, // MAKE SURE DATA ENTRY IS LARGE ENOUGH
    .address0 = 0xAA,
    .address1 = 0xBB,
    .endTrigger.triggerType = TRIG_NEVER,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000000,
    .pQueue = 0, // INSERT APPLICABLE POINTER: (dataQueue_t*)&xxx
    .pOutput = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
};

// CMD_TX_TEST
rfc_CMD_TX_TEST_t Ieee154g3_cmdTxTest =
{
    .commandNo = 0x0808,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .config.bUseCw = 0x0,
    .config.bFsOff = 0x1,
    .config.whitenMode = 0x2,
    .__dummy0 = 0x00,
    .txWord = 0xABCD,
    .__dummy1 = 0x00,
    .endTrigger.triggerType = 0x1,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .syncWord = 0x00000000,
    .endTime = 0x00000000,
};

// CMD_PROP_RX_ADV
rfc_CMD_PROP_RX_ADV_t Ieee154g3_cmdPropRxAdv =
{
    .commandNo = 0x3804,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = TRIG_NOW,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bRepeatOk = 0x0,
    .pktConf.bRepeatNok = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bCrcIncSw = 0x0,
    .pktConf.bCrcIncHdr = 0x0,
    .pktConf.endType = 0x0,
    .pktConf.filterOp = 0x1,
    .rxConf.bAutoFlushIgnored = 0x0,
    .rxConf.bAutoFlushCrcErr = 0x0,
    .rxConf.bIncludeHdr = 0x1,
    .rxConf.bIncludeCrc = 0x1,
    .rxConf.bAppendRssi = 0x1,
    .rxConf.bAppendTimestamp = 0x0,
    .rxConf.bAppendStatus = 0x1,
    .syncWord0 = 0x00000000,
    .syncWord1 = 0x00000000,
    .maxPktLen = 0x0000,
    .hdrConf.numHdrBits = 16,
    .hdrConf.lenPos = 0x0,
    .hdrConf.numLenBits = 11,
    .addrConf.addrType = 0x0,
    .addrConf.addrSize = 0x0,
    .addrConf.addrPos = 0x0,
    .addrConf.numAddr = 0x0,
    .lenOffset = -4,
    .endTrigger.triggerType = TRIG_NEVER,
    .endTrigger.bEnaCmd = 0x0,
    .endTrigger.triggerNo = 0x0,
    .endTrigger.pastTrig = 0x0,
    .endTime = 0x00000000,
    .pAddr = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .pQueue = 0, // INSERT APPLICABLE POINTER: (dataQueue_t*)&xxx
    .pOutput = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
};

// CMD_PROP_TX_ADV
rfc_CMD_PROP_TX_ADV_t Ieee154g3_cmdPropTxAdv =
{
    .commandNo = 0x3803,
    .status = 0x0000,
    .pNextOp = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .pktConf.bFsOff = 0x0,
    .pktConf.bUseCrc = 0x1,
    .pktConf.bCrcIncSw = 0x0,
    .pktConf.bCrcIncHdr = 0x0,
    .numHdrBits = 16,
    .pktLen = 0x0000,
    .startConf.bExtTxTrig = 0x0,
    .startConf.inputMode = 0x0,
    .startConf.source = 0x0,
    .preTrigger.triggerType = 0x0,
    .preTrigger.bEnaCmd = 0x0,
    .preTrigger.triggerNo = 0x0,
    .preTrigger.pastTrig = 0x0,
    .preTime = 0x00000000,
    .syncWord = 0x00000000,
    .pPkt = 0, // INSERT APPLICABLE POINTER: (uint8_t*)&xxx
};