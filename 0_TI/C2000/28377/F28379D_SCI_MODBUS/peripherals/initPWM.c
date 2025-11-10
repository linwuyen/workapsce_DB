/*
 * initPWM.c
 *
 *  Created on: 2022¦~3¤ë17¤é
 *      Author: cody_chen
 */

#include "initPWM.h"
#include "SFO_V8.h"

#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH

uint32_t             MEP_ScaleFactor;
uint16_t             BUCK_sfoStatus;

volatile uint32_t ePWM[9] =
                    {0, EPWM1_BASE, EPWM2_BASE, EPWM3_BASE, EPWM4_BASE,
                        EPWM5_BASE, EPWM6_BASE, EPWM7_BASE, EPWM8_BASE};

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

    HRPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_STOP_FREEZE);
    HRPWM_disablePhaseShiftLoad(base);
    HRPWM_setClockPrescaler(base, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);

    //
    // Set-up TBCLK
    HRPWM_setPeriodLoadMode(base, EPWM_PERIOD_SHADOW_LOAD);
    HRPWM_selectPeriodLoadEvent(base, EPWM_SHADOW_LOAD_MODE_COUNTER_ZERO);
    HRPWM_setTimeBasePeriod(base, CNT_BUCK_HRPWM);
    HRPWM_setPhaseShift(base, 0U);
    HRPWM_setTimeBaseCounter(base, 0U);

    //
    // set duty 0% initially
    HRPWM_setCounterCompareValue(base, HRPWM_COUNTER_COMPARE_A, 0);
    HRPWM_setCounterCompareValue(base, HRPWM_COUNTER_COMPARE_B, CNT_BUCK_HRPWM);

    //
    // ePWM1 SYNCO is generated on CTR=0
    HRPWM_setSyncOutPulseMode(base, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);

    //
    // Set up shadowing
    // Compare Value would load on count zero.
    HRPWM_setCounterCompareShadowLoadMode(base,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    HRPWM_setCounterCompareShadowLoadMode(base,
                                         EPWM_COUNTER_COMPARE_B,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    HRPWM_setCounterCompareShadowLoadEvent(base,
                                           HRPWM_CHANNEL_A,
                                           HRPWM_LOAD_ON_CNTR_ZERO);
    HRPWM_setCounterCompareShadowLoadEvent(base,
                                           HRPWM_CHANNEL_B,
                                           HRPWM_LOAD_ON_CNTR_ZERO);
    //
    // Set actions
    // CAU - Output A Low
    // CAD - Output A High
    // CBU - Output B High
    // CBD - Output B Low
    HRPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    HRPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    HRPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

    HRPWM_setActionQualifierAction(base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

    //
    // Align the HRPWM CMPA behavior with EPWM
    //
    HRPWM_setMEPEdgeSelect(base,
                           HRPWM_CHANNEL_A,
                           HRPWM_MEP_CTRL_FALLING_EDGE);

    HRPWM_setMEPControlMode(base,
                            HRPWM_CHANNEL_A,
                            HRPWM_MEP_DUTY_PERIOD_CTRL);

    HRPWM_setMEPEdgeSelect(base,
                           HRPWM_CHANNEL_B,
                           HRPWM_MEP_CTRL_FALLING_EDGE);

    HRPWM_setMEPControlMode(base,
                            HRPWM_CHANNEL_B,
                            HRPWM_MEP_DUTY_PERIOD_CTRL);

    HRPWM_enableAutoConversion(base);
    //
    // Enable EPWM

    HRPWM_setTimeBaseCounterMode(base, EPWM_COUNTER_MODE_UP_DOWN);


}

void initPWM(void)
{
    //
    // Calibrate MEP_ScaleFactor
    //
    do {
        BUCK_sfoStatus = SFO();
    } while(BUCK_sfoStatus == SFO_INCOMPLETE);

    //
    // Disable sync(Freeze clock to PWM as well)
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);


    initPWMxAB(BUCK_PWM_BASE);

    //
    // Enable sync and clock to PWM
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    enableAdcTriggerbyPWM(BUCK_PWM_BASE);

    FG_SETCLA(_CSTAT_INIT_PWMADC);
}

#ifdef _FLASH
#pragma SET_CODE_SECTION()
#endif //_FLASH
