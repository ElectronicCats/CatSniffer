/******************************************************************************
*  Filename:       user_if_task.c
*
*  Description:    User Interface task function implementation. This task handles
*                  periodic updates of the user interface.
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
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/PIN.h>
#include <xdc/runtime/System.h>
#include <stdio.h>
#include "host_if.h"
#include "task_event.h"
#include "version.h"

/* Board Header files */
#if defined DeviceFamily_CC13X0 || DeviceFamily_CC26X0
#include "board.h"
#else
#include "ti_drivers_config.h"
#endif

// Period of user updates (1 second)
#define USER_IF_TIMEOUT_PERIOD                  100000   // ticks of 10 us

void UserIfTask_waitInitDone(void);

// Pin table
static PIN_Config UserIfTask_pinTable[] =
{
    CONFIG_PIN_GLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};
static PIN_Handle UserIfTask_ledPin_Handle;
static PIN_State UserIfTask_ledPinState;

#define USER_MESSAGE "TI Packet Sniffer ver."
static const char UserIfTask_message[] = USER_MESSAGE VERSION_STRING "\n";


//! \brief User Interface task function
//!        The arguments are not used.
//!
//!        This task will periodically output a message to the host interface
//!        and blink an LED to inform the user that the sniffer is running. The
//!        period of the user updates is set to once per second. This task will
//!        run until the event EVENT_ID_USER_IF_TASK_END is received. This task
//!         will exit when receiving EVENT_ID_USER_IF_TASK_END.
//!
void userIfTask(UArg a0, UArg a1)
{
    // Wait for initilization of common modules to be done
    UserIfTask_waitInitDone();

    // Open LED pins
    UserIfTask_ledPin_Handle = PIN_open(&UserIfTask_ledPinState, UserIfTask_pinTable);
    if(!UserIfTask_ledPin_Handle)
    {
        System_abort("Error initializing board LED pins\n");
    }

    while(1)
    {
        // Let task sleep for timeout period
        Task_sleep(USER_IF_TIMEOUT_PERIOD);
        
        // Update user interface
        HostIF_writeBuffer((uint8_t*)UserIfTask_message, sizeof(UserIfTask_message));
        PIN_setOutputValue(UserIfTask_ledPin_Handle, CONFIG_PIN_GLED,!PIN_getOutputValue(CONFIG_PIN_GLED));

        // Wait for end task event
        UInt events = Event_pend(TaskEvent_Handle, EVENT_ID_USER_IF_TASK_END, Event_Id_NONE, BIOS_NO_WAIT);
        if(events & EVENT_ID_USER_IF_TASK_END)
        {
            // End this task
            PIN_close(UserIfTask_ledPin_Handle);
            Task_exit();
        }
    }
}


//! \brief Wait for initilization of the common modules between tasks in the system to be done
//! 
void UserIfTask_waitInitDone(void)
{
    UInt events = Event_pend(TaskEvent_Handle, EVENT_ID_USER_IF_TASK_INIT_DONE, Event_Id_NONE, BIOS_WAIT_FOREVER);
}
