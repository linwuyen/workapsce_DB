/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 */

/*
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

DL_TimerA_backupConfig gSWTIMERBackup;

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_SWTIMER_init();
    SYSCFG_DL_I2C_EEPROM_init();
    SYSCFG_DL_DEBUG_SCI_init();
    SYSCFG_DL_DMA_init();
    /* Ensure backup structures have no valid state */
	gSWTIMERBackup.backupRdy 	= false;


}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(SWTIMER_INST, &gSWTIMERBackup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(SWTIMER_INST, &gSWTIMERBackup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_TimerA_reset(SWTIMER_INST);
    DL_I2C_reset(I2C_EEPROM_INST);
    DL_UART_Main_reset(DEBUG_SCI_INST);


    DL_GPIO_enablePower(GPIOA);
    DL_TimerA_enablePower(SWTIMER_INST);
    DL_I2C_enablePower(I2C_EEPROM_INST);
    DL_UART_Main_enablePower(DEBUG_SCI_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_EEPROM_IOMUX_SDA,
        GPIO_I2C_EEPROM_IOMUX_SDA_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_EEPROM_IOMUX_SCL,
        GPIO_I2C_EEPROM_IOMUX_SCL_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_enableHiZ(GPIO_I2C_EEPROM_IOMUX_SDA);
    DL_GPIO_enableHiZ(GPIO_I2C_EEPROM_IOMUX_SCL);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_DEBUG_SCI_IOMUX_TX, GPIO_DEBUG_SCI_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_DEBUG_SCI_IOMUX_RX, GPIO_DEBUG_SCI_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_TEST_IOMUX);

    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);
    DL_GPIO_enableOutput(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);

}


