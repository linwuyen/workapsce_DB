/*
 * SPWM_Ctrl.c
 *
 *  Created on: May 27, 2024
 *      Author: roger_lin
 */
#include "common.h"
#include "C28ClaVars.h"
#include "SPWM_Ctrl.h"
typedef struct{
    uint32_t b0_pfc_ovp_n:1;
    uint32_t b1_pfc_uvp_n :1;
    uint32_t b2_pfc_ovp_2_n:1;
    uint32_t b3_pfc_uvp_2_n:1;
    uint32_t b4_pfc_otp_n:1;
    uint32_t b5_dc_ovp_n:1;
    uint32_t b6_ac_fail_5_n:1;

    uint32_t b7_fan_fail_1:1;
    uint32_t b8_fan_fail_2:1;
    uint32_t b9_fan_fail_3:1;
    uint32_t b10_fan_fail_4:1;
}st_Fake_singal;
st_Fake_singal Fake_singal;

ST_GSVAR sGbVars;

////===============================IO=======================

void scanFanFail(void)
{
    static uint16_t u16FanIo = 0;
    static uint16_t u16FanShadow = 0;
    static uint16_t u16Fan1Timeout = 0;
    static uint16_t u16Fan2Timeout = 0;
    static uint16_t u16Fan3Timeout = 0;
    static uint16_t u16Fan4Timeout = 0;

    u16FanIo  = (GPIO_readPin(FANFAIL1)<<0);
    u16FanIo |= (GPIO_readPin(FANFAIL2)<<1);
    u16FanIo |= (GPIO_readPin(FANFAIL3)<<2);
    u16FanIo |= (GPIO_readPin(FANFAIL4)<<3);

    if((u16FanShadow^u16FanIo) & (0x0001<<0)) {
        u16Fan1Timeout = 0;
        sParams.sDin.i_stat.b7_fan_fail_1 = 0;
    }
    else if(u16Fan1Timeout<1000){
        u16Fan1Timeout++;
    }
    else {
        sParams.sDin.i_stat.b7_fan_fail_1 = 1;
    }

    if((u16FanShadow^u16FanIo) & (0x0001<<1)) {
        u16Fan2Timeout = 0;
        sParams.sDin.i_stat.b8_fan_fail_2 = 0;
    }
    else if(u16Fan2Timeout<1000){
        u16Fan2Timeout++;
    }
    else {
        sParams.sDin.i_stat.b8_fan_fail_2 = 1;
    }

    if((u16FanShadow^u16FanIo) & (0x0001<<2)) {
        u16Fan3Timeout = 0;
        sParams.sDin.i_stat.b9_fan_fail_3 = 0;
    }
    else if(u16Fan3Timeout<1000){
        u16Fan3Timeout++;
    }
    else {
        sParams.sDin.i_stat.b9_fan_fail_3 = 1;
    }

    if((u16FanShadow^u16FanIo) & (0x0001<<3)) {
        u16Fan4Timeout = 0;
        sParams.sDin.i_stat.b10_fan_fail_4 = 0;
    }
    else if(u16Fan4Timeout<1000){
        u16Fan4Timeout++;
    }
    else {
        sParams.sDin.i_stat.b10_fan_fail_4 = 1;
    }

    u16FanShadow = u16FanIo;
}

