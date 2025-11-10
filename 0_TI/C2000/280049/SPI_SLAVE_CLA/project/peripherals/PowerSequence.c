/*
 * PowerSequence.c
 *
 *  Created on: 2022¦~3¤ë24¤é
 *      Author: cody_chen
 */

#include "HwConfig.h"
#include "PowerSequence.h"
#include "initPWM.h"
#include "mbcmd.h"

void initPower(HAL_DRV s) {

    s->sVref.f32HighLimit = 1.0f;
    s->sVref.f32LowLimit = 0.0f;
    s->sVref.f32Step = VREF_RAMP_STEP;
    s->sVref.f32Out = 0.0f;
    s->sVref.f32Target = VREF_PU;

    FG_SETCLA(_CSTAT_INIT_DRV_PARAM);
}


void execPowerSequence(HAL_DRV s) {


}
