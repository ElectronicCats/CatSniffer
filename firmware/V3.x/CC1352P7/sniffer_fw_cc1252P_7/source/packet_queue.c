/******************************************************************************
*  Filename:       packet_queue.c
*
*  Description:    Source file for the packet queue module.
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

#include "packet_queue.h"
#include "data_packet.h"

#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/BIOS.h>
#include <stdbool.h>


#define NUM_BUFFER_SLOTS                3

typedef struct Message_Obj
{
    DataPacket_Obj* p; // Pointer to a data packet buffer slot
    uint16_t length;   // Data packet length
} Message_Obj;


static DataPacket_Obj PacketQueue_dataPacket[NUM_BUFFER_SLOTS];

// Queue of pointers to free buffer slots
static Mailbox_Handle freeQueue;

// Queue of pointers to full (occupied) buffer slots
static Mailbox_Handle fullQueue;

static bool PacketQueue_isInitialized = false;


void PacketQueue_init(void)
{    
    if(!PacketQueue_isInitialized)
    {  
        // Create mailbox objects with default error handling and parameters
        freeQueue = Mailbox_create(sizeof(Message_Obj), NUM_BUFFER_SLOTS, NULL, NULL);
        fullQueue = Mailbox_create(sizeof(Message_Obj), NUM_BUFFER_SLOTS, NULL, NULL);
        
        for(int i=0; i<NUM_BUFFER_SLOTS; i++)
        {
            // Enqueue all buffer slot pointers onto the free queue
            Message_Obj msg;
            msg.p = &PacketQueue_dataPacket[i];
            msg.length = 0;
            Mailbox_post(freeQueue, &msg, BIOS_WAIT_FOREVER);
        }
        
        PacketQueue_isInitialized = true;
    }
}


DataPacket_Obj* PacketQueue_allocBufferSlot(void)
{
    // Take a buffer slot pointer from the free queue.
    // This blocks until one is available. 
    Message_Obj msg;
    Mailbox_pend(freeQueue, &msg, BIOS_WAIT_FOREVER);
    return msg.p;
}


void PacketQueue_freeBufferSlot(DataPacket_Obj* dataPacket)
{
    Message_Obj msg;
    msg.p = dataPacket;
    msg.length = 0;
    Mailbox_post(freeQueue, &msg, BIOS_WAIT_FOREVER);
}


void PacketQueue_enqueue(DataPacket_Obj* dataPacket, uint16_t packetLength)
{
    Message_Obj msg;
    msg.p = dataPacket;
    msg.length = packetLength;
    Mailbox_post(fullQueue, &msg, BIOS_WAIT_FOREVER);
}


void PacketQueue_dequeue(DataPacket_Obj** dataPacket, uint16_t* packetLength)
{ 
    Message_Obj msg;
    Mailbox_pend(fullQueue, &msg, BIOS_WAIT_FOREVER);
    *dataPacket = msg.p;
    *packetLength = msg.length;
}































