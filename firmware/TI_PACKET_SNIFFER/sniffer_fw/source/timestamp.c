/******************************************************************************
*  Filename:       timestamp.c
*
*  Description:    Source file timestamp functionality.
*
* Copyright (C) 2017-2018 Texas Instruments Incorporated - http://www.ti.com/
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

#include "timestamp.h"
#include "common.h"
#include "radio_if.h"
#include "radio_if_dataqueue.h"
#include "phy_manager.h"
#include <ti/sysbios/knl/Swi.h>


// Local functions
static uint64_t Timestamp_convertToUs(uint32_t radioTimestamp, uint8_t tickFrequency);
static void Timestamp_setPacketTimestamp(uint64_t timeStampUs, DataPacket_Obj* dataPacket);
static void Timestamp_ratOverflowCb(uint32_t ratCbTime);

static uint64_t Timestamp_getFullRadioTimestamp(uint32_t radioTimeStamp);

// Local variables
// These variables are not static to allow access from test application
uint32_t Timestamp_ratOverflowCounter = 0;              // Keeps track of how many times the radio timer has wrapped around
uint32_t Timestamp_lastRatCbTime = 0;

// defines
#define NUM_TIMESTAMP_BYTES                     6       // 6 Timestamp bytes in data packet
#define NUM_BITS_PER_BYTE                       8

void Timestamp_init(void)
{
    // Reset local variables
    Timestamp_ratOverflowCounter = 0;

    // Register Radio timer callback function
    RadioIF_registerRatOverflowCb(&Timestamp_ratOverflowCb);
}


uint16_t Timestamp_getRatOverflowCounter(void)
{
    return Timestamp_ratOverflowCounter;
}


uint16_t Timestamp_extractTimestamp(uint8_t* packetBuffer, uint16_t length, DataPacket_Obj* dataPacket)
{
    uint32_t radioTimestamp = 0;
    
    if((PhyManager_getRfApi() == IEEE_802_15_4) || (PhyManager_getRfApi() == BLE_5_1M))
    {
        // Handling for IEEE 802.15.4 and BLE 5 RF APIs
        int timestampIndex =  length - RADIO_IF_TIMESTAMP_LEN;
        radioTimestamp = Common_get32BitValueLE(&packetBuffer[timestampIndex]);
    }
    else
    {
        // Handling for the other RF APIs
        int timestampIndex = (length - RADIO_IF_STATUS_LEN - RADIO_IF_TIMESTAMP_LEN);
        uint16_t statusIndex = length - RADIO_IF_STATUS_LEN;
        radioTimestamp = Common_get32BitValueLE(&packetBuffer[timestampIndex]);
        
        // Remove timestamp bytes from data packet payload
        uint8_t status = packetBuffer[statusIndex];
        packetBuffer[timestampIndex] = status; // Overwrites first timestamp byte
    }

    // Set timestamp of data packet
    uint64_t timeStampUs = Timestamp_convertToUs(radioTimestamp, RADIO_TIMER_FREQUENCY_MHZ);
    Timestamp_setPacketTimestamp(timeStampUs, dataPacket);

    // Return updated payload length after timestamp bytes are removed
    return length -= RADIO_IF_TIMESTAMP_LEN;
}


//! \brief Convert the 4 bytes timestamp value set by RF Core to 8 bytes timestamp
//!        in microseconds.
//!
//! \param[in] radioTimestamp
//!            Timestamp set by RF Core
//!
//! \param[in] tickFrequency
//!            Tick frequency of Radio timer
//!
//! \return Timestamp in microseconds
uint64_t Timestamp_convertToUs(uint32_t radioTimestamp, uint8_t tickFrequency)
{
    // Add contribution from the overflow counter
    uint64_t fullRadioTimeStamp = Timestamp_getFullRadioTimestamp(radioTimestamp);

    if(tickFrequency != 0)
    {
        return fullRadioTimeStamp/tickFrequency;
    }

    return 0;
}


//! \brief Set 6 bytes timestamp field in data packet
//!
//! \param[in] timeStampUs
//!            Timestamp in microseconds.
//!
//! \param[out] dataPacket
//!            Pointer to data packet
//!
//! \return None
void Timestamp_setPacketTimestamp(uint64_t timeStampUs, DataPacket_Obj* dataPacket)
{
    // Set the 6 timestamp bytes in data packet in little endian order
    for(int i = 0; i < NUM_TIMESTAMP_BYTES; i++)
    {
        uint8_t mumBitShifts = i * NUM_BITS_PER_BYTE;
        dataPacket->timeStamp[i] = (timeStampUs & ((uint64_t)0xFF << mumBitShifts)) >> mumBitShifts;
    }
}


//! \brief Get full radio timestamp including contributions from overflow counter
//!
//! \param[in] radioTimeStamp
//!            4 bytes timestamp as set by RF Core.
//!
//! \return 8 bytes extended timestamp with same resolution as radio timestamp
uint64_t Timestamp_getFullRadioTimestamp(uint32_t radioTimeStamp)
{
    uint64_t fullRadioTimeStamp = radioTimeStamp;

    // The Radio timer overflow callback may happen just before or just after
    // a packet is received. This must be accounted for when wrap counter
    // contribution is added to the packet's timestamp. 

    // Temporarily disable SWI when the local variables are accessed. These
    // variables are both accessed by the RAT Overflow callback function
    // (which runs in SWI context) and this function.
    uint32_t key = Swi_disable();

    if(Timestamp_ratOverflowCounter > 0)
    {
        uint32_t tempOverflowCounter = Timestamp_ratOverflowCounter;

        if((radioTimeStamp > RADIO_TIMER_THIRD_QUARTER_CYCLE_VALUE) && (Timestamp_lastRatCbTime < RADIO_TIMER_FIRST_QUARTER_CYCLE_VALUE))
        {
            // The packet was received just before timer wrap, but wrap counter has already been increased.
            // We need to subtract 1 from the temporary wrap counter value for this packet. 
            tempOverflowCounter--;
        }
        else if((radioTimeStamp < RADIO_TIMER_FIRST_QUARTER_CYCLE_VALUE) && (Timestamp_lastRatCbTime > RADIO_TIMER_THIRD_QUARTER_CYCLE_VALUE))
        {
            // The packet was received just after timer wrap, but wrap counter has not yet been increased.
            // We need to add 1 to the temporary wrap counter value for this packet. 
            tempOverflowCounter++;
        }

        fullRadioTimeStamp += ((uint64_t)tempOverflowCounter << 32);
    }

    // Re-enable SWI before return from function
    Swi_restore(key);

    return fullRadioTimeStamp;
}


//! \brief Callback function for Radio Timer (RAT) interrupt
//!
//!        This function updates the file scope variables
//!        Timestamp_ratOverflowCounter and Timestamp_lastRatCbTime.
//!
//! \param[in] ratCbTime
//!            The time (value of RAT timer) when the callback happened
//!
//! \return None
void Timestamp_ratOverflowCb(uint32_t ratCbTime)
{
    // If the new RAT Callback time is less than last RAT callback time
    // the RAT timer has wrapped around since last callback. 
    if(ratCbTime < Timestamp_lastRatCbTime)
    {
        Timestamp_ratOverflowCounter++;
    }
    
    Timestamp_lastRatCbTime = ratCbTime;
}




