/******************************************************************************
*  Filename:       ll_manager.c
*
*  Description:    Source code for Link Layer Manager
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

#include "ll_manager.h"
#include "ll_default.h"
#include "ble/ll_ble.h"
#include "../phy/phy_manager.h"

// The currently used Link Layer type (BLE or Default)
static LL_Type LLManager_currentLLType = DEFAULT;

const LL_Obj LL_table[] =
{
    { LLBle_processPacket, LLBle_reset },               // BLE Link Layer
    { LLDefault_processPacket, LLDefault_reset }        // Default Link Layer
};

LL_Type LL_getLlType(void)
{
    return LLManager_currentLLType;
}


void LL_setLLType(LL_Type llType)
{
    LLManager_currentLLType = llType;
    
    // Reset the Link Layer before use
    LL_table[LLManager_currentLLType].pResetFunc();
}


void LL_init(void)
{
    if(PhyManager_isBlePhySupported())
    {
        // Only the BLE link layer have an init function
        LLBle_init();
    }
}



