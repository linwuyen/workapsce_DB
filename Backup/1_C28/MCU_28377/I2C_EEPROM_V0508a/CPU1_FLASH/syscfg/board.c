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
	CPUTIMER_init();
	GPIO_init();
	I2C_init();
	SCI_init();

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
	
	// GPIO2 -> DEBUG_C28 Pinmux
	GPIO_setPinConfig(GPIO_2_GPIO2);
	//
	// I2CA -> EEPROM Pinmux
	//
	GPIO_setPinConfig(EEPROM_I2CSDA_PIN_CONFIG);
	GPIO_setPadConfig(EEPROM_I2CSDA_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(EEPROM_I2CSDA_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(EEPROM_I2CSCL_PIN_CONFIG);
	GPIO_setPadConfig(EEPROM_I2CSCL_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(EEPROM_I2CSCL_GPIO, GPIO_QUAL_SYNC);

	//
	// SCIA -> DEBUG_SCI Pinmux
	//
	GPIO_setPinConfig(DEBUG_SCI_SCIRX_PIN_CONFIG);
	GPIO_setPadConfig(DEBUG_SCI_SCIRX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(DEBUG_SCI_SCIRX_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(DEBUG_SCI_SCITX_PIN_CONFIG);
	GPIO_setPadConfig(DEBUG_SCI_SCITX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(DEBUG_SCI_SCITX_GPIO, GPIO_QUAL_ASYNC);


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
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	DEBUG_C28_init();
}

void DEBUG_C28_init(){
	GPIO_writePin(DEBUG_C28, 0);
	GPIO_setPadConfig(DEBUG_C28, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(DEBUG_C28, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(DEBUG_C28, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(DEBUG_C28, GPIO_CORE_CPU1);
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
	I2C_setTargetAddress(EEPROM_BASE, 80);
	I2C_disableLoopback(EEPROM_BASE);
	I2C_setBitCount(EEPROM_BASE, I2C_BITCOUNT_8);
	I2C_setDataCount(EEPROM_BASE, 1);
	I2C_setAddressMode(EEPROM_BASE, I2C_ADDR_MODE_7BITS);
	I2C_enableFIFO(EEPROM_BASE);
	I2C_setEmulationMode(EEPROM_BASE, I2C_EMULATION_FREE_RUN);
	I2C_enableModule(EEPROM_BASE);
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

