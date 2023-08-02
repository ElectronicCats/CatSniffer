/******************************************************************************
*  Filename:       packet_sniffer_fw.c
*
*  Description:    Main file for the packet sniffer fw application.
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

#include <stdlib.h>
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "host_if_task.h"
#include "data_task.h"
#include "user_if_task.h"
#include "control_task.h"
#include "task_event.h"

/* Board Header files */
#if defined DeviceFamily_CC13X0 || DeviceFamily_CC26X0
#include "board.h"
#else
#include "ti_drivers_config.h"
#endif


/***** Defines *****/
#define HOST_IF_TASK_PRIORITY           1
#define DATA_TASK_PRIORITY              2
#define USER_IF_TASK_PRIORITY           3
#define CONTROL_TASK_PRIORITY           4


static Task_Params hostIfTaskParams;
static Task_Params dataTaskParams;
static Task_Params userIfTaskParams;
static Task_Params controlTaskParams;


void task_init(void)
{
    // Create the tasks
    Task_Params_init(&hostIfTaskParams);
    hostIfTaskParams.priority = HOST_IF_TASK_PRIORITY;
    Task_create(hostIfTask, &hostIfTaskParams, NULL);
  
    Task_Params_init(&dataTaskParams);
    dataTaskParams.priority = DATA_TASK_PRIORITY;
    Task_create(dataTask, &dataTaskParams, NULL);
    
    Task_Params_init(&userIfTaskParams);
    userIfTaskParams.priority = USER_IF_TASK_PRIORITY;
    Task_create(userIfTask, &userIfTaskParams, NULL);
    
    Task_Params_init(&controlTaskParams);
    controlTaskParams.priority = CONTROL_TASK_PRIORITY;
    // Increase stack size for the control task
    controlTaskParams.stackSize = 768;
    Task_create(controlTask, &controlTaskParams, NULL);
    
    // Initialize task event module
    TaskEvent_init();
}


//! \brief main function
int main(void)
{
    // Call board init functions.
    Board_init();

#ifndef STATIC_TASK
    // Initialize tasks
    task_init();
#endif
        
    // Start BIOS
    BIOS_start();

    return (0);
}



