//###########################################################################
//
// FILE:    Output_Control_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211014
//###########################################################################



#include "Global_VariableDefs.h"

volatile OUTPUT_CONTROL_MODULE_VARIABLES_REG Output_Control_Module;








/////////////////////////////////////////////////////////////////////
//
// 模組初始化
//
void Init_Output_Control_Module_Variables(void)
{
    Output_Control_Module.Flag.all = 0;

    Output_Control_Module.WAVE_Compensate_Offset = 0;

    Output_Control_Module.Compensate_Control_Stable_Count = 10;

    Output_Control_Module.Control_Mode_Stable_Count = 10;

    Output_Control_Module.DCV_Compensate_State = DCV_NO_COMPENSATE;
    Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;
    Output_Control_Module.CC_Compensate_State = CC_NO_COMPENSATE;
    Output_Control_Module.CP_Compensate_State = CP_NO_COMPENSATE;

    Output_Control_Module.Power_Control_Mode = CV_CONTROL_MODE;



    Output_Control_Module.OCP_Bypass_Control_State = OCP_CHECK_NORMAL;

    Output_Control_Module.DCAC_ENA_shadow = 0;


    Output_Control_Module.MS_Balance_Compensate_State = MS_BALANCE_NO_COMPENSATE;


    Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;
    Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;

    Output_Control_Module.Freq_Compensation_gain = 1.0;

    Output_Control_Module.Check_CC_CP_Delay_Counter = 0;
}





////////////////////////////////////////////////////////////////////
//
// 補償控制
//
void Compensate_Calc_Process(void)
{

#ifndef BYPASS_CAL_CV_CC_CP_COMPENSATE

    if ( Main_State_Module.Machine_state == OUTPUT_STATE )
    {
        //
        // 判斷CV, CC, CP
        //
        switch( Output_Control_Module.Power_Control_Mode )
        {
            /*-----------------------------------------------------------------------------------------*/
            case CV_CONTROL_MODE:
            {
                //
                // Check_CC_CP_Delay_Counter
                // 主要是針對高頻輸出時, 延遲讓輸出穩定再判斷CC或CP
                // 避免容易頂到而誤判
                //

                if ( Output_Control_Module.Check_CC_CP_Delay_Counter == 0 )
                {
                    float i_rms;
                    float i_rms_limit;
                    float s_va;
                    float power_limit;


                    //
                    // 判斷進入CC
                    // 優先判斷CC,因為CC發生時, 不會再超過Power limit
                    //

                    i_rms = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
                    i_rms_limit = (float)Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all * 0.0001;

                    if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable == 1 )
                         && ( i_rms > ( i_rms_limit * IRMS_LIMIT_RATIO ) ) )
                    {
                        Output_Control_Module.CC_Compensate_State = CC_NO_COMPENSATE;

                        //
                        // 設定EXTIO
                        //
                        Global_Variables.EXT_OUTPUT_FLAG.bit.I_LIMITER = 1;

                        //
                        // 轉態
                        //
                        Output_Control_Module.Power_Control_Mode = CC_CONTROL_MODE;

                        break;
                    }

                    //
                    // 判斷進入CP
                    //

                    //s_va = Global_Variables.POWER_DISPLAY_VARIABLES.S_display_shadow;
                    s_va = Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all;
                    power_limit = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level;

                    if ( s_va > ( power_limit * POWER_LIMIT_RATIO ) )
                    {
                        Output_Control_Module.CP_Compensate_State = CP_NO_COMPENSATE;

                        //
                        // 設定EXTIO
                        //
                        Global_Variables.EXT_OUTPUT_FLAG.bit.I_LIMITER = 1;

                        //
                        // 轉態
                        //
                        Output_Control_Module.Power_Control_Mode = CP_CONTROL_MODE;

                        break;
                    }
                }
                else
                {
                    Output_Control_Module.Check_CC_CP_Delay_Counter --;
                }


                //
                // 執行CV補償
                //

                if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow == 1 ) )
                {
                    CV_Compensate_Loop();
                }

                break;
            }

            /*-----------------------------------------------------------------------------------------*/
            case CC_CONTROL_MODE:
            {

                if ( ( PID_Control_Module.CC_Control_loop.Out >= 0 ) && ( PID_Control_Module.CC_Control_loop.Err >= 0 ) )
                {
                    //
                    // CC_Control_loop.Out, CC補償 >= 0, 正常CC是箝制所以不會 > 0
                    // CC_Control_loop.Err, ( Irms limit - Irms ) >= 0, 表示輸出Irms比limit還小
                    // 表示穩定離開CC
                    //

                    Output_Control_Module.Control_Mode_Stable_Count --;
                }
                else
                {
                    Output_Control_Module.Control_Mode_Stable_Count = 10;
                }


                if ( ( Output_Control_Module.Control_Mode_Stable_Count <= 0 )
                     || ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable == 0 )
                     || ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) )
                {
                    //
                    // 穩定離開CC, 或關閉I rms Enable, 或輸出OFF
                    //

                    Output_Control_Module.Control_Mode_Stable_Count = 10;

                    Output_Control_Module.DCV_Compensate_State = DCV_NO_COMPENSATE;
                    Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                    Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;
                    Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;

                    //
                    // 設定EXTIO
                    //
                    Global_Variables.EXT_OUTPUT_FLAG.bit.I_LIMITER = 0;

                    //
                    // 轉態
                    //
                    Output_Control_Module.Power_Control_Mode = CV_CONTROL_MODE;

                    break;
                }

                //
                // 執行CC補償控制
                //
                CC_Compensate_PID_Loop();

                break;
            }

            /*-----------------------------------------------------------------------------------------*/
            case CP_CONTROL_MODE:
            {

                if ( ( PID_Control_Module.CP_Control_loop.Out >= 0 ) && ( PID_Control_Module.CP_Control_loop.Err >= 0 ) )
                {
                    //
                    // CP_Control_loop.Out, CP補償 >= 0, 正常CP是箝制所以不會 > 0
                    // CP_Control_loop.Err, ( Power limit - VA ) >= 0, 表示輸出功率比limit還小
                    // 表示穩定離開CP
                    //

                    Output_Control_Module.Control_Mode_Stable_Count --;
                }
                else
                {
                    Output_Control_Module.Control_Mode_Stable_Count = 10;
                }

                if ( ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
                     || ( Output_Control_Module.Control_Mode_Stable_Count <= 0 ) )
                {
                    //
                    // 穩定離開CP, 或輸出OFF
                    //

                    Output_Control_Module.Control_Mode_Stable_Count = 10;

                    Output_Control_Module.DCV_Compensate_State = DCV_NO_COMPENSATE;
                    Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                    Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;
                    Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;

                    //
                    // 設定EXTIO
                    //
                    Global_Variables.EXT_OUTPUT_FLAG.bit.I_LIMITER = 0;

                    //
                    // 轉態
                    //
                    Output_Control_Module.Power_Control_Mode = CV_CONTROL_MODE;

                    break;
                }

                CP_Compensate_PID_Loop();

                break;
            }
        }
    }
    else
    {
        Output_Control_Module.DCV_Compensate_State = DCV_NO_COMPENSATE;
        Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

        Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;
        Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;

    }

