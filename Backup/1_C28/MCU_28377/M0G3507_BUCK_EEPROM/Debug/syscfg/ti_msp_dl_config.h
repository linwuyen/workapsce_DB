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
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

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




/* Defines for I2C_EEPROM */
#define I2C_EEPROM_INST                                                     I2C0
#define I2C_EEPROM_INST_IRQHandler                               I2C0_IRQHandler
#define I2C_EEPROM_INST_INT_IRQN                                   I2C0_INT_IRQn
#define GPIO_I2C_EEPROM_SDA_PORT                                           GPIOA
#define GPIO_I2C_EEPROM_SDA_PIN                                    DL_GPIO_PIN_0
#define GPIO_I2C_EEPROM_IOMUX_SDA                                 (IOMUX_PINCM1)
#define GPIO_I2C_EEPROM_IOMUX_SDA_FUNC                  IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_EEPROM_SCL_PORT                                           GPIOA
#define GPIO_I2C_EEPROM_SCL_PIN                                    DL_GPIO_PIN_1
#define GPIO_I2C_EEPROM_IOMUX_SCL                                 (IOMUX_PINCM2)
#define GPIO_I2C_EEPROM_IOMUX_SCL_FUNC                  IOMUX_PINCM2_PF_I2C0_SCL


/* Defines for UART_0_DEBUG */
#define UART_0_DEBUG_INST                                                  UART0
#define UART_0_DEBUG_INST_IRQHandler                            UART0_IRQHandler
#define UART_0_DEBUG_INST_INT_IRQN                                UART0_INT_IRQn
#define GPIO_UART_0_DEBUG_RX_PORT                                          GPIOA
#define GPIO_UART_0_DEBUG_TX_PORT                                          GPIOA
#define GPIO_UART_0_DEBUG_RX_PIN                                  DL_GPIO_PIN_11
#define GPIO_UART_0_DEBUG_TX_PIN                                  DL_GPIO_PIN_10
#define GPIO_UART_0_DEBUG_IOMUX_RX                               (IOMUX_PINCM22)
#define GPIO_UART_0_DEBUG_IOMUX_TX                               (IOMUX_PINCM21)
#define GPIO_UART_0_DEBUG_IOMUX_RX_FUNC                IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_DEBUG_IOMUX_TX_FUNC                IOMUX_PINCM21_PF_UART0_TX
#define UART_0_DEBUG_BAUD_RATE                                            (9600)
#define UART_0_DEBUG_IBRD_32_MHZ_9600_BAUD                                 (208)
#define UART_0_DEBUG_FBRD_32_MHZ_9600_BAUD                                  (21)





/* Defines for ADC0_BUCK */
#define ADC0_BUCK_INST                                                      ADC0
#define ADC0_BUCK_INST_IRQHandler                                ADC0_IRQHandler
#define ADC0_BUCK_INST_INT_IRQN                                  (ADC0_INT_IRQn)
#define ADC0_BUCK_ADCMEM_NTC_BUCK_MOS1                        DL_ADC12_MEM_IDX_0
#define ADC0_BUCK_ADCMEM_NTC_BUCK_MOS1_REF         DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC0_BUCK_ADCMEM_NTC_BUCK_MOS1_REF_VOLTAGE                                      -1 // VDDA cannot be determined
#define ADC0_BUCK_ADCMEM_NTC_BUCK_MOS2                        DL_ADC12_MEM_IDX_1
#define ADC0_BUCK_ADCMEM_NTC_BUCK_MOS2_REF         DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC0_BUCK_ADCMEM_NTC_BUCK_MOS2_REF_VOLTAGE                                      -1 // VDDA cannot be determined
#define ADC0_BUCK_ADCMEM_NTC_BUCK_SHUNT1                      DL_ADC12_MEM_IDX_2
#define ADC0_BUCK_ADCMEM_NTC_BUCK_SHUNT1_REF         DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC0_BUCK_ADCMEM_NTC_BUCK_SHUNT1_REF_VOLTAGE                                      -1 // VDDA cannot be determined
#define ADC0_BUCK_ADCMEM_NTC_BUCK_SHUNT2                      DL_ADC12_MEM_IDX_3
#define ADC0_BUCK_ADCMEM_NTC_BUCK_SHUNT2_REF         DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC0_BUCK_ADCMEM_NTC_BUCK_SHUNT2_REF_VOLTAGE                                      -1 // VDDA cannot be determined
#define GPIO_ADC0_BUCK_C2_PORT                                             GPIOA
#define GPIO_ADC0_BUCK_C2_PIN                                     DL_GPIO_PIN_25
#define GPIO_ADC0_BUCK_C3_PORT                                             GPIOA
#define GPIO_ADC0_BUCK_C3_PIN                                     DL_GPIO_PIN_24
#define GPIO_ADC0_BUCK_C7_PORT                                             GPIOA
#define GPIO_ADC0_BUCK_C7_PIN                                     DL_GPIO_PIN_22

