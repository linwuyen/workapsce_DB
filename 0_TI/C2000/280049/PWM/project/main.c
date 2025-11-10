//
// Included Files
//

#include "ControlPWM.h"

uint16_t AA=1000;
uint16_t BB=1000;

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
   // C2000Ware_libraries_init();
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
    EINT;
    ERTM;
    // IDLE loop. Just sit and loop forever (optional):
    //

    for(;;)
    {
        // Enter code here
        tstPWMFUN();
    }
}
//
// End of File
//
