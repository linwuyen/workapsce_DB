/*
 * CTYPEDEF.h
 *
 *  Created on: 2022¦~3¤ë22¤é
 *      Author: cody_chen
 */

#ifndef CTYPEDEF_H_
#define CTYPEDEF_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memcfg.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#ifndef bool_t
typedef bool bool_t;
#endif

#ifndef true
#define true  ((bool_t)1)
#endif

#ifndef false
#define false ((bool_t)0)
#endif

#define _2PI()                           6.283185f

typedef struct {
    float32_t f32Gain;
    float32_t f32Offset;
    float32_t f32Out;
    float32_t  f32Base;
    float32_t  f32Real;
}  ST_CAL;
typedef ST_CAL * HAL_CAL;

typedef struct {
    float32_t f32Now;
    float32_t f32K1;  //f32k1 = 1.0F / (1.0F + ((2PI() * fc) / sample_rate_hz));
    float32_t f32Out;
}   ST_LPF;
typedef ST_LPF * HAL_LPF;


typedef struct {
    float32_t f32Now;
    float32_t f32K1;
    float32_t f32Out;
}    ST_IIR;
typedef ST_IIR * HAL_IIR;

typedef struct {
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
    uint16_t   u16StopUi;        // Parameter: Used to stop the integral term.
}   ST_PI;
typedef ST_PI * HAL_PI;

typedef struct {    float32_t    f32Target;     // Input: Target input (pu)
                    float32_t    f32Step;       // Parameter: Increase/Decrease Step (pu)
                    float32_t    f32LowLimit;   // Parameter: Minimum limit (pu)
                    float32_t    f32HighLimit;  // Parameter: Maximum limit (pu)
                    float32_t    f32Out;        // Output: Target output (pu)
}  ST_RAMPCTRL;
typedef ST_RAMPCTRL * HAL_RAMPCTRL;

typedef struct {     float32_t    f32Target1;   // Input: Target input (pu)
                     float32_t    f32Target2;   // Input: Target input (pu)
                     float32_t    f32Step1;
                     float32_t    f32Step2;
                     float32_t    f32Error;
                     float32_t    f32LowLimit;  // Parameter: Minimum limit (pu)
                     float32_t    f32HighLimit; // Parameter: Maximum limit (pu)
                     float32_t    f32Out;       // Output: Target output (pu)
                     bool_t       blCountMode;
}  ST_RAMPUPDN;
typedef ST_RAMPUPDN * HAL_RAMPUPDN;

typedef struct { float32_t    f32Target;        // Input: Target input (pu)
                 float32_t    f32Step;          // Parameter: Increase/Decrease Step (pu)
                 float32_t    f32LowLimit;      // Parameter: Minimum limit (pu)
                 float32_t    f32HighLimit;     // Parameter: Maximum limit (pu)
                 float32_t    f32Out;           // Output: Target output (pu)
} ST_SAWTOOTH;
typedef ST_SAWTOOTH * HAL_SAWTOOTH;

typedef struct {
    float32_t *pf32Buf;
    float32_t f32Scale;
    float32_t f32Sum;
    float32_t f32Avg;
    float32_t f32Now;
    uint16_t  u16Index;
    uint16_t  u16MemSize;
    uint16_t  u16Cycle;
    uint16_t  u16CycleIndex;
}ST_MVFIR;
typedef ST_MVFIR * HAL_MVFIR;

enum {
    _LUKTB_INIT_SUCCESS = (0x00000001<<0),
    _LUKTB_NO_ERROR    =   0x00000000,
    _LUKTB_INITL_FAIL  = ~(0x00000001<<0),
};

typedef struct {
    int32_t   fgStauts;
    float32_t *f32Table;
    float32_t  f32Points;
    float32_t f32InputPU;
    float32_t f32ValidData;
    float32_t f32Real;
    uint32_t  u32Integer;
    float32_t f32Fraction;
    float32_t *p1;
    float32_t *p2;
    float32_t f32Out;
} ST_LUKTB;

typedef ST_LUKTB * HAL_LUKTB;

/*  @ Lookup Table Example
 *
    float32_t f32V2F[9] = {
        -2.0, -1.0, -0.5, 0.2, 2.5, 9.0, 10.0, 15.0, 20.0
    };

    ST_LUKTB sV2F;

    float32_t j = 0.0f;

    int main()
    {
        initLookupTable(3, f32V2F, &sV2F);

         for(j=-1.0f; j<1.0; j+=0.01) {
             sV2F.f32InputPU = j;
             scanF32PUtoF32(&sV2F);
             printf("%f:, real = %f, out = %f\n", j, sV2F.f32Real, sV2F.f32Out);
         }
        return 0;
    }
 *
 */



typedef struct {
    uint32_t  u32PwmBase;
    uint16_t  u16CtrlReg;
    uint16_t  u16PwmDuty;
    uint16_t  u16PwmA;
    uint16_t  u16PwmB;
} ST_PWM;

typedef struct {
    uint32_t  u32PwmBase;
    uint32_t  u32PwmDuty;
    uint32_t  u32PwmA;
    uint32_t  u32PwmB;
} ST_HRPWM;

typedef ST_PWM * HAL_PWM;


#define FG_GET(x, src)   ((src & (x)) == (x)) //((src & (x)) == (x))
#define FG_GETn(x, src)  (!FG_GET(x, src)) //!((src & (x)) == (x))
#define FG_AND(x, src)   (src & (x))
#define FG_SET(x, src)   {src |= (x);}
#define FG_RST(x, src)   {src &= ~(x);}

#define UPDATE_PARK_BY_C28          1

#define CNV_SPU2UPU(x)   ((float32_t) x * 0.5f + 0.5f)
#define CNV_UPU2SPU(x)   (((float32_t) x - 0.5f) * 2.0f)



#endif /* CTYPEDEF_H_ */
