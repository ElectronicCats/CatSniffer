/******************************************************************************
*  Filename:       ll_ble_timer.c
*
*  Description:    Source code for the BLE Link Layer Timer module
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

#include "ll_ble_timer.h"
#include "../../radio_if.h"
#include <xdc/runtime/System.h>


LLBle_timerEventCb LLBle_connEventTimerCbFunc = NULL;
LLBle_timerEventCb LLBle_supervisionTimerCbFunc = NULL;
bool LLBle_isTimerStarted = false;

// Struct definition for BLE timer
typedef struct LLBle_Timer_Obj
{
    LLBle_timerEventCb timerCbFunc;             // Callback function called upon timer expiry
    RadioIf_TimerId radioTimerId;               // Radio timer ID
    uint32_t timeout;                           // Timeout value (absolute value in us since radio timer was started)
    bool isStarted;                             // flag to check if timer if started or not
} LLBle_Timer_Obj;

// Two timers are defined: Connection event timer and supervision timer. The order of these must match the 
// LLBle_TimerId enum since the enum value is used as lookup in this array. 
LLBle_Timer_Obj LLBle_timers[2] = 
{
    { NULL, RADIO_IF_TIMER_0, 0, false },       // Connection event timer
    { NULL, RADIO_IF_TIMER_1, 0, false }        // Supervision timer
};

void LLBleTimer_create(LLBle_TimerId timerId, LLBle_timerEventCb pCbFunc)
{
    LLBle_timers[timerId].timerCbFunc = pCbFunc;
}

void LLBleTimer_setTimeout(LLBle_TimerId timerId, uint32_t timeout)
{
    LLBle_timers[timerId].timeout = timeout;
}


void LLBleTimer_start(LLBle_TimerId timerId)
{
    RadioIf_configureRatTimerEvent(LLBle_timers[timerId].radioTimerId, LLBle_timers[timerId].timeout, LLBle_timers[timerId].timerCbFunc);
    LLBle_timers[timerId].isStarted = true;
}


void LLBleTimer_stop(LLBle_TimerId timerId)
{
    if(LLBle_timers[timerId].isStarted)
    {
        RadioIf_cancelRatTimerEvent((RadioIf_TimerId)timerId);
        LLBle_timers[timerId].isStarted = false;
    }
}


void LLBle_connEventTimerCb(uint32_t ratTime)
{
    if(LLBle_connEventTimerCbFunc != NULL)
    {
        LLBle_connEventTimerCbFunc(ratTime);
    }
}


void LLBle_supervisionTimerCb(uint32_t ratTime)
{
    if(LLBle_supervisionTimerCbFunc)
    {
        LLBle_supervisionTimerCbFunc(ratTime);
    }
}



