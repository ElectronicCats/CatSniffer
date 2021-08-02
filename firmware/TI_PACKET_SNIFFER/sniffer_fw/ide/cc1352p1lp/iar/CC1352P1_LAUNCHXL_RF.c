/*
 * Copyright (c) 2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/rf/RF.h>

#include "Board.h"

/*
 *  Board-specific callback function to set the correct antenna path.
 *
 *  This function is called by the RF driver on global driver events.
 *  It contains a default implementation to set the correct antenna path.
 *  This function is defined in the file CC1352P1_LAUNCHXL_fxns.c
 */
extern void rfDriverCallback(RF_Handle client, RF_GlobalEvent events, void* arg);

const RFCC26XX_HWAttrsV2 RFCC26XX_hwAttrs = {
    .hwiPriority        = ~0,     /* Lowest HWI priority */
    .swiPriority        = 0,      /* Lowest SWI priority */
    .xoscHfAlwaysNeeded = true,   /* Keep XOSC dependency while in standby */

    /* Register the board specific callback */
    .globalCallback     = &rfDriverCallback,

    /* Subscribe the callback to both events */
    .globalEventMask    = RF_GlobalEventRadioSetup | RF_GlobalEventRadioPowerDown
};

/*
 * ======== Antenna switching ========
 */
static PIN_Handle antennaPins;
static PIN_State antennaState;
static bool initialized;

void initAntennaSwitch()
{
    PIN_Config antennaConfig[] = {
        Board_RF_24GHZ | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,      /* Path disabled */
        Board_RF_HIGH_PA | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,    /* Path disabled */
        Board_RF_SUB1GHZ | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,    /* Path disabled */
        PIN_TERMINATE
    };
    antennaPins = PIN_open(&antennaState, antennaConfig);
    initialized = true;
}

/*
 * ======== CC1352P1_LAUNCHXL_rfDriverCallback ========
 * Sets up the antenna switch depending on the current PHY configuration.
 * Truth table:
 *
 * Path        DIO28 DIO29 DIO30
 * =========== ===== ===== =====
 * Off         0     0     0
 * Sub-1 GHz   0     0     1
 * 2.4 GHz     1     0     0
 * 20 dBm TX   0     1     0
 */
void rfDriverCallback(RF_Handle client, RF_GlobalEvent events, void *arg)
{
    /* Local variable. */
    bool    sub1GHz   = false;
    uint8_t loDivider = 0;

    if (!initialized) {
        initAntennaSwitch();
    }

    /* Switch off all paths first. Needs to be done anyway in every sub-case below. */
    PINCC26XX_setOutputValue(Board_RF_24GHZ, 0);
    PINCC26XX_setOutputValue(Board_RF_HIGH_PA, 0);
    PINCC26XX_setOutputValue(Board_RF_SUB1GHZ, 0);

    if (events & RF_GlobalEventRadioSetup) {
        /* Decode the current PA configuration. */
        RF_TxPowerTable_PAType paType = (RF_TxPowerTable_PAType)RF_getTxPower(client).paType;

        /* Decode the generic argument as a setup command. */
        RF_RadioSetup* setupCommand = (RF_RadioSetup*)arg;

        switch (setupCommand->common.commandNo) {
            case (CMD_RADIO_SETUP):
            case (CMD_BLE5_RADIO_SETUP):
                    loDivider = RF_LODIVIDER_MASK & setupCommand->common.loDivider;

                    /* Sub-1GHz front-end. */
                    if (loDivider != 0) {
                        sub1GHz = true;
                    }
                    break;
            case (CMD_PROP_RADIO_DIV_SETUP):
                    loDivider = RF_LODIVIDER_MASK & setupCommand->prop_div.loDivider;

                    /* Sub-1GHz front-end. */
                    if (loDivider != 0) {
                        sub1GHz = true;
                    }
                    break;
            default:break;
        }

        if (sub1GHz) {
            /* Sub-1 GHz */
            if (paType == RF_TxPowerTable_HighPA) {
                /* PA enable --> HIGH PA
                 * LNA enable --> Sub-1 GHz
                 */
                PINCC26XX_setMux(antennaPins, Board_RF_24GHZ, PINCC26XX_MUX_GPIO);
                /* Note: RFC_GPO3 is a work-around because the RFC_GPO1 (PA enable signal) is sometimes not
                         de-asserted on CC1352 Rev A. */
                PINCC26XX_setMux(antennaPins, Board_RF_HIGH_PA, PINCC26XX_MUX_RFC_GPO3);
                PINCC26XX_setMux(antennaPins, Board_RF_SUB1GHZ, PINCC26XX_MUX_RFC_GPO0);
            } else {
                /* RF core active --> Sub-1 GHz */
                PINCC26XX_setMux(antennaPins, Board_RF_24GHZ, PINCC26XX_MUX_GPIO);
                PINCC26XX_setMux(antennaPins, Board_RF_HIGH_PA, PINCC26XX_MUX_GPIO);
                PINCC26XX_setMux(antennaPins, Board_RF_SUB1GHZ, PINCC26XX_MUX_GPIO);
                PINCC26XX_setOutputValue(Board_RF_SUB1GHZ, 1);
            }
        } else {
            /* 2.4 GHz */
            if (paType == RF_TxPowerTable_HighPA)
            {
                /* PA enable --> HIGH PA
                 * LNA enable --> 2.4 GHz
                 */
                PINCC26XX_setMux(antennaPins, Board_RF_24GHZ, PINCC26XX_MUX_RFC_GPO0);
                /* Note: RFC_GPO3 is a work-around because the RFC_GPO1 (PA enable signal) is sometimes not
                         de-asserted on CC1352 Rev A. */
                PINCC26XX_setMux(antennaPins, Board_RF_HIGH_PA, PINCC26XX_MUX_RFC_GPO3);
                PINCC26XX_setMux(antennaPins, Board_RF_SUB1GHZ, PINCC26XX_MUX_GPIO);
            } else {
                /* RF core active --> 2.4 GHz */
                PINCC26XX_setMux(antennaPins, Board_RF_24GHZ, PINCC26XX_MUX_GPIO);
                PINCC26XX_setMux(antennaPins, Board_RF_HIGH_PA, PINCC26XX_MUX_GPIO);
                PINCC26XX_setMux(antennaPins, Board_RF_SUB1GHZ, PINCC26XX_MUX_GPIO);
                PINCC26XX_setOutputValue(Board_RF_24GHZ, 1);
            }
        }
    } else {
        /* Reset the IO multiplexer to GPIO functionality */
        PINCC26XX_setMux(antennaPins, Board_RF_24GHZ, PINCC26XX_MUX_GPIO);
        PINCC26XX_setMux(antennaPins, Board_RF_HIGH_PA, PINCC26XX_MUX_GPIO);
        PINCC26XX_setMux(antennaPins, Board_RF_SUB1GHZ, PINCC26XX_MUX_GPIO);
    }
}
