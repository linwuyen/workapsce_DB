//###########################################################################
//
// FILE:    DSP_ADC_16bit_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210909
//###########################################################################

#include "Global_VariableDefs.h"

volatile DSP_ADC_16BIT_MODULE_VARIABLES_REG DSP_ADC_16bit_Module_Variables;




void Trig_DSP_ADC_16bit_Convert(void)
{
    //
    //convert, wait for completion, and store results
    //start conversions immediately via software, ADCA
    //
    AdcaRegs.ADCSOCFRC1.all = 0x0001; //SOC0

}







////////////////////////////////////////////////////////////////////
void Get_DSP_ADC_16bit_Data(void)
{

    //
    //wait for ADCA to complete, then acknowledge flag
    //
    while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
    {
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    }

    //
    //store results
    //
    DSP_ADC_16bit_Module_Variables.Channel1 = AdcaResultRegs.ADCRESULT0;

    //
    //at this point, conversion results are stored in
    //AdcaResult0, AdcaResult1, AdcbResult0, and AdcbResult1
    //

    //
    //software breakpoint, hit run again to get updated conversions
    //
    //asm("   ESTOP0");

}


////////////////////////////////////////////////////////////////////
void Init_DSP_ADC_16bit_Module_Variables(void)
{
    DSP_ADC_16bit_Module_Variables.Channel1 = 0;
}







////////////////////////////////////////////////////////////////////
void Init_DSP_ADC_16bit_Module(void)
{
    Uint16 acqps;

    EALLOW;

    //
    //write configurations
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4

    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_16BIT, ADC_SIGNALMODE_DIFFERENTIAL);

    //
    //Set pulse positions to late
    //
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //轉換完成再觸發INT

    //
    //power up the ADCs
    //
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //
    //delay for 1ms to allow ADC time to power up
    //
    DELAY_US(1000);

    //
    //determine minimum acquisition window (in SYSCLKS) based on resolution
    //
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
    {
        acqps = 14; //75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; //320ns
    }

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0x02;  //SOC0 will convert pin A2-A3
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared


    EDIS;

}



//
// End of file
//