#endif //BYPASS_CAL_CV_CC_CP_COMPENSATE


}



////////////////////////////////////////////////////////////////////
//
// CC補償控制 - PID Module
//
void CC_Compensate_PID_Loop(void)
{
    //
    // 處理DC的回授控制參數
    //

    PID_Control_Module.CC_Control_loop.Ref = (float)Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow * 0.0001;

    PID_Control_Module.CC_Control_loop.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;


    //
    // 補償狀態機
    //
    switch( Output_Control_Module.CC_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case CC_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.CC_Control_loop.Out = 0;
            PID_Control_Module.CC_Control_loop.OutPreSat = 0;
            PID_Control_Module.CC_Control_loop.SatErr = 0;
            PID_Control_Module.CC_Control_loop.Ud = 0;
            PID_Control_Module.CC_Control_loop.Ui = 0;
            PID_Control_Module.CC_Control_loop.Up = 0;
            PID_Control_Module.CC_Control_loop.Up1 = 0;

            //
            // Reset VREF補償量
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_3 = 0;

            //轉態
            Output_Control_Module.CC_Compensate_State = CC_PID_COMPENSATE;

            break;

        }
        /*-----------------------------------------------------------------------------------------*/
        case CC_PID_COMPENSATE:
        {

            //
            // DCV補償演算
            //
            CC_Control_Loop_Calc();


            //
            // Update到CLA模組去補償VREF DAC
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_3 = PID_Control_Module.CC_Control_loop.Out;


            break;
        }
    }
}



////////////////////////////////////////////////////////////////////
//
// CP補償控制 - PID Module
//
void CP_Compensate_PID_Loop(void)
{

    //
    // 設定PID控制器Fdb
    //
    //PID_Control_Module.CP_Control_loop.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.S_display_shadow;
    PID_Control_Module.CP_Control_loop.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all;

    //
    // 設定PID控制器Ref
    //
    PID_Control_Module.CP_Control_loop.Ref = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level;

    //
    // 補償狀態機
    //
    switch( Output_Control_Module.CP_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case CP_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.CP_Control_loop.Out = 0;
            PID_Control_Module.CP_Control_loop.OutPreSat = 0;
            PID_Control_Module.CP_Control_loop.SatErr = 0;
            PID_Control_Module.CP_Control_loop.Ud = 0;
            PID_Control_Module.CP_Control_loop.Ui = 0;
            PID_Control_Module.CP_Control_loop.Up = 0;
            PID_Control_Module.CP_Control_loop.Up1 = 0;

            //
            // Reset VREF補償量
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_3 = 0;

            //轉態
            Output_Control_Module.CP_Compensate_State = CP_PID_COMPENSATE;

            break;

        }
        /*-----------------------------------------------------------------------------------------*/
        case CP_PID_COMPENSATE:
        {

            //
            // DCV補償演算
            //
            CP_Control_Loop_Calc();


            //
            // Update到CLA模組去補償VREF DAC
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_3 = PID_Control_Module.CP_Control_loop.Out;


            break;
        }
    }
}













