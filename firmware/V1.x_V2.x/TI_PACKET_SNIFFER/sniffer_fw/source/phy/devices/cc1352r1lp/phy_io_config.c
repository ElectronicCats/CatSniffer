/******************************************************************************
*  Filename:       phy_tables.c
*
*  Description:    Source file for PHY IO configuration
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

#include "../../phy_io_config.h"


/* TI Drivers */
#include <ti/drivers/PIN.h>

/* Board Header files */
#include "Board.h"

/* Pin driver handle */
static PIN_Handle Phy_pinHandle;
static PIN_State Phy_pinState;

#define RF_SWITCH_PIN                           Board_DIO30_RFSW
#define RF_SWITCH_SETTING_2_4_GHZ               0
#define RF_SWITCH_SETTING_1_GHZ                 1

PIN_Config Phy_pinTable[] =
{
    RF_SWITCH_PIN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

void Phy_IoPinInit(void)
{
    Phy_pinHandle = PIN_open(&Phy_pinState, Phy_pinTable);
}

void Phy_configureIoPins(const Phy_RfApi rfApi)
{  
    if(rfApi == PROPRIETARY)
    {
        PIN_setOutputValue(Phy_pinHandle, RF_SWITCH_PIN, RF_SWITCH_SETTING_1_GHZ);
    }
    else if(rfApi == PROPRIETARY_15_4_G)
    {
        PIN_setOutputValue(Phy_pinHandle, RF_SWITCH_PIN, RF_SWITCH_SETTING_1_GHZ);
    }
    else if(rfApi == IEEE_802_15_4)
    {
        PIN_setOutputValue(Phy_pinHandle, RF_SWITCH_PIN, RF_SWITCH_SETTING_2_4_GHZ);
    }
    else if(rfApi == BLE_5_1M)
    {
        PIN_setOutputValue(Phy_pinHandle, RF_SWITCH_PIN, RF_SWITCH_SETTING_2_4_GHZ);
    }
}
