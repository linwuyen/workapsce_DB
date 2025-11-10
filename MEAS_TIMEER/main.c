#include "common.h"
#include "shareram.h"
#include "mb_slave/ModbusSlave.h"
#include "cTimeMeas.h"
#include "./cb_slave/cbVIshare.h"

#pragma DATA_SECTION(sAccessCPU1,"RW_CPU1");
ST_SHARERAM sAccessCPU1;
#pragma DATA_SECTION(sReadCPU2,"RD_CPU2");
ST_SHARERAM sReadCPU2;

#define ALLOW_CPU2_ACCESS_GSRAM  (MEMCFG_SECT_GS10|MEMCFG_SECT_GS11|MEMCFG_SECT_GS15)

uint32_t MEP_ScaleFactor;

volatile uint32_t ePWM[] = {0, EPWM1_BASE, EPWM2_BASE, EPWM3_BASE, EPWM4_BASE};

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();


#ifdef _STANDALONE
    //
    // Send boot command to allow the CPU2 application to begin execution
    //
    Device_bootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#endif // _STANDALONE

    //
    // Give memory access to GS13 RAM to CPU2
    //
    MemCfg_setGSRAMMasterSel(ALLOW_CPU2_ACCESS_GSRAM,
                             MEMCFG_GSRAMMASTER_CPU2);


    // The fapi_ram_LoadStart, fapi_ram_LoadSize, and fapi_ram_RunStart symbols
    // are created by the linker. Refer to the device .cmd file.
    //
    memcpy(&fapi_ram_RunStart, &fapi_ram_LoadStart, (size_t)&fapi_ram_LoadSize);

    // FLASH Initialization:
    // The "FLASH_init()" should be called after or during initialization functions like
    // Device_init() or Device_enableAllPeripherals().
    FLASH_init();


    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
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
    // Initialize SysConfig Configuration
    //
    Board_init();
    initCLA();
    initPWM();

    initHwConfigAndDrvParams();


    //
    // Sync CPUs so the blinking starts at the same time, though the LEDs toggle at different frequency
    //
    IPC_sync(IPC_CPU1_L_CPU2_R, IPC_SYNC);
    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Loop Forever
    //
    for(;;)
    {

        // Start timing
        startTimerMeasure(&sDrv.tpMainCost);

        pollTimeTask();

        // Stop timing
        stopTimerMeasure(&sDrv.tpMainCost);
        // Recorded at 202401219 14:00 The program running time is 5.16usec

    }
}

//
// End of File
//

