/******************************************************************************
*  Filename:       ll_common.c
*
*  Description:    Source code for common Link Layer functionality
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

#include "ll_common.h"
#include "../common.h"
#include "../radio_if.h"


uint32_t LLCommon_calculatePacketStartTime(uint8_t* timeStamp)
{   
    // convert Radio Timer (RAT) timestamp to microseconds
    uint32_t ratTime = Common_get32BitValueLE(timeStamp);
    uint32_t time = ratTime/RADIO_TIMER_TICKS_PER_MICROSECOND;
    
    return time;
}


uint32_t LLCommon_convertRadioTimeToUs(uint32_t timerValue)
{   
    // convert Radio Timer (RAT) timestamp to microseconds
    uint32_t time = timerValue/RADIO_TIMER_TICKS_PER_MICROSECOND;
    
    return time;
}


uint32_t LLCommon_subtractTimerValue(uint32_t timerValue, uint32_t valueToSubtract)
{
    return (timerValue - valueToSubtract) & RADIO_TIMER_MAX_VALUE_US;
}


uint32_t LLCommon_addTimerValue(uint32_t timerValue, uint32_t valueToAdd)
{
    return (timerValue + valueToAdd) & RADIO_TIMER_MAX_VALUE_US;
}
