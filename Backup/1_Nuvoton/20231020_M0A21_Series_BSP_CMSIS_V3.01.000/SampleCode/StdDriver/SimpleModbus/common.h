/*
 * common.h
 *
 *  Created on: 2022¦~3¤ë9¤é
 *      Author: cody_chen
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "stdio.h"
#include "NuMicro.h"

#include "ctypedef.h"
#include "g_timer.h"


#define SW_TIMER        16000000

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

#define GET_TIMERCNT    getTimerCounts(TIMER0)  // Must be a counting up of Counter


#define FG_GET(x, src)   ((src & (x)) == (x)) //((src & (x)) == (x))
#define FG_GETn(x, src)  (!FG_GET(x, src)) //!((src & (x)) == (x))
#define FG_AND(x, src)   (src & (x))
#define FG_SET(x, src)   {src |= (x);}
#define FG_RST(x, src)   {src &= ~(x);}
#define FG_SWTO(x, src)  {src = (x);}

extern void UART0_Init(void);

extern uint32_t writesuccess;


#endif /* COMMON_H_ */
