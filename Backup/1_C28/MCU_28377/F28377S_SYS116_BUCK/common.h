/*
 * common.h
 *
 *  Created on: 2022�~3��9��
 *      Author: cody_chen
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>
#include "device.h"
#include "driverlib.h"
#include "board.h"

#include "ctypedef.h"

#include "HwConfig.h"

#define SW_TIMER        50000000

#define T_500US         (SW_TIMER/2000)
#define T_1MS           (SW_TIMER/1000)
#define T_2MS           (SW_TIMER/500)
#define T_2D5MS         (SW_TIMER/400)
#define T_5MS           (SW_TIMER/200)
#define T_10MS          (SW_TIMER/100)
#define T_20MS          (SW_TIMER/50)
#define T_25MS          (SW_TIMER/40)
#define T_50MS          (SW_TIMER/20)
#define T_100MS         (SW_TIMER/10)
#define T_200MS         (SW_TIMER/5)
#define T_500MS         (SW_TIMER/2)
#define T_1S            (SW_TIMER/1)


#include "cmath.h"

typedef struct {

    ST_RAMPCTRL sVref;

    ST_RAMPUPDN sTest;

    ST_DAC sDAC;
    
    float32_t   f32VO;
    float32_t   f32VIN;
    float32_t   f32IL;
    float32_t   f32IO;
    float32_t   f32IEL;

    uint32_t    u32IsrCnt;

    float32_t   f32RemoteVref;
    float32_t   f32RemoteIELref;

    uint16_t    fgDrv;
    uint16_t    fgFault;

    uint16_t    u16EnablePwm;
    uint16_t    u16DacCh;
    uint16_t    u16EnableEL;
    uint16_t    u16ELVonEnable;

    uint32_t    u32HeartBeat;

}  ST_DRV;

typedef ST_DRV* HAL_DRV;

extern volatile ST_DRV sDrv;

#include "linkPeripheral.h"
#include "PowerSequence.h"

/* Hardware GPIO Link */
// Output Pin
#define SET_ELOAD_ON()                  GPIO_writePin(EL_ON_OFF, 0);
#define RST_ELOAD_ON()                  GPIO_writePin(EL_ON_OFF, 1);
#define SET_DAC_nLDAC()                 GPIO_writePin(DAC8812_nLDAC, 1);
#define RST_DAC_nLDAC()                 GPIO_writePin(DAC8812_nLDAC, 0);
// Input Pin
#define RD_EL_UnReg()                   GPIO_readPin(EL_UnReg_STATUS);
#define RD_EL_OTP()                     GPIO_readPin(EL_OTP_STATUS);
#define RD_EL_LOAD_ON()                 GPIO_readPin(EL_ON_OFF);
/* Hardware GPIO Link */

#endif /* COMMON_H_ */
