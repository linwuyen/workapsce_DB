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

#include "board.h"

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void Board_init()
{
	EALLOW;

	PinMux_init();
	SYNC_init();
	ADC_init();
	CMPSS_init();
	CPUTIMER_init();
	DAC_init();
	EPWM_init();
	GPIO_init();
	I2C_init();
	SCI_init();
	SPI_init();
	INTERRUPT_init();

	EDIS;
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init()
{
	//
	// PinMux for modules assigned to CPU1
	//
	
	//
	// EPWM4 -> BUCK_PWM Pinmux
	//
	GPIO_setPinConfig(BUCK_PWM_EPWMA_PIN_CONFIG);
	GPIO_setPadConfig(BUCK_PWM_EPWMA_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(BUCK_PWM_EPWMA_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(BUCK_PWM_EPWMB_PIN_CONFIG);
	GPIO_setPadConfig(BUCK_PWM_EPWMB_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(BUCK_PWM_EPWMB_GPIO, GPIO_QUAL_SYNC);

	// GPIO22 -> C28_WORK Pinmux
	GPIO_setPinConfig(GPIO_22_GPIO22);
	// GPIO23 -> C28_ERROR Pinmux
	GPIO_setPinConfig(GPIO_23_GPIO23);
	// GPIO24 -> CLA_WORK Pinmux
	GPIO_setPinConfig(GPIO_24_GPIO24);
	// GPIO0 -> DEBUG_0 Pinmux
	GPIO_setPinConfig(GPIO_0_GPIO0);
	// GPIO20 -> DEBUG_1 Pinmux
	GPIO_setPinConfig(GPIO_20_GPIO20);
	// GPIO21 -> DEBUG_2 Pinmux
	GPIO_setPinConfig(GPIO_21_GPIO21);
	// GPIO99 -> DEBUG_3 Pinmux
	GPIO_setPinConfig(GPIO_99_GPIO99);
	// GPIO19 -> EL_UnReg_STATUS Pinmux
	GPIO_setPinConfig(GPIO_19_GPIO19);
	// GPIO47 -> EL_OTP_STATUS Pinmux
	GPIO_setPinConfig(GPIO_47_GPIO47);
	// GPIO46 -> EL_ON_OFF Pinmux
	GPIO_setPinConfig(GPIO_46_GPIO46);
	// GPIO57 -> DAC8812_nLDAC Pinmux
	GPIO_setPinConfig(GPIO_57_GPIO57);
	// GPIO11 -> PWM_LOAD Pinmux
	GPIO_setPinConfig(GPIO_11_GPIO11);
	//
	// I2CA -> EEPROM Pinmux
	//
	GPIO_setPinConfig(EEPROM_I2CSDA_PIN_CONFIG);
	GPIO_setPadConfig(EEPROM_I2CSDA_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(EEPROM_I2CSDA_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(EEPROM_I2CSCL_PIN_CONFIG);
	GPIO_setPadConfig(EEPROM_I2CSCL_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(EEPROM_I2CSCL_GPIO, GPIO_QUAL_ASYNC);

	//
	// SCIA -> DEBUG_SCI Pinmux
	//
	GPIO_setPinConfig(DEBUG_SCI_SCIRX_PIN_CONFIG);
	GPIO_setPadConfig(DEBUG_SCI_SCIRX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(DEBUG_SCI_SCIRX_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(DEBUG_SCI_SCITX_PIN_CONFIG);
	GPIO_setPadConfig(DEBUG_SCI_SCITX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(DEBUG_SCI_SCITX_GPIO, GPIO_QUAL_ASYNC);

	//
	// SPIA -> DAC8812 Pinmux
	//
	GPIO_setPinConfig(DAC8812_SPIPICO_PIN_CONFIG);
	GPIO_setPadConfig(DAC8812_SPIPICO_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DAC8812_SPIPICO_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(DAC8812_SPIPOCI_PIN_CONFIG);
	GPIO_setPadConfig(DAC8812_SPIPOCI_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DAC8812_SPIPOCI_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(DAC8812_SPICLK_PIN_CONFIG);
	GPIO_setPadConfig(DAC8812_SPICLK_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DAC8812_SPICLK_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(DAC8812_SPIPTE_PIN_CONFIG);
	GPIO_setPadConfig(DAC8812_SPIPTE_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DAC8812_SPIPTE_GPIO, GPIO_QUAL_ASYNC);


}

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
void ADC_init(){
	AD_A_init();
	AD_B_init();
	AD_C_init();
}

void AD_A_init(){
	//
	// ADC Initialization: Write ADC configurations and power up the ADC
	//
	// Configures the analog-to-digital converter module prescaler.
	//
	ADC_setPrescaler(AD_A_BASE, ADC_CLK_DIV_4_0);
	//
	// Configures the analog-to-digital converter resolution and signal mode.
	//
	ADC_setMode(AD_A_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
	//
	// Sets the timing of the end-of-conversion pulse
	//
	ADC_setInterruptPulseMode(AD_A_BASE, ADC_PULSE_END_OF_CONV);
	//
	// Powers up the analog-to-digital converter core.
	//
	ADC_enableConverter(AD_A_BASE);
	//
	// Delay for 1ms to allow ADC time to power up
	//
	DEVICE_DELAY_US(500);
	//
	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	//
	// Disables SOC burst mode.
	//
	ADC_disableBurstMode(AD_A_BASE);
	//
	// Sets the priority mode of the SOCs.
	//
	ADC_setSOCPriority(AD_A_BASE, ADC_PRI_ALL_ROUND_ROBIN);
	//
	// Start of Conversion 0 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_A_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_A_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 1 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 1
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_A_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_A_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 2 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 2
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_A_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_A_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 3 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 3
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_A_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_A_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_NONE);
	//
	// ADC Interrupt 1 Configuration
	// 		SOC/EOC number	: 3
	// 		Interrupt Source: enabled
	// 		Continuous Mode	: disabled
	//
	ADC_setInterruptSource(AD_A_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER3);
	ADC_enableInterrupt(AD_A_BASE, ADC_INT_NUMBER1);
	ADC_clearInterruptStatus(AD_A_BASE, ADC_INT_NUMBER1);
	ADC_disableContinuousMode(AD_A_BASE, ADC_INT_NUMBER1);
}
void AD_B_init(){
	//
	// ADC Initialization: Write ADC configurations and power up the ADC
	//
	// Configures the analog-to-digital converter module prescaler.
	//
	ADC_setPrescaler(AD_B_BASE, ADC_CLK_DIV_4_0);
	//
	// Configures the analog-to-digital converter resolution and signal mode.
	//
	ADC_setMode(AD_B_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
	//
	// Sets the timing of the end-of-conversion pulse
	//
	ADC_setInterruptPulseMode(AD_B_BASE, ADC_PULSE_END_OF_CONV);
	//
	// Powers up the analog-to-digital converter core.
	//
	ADC_enableConverter(AD_B_BASE);
	//
	// Delay for 1ms to allow ADC time to power up
	//
	DEVICE_DELAY_US(500);
	//
	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	//
	// Disables SOC burst mode.
	//
	ADC_disableBurstMode(AD_B_BASE);
	//
	// Sets the priority mode of the SOCs.
	//
	ADC_setSOCPriority(AD_B_BASE, ADC_PRI_ALL_ROUND_ROBIN);
	//
	// Start of Conversion 0 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_B_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_B_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 1 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 1
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_B_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_B_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 2 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 2
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_B_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_B_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 3 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 3
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_B_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_B_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_NONE);
}
void AD_C_init(){
	//
	// ADC Initialization: Write ADC configurations and power up the ADC
	//
	// Configures the analog-to-digital converter module prescaler.
	//
	ADC_setPrescaler(AD_C_BASE, ADC_CLK_DIV_4_0);
	//
	// Configures the analog-to-digital converter resolution and signal mode.
	//
	ADC_setMode(AD_C_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
	//
	// Sets the timing of the end-of-conversion pulse
	//
	ADC_setInterruptPulseMode(AD_C_BASE, ADC_PULSE_END_OF_CONV);
	//
	// Powers up the analog-to-digital converter core.
	//
	ADC_enableConverter(AD_C_BASE);
	//
	// Delay for 1ms to allow ADC time to power up
	//
	DEVICE_DELAY_US(500);
	//
	// SOC Configuration: Setup ADC EPWM channel and trigger settings
	//
	// Disables SOC burst mode.
	//
	ADC_disableBurstMode(AD_C_BASE);
	//
	// Sets the priority mode of the SOCs.
	//
	ADC_setSOCPriority(AD_C_BASE, ADC_PRI_ALL_ROUND_ROBIN);
	//
	// Start of Conversion 0 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 0
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_C_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_C_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 1 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 1
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN4
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_C_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN4, 32U);
	ADC_setInterruptSOCTrigger(AD_C_BASE, ADC_SOC_NUMBER1, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 2 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 2
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN5
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_C_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN5, 32U);
	ADC_setInterruptSOCTrigger(AD_C_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);
	//
	// Start of Conversion 3 Configuration
	//
	//
	// Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
	// 	  	SOC number		: 3
	//	  	Trigger			: ADC_TRIGGER_EPWM4_SOCA
	//	  	Channel			: ADC_CH_ADCIN5
	//	 	Sample Window	: 32 SYSCLK cycles
	//		Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
	//
	ADC_setupSOC(AD_C_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM4_SOCA, ADC_CH_ADCIN5, 32U);
	ADC_setInterruptSOCTrigger(AD_C_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_NONE);
}

//*****************************************************************************
//
// CMPSS Configurations
//
//*****************************************************************************
void CMPSS_init(){
	IO_SENSE_init();
	IO_SHARE_init();
	VIN_SENSE_init();
	IL_SENSE_init();
	OVP_SENSE_init();
	VO_SENSE_init();
	VO_SHARE_init();
	IEL_SENSE_init();
}

void IO_SENSE_init(){
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(IO_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(IO_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(IO_SENSE_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(IO_SENSE_BASE,0U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(IO_SENSE_BASE,0U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(IO_SENSE_BASE, 0U, 1U, 1U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(IO_SENSE_BASE, 0U, 1U, 1U);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(IO_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(IO_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(IO_SENSE_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(IO_SENSE_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(IO_SENSE_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(IO_SENSE_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(IO_SENSE_BASE,false,false);
    //
    // Disables the CMPSS module.
    //
    CMPSS_disableModule(IO_SENSE_BASE);

    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
}
void IO_SHARE_init(){
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(IO_SHARE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(IO_SHARE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(IO_SHARE_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(IO_SHARE_BASE,0U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(IO_SHARE_BASE,0U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(IO_SHARE_BASE, 0U, 1U, 1U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(IO_SHARE_BASE, 0U, 1U, 1U);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(IO_SHARE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(IO_SHARE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(IO_SHARE_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(IO_SHARE_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(IO_SHARE_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(IO_SHARE_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(IO_SHARE_BASE,false,false);
    //
    // Disables the CMPSS module.
    //
    CMPSS_disableModule(IO_SHARE_BASE);

    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
}
void VIN_SENSE_init(){
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(VIN_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(VIN_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(VIN_SENSE_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(VIN_SENSE_BASE,0U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(VIN_SENSE_BASE,0U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(VIN_SENSE_BASE, 0U, 1U, 1U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(VIN_SENSE_BASE, 0U, 1U, 1U);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(VIN_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(VIN_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(VIN_SENSE_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(VIN_SENSE_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(VIN_SENSE_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(VIN_SENSE_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(VIN_SENSE_BASE,false,false);
    //
    // Disables the CMPSS module.
    //
    CMPSS_disableModule(VIN_SENSE_BASE);

    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
}
void IL_SENSE_init(){
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(IL_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(IL_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(IL_SENSE_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(IL_SENSE_BASE,0U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(IL_SENSE_BASE,0U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(IL_SENSE_BASE, 0U, 1U, 1U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(IL_SENSE_BASE, 0U, 1U, 1U);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(IL_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(IL_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(IL_SENSE_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(IL_SENSE_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(IL_SENSE_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(IL_SENSE_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(IL_SENSE_BASE,false,false);
    //
    // Disables the CMPSS module.
    //
    CMPSS_disableModule(IL_SENSE_BASE);

    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
}
void OVP_SENSE_init(){
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(OVP_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(OVP_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(OVP_SENSE_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(OVP_SENSE_BASE,0U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(OVP_SENSE_BASE,0U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(OVP_SENSE_BASE, 0U, 1U, 1U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(OVP_SENSE_BASE, 0U, 1U, 1U);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(OVP_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(OVP_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(OVP_SENSE_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(OVP_SENSE_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(OVP_SENSE_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(OVP_SENSE_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(OVP_SENSE_BASE,false,false);
    //
    // Disables the CMPSS module.
    //
    CMPSS_disableModule(OVP_SENSE_BASE);

    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
}
void VO_SENSE_init(){
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(VO_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(VO_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(VO_SENSE_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(VO_SENSE_BASE,0U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(VO_SENSE_BASE,0U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(VO_SENSE_BASE, 0U, 1U, 1U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(VO_SENSE_BASE, 0U, 1U, 1U);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(VO_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(VO_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(VO_SENSE_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(VO_SENSE_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(VO_SENSE_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(VO_SENSE_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(VO_SENSE_BASE,false,false);
    //
    // Disables the CMPSS module.
    //
    CMPSS_disableModule(VO_SENSE_BASE);

    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
}
void VO_SHARE_init(){
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(VO_SHARE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(VO_SHARE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(VO_SHARE_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(VO_SHARE_BASE,0U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(VO_SHARE_BASE,0U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(VO_SHARE_BASE, 0U, 1U, 1U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(VO_SHARE_BASE, 0U, 1U, 1U);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(VO_SHARE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(VO_SHARE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(VO_SHARE_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(VO_SHARE_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(VO_SHARE_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(VO_SHARE_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(VO_SHARE_BASE,false,false);
    //
    // Disables the CMPSS module.
    //
    CMPSS_disableModule(VO_SHARE_BASE);

    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
}
void IEL_SENSE_init(){
    //
    // Sets the configuration for the high comparator.
    //
    CMPSS_configHighComparator(IEL_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the low comparator.
    //
    CMPSS_configLowComparator(IEL_SENSE_BASE,(CMPSS_INSRC_DAC));
    //
    // Sets the configuration for the internal comparator DACs.
    //
    CMPSS_configDAC(IEL_SENSE_BASE,(CMPSS_DACVAL_SYSCLK | CMPSS_DACREF_VDDA | CMPSS_DACSRC_SHDW));
    //
    // Sets the value of the internal DAC of the high comparator.
    //
    CMPSS_setDACValueHigh(IEL_SENSE_BASE,0U);
    //
    // Sets the value of the internal DAC of the low comparator.
    //
    CMPSS_setDACValueLow(IEL_SENSE_BASE,0U);
    //
    //  Configures the digital filter of the high comparator.
    //
    CMPSS_configFilterHigh(IEL_SENSE_BASE, 0U, 1U, 1U);
    //
    // Configures the digital filter of the low comparator.
    //
    CMPSS_configFilterLow(IEL_SENSE_BASE, 0U, 1U, 1U);
    //
    // Sets the output signal configuration for the high comparator.
    //
    CMPSS_configOutputsHigh(IEL_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the output signal configuration for the low comparator.
    //
    CMPSS_configOutputsLow(IEL_SENSE_BASE,(CMPSS_TRIPOUT_ASYNC_COMP | CMPSS_TRIP_ASYNC_COMP));
    //
    // Sets the comparator hysteresis settings.
    //
    CMPSS_setHysteresis(IEL_SENSE_BASE,0U);
    //
    // Configures the comparator subsystem's ramp generator.
    //
    CMPSS_configRamp(IEL_SENSE_BASE,0U,0U,0U,1U,true);
    //
    // Disables reset of HIGH comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCHigh(IEL_SENSE_BASE);
    //
    // Disables reset of LOW comparator digital filter output latch on PWMSYNC
    //
    CMPSS_disableLatchResetOnPWMSYNCLow(IEL_SENSE_BASE);
    //
    // Configures whether or not the digital filter latches are reset by PWMSYNC
    //
    CMPSS_configLatchOnPWMSYNC(IEL_SENSE_BASE,false,false);
    //
    // Disables the CMPSS module.
    //
    CMPSS_disableModule(IEL_SENSE_BASE);

    //
    // Delay for CMPSS DAC to power up.
    //
    DEVICE_DELAY_US(500);
}

//*****************************************************************************
//
// CPUTIMER Configurations
//
//*****************************************************************************
void CPUTIMER_init(){
	SWTIRMER_init();
}

void SWTIRMER_init(){
	CPUTimer_setEmulationMode(SWTIRMER_BASE, CPUTIMER_EMULATIONMODE_RUNFREE);
	CPUTimer_selectClockSource(SWTIRMER_BASE, CPUTIMER_CLOCK_SOURCE_SYS, CPUTIMER_CLOCK_PRESCALER_1);
	CPUTimer_setPreScaler(SWTIRMER_BASE, 3U);
	CPUTimer_setPeriod(SWTIRMER_BASE, 50000000U);
	CPUTimer_disableInterrupt(SWTIRMER_BASE);
	CPUTimer_stopTimer(SWTIRMER_BASE);

	CPUTimer_reloadTimerCounter(SWTIRMER_BASE);
	CPUTimer_startTimer(SWTIRMER_BASE);
}

//*****************************************************************************
//
// DAC Configurations
//
//*****************************************************************************
void DAC_init(){
	DACA_TP209_init();
	DACB_TP208_init();
	DACC_TP210_init();
}

void DACA_TP209_init(){
	//
	// Set DAC reference voltage.
	//
	DAC_setReferenceVoltage(DACA_TP209_BASE, DAC_REF_ADC_VREFHI);
	//
	// Set DAC load mode.
	//
	DAC_setLoadMode(DACA_TP209_BASE, DAC_LOAD_SYSCLK);
	//
	// Enable the DAC output
	//
	DAC_enableOutput(DACA_TP209_BASE);
	//
	// Set the DAC shadow output
	//
	DAC_setShadowValue(DACA_TP209_BASE, 0U);

	//
	// Delay for buffered DAC to power up.
	//
	DEVICE_DELAY_US(500);
}
void DACB_TP208_init(){
	//
	// Set DAC reference voltage.
	//
	DAC_setReferenceVoltage(DACB_TP208_BASE, DAC_REF_ADC_VREFHI);
	//
	// Set DAC load mode.
	//
	DAC_setLoadMode(DACB_TP208_BASE, DAC_LOAD_SYSCLK);
	//
	// Enable the DAC output
	//
	DAC_enableOutput(DACB_TP208_BASE);
	//
	// Set the DAC shadow output
	//
	DAC_setShadowValue(DACB_TP208_BASE, 0U);

	//
	// Delay for buffered DAC to power up.
	//
	DEVICE_DELAY_US(500);
}
void DACC_TP210_init(){
	//
	// Set DAC reference voltage.
	//
	DAC_setReferenceVoltage(DACC_TP210_BASE, DAC_REF_ADC_VREFHI);
	//
	// Set DAC load mode.
	//
	DAC_setLoadMode(DACC_TP210_BASE, DAC_LOAD_SYSCLK);
	//
	// Enable the DAC output
	//
	DAC_enableOutput(DACC_TP210_BASE);
	//
	// Set the DAC shadow output
	//
	DAC_setShadowValue(DACC_TP210_BASE, 0U);

	//
	// Delay for buffered DAC to power up.
	//
	DEVICE_DELAY_US(500);
}

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
void EPWM_init(){
    EPWM_setClockPrescaler(BUCK_PWM_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);	
    EPWM_setTimeBasePeriod(BUCK_PWM_BASE, 0);	
    EPWM_setTimeBaseCounter(BUCK_PWM_BASE, 0);	
    EPWM_setTimeBaseCounterMode(BUCK_PWM_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);	
    EPWM_disablePhaseShiftLoad(BUCK_PWM_BASE);	
    EPWM_setPhaseShift(BUCK_PWM_BASE, 0);	
    EPWM_setCounterCompareValue(BUCK_PWM_BASE, EPWM_COUNTER_COMPARE_A, 0);	
    EPWM_setCounterCompareShadowLoadMode(BUCK_PWM_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setCounterCompareValue(BUCK_PWM_BASE, EPWM_COUNTER_COMPARE_B, 0);	
    EPWM_setCounterCompareShadowLoadMode(BUCK_PWM_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(BUCK_PWM_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	C28_WORK_init();
	C28_ERROR_init();
	CLA_WORK_init();
	DEBUG_0_init();
	DEBUG_1_init();
	DEBUG_2_init();
	DEBUG_3_init();
	EL_UnReg_STATUS_init();
	EL_OTP_STATUS_init();
	EL_ON_OFF_init();
	DAC8812_nLDAC_init();
	PWM_LOAD_init();
}

void C28_WORK_init(){
	GPIO_writePin(C28_WORK, 0);
	GPIO_setPadConfig(C28_WORK, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(C28_WORK, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(C28_WORK, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(C28_WORK, GPIO_CORE_CPU1);
}
void C28_ERROR_init(){
	GPIO_setPadConfig(C28_ERROR, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(C28_ERROR, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(C28_ERROR, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(C28_ERROR, GPIO_CORE_CPU1);
}
void CLA_WORK_init(){
	GPIO_setPadConfig(CLA_WORK, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(CLA_WORK, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(CLA_WORK, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(CLA_WORK, GPIO_CORE_CPU1_CLA1);
}
void DEBUG_0_init(){
	GPIO_setPadConfig(DEBUG_0, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DEBUG_0, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(DEBUG_0, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(DEBUG_0, GPIO_CORE_CPU1);
}
void DEBUG_1_init(){
	GPIO_setPadConfig(DEBUG_1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DEBUG_1, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(DEBUG_1, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(DEBUG_1, GPIO_CORE_CPU1);
}
void DEBUG_2_init(){
	GPIO_setPadConfig(DEBUG_2, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DEBUG_2, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(DEBUG_2, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(DEBUG_2, GPIO_CORE_CPU1);
}
void DEBUG_3_init(){
	GPIO_setPadConfig(DEBUG_3, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DEBUG_3, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(DEBUG_3, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(DEBUG_3, GPIO_CORE_CPU1);
}
void EL_UnReg_STATUS_init(){
	GPIO_setPadConfig(EL_UnReg_STATUS, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(EL_UnReg_STATUS, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(EL_UnReg_STATUS, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(EL_UnReg_STATUS, GPIO_CORE_CPU1);
}
void EL_OTP_STATUS_init(){
	GPIO_setPadConfig(EL_OTP_STATUS, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(EL_OTP_STATUS, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(EL_OTP_STATUS, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(EL_OTP_STATUS, GPIO_CORE_CPU1);
}
void EL_ON_OFF_init(){
	GPIO_setPadConfig(EL_ON_OFF, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(EL_ON_OFF, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(EL_ON_OFF, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(EL_ON_OFF, GPIO_CORE_CPU1);
}
void DAC8812_nLDAC_init(){
	GPIO_setPadConfig(DAC8812_nLDAC, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(DAC8812_nLDAC, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(DAC8812_nLDAC, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(DAC8812_nLDAC, GPIO_CORE_CPU1);
}
void PWM_LOAD_init(){
	GPIO_writePin(PWM_LOAD, 0);
	GPIO_setPadConfig(PWM_LOAD, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(PWM_LOAD, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(PWM_LOAD, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(PWM_LOAD, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// I2C Configurations
//
//*****************************************************************************
void I2C_init(){
	EEPROM_init();
}

void EEPROM_init(){
	I2C_disableModule(EEPROM_BASE);
	I2C_initController(EEPROM_BASE, DEVICE_SYSCLK_FREQ, 400000, I2C_DUTYCYCLE_33);
	I2C_setConfig(EEPROM_BASE, I2C_CONTROLLER_SEND_MODE);
	I2C_setTargetAddress(EEPROM_BASE, 0);
	I2C_disableLoopback(EEPROM_BASE);
	I2C_setBitCount(EEPROM_BASE, I2C_BITCOUNT_8);
	I2C_setDataCount(EEPROM_BASE, 1);
	I2C_setAddressMode(EEPROM_BASE, I2C_ADDR_MODE_7BITS);
	I2C_enableFIFO(EEPROM_BASE);
	I2C_setEmulationMode(EEPROM_BASE, I2C_EMULATION_STOP_SCL_LOW);
	I2C_enableModule(EEPROM_BASE);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_AD_A_1
	Interrupt_register(INT_AD_A_1, &INT_ADC_EOC_ISR);
	Interrupt_enable(INT_AD_A_1);
}
//*****************************************************************************
//
// SCI Configurations
//
//*****************************************************************************
void SCI_init(){
	DEBUG_SCI_init();
}

void DEBUG_SCI_init(){
	SCI_clearInterruptStatus(DEBUG_SCI_BASE, SCI_INT_RXFF | SCI_INT_TXFF | SCI_INT_FE | SCI_INT_OE | SCI_INT_PE | SCI_INT_RXERR | SCI_INT_RXRDY_BRKDT | SCI_INT_TXRDY);
	SCI_clearOverflowStatus(DEBUG_SCI_BASE);
	SCI_resetTxFIFO(DEBUG_SCI_BASE);
	SCI_resetRxFIFO(DEBUG_SCI_BASE);
	SCI_resetChannels(DEBUG_SCI_BASE);
	SCI_setConfig(DEBUG_SCI_BASE, DEVICE_LSPCLK_FREQ, DEBUG_SCI_BAUDRATE, (SCI_CONFIG_WLEN_8|SCI_CONFIG_STOP_ONE|SCI_CONFIG_PAR_EVEN));
	SCI_disableLoopback(DEBUG_SCI_BASE);
	SCI_performSoftwareReset(DEBUG_SCI_BASE);
	SCI_enableFIFO(DEBUG_SCI_BASE);
	SCI_enableModule(DEBUG_SCI_BASE);
}

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
void SPI_init(){
	DAC8812_init();
}

void DAC8812_init(){
	SPI_disableModule(DAC8812_BASE);
	SPI_setConfig(DAC8812_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL1PHA0,
				  SPI_MODE_CONTROLLER, 10000000, 9);
	SPI_setPTESignalPolarity(DAC8812_BASE, SPI_PTE_ACTIVE_LOW);
	SPI_enableFIFO(DAC8812_BASE);
	SPI_disableLoopback(DAC8812_BASE);
	SPI_setEmulationMode(DAC8812_BASE, SPI_EMULATION_FREE_RUN);
	SPI_enableModule(DAC8812_BASE);
}

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************
void SYNC_init(){
	SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);
	//
	// For EPWM1, the sync input is: SYSCTL_SYNC_IN_SRC_EXTSYNCIN1
	//
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM7, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM10, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP1, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	//
	// SOCA
	//
	SysCtl_enableExtADCSOCSource(0);
	//
	// SOCB
	//
	SysCtl_enableExtADCSOCSource(0);
}
