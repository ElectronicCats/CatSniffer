/******************************************************************************
*  Filename:       radio_if_dataqueue.c
*
*  Description:    Source file for configuration of the radio interface data queue.
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


#include "radio_if_dataqueue.h"
#include "phy_manager.h"
#include "common.h"
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_data_entry.h)
#include <ti/drivers/rf/RF.h>


// Defines for the data queue
#define RF_QUEUE_DATA_ENTRY_HEADER_SIZE  8      // Header size of a Generic Data Entry
#define RF_QUEUE_QUEUE_ALIGN_PADDING(length)  (4-((length + RF_QUEUE_DATA_ENTRY_HEADER_SIZE)%4)) // Padding offset

#define RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(numEntries, dataSize, appendedBytes)                                                    \
(numEntries*(RF_QUEUE_DATA_ENTRY_HEADER_SIZE + dataSize + appendedBytes + RF_QUEUE_QUEUE_ALIGN_PADDING(dataSize + appendedBytes)))

#define MAX_LENGTH                      2047    // Max length the radio will accept
#ifndef NUM_DATA_ENTRIES
#define NUM_DATA_ENTRIES                3       // Number of data entries
#endif


// In 802.15.4G mode (using CMD_PROP_RX_ADV) the entries will contain the following:
// 2 header bytes (RF_cmdPropRxAdv.rxConf.bIncludeHdr = 0x1, RF_cmdPropRxAdv.hdrConf.numHdrBits = 16)
// 1 RSSI byte (RF_cmdPropRxAdv.rxConf.bAppendRssi = 0x1)
// 1 status byte (RF_cmdPropRxAdv.rxConf.bAppendStatus = 0x1)
// 4 CRC bytes (RF_cmdPropRxAdv.rxConf.bIncludeCrc = 0x1, RF_cmdPropRxAdv.formatConf.whitenMode = 0x4)
// 4 Timestamp bytes (RF_cmdPropRxAdv.rxConf.bAppendTimestamp = 0x1)
#define NUM_APPENDED_BYTES              12
#define ENTRY_LENGTH_CONFIG_SIZE        2

static dataQueue_t RadioIF_dataQueue;



#if defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC26X0)
// Place the RX buffer in GPRAM for CC13x0 family to save SRAM space
#ifdef __ICCARM__
#pragma location=".gpram"
#elif __TI_COMPILER_VERSION__
#pragma DATA_SECTION(RadioIF_rxDataBuffer, ".gpram_data")
#endif
#endif
// Storage for the data entries
static uint8_t RadioIF_rxDataBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES, MAX_LENGTH, NUM_APPENDED_BYTES)];

// Data entry pointer to keep track of read items
static rfc_dataEntryGeneral_t* RadioIF_readEntry;

// Local functions
static uint8_t RadioIF_createDataQueue(dataQueue_t *dataQueue, uint8_t *buf, uint16_t buf_len, uint8_t numEntries, uint16_t length);
static void RadioIF_nextEntry(void);
static void handle154gPacket(uint8_t* buffer, uint16_t elementLength);
static inline bool isFourOctetFcsType(uint8_t phyHeaderLowByte);
static inline void invertBytes(uint8_t* buffer, uint8_t length);

void RadioIF_dataQueueInit(void)
{
    RadioIF_createDataQueue(&RadioIF_dataQueue, RadioIF_rxDataBuffer, sizeof(RadioIF_rxDataBuffer), NUM_DATA_ENTRIES, MAX_LENGTH + NUM_APPENDED_BYTES);
}


dataQueue_t* RadioIF_dataQueueGet(void)
{
    return &RadioIF_dataQueue;
}


bool RadioIF_hasPacket(void)
{
    return (RadioIF_readEntry->status == DATA_ENTRY_FINISHED);
}


uint16_t RadioIF_takePacket(uint8_t* buffer, uint16_t maxLen)
{
    if(!RadioIF_hasPacket()) return 0;

    uint16_t elementLength = Common_get16BitValueLE(&RadioIF_readEntry->data);

    // Discard packets larger than buffer size
    if(elementLength <= maxLen)
    {
        memcpy(buffer, (uint8_t*)(&RadioIF_readEntry->data + ENTRY_LENGTH_CONFIG_SIZE), elementLength);
        
        if(PhyManager_getRfApi() == PROPRIETARY_15_4_G)
        {
            // Perform required buffer modications for IEEE 802.15.4g packets 
            handle154gPacket(buffer, elementLength);
        }
    }

    // free the entry for radio to use
    RadioIF_nextEntry();

    return elementLength;
}


void RadioIF_resetDataQueue(void)
{
    uint8_t* firstEntry = RadioIF_rxDataBuffer;
    
    // Set read pointer and data queue current entry pointer both to first entry
    RadioIF_dataQueue.pCurrEntry = firstEntry;
    RadioIF_readEntry = (rfc_dataEntryGeneral_t*)firstEntry;
}


//! \brief Free current read entry for radio to use, and move to next entry
//!
//! \return None
void RadioIF_nextEntry(void)
{
  // Set status to pending
  RadioIF_readEntry->status = DATA_ENTRY_PENDING;

  // Move read entry pointer to next entry
  RadioIF_readEntry = (rfc_dataEntryGeneral_t*)RadioIF_readEntry->pNextEntry;
}


//! \brief Create data queue
//!
//! \param[in] dataQueue
//!            Pointer to data queue object
//!
//! \param[in] buf
//!            Buffer for RX data entries
//!
//! \param[in] bufLength
//!            Buffer length
//!
//! \param[in] numEntries
//!            Number of data entries
//!
//! \param[in] length
//!            Length of each data entry
//!
//! \return 0 if configuration is successful or 1 if queue does not fit in buffer.
uint8_t RadioIF_createDataQueue(dataQueue_t *dataQueue, uint8_t *buf, uint16_t bufLength, uint8_t numEntries, uint16_t length)
{
    if (bufLength < (numEntries * (length + RF_QUEUE_DATA_ENTRY_HEADER_SIZE + RF_QUEUE_QUEUE_ALIGN_PADDING(length))))
    {
        // queue does not fit into buffer
        return (1);
    }

    // Padding needed for 4-byte alignment?
    uint8_t pad = 4-((length + RF_QUEUE_DATA_ENTRY_HEADER_SIZE)%4);

    // Configure each data entry
    uint8_t *first_entry = buf;
    int i;
    for (i = 0; i < numEntries; i++)
    {
        buf = first_entry + i * (RF_QUEUE_DATA_ENTRY_HEADER_SIZE + length + pad);
        ((rfc_dataEntry_t*)buf)->status        = DATA_ENTRY_PENDING;        // Pending - starting state
        ((rfc_dataEntry_t*)buf)->config.type   = DATA_ENTRY_TYPE_GEN;       // General Data Entry
        ((rfc_dataEntry_t*)buf)->config.lenSz  = ENTRY_LENGTH_CONFIG_SIZE;  // Two bytes length indicator
        ((rfc_dataEntry_t*)buf)->length        = length;                    // Total length of data field

        ((rfc_dataEntryGeneral_t*)buf)->pNextEntry = &(((rfc_dataEntryGeneral_t*)buf)->data)+length+pad;
    }

    // Make circular Last.Next -> First
    ((rfc_dataEntry_t*)buf)->pNextEntry = first_entry;

    // Create Data Entry Queue and configure for circular buffer Data Entries
    dataQueue->pCurrEntry = first_entry;
    dataQueue->pLastEntry = NULL;

    // Set read pointer to first entry
    RadioIF_readEntry = (rfc_dataEntryGeneral_t*)first_entry;

    return (0);
}


//! \brief Check if a 15.4g packet has 4 or 2 bytes FCS type
//!
//! \param[in] phyHeaderLowByte
//!            Low byte of PHY header as read from RFCore in MSbit format
//!
//! \return true for 4 octet FCS, false for 2 octects FCS
static inline bool isFourOctetFcsType(uint8_t phyHeaderLowByte)
{
    // FCS Type is bit 3 MSB -> bit 4 LSB
    return (~phyHeaderLowByte & 0x10);
}

//! \brief Inverts all the bits for each byte in buffer
//!
//! \param[in] buffer
//!            Pointer to start of data buffer
//!
//! \param[in] length
//!            Buffer length (number of bytes to invert)
static inline void invertBytes(uint8_t* buffer, uint8_t length)
{
    for(int i=0; i<length; i++)
    { 
        // Invert each bit of the value
        uint8_t value = *(buffer + i);
        *(buffer + i) = ~value;
    }
}


//! \brief Perform buffer modifications required for IEEE 802.15.4g packets
//!
//! \param[in] buffer
//!            Pointer to start of data buffer
//!
//! \param[in] elementLength
//!            Length of the queue element for the received packet
static void handle154gPacket(uint8_t* buffer, uint16_t elementLength)
{
    // Invert CRC values for 32 bits CRC mode. For this mode the CRC bytes read from
    // RFCore are inverted. 
    uint8_t phyHdrLowByte = buffer[1];
    if(isFourOctetFcsType(phyHdrLowByte))
    {
        // Check if length of the entry is valid. It must be at least the number of appended bytes.
        // If length is invalid no inversion is done.
        if(elementLength >= NUM_APPENDED_BYTES)
        {
            // Find CRC offset from buffer start 
            uint16_t crcOffset = elementLength - 4 - 1 - 4 - 1; // Subtract the appended 4 bytes CRC, 1 byte RSSI, 4 bytes timestamp and 1 byte RSSI

            // Invert the 4 CRC byte values
            invertBytes(&buffer[crcOffset], 4);
        }   
    }
}






