/******************************************************************************
*  Filename:       common.c
*
*  Description:    Source file common functionality.
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

#include "common.h"

inline uint32_t Common_get32BitValueLE(uint8_t* buffer)
{
    uint32_t value = 0;
    value |= (uint32_t)buffer[3] << 24;
    value |= (uint32_t)buffer[2] << 16;
    value |= (uint32_t)buffer[1] <<  8;
    value |= (uint32_t)buffer[0];

    return value;
}


inline uint16_t Common_get16BitValueLE(uint8_t* buffer)
{
    uint16_t value = 0;
    value |= (uint16_t)buffer[1] <<  8;
    value |= (uint16_t)buffer[0];

    return value;
}


inline void Common_set16BitValueLE(uint8_t* buffer, uint16_t value)
{
    buffer[0] = value & 0xFF;
    buffer[1] = (value & 0xFF00) >> 8;
}


inline void Common_set32BitValueLE(uint8_t* buffer, uint32_t value)
{
    buffer[0] = value & 0xFF;
    buffer[1] = (value & 0xFF00) >> 8;
    buffer[2] = (value & 0xFF0000) >> 16;
    buffer[3] = (value & 0xFF000000) >> 24;
}