static const DL_SYSCTL_SYSPLLConfig gSYSPLLConfig = {
    .inputFreq              = DL_SYSCTL_SYSPLL_INPUT_FREQ_16_32_MHZ,
	.rDivClk2x              = 3,
	.rDivClk1               = 0,
	.rDivClk0               = 0,
	.enableCLK2x            = DL_SYSCTL_SYSPLL_CLK2X_ENABLE,
	.enableCLK1             = DL_SYSCTL_SYSPLL_CLK1_DISABLE,
	.enableCLK0             = DL_SYSCTL_SYSPLL_CLK0_DISABLE,
	.sysPLLMCLK             = DL_SYSCTL_SYSPLL_MCLK_CLK2X,
	.sysPLLRef              = DL_SYSCTL_SYSPLL_REF_SYSOSC,
	.qDiv                   = 9,
	.pDiv                   = DL_SYSCTL_SYSPLL_PDIV_2
};
SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
    DL_SYSCTL_setFlashWaitState(DL_SYSCTL_FLASH_WAIT_STATE_2);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
	/* Set default configuration */
	DL_SYSCTL_disableHFXT();
	DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_configSYSPLL((DL_SYSCTL_SYSPLLConfig *) &gSYSPLLConfig);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_2);
    DL_SYSCTL_setMCLKSource(SYSOSC, HSCLK, DL_SYSCTL_HSCLK_SOURCE_SYSPLL);

}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (10000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   39062.5 Hz = 10000000 Hz / (8 * (255 + 1))
 */
static const DL_TimerA_ClockConfig gSWTIMERClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 255U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * SWTIMER_INST_LOAD_VALUE = (1 * 39062.5 Hz) - 1
 */
static const DL_TimerA_TimerConfig gSWTIMERTimerConfig = {
    .period     = SWTIMER_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_SWTIMER_init(void) {

    DL_TimerA_setClockConfig(SWTIMER_INST,
        (DL_TimerA_ClockConfig *) &gSWTIMERClockConfig);

    DL_TimerA_initTimerMode(SWTIMER_INST,
        (DL_TimerA_TimerConfig *) &gSWTIMERTimerConfig);
    DL_TimerA_enableClock(SWTIMER_INST);




}


static const DL_I2C_ClockConfig gI2C_EEPROMClockConfig = {
    .clockSel = DL_I2C_CLOCK_BUSCLK,
    .divideRatio = DL_I2C_CLOCK_DIVIDE_1,
};

SYSCONFIG_WEAK void SYSCFG_DL_I2C_EEPROM_init(void) {

    DL_I2C_setClockConfig(I2C_EEPROM_INST,
        (DL_I2C_ClockConfig *) &gI2C_EEPROMClockConfig);
    DL_I2C_setAnalogGlitchFilterPulseWidth(I2C_EEPROM_INST,
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_5NS);
    DL_I2C_enableAnalogGlitchFilter(I2C_EEPROM_INST);
    DL_I2C_setDigitalGlitchFilterPulseWidth(I2C_EEPROM_INST,
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_1);

    /* Configure Controller Mode */
    DL_I2C_resetControllerTransfer(I2C_EEPROM_INST);
    /* Set frequency to 400000 Hz*/
    DL_I2C_setTimerPeriod(I2C_EEPROM_INST, 9);
    DL_I2C_setControllerTXFIFOThreshold(I2C_EEPROM_INST, DL_I2C_TX_FIFO_LEVEL_BYTES_7);
    DL_I2C_setControllerRXFIFOThreshold(I2C_EEPROM_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_8);
    DL_I2C_enableControllerClockStretching(I2C_EEPROM_INST);

    /* Configure Target Mode */
    DL_I2C_setTargetOwnAddress(I2C_EEPROM_INST, I2C_EEPROM_TARGET_OWN_ADDR);
    DL_I2C_setTargetTXFIFOThreshold(I2C_EEPROM_INST, DL_I2C_TX_FIFO_LEVEL_BYTES_1);
    DL_I2C_setTargetRXFIFOThreshold(I2C_EEPROM_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);

    DL_I2C_enableTargetClockStretching(I2C_EEPROM_INST);
    /* Configure Interrupts */
    DL_I2C_enableInterrupt(I2C_EEPROM_INST,
                           DL_I2C_INTERRUPT_TARGET_ARBITRATION_LOST |
                           DL_I2C_INTERRUPT_TARGET_EVENT1_DMA_DONE |
                           DL_I2C_INTERRUPT_TARGET_EVENT2_DMA_DONE |
                           DL_I2C_INTERRUPT_TARGET_GENERAL_CALL |
                           DL_I2C_TARGET_INTERRUPT_OVERFLOW |
                           DL_I2C_INTERRUPT_TARGET_RXFIFO_FULL |
                           DL_I2C_INTERRUPT_TARGET_RXFIFO_OVERFLOW |
                           DL_I2C_INTERRUPT_TARGET_RXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_TARGET_RX_DONE |
                           DL_I2C_INTERRUPT_TARGET_START |
                           DL_I2C_INTERRUPT_TARGET_STOP |
                           DL_I2C_INTERRUPT_TARGET_TXFIFO_EMPTY |
                           DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_TARGET_TXFIFO_UNDERFLOW |
                           DL_I2C_INTERRUPT_TARGET_TX_DONE);


    /* Enable module */
    DL_I2C_enableController(I2C_EEPROM_INST);
    DL_I2C_enableTarget(I2C_EEPROM_INST);


}


static const DL_UART_Main_ClockConfig gDEBUG_SCIClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Main_Config gDEBUG_SCIConfig = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_EVEN,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_DEBUG_SCI_init(void)
{
    DL_UART_Main_setClockConfig(DEBUG_SCI_INST, (DL_UART_Main_ClockConfig *) &gDEBUG_SCIClockConfig);

    DL_UART_Main_init(DEBUG_SCI_INST, (DL_UART_Main_Config *) &gDEBUG_SCIConfig);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 115200
     *  Actual baud rate: 115190.78
     */
    DL_UART_Main_setOversampling(DEBUG_SCI_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(DEBUG_SCI_INST, DEBUG_SCI_IBRD_40_MHZ_115200_BAUD, DEBUG_SCI_FBRD_40_MHZ_115200_BAUD);


    /* Configure Interrupts */
    DL_UART_Main_enableInterrupt(DEBUG_SCI_INST,
                                 DL_UART_MAIN_INTERRUPT_DMA_DONE_RX);
    /* Setting the Interrupt Priority */
    NVIC_SetPriority(DEBUG_SCI_INST_INT_IRQN, 2);

    /* Configure DMA Receive Event */
    DL_UART_Main_enableDMAReceiveEvent(DEBUG_SCI_INST, DL_UART_DMA_INTERRUPT_RX);
    /* Configure FIFOs */
    DL_UART_Main_enableFIFOs(DEBUG_SCI_INST);
    DL_UART_Main_setRXFIFOThreshold(DEBUG_SCI_INST, DL_UART_RX_FIFO_LEVEL_1_2_FULL);
    DL_UART_Main_setTXFIFOThreshold(DEBUG_SCI_INST, DL_UART_TX_FIFO_LEVEL_1_2_EMPTY);

    /* Configure analog glitch filter */
    DL_UART_Main_enableAnalogGlitchFilter(DEBUG_SCI_INST);
    DL_UART_Main_setAnalogPulseWidth(DEBUG_SCI_INST, DL_UART_PULSE_WIDTH_25_NS);
    /* Configure digital glitch filter */
    DL_UART_setDigitalPulseWidth(DEBUG_SCI_INST, 5);

    DL_UART_Main_enable(DEBUG_SCI_INST);
}

static const DL_DMA_Config gDMA_CH0Config = {
    .transferMode   = DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE,
    .extendedMode   = DL_DMA_NORMAL_MODE,
    .destIncrement  = DL_DMA_ADDR_INCREMENT,
    .srcIncrement   = DL_DMA_ADDR_UNCHANGED,
    .destWidth      = DL_DMA_WIDTH_WORD,
    .srcWidth       = DL_DMA_WIDTH_WORD,
    .trigger        = DEBUG_SCI_INST_DMA_TRIGGER,
    .triggerType    = DL_DMA_TRIGGER_TYPE_EXTERNAL,
};

SYSCONFIG_WEAK void SYSCFG_DL_DMA_CH0_init(void)
{
    DL_DMA_setDestIncrement(DMA, DMA_CH0_CHAN_ID, DL_DMA_ADDR_INCREMENT);
    DL_DMA_initChannel(DMA, DMA_CH0_CHAN_ID , (DL_DMA_Config *) &gDMA_CH0Config);
}
SYSCONFIG_WEAK void SYSCFG_DL_DMA_init(void){
    SYSCFG_DL_DMA_CH0_init();
}