////////////////////////////////////////////////////////////////////
//
// CV補償控制
//
void CV_Compensate_Loop(void)
{
    //
    // 判斷output mode轉態
    //
    switch(Global_Variables.OUTPUT_MODE_FLAG_Shadow.all)
    {
        /*-----------------------------------------------------------------------------------------*/
        case DC_INT_MODE: //DC-INT
        {
            int32 change_level;

            //
            // 根據機種及檔位設定Level
            //
            if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                  && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == HIGH_RANGE ) )
            {
                change_level = DCV_COMPENSATE_LOOP_CHANGE_LEVEL_HR;
            }
            else
            {
                change_level = DCV_COMPENSATE_LOOP_CHANGE_LEVEL;
            }

            //
            // 低電壓補償WAVE, 高電壓補償VREF
            //
            if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow < change_level )
                 && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow > -change_level ) )
            {
                DCV_Compensate_PID_Loop2();
            }
            else
            {
                DCV_Compensate_PID_Loop1();
            }
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case AC_INT_MODE: //AC-INT
        case AC_SYNC_MODE: //AC-SYNC
        case AC_VCA_MODE: //AC-VCA
        {
            if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_SIN )
                 || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_TRI ) )
            {
                //ACDCV_Compensate_Loop();
                ACDCV_Compensate_PID_Loop();
            }
            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case ACDC_SYNC_MODE: //ACDC-SYNC
        case ACDC_INT_MODE: //ACDC-INT
        {
            if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 )
            {
                int32 change_level;

                //
                // 根據機種及檔位設定Level
                //
                if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                      && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == HIGH_RANGE ) )
                {
                    change_level = DCV_COMPENSATE_LOOP_CHANGE_LEVEL_HR;
                }
                else
                {
                    change_level = DCV_COMPENSATE_LOOP_CHANGE_LEVEL;
                }

                //
                // 低電壓補償WAVE, 高電壓補償VREF
                //
                if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow < change_level )
                     && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow > -change_level ) )
                {
                    DCV_Compensate_PID_Loop2();
                }
                else
                {
                    DCV_Compensate_PID_Loop1();
                }
            }
            else
            {
                if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_SIN )
                     || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_TRI ) )
                {
                    //ACDCV_Compensate_Loop();
                    ACDCV_Compensate_PID_Loop();
                }
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case MASTER_DC_INT_MODE:
        case SLAVE_DC_INT_MODE:
        {
            int32 change_level;

            //
            // 根據機種及檔位設定Level
            //
            if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                  && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == HIGH_RANGE ) )
            {
                change_level = DCV_COMPENSATE_LOOP_CHANGE_LEVEL_HR;
            }
            else
            {
                change_level = DCV_COMPENSATE_LOOP_CHANGE_LEVEL;
            }

            //
            // 低電壓補償WAVE, 高電壓補償VREF
            //
            if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow < change_level )
                 && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow > -change_level ) )
            {
                MS_DCV_Compensate_PID_Loop2();
            }
            else
            {
                MS_DCV_Compensate_PID_Loop1();
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case MASTER_AC_INT_MODE:
        case SLAVE_AC_INT_MODE:
        {
            if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_SIN )
                || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_TRI ) )
            {
                MS_ACDCV_Compensate_PID_Loop();
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case MASTER_ACDC_INT_MODE:
        case SLAVE_ACDC_INT_MODE:
        {
            if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 )
            {
                int32 change_level;

                //
                // 根據機種及檔位設定Level
                //
                if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                      && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == HIGH_RANGE ) )
                {
                    change_level = DCV_COMPENSATE_LOOP_CHANGE_LEVEL_HR;
                }
                else
                {
                    change_level = DCV_COMPENSATE_LOOP_CHANGE_LEVEL;
                }

                //
                // 低電壓補償WAVE, 高電壓補償VREF
                //
                if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow < change_level )
                     && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow > -change_level ) )
                {
                    MS_DCV_Compensate_PID_Loop2();
                }
                else
                {
                    MS_DCV_Compensate_PID_Loop1();
                }
            }
            else
            {
                if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_SIN )
                    || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_TRI ) )
                {
                    MS_ACDCV_Compensate_PID_Loop();
                }
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case AC_EXT_MODE: //AC-EXT
        case ACDC_EXT_MODE: //ACDC-EXT
        case AC_ADD_MODE: //AC-ADD
        case ACDC_ADD_MODE: //ACDC-ADD
        case SEQ_MODE: //SEQ
        default:
        {

            break;
        }
    }

    if ( ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
         && ( Global_Variables.POWER_STATUS.bit.MS_Master == 0 ) )
    {
        //
        // MS連線, Slave
        // 進行MS平衡補償控制
        //

        if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Balance_Compensate_Enable == 1 )
             && ( ( Output_Control_Module.MS_DCV_Compensate_State == DCV_STABLE_COMPENSATE )
                  || ( Output_Control_Module.MS_ACDCV_Compensate_State == ACDCV_ACDCV_STABLE_NO_COMPENSATE )
                  || ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == SLAVE_AC_INT_MODE )
                       && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user != WAVE_SIN )
                       && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user != WAVE_TRI ) )
                  || ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == SLAVE_ACDC_INT_MODE )
                       && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all != 0 )
                       && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user != WAVE_SIN )
                       && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user != WAVE_TRI ) ) ) )
        {
            //
            // 穩態均流補償控制
            //

            //MS_Balance_Compensate_Loop();
            MS_Balance_Compensate_PID_Loop();
        }
    }
    else
    {

        //
        // Reset VREF offset 1
        //
        CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

        //
        //轉態
        //
        Output_Control_Module.MS_Balance_Compensate_State = MS_BALANCE_NO_COMPENSATE;

    }


}




