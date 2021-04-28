/******************************************************************************
*  Filename:       ll_wbms_channel.c
*
*  Description:    Source code for the wBMS Link Layer channel table functionality
*
* Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
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

#include "ll_wbms_channel.h"
#include <string.h>


// Channel table information struct
typedef struct ChannelTable_Obj
{
    uint8_t currentIndex;                     // Current index in the table
    uint8_t numChannels;                      // Number of channels in the table
    uint8_t* const pHopTable;                 // Pointer to channel table
    bool bLocked;                             // Is channel table locked for updates?
} ChannelTable_Obj;


// Hopping table for configuration channels (scanning/pairing state)
uint8_t LLWbms_configHopTable[] =
{
    37,38,39,
};


// Hopping table for data channels with default values (connected state)
const uint8_t LLWbms_dataHopTableDefault[MAX_DATA_CHANNELS] =
{
    20, 2, 21, 3, 22,  
    4, 23, 5, 24, 6, 
    25, 7, 26, 8, 27, 
    9, 28, 10, 29, 11, 
    30, 12, 31, 13, 32, 
    14, 33, 15, 34, 16, 
    35, 17, 36, 18, 0, 
    19, 1,
};


// Hopping table for data channels (connected state)
uint8_t LLWbms_dataHopTable[MAX_DATA_CHANNELS] = {0};

// Channel table info for configuration channels
ChannelTable_Obj LLWbms_configChannelInfo = { 0, 3, LLWbms_configHopTable, false };

// Channel table info for data channels (connected state)
ChannelTable_Obj LLWbms_dataChannelInfo = { 0, 37, LLWbms_dataHopTable, false };

// Set configuration channel table as active by default
ChannelTable_Obj* pActiveChannelInfo = &LLWbms_configChannelInfo;               

void LLWbms_setActiveChannelTable(LL_ChannelTable channelTable)
{
    if(channelTable == ChannelTable_Config)
    {
        pActiveChannelInfo = &LLWbms_configChannelInfo;
    }
    else
    {
        pActiveChannelInfo = &LLWbms_dataChannelInfo;
    }
}


uint8_t LLWbms_channelGetNext(void)
{ 
    pActiveChannelInfo->currentIndex++;
    if(pActiveChannelInfo->currentIndex >= pActiveChannelInfo->numChannels)
    {
        pActiveChannelInfo->currentIndex = 0;
    }
    
    return pActiveChannelInfo->pHopTable[pActiveChannelInfo->currentIndex];
}


uint8_t LLWbms_channelGetCurrent(void)
{
    uint8_t index = pActiveChannelInfo->currentIndex;
    return pActiveChannelInfo->pHopTable[index];
}


void LLWbms_channelReset(void)
{
    LLWbms_configChannelInfo.currentIndex = 0;
    LLWbms_configChannelInfo.bLocked = false;
    LLWbms_dataChannelInfo.currentIndex = 0;
    LLWbms_dataChannelInfo.numChannels = MAX_DATA_CHANNELS;
    LLWbms_dataChannelInfo.bLocked = false;
    
    // Initialize data hopping table with default values
    memcpy(LLWbms_dataHopTable, LLWbms_dataHopTableDefault, MAX_DATA_CHANNELS);
}


void LLWbms_channelUpdateDataTable(uint8_t* channelTable, uint8_t numChannels)
{
    if(numChannels > MAX_DATA_CHANNELS) return;
    
    // Check if the data table is locked for updates.
    // If it is locked the update request is ignored.
    if(LLWbms_dataChannelInfo.bLocked) return;
        
    // Update data channel table
    memcpy(LLWbms_dataChannelInfo.pHopTable, channelTable, numChannels);  
    LLWbms_dataChannelInfo.numChannels = numChannels;
    LLWbms_dataChannelInfo.currentIndex = 0;
}


void LLWbms_channelLockDataTable(void)
{
    LLWbms_dataChannelInfo.bLocked = true;
}


void LLWbms_channelUnlockDataTable(void)
{
    LLWbms_dataChannelInfo.bLocked = false;
}

