/*
 * initPWM.c
 *
 *  Created on: 2022¦~3¤ë17¤é
 *      Author: cody_chen
 */

#include "initPWM.h"

#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH

void enableAdcTriggerbyPWM(uint32_t base)
{
    //
    // Disable SOCA
    //
    EPWM_disableADCTrigger(base, EPWM_SOC_A);

    //
    // Configure the SOC to occur on the first up-count event
    EPWM_setADCTriggerSource(base, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);
    EPWM_setADCTriggerEventPrescale(base, EPWM_SOC_A, 1);

    //
    // Start ePWM1, enabling SOCA and putting the counter in up-count mode
    //
    EPWM_enableADCTrigger(base, EPWM_SOC_A);
}


void initPWMxAB(uint32_t base)
{

    EPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_STOP_FREEZE);
    EPWM_disablePhaseShiftLoad(base);
    EPWM_setClockPrescaler(base, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);

    //
    // Set-up TBCLK
    EPWM_setPeriodLoadMode(base, EPWM_PERIOD_SHADOW_LOAD);
    EPWM_selectPeriodLoadEvent(base, EPWM_SHADOW_LOAD_MODE_COUNTER_ZERO);
    EPWM_setTimeBasePeriod(base, CNT_BUCK_PWM);
    EPWM_setPhaseShift(base, 0U);
    EPWM_setTimeBaseCounter(base, 0U);

    //
    // set duty 0% initially
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_A, 0);
    EPWM_setCounterCompareValue(base, EPWM_COUNTER_COMPARE_B, CNT_BUCK_PWM);

    //
    // ePWM1 SYNCO is generated on CTR=0
    EPWM_setSyncOutPulseMode(base, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);

    //
    // Set up shadowing
    // Compare Value would load on count zero.
    EPWM_setCounterCompareShadowLoadMode(base,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareShadowLoadMode(base,
                                         EPWM_COUNTER_COMPARE_B,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    //
    // Set actions
    // CAU - Output A Low
    // CAD - Output A High
    // CBU - Output B High
    // CBD - Output B Low
    EPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    EPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    EPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

    EPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
    //
    // Enable EPWM
    EPWM_forceSyncPulse(base);
    EPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_UP_DOWN);


}

void initPWM(void)
{
    //
    // Disable sync(Freeze clock to PWM as well)
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);


    initPWMxAB(BUCK_PWM1_BASE);

    //
    // Enable sync and clock to PWM
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    enableAdcTriggerbyPWM(BUCK_PWM1_BASE);

    FG_SETCLA(_CSTAT_INIT_PWMADC);
}

#ifdef _FLASH
#pragma SET_CODE_SECTION()
#endif //_FLASH
