/*
 * common.h
 *
 *  Created on: 2022¦~3¤ë9¤é
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

    float32_t   f32VO;
    float32_t   f32VIN;
    float32_t   f32IL;
    float32_t   f32IO;
    float32_t   f32IEL;

    uint32_t    u32IsrCnt;

    float32_t   f32RemoteVref;


    uint16_t    fgDrv;
    uint16_t    fgFault;

    uint16_t    u16EnablePwm;

}  ST_DRV;

typedef ST_DRV* HAL_DRV;

extern volatile ST_DRV sDrv;

#include "linkPeripheral.h"
#include "PowerSequence.h"

#endif /* COMMON_H_ */
