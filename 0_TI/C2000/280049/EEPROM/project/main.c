// Included Files
//
#include "Common.h"
#include "emu_eeprom.h"
#include "tst.h"


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
    //
    // C2000Ware Library initialization
    //
    C2000Ware_libraries_init();
    //
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
//    EINT;
//    ERTM;
    // Enter code here
    Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 100);
    Fapi_setActiveFlashBank(Fapi_FlashBank0);
    Fapi_setActiveFlashBank(Fapi_FlashBank1);

    Flash_disableECC(FLASH0ECC_BASE);

    //
    // IDLE loop. Just sit and loop forever (optional):
    //
    for(;;)
    {
        execEmuEeprom();
        tstEmuEeprom();

    }
}
//
// End of File
//
