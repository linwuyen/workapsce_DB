//
// Included Files

#include "common.h"
#include "ModbusSlave.h"
#include "spi_slave.h"


uint32_t u32Test[5] = {0x00000000,0x11112222,0x33334444,0x56565656,0x77888877};
ST_SPI_SLAVE sSpiS = {
                       .u32Fsm = _IS_THRER_A_SPI_PACK,
                       .u32Timeout = 0,
                       .u32TimeStamp = 0,
                       .u32TimeMark = 0,
                       .u32ErrCnts = 0,
                       .p32Data = u32Test,
                       .s16Rsize = 4

};

void task25msec(void * s)
{
    GPIO_togglePin(LED_GPIO31);
}

void task2D5msec(void * s)
{
    u32Test[3] = u32Test[1];
    u32Test[4] = u32Test[2];
}

void task500usec(void * s)
{

}

uint16_t id_ttask = 0;
ST_TIMETASK time_task[] = {
        {task500usec,         0,   T_500US},
        {task2D5msec,         0,   T_2D5MS},
        {task25msec,          0,   T_25MS},
        {0, 0, 0}
};

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();
    //
    // Disable pin locks and enable internal pull-ups.
    //
    Device_initGPIO();
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
    // Configure ePWM1, ePWM2, and TZ GPIOs/Modules
    //
    Board_init();

   initCLA();

    //
    // C2000Ware Library initialization
    //
    C2000Ware_libraries_init();
    //
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
    EINT;
    ERTM;
    //
    // IDLE loop. Just sit and loop forever (optional):
    //
    for(;;)
    {
        // Enter code here
        scanTimeTask(&time_task[id_ttask++], (void *)0);
        if(0 == time_task[id_ttask].fn) id_ttask = 0;

        exeModbusSlave((SCI_MODBUS *)&mbcomm);

        excSpiSlave(&sSpiS);
    }
}
//
// End of File
//
