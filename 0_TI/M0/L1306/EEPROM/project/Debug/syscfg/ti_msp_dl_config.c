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
 *  DO NOT EDIT - This file is generated for the MSPM0L130X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

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
    SYSCFG_DL_PWM_0_init();
    SYSCFG_DL_CAPTURE_0_init();
    SYSCFG_DL_COMPARE_0_init();
    SYSCFG_DL_SWTIMER_init();
    SYSCFG_DL_I2C_EEPROM_init();
    SYSCFG_DL_DEBUG_SCI_init();
    SYSCFG_DL_SPI_0_init();
    SYSCFG_DL_ADC12_0_init();
    SYSCFG_DL_VREF_init();
    SYSCFG_DL_OPA_0_init();
    SYSCFG_DL_GPAMP_0_init();
    SYSCFG_DL_DMA_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_TimerG_reset(PWM_0_INST);
    DL_TimerG_reset(CAPTURE_0_INST);
    DL_TimerG_reset(COMPARE_0_INST);
    DL_TimerG_reset(SWTIMER_INST);
    DL_I2C_reset(I2C_EEPROM_INST);
    DL_UART_Main_reset(DEBUG_SCI_INST);
    DL_SPI_reset(SPI_0_INST);
    DL_ADC12_reset(ADC12_0_INST);
    DL_VREF_reset(VREF);
    DL_OPA_reset(OPA_0_INST);


    DL_GPIO_enablePower(GPIOA);
    DL_TimerG_enablePower(PWM_0_INST);
    DL_TimerG_enablePower(CAPTURE_0_INST);
    DL_TimerG_enablePower(COMPARE_0_INST);
    DL_TimerG_enablePower(SWTIMER_INST);
    DL_I2C_enablePower(I2C_EEPROM_INST);
    DL_UART_Main_enablePower(DEBUG_SCI_INST);
    DL_SPI_enablePower(SPI_0_INST);
    DL_ADC12_enablePower(ADC12_0_INST);
    DL_VREF_enablePower(VREF);
    DL_OPA_enablePower(OPA_0_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C0_IOMUX,GPIO_PWM_0_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C0_PORT, GPIO_PWM_0_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C1_IOMUX,GPIO_PWM_0_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C1_PORT, GPIO_PWM_0_C1_PIN);

    DL_GPIO_initPeripheralInputFunction(GPIO_CAPTURE_0_C0_IOMUX,GPIO_CAPTURE_0_C0_IOMUX_FUNC);

    DL_GPIO_initPeripheralInputFunction(GPIO_COMPARE_0_C0_IOMUX,GPIO_COMPARE_0_C0_IOMUX_FUNC);

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

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_SPI_0_IOMUX_SCLK, GPIO_SPI_0_IOMUX_SCLK_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_SPI_0_IOMUX_PICO, GPIO_SPI_0_IOMUX_PICO_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_SPI_0_IOMUX_POCI, GPIO_SPI_0_IOMUX_POCI_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_SPI_0_IOMUX_CS0, GPIO_SPI_0_IOMUX_CS0_FUNC);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS2_USER_LED_2_IOMUX);

    DL_GPIO_setPins(GPIOA, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS2_USER_LED_2_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS2_USER_LED_2_PIN);

}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}


/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_0ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerG_PWMConfig gPWM_0Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_0_init(void) {

    DL_TimerG_setClockConfig(
        PWM_0_INST, (DL_TimerG_ClockConfig *) &gPWM_0ClockConfig);

    DL_TimerG_initPWMMode(
        PWM_0_INST, (DL_TimerG_PWMConfig *) &gPWM_0Config);

    DL_TimerG_setCaptureCompareValue(PWM_0_INST, 1000, DL_TIMER_CC_0_INDEX);
    DL_TimerG_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);

    DL_TimerG_setCaptureCompareValue(PWM_0_INST, 1000, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

    DL_TimerG_enableClock(PWM_0_INST);


    
    DL_TimerG_setCCPDirection(PWM_0_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT );

}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gCAPTURE_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * CAPTURE_0_INST_LOAD_VALUE = (0 ms * 32000000 Hz) - 1
 */
static const DL_TimerG_CaptureConfig gCAPTURE_0CaptureConfig = {
    .captureMode    = DL_TIMER_CAPTURE_MODE_EDGE_TIME,
    .period         = CAPTURE_0_INST_LOAD_VALUE,
    .startTimer     = DL_TIMER_STOP,
    .edgeCaptMode   = DL_TIMER_CAPTURE_EDGE_DETECTION_MODE_RISING,
    .inputChan      = DL_TIMER_INPUT_CHAN_0,
    .inputInvMode   = DL_TIMER_CC_INPUT_INV_NOINVERT,
};

