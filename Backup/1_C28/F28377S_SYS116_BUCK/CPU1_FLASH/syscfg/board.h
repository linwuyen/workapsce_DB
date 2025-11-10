/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// EPWM1 -> BUCK_PWM Pinmux
//
//
// EPWM1A - GPIO Settings
//
#define GPIO_PIN_EPWM1A 0
#define BUCK_PWM_EPWMA_GPIO 0
#define BUCK_PWM_EPWMA_PIN_CONFIG GPIO_0_EPWM1A
//
// EPWM1B - GPIO Settings
//
#define GPIO_PIN_EPWM1B 1
#define BUCK_PWM_EPWMB_GPIO 1
#define BUCK_PWM_EPWMB_PIN_CONFIG GPIO_1_EPWM1B
//
// GPIO22 - GPIO Settings
//
#define C28_WORK_GPIO_PIN_CONFIG GPIO_22_GPIO22
//
// GPIO23 - GPIO Settings
//
#define C28_ERROR_GPIO_PIN_CONFIG GPIO_23_GPIO23
//
// GPIO24 - GPIO Settings
//
#define CLA_WORK_GPIO_PIN_CONFIG GPIO_24_GPIO24
//
// GPIO19 - GPIO Settings
//
#define DEBUG_0_GPIO_PIN_CONFIG GPIO_19_GPIO19
//
// GPIO20 - GPIO Settings
//
#define DEBUG_1_GPIO_PIN_CONFIG GPIO_20_GPIO20
//
// GPIO21 - GPIO Settings
//
#define DEBUG_2_GPIO_PIN_CONFIG GPIO_21_GPIO21
//
// GPIO99 - GPIO Settings
//
#define DEBUG_3_GPIO_PIN_CONFIG GPIO_99_GPIO99
//
// GPIO42 - GPIO Settings
//
#define EL_UnReg_STATUS_GPIO_PIN_CONFIG GPIO_42_GPIO42
//
// GPIO47 - GPIO Settings
//
#define EL_OTP_STATUS_GPIO_PIN_CONFIG GPIO_47_GPIO47
//
// GPIO46 - GPIO Settings
//
#define EL_ON_OFF_GPIO_PIN_CONFIG GPIO_46_GPIO46
//
// GPIO57 - GPIO Settings
//
#define DAC8812_nLDAC_GPIO_PIN_CONFIG GPIO_57_GPIO57
//
// GPIO11 - GPIO Settings
//
#define PWM_LOAD_GPIO_PIN_CONFIG GPIO_11_GPIO11

//
// I2CA -> EEPROM Pinmux
//
//
// SDAA - GPIO Settings
//
#define GPIO_PIN_SDAA 91
#define EEPROM_I2CSDA_GPIO 91
#define EEPROM_I2CSDA_PIN_CONFIG GPIO_91_SDAA
//
// SCLA - GPIO Settings
//
#define GPIO_PIN_SCLA 92
#define EEPROM_I2CSCL_GPIO 92
#define EEPROM_I2CSCL_PIN_CONFIG GPIO_92_SCLA

//
// SCIA -> DEBUG_SCI Pinmux
//
//
// SCIRXDA - GPIO Settings
//
#define GPIO_PIN_SCIRXDA 85
#define DEBUG_SCI_SCIRX_GPIO 85
#define DEBUG_SCI_SCIRX_PIN_CONFIG GPIO_85_SCIRXDA
//
// SCITXDA - GPIO Settings
//
#define GPIO_PIN_SCITXDA 84
#define DEBUG_SCI_SCITX_GPIO 84
#define DEBUG_SCI_SCITX_PIN_CONFIG GPIO_84_SCITXDA

