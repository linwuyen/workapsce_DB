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
// EPWM1 -> BUCK_PWM1 Pinmux
//
//
// EPWM1_A - GPIO Settings
//
#define GPIO_PIN_EPWM1_A 0
#define BUCK_PWM1_EPWMA_GPIO 0
#define BUCK_PWM1_EPWMA_PIN_CONFIG GPIO_0_EPWM1_A
//
// EPWM1_B - GPIO Settings
//
#define GPIO_PIN_EPWM1_B 1
#define BUCK_PWM1_EPWMB_GPIO 1
#define BUCK_PWM1_EPWMB_PIN_CONFIG GPIO_1_EPWM1_B
//
// GPIO34 - GPIO Settings
//
#define LED_GPIO34_GPIO_PIN_CONFIG GPIO_34_GPIO34
//
// GPIO31 - GPIO Settings
//
#define LED_GPIO31_GPIO_PIN_CONFIG GPIO_31_GPIO31

//
// SCIA -> DEBUG_SCI Pinmux
//
//
// SCIA_RX - GPIO Settings
//
#define GPIO_PIN_SCIA_RX 28
#define DEBUG_SCI_SCIRX_GPIO 28
#define DEBUG_SCI_SCIRX_PIN_CONFIG GPIO_28_SCIA_RX
//
// SCIA_TX - GPIO Settings
//
#define GPIO_PIN_SCIA_TX 29
#define DEBUG_SCI_SCITX_GPIO 29
#define DEBUG_SCI_SCITX_PIN_CONFIG GPIO_29_SCIA_TX

//
// SPIA -> SPIA_SLAVE Pinmux
//
//
// SPIA_PICO - GPIO Settings
//
#define GPIO_PIN_SPIA_PICO 16
#define SPIA_SLAVE_SPIPICO_GPIO 16
#define SPIA_SLAVE_SPIPICO_PIN_CONFIG GPIO_16_SPIA_SIMO
//
// SPIA_POCI - GPIO Settings
//
#define GPIO_PIN_SPIA_POCI 17
#define SPIA_SLAVE_SPIPOCI_GPIO 17
#define SPIA_SLAVE_SPIPOCI_PIN_CONFIG GPIO_17_SPIA_SOMI
//
// SPIA_CLK - GPIO Settings
//
#define GPIO_PIN_SPIA_CLK 9
#define SPIA_SLAVE_SPICLK_GPIO 9
#define SPIA_SLAVE_SPICLK_PIN_CONFIG GPIO_9_SPIA_CLK
//
// SPIA_PTE - GPIO Settings
//
#define GPIO_PIN_SPIA_PTE 11
#define SPIA_SLAVE_SPIPTE_GPIO 11
#define SPIA_SLAVE_SPIPTE_PIN_CONFIG GPIO_11_SPIA_STE

//*****************************************************************************
//
// CPUTIMER Configurations
//
//*****************************************************************************
#define SWTIRMER_BASE CPUTIMER0_BASE
void SWTIRMER_init();

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
#define BUCK_PWM1_BASE EPWM1_BASE
#define BUCK_PWM1_TBPRD 0
#define BUCK_PWM1_COUNTER_MODE EPWM_COUNTER_MODE_STOP_FREEZE
#define BUCK_PWM1_TBPHS 0
#define BUCK_PWM1_CMPA 0
#define BUCK_PWM1_CMPB 0
#define BUCK_PWM1_CMPC 0
#define BUCK_PWM1_CMPD 0
#define BUCK_PWM1_DBRED 0
#define BUCK_PWM1_DBFED 0
#define BUCK_PWM1_TZA_ACTION EPWM_TZ_ACTION_HIGH_Z
#define BUCK_PWM1_TZB_ACTION EPWM_TZ_ACTION_HIGH_Z
#define BUCK_PWM1_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define LED_GPIO34 34
void LED_GPIO34_init();
#define LED_GPIO31 31
void LED_GPIO31_init();

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
#define DEBUG_SCI_FIFO_TX_LVL SCI_FIFO_TX0
#define DEBUG_SCI_FIFO_RX_LVL SCI_FIFO_RX0
void DEBUG_SCI_init();

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define SPIA_SLAVE_BASE SPIA_BASE
#define SPIA_SLAVE_BITRATE 1000000
void SPIA_SLAVE_init();

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
void	CPUTIMER_init();
void	EPWM_init();
void	GPIO_init();
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