////////////////////////////////////////////////////////////////////
//
// DCV補償控制 - PID Module
// |DCV Set| >= 50V
//
void DCV_Compensate_PID_Loop1(void)
{
    //
    // 處理DC的回授控制參數
    //

    PID_Control_Module.DCV_Control_loop1.Ref = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow * 0.0001;

    PID_Control_Module.DCV_Control_loop1.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;


    //
    // 補償狀態機
    //
    switch( Output_Control_Module.DCV_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case DCV_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.DCV_Control_loop1.Out = 0;
            PID_Control_Module.DCV_Control_loop1.OutPreSat = 0;
            PID_Control_Module.DCV_Control_loop1.SatErr = 0;
            PID_Control_Module.DCV_Control_loop1.Ud = 0;
            PID_Control_Module.DCV_Control_loop1.Ui = 0;
            PID_Control_Module.DCV_Control_loop1.Up = 0;
            PID_Control_Module.DCV_Control_loop1.Up1 = 0;


            //
            // Reset WAVE及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;


            //
            // 重置Timer
            //
            Output_Control_Module.Compensate_Delay_Start_Time = CpuTimer0.InterruptCount;

            //
            // 轉態
            //
            Output_Control_Module.DCV_Compensate_State = DCV_DELAY_COMPENSATE;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_DELAY_COMPENSATE:
        {
            if ( Calc_Elapsed_Time( Output_Control_Module.Compensate_Delay_Start_Time, CpuTimer0.InterruptCount ) > COMPENSATE_DELAY_TIME )
            {
                //
                // 轉態
                //
                Output_Control_Module.DCV_Compensate_State = DCV_PID_COMPENSATE;

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_PID_COMPENSATE:
        {

            //
            // DCV補償演算
            //
            DCV_Control_Loop1_Calc();


            if ( PID_Control_Module.DCV_Control_loop1.Ref >= 0  )
            {
                //
                // DCV set >= 0
                //

                //
                // Update到CLA模組去補償VREF DAC
                //
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = PID_Control_Module.DCV_Control_loop1.Out;

            }
            else
            {
                //
                // DCV set < 0
                //

                //
                // Update到CLA模組去補償VREF DAC
                //
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = -PID_Control_Module.DCV_Control_loop1.Out;

            }


            break;
        }

    }

}


////////////////////////////////////////////////////////////////////
//
// DCV補償控制 - PID Module
// |DCV Set| < 50V
//
void DCV_Compensate_PID_Loop2(void)
{
    //
    // 處理DC的回授控制參數
    //

    PID_Control_Module.DCV_Control_loop2.Ref = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow * 0.0001;

    PID_Control_Module.DCV_Control_loop2.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;


    //
    // 補償狀態機
    //
    switch( Output_Control_Module.DCV_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case DCV_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.DCV_Control_loop2.Out = 0;
            PID_Control_Module.DCV_Control_loop2.OutPreSat = 0;
            PID_Control_Module.DCV_Control_loop2.SatErr = 0;
            PID_Control_Module.DCV_Control_loop2.Ud = 0;
            PID_Control_Module.DCV_Control_loop2.Ui = 0;
            PID_Control_Module.DCV_Control_loop2.Up = 0;
            PID_Control_Module.DCV_Control_loop2.Up1 = 0;

            //
            // Reset WAVE及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;

            //
            // 重置Timer
            //
            Output_Control_Module.Compensate_Delay_Start_Time = CpuTimer0.InterruptCount;

            //
            // 轉態
            //
            Output_Control_Module.DCV_Compensate_State = DCV_DELAY_COMPENSATE;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_DELAY_COMPENSATE:
        {
            if ( Calc_Elapsed_Time( Output_Control_Module.Compensate_Delay_Start_Time, CpuTimer0.InterruptCount ) > COMPENSATE_DELAY_TIME )
            {
                //
                // 轉態
                //
                Output_Control_Module.DCV_Compensate_State = DCV_PID_COMPENSATE;

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_PID_COMPENSATE:
        {

            //
            // DCV補償演算
            //
            DCV_Control_Loop2_Calc();

            //
            // 更新
            //
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset = -PID_Control_Module.DCV_Control_loop2.Out;



            break;
        }

    }

}





////////////////////////////////////////////////////////////////////
//
// ACDCV補償控制 - PID Module
//
void ACDCV_Compensate_PID_Loop(void)
{

    //
    // 處理DC的回授控制參數
    //
    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 0 )
    {
        PID_Control_Module.DCV_Control_loop2.Ref = 0;
    }
    else
    {
        PID_Control_Module.DCV_Control_loop2.Ref = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow * 0.0001;

    }
    PID_Control_Module.DCV_Control_loop2.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;

    //
    // 處理AC的回授控制參數
    //
    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_VCA_MODE )
    {
        //
        // VCA Mode
        //
        PID_Control_Module.ACV_Control_loop.Ref = Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set;
    }
    else
    {
        //
        // 其他 Mode
        //
        PID_Control_Module.ACV_Control_loop.Ref = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow * 0.0001;
    }


    //
    // 如果ACV單位為Vpp且波型為TRI
    // 則目前的acv_ref是對應ACV(peak-peak), 要轉換成rms
    //
    if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit == 1 ) && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_TRI ) )
    {
        PID_Control_Module.ACV_Control_loop.Ref = PID_Control_Module.ACV_Control_loop.Ref * 0.5 * Global_Variables.Pre_Calc_Variables.inv_Wave_TRI_CF_set_user;
    }

    PID_Control_Module.ACV_Control_loop.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display_shadow;


    //
    // 補償狀態機
    //
    switch( Output_Control_Module.ACDCV_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.ACV_Control_loop.Out = 0;
            PID_Control_Module.ACV_Control_loop.OutPreSat = 0;
            PID_Control_Module.ACV_Control_loop.SatErr = 0;
            PID_Control_Module.ACV_Control_loop.Ud = 0;
            PID_Control_Module.ACV_Control_loop.Ui = 0;
            PID_Control_Module.ACV_Control_loop.Up = 0;
            PID_Control_Module.ACV_Control_loop.Up1 = 0;

            PID_Control_Module.DCV_Control_loop2.Out = 0;
            PID_Control_Module.DCV_Control_loop2.OutPreSat = 0;
            PID_Control_Module.DCV_Control_loop2.SatErr = 0;
            PID_Control_Module.DCV_Control_loop2.Ud = 0;
            PID_Control_Module.DCV_Control_loop2.Ui = 0;
            PID_Control_Module.DCV_Control_loop2.Up = 0;
            PID_Control_Module.DCV_Control_loop2.Up1 = 0;

            //
            // Reset WAVE及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;

            //
            // Reset 穩定判斷Counter
            //
            Output_Control_Module.Compensate_Control_Stable_Count = 10;

            //
            // 重置Timer
            //
            Output_Control_Module.Compensate_Delay_Start_Time = CpuTimer0.InterruptCount;

            //
            // 轉態
            //
            Output_Control_Module.ACDCV_Compensate_State = ACDCV_DELAY_COMPENSATE;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_DELAY_COMPENSATE:
        {
            if ( Calc_Elapsed_Time( Output_Control_Module.Compensate_Delay_Start_Time, CpuTimer0.InterruptCount ) > COMPENSATE_DELAY_TIME )
            {
                //
                // 轉態
                //
                Output_Control_Module.ACDCV_Compensate_State = ACDCV_DCV_PID_COMPENSATE;

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_DCV_PID_COMPENSATE:
        {
            float stable_level;

            //
            // DCV補償演算
            //
            DCV_Control_Loop2_Calc();


            //
            // 更新
            //
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset = -PID_Control_Module.DCV_Control_loop2.Out;

            //
            // 穩定判斷
            //
            //if ( fabs( PID_Control_Module.DCV_Control_loop2.Err ) < 1.5 )

            //
            // 20230711
            //
            // 根據DC輸出精度規格 => ( | 0.5% of set| + 0.5V/1.0V )
            //
            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5075 )
            {
                stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 0.5;
            }
            else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                      && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == LOW_RANGE ) )
            {
                stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 0.5;
            }
            else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                      && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == HIGH_RANGE ) )
            {
                stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 1.0;
            }

            //
            // 20230711
            // 5150機器實測會穩不下來, 所以改用V_avg_display.all來判斷
            //
            if ( fabs( PID_Control_Module.DCV_Control_loop2.Ref - Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all ) < stable_level )
            {
                Output_Control_Module.Compensate_Control_Stable_Count --;
            }
            else
            {
                Output_Control_Module.Compensate_Control_Stable_Count = 10;
            }


            //
            // 判斷穩定計數器再轉態
            //
            if ( Output_Control_Module.Compensate_Control_Stable_Count == 0 )
            {
                Output_Control_Module.Compensate_Control_Stable_Count = 10;

                //
                // 轉態
                //
                Output_Control_Module.ACDCV_Compensate_State = ACDCV_DCV_STABLE_COMPENSATE;
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_DCV_STABLE_COMPENSATE:
        {
            //
            // DCV補償演算
            //
            DCV_Control_Loop2_Calc();


            //
            // 更新
            //
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset = -PID_Control_Module.DCV_Control_loop2.Out;


            //
            // 判斷誤差是否太大
            //
            if ( fabs( PID_Control_Module.DCV_Control_loop2.Err ) > ( fabs( PID_Control_Module.DCV_Control_loop2.Ref ) * 0.05 + 15.0 ) )
            {
                //轉態
                Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                break;
            }

            //
            // 判斷是否有ACV設定值
            // 如果ACV設定值不等於0則直接轉態開始補償ACV
            //
            if ( PID_Control_Module.ACV_Control_loop.Ref != 0 )
            {

                //轉態
                Output_Control_Module.ACDCV_Compensate_State = ACDCV_ACV_PID_COMPENSATE;

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_ACV_PID_COMPENSATE:
        {
            //
            // DCV補償演算
            //
            DCV_Control_Loop2_Calc();


            //
            // 更新
            //
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset = -PID_Control_Module.DCV_Control_loop2.Out;

            //
            // ACV補償演算
            //
            ACV_Control_Loop_Calc();


            //
            // Update到CLA模組去補償VREF DAC
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = PID_Control_Module.ACV_Control_loop.Out;

            //
            // 判斷誤差是否太大
            //
            if ( fabs( PID_Control_Module.ACV_Control_loop.Err ) > ( fabs( PID_Control_Module.ACV_Control_loop.Ref ) * 0.05 + 15.0 ) )
            {
                //轉態
                Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                break;
            }


            break;
        }


    }
}







////////////////////////////////////////////////////////////////////
//
// MS DCV補償控制 - PID Module
// |DCV Set| >= 50V
//
void MS_DCV_Compensate_PID_Loop1(void)
{
    //
    // 處理DC的回授控制參數
    //

    PID_Control_Module.DCV_Control_loop1.Ref = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow * 0.0001;

    PID_Control_Module.DCV_Control_loop1.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;


    //
    // 補償狀態機
    //
    switch( Output_Control_Module.MS_DCV_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case DCV_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.DCV_Control_loop1.Out = 0;
            PID_Control_Module.DCV_Control_loop1.OutPreSat = 0;
            PID_Control_Module.DCV_Control_loop1.SatErr = 0;
            PID_Control_Module.DCV_Control_loop1.Ud = 0;
            PID_Control_Module.DCV_Control_loop1.Ui = 0;
            PID_Control_Module.DCV_Control_loop1.Up = 0;
            PID_Control_Module.DCV_Control_loop1.Up1 = 0;


            //
            // Reset WAVE及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;


            Output_Control_Module.Compensate_Control_Stable_Count = 10;

            //
            // 重置Timer
            //
            Output_Control_Module.Compensate_Delay_Start_Time = CpuTimer0.InterruptCount;

            //
            // 轉態
            //
            //Output_Control_Module.MS_DCV_Compensate_State = DCV_DELAY_COMPENSATE;
            Output_Control_Module.MS_DCV_Compensate_State = DCV_PID_COMPENSATE;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_DELAY_COMPENSATE:
        {
            if ( Calc_Elapsed_Time( Output_Control_Module.Compensate_Delay_Start_Time, CpuTimer0.InterruptCount ) > COMPENSATE_DELAY_TIME )
            {
                //
                // 轉態
                //
                Output_Control_Module.MS_DCV_Compensate_State = DCV_PID_COMPENSATE;

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_PID_COMPENSATE:
        {
            float stable_level;

            //
            // DCV補償演算
            //
            DCV_Control_Loop1_Calc();


            if ( PID_Control_Module.DCV_Control_loop1.Ref >= 0  )
            {
                //
                // DCV set >= 0
                //

                //
                // Update到CLA模組去補償VREF DAC
                //
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = PID_Control_Module.DCV_Control_loop1.Out;

            }
            else
            {
                //
                // DCV set < 0
                //

                //
                // Update到CLA模組去補償VREF DAC
                //
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = -PID_Control_Module.DCV_Control_loop1.Out;

            }


            //
            // MS連線
            // 穩定後轉態
            //
            //if ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
            if ( ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
                 && ( Global_Variables.POWER_STATUS.bit.MS_Master == 0 ) )
            {
                //if ( fabs( PID_Control_Module.DCV_Control_loop1.Err ) < 0.05 )
                //if ( fabs( PID_Control_Module.DCV_Control_loop1.Err ) < 0.5 )


                //
                // 20230711
                //
                // 根據DC輸出精度規格 => ( | 0.5% of set| + 0.5V/1.0V )
                //
                if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5075 )
                {
                    stable_level = PID_Control_Module.DCV_Control_loop1.Ref * 0.005 + 0.5;
                }
                else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                          && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == LOW_RANGE ) )
                {
                    stable_level = PID_Control_Module.DCV_Control_loop1.Ref * 0.005 + 0.5;
                }
                else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                          && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == HIGH_RANGE ) )
                {
                    stable_level = PID_Control_Module.DCV_Control_loop1.Ref * 0.005 + 1.0;
                }

                //
                // 20230711
                // 5150機器實測會穩不下來, 所以改用V_avg_display.all來判斷
                //
                if ( fabs( PID_Control_Module.DCV_Control_loop1.Ref - Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all ) < stable_level )
                {
                    Output_Control_Module.Compensate_Control_Stable_Count --;
                }
                else
                {
                    Output_Control_Module.Compensate_Control_Stable_Count = 10;
                }

                //
                // 判斷穩定計數器再轉態
                //
                if ( Output_Control_Module.Compensate_Control_Stable_Count == 0 )
                {
                    Output_Control_Module.Compensate_Control_Stable_Count = 10;

                    //
                    // 轉態
                    //
                    Output_Control_Module.MS_DCV_Compensate_State = DCV_STABLE_COMPENSATE;
                    break;
                }
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_STABLE_COMPENSATE:
        {
            //
            // MS模式下
            // 穩定下由MS來補償
            //

            //
            // 判斷誤差是否太大
            //
            if ( fabs( PID_Control_Module.DCV_Control_loop1.Err ) > ( fabs( PID_Control_Module.DCV_Control_loop1.Ref ) * 0.05 + 5.0 ) )
            {
                //轉態
                Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;

                break;
            }


            break;
        }

    }

}



