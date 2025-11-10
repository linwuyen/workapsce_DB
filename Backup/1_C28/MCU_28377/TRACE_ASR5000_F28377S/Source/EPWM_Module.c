//###########################################################################
//
// FILE:    EPWM_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210902
//###########################################################################

#include "Global_VariableDefs.h"



////////////////////////////////////////////////////////////////////
void Init_EPWM_Module(void)
{
    //
    // Enable PWM1, PWM2 and PWM3
    //
    CpuSysRegs.PCLKCR2.bit.EPWM5=1;
    CpuSysRegs.PCLKCR2.bit.EPWM7=1;
    CpuSysRegs.PCLKCR2.bit.EPWM8=1;
    CpuSysRegs.PCLKCR2.bit.EPWM4=1;


    //Fan control, GPIO9, EPWM5B
    InitEPwm5Gpio();

    //CLK0, GPIO13, EPWM7B
    InitEPwm7Gpio();

    //CLK180, GPIO15, EPWM8B
    InitEPwm8Gpio();

    //
    // For this example, only initialize the ePWM
    //
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    //Fan control, GPIO9, EPWM5B
    Init_EPWM5_Reg();

    //CLK0, GPIO13, EPWM7B
    Init_EPWM7_Reg();

    //CLK180, GPIO15, EPWM8B
    Init_EPWM8_Reg();

    //計算CPU cycle
    Init_EPWM4_Reg();


    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

}


////////////////////////////////////////////////////////////////////
//
//Fan control, GPIO9, EPWM5B
//
void Init_EPWM5_Reg(void)
{
    //
    // Setup TBCLK
    //
    EPwm5Regs.TBPRD = EPWM5_TIMER_TBPRD - 1;   // Set timer period
    EPwm5Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm5Regs.TBCTR = 0x0000;                  // Clear counter

    //
    // Set Compare values
    //
    //EPwm5Regs.CMPB.bit.CMPB = EPWM5_TIMER_TBPRD * EPWM5_DUTY_OFF;  // Set Compare B value
    EPwm5Regs.CMPB.bit.CMPB = EPWM5_TIMER_TBPRD * FAN_STANDBY_DUTY;  // Set Compare B value
    //EPwm5Regs.CMPB.bit.CMPB = EPWM5_TIMER_TBPRD * 0.8;  // Set Compare B value
    //EPwm5Regs.CMPB.bit.CMPB = EPWM5_TIMER_TBPRD * 0;  // Set Compare B value

    //
    // Setup counter mode
    //
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;   // Count up
    EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;      // Disable phase loading
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;     // Clock ratio to SYSCLKOUT
    EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Setup shadowing
    //
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set actions
    //
    EPwm5Regs.AQCTLB.bit.PRD = AQ_SET;
    EPwm5Regs.AQCTLB.bit.CBU = AQ_CLEAR;

}

////////////////////////////////////////////////////////////////////
//
//CLK0, GPIO13, EPWM7B
//
void Init_EPWM7_Reg(void)
{
    //
    // Setup TBCLK
    //
    EPwm7Regs.TBPRD = EPWM7_TIMER_TBPRD - 1;        // Set timer period
    EPwm7Regs.TBPHS.bit.TBPHS = EPWM7_TIMER_TBPHS;  // Phase
    EPwm7Regs.TBCTR = 0x0000;                       // Clear counter

    //
    // Set Compare values
    //
    EPwm7Regs.CMPB.bit.CMPB = EPWM7_TIMER_TBPRD * EPWM7_DUTY_OFF;  // Set Compare B value

    //
    // Setup counter mode
    //
    EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;      // Count up
    EPwm7Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Enable phase loading
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Clock ratio to SYSCLKOUT
    EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm7Regs.TBCTL.bit.SWFSYNC = 1;               //
    EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;     //

    //
    // Setup shadowing
    //
    EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set actions
    //
    EPwm7Regs.AQCTLB.bit.PRD = AQ_SET;
    EPwm7Regs.AQCTLB.bit.CBU = AQ_CLEAR;


}

////////////////////////////////////////////////////////////////////
//
//CLK180, GPIO15, EPWM8B
//
void Init_EPWM8_Reg(void)
{
    //
    // Setup TBCLK
    //
    EPwm8Regs.TBPRD = EPWM8_TIMER_TBPRD - 1;        // Set timer period
    EPwm8Regs.TBPHS.bit.TBPHS = EPWM8_TIMER_TBPHS;  // Phase
    EPwm8Regs.TBCTR = 0x0000;                       // Clear counter

    //
    // Set Compare values
    //
    EPwm8Regs.CMPB.bit.CMPB = EPWM8_TIMER_TBPRD * EPWM8_DUTY_OFF;  // Set Compare B value

    //
    // Setup counter mode
    //
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;      // Count up
    EPwm8Regs.TBCTL.bit.PHSEN = TB_ENABLE;          // Enable phase loading
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Clock ratio to SYSCLKOUT
    EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm8Regs.TBCTL.bit.SWFSYNC = 1;               //
    EPwm8Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;     //

    //
    // Setup shadowing
    //
    EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set actions
    //
    EPwm8Regs.AQCTLB.bit.PRD = AQ_SET;
    EPwm8Regs.AQCTLB.bit.CBU = AQ_CLEAR;



}



////////////////////////////////////////////////////////////////////
//
// 計算CPU cycle, EPWM4
// EPwm4Regs.TBCTR每次累加1 = 2 cycle = 10ns
//
void Init_EPWM4_Reg(void)
{
    //
    // Setup TBCLK
    //
    EPwm4Regs.TBPRD = 0xFFFF;   // Set timer period
    EPwm4Regs.TBCTR = 0x0000;   // Clear counter

    //
    // Setup counter mode
    //
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;   // Count up
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;     // Clock ratio to SYSCLKOUT
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;

}





//
// End of file
//
