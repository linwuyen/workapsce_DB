//###########################################################################
//
// FILE:    EMIF_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210819
//###########################################################################

#include "Global_VariableDefs.h"






////////////////////////////////////////////////////////////////////
void Init_EMIF1_Reg(void)
{
    //
    // Configure to run EMIF1 on half Rate (EMIF1CLK = CPU1SYSCLK/2)
    //
    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EMIF1CLKDIV = 0x1; //EMIF1CLK = 100MHz
    EDIS;

    EALLOW;
    //
    // Disable Access Protection (CPU_FETCH/CPU_WR/DMA_WR)
    //
    Emif1ConfigRegs.EMIF1ACCPROT0.all = 0x0;

    //
    // Commit the configuration related to protection. Till this bit remains
    // set content of EMIF1ACCPROT0 register can't be changed.
    //
    Emif1ConfigRegs.EMIF1COMMIT.all = 0x1;

    //
    // Lock the configuration so that EMIF1COMMIT register can't be changed
    // any more.
    //
    Emif1ConfigRegs.EMIF1LOCK.all = 0x1;

    EDIS;

    //
    // Configure GPIO pins for EMIF1
    //
    setup_emif1_pinmux_sdram_16bit(0);

    //
    // Configure SDRAM control registers
    //
    // Need to be programmed based on SDRAM Data-Sheet.
    //T_RFC = 60ns = 0x6
    //T_RP  = 18ns = 0x1
    //T_RCD = 18ns = 0x1
    //T_WR  = 1CLK + 6 ns = 0x1
    //T_RAS = 42ns = 0x4
    //T_RC  = 60ns = 0x6
    //T_RRD = 12ns = 0x1
    //
    //Emif1Regs.SDRAM_TR.all = 0x31114610;
    Emif1Regs.SDRAM_TR.bit.T_RFC = 0x06;
    Emif1Regs.SDRAM_TR.bit.T_RP = 0x01;
    Emif1Regs.SDRAM_TR.bit.T_RCD = 0x01;
    Emif1Regs.SDRAM_TR.bit.T_WR = 0x01;
    Emif1Regs.SDRAM_TR.bit.T_RAS = 0x04;
    Emif1Regs.SDRAM_TR.bit.T_RC = 0x06;
    Emif1Regs.SDRAM_TR.bit.T_RRD = 0x01;


    //
    //Txsr = 70ns = 0x7
    //
    Emif1Regs.SDR_EXT_TMNG.bit.T_XS = 0x7;

    //
    //Tref = 64ms for 8192 ROW, RR = 64000*100(Tfrq)/8192 = 781.25 (0x30E)
    //
    Emif1Regs.SDRAM_RCR.bit.REFRESH_RATE = 0x30E;

    //
    //PAGESIZE=1 (512 elements per ROW), IBANK = 2 (4 BANK), CL = 2,
    //NM = 1 (16bit)
    //
    //Emif1Regs.SDRAM_CR.all = 0x4521;
    Emif1Regs.SDRAM_CR.bit.BIT_11_9_LOCK = 0x01;
    Emif1Regs.SDRAM_CR.bit.CL = 0x02;
    Emif1Regs.SDRAM_CR.bit.IBANK = 0x02;
    Emif1Regs.SDRAM_CR.bit.PAGESIGE = 0x01;
    Emif1Regs.SDRAM_CR.bit.NM = 0x01;
    Emif1Regs.SDRAM_CR.bit.BIT_11_9_LOCK = 0x00;

    //
    //Configure the access timing for CS2 space
    //
    Emif1Regs.ASYNC_CS2_CR.bit.SS = 1;
    Emif1Regs.ASYNC_CS2_CR.bit.EW = 0;
    Emif1Regs.ASYNC_CS2_CR.bit.W_SETUP = 2;
    Emif1Regs.ASYNC_CS2_CR.bit.W_HOLD = 2;
    Emif1Regs.ASYNC_CS2_CR.bit.W_STROBE = 2;
    Emif1Regs.ASYNC_CS2_CR.bit.R_SETUP = 2;
    Emif1Regs.ASYNC_CS2_CR.bit.R_HOLD = 2;
    Emif1Regs.ASYNC_CS2_CR.bit.R_STROBE = 2;
    Emif1Regs.ASYNC_CS2_CR.bit.TA = 2;
    Emif1Regs.ASYNC_CS2_CR.bit.ASIZE = 0x1;





}










//
// End of file
//
