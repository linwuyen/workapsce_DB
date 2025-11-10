/*
 * cla_cmath.h
 *
 *  Created on: 2022¦~3¤ë22¤é
 *      Author: cody_chen
 *
 */
#ifndef CLA_CMATH_H_
#define CLA_CMATH_H_

#include "ctypedef.h"

static inline void CLA_getAcCaliData(float32_t *f32Input, ST_CAL * v)
{
    v->f32Out = (*f32Input - v->f32Offset) * v->f32Gain;
}

static inline void CLA_getCaliData(float32_t *f32Input, ST_CAL * v)
{
    v->f32Out = *f32Input * v->f32Gain + v->f32Offset;
    v->f32Real = v->f32Out * v->f32Base;
}

static inline void CLA_mLPF(HAL_LPF v)
{
    v->f32Out = ( v->f32K1 * v->f32Out) + ((1.0f - v->f32K1) * v->f32Now);
}

static inline void CLA_mIIR(HAL_IIR v)
{
    v->f32Out += (v->f32K1 * (v->f32Now - v->f32Out));
}

static inline float32_t CLA_csatf(float32_t data, float32_t max, float32_t min)
{
    if(max < data) return max;
    if(min > data) return min;
    return data;
}

static inline void CLA_mPiLoop(volatile ST_PI *p) //710nsec_now
{
    p->f32Err = CLA_csatf(p->f32Err, 1.0f, -1.0f);

    /* integral term */
    if((p->f32Sum == p->f32Out)&&(false == p->u16StopUi)) {
        p->f32Idata = p->f32Itemp + p->f32Ki * p->f32Err;
    }
    else {
        p->f32Idata = p->f32Itemp;
    }
    p->f32Itemp = p->f32Idata;


    /* proportional term */
//    p->f32Pdata = CLA_csatf(p->f32Kp * p->f32Err, 1.0f, -1.0f);
    p->f32Pdata = p->f32Kp * p->f32Err;

    /* control output */
    p->f32Sum = p->f32Pdata + p->f32Idata;
    p->f32Out = CLA_csatf(p->f32Sum, p->f32Max, p->f32Min);
}
#endif /* CLA_CMATH_H_ */
