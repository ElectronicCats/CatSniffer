/******************************************************************************
*  Filename:       output_if.c
*
*  Description:    Source file for output interface.
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
#include <ti/drivers/UART.h>

UART_Handle uart;
UART_Params uartParams;
bool isInitialized=false;

void HIF_init()
{
    if(!isInitialized)
    {
         /*char input;
         const char echoPrompt[] = "\fEchoing characters:\r\n";*/

        /* Create a UART */
        UART_Params_init(&uartParams);
        uartParams.writeDataMode = UART_DATA_BINARY;
        uartParams.readDataMode = UART_DATA_BINARY;
        uartParams.readReturnMode = UART_RETURN_FULL;
        uartParams.readEcho = UART_ECHO_OFF;
        uartParams.baudRate = 9600;
        uart = UART_open(Board_UART0, &uartParams);

        if (uart == NULL) {
            System_abort("Error opening the UART");
        }

       /* UART_write(uart, echoPrompt, sizeof(echoPrompt));


        while (1) {
            UART_read(uart, &input, 1);
            UART_write(uart, &input, 1);
        }*/

        isInitialized = true;
    }
}


void HIF_writeBuffer(uint8_t* buffer, uint16_t len)
{
    UART_write(uart, buffer, len);
}

void HIF_readBuffer(uint8_t* buffer, uint8_t len)
{
    UART_read(uart, buffer, len);
}
