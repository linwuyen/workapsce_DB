//###########################################################################
//
// FILE:    eCAP_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20230613
//###########################################################################

#include "Global_VariableDefs.h"






////////////////////////////////////////////////////////////////////
void Init_eCAP_Reg(void)
{

    //InitECap1Gpio(99);

    EALLOW;
    InputXbarRegs.INPUT7SELECT = 99;       // Set eCAP1 source to GPIO-pin

    GpioCtrlRegs.GPDPUD.bit.GPIO99 = 0; // Enable pull-up
    GpioCtrlRegs.GPDDIR.bit.GPIO99 = 0; // input
    //GpioCtrlRegs.GPDCTRL.bit.QUALPRD3 = 200;    //(200*2)/200M = 2uSec
    GpioCtrlRegs.GPDCTRL.bit.QUALPRD3 = 255;    //(255*2)/200M = 2.55uSec
    GpioCtrlRegs.GPDQSEL1.bit.GPIO99= 2;    //Qualification (6 samples)
    EDIS;

    //
    // ECAP module 1 config
    //
    ECap1Regs.ECCTL1.bit.CAP1POL = 0;   //EC_RISING;
    ECap1Regs.ECCTL1.bit.CAP2POL = 1;   //EC_FALLING;
    ECap1Regs.ECCTL1.bit.CAP3POL = 0;   //EC_RISING;
    ECap1Regs.ECCTL1.bit.CAP4POL = 1;   //EC_FALLING;
    ECap1Regs.ECCTL1.bit.CTRRST1 = 1;   //EC_DELTA_MODE;
    ECap1Regs.ECCTL1.bit.CTRRST2 = 1;   //EC_DELTA_MODE;
    ECap1Regs.ECCTL1.bit.CTRRST3 = 1;   //EC_DELTA_MODE;
    ECap1Regs.ECCTL1.bit.CTRRST4 = 1;   //EC_DELTA_MODE;
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;   //EC_ENABLE;
    ECap1Regs.ECCTL1.bit.PRESCALE = 0;  //EC_DIV1;

    ECap1Regs.ECCTL2.bit.CAP_APWM = 0;  //EC_CAP_MODE;
    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;   //EC_CONTINUOUS;
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 2; //3;//EC_SYNCO_DIS;
    ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;  //EC_DISABLE;
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1; //EC_RUN;

}










//
// End of file
//
