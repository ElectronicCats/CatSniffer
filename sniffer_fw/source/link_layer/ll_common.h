/******************************************************************************
*  Filename:       ll_common.h
*
*  Description:    Header file for the common Link Layer functionality
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
#ifndef LL_COMMON_H
#define LL_COMMON_H

#include <stdint.h>

//! \brief Calculate start time of a packet given the timestamp.
//!        The time is an absolute value in microseconds from Radio timer start
//!
//! \param[in] timeStamp
//!            Radio timer timestamp for the packet
//! 
//! \return start time in microseconds
uint32_t LLCommon_calculatePacketStartTime(uint8_t* timeStamp);


//! \brief Convert value of radio timer to microseconds
//!
//! \param[in] timerValue
//!            Value of radio timer
//! 
//! \return time in microseconds
uint32_t LLCommon_convertRadioTimeToUs(uint32_t timerValue);


//! \brief Subtract value from timer value 
//!
//!        This function subtracts a value from timer value in us, taking
//!        timer wraparound into account. Timer values resulting in values
//!        above max timer value is truncated to fit. This function will always return a positive result, 
//!        and should not be used to compare two timer values. 
//!
//! \param[in] timerValue
//!            Value of radio timer in us
//! 
//! \param[in] valueToSubtract
//!            Value to subtract from timer (in us). This value represent either a time interval, or
//!            an event that occured before timerValue. 
//!  
//!
//! \return uint32
//!         New timer value in us
//!
uint32_t LLCommon_subtractTimerValue(uint32_t timerValue, uint32_t valueToSubtract);


//! \brief Add value to timer value 
//!
//!        This function adds a value to timer value in us, taking
//!        timer wraparound into account. Timer values resulting in values
//!        above max timer value is truncated to fit. 
//!
//! \param[in] timerValue
//!            Value of radio timer in us
//! 
//! \param[in] valueToAdd
//!            Value to add to timer (in us)
//!  
//!
//! \return uint32
//!         New timer value in us
//!
uint32_t LLCommon_addTimerValue(uint32_t timerValue, uint32_t valueToAdd);


#endif

