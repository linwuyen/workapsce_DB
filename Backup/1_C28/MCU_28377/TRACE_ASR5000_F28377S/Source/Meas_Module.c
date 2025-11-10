//###########################################################################
//
// FILE:    Meas_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211116
//###########################################################################



#include "Global_VariableDefs.h"

volatile MEAS_MODULE_VARIABLES_REG Meas_Module;



/////////////////////////////////////////////////////////////////////
//
// 變數初始化
//
void Init_Meas_Module_Variables(void)
{
    Meas_Module.Flags.all = 0;
    Meas_Module.Meas_sampling_state = meas_sampling_ready_state;
    Meas_Module.Sampling_index = 0;
    Meas_Module.Total_Sample_Points = 0;
    Meas_Module.V_square_sum = 0;
    Meas_Module.I_square_sum = 0;
    Meas_Module.V_sum = 0;
    Meas_Module.I_sum = 0;

    Meas_Module.MoveAvg_Total_Count = 1;
    Meas_Module.MoveAvg_Count = 1;

    Meas_Module.MoveAvg_V_rms = 0;
    Meas_Module.MoveAvg_V_rms_sum = 0;
    Meas_Module.MoveAvg_V_avg = 0;
    Meas_Module.MoveAvg_V_avg_sum = 0;
    Meas_Module.MoveAvg_p_V_peak = 0;
    Meas_Module.MoveAvg_p_V_peak_sum = 0;
    Meas_Module.MoveAvg_n_V_peak = 0;
    Meas_Module.MoveAvg_n_V_peak_sum = 0;
    Meas_Module.MoveAvg_AC_V_rms = 0;
    Meas_Module.MoveAvg_AC_V_rms_sum = 0;

    Meas_Module.MoveAvg_I_rms = 0;
    Meas_Module.MoveAvg_I_rms_sum = 0;
    Meas_Module.MoveAvg_I_avg = 0;
    Meas_Module.MoveAvg_I_avg_sum = 0;
    Meas_Module.MoveAvg_p_I_peak = 0;
    Meas_Module.MoveAvg_p_I_peak_sum = 0;
    Meas_Module.MoveAvg_n_I_peak = 0;
    Meas_Module.MoveAvg_n_I_peak_sum = 0;

    Meas_Module.MoveAvg_P = 0;
    Meas_Module.MoveAvg_P_sum = 0;

    Meas_Module.Meas_Period = 0;

}


