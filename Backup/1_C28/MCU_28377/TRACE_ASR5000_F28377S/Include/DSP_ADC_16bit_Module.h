//###########################################################################
//
// FILE:    DSP_ADC_16bit_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210909
//###########################################################################


typedef struct DSP_ADC_16BIT_MODULE_VARIABLES_REG{

   Uint16  Channel1;                                         // ADC Channel 1  value

}DSP_ADC_16BIT_MODULE_VARIABLES_REG;




extern volatile DSP_ADC_16BIT_MODULE_VARIABLES_REG DSP_ADC_16bit_Module_Variables;


void Init_DSP_ADC_16bit_Module(void);
void Init_DSP_ADC_16bit_Module_Variables(void);

void Get_DSP_ADC_16bit_Data(void);
void Trig_DSP_ADC_16bit_Convert(void);

//
// End of file
//