////////////////////////////////////////////////////////////////////
//
// MS DCV補償控制 - PID Module
// |DCV Set| < 50V
//
void MS_DCV_Compensate_PID_Loop2(void)
{
    //
    // 處理DC的回授控制參數
    //

    PID_Control_Module.DCV_Control_loop2.Ref = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow * 0.0001;

    PID_Control_Module.DCV_Control_loop2.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;


    //
    // 補償狀態機
    //
    switch( Output_Control_Module.MS_DCV_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case DCV_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.DCV_Control_loop2.Out = 0;
            PID_Control_Module.DCV_Control_loop2.OutPreSat = 0;
            PID_Control_Module.DCV_Control_loop2.SatErr = 0;
            PID_Control_Module.DCV_Control_loop2.Ud = 0;
            PID_Control_Module.DCV_Control_loop2.Ui = 0;
            PID_Control_Module.DCV_Control_loop2.Up = 0;
            PID_Control_Module.DCV_Control_loop2.Up1 = 0;

            //
            // Reset WAVE及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;

            Output_Control_Module.Compensate_Control_Stable_Count = 10;

            //
            // 重置Timer
            //
            Output_Control_Module.Compensate_Delay_Start_Time = CpuTimer0.InterruptCount;

            //
            // 轉態
            //
            //Output_Control_Module.MS_DCV_Compensate_State = DCV_DELAY_COMPENSATE;
            Output_Control_Module.MS_DCV_Compensate_State = DCV_PID_COMPENSATE;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_DELAY_COMPENSATE:
        {
            if ( Calc_Elapsed_Time( Output_Control_Module.Compensate_Delay_Start_Time, CpuTimer0.InterruptCount ) > COMPENSATE_DELAY_TIME )
            {
                //
                // 轉態
                //
                Output_Control_Module.MS_DCV_Compensate_State = DCV_PID_COMPENSATE;

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_PID_COMPENSATE:
        {
            float stable_level;

            //
            // DCV補償演算
            //
            DCV_Control_Loop2_Calc();

            //
            // 更新
            //
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset = -PID_Control_Module.DCV_Control_loop2.Out;


            //
            // MS連線
            // 穩定後轉態
            //
            //if ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
            if ( ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
                 && ( Global_Variables.POWER_STATUS.bit.MS_Master == 0 ) )
            {
                //if ( fabs( PID_Control_Module.DCV_Control_loop1.Err ) < 0.05 )
                //if ( fabs( PID_Control_Module.DCV_Control_loop1.Err ) < 0.5 )

                //
                // 20230711
                //
                // 根據DC輸出精度規格 => ( | 0.5% of set| + 0.5V/1.0V )
                //
                if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5075 )
                {
                    stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 0.5;
                }
                else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                          && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == LOW_RANGE ) )
                {
                    stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 0.5;
                }
                else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                          && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == HIGH_RANGE ) )
                {
                    stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 1.0;
                }

                //
                // 20230711
                // 5150機器實測會穩不下來, 所以改用V_avg_display.all來判斷
                //
                if ( fabs( PID_Control_Module.DCV_Control_loop2.Ref - Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all ) < stable_level )
                {
                    Output_Control_Module.Compensate_Control_Stable_Count --;
                }
                else
                {
                    Output_Control_Module.Compensate_Control_Stable_Count = 10;
                }

                //
                // 判斷穩定計數器再轉態
                //
                if ( Output_Control_Module.Compensate_Control_Stable_Count == 0 )
                {
                    Output_Control_Module.Compensate_Control_Stable_Count = 10;

                    //
                    // 轉態
                    //
                    Output_Control_Module.MS_DCV_Compensate_State = DCV_STABLE_COMPENSATE;
                    break;
                }
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case DCV_STABLE_COMPENSATE:
        {
            //
            // MS模式下
            // 穩定下由MS來補償
            //

            //
            // 判斷誤差是否太大
            //
            if ( fabs( PID_Control_Module.DCV_Control_loop2.Err ) > ( fabs( PID_Control_Module.DCV_Control_loop2.Ref ) * 0.05 + 5.0 ) )
            {
                //轉態
                Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;

                break;
            }


            break;
        }

    }


}












