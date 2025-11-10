
#include "common.h"
#include "ModbusSlave.h"
#include "eeprom.h"

#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH

volatile ST_DRV sDrv;


void task25msec(void * s)
{
//    TGE_DEBUG_TIME_TASK();

}

void task2D5msec(void * s)
{

    TGE_DEBUG_TIME_TASK();
//    SET_DEBUG_TIME_TASK();

    execPowerSequence((HAL_DRV)&sDrv);

//    RST_DEBUG_TIME_TASK();


}

typedef struct _ST_TIMETASK{
    void (*fn) (void * s);
    uint32_t cnt;
    uint32_t max;
} ST_TIMETASK;

uint16_t id_ttask = 0;
ST_TIMETASK time_task[] = {
        {task2D5msec,         0,   T_2D5MS},
        {task25msec,          0,   T_25MS},
        {0, 0, 0}
};


bool_t scanTimeTask(ST_TIMETASK *t, void *s)
{
    static uint32_t delta_t;
    static uint32_t u32Cnt = 0;

    u32Cnt = SW_TIMER - (uint32_t)CPUTimer_getTimerCount(SWTIRMER_BASE);

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


#ifdef _FLASH
#pragma SET_CODE_SECTION()
#endif //_FLASH


void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Board_init();


    initCLA();
    initPower((HAL_DRV)&sDrv);
    initPWM();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Loop Forever
    //
    while(1)
    {
        SET_DEBUG_MAIN_WHILE();
        scanTimeTask(&time_task[id_ttask++], (void *)0);
        if(0 == time_task[id_ttask].fn) id_ttask = 0;

        exeModbusSlave((SCI_MODBUS *)&mbcomm);

        execEEPROM();
        RST_DEBUG_MAIN_WHILE();
    }

}


//
// End of File
//
