//###########################################################################
//
// FILE:    Phase_Sync_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20230615
//###########################################################################

#include "Global_VariableDefs.h"



volatile PHASE_SYNC_MODULE_REG Phase_Sync_Module;


////////////////////////////////////////////////////////////////////
//
//
//
void Init_Phase_Sync_Module_Variables(void)
{
    Phase_Sync_Module.LINE_Phase_Sync_Flags.all = 0;

    Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err = 0;

    //Phase_Sync_Module.LINE_Phase_Sync_Variables.K_Angle = 5e-6;
    Phase_Sync_Module.LINE_Phase_Sync_Variables.K_Angle = 0.0001; //實測後會收斂

    //Phase_Sync_Module.LINE_Phase_Sync_Variables.K_f = 5.0;
    //Phase_Sync_Module.LINE_Phase_Sync_Variables.K_f = 0.5; //實測後比較穩
    Phase_Sync_Module.LINE_Phase_Sync_Variables.K_f = 50.0; //實測後收斂快速

    Phase_Sync_Module.LINE_Phase_Sync_Variables.SYNC_T = 0;
    Phase_Sync_Module.LINE_Phase_Sync_Variables.delta_T = 0;
    Phase_Sync_Module.LINE_Phase_Sync_Variables.ferr = 0;
    Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate = 40.0;
    Phase_Sync_Module.LINE_Phase_Sync_Variables.fs = 0;
    //Phase_Sync_Module.LINE_Phase_Sync_Variables.Phase_shift = 2048.0;
    Phase_Sync_Module.LINE_Phase_Sync_Variables.Phase_shift = 0; //配合實測為0, 但還需確認
    Phase_Sync_Module.LINE_Phase_Sync_Variables.dds_addr = 0;

}



////////////////////////////////////////////////////////////////////
//
//
//
void Phase_Sync_Process(void)
{
    float festimate, ferr, K_f, delta_T, Angle_err, K_Angle;

    Phase_Sync_Module.LINE_Phase_Sync_Variables.fs = EXTAD_SAMPLE_FREQ / cpu_timer_module.EXTAD_TTL_Period;
    Phase_Sync_Module.LINE_Phase_Sync_Variables.delta_T = 1 / Phase_Sync_Module.LINE_Phase_Sync_Variables.fs;
    Phase_Sync_Module.LINE_Phase_Sync_Variables.ferr = Phase_Sync_Module.LINE_Phase_Sync_Variables.fs - Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate;

    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source == SYNC_EXT )
    {
        Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err = Phase_Sync_Module.LINE_Phase_Sync_Variables.fs * SYNC_EXT_PHASE_OFFSET
                                                                + Phase_Sync_Module.LINE_Phase_Sync_Variables.Phase_shift
                                                                - Phase_Sync_Module.LINE_Phase_Sync_Variables.dds_addr;
    }
    else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source == SYNC_LINE )
    {
        Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err = Phase_Sync_Module.LINE_Phase_Sync_Variables.Phase_shift
                                                                - Phase_Sync_Module.LINE_Phase_Sync_Variables.dds_addr;
    }

    //
    // 箝制Angle_err
    //
    if ( Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err > 2047.0 )
    {
        Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err = Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err - 4095.0;
    }
    else if ( Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err < -2047.0 )
    {
        Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err = Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err + 4095.0;
    }

    festimate = Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate;
    ferr = Phase_Sync_Module.LINE_Phase_Sync_Variables.ferr;
    delta_T = Phase_Sync_Module.LINE_Phase_Sync_Variables.delta_T;
    Angle_err = Phase_Sync_Module.LINE_Phase_Sync_Variables.Angle_err;
    K_f = Phase_Sync_Module.LINE_Phase_Sync_Variables.K_f;
    K_Angle = Phase_Sync_Module.LINE_Phase_Sync_Variables.K_Angle;

    //
    // 鎖相演算法
    //
    Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate = festimate + ferr * delta_T * K_f + Angle_err * K_Angle;


    //
    // 箝制f estimate
    //
    if ( Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate > 2000.0 )
    {
        Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate = 2000.0;
    }
    else if ( Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate <= 1.0 )
    {
        Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate = 1.0;
    }

    Global_Variables.FreqStep = Phase_Sync_Module.LINE_Phase_Sync_Variables.festimate * _1HZ;

    Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all = Phase_Sync_Module.LINE_Phase_Sync_Variables.fs;

    Global_Variables.POWER_DISPLAY_VARIABLES.FREQ_display.all = Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all;
}






//
// End of file
//
