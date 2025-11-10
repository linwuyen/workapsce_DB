#include "common.h"

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

    initPWM();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Loop Forever
    //
    while(1) {

    }

}

//
// End of File
//

