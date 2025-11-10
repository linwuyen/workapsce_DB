/*
 * initPWM.h
 *
 *  Created on: 2022¦~3¤ë17¤é
 *      Author: cody_chen
 */

#ifndef PERIPHERALS_INITPWM_H_
#define PERIPHERALS_INITPWM_H_

#include "common.h"

extern void initPWM(void);
extern void set3PWM(float32_t f32DutyA, float32_t f32DutyB, float32_t f32DutyC, bool_t blEnable);


#endif /* PERIPHERALS_INITPWM_H_ */