void Update_IO(void)
{

#if (SCAN_ERROR_MODE == DISABLE_SCAN_ERROR)
    sParams.sDin.i_stat.b0_pfc_ovp_n   = 1;
    sParams.sDin.i_stat.b1_pfc_uvp_n   = 1;
    sParams.sDin.i_stat.b2_pfc_ovp_2_n = 0;
    sParams.sDin.i_stat.b3_pfc_uvp_2_n = 0;
    sParams.sDin.i_stat.b4_pfc_otp_n   = 1;
    sParams.sDin.i_stat.b5_dc_ovp_n    = 0;
    sParams.sDin.i_stat.b6_ac_fail_5_n = 1;
    sParams.sDin.i_stat.b7_fan_fail_1  = 0;
    sParams.sDin.i_stat.b8_fan_fail_2  = 0;
    sParams.sDin.i_stat.b9_fan_fail_3  = 0;
    sParams.sDin.i_stat.b10_fan_fail_4 = 0;
#elif (SCAN_ERROR_MODE == FEED_FACK_ERROR)
    sParams.sDin.i_stat.b0_pfc_ovp_n   = Fake_singal.b0_pfc_ovp_n;
    sParams.sDin.i_stat.b1_pfc_uvp_n   = Fake_singal.b1_pfc_uvp_n;
    sParams.sDin.i_stat.b2_pfc_ovp_2_n = Fake_singal.b2_pfc_ovp_2_n;
    sParams.sDin.i_stat.b3_pfc_uvp_2_n = Fake_singal.b3_pfc_uvp_2_n;
    sParams.sDin.i_stat.b4_pfc_otp_n   = Fake_singal.b4_pfc_otp_n;
    sParams.sDin.i_stat.b5_dc_ovp_n    = Fake_singal.b5_dc_ovp_n;
    sParams.sDin.i_stat.b6_ac_fail_5_n   = Fake_singal.b6_ac_fail_5_n;
    sParams.sDin.i_stat.b7_fan_fail_1  = Fake_singal.b7_fan_fail_1;
    sParams.sDin.i_stat.b8_fan_fail_2  = Fake_singal.b8_fan_fail_2;
    sParams.sDin.i_stat.b9_fan_fail_3  = Fake_singal.b9_fan_fail_3;
    sParams.sDin.i_stat.b10_fan_fail_4 = Fake_singal.b10_fan_fail_4;
#else
    sParams.sDin.i_stat.b0_pfc_ovp_n    = GPIO_readPin(PFC_OVP);
    sParams.sDin.i_stat.b1_pfc_uvp_n    = GPIO_readPin(PFC_UVP);
    sParams.sDin.i_stat.b2_pfc_ovp_2_n  = GPIO_readPin(PFC_OVP2);
    sParams.sDin.i_stat.b3_pfc_uvp_2_n  = GPIO_readPin(PFC_UVP2);
    sParams.sDin.i_stat.b4_pfc_otp_n    = GPIO_readPin(PFC_OTP);
    sParams.sDin.i_stat.b5_dc_ovp_n     = GPIO_readPin(DCOVP);
    sParams.sDin.i_stat.b6_ac_fail_5_n  = GPIO_readPin(ACFAIL5);
    scanFanFail();
#endif

   sParams.sDin.i_stat.b16_off_pwm_n  = sParams.sDout.o_stat.b16_off_pwm_n;
   sParams.sDin.i_stat.b17_master  = sParams.sDout.o_stat.b17_master;

   GPIO_writePin(OFF_PWMn  ,sParams.sDout.o_stat.b16_off_pwm_n);
   GPIO_writePin(EN_MASTER ,sParams.sDout.o_stat.b17_master);
}





//==============================ADC_LOOKUPTABLE===================================
#define ADC_MAX 4096
#define lookupTableSize  33
uint16 dutyCycle ;

int16_t TableAdc2DutyCnt[lookupTableSize] = {
  409, 409, 409, 409, 839, 1269, 1700, 1700,
  1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700,
  1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700,
  1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700
};


int16_t getAdc2DutyCnt(uint16_t adcValue) {
    float scale = (float)(lookupTableSize - 1) / ADC_MAX;
    float indexFloat = adcValue * scale;
    int16_t index = (int16_t)indexFloat;
    int16_t nextIndex = (index + 1 < lookupTableSize) ? index + 1 : index;

    int16_t value1 = TableAdc2DutyCnt[index];
    int16_t value2 = TableAdc2DutyCnt[nextIndex];

    float fraction = indexFloat - index;
    int16_t DutyCnt = value1 + (int16_t)((value2 - value1) * fraction);

    return DutyCnt;
}

uint16 cnvAdc2Duty() {

 //MCP3301 TEMP
   dutyCycle = getAdc2DutyCnt(sGbVars.u16TempC_AD);

   return dutyCycle;

}

////=================================ADC======================================


