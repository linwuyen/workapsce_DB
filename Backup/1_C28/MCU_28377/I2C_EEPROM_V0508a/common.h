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
#include "driverlib.h"
#include "device.h"
#include "board.h"

#include "ctypedef.h"


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

extern float32_t f32A1;
extern float32_t f32A2;
extern float32_t f32A3;

extern float32_t u16B1;
extern uint16_t  u16B2;
extern uint16_t  u16B3;

extern uint16_t u16C1;
extern uint16_t u16C2;
extern uint16_t u16C3;


#endif /* COMMON_H_ */
