/*
 * initPWM.h
 *
 *  Created on: Mar 18, 2024
 *      Author: cody_chen
 */

#ifndef INIT_PWM_H
#define INIT_PWM_H

#include "common.h"
#include "SFO_V8.h"
#include "HwConfig.h"

#define ADC_TRG_SRC     EPWM_SOC_A
#define PWM_BASE        BUCK_PWM1_BASE
#define PWM_KHZ         BUCK_PWM_KHZ
#define T_DB_NSEC       T_BUCK_PWM_DEADBAND_NSEC

#define PWM_CNTS        (uint16_t)(((uint32_t)EPWMCLK_FREQ/1000/PWM_KHZ)>>1)
#define HRPWM_CNTS      (uint32_t)(((uint32_t)PWM_CNTS)<<8U)

#define PWM_DB_CNTS     (uint16_t)(EPWMCLK_FREQ/1000*T_DB_NSEC/1000000)
#define HRPWM_DB_CNTS   (uint32_t)(((uint32_t)PWM_DB_CNTS)<<8U)

#define COUNTING_MAX    (HRPWM_CNTS-1)
#define DB_H_CNTS       (HRPWM_DB_CNTS>>1)
#define DB_L_CNTS       (HRPWM_CNTS - DB_H_CNTS)

#define SET_HRCMPA(cmp, base)   HWREG(base + HRPWM_O_CMPA) = cmp << 8U;
#define SET_HRCMPB(cmp, base)   HWREG(base + HRPWM_O_CMPB) = cmp << 8U;

#define SET_DB_HRPWMA(cmp) (DB_H_CNTS<cmp? (cmp - DB_H_CNTS): 0U)
#define SET_DB_HRPWMB(cmp) (DB_L_CNTS<cmp? HRPWM_CNTS: (cmp + DB_H_CNTS))

#define RST_HRPWMAB_DUTY(base)           SET_HRCMPA(0, base); \
                                         SET_HRCMPB(HRPWM_CNTS, base);

#define SET_HRPWMAB_DUTY(cmp, base)      SET_HRCMPA(SET_DB_HRPWMA(cmp), base); \
                                         SET_HRCMPB(SET_DB_HRPWMB(cmp), base);

#define SET_HRPWMA_DUTY(cmp, base)       SET_HRCMPA(cmp, base); \
                                         SET_HRCMPB(HRPWM_CNTS, base);

#define SET_HRPWMB_DUTY(cmp, base)       SET_HRCMPA(0, base); \
                                         SET_HRCMPB(cmp, base);


static inline void enableAdcTriggerbyPWM(void)
{
    //
    // Disable SOCA
    //
    EPWM_disableADCTrigger(PWM_BASE, ADC_TRG_SRC);

    //
    // Configure the SOC to occur on the first up-count event
    EPWM_setADCTriggerSource(PWM_BASE, ADC_TRG_SRC, EPWM_SOC_TBCTR_ZERO);
    EPWM_setADCTriggerEventPrescale(PWM_BASE, ADC_TRG_SRC, 1);

    //
    // Start ePWM1, enabling SOCA and putting the counter in up-count mode
    //
    EPWM_enableADCTrigger(PWM_BASE, ADC_TRG_SRC);
}


static inline void initHRPWMxAB(void)
{

    HRPWM_setTimeBaseCounterMode(PWM_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);
    HRPWM_disablePhaseShiftLoad(PWM_BASE);
    HRPWM_setClockPrescaler(PWM_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);

    //
    // Set-up TBCLK
    HRPWM_setPeriodLoadMode(PWM_BASE, EPWM_PERIOD_SHADOW_LOAD);
    HRPWM_selectPeriodLoadEvent(PWM_BASE, EPWM_SHADOW_LOAD_MODE_COUNTER_ZERO);
    HRPWM_setTimeBasePeriod(PWM_BASE, HRPWM_CNTS);
    HRPWM_setPhaseShift(PWM_BASE, 0U);
    HRPWM_setTimeBaseCounter(PWM_BASE, 0U);

    //
    // set duty 0% initially
    HRPWM_setCounterCompareValue(PWM_BASE, HRPWM_COUNTER_COMPARE_A, 0);
    HRPWM_setCounterCompareValue(PWM_BASE, HRPWM_COUNTER_COMPARE_B, HRPWM_CNTS);

    //
    // ePWM1 SYNCO is generated on CTR=0
    HRPWM_setSyncOutPulseMode(PWM_BASE, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);

    //
    // Set up shadowing
    // Compare Value would load on count zero.
    HRPWM_setCounterCompareShadowLoadMode(PWM_BASE,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    HRPWM_setCounterCompareShadowLoadMode(PWM_BASE,
                                         EPWM_COUNTER_COMPARE_B,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    HRPWM_setCounterCompareShadowLoadEvent(PWM_BASE,
                                           HRPWM_CHANNEL_A,
                                           HRPWM_LOAD_ON_CNTR_ZERO);
    HRPWM_setCounterCompareShadowLoadEvent(PWM_BASE,
                                           HRPWM_CHANNEL_B,
                                           HRPWM_LOAD_ON_CNTR_ZERO);
    //
    // Set actions
    // CAU - Output A Low
    // CAD - Output A High
    // CBU - Output B High
    // CBD - Output B Low
    HRPWM_setActionQualifierAction(PWM_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    HRPWM_setActionQualifierAction(PWM_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    HRPWM_setActionQualifierAction(PWM_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

    HRPWM_setActionQualifierAction(PWM_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

    //
    // Align the HRPWM CMPA behavior with EPWM
    //
    HRPWM_setMEPEdgeSelect(PWM_BASE,
                           HRPWM_CHANNEL_A,
                           HRPWM_MEP_CTRL_FALLING_EDGE);

    HRPWM_setMEPControlMode(PWM_BASE,
                            HRPWM_CHANNEL_A,
                            HRPWM_MEP_DUTY_PERIOD_CTRL);

    HRPWM_setMEPEdgeSelect(PWM_BASE,
                           HRPWM_CHANNEL_B,
                           HRPWM_MEP_CTRL_FALLING_EDGE);

    HRPWM_setMEPControlMode(PWM_BASE,
                            HRPWM_CHANNEL_B,
                            HRPWM_MEP_DUTY_PERIOD_CTRL);

    HRPWM_enableAutoConversion(PWM_BASE);
    //
    // Enable EPWM

    HRPWM_setTimeBaseCounterMode(PWM_BASE, EPWM_COUNTER_MODE_UP_DOWN);


}

static inline void initPWM(void)
{

    uint16_t u16Status;
    //
    // Calibrate MEP_ScaleFactor
    do {
        u16Status = SFO();
    } while(u16Status == SFO_INCOMPLETE);

    //
    // Disable sync(Freeze clock to PWM as well)
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);


    initHRPWMxAB();

    //
    // Enable sync and clock to PWM
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    enableAdcTriggerbyPWM();
}


#endif /* INIT_PWM_H */