SYSCONFIG_WEAK void SYSCFG_DL_CAPTURE_0_init(void) {

    DL_TimerG_setClockConfig(CAPTURE_0_INST,
        (DL_TimerG_ClockConfig *) &gCAPTURE_0ClockConfig);

    DL_TimerG_initCaptureMode(CAPTURE_0_INST,
        (DL_TimerG_CaptureConfig *) &gCAPTURE_0CaptureConfig);
    DL_TimerG_enableClock(CAPTURE_0_INST);

}


/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gCOMPARE_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerG_CompareConfig gCOMPARE_0CompareConfig = {
    .compareMode    = DL_TIMER_COMPARE_MODE_EDGE_COUNT,
    .count          = 0,
    .startTimer     = DL_TIMER_STOP,
    .edgeDetectMode = DL_TIMER_COMPARE_EDGE_DETECTION_MODE_RISING,
    .inputChan      = DL_TIMER_INPUT_CHAN_0,
    .inputInvMode   = DL_TIMER_CC_INPUT_INV_NOINVERT,
};

SYSCONFIG_WEAK void SYSCFG_DL_COMPARE_0_init(void) {

    DL_TimerG_setClockConfig(COMPARE_0_INST,
        (DL_TimerG_ClockConfig *) &gCOMPARE_0ClockConfig);
    DL_TimerG_initCompareMode(COMPARE_0_INST,
        (DL_TimerG_CompareConfig *) &gCOMPARE_0CompareConfig);

    DL_TimerG_enableClock(COMPARE_0_INST);




}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (8000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   62500 Hz = 8000000 Hz / (4 * (127 + 1))
 */
static const DL_TimerG_ClockConfig gSWTIMERClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_4,
    .prescale    = 127U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * SWTIMER_INST_LOAD_VALUE = (1 * 62500 Hz) - 1
 */
static const DL_TimerG_TimerConfig gSWTIMERTimerConfig = {
    .period     = SWTIMER_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_START,
};

SYSCONFIG_WEAK void SYSCFG_DL_SWTIMER_init(void) {

    DL_TimerG_setClockConfig(SWTIMER_INST,
        (DL_TimerG_ClockConfig *) &gSWTIMERClockConfig);

    DL_TimerG_initTimerMode(SWTIMER_INST,
        (DL_TimerG_TimerConfig *) &gSWTIMERTimerConfig);
    DL_TimerG_enableClock(SWTIMER_INST);




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
    DL_I2C_setTimerPeriod(I2C_EEPROM_INST, 7);
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
                           DL_I2C_INTERRUPT_TARGET_TX_DONE |
                           DL_I2C_INTERRUPT_TIMEOUT_A);


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
     *  Actual baud rate: 115211.52
     */
    DL_UART_Main_setOversampling(DEBUG_SCI_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(DEBUG_SCI_INST, DEBUG_SCI_IBRD_32_MHZ_115200_BAUD, DEBUG_SCI_FBRD_32_MHZ_115200_BAUD);


    /* Configure Interrupts */
    DL_UART_Main_enableInterrupt(DEBUG_SCI_INST,
                                 DL_UART_MAIN_INTERRUPT_DMA_DONE_RX);
    /* Setting the Interrupt Priority */
    NVIC_SetPriority(DEBUG_SCI_INST_INT_IRQN, 2);

    /* Configure DMA Receive Event */
    DL_UART_Main_enableDMAReceiveEvent(DEBUG_SCI_INST, DL_UART_DMA_INTERRUPT_RX);
    /* Configure FIFOs */
    DL_UART_Main_enableFIFOs(DEBUG_SCI_INST);
    DL_UART_Main_setRXFIFOThreshold(DEBUG_SCI_INST, DL_UART_RX_FIFO_LEVEL_ONE_ENTRY);
    DL_UART_Main_setTXFIFOThreshold(DEBUG_SCI_INST, DL_UART_TX_FIFO_LEVEL_1_2_EMPTY);

    /* Configure analog glitch filter */
    DL_UART_Main_enableAnalogGlitchFilter(DEBUG_SCI_INST);
    DL_UART_Main_setAnalogPulseWidth(DEBUG_SCI_INST, DL_UART_PULSE_WIDTH_25_NS);
    /* Configure digital glitch filter */
    DL_UART_setDigitalPulseWidth(DEBUG_SCI_INST, 5);

    DL_UART_Main_enable(DEBUG_SCI_INST);
}

static const DL_SPI_Config gSPI_0_config = {
    .mode        = DL_SPI_MODE_CONTROLLER,
    .frameFormat = DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0,
    .parity      = DL_SPI_PARITY_NONE,
    .dataSize    = DL_SPI_DATA_SIZE_8,
    .bitOrder    = DL_SPI_BIT_ORDER_MSB_FIRST,
    .chipSelectPin = DL_SPI_CHIP_SELECT_0,
};

static const DL_SPI_ClockConfig gSPI_0_clockConfig = {
    .clockSel    = DL_SPI_CLOCK_BUSCLK,
    .divideRatio = DL_SPI_CLOCK_DIVIDE_RATIO_1
};

SYSCONFIG_WEAK void SYSCFG_DL_SPI_0_init(void) {
    DL_SPI_setClockConfig(SPI_0_INST, (DL_SPI_ClockConfig *) &gSPI_0_clockConfig);

    DL_SPI_init(SPI_0_INST, (DL_SPI_Config *) &gSPI_0_config);

    /* Configure Controller mode */
    /*
     * Set the bit rate clock divider to generate the serial output clock
     *     outputBitRate = (spiInputClock) / ((1 + SCR) * 2)
     *     8000000 = (32000000)/((1 + 1) * 2)
     */
    DL_SPI_setBitRateSerialClockDivider(SPI_0_INST, 1);
    /* Set RX and TX FIFO threshold levels */
    DL_SPI_setFIFOThreshold(SPI_0_INST, DL_SPI_RX_FIFO_LEVEL_1_2_FULL, DL_SPI_TX_FIFO_LEVEL_1_2_EMPTY);

    /* Enable module */
    DL_SPI_enable(SPI_0_INST);
}

/* ADC12_0 Initialization */
static const DL_ADC12_ClockConfig gADC12_0ClockConfig = {
    .clockSel       = DL_ADC12_CLOCK_SYSOSC,
    .divideRatio    = DL_ADC12_CLOCK_DIVIDE_1,
    .freqRange      = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};
SYSCONFIG_WEAK void SYSCFG_DL_ADC12_0_init(void)
{
    DL_ADC12_setClockConfig(ADC12_0_INST, (DL_ADC12_ClockConfig *) &gADC12_0ClockConfig);
    DL_ADC12_configConversionMem(ADC12_0_INST, ADC12_0_ADCMEM_0,
        DL_ADC12_INPUT_CHAN_0, DL_ADC12_REFERENCE_VOLTAGE_VDDA, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    DL_ADC12_enableConversions(ADC12_0_INST);
}


static const DL_VREF_Config gVREFConfig = {
    .vrefEnable     = DL_VREF_ENABLE_ENABLE,
    .bufConfig      = DL_VREF_BUFCONFIG_OUTPUT_1_4V,
    .shModeEnable   = DL_VREF_SHMODE_DISABLE,
    .holdCycleCount = DL_VREF_HOLD_MIN,
    .shCycleCount   = DL_VREF_SH_MIN,
};

SYSCONFIG_WEAK void SYSCFG_DL_VREF_init(void) {
    DL_VREF_configReference(VREF,
        (DL_VREF_Config *) &gVREFConfig);
}


static const DL_OPA_Config gOPA_0Config0 = {
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN1_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
};

SYSCONFIG_WEAK void SYSCFG_DL_OPA_0_init(void)
{
    DL_OPA_init(OPA_0_INST, (DL_OPA_Config *) &gOPA_0Config0);

    DL_OPA_enable(OPA_0_INST);
}

static const DL_GPAMP_Config gGPAMP_0Config = {
    .pselChannel    = DL_GPAMP_PSEL_OPEN,
    .nselChannel    = DL_GPAMP_NSEL_OPEN,
    .outputPinState = DL_GPAMP_OUTPUT_PIN_STATE_DISABLED,
    .choppingMode   = DL_GPAMP_CHOPPING_MODE_DISABLED,
    .choppingFreq   = DL_GPAMP_CHOPPING_FREQ_16KHZ,
};

SYSCONFIG_WEAK void SYSCFG_DL_GPAMP_0_init(void)
{
    DL_GPAMP_init((DL_GPAMP_Config *) &gGPAMP_0Config);
    DL_GPAMP_enable();
}

static const DL_DMA_Config gDMA_CH0Config = {
    .transferMode   = DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE,
    .extendedMode   = DL_DMA_NORMAL_MODE,
    .destIncrement  = DL_DMA_ADDR_INCREMENT,
    .srcIncrement   = DL_DMA_ADDR_UNCHANGED,
    .destWidth      = DL_DMA_WIDTH_BYTE,
    .srcWidth       = DL_DMA_WIDTH_BYTE,
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


