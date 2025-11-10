//#############################################################################
//
// FILE:   empty_sysconfig_main.c
//
// TITLE:  Empty Pinmux Example
//
// Empty SysCfg & Driverlib Example
//
// This example is an empty project setup for SysConfig and Driverlib 
// development.
//
//#############################################################################
//
// $Release Date:  $
// $Copyright:
// Copyright (C) 2013-2021 Texas Instruments Incorporated - http://www.ti.com/
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


#include <math.h>
#include "common.h"
#include "ModbusSlave.h"

#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH

float32_t csatf(float32_t data, float32_t max, float32_t min)
{
    if(max < data) return max;
    if(min > data) return min;
    return data;
}

float32_t csatPU(float32_t data)
{
    return csatf(data, 1.0f, -1.0f);
}

float32_t csatPosPU(float32_t data)
{
    return csatf(data, 1.0f, 0.0f);
}

float32_t csatNegPU(float32_t data)
{
    return csatf(data, 0.0f, -1.0f);
}

typedef struct { float32_t    f32Target;     // Input: Target input (pu)
                 float32_t    f32Step;       // Parameter: Increase/Decrease Step (pu)
                 float32_t    f32LowLimit;   // Parameter: Minimum limit (pu)
                 float32_t    f32HighLimit;  // Parameter: Maximum limit (pu)
                 float32_t    f32Output;     // Output: Target output (pu)
               } ST_SAWTOOTH;

typedef ST_SAWTOOTH * HAL_SAWTOOTH;

void execSawTooth(HAL_SAWTOOTH v)
{
    if (v->f32Target > v->f32Output)  {
        v->f32Output += v->f32Step;
        if (v->f32Target < v->f32Output)  v->f32Output = v->f32Target;
        if (v->f32Output > v->f32HighLimit) v->f32Output = v->f32HighLimit;
    }
    else if (v->f32Target < v->f32Output){
        v->f32Output -= v->f32Step;
        if (v->f32Target > v->f32Output)  v->f32Output = v->f32Target;
        if (v->f32Output < v->f32LowLimit) v->f32Output = v->f32LowLimit;
    }
    else {
        v->f32Output = 0.0f;
    }
}

typedef struct {
    float32_t f32Amplitude;
    float32_t f32Period;
    float32_t f32ShiftAngle;
    float32_t f32Offset;
    ST_SAWTOOTH sTooth;

    float32_t f32AngleA;
    float32_t f32AngleB;
    float32_t f32AngleC;


    uint16_t u16PhaseA;
    uint16_t u16PhaseB;
    uint16_t u16PhaseC;

} ST_TEST;

ST_TEST sTest;

#define OUTPUT_FREQ     60.0f
#define SAMPEL_FREQ     100000.0f
#define SAMPLE_STEP     (1.0f/SAMPEL_FREQ)
#define _2PI            (2.0f*3.14159f)
#define ANGLE_STEP      (1.0f/360.0f)


void task25msec(void * s)
{
    static uint16_t u16dly = 0;

    //RED_LED_OFF;

    if(0 == u16dly) {
        u16dly = 20;
        BLUE_LED_TOGGLE;

        sTest.f32Amplitude = regMbusData.sfAMPlitude;
        sTest.f32Period = regMbusData.sfOUTputFREQuency;
        sTest.f32ShiftAngle = regMbusData.sfPHASeSHIFt;
    }
    else u16dly--;

    //RED_LED_ON;
}

void task2D5msec(void * s)
{
    sTest.sTooth.f32Step = sTest.f32Period*SAMPLE_STEP;
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
        delta_t = u32Cnt + 0xFFFFFFFF - t->cnt;
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


    CPUTimer_enableInterrupt(ISR_TIMER0_BASE);

    //
    // Enables CPU int1 which are connected to CPU-Timer 0
    Interrupt_enable(INT_ISR_TIMER0);

    //
    // Starts CPU-Timer 0
    CPUTimer_startTimer(ISR_TIMER0_BASE);

    sTest.f32Amplitude = 1.0f;
    sTest.f32Period = 60.0f;
    sTest.f32ShiftAngle = 0.0f;
    sTest.f32Offset = 0.0f;
    sTest.sTooth.f32Target = 1.0f;
    sTest.sTooth.f32HighLimit = 1.0f;
    sTest.sTooth.f32LowLimit = 0.0f;
    sTest.sTooth.f32Step = OUTPUT_FREQ*SAMPLE_STEP;
    sTest.u16PhaseA = 0;
    sTest.u16PhaseB = 0;
    sTest.u16PhaseC = 0;


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

        exeModbusSlave(&mbcomm);
    }
}

__interrupt void INT_ISR_TIMER0_ISR(void) {
    GPIO_writePin(DEBUG_IO, 1);

    execSawTooth((HAL_SAWTOOTH)&sTest.sTooth);

    sTest.f32AngleA = sTest.sTooth.f32Output+sTest.f32ShiftAngle*ANGLE_STEP*0.0f;
    if(1.0f < sTest.f32AngleA) sTest.f32AngleA -= 1.0f;

    sTest.f32AngleB = sTest.sTooth.f32Output+sTest.f32ShiftAngle*ANGLE_STEP*1.0f;
    if(1.0f < sTest.f32AngleB) sTest.f32AngleB -= 1.0f;

    sTest.f32AngleC = sTest.sTooth.f32Output+sTest.f32ShiftAngle*ANGLE_STEP*2.0f;
    if(1.0f < sTest.f32AngleC) sTest.f32AngleC -= 1.0f;


    sTest.u16PhaseA = (uint16_t)((csatPU(sTest.f32Amplitude*sinf(_2PI*sTest.f32AngleA))*0.5f+0.5f)*4095.0f);
    sTest.u16PhaseB = (uint16_t)((csatPU(sTest.f32Amplitude*sinf(_2PI*sTest.f32AngleB))*0.5f+0.5f)*4095.0f);
    sTest.u16PhaseC = (uint16_t)((csatPU(sTest.f32Amplitude*sinf(_2PI*sTest.f32AngleC))*0.5f+0.5f)*4095.0f);


    DAC_setShadowValue(DEBUG_DACA_BASE, sTest.u16PhaseA);

    DAC_setShadowValue(DEBUG_DACB_BASE, sTest.u16PhaseB);

    DAC_setShadowValue(DEBUG_DACC_BASE, sTest.u16PhaseC);

    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INT_ISR_TIMER0_INTERRUPT_ACK_GROUP);

    GPIO_writePin(DEBUG_IO, 0);
}

//
// End of File
//