/* Defines for ADC1_DAB */
#define ADC1_DAB_INST                                                       ADC1
#define ADC1_DAB_INST_IRQHandler                                 ADC1_IRQHandler
#define ADC1_DAB_INST_INT_IRQN                                   (ADC1_INT_IRQn)
#define ADC1_DAB_ADCMEM_NTC_DAB_MOS1                          DL_ADC12_MEM_IDX_0
#define ADC1_DAB_ADCMEM_NTC_DAB_MOS1_REF         DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC1_DAB_ADCMEM_NTC_DAB_MOS1_REF_VOLTAGE                                      -1 // VDDA cannot be determined
#define ADC1_DAB_ADCMEM_NTC_DAB_MOS2                          DL_ADC12_MEM_IDX_1
#define ADC1_DAB_ADCMEM_NTC_DAB_MOS2_REF         DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC1_DAB_ADCMEM_NTC_DAB_MOS2_REF_VOLTAGE                                      -1 // VDDA cannot be determined
#define ADC1_DAB_ADCMEM_NTC_BUCK_PCB                          DL_ADC12_MEM_IDX_2
#define ADC1_DAB_ADCMEM_NTC_BUCK_PCB_REF         DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC1_DAB_ADCMEM_NTC_BUCK_PCB_REF_VOLTAGE                                      -1 // VDDA cannot be determined
#define GPIO_ADC1_DAB_C0_PORT                                              GPIOA
#define GPIO_ADC1_DAB_C0_PIN                                      DL_GPIO_PIN_15
#define GPIO_ADC1_DAB_C1_PORT                                              GPIOA
#define GPIO_ADC1_DAB_C1_PIN                                      DL_GPIO_PIN_16
#define GPIO_ADC1_DAB_C2_PORT                                              GPIOA
#define GPIO_ADC1_DAB_C2_PIN                                      DL_GPIO_PIN_17



/* Port definition for Pin Group GD_nPG1 */
#define GD_nPG1_PORT                                                     (GPIOA)

/* Defines for PIN_0: GPIOA.26 with pinCMx 59 on package pin 1 */
#define GD_nPG1_PIN_0_PIN                                       (DL_GPIO_PIN_26)
#define GD_nPG1_PIN_0_IOMUX                                      (IOMUX_PINCM59)
/* Port definition for Pin Group GD_nPG2 */
#define GD_nPG2_PORT                                                     (GPIOA)

/* Defines for PIN_1: GPIOA.27 with pinCMx 60 on package pin 2 */
#define GD_nPG2_PIN_1_PIN                                       (DL_GPIO_PIN_27)
#define GD_nPG2_PIN_1_IOMUX                                      (IOMUX_PINCM60)
/* Port definition for Pin Group GD_Enable */
#define GD_Enable_PORT                                                   (GPIOA)

/* Defines for PIN_2: GPIOA.9 with pinCMx 20 on package pin 14 */
#define GD_Enable_PIN_2_PIN                                      (DL_GPIO_PIN_9)
#define GD_Enable_PIN_2_IOMUX                                    (IOMUX_PINCM20)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_I2C_EEPROM_init(void);
void SYSCFG_DL_UART_0_DEBUG_init(void);
void SYSCFG_DL_ADC0_BUCK_init(void);
void SYSCFG_DL_ADC1_DAB_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
