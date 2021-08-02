/******************************************************************************
*  Filename:       packet_queue.h
*
*  Description:    Header file for the packet queue module.
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
#ifndef PACKET_QUEUE_H
#define PACKET_QUEUE_H

#include "data_packet.h"


//! \brief Initialize the packet queue
//!
//! \return None
extern void PacketQueue_init(void);


//! \brief Allocates a packet buffer slot
//!        This function blocks until a slot is available. 
//!
//! \return Pointer to the allocated buffer slot memory
extern DataPacket_Obj* PacketQueue_allocBufferSlot(void);


//! \brief Free a packet buffer slot
//!
//! \param[in] dataPacket
//!            Pointer to the buffer slot to free
//!
//! \return none
extern void PacketQueue_freeBufferSlot(DataPacket_Obj* dataPacket);


//! \brief Enqueue a packet on the queue
//!
//! \param[in] dataPacket
//!            Pointer to the packet buffer slot
//!
//! \param[in] packetLength 
//!            Length of the packet.
//!
//! \return none
extern void PacketQueue_enqueue(DataPacket_Obj* dataPacket, uint16_t packetLength);


//! \brief Dequeue (take) a packet from the queue
//!
//! \param[in] dataPacket
//!            Pointer to the packet buffer slot
//!
//! \param[in] packetLength 
//!            Length of the packet.
//!
//! \return none
extern void PacketQueue_dequeue(DataPacket_Obj** dataPacket, uint16_t* packetLength);


#endif

