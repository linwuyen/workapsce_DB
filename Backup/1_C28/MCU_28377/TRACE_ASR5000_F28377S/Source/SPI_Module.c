//###########################################################################
//
// FILE:    SPI_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210825
//###########################################################################

#include "Global_VariableDefs.h"

SPI_MODULE_VARIABLE_REG SPI_Module_Variables;



////////////////////////////////////////////////////////////////////
//
// ADS8353ªì©l¤Æ¡¨32-CLK, single-SDO mode¡¨
//
void Init_ADS8353(void)
{
    /*----------------------------------------*/
    //
    // Set ADCTRL5
    //
    SpiaRegs.SPITXBUF = 0x0000;
    SpiaRegs.SPITXBUF = 0x0000;
    SpiaRegs.SPITXBUF = 0x0010;

    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;

    /*----------------------------------------*/
    DELAY_US(10);

    /*----------------------------------------*/
    //
    // Set ADS8353 to ¡¨32-CLK, single-SDO mode¡¨
    //
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 1;

    SpiaRegs.SPITXBUF = 0x8400;

    SpiaRegs.SPITXBUF = 0x0000;
    SpiaRegs.SPITXBUF = 0x0000;

    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;

    /*----------------------------------------*/
    DELAY_US(10);

    /*----------------------------------------*/
    //
    // Clear ADCTRL5
    //
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;

    SpiaRegs.SPITXBUF = 0x0000;
    SpiaRegs.SPITXBUF = 0x0000;
    SpiaRegs.SPITXBUF = 0x0000;

    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;

    /*----------------------------------------*/
    DELAY_US(10);
}




////////////////////////////////////////////////////////////////////
void Init_SPI_A_Reg(void)
{

    //
    // Initialize SPI FIFO registers
    SpiaRegs.SPIFFTX.all = 0x0000;
    SpiaRegs.SPIFFTX.bit.SPIRST = 1;          //bit15,SPI Reset
    SpiaRegs.SPIFFTX.bit.SPIFFENA = 1;      //bit14,SPI FIFO Enhancements Enable
    SpiaRegs.SPIFFTX.bit.TXFIFO = 0;            // Reset TX FIFO
    SpiaRegs.SPIFFTX.bit.TXFIFO = 1;            // Reset TX FIFO
    SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;
    SpiaRegs.SPIFFTX.bit.TXFFIENA = 0;        //bit5,TX FIFO Interrupt Enable
    SpiaRegs.SPIFFTX.bit.TXFFIL = 0;        //bit4-0,

    SpiaRegs.SPIFFRX.all = 0x0000;            //Disable All Receive Register
    SpiaRegs.SPIFFRX.bit.RXFFOVFCLR = 1;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;
    SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1;
    SpiaRegs.SPIFFRX.bit.RXFFIENA = 0;
    SpiaRegs.SPIFFRX.bit.RXFFIL = 0;


    //
    // Initialize SPI-A
    //
    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Enable loop-back
    //
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
    SpiaRegs.SPICCR.bit.SPICHAR = (16-1);
    //20210708
    //SpiaRegs.SPICCR.bit.SPILBK = 1;

    //
    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    //
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
    SpiaRegs.SPICTL.bit.SPIINTENA = 0;

    //
    // Set the baud rate
    //
    //
    // For SPIBRR = 3 to 127: SPI Baud Rate = LSPCLK / (SPIBRR + 1)
    // For SPIBRR = 0, 1, or 2: SPI Baud Rate = LSPCLK / 4
    // LSPCLK = 200MHz
    //
    //SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = 9;

    //
    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    //
    SpiaRegs.SPIPRI.bit.FREE = 1;

    //
    // Release the SPI from reset
    //
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;


    SpiaRegs.SPITXBUF = 0x0000;

    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;


}



