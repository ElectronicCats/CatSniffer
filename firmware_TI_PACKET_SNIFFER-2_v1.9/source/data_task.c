/******************************************************************************
*  Filename:       data_task.c
*
*  Description:    Source file for the Data Task. This task process data
*                  received from the radio.
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

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/System.h>
#include "host_if.h"
#include "radio_if.h"
#include "radio_if_dataqueue.h"
#include "packet_handler.h"
#include "data_packet.h"
#include "timestamp.h"
#include "packet_queue.h"
#include "ll_manager.h"
#include <ti/drivers/PIN.h>
#include "Board.h"

#include <stdbool.h>

static void DataTask_rfCoreEventCb(uint32_t rfCoreEvent);
static void DataTask_initEventHandler(void);
static void DataTask_processPacket(void);
static void DataTask_handleBufferOverflow(void);

#define EVENT_ID_RX_ENTRY_DONE                  Event_Id_01
#define EVENT_ID_RX_BUFFER_FULL                 Event_Id_02
static Event_Handle DataTask_EventHandle;


//! \brief Data task function
//!        The arguments are not used.
//!
//!        This task blocks waiting for either 
//!        - a new packet available in the radio interface data queue, 
//!        - or an event triggered when the RX buffers are full. 
//!        When a new packet is available it takes the packet
//!        out of the queue, processes the packet and forwards it to the host.
//!
void dataTask(UArg a0, UArg a1)
{
    // Initialize the modules to be used by this task
    HostIF_init();
    Timestamp_init();
    PacketQueue_init();
    LL_init();
        
    DataTask_initEventHandler();
    
    while (true)
    {
        // Event_pend blocks until one or more of the events are received 
        UInt events = Event_pend(DataTask_EventHandle, Event_Id_NONE, EVENT_ID_RX_ENTRY_DONE | EVENT_ID_RX_BUFFER_FULL, BIOS_WAIT_FOREVER);
        
        // Process the received packets
        if(events & EVENT_ID_RX_ENTRY_DONE)
        {
            // Process all packets in the RX packet queue
            while(RadioIF_hasPacket())
            {             
                DataTask_processPacket();          
            }
        }
        
        if(events & EVENT_ID_RX_BUFFER_FULL)
        {
            // Process all packets in the RX packet queue
            while(RadioIF_hasPacket())
            {
                DataTask_processPacket();
            }
            
            DataTask_handleBufferOverflow();
        }
    }
}



//! \brief Process a data packet
//!
//!       This function first allocates a packet buffer slot on the Host IF 
//!       queue. Then it reads the packet from the RF Core queue, processes
//!       the timestamp field and formats it according to the Host IF format. 
//!       Finally, it enqueues the packet on the Host IF packet queue. This will
//!       signal to the Host IF packet that a new packet is ready to be sent on
//!       the Host IF interface. 
void DataTask_processPacket(void)
{
    // Allocate a buffer slot for the new packet
    DataPacket_Obj* p = PacketQueue_allocBufferSlot();
    
    // Disable task switch to make sure the processing of one packet is not interrupted.
    int key = Task_disable(); 
    
    // Take a packet from the RX packet queue
    uint16_t len = RadioIF_takePacket((uint8_t*)p->payload, sizeof(p->payload));
    
    len = Timestamp_extractTimestamp((uint8_t*)p->payload, len, p);
    
    // Process packet by the current Link Layer type
    LL_Type llType = LL_getLlType();
    len = LL_table[llType].pProcessPacketFunc(p->payload, len);
   
    // Do packet processing
    uint16_t payloadLength = PacketHandler_processPacket(p, len);

    uint16_t packetLength = PACKET_NON_PAYLOAD_LENGTH + payloadLength;
    Task_restore(key);

    // Enqueue packet on the Host IF queue
    PacketQueue_enqueue(p, packetLength);
}


//! \brief Handle a buffer overflow.
//!
//!       This function handles an RX buffer overflow and sends an error packet
//!       on the Host interface. 
static void DataTask_handleBufferOverflow(void)
{
    // Allocate a buffer slot for an error packet
    DataPacket_Obj* p = PacketQueue_allocBufferSlot();
    
    PacketHandler_processError((uint8_t*)p, RADIO_IF_STATUS_RX_BUFFER_OVERFLOW);
    RadioIF_handleBufferOverflow();
    
    // Enqueue error packet on the Host IF queue
    PacketQueue_enqueue(p, sizeof(ErrorReportPacket_Obj));
}


//! \brief Initialize event handling
//! 
void DataTask_initEventHandler(void)
{
    // Create event with default parameters 
    DataTask_EventHandle = Event_create(NULL, NULL);

    if (DataTask_EventHandle == NULL)
    {
        System_abort("Event create failed");
    }

    // Register Radio timer callback function
    RadioIF_registerRfCoreEventCb(&DataTask_rfCoreEventCb);
}


//! \brief RF Core event callback handler. 
//! 
//!        This function handles two events from RF Core: RX_ENTRY_DONE and
//!        RX_BUFFER_FULL events. 
//! 
//! \param[in] rfCoreEvent
//!            RF Core event number
void DataTask_rfCoreEventCb(uint32_t rfCoreEvent)
{
    if(rfCoreEvent & RX_ENTRY_DONE_EVENT)
    {
        Event_post(DataTask_EventHandle, EVENT_ID_RX_ENTRY_DONE);
    }
    
    if(rfCoreEvent & RX_BUFFER_FULL_EVENT)
    {
        Event_post(DataTask_EventHandle, EVENT_ID_RX_BUFFER_FULL);
    }
}

