/******************************************************************************
*  Filename:       ll_manager.h
*
*  Description:    Header file for Link Layer Manager
*
* Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
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
#ifndef LL_MANAGER_H
#define LL_MANAGER_H

#include <stdint.h>

typedef uint16_t (*LL_processPacket)(uint8_t* packetData, uint16_t packetLength);
typedef void (*LL_reset)(void);

// Link Layer types
typedef enum LL_Type
{
    BLE=0,
    DEFAULT=1,
} LL_Type;


// Struct definition for Link Layer
typedef struct LL_Obj
{
    LL_processPacket pProcessPacketFunc;
    LL_reset pResetFunc;
} LL_Obj;


//! \brief Initialize Link Layers 
//!
//! \return none 
extern void LL_init(void);


//! \brief Get the currently used Link Layer type 
//!
//! \return none 
extern LL_Type LL_getLlType(void);


//! \brief Set the currently used Link Layer type 
//!
//! \return none
extern void LL_setLLType(LL_Type llType);

// Table of Link Layers
extern const LL_Obj LL_table[];

#endif

