/*
 * c28math.h
 *
 *  Created on: Mar 22, 2024
 *      Author: User
 */

#ifndef C28MATH_H_
#define C28MATH_H_

#include "common.h"


#ifndef bool_t
typedef bool bool_t;
#endif

#ifndef true
#define true  ((bool_t)1)
#endif

#ifndef false
#define false ((bool_t)0)
#endif

#define _2PI                           6.2831853071f

typedef volatile struct {
    float32_t x;
    float32_t y;
} ST_POINT;

#define DEFAULT_POINT   (ST_POINT){.x = 0, .y = 0}

typedef volatile struct {
    ST_POINT p1;
    ST_POINT p2;
} ST_LINE;

#define DEFAULT_LINE   (ST_LINE){.p1 = DEFAULT_POINT, .p2 = DEFAULT_POINT}

typedef ST_LINE * HAL_LINE;

typedef volatile struct {
    float32_t f32Input;
    float32_t f32Gain;
    float32_t f32Offset;
    float32_t f32Out;
    float32_t  f32Base;
    float32_t  f32Real;
}  ST_CAL;

typedef ST_CAL * HAL_CAL;

typedef volatile struct {
    float32_t f32Input;
    float32_t f32K1;  //f32k1 = 2.0f * 3.14159265359f * v->Fc / v->Fs;
    float32_t f32Err;
    float32_t f32Out;
}   ST_LPF;
typedef  ST_LPF * HAL_LPF;

#define CAL_LPF_K1(Fc_hz, Fs_hz)  (float32_t)(_2PI * Fc_hz / Fs_hz)

typedef volatile struct {
    float32_t  f32Out;           // Output: controller output
    float32_t  f32Max;           // Parameter: Limit the controller output by Maximum
    float32_t  f32Min;           // Parameter: Limit the controller output by Minimum
    float32_t  f32Kp;            // Parameter: proportional loop gain
    float32_t  f32Ki;            // Parameter: integral gain (0.0~1.0)
    float32_t  f32Err;           // Data: (f32) Error between the feedback and reference
    float32_t  f32LimitErr;      // Data: (f32) Limit f32Err
    float32_t  f32Pdata;         // Data: (f32) Save the proportional term results
    float32_t  f32Idata;         // Data: (f32) Save the integral term
    float32_t  f32Itemp;         // Data: (F32) Temporarily save the integral term i(k-1)
    float32_t  f32Sum;           // Data: Sum of proportional term and integral term
    uint16_t   u16Reserved;
    uint16_t   u16StopUi;        // Parameter: Used to stop the integral term.
}   ST_PI;
typedef ST_PI * HAL_PI;

static inline void getAcCaliData(float32_t f32Input,  HAL_CAL v)
{
    v->f32Input = f32Input;
    v->f32Out = (v->f32Input - v->f32Offset) * v->f32Gain;
    v->f32Real = v->f32Out * v->f32Base;
}

static inline void getCaliData(float32_t f32Input, HAL_CAL v)
{
    v->f32Input = f32Input;
    v->f32Out = v->f32Input * v->f32Gain + v->f32Offset;
    v->f32Real = v->f32Out * v->f32Base;
}

static inline void getCaliGainAndOffset(HAL_LINE d, HAL_CAL v)
{
    v->f32Gain = (d->p2.y-d->p1.y)/v->f32Base/(d->p2.x-d->p1.x);
    v->f32Offset = d->p2.y/v->f32Base - v->f32Gain*d->p2.x;
}

static inline void mLPF(float32_t f32Input, HAL_LPF v)
{
    v->f32Input = f32Input;
    v->f32Err = v->f32K1 * (v->f32Input- v->f32Out);
    if((0.0000305f < v->f32Err) || (-0.0000305f > v->f32Err)) {
        v->f32Out += v->f32Err;
    }
    else {
        v->f32Out = f32Input;
    }
}

static inline float32_t csatf(float32_t data, float32_t max, float32_t min)
{
    if(max < data) return max;
    if(min > data) return min;
    return data;
}

static inline void mPI(volatile ST_PI *p)
{
    p->f32Err = csatf(p->f32Err, 1.0f, -1.0f);

    /* integral term */
    if((p->f32Sum == p->f32Out)&&(false == p->u16StopUi)) {
        p->f32Idata = p->f32Itemp + p->f32Ki * p->f32Err;
    }
    else {
        p->f32Idata = p->f32Itemp;
    }
    p->f32Itemp = p->f32Idata;

    /* proportional term */
    p->f32Pdata = p->f32Kp * p->f32Err;

    /* control output */
    p->f32Sum = p->f32Pdata + p->f32Idata;
    p->f32Out = csatf(p->f32Sum, p->f32Max, p->f32Min);
}

#endif /* C28MATH_H_ */
