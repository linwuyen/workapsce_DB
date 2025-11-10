#include "common.h"
#include "emu_eeprom.h"
#include "tst.h"
#include "i2c_eeprom.h"
#include "ModbusSlave.h"

void task25msec(void * s)
{

}

void taskT_100msec(void * s)
{
   DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN );
}


typedef struct _ST_TIMETASK
{
    void (*fn) (void * s);
    uint32_t cnt;
    uint32_t max;
} ST_TIMETASK;

uint16_t id_ttask = 0;
ST_TIMETASK time_task[] =
{
        {task25msec,         0,   T_25MS},
        {taskT_100msec,      0,  T_100MS},
        {0, 0, 0}
};


bool_t scanTimeTask(ST_TIMETASK *t, void *s)
{
    uint32_t delta_t;
    uint32_t u32Cnt = SWTIMER_CNT;

    if(t->cnt > u32Cnt) {
        delta_t = u32Cnt + SW_TIMER - t->cnt;
    }
    else {
        delta_t = u32Cnt - t->cnt;
    }

    if(delta_t >= t->max) {
        t->fn(s);
        t->cnt = u32Cnt;
        return true;
    }
    else {
        return false;
    }
}

RAMFUNC void ramMainLoop(void)
{
    while (1) {
    }
}

int main(void)
{
    SYSCFG_DL_init();

    while (1)
    {

        scanTimeTask(&time_task[id_ttask++], (void *)0);
        if(0 == time_task[id_ttask].fn) id_ttask = 0;

           exeModbusSlave((SCI_MODBUS *)&mbcomm);

           execEmuEeprom();

           tstEmuEeprom();

           execI2cSlave();

           //檢查RAM與FLASH寫入
           tstWrite();

           //檢查I2CBUS
           checkI2CBus();

    }
}