//
// SPIA -> DAC8812 Pinmux
//
//
// SPIA_PICO - GPIO Settings
//
#define GPIO_PIN_SPIA_PICO 58
#define DAC8812_SPIPICO_GPIO 58
#define DAC8812_SPIPICO_PIN_CONFIG GPIO_58_SPISIMOA
//
// SPIA_POCI - GPIO Settings
//
#define GPIO_PIN_SPIA_POCI 59
#define DAC8812_SPIPOCI_GPIO 59
#define DAC8812_SPIPOCI_PIN_CONFIG GPIO_59_SPISOMIA
//
// SPIA_CLK - GPIO Settings
//
#define GPIO_PIN_SPIA_CLK 60
#define DAC8812_SPICLK_GPIO 60
#define DAC8812_SPICLK_PIN_CONFIG GPIO_60_SPICLKA
//
// SPIA_PTE - GPIO Settings
//
#define GPIO_PIN_SPIA_PTE 61
#define DAC8812_SPIPTE_GPIO 61
#define DAC8812_SPIPTE_PIN_CONFIG GPIO_61_SPISTEA

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
#define AD_A_BASE ADCA_BASE
#define AD_A_RESULT_BASE ADCARESULT_BASE
#define AD_A_SOC0 ADC_SOC_NUMBER0
#define AD_A_FORCE_SOC0 ADC_FORCE_SOC0
#define AD_A_SAMPLE_WINDOW_SOC0 160
#define AD_A_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_EPWM1_SOCA
#define AD_A_CHANNEL_SOC0 ADC_CH_ADCIN2_ADCIN3
#define AD_A_SOC1 ADC_SOC_NUMBER1
#define AD_A_FORCE_SOC1 ADC_FORCE_SOC1
#define AD_A_SAMPLE_WINDOW_SOC1 160
#define AD_A_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_EPWM1_SOCA
#define AD_A_CHANNEL_SOC1 ADC_CH_ADCIN2_ADCIN3
#define AD_A_SOC2 ADC_SOC_NUMBER2
#define AD_A_FORCE_SOC2 ADC_FORCE_SOC2
#define AD_A_SAMPLE_WINDOW_SOC2 160
#define AD_A_TRIGGER_SOURCE_SOC2 ADC_TRIGGER_EPWM1_SOCA
#define AD_A_CHANNEL_SOC2 ADC_CH_ADCIN14_ADCIN15
#define AD_A_SOC3 ADC_SOC_NUMBER3
#define AD_A_FORCE_SOC3 ADC_FORCE_SOC3
#define AD_A_SAMPLE_WINDOW_SOC3 160
#define AD_A_TRIGGER_SOURCE_SOC3 ADC_TRIGGER_EPWM1_SOCA
#define AD_A_CHANNEL_SOC3 ADC_CH_ADCIN14_ADCIN15
void AD_A_init();

#define AD_B_BASE ADCB_BASE
#define AD_B_RESULT_BASE ADCBRESULT_BASE
#define AD_B_SOC0 ADC_SOC_NUMBER0
#define AD_B_FORCE_SOC0 ADC_FORCE_SOC0
#define AD_B_SAMPLE_WINDOW_SOC0 160
#define AD_B_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_EPWM1_SOCA
#define AD_B_CHANNEL_SOC0 ADC_CH_ADCIN2_ADCIN3
#define AD_B_SOC1 ADC_SOC_NUMBER1
#define AD_B_FORCE_SOC1 ADC_FORCE_SOC1
#define AD_B_SAMPLE_WINDOW_SOC1 160
#define AD_B_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_EPWM1_SOCA
#define AD_B_CHANNEL_SOC1 ADC_CH_ADCIN2_ADCIN3
#define AD_B_SOC2 ADC_SOC_NUMBER2
#define AD_B_FORCE_SOC2 ADC_FORCE_SOC2
#define AD_B_SAMPLE_WINDOW_SOC2 160
#define AD_B_TRIGGER_SOURCE_SOC2 ADC_TRIGGER_EPWM1_SOCA
#define AD_B_CHANNEL_SOC2 ADC_CH_ADCIN2_ADCIN3
#define AD_B_SOC3 ADC_SOC_NUMBER3
#define AD_B_FORCE_SOC3 ADC_FORCE_SOC3
#define AD_B_SAMPLE_WINDOW_SOC3 160
#define AD_B_TRIGGER_SOURCE_SOC3 ADC_TRIGGER_EPWM1_SOCA
#define AD_B_CHANNEL_SOC3 ADC_CH_ADCIN2_ADCIN3
void AD_B_init();

