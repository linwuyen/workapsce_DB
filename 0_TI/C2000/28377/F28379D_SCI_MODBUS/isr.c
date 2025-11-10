/*
 * isr.c
 *
 *  Created on: 2022¦~3¤ë15¤é
 *      Author: cody_chen
 */
#include <math.h>
#include "common.h"
#include "cla_task.h"

#define PWM_SAMPLE_RATE    BUCK_PWM_KHZ
#define ISR_100USEC        (uint32_t)((float)BUCK_PWM_KHZ*1000.0f*0.0001)
#define ISR_200USEC        (uint32_t)((float)BUCK_PWM_KHZ*1000.0f*0.0002)
#define ISR_500USEC        (uint32_t)((float)BUCK_PWM_KHZ*1000.0f*0.0005)
#define ISR_1MSEC          (uint32_t)((float)BUCK_PWM_KHZ*1000.0f*0.0010)

#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH

void debugDAC(HAL_DRV s)
{
//    DAC_UPU(sCLA.sVO.sLPF.f32Out, DACA_TP209_BASE);
    DAC_UPU(sDrv.sVref.f32Out, DACB_TP208_BASE);
    DAC_UPU(sDrv.sVref.f32Target, DACC_TP210_BASE);
#if FORCE_OUTPUT_PWM
    DAC_SPU( sCLA.f32TestPU, DACA_TP209_BASE);
#endif
}

void scanBrownInOut(HAL_DRV s)
{

    if(false == FG_GETCLA(_CSTAT_BROWN_IN)) {
        if(VIN_BROWN_IN_PU < sCLA.sVIN.sLPF.f32Out) {
            sCLA.f32OpenGain = -1.0f;
            FG_SETCLA(_CSTAT_BROWN_IN);
        }

    }
    else {
        if(VIN_BROWN_OUT_PU > sCLA.sVIN.sLPF.f32Out) {
            sDrv.u16EnablePwm = false;
            FG_RSTCLA(_CSTAT_OUTPUT_READY);
        }
        else {
            if(s->sVref.f32Target != s->sVref.f32Out) {
                sCLA.f32OpenGain = s->sVref.f32Out / (sCLA.sVIN.sLPF.f32Out * VIN2VOUT_SCALE);
            }
            else {
                sCLA.f32OpenGain = sCLA.sVO.sLPF.f32Out / (sCLA.sVIN.sLPF.f32Out * VIN2VOUT_SCALE);
            }

            if(true == sDrv.u16EnablePwm) {
                FG_SETCLA(_CSTAT_ENABLE_PWM);
            }
            else {
                FG_RSTCLA(_CSTAT_ENABLE_PWM);
            }
        }
    }

}

void ctrlVrefRamp(HAL_DRV s)
{

    if(false == FG_GETCLA(_CSTAT_OUTPUT_READY)) {
        s->sVref.f32Target = s->sVref.f32Out = sCLA.sVO.sLPF.f32Out;
    }
    else {
        s->sVref.f32Target = s->f32RemoteVref;
        execRampCtrl((HAL_RAMPCTRL)&s->sVref);

    }

    sCLA.f32Vref = s->sVref.f32Out;

}

void execCalibration(HAL_DRV s)
{
    if(true == FG_GETCLA(_CSTAT_CALIBRATION)) {
        sCLA.sVO.sCali.f32Offset = -VO_SENSE;

        sCLA.sVIN.sCali.f32Offset = - VIN_SENSE;

        sCLA.sIO.sCali.f32Offset = -IO_SENSE;

        sCLA.sIL.sCali.f32Offset = -IL_SENSE;

        sCLA.sIEL.sCali.f32Offset = -IEL_SENSE;

        FG_RSTCLA(_CSTAT_CALIBRATION);
    }
    sCLA.f32Vref = s->sVref.f32Out;

}


void isrTask1msec(HAL_DRV s)
{
    SET_DEBUG_ADC_EOC();
    scanBrownInOut(s);

    execCalibration(s);
    RST_DEBUG_ADC_EOC();
}


void isrTask100usec(HAL_DRV s)
{
    SET_DEBUG_ADC_EOC();
    ctrlVrefRamp(s);
    RST_DEBUG_ADC_EOC();

    GPIO_togglePin(DEBUG_0);
}

typedef struct {
    void (*fn) (HAL_DRV s);
    uint32_t cnt;
    uint32_t max;
} ST_ISRTASK;

uint16_t id_isr_task = 0;
ST_ISRTASK isr_task[] = {
        {isrTask1msec,         0,   ISR_1MSEC},
        {isrTask100usec,       0,   ISR_100USEC},
        {0, 0, 0}
};


void isrTimeTask(ST_ISRTASK *t, HAL_DRV s)
{
    static uint32_t delta_t;

    s->u32IsrCnt++;
    s->u32IsrCnt &= 0x7FFFFFFF;

    if(t->cnt > s->u32IsrCnt) {
        delta_t = s->u32IsrCnt + 0x7FFFFFFF - t->cnt;
    }
    else {
        delta_t = s->u32IsrCnt - t->cnt;
    }

    if(delta_t >= t->max) {
        t->fn(s);
        t->cnt = s->u32IsrCnt;
    }
}

void C28_Measurement(HAL_DRV s) {
    sCLA.sVO.sLPF.f32Now = sCLA.sVO.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sVO.sLPF);
    s->f32VO = sCLA.sVO.sLPF.f32Out;

    sCLA.sVIN.sLPF.f32Now = sCLA.sVIN.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sVIN.sLPF);
    s->f32VIN = sCLA.sVIN.sLPF.f32Out;

    sCLA.sIO.sLPF.f32Now = sCLA.sIO.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sIO.sLPF);
    s->f32IO = sCLA.sIO.sLPF.f32Out;

    sCLA.sIL.sLPF.f32Now = sCLA.sIL.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sIL.sLPF);
    s->f32IL = sCLA.sIL.sLPF.f32Out;

    sCLA.sIEL.sLPF.f32Now = sCLA.sIEL.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sIEL.sLPF);
    s->f32IEL = sCLA.sIEL.sLPF.f32Out;

}


__interrupt void INT_ADC_EOC_ISR (void)
{
    if(FG_GETCLA(_CSTAT_TASK1_OK)) {
        SET_DEBUG_ADC_EOC();

        isrTimeTask(&isr_task[id_isr_task++], (HAL_DRV)&sDrv);
        if(0 == isr_task[id_isr_task].fn) id_isr_task = 0;

        C28_Measurement((HAL_DRV)&sDrv);

        debugDAC((HAL_DRV)&sDrv);

        FG_SETCLA(_CSTAT_PWMADC_OK)
    }

    // Clear the interrupt flag
    ADC_clearInterruptStatus(AD_A_BASE, ADC_INT_NUMBER1);

    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INT_AD_A_1_INTERRUPT_ACK_GROUP);

    RST_DEBUG_ADC_EOC();
}

#ifdef _FLASH
#pragma SET_CODE_SECTION()
#endif //_FLASH