__interrupt void INT_CV_AD_1_ISR(void)
{
    sGbVars.u32Heartbeats++;
    sGbVars.u32Heartbeats &= 0x7FFFFFFF;
    sParams.sHeartbeats.u32Data = sGbVars.u32Heartbeats;

//CV_AD adcValue
    sGbVars.u16CV_AD = ADC_readResult(CV_AD_RESULT_BASE, ADC_SOC_NUMBER0);
    sGbVars.u16TempC_AD = ADC_readResult(MCP3301_TEMP_RESULT_BASE, ADC_SOC_NUMBER1);


    DAC_setShadowValue(CC_DA_BASE, sGbVars.u16CV_AD);
    sParams.sTemp_C.u16Data[0] = sGbVars.u16TempC_AD;
    sParams.sTemp_C.u16Data[1] = 0;

    Interrupt_clearACKGroup(INT_CV_AD_1_INTERRUPT_ACK_GROUP);
    ADC_clearInterruptStatus(CV_AD_BASE, ADC_INT_NUMBER1);
}



////===============================PWM=============================
typedef struct
{
    uint32_t u32BASE;
    uint16_t u16TBperiodCount;
    uint16_t u16count;
    uint16_t u16phaseCount;
    uint16_t u16CompCount;
} PWM_Config_st;
typedef PWM_Config_st *HAL_PWM_Config;


PWM_Config_st CLK0 = {
                       .u32BASE = EPWM1_BASE,
                       .u16TBperiodCount = 500,
                       .u16CompCount = 250,

};

PWM_Config_st CLK180 = {
                         .u32BASE = EPWM2_BASE,
                         .u16TBperiodCount = 500,
                         .u16CompCount = 250,
                         .u16phaseCount = 500,

};

PWM_Config_st FANCTRL5 = {
                         //========#include "board.h"
                         .u32BASE = EPWM3_BASE,
                         //25khz
                         .u16TBperiodCount = 2000,
                         //50% duty  DutyCtcle
                         .u16CompCount = 0,
                         .u16phaseCount = 0

};


void Setup_EPWM(HAL_PWM_Config p)
{
//TB
    EPWM_setClockPrescaler(p->u32BASE, EPWM_CLOCK_DIVIDER_1,
                           EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setTimeBasePeriod(p->u32BASE, p->u16TBperiodCount);
    EPWM_setTimeBaseCounter(p->u32BASE, p->u16count);
    EPWM_setTimeBaseCounterMode(p->u32BASE, EPWM_COUNTER_MODE_UP_DOWN);

//phase
    EPWM_forceSyncPulse(CLK0_5_BASE);
    EPWM_enablePhaseShiftLoad(CLK180_5_BASE);
    EPWM_setPhaseShift(p->u32BASE, p->u16phaseCount);

//CC
    EPWM_setCounterCompareValue(p->u32BASE, EPWM_COUNTER_COMPARE_A,
                                p->u16CompCount);
    EPWM_setCounterCompareShadowLoadMode(p->u32BASE, EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(p->u32BASE, EPWM_COUNTER_COMPARE_B,
                                p->u16CompCount);
    EPWM_setCounterCompareShadowLoadMode(p->u32BASE, EPWM_COUNTER_COMPARE_B,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

//AQ-A
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_NO_CHANGE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_NO_CHANGE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_NO_CHANGE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_NO_CHANGE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
//AQ-B
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_NO_CHANGE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_NO_CHANGE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_NO_CHANGE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(p->u32BASE, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_NO_CHANGE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
//DB

    EPWM_setRisingEdgeDelayCountShadowLoadMode(p->u32BASE,
                                               EPWM_RED_LOAD_ON_CNTR_ZERO);
    EPWM_disableRisingEdgeDelayCountShadowLoadMode(p->u32BASE);
    EPWM_setFallingEdgeDelayCountShadowLoadMode(p->u32BASE,
                                                EPWM_FED_LOAD_ON_CNTR_ZERO);
    EPWM_disableFallingEdgeDelayCountShadowLoadMode(p->u32BASE);
}

void init_EPWM(void)
{
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    Setup_EPWM(&CLK0);
    Setup_EPWM(&CLK180);
    Setup_EPWM(&FANCTRL5);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}


void updateFANCTRL5Duty( void)
{
    EPWM_setCounterCompareValue(EPWM3_BASE, EPWM_COUNTER_COMPARE_A, cnvAdc2Duty());

}