////////////////////////////////////////////////////////////////////
//
// MS ACDCV補償控制 - PID Module
//
void MS_ACDCV_Compensate_PID_Loop(void)
{
    //
    // 處理DC的回授控制參數
    //
    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 0 )
    {
        PID_Control_Module.DCV_Control_loop2.Ref = 0;
    }
    else
    {
        PID_Control_Module.DCV_Control_loop2.Ref = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow * 0.0001;

    }
    PID_Control_Module.DCV_Control_loop2.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;


    //
    // 處理AC的回授控制參數
    //

    PID_Control_Module.ACV_Control_loop.Ref = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow * 0.0001;

    //
    // 如果ACV單位為Vpp且波型為TRI
    // 則目前的acv_ref是對應ACV(peak-peak), 要轉換成rms
    //
    if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit == 1 ) && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_TRI ) )
    {
        PID_Control_Module.ACV_Control_loop.Ref = PID_Control_Module.ACV_Control_loop.Ref * 0.5 * Global_Variables.Pre_Calc_Variables.inv_Wave_TRI_CF_set_user;
    }

    PID_Control_Module.ACV_Control_loop.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display_shadow;


    //
    // 補償狀態機
    //
    switch( Output_Control_Module.MS_ACDCV_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.ACV_Control_loop.Out = 0;
            PID_Control_Module.ACV_Control_loop.OutPreSat = 0;
            PID_Control_Module.ACV_Control_loop.SatErr = 0;
            PID_Control_Module.ACV_Control_loop.Ud = 0;
            PID_Control_Module.ACV_Control_loop.Ui = 0;
            PID_Control_Module.ACV_Control_loop.Up = 0;
            PID_Control_Module.ACV_Control_loop.Up1 = 0;

            PID_Control_Module.DCV_Control_loop2.Out = 0;
            PID_Control_Module.DCV_Control_loop2.OutPreSat = 0;
            PID_Control_Module.DCV_Control_loop2.SatErr = 0;
            PID_Control_Module.DCV_Control_loop2.Ud = 0;
            PID_Control_Module.DCV_Control_loop2.Ui = 0;
            PID_Control_Module.DCV_Control_loop2.Up = 0;
            PID_Control_Module.DCV_Control_loop2.Up1 = 0;

            //
            // Reset WAVE及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;

            //
            // Reset 穩定判斷Counter
            //
            Output_Control_Module.Compensate_Control_Stable_Count = 10;

            //
            // 重置Timer
            //
            Output_Control_Module.Compensate_Delay_Start_Time = CpuTimer0.InterruptCount;

            //
            // 轉態
            //
            //Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_DELAY_COMPENSATE;
            Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_DCV_PID_COMPENSATE;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_DELAY_COMPENSATE:
        {
            if ( Calc_Elapsed_Time( Output_Control_Module.Compensate_Delay_Start_Time, CpuTimer0.InterruptCount ) > COMPENSATE_DELAY_TIME )
            {
                //
                // 轉態
                //
                Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_DCV_PID_COMPENSATE;

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_DCV_PID_COMPENSATE:
        {
            float stable_level;

            //
            // DCV補償演算
            //
            DCV_Control_Loop2_Calc();


            //
            // 更新
            //
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset = -PID_Control_Module.DCV_Control_loop2.Out;

            //
            // 穩定判斷
            //
            //if ( fabs( PID_Control_Module.DCV_Control_loop2.Err ) < 1.5 )


            //
            // 20230711
            //
            // 根據DC輸出精度規格 => ( | 0.5% of set| + 0.5V/1.0V )
            //
            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5075 )
            {
                stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 0.5;
            }
            else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                      && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == LOW_RANGE ) )
            {
                stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 0.5;
            }
            else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                      && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range == HIGH_RANGE ) )
            {
                stable_level = PID_Control_Module.DCV_Control_loop2.Ref * 0.005 + 1.0;
            }

            //
            // 20230711
            // 5150機器實測會穩不下來, 所以改用V_avg_display.all來判斷
            //
            if ( fabs( PID_Control_Module.DCV_Control_loop2.Ref - Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all ) < stable_level )
            {
                Output_Control_Module.Compensate_Control_Stable_Count --;
            }
            else
            {
                Output_Control_Module.Compensate_Control_Stable_Count = 10;
            }


            //
            // 判斷穩定計數器再轉態
            //
            if ( Output_Control_Module.Compensate_Control_Stable_Count == 0 )
            {
                Output_Control_Module.Compensate_Control_Stable_Count = 10;

                //
                // 轉態
                //
                Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_DCV_STABLE_COMPENSATE;
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_DCV_STABLE_COMPENSATE:
        {
            //
            // DCV補償演算
            //
            DCV_Control_Loop2_Calc();


            //
            // 更新
            //
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset = -PID_Control_Module.DCV_Control_loop2.Out;


            //
            // 判斷誤差是否太大
            //
            if ( fabs( PID_Control_Module.DCV_Control_loop2.Err ) > ( fabs( PID_Control_Module.DCV_Control_loop2.Ref ) * 0.05 + 15.0 ) )
            {
                //轉態
                Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                break;
            }

            //
            // 判斷是否有ACV設定值
            // 如果ACV設定值不等於0則直接轉態開始補償ACV
            //
            if ( PID_Control_Module.ACV_Control_loop.Ref != 0 )
            {

                //轉態
                Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_ACV_PID_COMPENSATE;

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_ACV_PID_COMPENSATE:
        {
            //
            // DCV補償演算
            //
            DCV_Control_Loop2_Calc();


            //
            // 更新
            //
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset = -PID_Control_Module.DCV_Control_loop2.Out;

            //
            // ACV補償演算
            //
            ACV_Control_Loop_Calc();


            //
            // Update到CLA模組去補償VREF DAC
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = PID_Control_Module.ACV_Control_loop.Out;

            //
            // 判斷誤差是否太大
            //
            if ( fabs( PID_Control_Module.ACV_Control_loop.Err ) > ( fabs( PID_Control_Module.ACV_Control_loop.Ref ) * 0.05 + 15.0 ) )
            {
                //轉態
                Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                break;
            }


            //
            // MS連線, 穩定後轉態
            //
            if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Balance_Compensate_Enable == 1 )
                 && ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE ) )
            {
                //if ( fabs( PID_Control_Module.ACV_Control_loop.Err ) < 0.02 )
                if ( fabs( PID_Control_Module.ACV_Control_loop.Err ) < 0.1 )
                {
                    Output_Control_Module.Compensate_Control_Stable_Count --;
                }
                else
                {
                    Output_Control_Module.Compensate_Control_Stable_Count = 10;
                }

                //
                // 判斷穩定計數器再轉態
                //
                if ( Output_Control_Module.Compensate_Control_Stable_Count == 0 )
                {
                    Output_Control_Module.Compensate_Control_Stable_Count = 10;

                    //
                    // 轉態
                    //
                    Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_ACDCV_STABLE_NO_COMPENSATE;

                    break;
                }
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ACDCV_ACDCV_STABLE_NO_COMPENSATE:
        {
            //
            // 判斷誤差是否太大
            //
            if ( fabs( PID_Control_Module.ACV_Control_loop.Err ) > ( fabs( PID_Control_Module.ACV_Control_loop.Ref ) * 0.05 + 15.0 ) )
            {
                //轉態
                Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                break;
            }

            break;
        }
    }
}


