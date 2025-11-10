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
    SYSCFG_DL_DEBUG_SCI_init();
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
    DL_UART_Main_reset(DEBUG_SCI_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_TimerA_enablePower(SWTIMER_INST);
    DL_UART_Main_enablePower(DEBUG_SCI_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

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
 * SWTIMER_INST_LOAD_VALUE = (1.0 * 39062.5 Hz) - 1
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


    /* Configure FIFOs */
    DL_UART_Main_enableFIFOs(DEBUG_SCI_INST);
    DL_UART_Main_setRXFIFOThreshold(DEBUG_SCI_INST, DL_UART_RX_FIFO_LEVEL_1_2_FULL);
    DL_UART_Main_setTXFIFOThreshold(DEBUG_SCI_INST, DL_UART_TX_FIFO_LEVEL_1_2_EMPTY);

    DL_UART_Main_enable(DEBUG_SCI_INST);
}

