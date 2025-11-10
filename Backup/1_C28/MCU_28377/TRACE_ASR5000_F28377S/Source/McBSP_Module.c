//###########################################################################
//
// FILE:    McBSP_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210913
//###########################################################################


//#define McBSP_MASTER


#include "Global_VariableDefs.h"


volatile McBSP_MODULE_VARIABLES_REG McBSP_Module_Variables;





/////////////////////////////////////////////////////////////////////
void Init_McBSP_Module_Variables(void)
{

    McBSP_Module_Variables.McBSP_spi_state = SPI_Ready_state;

    local_memory_reset( McBSP_Module_Variables.RXBUFFER , SPI_BUFFER_SIZE );
    local_memory_reset( McBSP_Module_Variables.TXBUFFER , SPI_BUFFER_SIZE );

    McBSP_Module_Variables.Flags.all = 0;


    McBSP_Module_Variables.TXBUFFER[0] = 0xFFFF;
    McBSP_Module_Variables.TXBUFFER[1] = 0;


}




/////////////////////////////////////////////////////////////////////
void Init_McBSP_B_Reg(void)
{

    //CLKX = SPICLK
    //FSX = SPISTE

    /*------------------------------------------*/
    McbspbRegs.SPCR1.all = 0x0000;
    // Reset Receiver, Right justify word, Digital loopback disable.

    McbspbRegs.SPCR2.all = 0x0000;
    // Reset FS generator, sample rate generator & transmitter

    /*------------------------------------------*/
    // Digital loop back
    //McbspbRegs.SPCR1.bit.DLB = 1;

    /*------------------------------------------*/
    //設定 for spi slave

    McbspbRegs.SPCR1.bit.CLKSTP = 0x03;
    McbspbRegs.PCR.bit.CLKXP = 0;
    McbspbRegs.PCR.bit.CLKRP = 1;

    McbspbRegs.PCR.bit.CLKXM = 0;
    // McBSP設定SPI Slave, CLKXM = 0

    McbspbRegs.PCR.bit.SCLKME = 0;
    McbspbRegs.SRGR2.bit.CLKSM = 1;
    // SRG來源選擇CPU Clock

    //McbspbRegs.SRGR1.bit.CLKGDV = 0x09;
    // SRG頻率來源 / ( CLKGDV + 1 ) = CLKG
    // 預設LSPCLK = SYSCLK / 1 = 200MHz
    // 所以CLKG = 200M / ( 9 + 1 ) = 20MHz

    McbspbRegs.SRGR1.bit.CLKGDV = 0x01;
    // SRG頻率來源 / ( CLKGDV + 1 ) = CLKG
    // 預設LSPCLK = SYSCLK / 1 = 200MHz
    // 所以CLKG = 200M / ( 1 + 1 ) = 100MHz

    McbspbRegs.PCR.bit.FSXM = 0;
    //McbspbRegs.SRGR2.bit.FSGM = 0;
    // for spi slave

    McbspbRegs.PCR.bit.FSXP = 1;
    // FSX Low動作

    McbspbRegs.XCR2.bit.XDATDLY = 0;
    // FSX setup time
    // 1 for master mode.
    // 0 for slave mode (Transmit)

    McbspbRegs.RCR2.bit.RDATDLY = 0;
    // FSX setup time
    // 1 for master mode.
    // 0 for slave mode (Receive)

    /*------------------------------------------*/
    McbspbRegs.XCR2.bit.XPHASE = 0;
    McbspbRegs.RCR2.bit.RPHASE = 0;
    McbspbRegs.XCR1.bit.XFRLEN1 = 0;
    McbspbRegs.RCR1.bit.RFRLEN1 = 0;
    McbspbRegs.RCR1.bit.RWDLEN1 = 5;     // 32-bit word
    McbspbRegs.XCR1.bit.XWDLEN1 = 5;     // 32-bit word

    /*------------------------------------------*/
    McbspbRegs.SPCR2.bit.GRST = 1;
    // Enable the sample rate generator

    delay_loop();
    // Wait at least 2 SRG clock cycles

    McbspbRegs.SPCR2.bit.XRST = 1;
    // Release TX from Reset
    McbspbRegs.SPCR1.bit.RRST = 1;
    // Release RX from Reset

    delay_loop();
    // Wait at least 2 SRG clock cycles

    McbspbRegs.SPCR2.bit.FRST = 1;
    // Frame Sync Generator reset

    McbspbRegs.SPCR2.bit.XRST = 1;    // Release TX from Reset
    McbspbRegs.SPCR1.bit.RRST = 1;    // Release RX from Reset

    while ( McbspbRegs.SPCR2.bit.XRDY == 0 )
    {
        //wait XRDY
        asm("       NOP");
    }

    McbspbRegs.DXR2.all = McBSP_Module_Variables.TXBUFFER[0];
    McbspbRegs.DXR1.all = McBSP_Module_Variables.TXBUFFER[1];

}


/////////////////////////////////////////////////////////////////////
void Init_McBSP_B_GPIO(void)
{
    //SIMO MDRB
    GPIO_SetupPinMux(25, GPIO_MUX_CPU1, 3);
    GPIO_SetupPinOptions(25, GPIO_INPUT, GPIO_PULLUP);

    //SOMI MDXB
    GPIO_SetupPinMux(24, GPIO_MUX_CPU1, 3);
    GPIO_SetupPinOptions(24, GPIO_OUTPUT, GPIO_ASYNC);

    //CLK MCLKB
    GPIO_SetupPinMux(26, GPIO_MUX_CPU1, 3);
    GPIO_SetupPinOptions(26, GPIO_INPUT, GPIO_PULLUP);

    //CS
    GPIO_SetupPinMux(27, GPIO_MUX_CPU1, 3);
    GPIO_SetupPinOptions(27, GPIO_INPUT, GPIO_PULLUP);
}




//
// End of file
//
