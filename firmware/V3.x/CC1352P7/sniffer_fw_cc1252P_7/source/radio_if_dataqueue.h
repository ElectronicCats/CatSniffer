/******************************************************************************
*  Filename:       radio_if_dataqueue.h
*
*  Description:    Header file for configuration of the radio interface data queue.
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
#ifndef RADIO_IF_DATAQUEUE_H
#define RADIO_IF_DATAQUEUE_H


/* driverlib header files */
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_data_entry.h)
#include <stdbool.h>

// Length of field added by Rf Core
#define RADIO_IF_RSSI_LEN                               1
#define RADIO_IF_TIMESTAMP_LEN                          4
#define RADIO_IF_STATUS_LEN                             1

//! \brief Initialize the radio data queue
//!
//! \return None
extern void RadioIF_dataQueueInit(void);


//! \brief Get pointer to radio data queue
//!
//! \return Pointer to data queue
extern dataQueue_t* RadioIF_dataQueueGet(void);


//! \brief Check if the data queue has received one or more packets
//!
//! \return true if one or more packets are received, false otherwise.
extern bool RadioIF_hasPacket(void);


//! \brief Take a packet from the data queue
//!
//!        This function will copy data from the data queue to the buffer given
//!        as parameter.
//!
//! \param[in] buffer
//!            Pointer to buffer where the radio packet will be copied. This
//!            buffer must be allocated by the caller.
//!
//! \param[in] maxLen
//!            Max number of bytes to copy.
//!
//! \return number of bytes copied to the data buffer.
extern uint16_t RadioIF_takePacket(uint8_t* buffer, uint16_t maxLen);


//! \brief Resets data queue internally used read and write pointers
//!
//! \return None
extern void RadioIF_resetDataQueue(void);


#endif

