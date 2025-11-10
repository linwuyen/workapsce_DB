/*
 * cla_task.h
 *
 *  Created on: 2022¦~3¤ë29¤é
 *      Author: cody_chen
 */

#ifndef CLA_TASK_H_
#define CLA_TASK_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memcfg.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "cla_cmath.h"

#define FG_GETCLA(x)   FG_GET(x, sCLA.fgStatus)
#define FG_SETCLA(x)   FG_SET(x, sCLA.fgStatus)
#define FG_RSTCLA(x)   FG_RST(x, sCLA.fgStatus)

#define FG_GETERR(x)   FG_GET(x, sCLA.fgError)
#define FG_SETERR(x)   FG_SET(x, sCLA.fgError)
#define FG_RSTERR(x)   FG_RST(x, sCLA.fgError)


#define FG_GETPWM(x)   FG_GET(x, sCLA.sPWM.u16CtrlReg)
#define FG_SETPWM(x)   FG_SET(x, sCLA.sPWM.u16CtrlReg)
#define FG_RSTPWM(x)   FG_RST(x, sCLA.sPWM.u16CtrlReg)

enum {
    _CSTAT_NO_ACTION      =  (0x0000),
    _CSTAT_INIT_DRV_PARAM =  (0x0001<<0),
    _CSTAT_INIT_CLA_PARAM =  (0x0001<<1),
    _CSTAT_INIT_PWMADC    =  (0x0001<<2),
    _CSTAT_TASK1_OK       =  (0x0001<<4),
    _CSTAT_PWMADC_OK      =  (0x0001<<5),
    _CSTAT_INIT_SUCCESS   =  (_CSTAT_INIT_DRV_PARAM|_CSTAT_INIT_CLA_PARAM|_CSTAT_INIT_PWMADC),
    _CSTAT_BROWN_IN       =  (0x0001<<6),
    _CSTAT_ENABLE_PWM     =  (0x0001<<7),
    _CSTAT_OUTPUT_READY   =  (_CSTAT_BROWN_IN|_CSTAT_ENABLE_PWM),
    _CSTAT_CALIBRATION    =  (0x0001<<8),
};

typedef int16_t FG_CLASTAT;

enum {
    _NO_ERROR     = (0x0000),
    _ERR_VO_OVP   = (0x0001<<0),
    _ERR_VIN_UVP  = (0x0001<<1),
    _ERR_IO_OCP   = (0x0001<<2),
    _ALL_ERROR    = (_ERR_VO_OVP|_ERR_VIN_UVP|_ERR_IO_OCP),
    _WARNING_VO   = (0x0001<<8),
    _WARNING_VIN  = (0x0001<<9),
    _WARNING_IO   = (0x0001<<10),
    _ALL_WARNING  = (_WARNING_VO|_WARNING_VIN|_WARNING_IO),
};

typedef uint16_t FG_CLAERR;

typedef struct {
    float32_t  f32Adc;
    ST_LPF     sLPF;
    ST_CAL     sCali;
} ST_ADCLPF;

typedef struct {
    float32_t  f32Adc;
    ST_IIR     sIIR;
    ST_CAL     sCali;
} ST_ADCIIR;

typedef struct {

    ST_ADCLPF  sVIN;
    ST_ADCLPF  sIO;
    ST_ADCLPF  sIL;
    ST_ADCLPF  sIEL;
    ST_ADCLPF  sVO;
    ST_ADCIIR  sTempC;

    ST_ADCLPF  sVREF;

    ST_PI sLoopI;
    ST_PI sLoopV;

    float32_t f32IrefLimit;
    float32_t f32Iref;
    float32_t f32Vref;
    float32_t f32Vout;
    float32_t f32OpenGain;
    float32_t f32Duty;
    float32_t f32PwmPU;

    float32_t f32TestPU;
    uint32_t  u32DutyStep;
    uint32_t  u32MinStep;
    uint32_t  u32UnitStep;
    uint32_t  u32RollStep;

    ST_HRPWM  sHRPWM;

    FG_CLASTAT  fgStatus;
    FG_CLAERR   fgError;
}  ST_CLA;

extern volatile ST_CLA sCLA;

//CLA C Tasks defined in Cla1Tasks_C.cla
__attribute__((interrupt))  void Cla1Task1();
__attribute__((interrupt))  void Cla1Task2();
__attribute__((interrupt))  void Cla1Task3();
__attribute__((interrupt))  void Cla1Task4();
__attribute__((interrupt))  void Cla1Task5();
__attribute__((interrupt))  void Cla1Task6();
__attribute__((interrupt))  void Cla1Task7();
__attribute__((interrupt))  void Cla1Task8();

#endif /* CLA_TASK_H_ */
