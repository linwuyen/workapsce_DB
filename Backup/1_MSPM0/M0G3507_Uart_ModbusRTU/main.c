#include "common.h"
#include "timetask.h"

#include "mb_slave/ModbusSlave.h"



void task25msec(void * s)
{
    DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);
}

void task2D5msec(void * s)
{

}


uint16_t id_ttask = 0;
ST_TIMETASK time_task[] = {
        {task2D5msec,         0,   T_2D5MS},
        {task25msec,          0,   T_25MS},
        {0, 0, 0}
};


int main(void)
{
    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    /* Default: LED1 and LED3 ON, LED2 OFF */
    //DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);
    //DL_GPIO_setPins(GPIO_LEDS_PORT,GPIO_LEDS_USER_TEST_PIN);

    while (1) {
        scanTimeTask(&time_task[id_ttask++], (void *)0);
        if(0 == time_task[id_ttask].fn) id_ttask = 0;


        exeModbusSlave((SCI_MODBUS *)&mbcomm);
    }
}
