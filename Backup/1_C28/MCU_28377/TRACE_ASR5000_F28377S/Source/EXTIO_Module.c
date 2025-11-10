//###########################################################################
//
// FILE:    EXTIO_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210819
//###########################################################################

#include "Global_VariableDefs.h"






////////////////////////////////////////////////////////////////////
Uint16 Read_EXTIO_IN(void)
{
    Uint16 data;
    long *addr;

    addr = (long *)EXTIO_IN_ADDR;

    memcpy_fast( &data, addr, 1 );

    data = data & 0xFF;

    return ( data );

}


////////////////////////////////////////////////////////////////////
void Write_EXTIO_OUT(Uint16 data)
{
    long *addr;

    addr = (long *)EXTIO_OUT_ADDR;

    //*addr = data;

    memcpy_fast( addr, &data, 1 );

}


////////////////////////////////////////////////////////////////////
void Read_EXTIO_Process(void)
{
    Uint16 buf;


    buf = Read_EXTIO_IN(); //73 cycles

    Global_Variables.EXT_INPUT_FLAG.bit.SEQ_START = 0x01 & buf;
    buf = buf >> 1;
    Global_Variables.EXT_INPUT_FLAG.bit.OUTPUT_ON = 0x01 & buf;
    buf = buf >> 1;
    Global_Variables.EXT_INPUT_FLAG.bit.OUTPUT_OFF = 0x01 & buf;
    buf = buf >> 1;
    Global_Variables.EXT_INPUT_FLAG.bit.UNDEFINED_IN0 = 0x01 & buf;
    buf = buf >> 1;
    Global_Variables.EXT_INPUT_FLAG.bit.BRANCH2 = 0x01 & buf;
    buf = buf >> 1;
    Global_Variables.EXT_INPUT_FLAG.bit.BRANCH1 = 0x01 & buf;
    buf = buf >> 1;
    Global_Variables.EXT_INPUT_FLAG.bit.SEQ_HOLD = 0x01 & buf;
    buf = buf >> 1;
    Global_Variables.EXT_INPUT_FLAG.bit.SEQ_STOP = 0x01 & buf;

    //
    // H -> L, 觸發SEQ START
    //
    if ( ( Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.SEQ_START == 1 ) && ( Global_Variables.EXT_INPUT_FLAG.bit.SEQ_START == 0 ) )
    {
        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 1;
    }

    //
    // H -> L, 觸發Output ON
    //
    if ( ( Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.OUTPUT_ON == 1 ) && ( Global_Variables.EXT_INPUT_FLAG.bit.OUTPUT_ON == 0 ) )
    {
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 1;

    }

    //
    // H -> L, 觸發Output OFF
    //
    if ( ( Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.OUTPUT_OFF == 1 ) && ( Global_Variables.EXT_INPUT_FLAG.bit.OUTPUT_OFF == 0 ) )
    {
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

    }


    //
    // H -> L, 觸發SEQ BRANCH2
    //
    if ( ( Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.BRANCH2 == 1 ) && ( Global_Variables.EXT_INPUT_FLAG.bit.BRANCH2 == 0 ) )
    {
        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2 = 1;
    }

    //
    // H -> L, 觸發SEQ BRANCH1
    //
    if ( ( Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.BRANCH1 == 1 ) && ( Global_Variables.EXT_INPUT_FLAG.bit.BRANCH1 == 0 ) )
    {
        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1 = 1;
    }

    //
    // H -> L, 觸發SEQ HOLD
    //
    if ( ( Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.SEQ_HOLD == 1 ) && ( Global_Variables.EXT_INPUT_FLAG.bit.SEQ_HOLD == 0 ) )
    {
        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Hold = 1;
    }


    //
    // H -> L, 觸發SEQ STOP
    //
    if ( ( Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.SEQ_STOP == 1 ) && ( Global_Variables.EXT_INPUT_FLAG.bit.SEQ_STOP == 0 ) )
    {
        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Stop = 1;
    }

    //
    // 儲存更新EXT_INPUT Shadow
    //
    Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.SEQ_START = Global_Variables.EXT_INPUT_FLAG.bit.SEQ_START;
    Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.OUTPUT_ON = Global_Variables.EXT_INPUT_FLAG.bit.OUTPUT_ON;
    Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.OUTPUT_OFF = Global_Variables.EXT_INPUT_FLAG.bit.OUTPUT_OFF;
    Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.BRANCH2 = Global_Variables.EXT_INPUT_FLAG.bit.BRANCH1;
    Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.BRANCH1 = Global_Variables.EXT_INPUT_FLAG.bit.SEQ_START;
    Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.SEQ_HOLD = Global_Variables.EXT_INPUT_FLAG.bit.SEQ_HOLD;
    Global_Variables.EXT_INPUT_FLAG_SHADOW.bit.SEQ_STOP = Global_Variables.EXT_INPUT_FLAG.bit.SEQ_STOP;

}



//
// End of file
//
