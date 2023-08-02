/******************************************************************************
*  Filename:       radio_if.c
*
*  Description:    Source file for configuration of the radio interface.
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

#include "radio_if.h"
#include "radio_if_dataqueue.h"
#include "phy_manager.h"

// TI-RTOS radio driver
#include <ti/drivers/rf/RF.h>

// TI-RTOS
#include <ti/sysbios/hal/Hwi.h>

// Default PHY number (the PHY number selected at startup)
#define DEFAULT_PHY_NUMBER       0

// Struct definition for Radio IF general timer
typedef struct RadioIf_Timer_Obj
{
    ratTimerEventCb timerCbFunc;        // Callback function called upon timer expiry
    RF_RatHandle handle;                // RF driver Radio timer handle 
    RF_RatSelectChannel channel;        // RF driver Radio timer channel
} RadioIf_Timer_Obj;


// Two general radio timers are defined. The order of these must match the 
// RadioIfTimerChannel enum since the enum value is used as lookup in this array. 
RadioIf_Timer_Obj RadioIf_timers[] = 
{
    { NULL, -1, RF_RatChannel0 },                       // Timer 0
    { NULL, -1, RF_RatChannel1 }                        // Timer 1
};

static uint8_t RadioIF_numBufferOverflow = 0;
static RF_Object RadioIF_RF_Obj;
static RF_Handle RadioIF_Rf_Handle;
static RF_CmdHandle RadioIF_Cmd_Handle;
static RF_RatHandle RadioIF_ratOverFlowEventHandle = -1;
static ratOverflowCb RadioIF_ratCbFunc = NULL;
static rfCoreEventCb RadioIF_rfCoreEventCbFunc = NULL;

// Local functions
static void RadioIF_ratCallback(RF_Handle h, RF_RatHandle rh, RF_EventMask e, uint32_t timeout);
static void RadioIF_flushDataQueue(void);
static void RadioIF_enableRadioTimerOverflowCallback(void);
static void RadioIF_startRfCoreRx(void);
static void RadioIF_rfCoreEventCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);
static void RadioIF_registerRfCoreEventCbApiSpecific(rfCoreEventCb func);


void RadioIF_init(void)
{
    RadioIF_dataQueueInit();
    PhyManager_init();
    PhyManager_setPhy(DEFAULT_PHY_NUMBER);
}


void RadioIF_startRx(void)
{
    RadioIF_flushDataQueue(); 
    RadioIF_startRfCoreRx();
    RadioIF_enableRadioTimerOverflowCallback();
}


void RadioIF_stopRx(void)
{
    // Cancel and flush CMD_PROP_RX
    RF_flushCmd(RadioIF_Rf_Handle, RF_CMDHANDLE_FLUSH_ALL, 0);
    
    // Disable RAT timer channels. This is done to avoid callbacks
    // after RF driver is closed. 
    RF_ratDisableChannel(RadioIF_Rf_Handle, RF_RatChannel0);
    RF_ratDisableChannel(RadioIF_Rf_Handle, RF_RatChannel1);
    RF_ratDisableChannel(RadioIF_Rf_Handle, RF_RatChannel2);
    
    // Close RF driver
    RF_close(RadioIF_Rf_Handle);

    // Clear RX queue entries and reset queue
    RadioIF_flushDataQueue();
}


uint8_t RadioIF_getStatus(void)
{
    uint8_t status = RADIO_IF_STATUS_OK;

    if(RadioIF_numBufferOverflow>0)
    {
        status = RADIO_IF_STATUS_RX_BUFFER_OVERFLOW;
    }

    return status;
}


//! \brief Clears all entries in the radio data queue and resets 
//!        read and write pointers.
//!
//! \return None
void RadioIF_flushDataQueue(void)
{
    RadioIF_resetDataQueue();
}


void RadioIF_handleBufferOverflow(void)
{
    // Make sure this function is not interrupted by HWI or task switch
    int key = Hwi_disable();
    RadioIF_flushDataQueue();

    // Restart the RF Core in RX
    RadioIf_startRxCmd();

    // Update status
    RadioIF_numBufferOverflow=0;
    Hwi_restore(key);
}


void RadioIF_setFrequency(uint16_t frequency, uint16_t fractFrequency)
{
    PhyManager_setFrequency(frequency, fractFrequency);
}
    

void RadioIF_initTx(void)
{
    RF_Params rfParams;
    RF_Params_init(&rfParams);

    // Open RF driver
    RadioIF_Rf_Handle = RF_open(&RadioIF_RF_Obj, PhyManager_getRfMode(), PhyManager_getSetupCmd(), &rfParams);

    // Issue the RF Core FS command
    RF_postCmd(RadioIF_Rf_Handle, PhyManager_getFsCmd(), RF_PriorityNormal, NULL, 0);
}


void RadioIF_sendPacket(uint8_t* packet, uint16_t packetLength)
{
    PhyManager_configureTxCommand(packet, packetLength);
    
    // For IEEE 802.15.4 we must subscribe to the RF_EventLastFGCmdDone event, otherwise the function hangs
    RF_EventMask result = RF_runCmd(RadioIF_Rf_Handle, PhyManager_getTxCmd(), RF_PriorityNormal, NULL, (RF_EventLastCmdDone | RF_EventLastFGCmdDone));
}


void RadioIF_disableTx(void)
{
    // Cancel and flush CMD_PROP_RX
    RF_flushCmd(RadioIF_Rf_Handle, RF_CMDHANDLE_FLUSH_ALL, 0);
    
    // Close RF driver
    RF_close(RadioIF_Rf_Handle);
}


//! \brief Callback function called upon RAT events
//!
//! \return None
void RadioIF_ratCallback(RF_Handle h, RF_RatHandle rh, RF_EventMask e, uint32_t timeout)
{
    if(e & RF_EventRatCh)
    {
        uint32_t currentTime = RF_getCurrentTime();
        if(RadioIF_ratCbFunc && (rh == RadioIF_ratOverFlowEventHandle))
        {
            RadioIF_ratCbFunc(currentTime);
          
            // Re-enable overflow interrupt
            RadioIF_enableRadioTimerOverflowCallback();
        }
                
        if(rh == RadioIf_timers[0].handle)
        {
            RadioIf_timers[0].timerCbFunc(currentTime);
        }
        
        if(rh == RadioIf_timers[1].handle)
        {
            RadioIf_timers[1].timerCbFunc(currentTime);
        }
    }
}


void RadioIF_enableRadioTimerOverflowCallback(void)
{
    uint32_t compareTime = 0;
    uint32_t currentTime = RF_getCurrentTime();
    
    // Set compareTime to next MSB number of RAT timer with the rest of bytes 0x0
    // (0x10000000, 0x20000000, 0x30000000, ....)
    compareTime = (currentTime & 0xF0000000) + 0x10000000;
    
    RF_RatConfigCompare ratCompareCfg;
    ratCompareCfg.callback = &RadioIF_ratCallback;
    ratCompareCfg.channel = RF_RatChannel2;
    ratCompareCfg.timeout = compareTime;

    // Configure RAT compare callback at timer overflow value
    RadioIF_ratOverFlowEventHandle = RF_ratCompare(RadioIF_Rf_Handle, &ratCompareCfg, NULL);
}


void RadioIF_registerRatOverflowCb(ratOverflowCb func)
{
    RadioIF_ratCbFunc = func;
}


bool RadioIf_setPhy(uint8_t phyNumber)
{
    return PhyManager_setPhy(phyNumber);
}


//! \brief Start RF Core in RX
//!
//! \return None
void RadioIF_startRfCoreRx(void)
{
    RF_Params rfParams;
    RF_Params_init(&rfParams);

    // Open RF driver
    RadioIF_Rf_Handle = RF_open(&RadioIF_RF_Obj, PhyManager_getRfMode(), PhyManager_getSetupCmd(), &rfParams);
  
    // Issue CMD_FS
    RF_runCmd(RadioIF_Rf_Handle, PhyManager_getFsCmd(), RF_PriorityNormal, NULL, 0);

    // Issue Rf Core RX command to start RX
    RadioIF_Cmd_Handle = RF_postCmd(RadioIF_Rf_Handle, PhyManager_getRxCmd(), RF_PriorityNormal, &RadioIF_rfCoreEventCallback, IRQ_RX_BUF_FULL | IRQ_RX_ENTRY_DONE);
}


extern void RadioIF_registerRfCoreEventCb(rfCoreEventCb func)
{
    RadioIF_registerRfCoreEventCbApiSpecific(func);
}


//! \brief Callback function called upon RF Core events
//!
//! \return None
void RadioIF_rfCoreEventCallback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    uint32_t events = 0;
    
    if(e & RF_EventRxEntryDone)
    {
        events |= RX_ENTRY_DONE_EVENT;
    }
    
    if(e & RF_EventRxBufFull)
    {
        events |= RX_BUFFER_FULL_EVENT;
        if(PhyManager_getRfApi() == IEEE_802_15_4)
        {
            // Abort RX command (For IEEE 15.4 API RX does not end automatically on this event)
            RF_cancelCmd(RadioIF_Rf_Handle, RadioIF_Cmd_Handle, 0);
        }
    }
    
    if(events && RadioIF_rfCoreEventCbFunc)
    {
        RadioIF_rfCoreEventCbFunc(events);
    }
}

void RadioIF_registerRfCoreEventCbApiSpecific(rfCoreEventCb func)
{
    RadioIF_rfCoreEventCbFunc = func;
}


void RadioIf_stopRxCmd(void)
{
    // Cancel and flush CMD_PROP_RX
    RF_flushCmd(RadioIF_Rf_Handle, RF_CMDHANDLE_FLUSH_ALL, 0);
}

void RadioIf_startRxCmd(void)
{
    // Issue Rf Core RX command to start RX
    RadioIF_Cmd_Handle = RF_postCmd(RadioIF_Rf_Handle, PhyManager_getRxCmd(), RF_PriorityNormal, &RadioIF_rfCoreEventCallback, IRQ_RX_BUF_FULL | IRQ_RX_ENTRY_DONE);
}


void RadioIf_runFsCmd(void)
{
    // Run the CMD_FS
    RF_postCmd(RadioIF_Rf_Handle, PhyManager_getFsCmd(), RF_PriorityNormal, NULL, 0);
}


void RadioIf_configureRatTimerEvent(RadioIf_TimerId timerId, uint32_t timeout, ratTimerEventCb func)
{
    RadioIf_timers[timerId].timerCbFunc = func;
    uint32_t compareTime = timeout * RADIO_TIMER_FREQUENCY_MHZ;
    
    RF_RatConfigCompare ratCompareCfg;
    ratCompareCfg.callback = &RadioIF_ratCallback;
    ratCompareCfg.channel = RadioIf_timers[timerId].channel;
    ratCompareCfg.timeout = compareTime;

    // Configure RAT compare callback
    RadioIf_timers[timerId].handle = RF_ratCompare(RadioIF_Rf_Handle, &ratCompareCfg, NULL);
}


void RadioIf_cancelRatTimerEvent(RadioIf_TimerId timerId)
{
    RF_ratDisableChannel(RadioIF_Rf_Handle, RadioIf_timers[timerId].handle);
}
