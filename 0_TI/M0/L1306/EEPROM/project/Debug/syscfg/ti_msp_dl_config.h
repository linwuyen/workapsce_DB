/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0L130X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0L130X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMG2
#define PWM_0_INST_IRQHandler                                   TIMG2_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMG2_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                             32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                          DL_GPIO_PIN_3
#define GPIO_PWM_0_C0_IOMUX                                       (IOMUX_PINCM4)
#define GPIO_PWM_0_C0_IOMUX_FUNC                      IOMUX_PINCM4_PF_TIMG2_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                         DL_GPIO_PIN_22
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM23)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM23_PF_TIMG2_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for CAPTURE_0 */
#define CAPTURE_0_INST                                                   (TIMG1)
#define CAPTURE_0_INST_IRQHandler                               TIMG1_IRQHandler
#define CAPTURE_0_INST_INT_IRQN                                 (TIMG1_INT_IRQn)
#define CAPTURE_0_INST_LOAD_VALUE                                           (0U)
/* GPIO defines for channel 0 */
#define GPIO_CAPTURE_0_C0_PORT                                             GPIOA
#define GPIO_CAPTURE_0_C0_PIN                                      DL_GPIO_PIN_7
#define GPIO_CAPTURE_0_C0_IOMUX                                   (IOMUX_PINCM8)
#define GPIO_CAPTURE_0_C0_IOMUX_FUNC                  IOMUX_PINCM8_PF_TIMG1_CCP0





/* Defines for COMPARE_0 */
#define COMPARE_0_INST                                                   (TIMG0)
#define COMPARE_0_INST_IRQHandler                               TIMG0_IRQHandler
#define COMPARE_0_INST_INT_IRQN                                 (TIMG0_INT_IRQn)
/* GPIO defines for channel 0 */
#define GPIO_COMPARE_0_C0_PORT                                             GPIOA
#define GPIO_COMPARE_0_C0_PIN                                      DL_GPIO_PIN_5
#define GPIO_COMPARE_0_C0_IOMUX                                   (IOMUX_PINCM6)
#define GPIO_COMPARE_0_C0_IOMUX_FUNC                  IOMUX_PINCM6_PF_TIMG0_CCP0




/* Defines for SWTIMER */
#define SWTIMER_INST                                                     (TIMG4)
#define SWTIMER_INST_IRQHandler                                 TIMG4_IRQHandler
#define SWTIMER_INST_INT_IRQN                                   (TIMG4_INT_IRQn)
#define SWTIMER_INST_LOAD_VALUE                                         (62499U)




/* Defines for I2C_EEPROM */
#define I2C_EEPROM_INST                                                     I2C0
#define I2C_EEPROM_INST_IRQHandler                               I2C0_IRQHandler
#define I2C_EEPROM_INST_INT_IRQN                                   I2C0_INT_IRQn
#define I2C_EEPROM_BUS_SPEED_HZ                                           400000
#define I2C_EEPROM_TARGET_OWN_ADDR                                          0x50
#define GPIO_I2C_EEPROM_SDA_PORT                                           GPIOA
#define GPIO_I2C_EEPROM_SDA_PIN                                   DL_GPIO_PIN_10
#define GPIO_I2C_EEPROM_IOMUX_SDA                                (IOMUX_PINCM11)
#define GPIO_I2C_EEPROM_IOMUX_SDA_FUNC                 IOMUX_PINCM11_PF_I2C0_SDA
#define GPIO_I2C_EEPROM_SCL_PORT                                           GPIOA
#define GPIO_I2C_EEPROM_SCL_PIN                                   DL_GPIO_PIN_11
#define GPIO_I2C_EEPROM_IOMUX_SCL                                (IOMUX_PINCM12)
#define GPIO_I2C_EEPROM_IOMUX_SCL_FUNC                 IOMUX_PINCM12_PF_I2C0_SCL


/* Defines for DEBUG_SCI */
#define DEBUG_SCI_INST                                                     UART0
#define DEBUG_SCI_INST_IRQHandler                               UART0_IRQHandler
#define DEBUG_SCI_INST_INT_IRQN                                   UART0_INT_IRQn
#define GPIO_DEBUG_SCI_RX_PORT                                             GPIOA
#define GPIO_DEBUG_SCI_TX_PORT                                             GPIOA
#define GPIO_DEBUG_SCI_RX_PIN                                     DL_GPIO_PIN_26
#define GPIO_DEBUG_SCI_TX_PIN                                     DL_GPIO_PIN_25
#define GPIO_DEBUG_SCI_IOMUX_RX                                  (IOMUX_PINCM27)
#define GPIO_DEBUG_SCI_IOMUX_TX                                  (IOMUX_PINCM26)
#define GPIO_DEBUG_SCI_IOMUX_RX_FUNC                   IOMUX_PINCM27_PF_UART0_RX
#define GPIO_DEBUG_SCI_IOMUX_TX_FUNC                   IOMUX_PINCM26_PF_UART0_TX
#define DEBUG_SCI_BAUD_RATE                                             (115200)
#define DEBUG_SCI_IBRD_32_MHZ_115200_BAUD                                   (17)
#define DEBUG_SCI_FBRD_32_MHZ_115200_BAUD                                   (23)