/////////////////////////////////////////////////////////////////////
//
// 量測變數處理流程
//
void Measurement_Calc_Process(void)
{
    double double_buf, double_buf2;
    float float_buf;
    float abs_p_Ipeak, abs_n_Ipeak;

    float offset;

    float total_rms, total_avg;


    //////////////////////////////////////////////////////////////////////////////////////////////
    //電壓V_FB區

    //
    // 計算V rms
    //
    //offset = Global_Variables.Calibration_Variables.V_rms_Offset.all;
    double_buf = CLA_to_CPU_Variables.V_square_sum / (double)CLA_to_CPU_Variables.Total_Sample_Points; //平均
    Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow = (float)sqrt(double_buf);

    //Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow;
    /*
    if ( Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow <= fabs( Global_Variables.Calibration_Variables.V_rms_Offset.all ) )
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = 0;
    }
    else
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow;
    }
    */


    //
    // 計算V avg
    //
    double_buf = CLA_to_CPU_Variables.V_sum / (double)CLA_to_CPU_Variables.Total_Sample_Points; //平均
    Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow = (float)double_buf;
    //Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;


    //
    // 計算AC V rms
    //
    total_rms = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow;
    total_avg = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;
    Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display_shadow = (float)sqrt( total_rms * total_rms - total_avg * total_avg );



    // CHA avg
    double_buf = CLA_to_CPU_Variables.ADS8353_CHA_sum / (double)CLA_to_CPU_Variables.Total_Sample_Points;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_avg_display_shadow = (float)double_buf;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_avg_display.all = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_avg_display_shadow;


    //
    // +Vpk 處理
    //
    if ( CLA_to_CPU_Variables.p_V_peak > Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display_shadow )
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display_shadow = CLA_to_CPU_Variables.p_V_peak;

    }


    //
    // -Vpk 處理
    //
    if ( CLA_to_CPU_Variables.n_V_peak < Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display_shadow )
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display_shadow = CLA_to_CPU_Variables.n_V_peak;

    }


    //
    // V peak hold
    //
    Global_Variables.POWER_DISPLAY_VARIABLES.V_peak_hold_display.all = CLA_to_CPU_Variables.VPK_Hold;

    //////////////////////////////////////////////////////////////////////////////////////////////
    //電流I_FB區

    //
    // 計算I rms
    //
    //offset = Global_Variables.Calibration_Variables.I_rms_Offset.all;
    double_buf = CLA_to_CPU_Variables.I_square_sum / (double)CLA_to_CPU_Variables.Total_Sample_Points; //平均
    Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow = (float)sqrt(double_buf);

    //Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
    /*
    if ( Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow <= fabs( Global_Variables.Calibration_Variables.I_rms_Offset.all ) )
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = 0;
    }
    else
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
    }
    */



    //
    // 計算I avg
    //
    double_buf = CLA_to_CPU_Variables.I_sum / (double)CLA_to_CPU_Variables.Total_Sample_Points; //平均
    Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display_shadow = (float)double_buf;
    //Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display_shadow;

    //
    // CHB avg
    double_buf = CLA_to_CPU_Variables.ADS8353_CHB_sum / (double)CLA_to_CPU_Variables.Total_Sample_Points;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_avg_display_shadow = (float)double_buf;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_avg_display.all = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_avg_display_shadow;

    //
    // CHB avg
    double_buf = CLA_to_CPU_Variables.ADS8353_CHB_minus_offset_squ_sum / (double)CLA_to_CPU_Variables.Total_Sample_Points;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display_shadow = (float)sqrt(double_buf);
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display.all = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display_shadow;

    //
    // +Ipk 處理
    //
    if ( CLA_to_CPU_Variables.p_I_peak > Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display_shadow )
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display_shadow = CLA_to_CPU_Variables.p_I_peak;

    }


    //
    // -Ipk 處理
    //
    if ( CLA_to_CPU_Variables.n_I_peak < Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display_shadow )
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display_shadow = CLA_to_CPU_Variables.n_I_peak;

    }



    //
    // I peak hold
    //
    Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.all = CLA_to_CPU_Variables.IPK_Hold;


    //////////////////////////////////////////////////////////////////////////////////////////////

    // 計算P
    //
    Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow.all = (float)( CLA_to_CPU_Variables.P_sum / (double)CLA_to_CPU_Variables.Total_Sample_Points ); //平均
    //Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow;


    // 計算S shadow
    //
    if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Remote_Sense_Enable == 1 )
         && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == DC_INT_MODE ) )
    {
        //
        // Remote Sense啟用時, DC-INT
        //

        float dcv_set;

        dcv_set = fabs( (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow * 0.0001 );

        Global_Variables.POWER_DISPLAY_VARIABLES.S_display_shadow = dcv_set * Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
    }
    else if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Remote_Sense_Enable == 1 )
              && ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_INT_MODE )
                   || ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_SYNC_MODE ) ) )
    {
        //
        // Remote Sense啟用時, AC-INT, AC-SYNC
        //

        float acv_set;

        acv_set = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow * 0.0001;

        Global_Variables.POWER_DISPLAY_VARIABLES.S_display_shadow = acv_set * Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
    }
    else if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Remote_Sense_Enable == 1 )
              && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_VCA_MODE ) )
    {
        //
        // Remote Sense啟用時, AC-VCA
        //

        float acv_set;

        acv_set = Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set;

        Global_Variables.POWER_DISPLAY_VARIABLES.S_display_shadow = acv_set * Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
    }
    else
    {
        Global_Variables.POWER_DISPLAY_VARIABLES.S_display_shadow = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow * Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////
    //
    // MS模式下的平均功率處理
    //
    if ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
    {
        float MS_L1_float_buf;
        float MS_L2_float_buf;
        float MS_L3_float_buf;


        if ( Global_Variables.MS_Function_Variables.MS_Total_Units == 2 )
        {
            //
            // 計算平均功率
            // 單機x2
            //

            MS_L1_float_buf = Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[0].P_display_shadow.all;
            MS_L2_float_buf = Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[1].P_display_shadow.all;

            Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES_Total.P_display_shadow.all = ( MS_L1_float_buf + MS_L2_float_buf ) * 0.5;
        }
        else if ( Global_Variables.MS_Function_Variables.MS_Total_Units == 3 )
        {
            //
            // 計算平均功率
            // 單機x3
            //

            MS_L1_float_buf = Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[0].P_display_shadow.all;
            MS_L2_float_buf = Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[1].P_display_shadow.all;
            MS_L3_float_buf = Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[2].P_display_shadow.all;

            Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES_Total.P_display_shadow.all = ( MS_L1_float_buf + MS_L2_float_buf + MS_L3_float_buf ) * 0.333;
        }
    }



    //////////////////////////////////////////////////////////////////////////////////////////////
    //
    // 移動平均累加及計算
    //
    Meas_Module.MoveAvg_V_rms_sum = Meas_Module.MoveAvg_V_rms_sum + Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow;
    Meas_Module.MoveAvg_V_avg_sum = Meas_Module.MoveAvg_V_avg_sum + Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;
    Meas_Module.MoveAvg_AC_V_rms_sum = Meas_Module.MoveAvg_AC_V_rms_sum + Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display_shadow;


    Meas_Module.MoveAvg_I_rms_sum = Meas_Module.MoveAvg_I_rms_sum + Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
    Meas_Module.MoveAvg_I_avg_sum = Meas_Module.MoveAvg_I_avg_sum + Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display_shadow;

    Meas_Module.MoveAvg_P_sum = Meas_Module.MoveAvg_P_sum + Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow.all;

    Meas_Module.MoveAvg_Count--;

    if ( Meas_Module.MoveAvg_Count <= 0 )
    {

        //
        // 計算結果
        //
        Meas_Module.MoveAvg_V_rms = Meas_Module.MoveAvg_V_rms_sum / (float)Meas_Module.MoveAvg_Total_Count;
        Meas_Module.MoveAvg_V_avg = Meas_Module.MoveAvg_V_avg_sum / (float)Meas_Module.MoveAvg_Total_Count;
        Meas_Module.MoveAvg_AC_V_rms = Meas_Module.MoveAvg_AC_V_rms_sum / (float)Meas_Module.MoveAvg_Total_Count;

        Meas_Module.MoveAvg_I_rms = Meas_Module.MoveAvg_I_rms_sum / (float)Meas_Module.MoveAvg_Total_Count;
        Meas_Module.MoveAvg_I_avg = Meas_Module.MoveAvg_I_avg_sum / (float)Meas_Module.MoveAvg_Total_Count;

        Meas_Module.MoveAvg_P = Meas_Module.MoveAvg_P_sum / (float)Meas_Module.MoveAvg_Total_Count;

        //
        // 更新V display
        //
        Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = Meas_Module.MoveAvg_V_rms;
        Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all = Meas_Module.MoveAvg_V_avg;
        Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display.all = Meas_Module.MoveAvg_AC_V_rms;


        //
        // 更新I display
        //
        Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = Meas_Module.MoveAvg_I_rms;
        Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.all = Meas_Module.MoveAvg_I_avg;

        //
        // +Vpk display
        //
        offset = Global_Variables.Calibration_Variables.p_V_peak_Offset.all;
        Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display_shadow + offset;

        //
        // -Vpk display
        //
        offset = Global_Variables.Calibration_Variables.n_V_peak_Offset.all;
        Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display_shadow + offset;


        //
        // +Ipk display
        //
        offset = Global_Variables.Calibration_Variables.p_I_peak_Offset.all;
        Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display_shadow + offset;


        //
        // -Ipk display
        //
        offset = Global_Variables.Calibration_Variables.n_I_peak_Offset.all;
        Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display_shadow + offset;


        //
        // 處理V, I rms offset
        //
        if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) //輸出OFF
        {
            Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all + Global_Variables.Calibration_Variables.V_rms_Offset.all;
            Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all + Global_Variables.Calibration_Variables.I_rms_Offset.all;
        }
        else if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 1 ) //輸出ON
        {
            if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 1 ) && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 1 ) )
            {
                //
                // AC+DC
                //
                if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 ) && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all == 0 ) )
                {
                    Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all + Global_Variables.Calibration_Variables.V_rms_Offset.all;
                    Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all + Global_Variables.Calibration_Variables.I_rms_Offset.all;
                }
            }
            else if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 0 ) && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 1 ) )
            {
                //
                // AC
                //
                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 )
                {
                    Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all + Global_Variables.Calibration_Variables.V_rms_Offset.all;
                    Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all + Global_Variables.Calibration_Variables.I_rms_Offset.all;
                }
            }
            else if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 1 ) && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 0 ) )
            {
                //
                // DC
                //
                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all == 0 )
                {
                    Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all + Global_Variables.Calibration_Variables.V_rms_Offset.all;
                    Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all + Global_Variables.Calibration_Variables.I_rms_Offset.all;
                }
            }
        }

        //
        // rms 最小值箝制
        //
        if ( Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all <= 0 )
        {
            Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = 0;
        }

        if ( Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all <= 0 )
        {
            Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = 0;
        }

        //
        // 更新P display
        //
        Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all = Meas_Module.MoveAvg_P;

        //
        // 更新S display
        //
        Global_Variables.POWER_DISPLAY_VARIABLES.S_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all * Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all;

        //
        // 更新Q display
        //
        double_buf = (double)Global_Variables.POWER_DISPLAY_VARIABLES.S_display.all * (double)Global_Variables.POWER_DISPLAY_VARIABLES.S_display.all;
        double_buf2 = (double)Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all * (double)Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all;

        //
        #ifdef DEVELOP_SV74
        Global_Variables.POWER_DISPLAY_VARIABLES.Q_display.all = (float)Global_Variables.POWER_FUNCTIONS_ERROR_CODE;
        #else
        Global_Variables.POWER_DISPLAY_VARIABLES.Q_display.all = (float)sqrt( double_buf - double_buf2 );
        #endif

        //
        // 更新PF display
        //
        if ( Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all > Global_Variables.POWER_DISPLAY_VARIABLES.S_display.all )
        {
            Global_Variables.POWER_DISPLAY_VARIABLES.PF_display = 1000;
        }
        else if ( Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all <= 0 )
        {
            Global_Variables.POWER_DISPLAY_VARIABLES.PF_display = 0;
        }
        else
        {
            float_buf = ( Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all * 1000.0 ) / Global_Variables.POWER_DISPLAY_VARIABLES.S_display.all;
            Global_Variables.POWER_DISPLAY_VARIABLES.PF_display = (int16)float_buf;
        }

        //
        // 更新CF display
        //
        abs_p_Ipeak = fabs( Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display.all );
        abs_n_Ipeak = fabs( Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display.all );


        //
        #ifdef DEVELOP_SV74
        Global_Variables.POWER_DISPLAY_VARIABLES.CF_display = MS_Control_Module_Variables.MS_Connect_State;
        #else
        if ( abs_p_Ipeak >= abs_n_Ipeak )
        {
            Global_Variables.POWER_DISPLAY_VARIABLES.CF_display = (int16)( ( abs_p_Ipeak * 100.0 ) / Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all );
        }
        else
        {
            Global_Variables.POWER_DISPLAY_VARIABLES.CF_display = (int16)( ( abs_n_Ipeak * 100.0 ) / Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all );
        }
        #endif



        //
        // 顯示優化
        //
        if ( ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 1 ) && ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable == 1 ) ) //輸出ON
        {
            //
            // DC空載輸出顯示優化
            //
            if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 1 ) && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 0 ) )
            {
                if ( fabs( Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.all ) <= 0.03 )
                {
                    Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.all = 0;
                    Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all = 0;
                }
            }
        }
        else if ( ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) && ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable == 1 ) ) //輸出OFF
        {
            //
            // 電壓歸0
            //
            if ( Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all <= 1.0 )
            {
                Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display.all = 0;

                Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.S_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.PF_display = 0;

                //
                #ifndef DEVELOP_SV74
                Global_Variables.POWER_DISPLAY_VARIABLES.Q_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.CF_display = 0;
                #endif

            }

            //
            // 電流歸0
            //
            if ( Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all <= 0.1 )
            {
                Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display.all = 0;

                Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.S_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.PF_display = 0;

                //
                #ifndef DEVELOP_SV74
                Global_Variables.POWER_DISPLAY_VARIABLES.Q_display.all = 0;
                Global_Variables.POWER_DISPLAY_VARIABLES.CF_display = 0;
                #endif

            }
        }

        //
        // Reset移動平均累加
        //
        Display_Move_Avg_Reset();


    }

}



