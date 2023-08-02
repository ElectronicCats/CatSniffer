/******************************************************************************
*  Filename:       radio_if.h
*
*  Description:    Header file for configuration of the radio interface.
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
#ifndef RADIO_IF_H
#define RADIO_IF_H

#include <stdint.h>
#include <stdbool.h>

#define RADIO_TIMER_MAX_VALUE_US                            0x3FFFFFFF           // Maximum value of timer in us     
#define RADIO_TIMER_FREQUENCY_MHZ                           4 // 4 MHz
#define RADIO_TIMER_TICKS_PER_MICROSECOND                   RADIO_TIMER_FREQUENCY_MHZ
#define RADIO_TIMER_HALF_CYCLE_VALUE                        0x80000000
#define RADIO_TIMER_FIRST_QUARTER_CYCLE_VALUE               0x40000000
#define RADIO_TIMER_THIRD_QUARTER_CYCLE_VALUE               0xC0000000


// Selected IRQ event numbers from rf_mailbox
#define RX_ENTRY_DONE_EVENT                     1
#define RX_BUFFER_FULL_EVENT                    2


// Radio Interface status
typedef enum RadioIfStatus
{
    RADIO_IF_STATUS_OK,
    RADIO_IF_STATUS_RX_BUFFER_OVERFLOW,
} RadioIfStatus;

// Radio Timer channel
typedef enum RadioIf_TimerId
{
    RADIO_IF_TIMER_0 = 0,
    RADIO_IF_TIMER_1 = 1,
} RadioIf_TimerId;

//! Typedef for RAT timer overflow callback function
//! The ratCbTime is the time when the callback happened
typedef void (*ratOverflowCb)(uint32_t ratCbTime);

typedef void (*rxEntryDoneCb)(void);
typedef void (*rfCoreEventCb)(uint32_t rfCoreEvent);
typedef void (*ratTimerEventCb)(uint32_t ratTime);

//! \brief Initialize the radio interface
//!
//! \return None
extern void RadioIF_init(void);


//! \brief Start radio receive mode
//!
//! \return None
extern void RadioIF_startRx(void);


//! \brief Stop radio receive mode
//!
//! \return None
extern void RadioIF_stopRx(void);


//! \brief Get status of the radio interface.
//!
//! \return One of the values defined by RadioIfStatus
extern uint8_t RadioIF_getStatus(void);


//! \brief Handle an RX buffer overflow situation
//!
//! \return None
extern void RadioIF_handleBufferOverflow(void);


//! \brief Set frequency value
//!        The radio shall be stopped before calling this function. The new
//!        frequency takes effect when the radio is restarted after calling
//!        this function.
//!
//! \param[in] frequency
//!            Frequency in MHz
//!
//! \param[in] fractFrequency
//!            Fractional frequency in 1 MHz/65536 fractions.
//!
//! \return None
extern void RadioIF_setFrequency(uint16_t frequency, uint16_t fractFrequency);


//! \brief Initialize radio for packet transmit
//!
//! \return None
extern void RadioIF_initTx(void);


//! \brief Send packet.
//!        The radio shall be stopped before calling this function.
//!
//! \param[in] packet
//!            Pointer to packet data buffer
//!
//! \param[in] packetLength
//!            Length of packet data buffer
//!
//! \return None
extern void RadioIF_sendPacket(uint8_t* packet, uint16_t packetLength);


//! \brief Deinitialize/disable radio for packet transmit
//!        This function does clean up and close RF driver after packet transmit.
//!
//! \return None
extern void RadioIF_disableTx(void);


//! \brief Register function for Radio timer overflow callback
//!
//!        The callback function will be called two timer per timer cycle
//!        (When timer reaches half cycle value and when it wraps around)
//!
//! \return None
extern void RadioIF_registerRatOverflowCb(ratOverflowCb func);


extern void RadioIF_registerRxEntryDoneCb(rxEntryDoneCb func);
extern void RadioIF_registerRfCoreEventCb(rfCoreEventCb func);

//! \brief Set PHY (RF API and PHY setting)
//!
//! \param[in] phyNumber
//!            The number of the PHY to use. This number decodes into an RF API and a PHY setting. 
//!            (Examples of RF API are Proprietary, Proprietary in IEEE 802.15.4g mode, and 
//!             IEEE 802.15.4) 
//!
//! \return true if phyNumber is valid, or false otherwise
extern bool RadioIf_setPhy(uint8_t phyNumber);


//! \brief Stop (cancel) RX command
//!
//! \return none
extern void RadioIf_stopRxCmd(void);


//! \brief Start (Restart) RX command
//!
//!        The radio must have been initalized first before using this function (That is, Setup and FS commands must have run). 
//!        The functions RadioIf_stopRxCmd and RadioIf_startRxCmd can be used after radio is started or stopped with the functions
//!        RadioIF_startRx and RadioIF_stopRx.
//!         
//!
//! \return none
extern void RadioIf_startRxCmd();


//! \brief Configure RAT timer event 
//!
//! \param[in] timerId
//!            timer id (0 or 1)
//!
//! \param[in] timeout
//!            Absolute value of timeout in microseconds (that, microseconds since RAT timer value was 0) 
//!
//! \param[in] func
//!            Callback function at timer event 
//!
//! \return none
extern void RadioIf_configureRatTimerEvent(RadioIf_TimerId timerId, uint32_t timeout, ratTimerEventCb func);



//! \brief Cancel a previously configured RAT timer event 
//!
//! \param[in] timerId
//!            timer id (0 or 1)
//!
//! \return none
extern void RadioIf_cancelRatTimerEvent(RadioIf_TimerId timerId);


//! \brief Run CMD_FS 
//!
//! \return none
extern void RadioIf_runFsCmd(void);

#endif

