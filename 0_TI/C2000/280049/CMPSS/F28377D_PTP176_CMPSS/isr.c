/*
 * isr.c
 *
 *  Created on: Mar 18, 2024
 *      Author: cody_chen
 */

#include <math.h>
#include "common.h"



volatile ST_DRV sDrv = {
        .fgStatus = _CSTAT_INIT_DRV_PARAM,
        .sVref =  DEFAULT_VREF_LPF,
        .sVO =  { .sLPF = DEFAULT_VREF_LPF, .sCali = {.f32Gain = 1.0f, .f32Offset = 0.0f, .f32Base = VOUT_SCALE }},
        .sVIN = { .sLPF = DEFAULT_VREF_LPF, .sCali = {.f32Gain = 1.0f, .f32Offset = 0.0f, .f32Base = VIN_SCALE}},
        .sIO =  { .sLPF = DEFAULT_IO_LPF, .sCali = {.f32Gain = 1.0f, .f32Offset = 0.0f, .f32Base = IO_SCALE }},
        .sIL =  { .sLPF = DEFAULT_IL_LPF, .sCali = {.f32Gain = 1.0f, .f32Offset = 0.0f, .f32Base = IL_SCALE }},
        .regPwm = { .u16Enable = false },

        .sLoopV = { .f32Ki = _V_LOOP_KI, .f32Kp = _V_LOOP_KP,
                    .f32Min = -1.0f, .f32Max = 1.0f, .f32Itemp = 0.0f, .f32Out = 0.0f, .u16StopUi = false},

        .sLoopI = { .f32Ki = _I_LOOP_KI, .f32Kp = _I_LOOP_KP,
                    .f32Min = -1.0f, .f32Max = 1.0f, .f32Itemp = 0.0f, .f32Out = 0.0f, .u16StopUi = false},

        .f32IrefLimitP = RATED_IOUT_P_PU,
        .f32IrefLimitN = RATED_IOUT_N_PU,
        .f32Ovp = OVP_PU,
        .f32Ocp = OCP_PU,
        .f32VinOvp = VIN_OVP_PU,
        .u32HeartBeat = 0
};


#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH


__interrupt void INT_CPU1_ADCA_1_ISR (void)
{
    SET_DEBUG_ADC_EOC();

    sDrv.sVO.f32Adc = VO_SENSE;

    uint32_t u32DAC = (uint32_t)(sDrv.f32Vref * 4095.0f);
    DAC_setShadowValue(myDAC0_BASE, u32DAC);

    uint32_t u32Duty = (uint32_t)((float32_t) COUNTING_MAX * (CNV_SPU2UPU(sDrv.f32PwmPU)));


    SET_HRPWMAB_DUTY(u32Duty, PWM_BASE);


    sDrv.u32HeartBeat++;
    sDrv.u32HeartBeat &= 0x7FFFFFFF;

    // Clear the interrupt flag
    ADC_clearInterruptStatus(CPU1_ADCA_BASE, ADC_INT_NUMBER1);

    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INT_CPU1_ADCA_1_INTERRUPT_ACK_GROUP);

    RST_DEBUG_ADC_EOC();
}

__interrupt void INT_OVP_GPIO2_XINT_ISR(void)
{
    GPIO_writePin(YAN_XU_D4_LED, GPIO_readPin(IO_OVP_OUT_OUTPUTXBAR_GPIO));

    Interrupt_clearACKGroup(INT_OVP_GPIO2_XINT_INTERRUPT_ACK_GROUP);
}


#ifdef _FLASH
#pragma SET_CODE_SECTION()
#endif //_FLASH



