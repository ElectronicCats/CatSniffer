/******************************************************************************
*  Filename:       timestamp.h
*
*  Description:    Header file for timestamp functionality
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
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "data_packet.h"

//! \brief Initialize timestamp module
//!        This function resets the timestamp timer overflow counter.
//!
//! \return None
extern void Timestamp_init(void);


//! \brief Get current value of radio timer overflow counter
//!
//! \return Radio timer overflow counter value
extern uint16_t Timestamp_getRatOverflowCounter(void);


//! \brief Extract timestamp from radio packet
//!
//!        The timestamp bytes of radio packet payload is extracted and removed
//!        from the radio packet payload. The length and data packet timestamp
//!        field are updated.
//!
//! \param[in] packetBuffer
//!            Pointer to buffer for the data packet as read from radio
//!
//! \param[in] length
//!            Length of the payload field of the data packet.
//!
//! \param[in/out] Pointer to data packet to update
//!
//! \return Updated payload length after timestamp bytes are removed
extern uint16_t Timestamp_extractTimestamp(uint8_t* packetBuffer, uint16_t length, DataPacket_Obj* dataPacket);



#endif

