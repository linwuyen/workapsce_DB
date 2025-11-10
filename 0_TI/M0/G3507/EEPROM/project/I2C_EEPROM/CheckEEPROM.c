/*
 * Check_EEPROM.c
 *
 *  Created on: 2023�~12��7��
 *      Author: roger_lin
 */
//
#include <stdint.h>
#include "emu_eeprom.h"
#include "CheckEEPROM.h"

//==================EEPROM=================//

ST_I2C_EEPROM TestEEPROM;

void writeEEPROM()
{
    TestEEPROM.fsmEE = _EV_TIMEOUT;
    TestEEPROM.u32TimeOutCnt = 100000;
}

void calTimeOut()
{
    if (TestEEPROM.u32TimeOutCnt == 1)
    {
        TestEEPROM.fsmEE = _EV_PROGRAM_RAM;
    }

    TestEEPROM.u32TimeOutCnt--;
}

void tstWrite()
{

    switch (TestEEPROM.fsmEE)
    {

    case _EV_IDIE_RAM:
        //nothing
        break;

    case _EV_TIMEOUT:

        calTimeOut();
        break;

    case _EV_PROGRAM_RAM:

        setProgramEmuEeprom();
        TestEEPROM.fsmEE = _EV_WAIT_FREE_EEPROM;
        break;

    case _EV_WAIT_FREE_EEPROM:

        if (waittingEmuEeprom())
        {
            TestEEPROM.fsmEE = _EV_EEPROM_STATUS;
        }
        else
        {
            TestEEPROM.fsmEE = _EV_WAIT_FREE_EEPROM;
        }
        break;

    case _EV_EEPROM_STATUS:

        if (getEepromErrorStatus())
        {
            TestEEPROM.fsmEE |= _MASK_EERPROM_ERROR;
        }
        else
        {
            TestEEPROM.fsmEE = _EV_VERIFY_RAM;
        }
        break;

    case _EV_VERIFY_RAM:

        if (setVerifyEmuEeprom())
        {
            TestEEPROM.u32SuccessCnt++;
            TestEEPROM.fsmEE = _EV_IDIE_RAM;
        }
        else
        {
            TestEEPROM.fsmEE |= _MASK_EERPROM_ERROR;
        }
        break;

    case _MASK_EERPROM_ERROR:

    default:
        TestEEPROM.fsmEE = _EV_IDIE_RAM;
        break;
 }
}

//====================I2C========================//

void RE_resetPower()
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_enablePower(GPIOA);
    DL_I2C_reset(EEPROM_BASE);
    DL_I2C_enablePower(EEPROM_BASE);
}

void initGPIO()
{
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_EEPROM_IOMUX_SDA,
    GPIO_I2C_EEPROM_IOMUX_SDA_FUNC,DL_GPIO_INVERSION_DISABLE,
    DL_GPIO_RESISTOR_NONE,DL_GPIO_HYSTERESIS_DISABLE,DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_EEPROM_IOMUX_SCL,
    GPIO_I2C_EEPROM_IOMUX_SCL_FUNC,DL_GPIO_INVERSION_DISABLE,
    DL_GPIO_RESISTOR_NONE,DL_GPIO_HYSTERESIS_DISABLE,DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_enableHiZ(GPIO_I2C_EEPROM_IOMUX_SDA);
    DL_GPIO_enableHiZ(GPIO_I2C_EEPROM_IOMUX_SCL);
}

static const DL_I2C_ClockConfig gI2C_EEPROMClockConfig = {
        .clockSel = DL_I2C_CLOCK_BUSCLK,
        .divideRatio = DL_I2C_CLOCK_DIVIDE_1, };

void initI2C()
{
    DL_I2C_setClockConfig(I2C_EEPROM_INST,(DL_I2C_ClockConfig*) &gI2C_EEPROMClockConfig);
    DL_I2C_setAnalogGlitchFilterPulseWidth(I2C_EEPROM_INST, DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_50NS);
    DL_I2C_enableAnalogGlitchFilter(I2C_EEPROM_INST);
    DL_I2C_setDigitalGlitchFilterPulseWidth(I2C_EEPROM_INST, DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_1);

    /* Configure Target Mode */
    DL_I2C_setTargetOwnAddress(I2C_EEPROM_INST, I2C_EEPROM_TARGET_OWN_ADDR);
    DL_I2C_setTargetTXFIFOThreshold(I2C_EEPROM_INST,DL_I2C_TX_FIFO_LEVEL_BYTES_1);
    DL_I2C_setTargetRXFIFOThreshold(I2C_EEPROM_INST,DL_I2C_RX_FIFO_LEVEL_BYTES_1);
    DL_I2C_enableTargetTXEmptyOnTXRequest(I2C_EEPROM_INST);

    DL_I2C_enableTargetClockStretching(I2C_EEPROM_INST);
    /* Configure Interrupts */
    DL_I2C_enableInterrupt(I2C_EEPROM_INST,
    DL_I2C_INTERRUPT_TARGET_RXFIFO_TRIGGER |
    DL_I2C_INTERRUPT_TARGET_START |
    DL_I2C_INTERRUPT_TARGET_STOP |
    DL_I2C_INTERRUPT_TIMEOUT_A);

    /* Enable module */
    DL_I2C_enableController(I2C_EEPROM_INST);
    DL_I2C_enableTarget(I2C_EEPROM_INST);
}


    void checkI2CBus(void)
    {
        switch (TestEEPROM.fsmI2C)
        {

        case _EV_IDIE_I2C:

            TestEEPROM.REG_SCL = (DL_I2C_getSCLStatus(EEPROM_BASE));

            if (I2C_MBMON_SCL_CLEARED == TestEEPROM.REG_SCL)
            {
                TestEEPROM.u32SCL_Cnt++;
                if (CONTROLLER_SCL_CNT_TIMEOUT == TestEEPROM.u32SCL_Cnt)
                {
                    TestEEPROM.fsmI2C = _EV_RESET_I2C;
                }
                break;
            }
            else
            {
                TestEEPROM.u32SCL_Cnt = 0;
            }

        case _EV_RESET_I2C:

            DL_I2C_disableTargetClockStretching(EEPROM_BASE);
            TestEEPROM.fsmI2C = _EV_IDIE_I2C;
            break;

        case _MASK_I2C_BUS_ERROR:

        default:
            TestEEPROM.fsmI2C = _EV_IDIE_I2C;
            break;
        }
    }
