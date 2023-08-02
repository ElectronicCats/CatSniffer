/******************************************************************************
*  Filename:       ll_timer.c
*
*  Description:    Source code for the Link Layer Timer module
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

#include "ll_timer.h"
#include "../radio_if.h"
#include <xdc/runtime/System.h>


// Struct definition for BLE timer
typedef struct LLTimer_Timer_Obj
{
    LLTimer_timerEventCb timerCbFunc;           // Callback function called upon timer expiry
    RadioIf_TimerId radioTimerId;               // Radio timer ID
    uint32_t timeout;                           // Timeout value (absolute value in us since radio timer was started)
    bool isStarted;                             // flag to check if timer if started or not
} LLTimer_Timer_Obj;

// Two timers are defined: Connection event timer and supervision timer. The order of these must match the 
// LLBle_TimerId enum since the enum value is used as lookup in this array. 
LLTimer_Timer_Obj LLTimer_timers[2] = 
{
    { NULL, RADIO_IF_TIMER_0, 0, false },       // Connection event timer
    { NULL, RADIO_IF_TIMER_1, 0, false }        // Supervision timer
};


void LLTimer_registerCb(LLTimer_TimerId timerId, LLTimer_timerEventCb pCbFunc)
{
    LLTimer_timers[timerId].timerCbFunc = pCbFunc;
}


void LLTimer_setTimeout(LLTimer_TimerId timerId, uint32_t timeout)
{
    LLTimer_timers[timerId].timeout = timeout;
}


void LLTimer_start(LLTimer_TimerId timerId)
{
    RadioIf_configureRatTimerEvent(LLTimer_timers[timerId].radioTimerId, LLTimer_timers[timerId].timeout, LLTimer_timers[timerId].timerCbFunc);
    LLTimer_timers[timerId].isStarted = true;
}


void LLTimer_stop(LLTimer_TimerId timerId)
{
    if(LLTimer_timers[timerId].isStarted)
    {
        RadioIf_cancelRatTimerEvent((RadioIf_TimerId)timerId);
        LLTimer_timers[timerId].isStarted = false;
    }
}