////////////////////////////////////////////////////////////////////
//
// InitSpiaGpio - Initialize SPIA GPIOs
//
void Init_SPI_A_GPIO(void)
{

    GPIO_SetupPinOptions(19, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinOptions(16, GPIO_OUTPUT, GPIO_PULLUP);
    GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_PULLUP);

    Init_VREF_4094_Sequence();

    //SIMO A
    GPIO_SetupPinMux(16, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(16, GPIO_OUTPUT, GPIO_PULLUP);

    //SOMI A
    GPIO_SetupPinMux(17, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(17, GPIO_INPUT, GPIO_PULLUP);

    //CLK A
    GPIO_SetupPinMux(18, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(18, GPIO_OUTPUT, GPIO_PULLUP);

    //CS A
    GPIO_SetupPinMux(19, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(19, GPIO_OUTPUT, GPIO_PULLUP);

}





////////////////////////////////////////////////////////////////////
//
// InitSpibGpio - Initialize SPIB GPIOs for Master
//
void Init_SPI_B_GPIO_for_Master(void)
{

    //SIMO B
    GPIO_SetupPinMux(60, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(60, GPIO_OUTPUT, GPIO_PULLUP);
    //GPIO_SetupPinOptions(60, GPIO_INPUT, GPIO_PULLUP);


    //CLK B
    GPIO_SetupPinMux(58, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(58, GPIO_OUTPUT, GPIO_PULLUP);

    //CS B
    GPIO_SetupPinMux(59, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(59, GPIO_OUTPUT, GPIO_PULLUP);

}



////////////////////////////////////////////////////////////////////
//
// InitSpibGpio - Initialize SPIB GPIOs for Slave
//
void Init_SPI_B_GPIO_for_Slave(void)
{

    //SIMO B
    GPIO_SetupPinMux(60, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(60, GPIO_INPUT, GPIO_PULLUP | GPIO_ASYNC);


    //CLK B
    GPIO_SetupPinMux(58, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(58, GPIO_INPUT, GPIO_PULLUP | GPIO_ASYNC);

    //CS B
    GPIO_SetupPinMux(59, GPIO_MUX_CPU1, 6);
    GPIO_SetupPinOptions(59, GPIO_INPUT, GPIO_PULLUP | GPIO_ASYNC);

}






////////////////////////////////////////////////////////////////////
//
// Init SPIB for Master
//
void Init_SPI_B_Reg_for_Master(void)
{
    SpibRegs.SPICCR.bit.SPISWRESET = 0;     // SPI SW Reset

    SpibRegs.SPIFFTX.bit.SPIRST = 0;        // SPI Reset
    SpibRegs.SPIFFTX.bit.SPIRST = 1;
    SpibRegs.SPIFFTX.bit.SPIFFENA = 1;      // Enable FIFO
    SpibRegs.SPIFFTX.bit.TXFIFO = 0;        // Reset TX FIFO
    SpibRegs.SPIFFTX.bit.TXFIFO = 1;
    SpibRegs.SPIFFTX.bit.TXFFINTCLR = 1;    // Clear TX FIFO INT
    SpibRegs.SPIFFTX.bit.TXFFIENA = 0;      // Disable TX FIFO INT
    SpibRegs.SPIFFTX.bit.TXFFIL = 0;

    SpibRegs.SPIFFRX.bit.RXFFOVFCLR = 1;    // Clear RX FIFO Overflow Flag
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 0;   // Reset RX FIFO
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 1;
    SpibRegs.SPIFFRX.bit.RXFFINTCLR = 1;    // Clear RX FIFO INT
    SpibRegs.SPIFFRX.bit.RXFFIENA = 0;      // Disable RX FIFO INT
    SpibRegs.SPIFFRX.bit.RXFFIL = 3;


    SpibRegs.SPICCR.bit.CLKPOLARITY = 1;    // Refer to  below "SPI Clocking Schemes"
    SpibRegs.SPICCR.bit.HS_MODE = 0;        // Disable High Speed Mode
    SpibRegs.SPICCR.bit.SPILBK = 0;         // Disable SPI Loopback Mode
    SpibRegs.SPICCR.bit.SPICHAR = 15;       // Character Length = 16-bit word


    SpibRegs.SPICTL.bit.OVERRUNINTENA = 0;  // Disable Overrun Interrupt
    SpibRegs.SPICTL.bit.CLK_PHASE = 1;      // Refer to  below "SPI Clocking Schemes"
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;   // SPI Master
    SpibRegs.SPICTL.bit.TALK = 1;           // Enable Transmit
    SpibRegs.SPICTL.bit.SPIINTENA = 0;      // Disable SPI INT

    //
    // "SPI Clocking Schemes"
    //
    // CLKPOLARITY = 0 / CLK_PHASE = 0
    // Rising Edge Without Delay
    // Transmits data on the rising edge of the SPICLK signal
    // Receives data on the falling edge of the SPICLK signal.
    //
    // CLKPOLARITY = 0 / CLK_PHASE = 1
    // Rising edge with delay
    // Transmits data one half-cycle ahead of the rising edge of the SPICLK signal
    // Receives data on the rising edge of the SPICLK signal.
    //
    // CLKPOLARITY = 1 / CLK_PHASE = 0
    // Falling edge without delay
    // Transmits data on the falling edge of the SPICLK
    // Receives data on the rising edge of the SPICLK.
    //
    // CLKPOLARITY = 1 / CLK_PHASE = 1
    // Falling edge with delay
    // Transmits data one half-cycle ahead of the falling edge of the SPICLK signal
    // Receives data on the falling edge of the SPICLK signal.
    //


    //
    // For SPIBRR = 3 to 127: SPI Baud Rate = LSPCLK / (SPIBRR + 1)
    // For SPIBRR = 0, 1, or 2: SPI Baud Rate = LSPCLK / 4
    // LSPCLK = 200MHz
    //
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 9;   //20MHz
    //SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 19;   //10MHz

    SpibRegs.SPICCR.bit.SPISWRESET = 1;

}


////////////////////////////////////////////////////////////////////
//
//
//
void Init_SPI_B_Reg_for_Master_SDRAM_SYNC(void)
{
    SpibRegs.SPICCR.bit.SPISWRESET = 0;     // SPI SW Reset

    SpibRegs.SPIFFTX.bit.SPIRST = 0;        // SPI Reset
    SpibRegs.SPIFFTX.bit.SPIRST = 1;
    SpibRegs.SPIFFTX.bit.SPIFFENA = 1;      // Enable FIFO
    SpibRegs.SPIFFTX.bit.TXFIFO = 0;        // Reset TX FIFO
    SpibRegs.SPIFFTX.bit.TXFIFO = 1;
    SpibRegs.SPIFFTX.bit.TXFFINTCLR = 1;    // Clear TX FIFO INT
    SpibRegs.SPIFFTX.bit.TXFFIENA = 0;      // Disable TX FIFO INT
    SpibRegs.SPIFFTX.bit.TXFFIL = 0;

    SpibRegs.SPIFFRX.bit.RXFFOVFCLR = 1;    // Clear RX FIFO Overflow Flag
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 0;   // Reset RX FIFO
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 1;
    SpibRegs.SPIFFRX.bit.RXFFINTCLR = 1;    // Clear RX FIFO INT
    SpibRegs.SPIFFRX.bit.RXFFIENA = 0;      // Disable RX FIFO INT
    SpibRegs.SPIFFRX.bit.RXFFIL = 3;


    SpibRegs.SPICCR.bit.CLKPOLARITY = 1;    // Refer to  below "SPI Clocking Schemes"
    SpibRegs.SPICCR.bit.HS_MODE = 0;        // Disable High Speed Mode
    SpibRegs.SPICCR.bit.SPILBK = 0;         // Disable SPI Loopback Mode
    SpibRegs.SPICCR.bit.SPICHAR = 15;       // Character Length = 16-bit word


    SpibRegs.SPICTL.bit.OVERRUNINTENA = 0;  // Disable Overrun Interrupt
    SpibRegs.SPICTL.bit.CLK_PHASE = 1;      // Refer to  below "SPI Clocking Schemes"
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;   // SPI Master
    SpibRegs.SPICTL.bit.TALK = 1;           // Enable Transmit
    SpibRegs.SPICTL.bit.SPIINTENA = 0;      // Disable SPI INT

    //
    // "SPI Clocking Schemes"
    //
    // CLKPOLARITY = 0 / CLK_PHASE = 0
    // Rising Edge Without Delay
    // Transmits data on the rising edge of the SPICLK signal
    // Receives data on the falling edge of the SPICLK signal.
    //
    // CLKPOLARITY = 0 / CLK_PHASE = 1
    // Rising edge with delay
    // Transmits data one half-cycle ahead of the rising edge of the SPICLK signal
    // Receives data on the rising edge of the SPICLK signal.
    //
    // CLKPOLARITY = 1 / CLK_PHASE = 0
    // Falling edge without delay
    // Transmits data on the falling edge of the SPICLK
    // Receives data on the rising edge of the SPICLK.
    //
    // CLKPOLARITY = 1 / CLK_PHASE = 1
    // Falling edge with delay
    // Transmits data one half-cycle ahead of the falling edge of the SPICLK signal
    // Receives data on the falling edge of the SPICLK signal.
    //


    //
    // For SPIBRR = 3 to 127: SPI Baud Rate = LSPCLK / (SPIBRR + 1)
    // For SPIBRR = 0, 1, or 2: SPI Baud Rate = LSPCLK / 4
    // LSPCLK = 200MHz
    //
    //SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 9;   //20MHz
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 19;   //10MHz

    SpibRegs.SPICCR.bit.SPISWRESET = 1;

}




////////////////////////////////////////////////////////////////////
//
// Init SPIB for Slave
//
void Init_SPI_B_Reg_for_Slave(void)
{
    //EALLOW;  // This is needed to write to EALLOW protected registers
    //PieVectTable.SPIB_RX_INT = &spib_rx_isr;
    //EDIS;    // This is needed to disable write to EALLOW protected registers

    SpibRegs.SPICCR.bit.SPISWRESET = 0;     // SPI SW Reset

    SpibRegs.SPIFFTX.bit.SPIRST = 0;        // SPI Reset
    SpibRegs.SPIFFTX.bit.SPIRST = 1;
    SpibRegs.SPIFFTX.bit.SPIFFENA = 1;      // Enable FIFO
    SpibRegs.SPIFFTX.bit.TXFIFO = 0;        // Reset TX FIFO
    SpibRegs.SPIFFTX.bit.TXFIFO = 1;
    SpibRegs.SPIFFTX.bit.TXFFINTCLR = 1;    // Clear TX FIFO INT
    SpibRegs.SPIFFTX.bit.TXFFIENA = 0;      // Disable TX FIFO INT
    SpibRegs.SPIFFTX.bit.TXFFIL = 0;        // TXFFIL = 0

    SpibRegs.SPIFFRX.bit.RXFFOVFCLR = 1;    // Clear RX FIFO Overflow Flag
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 0;   // Reset RX FIFO
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 1;
    SpibRegs.SPIFFRX.bit.RXFFINTCLR = 1;    // Clear RX FIFO INT

    SpibRegs.SPIFFRX.bit.RXFFIENA = 1;      // Enable RX FIFO INT
    //SpibRegs.SPIFFRX.bit.RXFFIENA = 0;

    SpibRegs.SPIFFRX.bit.RXFFIL = 15;


    SpibRegs.SPICCR.bit.CLKPOLARITY = 1;    // Refer to  below "SPI Clocking Schemes"
    SpibRegs.SPICCR.bit.HS_MODE = 0;        // Disable High Speed Mode
    SpibRegs.SPICCR.bit.SPILBK = 0;         // Disable SPI Loopback Mode
    SpibRegs.SPICCR.bit.SPICHAR = 15;       // Character Length = 16-bit word


    SpibRegs.SPICTL.bit.OVERRUNINTENA = 0;  // Disable Overrun Interrupt
    SpibRegs.SPICTL.bit.CLK_PHASE = 1;      // Refer to  below "SPI Clocking Schemes"
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 0;   // SPI Slave
    SpibRegs.SPICTL.bit.TALK = 0;           // Disable Transmit
    SpibRegs.SPICTL.bit.SPIINTENA = 0;      // Disable SPI INT

    //
    // "SPI Clocking Schemes"
    //
    // CLKPOLARITY = 0 / CLK_PHASE = 0
    // Rising Edge Without Delay
    // Transmits data on the rising edge of the SPICLK signal
    // Receives data on the falling edge of the SPICLK signal.
    //
    // CLKPOLARITY = 0 / CLK_PHASE = 1
    // Rising edge with delay
    // Transmits data one half-cycle ahead of the rising edge of the SPICLK signal
    // Receives data on the rising edge of the SPICLK signal.
    //
    // CLKPOLARITY = 1 / CLK_PHASE = 0
    // Falling edge without delay
    // Transmits data on the falling edge of the SPICLK
    // Receives data on the rising edge of the SPICLK.
    //
    // CLKPOLARITY = 1 / CLK_PHASE = 1
    // Falling edge with delay
    // Transmits data one half-cycle ahead of the falling edge of the SPICLK signal
    // Receives data on the falling edge of the SPICLK signal.
    //

    //
    // For SPIBRR = 3 to 127: SPI Baud Rate = LSPCLK / (SPIBRR + 1)
    // For SPIBRR = 0, 1, or 2: SPI Baud Rate = LSPCLK / 4
    // LSPCLK = 200MHz
    //
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 9;   //20MHz
    //SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 19;   //10MHz

    SpibRegs.SPICCR.bit.SPISWRESET = 1;

    //
    // Enable interrupts
    //
    //PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    //PieCtrlRegs.PIEIER6.bit.INTx3 = 1;   // PIE Group 6, INT3

    //
    // Set CPU IER
    //
    //IER |= M_INT6;

}

////////////////////////////////////////////////////////////////////
//
//
//
void Init_SPI_B_Reg_for_Slave_SDRAM_SYNC(void)
{
    //EALLOW;  // This is needed to write to EALLOW protected registers
    //PieVectTable.SPIB_RX_INT = &spib_rx_isr;
    //EDIS;    // This is needed to disable write to EALLOW protected registers

    SpibRegs.SPICCR.bit.SPISWRESET = 0;     // SPI SW Reset

    SpibRegs.SPIFFTX.bit.SPIRST = 0;        // SPI Reset
    SpibRegs.SPIFFTX.bit.SPIRST = 1;
    SpibRegs.SPIFFTX.bit.SPIFFENA = 1;      // Enable FIFO
    SpibRegs.SPIFFTX.bit.TXFIFO = 0;        // Reset TX FIFO
    SpibRegs.SPIFFTX.bit.TXFIFO = 1;
    SpibRegs.SPIFFTX.bit.TXFFINTCLR = 1;    // Clear TX FIFO INT
    SpibRegs.SPIFFTX.bit.TXFFIENA = 0;      // Disable TX FIFO INT
    SpibRegs.SPIFFTX.bit.TXFFIL = 0;        // TXFFIL = 0

    SpibRegs.SPIFFRX.bit.RXFFOVFCLR = 1;    // Clear RX FIFO Overflow Flag
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 0;   // Reset RX FIFO
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 1;
    SpibRegs.SPIFFRX.bit.RXFFINTCLR = 1;    // Clear RX FIFO INT

    SpibRegs.SPIFFRX.bit.RXFFIENA = 1;      // Enable RX FIFO INT
    //SpibRegs.SPIFFRX.bit.RXFFIENA = 0;

    //SpibRegs.SPIFFRX.bit.RXFFIL = 5;
    SpibRegs.SPIFFRX.bit.RXFFIL = 2;


    SpibRegs.SPICCR.bit.CLKPOLARITY = 1;    // Refer to  below "SPI Clocking Schemes"
    SpibRegs.SPICCR.bit.HS_MODE = 0;        // Disable High Speed Mode
    SpibRegs.SPICCR.bit.SPILBK = 0;         // Disable SPI Loopback Mode
    SpibRegs.SPICCR.bit.SPICHAR = 15;       // Character Length = 16-bit word


    SpibRegs.SPICTL.bit.OVERRUNINTENA = 0;  // Disable Overrun Interrupt
    SpibRegs.SPICTL.bit.CLK_PHASE = 1;      // Refer to  below "SPI Clocking Schemes"
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 0;   // SPI Slave
    SpibRegs.SPICTL.bit.TALK = 0;           // Disable Transmit
    SpibRegs.SPICTL.bit.SPIINTENA = 0;      // Disable SPI INT

    //
    // "SPI Clocking Schemes"
    //
    // CLKPOLARITY = 0 / CLK_PHASE = 0
    // Rising Edge Without Delay
    // Transmits data on the rising edge of the SPICLK signal
    // Receives data on the falling edge of the SPICLK signal.
    //
    // CLKPOLARITY = 0 / CLK_PHASE = 1
    // Rising edge with delay
    // Transmits data one half-cycle ahead of the rising edge of the SPICLK signal
    // Receives data on the rising edge of the SPICLK signal.
    //
    // CLKPOLARITY = 1 / CLK_PHASE = 0
    // Falling edge without delay
    // Transmits data on the falling edge of the SPICLK
    // Receives data on the rising edge of the SPICLK.
    //
    // CLKPOLARITY = 1 / CLK_PHASE = 1
    // Falling edge with delay
    // Transmits data one half-cycle ahead of the falling edge of the SPICLK signal
    // Receives data on the falling edge of the SPICLK signal.
    //

    //
    // For SPIBRR = 3 to 127: SPI Baud Rate = LSPCLK / (SPIBRR + 1)
    // For SPIBRR = 0, 1, or 2: SPI Baud Rate = LSPCLK / 4
    // LSPCLK = 200MHz
    //
    //SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 9;   //20MHz
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 19;   //10MHz

    SpibRegs.SPICCR.bit.SPISWRESET = 1;

    //
    // Enable interrupts
    //
    //PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    //PieCtrlRegs.PIEIER6.bit.INTx3 = 1;   // PIE Group 6, INT3

    //
    // Set CPU IER
    //
    //IER |= M_INT6;

}


////////////////////////////////////////////////////////////////////
//
// SPI B RX Interrupt ISR
//
__interrupt void spib_rx_isr(void)
{
    SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig = 1;

    SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SpibRegs.SPIRXBUF;
    SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SpibRegs.SPIRXBUF;
    SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SpibRegs.SPIRXBUF;

    SpibRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
    SpibRegs.SPIFFRX.bit.RXFFINTCLR=1;  // Clear Interrupt flag

    //
    // Acknowledge this interrupt to receive more interrupts from group 6
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;
}


/////////////////////////////////////////////////////////////////////
void Init_SPI_Module_Variables(void)
{
    Uint16 i;

    for ( i = 0; i < 3; i ++ )
    {
        SPI_Module_Variables.SPIB_Tx_Data[i] = 0;
        SPI_Module_Variables.SPIB_Tx_Data_Shadow[i] = 0;

        SPI_Module_Variables.SPIB_Rx_Data[i] = 0;
        SPI_Module_Variables.SPIB_Rx_Data_Shadow[i] = 0;
    }

    SPI_Module_Variables.SPI_Module_Control_flag.all = 0;
}







//
// End of file
//