/////////////////////////////////////////////////////////////////////
//
// Display移動平均重置
//
void Display_Move_Avg_Reset(void)
{
    //
    // 清空移動平均累加
    //
    Meas_Module.MoveAvg_V_rms_sum = 0;
    Meas_Module.MoveAvg_V_avg_sum = 0;
    Meas_Module.MoveAvg_AC_V_rms_sum = 0;


    Meas_Module.MoveAvg_I_rms_sum = 0;
    Meas_Module.MoveAvg_I_avg_sum = 0;


    Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display_shadow = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout;
    Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display_shadow = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout;
    Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display_shadow = -Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak;
    Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display_shadow = Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak;


    Meas_Module.MoveAvg_P_sum = 0;

    //
    // Reload移動平均數
    //
    if ( Meas_Module.Meas_Period != 0 )
    {
        Meas_Module.MoveAvg_Total_Count = (Uint32)( DISPLAY_SAMPLING_RATE / Meas_Module.Meas_Period );

        if ( Meas_Module.MoveAvg_Total_Count == 0 )
        {
            Meas_Module.MoveAvg_Total_Count = 1;
        }
    }
    else
    {
        Meas_Module.MoveAvg_Total_Count = 1;
    }

    Meas_Module.MoveAvg_Count = Meas_Module.MoveAvg_Total_Count;

}















//
// End of file
//
