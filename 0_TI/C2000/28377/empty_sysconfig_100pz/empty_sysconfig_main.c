//#############################################################################
//
// FILE:   empty_sysconfig_main.c
//
// TITLE:  Empty Pinmux Example
//
//! \addtogroup driver_example_list
//! <h1> Empty SysCfg & Driverlib Example </h1>
//!
//! This example is an empty project setup for SysConfig and Driverlib 
//! development.
//
//#############################################################################
//
//
// $Copyright:
// Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################


//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

#include "common.h"
#include "ModbusSlave.h"



#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH


void task25msec(void * s)
{

}

void task2D5msec(void * s)
{


}

typedef struct _ST_TIMETASK{
    void (*fn) (void * s);
    uint32_t cnt;
    uint32_t max;
} ST_TIMETASK;

uint16_t id_ttask = 0;
ST_TIMETASK time_task[] = {
        {task2D5msec,         0,   T_2D5MS},
        {task25msec,          0,   T_25MS},
        {0, 0, 0}
};


bool_t scanTimeTask(ST_TIMETASK *t, void *s)
{
    static uint32_t delta_t;
    static uint32_t u32Cnt = 0;

    u32Cnt = SW_TIMER - (uint32_t)CPUTimer_getTimerCount(SWTIRMER_BASE);

    if(t->cnt > u32Cnt) {
        delta_t = u32Cnt + SW_TIMER - t->cnt;
    }
    else {
        delta_t = u32Cnt - t->cnt;
    }

    if(delta_t >= t->max) {
        t->fn(s);
        t->cnt = u32Cnt;
        return true;
    }
    else {
        return false;
    }
}


#ifdef _FLASH
#pragma SET_CODE_SECTION()
#endif //_FLASH

void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Board_init();



    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Loop Forever
    //
    while(1)
    {
        scanTimeTask(&time_task[id_ttask++], (void *)0);
        if(0 == time_task[id_ttask].fn) id_ttask = 0;

        exeModbusSlave((SCI_MODBUS *)&mbcomm);
    }

}
//
// End of File
//
