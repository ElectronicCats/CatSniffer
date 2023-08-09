/******************************************************************************
*  Filename:       common.h
*
*  Description:    Header file for common functionality
*
*  Copyright (C) 2017-2018 Texas Instruments Incorporated - http://www.ti.com/
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
#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>


//! \brief Get little endian value of 32 bit byte field that may start at an
//!        unaligned address.
//!        (32 bit pointer access should be avoided at non-word aligned
//!         address. Use this function instead. )
//!
//! \param[in] buffer
//!            Pointer to first byte of the 32 bit value
//!
//! \return 32 bit value
//!
extern inline uint32_t Common_get32BitValueLE(uint8_t* buffer);


//! \brief Get little endian value of 16 bit byte field that may start at an
//!        unaligned address.
//!        (16 bit pointer access should be avoided at non-word aligned
//!         address. Use this function instead. )
//!
//! \param[in] buffer
//!            Pointer to first byte of the 16 bit value
//!
//! \return 16 bit value
//!
extern inline uint16_t Common_get16BitValueLE(uint8_t* buffer);


//! \brief Set little endian value of 16 bit byte field that may start at an
//!        unaligned address.
//!        (16 bit pointer access should be avoided at non-word aligned
//!         address. Use this function instead. )
//!
//! \param[out] buffer
//!            Pointer to first byte of the 16 bit value
//!
//! \param[in] value to set
//!
extern inline void Common_set16BitValueLE(uint8_t* buffer, uint16_t value);


//! \brief Set little endian value of 32 bit byte field that may start at an
//!        unaligned address.
//!        (32 bit pointer access should be avoided at non-word aligned
//!         address. Use this function instead. )
//!
//! \param[out] buffer
//!            Pointer to first byte of the 32 bit value
//!
//! \param[in] value to set
//!
extern inline void Common_set32BitValueLE(uint8_t* buffer, uint32_t value);

#endif

