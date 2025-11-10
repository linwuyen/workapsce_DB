/*
 * Common.h
 *
 *  Created on: 2024¦~1¤ë5¤é
 *      Author: roger_lin
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

#include "F021_F28004x_C28x.h"


#define SW_TIMER          50000000
#define SWTIMER_CNT       SW_TIMER - (uint32_t)CPUTimer_getTimerCount(SWTIMER_BASE)

#define T_100MS            (SW_TIMER/5)


#define FG_GET(x, src)   ((src & (x)) == (x)) //((src & (x)) == (x))
#define FG_GETn(x, src)  (!FG_GET(x, src)) //!((src & (x)) == (x))
#define FG_AND(x, src)   (src & (x))
#define FG_SET(x, src)   {src |= (x);}
#define FG_RST(x, src)   {src &= ~(x);}
#define FG_SWTO(x, src)  {src = (x);}


#endif /* COMMON_H_ */
