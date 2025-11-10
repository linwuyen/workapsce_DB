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

    s->sTest.f32Target1 = 1.0f;   // Input: Target input (pu)
    s->sTest.f32Target2 = -1.0f;   // Input: Target input (pu)
    s->sTest.f32Step1 = 0.1f;
    s->sTest.f32Step2 = 0.1f;
    s->sTest.f32Error = 0.0f;
    s->sTest.f32LowLimit = -1.0f;
    s->sTest.f32HighLimit = 1.0f;
    s->sTest.f32Out = 0.0f;
    s->sTest.blCountMode = false;

    s->u32HeartBeat = 0;

    FG_SETCLA(_CSTAT_INIT_DRV_PARAM);
}


void execPowerSequence(HAL_DRV s) {


}
