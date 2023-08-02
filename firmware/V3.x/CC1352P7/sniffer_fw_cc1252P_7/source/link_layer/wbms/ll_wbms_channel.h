/******************************************************************************
*  Filename:       ll_wbms_channel.h
*
*  Description:    Header file for the wBMS Link Layer channel table functionality
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
#ifndef LL_WBMS_CHANNEL_H
#define LL_WBMS_CHANNEL_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_DATA_CHANNELS               37           // Maximum number of channels

typedef enum LL_ChannelTable
{
    ChannelTable_Config = 0,
    ChannelTable_Data = 1,
} LL_ChannelTable;


//! \brief Select which channel table to be the active 
//!        (Either the configuration or data channel table)
//! 
//! \param[in] LL_ChannelTable
//!            Active channel table
//!
//! \return none
void LLWbms_setActiveChannelTable(LL_ChannelTable);


//! \brief Get next channel to use
//! 
//! \return Next channel
uint8_t LLWbms_channelGetNext(void);


//! \brief Get currently used channel
//! 
//! \return Current channel
uint8_t LLWbms_channelGetCurrent(void);


//! \brief Reset channel state
//! 
//! \return none
void LLWbms_channelReset(void);


//! \brief Update data channel table
//!
//! \param[in] channelTable
//!            Pointer to the new channel hop table
//!
//! \param[in] numChannels
//!            Number of channels in the new channel table
//! 
//! \return none
void LLWbms_channelUpdateDataTable(uint8_t* channelTable, uint8_t numChannels);


//! \brief Lock data channel table for further updates
//!
//! \return none
void LLWbms_channelLockDataTable(void);


//! \brief Unlock data channel table for update
//!
//! \return none
void LLWbms_channelUnlockDataTable(void);

#endif

