/******************************************************************************
*  Filename:       host_if.c
*
*  Description:    Source file for the host interface.
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

#include "host_if.h"
#include "Board.h"
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/UART.h>
#include <ti/sysbios/BIOS.h>

static UART_Handle HostIF_Uart_Handle;
static UART_Params HostIF_uartParams;
static bool HostIF_isInitialized = false;

// Semaphore that for protecting uart calls against simultanous access from
// multiple tasks. Simulatanous access to uart from more than one task in same
// direction is not supported by the TI-RTOS driver.
static Semaphore_Struct HostIF_semStructUart;
static Semaphore_Handle HostIF_SemUart_Handle;

// UART read timeout
#define UART_READ_TIMEOUT_PERIOD                20000 // ticks of 10 us -> 200ms

void HostIF_init()
{
    if(!HostIF_isInitialized)
    {
        UART_init();
        UART_Params_init(&HostIF_uartParams);
        HostIF_uartParams.writeDataMode = UART_DATA_BINARY;
        HostIF_uartParams.readDataMode = UART_DATA_BINARY;
        HostIF_uartParams.readReturnMode = UART_RETURN_FULL;
        HostIF_uartParams.readEcho = UART_ECHO_OFF;
        HostIF_uartParams.baudRate = 921600;
        HostIF_uartParams.readTimeout = UART_READ_TIMEOUT_PERIOD;
        HostIF_Uart_Handle = UART_open(Board_UART0, &HostIF_uartParams);

        if (HostIF_Uart_Handle == NULL)
        {
            System_abort("Error opening the UART");
        }

        // Initialize the uart semaphore
        Semaphore_Params semParams;

        // Construct a new binary semaphore and initialize it to 1
        Semaphore_Params_init(&semParams);
        semParams.mode = Semaphore_Mode_BINARY;
        Semaphore_construct(&HostIF_semStructUart, 1, &semParams);

        HostIF_SemUart_Handle = Semaphore_handle(&HostIF_semStructUart);

        HostIF_isInitialized = true;
    }
}


void HostIF_writeBuffer(uint8_t* buffer, uint16_t len)
{
    // Suspend the task until UART is available
    Semaphore_pend(HostIF_SemUart_Handle, BIOS_WAIT_FOREVER);
    UART_write(HostIF_Uart_Handle, buffer, len);

    // Signal the other tasks waiting for UART semaphore
    Semaphore_post(HostIF_SemUart_Handle);
}


int32_t HostIF_readBuffer(uint8_t* buffer, uint16_t len)
{
    return UART_read(HostIF_Uart_Handle, buffer, len);
}
