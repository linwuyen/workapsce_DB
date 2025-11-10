/*
 * ControlPWM.c
 *
 *  Created on: 2024¦~2¤ë19¤é
 *      Author: roger_lin
 */
#include "ControlPWM.h"

HAL_DRV p = &ST_PWM_Ctl;

ST_PWM ST_PWM_Ctl =
        {
         .PWMfsm = _NO_ACTION,
         .u16TBPeriodCounter =0,
         .TBCountMode =EPWM_COUNTER_MODE_UP,

         .TBPulseMode= EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO,
         .CC_AcompCount = 50,
         .CC_BcompCount = 50
        };


void PWM_CONTROL (void)
{   //TB

    EPWM_setTimeBasePeriod(EPWM1_BASE, p->u16TBPeriodCounter);
    EPWM_setTimeBaseCounter(EPWM1_BASE, p->u16TBCounter);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_disablePhaseShiftLoad(EPWM1_BASE);
    EPWM_setPhaseShift(EPWM1_BASE,p->u16TBPeriodCounter);
    EPWM_setSyncOutPulseMode(EPWM1_BASE, p->TBPulseMode);
    EPWM_setCountModeAfterSync(EPWM1_BASE,EPWM_COUNT_MODE_UP_AFTER_SYNC);



    //CC_A
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, p->CC_AcompCount);
    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    //CC_B
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_B, p->CC_BcompCount);
    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);

    //AQ_A
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

    //AQ_B
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

    //DB
//    EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_RED, true);
//    EPWM_setRisingEdgeDelayCountShadowLoadMode(EPWM1_BASE, EPWM_RED_LOAD_ON_CNTR_ZERO);
//    EPWM_disableRisingEdgeDelayCountShadowLoadMode(EPWM1_BASE);
//    EPWM_setRisingEdgeDelayCount(EPWM1_BASE, 1000);
//    EPWM_setFallingEdgeDelayCountShadowLoadMode(EPWM1_BASE, EPWM_FED_LOAD_ON_CNTR_ZERO);
//    EPWM_disableFallingEdgeDelayCountShadowLoadMode(EPWM1_BASE);
//    EPWM_enableInterrupt(EPWM1_BASE);
//    EPWM_setInterruptEventCount(EPWM1_BASE, 1);
}
void PWMConfig(void)
{





}


void init_DAC (void)
{


}

//__interrupt
__interrupt void INT_myADC0_1_ISR(void)
{
 //   ADC_forceMultipleSOC(myADC0_BASE, (ADC_FORCE_SOC0 | ADC_FORCE_SOC1));

       GPIO_togglePin(myGPIO0);

       ST_PWM_Ctl.ADC0Results = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

       // Clear the interrupt flag
       //
       ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1);
       ADC_clearInterruptOverflowStatus(myADC0_BASE, ADC_INT_NUMBER1);
       //
       // Acknowledge the interrupt
       //
       Interrupt_clearACKGroup(INT_myADC0_1_INTERRUPT_ACK_GROUP);
}

void init_ADC (void)
{
    // ADC trigger

    // Disable SOCA
    EPWM_disableADCTrigger(EPWM1_BASE, EPWM_SOC_A);

    // Configure the SOC to occur on the first up-count event
    EPWM_setADCTriggerSource(EPWM1_BASE,EPWM_SOC_A,EPWM_SOC_TBCTR_ZERO);
//    EPWM_setADCTriggerSource(EPWM1_BASE,EPWM_SOC_A,EPWM_SOC_TBCTR_U_CMPA);

 //   EPWM_setADCTriggerEventCountInitValue(EPWM1_BASE,EPWM_SOC_A,1);
    EPWM_setADCTriggerEventPrescale(EPWM1_BASE, EPWM_SOC_A, 1);


    // Enable SOCA
    EPWM_enableADCTrigger(EPWM1_BASE,EPWM_SOC_A);

    ADC_forceSOC(myADC0_BASE,ADC_SOC_NUMBER0);
}

void init_DB(void)
{
    // Use EPWMA as the input for both RED and FED
       EPWM_setRisingEdgeDeadBandDelayInput(EPWM1_BASE, EPWM_DB_INPUT_EPWMA);
       EPWM_setFallingEdgeDeadBandDelayInput(EPWM1_BASE, EPWM_DB_INPUT_EPWMA);

       //
       // INVERT the delayed outputs (AL)
       //
       EPWM_setDeadBandDelayPolarity(EPWM1_BASE, EPWM_DB_RED, EPWM_DB_POLARITY_ACTIVE_HIGH);
       EPWM_setDeadBandDelayPolarity(EPWM1_BASE, EPWM_DB_FED, EPWM_DB_POLARITY_ACTIVE_LOW);

       //
       // Use the delayed signals instead of the original signals
       //
       EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_RED, true);
       EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_FED, true);

       //
       // DO NOT Switch Output A with Output B
       //
//       EPWM_setDeadBandOutputSwapMode(EPWM1_BASE, EPWM_DB_OUTPUT_A, false);
//       EPWM_setDeadBandOutputSwapMode(EPWM1_BASE, EPWM_DB_OUTPUT_B, false);

}

void init_Epwm (void)
{
//100K
//EPWM_COUNTER_MODE_UP_DOWN  PERIOD 500
//EPWM_COUNTER_MODE_UP EPWM_COUNTER_MODE_DOWN    PERIOD 10000 or 999

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //TB
    EPWM_setClockPrescaler(EPWM1_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);

    // Freeze the counter
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);

    EPWM_setTimeBasePeriod(EPWM1_BASE, 500);

    //CC CMPA
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, 250);
    EPWM_enableGlobalLoadRegisters(EPWM1_BASE, EPWM_GL_REGISTER_TBPRD_TBPRDHR);
    //CC CMPB
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_B, 250);
    EPWM_enableGlobalLoadRegisters(EPWM1_BASE, EPWM_GL_REGISTER_TBPRD_TBPRDHR);

    //AQ CMPA
    EPWM_setActionQualifierAction(EPWM1_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_HIGH,EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM1_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_LOW,EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE,EPWM_AQ_OUTPUT_A,EPWM_AQ_OUTPUT_HIGH,EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    //AQ CMPB
    EPWM_setActionQualifierAction(EPWM1_BASE,EPWM_AQ_OUTPUT_B,EPWM_AQ_OUTPUT_HIGH,EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM1_BASE,EPWM_AQ_OUTPUT_B,EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(EPWM1_BASE,EPWM_AQ_OUTPUT_B,EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);


    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    init_ADC();
    init_DB();

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}

void tstPWMFUN (void)
{
    switch(p->PWMfsm)
    {

    case _NO_ACTION:
        break;

    case _Enable_PWM:
     //   init_Epwm();
        PWM_CONTROL();
     //   p->PWMfsm = _NO_ACTION;
        break;

    case _Enable_ADC:
        init_ADC();
        p->PWMfsm = _NO_ACTION;
        break;

    case _Enable_DB:
        init_DB();
        break;

    case _PWM_CONFIG:
        PWMConfig();
        break;

    case _Enable_DAC:
      //  init_DAC();

        p->PWMfsm = _NO_ACTION;
        break;
    }
}
