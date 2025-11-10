/*
 * common.h
 *
 *  Created on: 2022¦~3¤ë9¤é
 *      Author: cody_chen
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "ti_msp_dl_config.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include "CheckEEPROM.h"
#include "ctypedef.h"

#define RAMFUNC \
    __attribute__((section(".TI.ramfunc"))) __attribute__((noinline))

//#define SW_TIMER        250000
#define SW_TIMER        62500

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

#define SWTIMER_CNT     SW_TIMER - (uint32_t)DL_TimerG_getTimerCount(SWTIMER_INST)

#define FG_GET(x, src)   ((src & (x)) == (x)) //((src & (x)) == (x))
#define FG_GETn(x, src)  (!FG_GET(x, src)) //!((src & (x)) == (x))
#define FG_AND(x, src)   (src & (x))
#define FG_SET(x, src)   {src |= (x);}
#define FG_RST(x, src)   {src &= ~(x);}
#define FG_SWTO(x, src)  {src = (x);}



#endif /* COMMON_H_ */
