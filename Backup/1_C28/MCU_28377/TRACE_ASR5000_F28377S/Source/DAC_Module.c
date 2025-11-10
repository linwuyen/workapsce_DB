//###########################################################################
//
// FILE:    DAC_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20230810
//###########################################################################

#include "Global_VariableDefs.h"


////////////////////////////////////////////////////////////////////
void Init_DAC_Module(void)
{
    EALLOW;
    DacaRegs.DACOUTEN.bit.DACOUTEN = 1;
    DacaRegs.DACCTL.bit.DACREFSEL = 1;
    DacaRegs.DACVALS.all = 0;
    EDIS;

}



//
// End of file
//