/* Defines for SPI_0 */
#define SPI_0_INST                                                         SPI0
#define SPI_0_INST_IRQHandler                                   SPI0_IRQHandler
#define SPI_0_INST_INT_IRQN                                       SPI0_INT_IRQn
#define GPIO_SPI_0_PICO_PORT                                              GPIOA
#define GPIO_SPI_0_PICO_PIN                                       DL_GPIO_PIN_9
#define GPIO_SPI_0_IOMUX_PICO                                   (IOMUX_PINCM10)
#define GPIO_SPI_0_IOMUX_PICO_FUNC                   IOMUX_PINCM10_PF_SPI0_PICO
#define GPIO_SPI_0_POCI_PORT                                              GPIOA
#define GPIO_SPI_0_POCI_PIN                                       DL_GPIO_PIN_4
#define GPIO_SPI_0_IOMUX_POCI                                    (IOMUX_PINCM5)
#define GPIO_SPI_0_IOMUX_POCI_FUNC                    IOMUX_PINCM5_PF_SPI0_POCI
/* GPIO configuration for SPI_0 */
#define GPIO_SPI_0_SCLK_PORT                                              GPIOA
#define GPIO_SPI_0_SCLK_PIN                                       DL_GPIO_PIN_6
#define GPIO_SPI_0_IOMUX_SCLK                                    (IOMUX_PINCM7)
#define GPIO_SPI_0_IOMUX_SCLK_FUNC                    IOMUX_PINCM7_PF_SPI0_SCLK
#define GPIO_SPI_0_CS0_PORT                                               GPIOA
#define GPIO_SPI_0_CS0_PIN                                        DL_GPIO_PIN_8
#define GPIO_SPI_0_IOMUX_CS0                                     (IOMUX_PINCM9)
#define GPIO_SPI_0_IOMUX_CS0_FUNC                      IOMUX_PINCM9_PF_SPI0_CS0



/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_0_REF_VOLTAGE                                          -1 // VDDA cannot be determined
#define GPIO_ADC12_0_C0_PORT                                               GPIOA
#define GPIO_ADC12_0_C0_PIN                                       DL_GPIO_PIN_27


/* Defines for VREF */
#define VREF_VOLTAGE_MV                                                     1400
#define GPIO_VREF_VREFPOS_PORT                                             GPIOA
#define GPIO_VREF_VREFPOS_PIN                                     DL_GPIO_PIN_23
#define GPIO_VREF_IOMUX_VREFPOS                                  (IOMUX_PINCM24)
#define GPIO_VREF_IOMUX_VREFPOS_FUNC                IOMUX_PINCM24_PF_UNCONNECTED
#define GPIO_VREF_VREFNEG_PORT                                             GPIOA
#define GPIO_VREF_VREFNEG_PIN                                     DL_GPIO_PIN_21
#define GPIO_VREF_IOMUX_VREFNEG                                  (IOMUX_PINCM22)
#define GPIO_VREF_IOMUX_VREFNEG_FUNC                IOMUX_PINCM22_PF_UNCONNECTED



/* Defines for OPA_0 */
#define OPA_0_INST                                                          OPA1
#define GPIO_OPA_0_IN0POS_PORT                                             GPIOA
#define GPIO_OPA_0_IN0POS_PIN                                     DL_GPIO_PIN_18
#define GPIO_OPA_0_IOMUX_IN0POS                                  (IOMUX_PINCM19)
#define GPIO_OPA_0_IOMUX_IN0POS_FUNC                IOMUX_PINCM19_PF_UNCONNECTED
#define GPIO_OPA_0_IN1NEG_PORT                                             GPIOA
#define GPIO_OPA_0_IN1NEG_PIN                                     DL_GPIO_PIN_17
#define GPIO_OPA_0_IOMUX_IN1NEG                                  (IOMUX_PINCM18)
#define GPIO_OPA_0_IOMUX_IN1NEG_FUNC                IOMUX_PINCM18_PF_UNCONNECTED
#define GPIO_OPA_0_OUT_PORT                                                GPIOA
#define GPIO_OPA_0_OUT_PIN                                        DL_GPIO_PIN_16
#define GPIO_OPA_0_IOMUX_OUT                                     (IOMUX_PINCM17)
#define GPIO_OPA_0_IOMUX_OUT_FUNC                   IOMUX_PINCM17_PF_UNCONNECTED


/* Defines for GPAMP_0 */



/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (0)
#define DEBUG_SCI_INST_DMA_TRIGGER                           (DMA_UART0_RX_TRIG)



/* Port definition for Pin Group GPIO_LEDS */
#define GPIO_LEDS_PORT                                                   (GPIOA)

/* Defines for USER_LED_1: GPIOA.13 with pinCMx 14 on package pin 17 */
#define GPIO_LEDS_USER_LED_1_PIN                                (DL_GPIO_PIN_13)
#define GPIO_LEDS_USER_LED_1_IOMUX                               (IOMUX_PINCM14)
/* Port definition for Pin Group GPIO_LEDS2 */
#define GPIO_LEDS2_PORT                                                  (GPIOA)

/* Defines for USER_LED_2: GPIOA.0 with pinCMx 1 on package pin 1 */
#define GPIO_LEDS2_USER_LED_2_PIN                                (DL_GPIO_PIN_0)
#define GPIO_LEDS2_USER_LED_2_IOMUX                               (IOMUX_PINCM1)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_CAPTURE_0_init(void);
void SYSCFG_DL_COMPARE_0_init(void);
void SYSCFG_DL_SWTIMER_init(void);
void SYSCFG_DL_I2C_EEPROM_init(void);
void SYSCFG_DL_DEBUG_SCI_init(void);
void SYSCFG_DL_SPI_0_init(void);
void SYSCFG_DL_ADC12_0_init(void);
void SYSCFG_DL_VREF_init(void);
void SYSCFG_DL_OPA_0_init(void);
void SYSCFG_DL_GPAMP_0_init(void);
void SYSCFG_DL_DMA_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