////////////////////////////////////////////////////////////////////
//
// MS模式下平衡控制-PID
//
void MS_Balance_Compensate_PID_Loop(void)
{
    //
    // 處理回授控制參數
    //
    PID_Control_Module.MS_Balance_Compensate_loop.Ref = Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES_Total.P_display_shadow.all;
    PID_Control_Module.MS_Balance_Compensate_loop.Fdb = Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow.all;


    //
    // 補償狀態機
    //
    switch( Output_Control_Module.MS_Balance_Compensate_State )
    {
        /*-----------------------------------------------------------------------------------------*/
        case MS_BALANCE_NO_COMPENSATE:
        {
            //
            // Init
            //
            PID_Control_Module.MS_Balance_Compensate_loop.Out = 0;
            PID_Control_Module.MS_Balance_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.MS_Balance_Compensate_loop.SatErr = 0;
            PID_Control_Module.MS_Balance_Compensate_loop.Ud = 0;
            PID_Control_Module.MS_Balance_Compensate_loop.Ui = 0;
            PID_Control_Module.MS_Balance_Compensate_loop.Up = 0;
            PID_Control_Module.MS_Balance_Compensate_loop.Up1 = 0;

            //
            // Update到CLA模組去補償VREF DAC
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

            //
            // 轉態
            //
            Output_Control_Module.MS_Balance_Compensate_State = MS_BALANCE_PID_COMPENSATE;

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case MS_BALANCE_PID_COMPENSATE:
        {

            MS_Balance_Compensate_loop_Calc();

            //
            // Update到CLA模組去補償VREF DAC
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = PID_Control_Module.MS_Balance_Compensate_loop.Out;

            break;
        }
    }
}





////////////////////////////////////////////////////////////////////
//
//
//
/*
void EXT_VCA_Calc_Process(void)
{
    float float_buf, scale, offset;


    scale = Global_Variables.Calibration_Variables.inv_ADC_to_EXTIN_L1_Scale;
    offset = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
    float_buf = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN;

    Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set = ( float_buf - offset ) * scale;

}
*/


////////////////////////////////////////////////////////////////////
//
//
//
void EXT_Freq_Calc_Process(void)
{

    Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all = EXTAD_SAMPLE_FREQ / cpu_timer_module.EXTAD_TTL_Period;

    Global_Variables.POWER_DISPLAY_VARIABLES.FREQ_display.all = Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all;

    if ( Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all < 1.0 )
    {
        Global_Variables.FreqStep_shadow = (Uint32)_1HZ;

    }
    else
    {
        Global_Variables.FreqStep_shadow = (Uint32)( Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all * _1HZ );
    }

    Global_Variables.FreqStep = Global_Variables.FreqStep_shadow;




    asm("       NOP");                    // Wait one cycle


}



////////////////////////////////////////////////////////////////////
void Input_Variables_Update(void)
{

    Global_Variables.INPUT_VARIABLES.V_FB = CLA_to_CPU_Variables.V_FB;
    Global_Variables.INPUT_VARIABLES.I_FB = CLA_to_CPU_Variables.I_FB;


}




////////////////////////////////////////////////////////////////////
void Output_Flags_Update(void)
{
    Handle_DCAC_ON_OCP_Delay_Process();

    CPU_to_CLA_Variables.OUTPUT_FLAGS_all = Global_Variables.OUTPUT_FLAGS.all;
    Global_Variables.POWER_STATUS.bit.PWM_Enable = Global_Variables.OUTPUT_FLAGS.bit.DCAC_ENA;


    GpioDataRegs.GPASET.bit.GPIO4 = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.VMONI_LO;
    GpioDataRegs.GPACLEAR.bit.GPIO4 = !Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.VMONI_LO;

    GpioDataRegs.GPASET.bit.GPIO6 = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.IMONI_LO;
    GpioDataRegs.GPACLEAR.bit.GPIO6 = !Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.IMONI_LO;

    //Global_Variables.POWER_STATUS.bit.Output_ON = Global_Variables.OUTPUT_FLAGS.bit.OUT_ON;
    Global_Variables.POWER_STATUS.bit.V_Range = Global_Variables.OUTPUT_FLAGS.bit.VM_HI_LO;
    Global_Variables.POWER_STATUS.bit.I_Range = Global_Variables.OUTPUT_FLAGS.bit.IM_HI_LO;

    Global_Variables.POWER_STATUS.bit.Control_Mode = Output_Control_Module.Power_Control_Mode;

}



////////////////////////////////////////////////////////////////////
void Handle_DCAC_ON_OCP_Delay_Process(void)
{
    switch ( Output_Control_Module.OCP_Bypass_Control_State )
    {
        case OCP_CHECK_NORMAL:
        {
            if ( ( Global_Variables.OUTPUT_FLAGS.bit.DCAC_ENA == 1 ) && ( Output_Control_Module.DCAC_ENA_shadow == 0 ) )
            {
                Output_Control_Module.Flag.bit.Bypass_OCP_Flag = 1;

                CPU_to_CLA_Variables.Bypass_OCP_Flag = Output_Control_Module.Flag.bit.Bypass_OCP_Flag;

                Output_Control_Module.OCP_Delay_start_time = CpuTimer2.InterruptCount;

                // 轉態
                Output_Control_Module.OCP_Bypass_Control_State = OCP_CHECK_DELAY;
            }

            Output_Control_Module.DCAC_ENA_shadow = Global_Variables.OUTPUT_FLAGS.bit.DCAC_ENA;

            break;
        }
        case OCP_CHECK_DELAY:
        {
            if ( Calc_Elapsed_Time( Output_Control_Module.OCP_Delay_start_time, CpuTimer2.InterruptCount ) > OCP_BYPASS_DELAY_TIME )
            {
                Output_Control_Module.Flag.bit.Bypass_OCP_Flag = 0;

                CPU_to_CLA_Variables.Bypass_OCP_Flag = Output_Control_Module.Flag.bit.Bypass_OCP_Flag;


                // 轉態
                Output_Control_Module.OCP_Bypass_Control_State = OCP_CHECK_NORMAL;

            }

            break;
        }
    }


}



//
// End of file
//
