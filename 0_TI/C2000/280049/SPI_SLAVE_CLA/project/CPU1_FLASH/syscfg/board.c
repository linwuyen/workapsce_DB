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
	CPUTIMER_init();
	EPWM_init();
	GPIO_init();
	SCI_init();
	SPI_init();

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
	// EPWM1 -> BUCK_PWM1 Pinmux
	//
	GPIO_setPinConfig(BUCK_PWM1_EPWMA_PIN_CONFIG);
	GPIO_setPadConfig(BUCK_PWM1_EPWMA_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(BUCK_PWM1_EPWMA_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(BUCK_PWM1_EPWMB_PIN_CONFIG);
	GPIO_setPadConfig(BUCK_PWM1_EPWMB_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(BUCK_PWM1_EPWMB_GPIO, GPIO_QUAL_SYNC);

	// GPIO34 -> LED_GPIO34 Pinmux
	GPIO_setPinConfig(GPIO_34_GPIO34);
	// GPIO31 -> LED_GPIO31 Pinmux
	GPIO_setPinConfig(GPIO_31_GPIO31);
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
	// SPIA -> SPIA_SLAVE Pinmux
	//
	GPIO_setPinConfig(SPIA_SLAVE_SPIPICO_PIN_CONFIG);
	GPIO_setPadConfig(SPIA_SLAVE_SPIPICO_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIA_SLAVE_SPIPICO_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIA_SLAVE_SPIPOCI_PIN_CONFIG);
	GPIO_setPadConfig(SPIA_SLAVE_SPIPOCI_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIA_SLAVE_SPIPOCI_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIA_SLAVE_SPICLK_PIN_CONFIG);
	GPIO_setPadConfig(SPIA_SLAVE_SPICLK_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIA_SLAVE_SPICLK_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIA_SLAVE_SPIPTE_PIN_CONFIG);
	GPIO_setPadConfig(SPIA_SLAVE_SPIPTE_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIA_SLAVE_SPIPTE_GPIO, GPIO_QUAL_ASYNC);


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
	CPUTimer_setEmulationMode(SWTIRMER_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
	CPUTimer_setPreScaler(SWTIRMER_BASE, 2U);
	CPUTimer_setPeriod(SWTIRMER_BASE, 50000000U);
	CPUTimer_disableInterrupt(SWTIRMER_BASE);
	CPUTimer_stopTimer(SWTIRMER_BASE);

	CPUTimer_reloadTimerCounter(SWTIRMER_BASE);
	CPUTimer_startTimer(SWTIRMER_BASE);
}

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
void EPWM_init(){
    EPWM_setClockPrescaler(BUCK_PWM1_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);	
    EPWM_setTimeBasePeriod(BUCK_PWM1_BASE, 0);	
    EPWM_setTimeBaseCounter(BUCK_PWM1_BASE, 0);	
    EPWM_setTimeBaseCounterMode(BUCK_PWM1_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);	
    EPWM_disablePhaseShiftLoad(BUCK_PWM1_BASE);	
    EPWM_setPhaseShift(BUCK_PWM1_BASE, 0);	
    EPWM_setCounterCompareValue(BUCK_PWM1_BASE, EPWM_COUNTER_COMPARE_A, 0);	
    EPWM_setCounterCompareShadowLoadMode(BUCK_PWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setCounterCompareValue(BUCK_PWM1_BASE, EPWM_COUNTER_COMPARE_B, 0);	
    EPWM_setCounterCompareShadowLoadMode(BUCK_PWM1_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(BUCK_PWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setRisingEdgeDelayCountShadowLoadMode(BUCK_PWM1_BASE, EPWM_RED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableRisingEdgeDelayCountShadowLoadMode(BUCK_PWM1_BASE);	
    EPWM_setFallingEdgeDelayCountShadowLoadMode(BUCK_PWM1_BASE, EPWM_FED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableFallingEdgeDelayCountShadowLoadMode(BUCK_PWM1_BASE);	
}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	LED_GPIO34_init();
	LED_GPIO31_init();
}

void LED_GPIO34_init(){
	GPIO_setPadConfig(LED_GPIO34, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(LED_GPIO34, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(LED_GPIO34, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(LED_GPIO34, GPIO_CORE_CPU1);
}
void LED_GPIO31_init(){
	GPIO_setPadConfig(LED_GPIO31, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(LED_GPIO31, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(LED_GPIO31, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(LED_GPIO31, GPIO_CORE_CPU1);
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
	SCI_setFIFOInterruptLevel(DEBUG_SCI_BASE, SCI_FIFO_TX0, SCI_FIFO_RX0);
	SCI_enableFIFO(DEBUG_SCI_BASE);
	SCI_enableModule(DEBUG_SCI_BASE);
}

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
void SPI_init(){
	SPIA_SLAVE_init();
}

void SPIA_SLAVE_init(){
	SPI_disableModule(SPIA_SLAVE_BASE);
	SPI_setConfig(SPIA_SLAVE_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
				  SPI_MODE_PERIPHERAL, 1000000, 16);
	SPI_setPTESignalPolarity(SPIA_SLAVE_BASE, SPI_PTE_ACTIVE_LOW);
	SPI_enableFIFO(SPIA_SLAVE_BASE);
	SPI_disableLoopback(SPIA_SLAVE_BASE);
	SPI_setEmulationMode(SPIA_SLAVE_BASE, SPI_EMULATION_FREE_RUN);
	SPI_enableModule(SPIA_SLAVE_BASE);
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
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP1, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP6, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	//
	// SOCA
	//
	SysCtl_enableExtADCSOCSource(0);
	//
	// SOCB
	//
	SysCtl_enableExtADCSOCSource(0);
}