#define AD_C_BASE ADCC_BASE
#define AD_C_RESULT_BASE ADCCRESULT_BASE
#define AD_C_SOC0 ADC_SOC_NUMBER0
#define AD_C_FORCE_SOC0 ADC_FORCE_SOC0
#define AD_C_SAMPLE_WINDOW_SOC0 160
#define AD_C_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_EPWM1_SOCA
#define AD_C_CHANNEL_SOC0 ADC_CH_ADCIN2_ADCIN3
#define AD_C_SOC1 ADC_SOC_NUMBER1
#define AD_C_FORCE_SOC1 ADC_FORCE_SOC1
#define AD_C_SAMPLE_WINDOW_SOC1 160
#define AD_C_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_EPWM1_SOCA
#define AD_C_CHANNEL_SOC1 ADC_CH_ADCIN2_ADCIN3
#define AD_C_SOC2 ADC_SOC_NUMBER2
#define AD_C_FORCE_SOC2 ADC_FORCE_SOC2
#define AD_C_SAMPLE_WINDOW_SOC2 160
#define AD_C_TRIGGER_SOURCE_SOC2 ADC_TRIGGER_EPWM1_SOCA
#define AD_C_CHANNEL_SOC2 ADC_CH_ADCIN2_ADCIN3
#define AD_C_SOC3 ADC_SOC_NUMBER3
#define AD_C_FORCE_SOC3 ADC_FORCE_SOC3
#define AD_C_SAMPLE_WINDOW_SOC3 160
#define AD_C_TRIGGER_SOURCE_SOC3 ADC_TRIGGER_EPWM1_SOCA
#define AD_C_CHANNEL_SOC3 ADC_CH_ADCIN2_ADCIN3
void AD_C_init();

#define AD_D_BASE ADCD_BASE
#define AD_D_RESULT_BASE ADCDRESULT_BASE
#define AD_D_SOC0 ADC_SOC_NUMBER0
#define AD_D_FORCE_SOC0 ADC_FORCE_SOC0
#define AD_D_SAMPLE_WINDOW_SOC0 160
#define AD_D_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_EPWM1_SOCA
#define AD_D_CHANNEL_SOC0 ADC_CH_ADCIN14_ADCIN15
#define AD_D_SOC1 ADC_SOC_NUMBER1
#define AD_D_FORCE_SOC1 ADC_FORCE_SOC1
#define AD_D_SAMPLE_WINDOW_SOC1 160
#define AD_D_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_EPWM1_SOCA
#define AD_D_CHANNEL_SOC1 ADC_CH_ADCIN14_ADCIN15
#define AD_D_SOC2 ADC_SOC_NUMBER2
#define AD_D_FORCE_SOC2 ADC_FORCE_SOC2
#define AD_D_SAMPLE_WINDOW_SOC2 160
#define AD_D_TRIGGER_SOURCE_SOC2 ADC_TRIGGER_EPWM1_SOCA
#define AD_D_CHANNEL_SOC2 ADC_CH_ADCIN2_ADCIN3
#define AD_D_SOC3 ADC_SOC_NUMBER3
#define AD_D_FORCE_SOC3 ADC_FORCE_SOC3
#define AD_D_SAMPLE_WINDOW_SOC3 160
#define AD_D_TRIGGER_SOURCE_SOC3 ADC_TRIGGER_EPWM1_SOCA
#define AD_D_CHANNEL_SOC3 ADC_CH_ADCIN2_ADCIN3
void AD_D_init();


//*****************************************************************************
//
// CMPSS Configurations
//
//*****************************************************************************
#define IO_SENSE_BASE CMPSS1_BASE
#define IO_SENSE_HIGH_COMP_BASE CMPSS1_BASE    
#define IO_SENSE_LOW_COMP_BASE CMPSS1_BASE    
void IO_SENSE_init();
#define IO_SHARE_BASE CMPSS2_BASE
#define IO_SHARE_HIGH_COMP_BASE CMPSS2_BASE    
#define IO_SHARE_LOW_COMP_BASE CMPSS2_BASE    
void IO_SHARE_init();
#define VIN_SENSE_BASE CMPSS3_BASE
#define VIN_SENSE_HIGH_COMP_BASE CMPSS3_BASE    
#define VIN_SENSE_LOW_COMP_BASE CMPSS3_BASE    
void VIN_SENSE_init();
#define IL_SENSE_BASE CMPSS4_BASE
#define IL_SENSE_HIGH_COMP_BASE CMPSS4_BASE    
#define IL_SENSE_LOW_COMP_BASE CMPSS4_BASE    
void IL_SENSE_init();
#define OVP_SENSE_BASE CMPSS5_BASE
#define OVP_SENSE_HIGH_COMP_BASE CMPSS5_BASE    
#define OVP_SENSE_LOW_COMP_BASE CMPSS5_BASE    
void OVP_SENSE_init();
#define VO_SENSE_BASE CMPSS6_BASE
#define VO_SENSE_HIGH_COMP_BASE CMPSS6_BASE    
#define VO_SENSE_LOW_COMP_BASE CMPSS6_BASE    
void VO_SENSE_init();
#define VO_SHARE_BASE CMPSS7_BASE
#define VO_SHARE_HIGH_COMP_BASE CMPSS7_BASE    
#define VO_SHARE_LOW_COMP_BASE CMPSS7_BASE    
void VO_SHARE_init();
#define IEL_SENSE_BASE CMPSS8_BASE
#define IEL_SENSE_HIGH_COMP_BASE CMPSS8_BASE    
#define IEL_SENSE_LOW_COMP_BASE CMPSS8_BASE    
void IEL_SENSE_init();

