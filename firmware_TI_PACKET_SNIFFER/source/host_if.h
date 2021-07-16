/******************************************************************************
*  Filename:       host_if.h
*
*  Description:    Header file for host interface.
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
#ifndef HOST_IF_H
#define HOST_IF_H

#include <stdint.h>

//! \brief Initialize the host interface
//!
//! \return None
extern void HostIF_init(void);


//! \brief Write data to the host interface
//!
//! \param[in] buffer
//!            Pointer to the buffer with data to write.
//!
//! \param[in] len
//!            Length of the data buffer
//!
//! \return None
extern void HostIF_writeBuffer(uint8_t* buffer, uint16_t len);


//! \brief Read data from the host interface
//!
//!        This function blocks until the number of bytes indicated by the 'len'
//!        argument is received.
//!
//! \param[in] buffer
//!            Pointer to the buffer with data to write.
//!
//! \param[in] len
//!            number of bytes to read.
//!
//! \return Number of bytes read from UART or -1 if an error occured
extern int32_t HostIF_readBuffer(uint8_t* buffer, uint16_t len);

#endif

