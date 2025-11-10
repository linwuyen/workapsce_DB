/*
 * timetask.c
 *
 *  Created on: May 17, 2024
 *      Author: User
 */

#include "common.h"

#include "spi_slave.h"
#include "flashapi/f021UserBox.h"

ST_PARAMS sParams;
uint32_t u32Eeprom[256] = {0x00000000,0x11112222,0x33334444,0x56565656,0x77888877};

ST_SPI_SLAVE sSpiS = {
                       .u32Fsm = _IS_THRER_A_SPI_PACK,
                       .u32Timeout = 0,
                       .u32TimeStamp = 0,
                       .u32TimeMark = 0,
                       .u32ErrCnts = 0,
                       .p32Data = (uint32_t*)&sParams,
                       .p32Eeprom = u32Eeprom,
                       .s16Rsize = 4

};
void task500msec(void * s)
{
    updateFANCTRL5Duty();

}
void task10msec(void * s)
{

}

void isThereNewDataInRAM(void)
{
    static uint32_t u32Temp = 0;
    if(sSpiS.u32CntWriteEeprom != u32Temp) {
        rstWriteUserBoxTimeStamp();
        u32Temp = sSpiS.u32CntWriteEeprom;
    }
}

void task1msec(void * s)
{
    static uint16_t u16ToggleDAC = 4095U;

    isThereNewDataInRAM();

    if(4095U == u16ToggleDAC) {
        DAC_setShadowValue(myDACB_BASE, 0U);
        u16ToggleDAC = 0;
    }
    else {
        DAC_setShadowValue(myDACB_BASE, 4095U);
        u16ToggleDAC = 4095U;
    }


    Update_IO();


}

extern void scanSpiAction(void);


void asapTask(void * s)
{
    excSpiSlave(&sSpiS);
    runUserBox();

}

ST_TIMETASK time_task[] = {
        {task1msec,           0,   T_1MS},
        {task10msec,          0,   T_10MS},
        {task500msec,          0,   T_500MS},
        {asapTask,            0,   0},
        END_OF_TASK
};

void pollTimeTask(void)
{
    scanTimeTask(time_task, (void *)0);

}