//*****************************************************************************
//
// CPUTIMER Configurations
//
//*****************************************************************************
#define SWTIRMER_BASE CPUTIMER2_BASE
void SWTIRMER_init();

//*****************************************************************************
//
// DAC Configurations
//
//*****************************************************************************
#define DACA_TP209_BASE DACA_BASE
void DACA_TP209_init();
#define DACB_TP208_BASE DACB_BASE
void DACB_TP208_init();
#define DACC_TP210_BASE DACC_BASE
void DACC_TP210_init();

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
#define BUCK_PWM_BASE EPWM1_BASE
#define BUCK_PWM_TBPRD 0
#define BUCK_PWM_COUNTER_MODE EPWM_COUNTER_MODE_STOP_FREEZE
#define BUCK_PWM_TBPHS 0
#define BUCK_PWM_CMPA 0
#define BUCK_PWM_CMPB 0
#define BUCK_PWM_CMPC 0
#define BUCK_PWM_CMPD 0
#define BUCK_PWM_DBRED 0
#define BUCK_PWM_DBFED 0
#define BUCK_PWM_TZA_ACTION EPWM_TZ_ACTION_HIGH_Z
#define BUCK_PWM_TZB_ACTION EPWM_TZ_ACTION_HIGH_Z
#define BUCK_PWM_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define C28_WORK 22
void C28_WORK_init();
#define C28_ERROR 23
void C28_ERROR_init();
#define CLA_WORK 24
void CLA_WORK_init();
#define DEBUG_0 19
void DEBUG_0_init();
#define DEBUG_1 20
void DEBUG_1_init();
#define DEBUG_2 21
void DEBUG_2_init();
#define DEBUG_3 99
void DEBUG_3_init();
#define EL_UnReg_STATUS 42
void EL_UnReg_STATUS_init();
#define EL_OTP_STATUS 47
void EL_OTP_STATUS_init();
#define EL_ON_OFF 46
void EL_ON_OFF_init();
#define DAC8812_nLDAC 57
void DAC8812_nLDAC_init();
#define PWM_LOAD 11
void PWM_LOAD_init();

//*****************************************************************************
//
// I2C Configurations
//
//*****************************************************************************
#define EEPROM_BASE I2CA_BASE
#define EEPROM_BITRATE 400000
#define EEPROM_TARGET_ADDRESS 85
#define EEPROM_OWN_TARGET_ADDRESS 0
void EEPROM_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_AD_A_1
#define INT_AD_A_1 INT_ADCA1
#define INT_AD_A_1_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP1
extern __interrupt void INT_ADC_EOC_ISR(void);

//*****************************************************************************
//
// SCI Configurations
//
//*****************************************************************************
#define DEBUG_SCI_BASE SCIA_BASE
#define DEBUG_SCI_BAUDRATE 115200
#define DEBUG_SCI_CONFIG_WLEN SCI_CONFIG_WLEN_8
#define DEBUG_SCI_CONFIG_STOP SCI_CONFIG_STOP_ONE
#define DEBUG_SCI_CONFIG_PAR SCI_CONFIG_PAR_EVEN
void DEBUG_SCI_init();

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define DAC8812_BASE SPIA_BASE
#define DAC8812_BITRATE 10000000
void DAC8812_init();

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	ADC_init();
void	CMPSS_init();
void	CPUTIMER_init();
void	DAC_init();
void	EPWM_init();
void	GPIO_init();
void	I2C_init();
void	INTERRUPT_init();
void	SCI_init();
void	SPI_init();
void	SYNC_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
