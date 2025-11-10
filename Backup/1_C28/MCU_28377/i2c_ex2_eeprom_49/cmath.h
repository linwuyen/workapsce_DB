/*
 * cmath.h
 *
 *  Created on: 2022¦~3¤ë22¤é
 *      Author: cody_chen
 *
 */

#ifndef CMATH_H_
#define CMATH_H_

#include <math.h>
#include <limits.h>

#include <stdint.h>
#include <stdbool.h>
#include "driverlib.h"

#include "ctypedef.h"

#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH

static inline float32_t csatf(float32_t data, float32_t max, float32_t min)
{
    if(max < data) return max;
    if(min > data) return min;
    return data;
}

static inline float32_t csatPU(float32_t data)
{
    return csatf(data, 1.0f, -1.0f);
}

static inline float32_t csatPosPU(float32_t data)
{
    return csatf(data, 1.0f, 0.0f);
}

static inline float32_t csatNegPU(float32_t data)
{
    return csatf(data, 0.0f, -1.0f);
}

static inline void getCaliData(float32_t *f32Input, ST_CAL * v)
{
    v->f32Out = *f32Input * v->f32Gain + v->f32Offset;
}


static inline void mLPF(HAL_LPF v) //21c
{
    v->f32Out = ( v->f32K1 * v->f32Out) + ((1.0f - v->f32K1) * v->f32Now);
}


static inline void mIIR(HAL_IIR v) //21c
{
    v->f32Out += (v->f32K1 * (v->f32Now - v->f32Out));
}


static inline void execRampCtrl(HAL_RAMPCTRL v) //310nsec_now
{
    if (v->f32Target > v->f32Out)  {
        v->f32Out += v->f32Step;
        if (v->f32Target < v->f32Out)  v->f32Out = v->f32Target;
        if (v->f32Out > v->f32HighLimit) v->f32Out = v->f32HighLimit;
    }
    else if (v->f32Target < v->f32Out){
        v->f32Out -= v->f32Step;
        if (v->f32Target > v->f32Out)  v->f32Out = v->f32Target;
        if (v->f32Out < v->f32LowLimit) v->f32Out = v->f32LowLimit;
    }
}


static inline void execRampUpDwon(HAL_RAMPUPDN v)
{
    if(false == v->blCountMode) {
        if (v->f32Target1 > v->f32Out)  {
            v->f32Error = v->f32Target1 - v->f32Out;
            if(v->f32Error > v->f32Step1) {
                v->f32Out += v->f32Step1;
            }
            else {
                v->f32Out = v->f32Target1;
                v->blCountMode = true;
            }

        }
        else if (v->f32Target1 < v->f32Out){
            v->f32Error = v->f32Out - v->f32Target1;
            if(v->f32Error > v->f32Step1) {
                v->f32Out -= v->f32Step1;
            }
            else {
                v->f32Out = v->f32Target1;
                v->blCountMode = true;
            }
        }
    }
    else {
        if (v->f32Target2 > v->f32Out)  {
            v->f32Error = v->f32Target2 - v->f32Out;
            if(v->f32Error > v->f32Step2) {
                v->f32Out += v->f32Step2;
            }
            else {
                v->f32Out = v->f32Target2;
                v->blCountMode = false;
            }

        }
        else if (v->f32Target2 < v->f32Out){
            v->f32Error = v->f32Out - v->f32Target2;
            if(v->f32Error > v->f32Step2) {
                v->f32Out -= v->f32Step2;
            }
            else {
                v->f32Out = v->f32Target2;
                v->blCountMode = false;
            }
        }
    }

    if (v->f32Out > v->f32HighLimit) v->f32Out = v->f32HighLimit;
    if (v->f32Out < v->f32LowLimit) v->f32Out = v->f32LowLimit;
}

static inline void execSawTooth(HAL_SAWTOOTH v)
{
    if (v->f32Target > v->f32Out)  {
        v->f32Out += v->f32Step;
        if (v->f32Target < v->f32Out)  v->f32Out = v->f32Target;
        if (v->f32Out > v->f32HighLimit) v->f32Out = v->f32HighLimit;
    }
    else if (v->f32Target < v->f32Out){
        v->f32Out -= v->f32Step;
        if (v->f32Target > v->f32Out)  v->f32Out = v->f32Target;
        if (v->f32Out < v->f32LowLimit) v->f32Out = v->f32LowLimit;
    }
    else {
        v->f32Out = 0.0f;
    }
}

static inline void mMVFIR(HAL_MVFIR v) //862nsec_now
{
    if((v->u16Index < v->u16MemSize)&&(v->u16CycleIndex < v->u16MemSize)) {
        if(v->u16CycleIndex <  v->u16Cycle) {
            v->pf32Buf[v->u16CycleIndex] = v->f32Now;
            v->f32Sum += v->pf32Buf[v->u16CycleIndex];
            v->u16CycleIndex++;
        }
        else {
            v->f32Sum -= v->pf32Buf[v->u16Index];
            v->pf32Buf[v->u16Index] = v->f32Now;
            v->f32Sum += v->pf32Buf[v->u16Index];
            v->u16Index++;
            if(v->u16Index >= v->u16Cycle) v->u16Index = 0;
        }
        v->f32Avg = v->f32Sum * v->f32Scale;
    }
    else {
        v->u16CycleIndex = v->u16Index = 0;
        v->f32Sum = v->f32Avg = 0.0f;
    }
}


#ifdef _FLASH
#pragma SET_CODE_SECTION()
#endif //_FLASH


#endif /* CMATH_H_ */
