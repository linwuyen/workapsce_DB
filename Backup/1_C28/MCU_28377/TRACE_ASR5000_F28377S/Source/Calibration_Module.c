//###########################################################################
//
// FILE:    Calibration_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211117
//###########################################################################



#include "Global_VariableDefs.h"


volatile CALIBRATION_MODULE_VARIABLES_REG Calibration_Module_Variables;


/////////////////////////////////////////////////////////////////////
//
// 初始化校正參數
//
void Init_Calibration_Variables(void)
{
    Uint16 i, j;

    Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state;
    Calibration_Module_Variables.Calibration_AutoStart_delay_time = 0;
    Calibration_Module_Variables.Control_Flags.all = 0;



    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_avg_display.all = 0;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_avg_display_shadow = 0;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_avg_display.all = 0;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_avg_display_shadow = 0;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display.all = 0;
    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display_shadow = 0;

    Calibration_Module_Variables.Calibration_process_Variables.ExtAD_sum = 0;
    Calibration_Module_Variables.Calibration_process_Variables.ExtAD_Total_Sample_Points = 100.0;

    for ( i = 0; i < 3; i++ )
    {
        Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][i] = 0;
        Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][i] = 0;

        Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][i] = 0;
        Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][i] = 0;

        for ( j = 0; j < 6; j++ )
        {
            Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[i][j].all = 0;
            Calibration_Module_Variables.Calibration_process_Variables.Meter_Value_LSB_buf[i][j] = 0;
            Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_Value[i][j] = 0;
            Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[i][j] = 0;
            Calibration_Module_Variables.Calibration_process_Variables.REF_DAC_Value[i][j] = 0;
        }
    }

    #ifdef DEVELOP_SV73
    //
    // 降壓 for測試使用
    //
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][0] = -5000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][1] = 5000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][2] = 0;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][3] = -9000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][4] = 9000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][5] = 0;

    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][0] = -500000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][1] = 500000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][2] = 0;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][3] = -1000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][4] = 1000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][5] = 0;
    #else
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][0] = -5000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][1] = 5000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][2] = 0;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][3] = -10000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][4] = 10000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_VOLTGE][5] = 0;

    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][0] = -1000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][1] = 1000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][2] = 0;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][3] = -2000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][4] = 2000000;
    Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[CALI_CURRENT][5] = 0;
    #endif
}




/////////////////////////////////////////////////////////////////////
//
// 校正流程
//
void Calibration_Process(void)
{
    float scale, offset;
    float scale_p, scale_n;
    float offset_p, offset_n;

    float meter_value_00, meter_value_01, meter_value_02;
    float digital_value_00, digital_value_01, digital_value_02;

    float Rs, Cs, inv_Rs, Zs, Xc;

    float x[3], y[3], a, b, c;

    float level01, level02;

    switch(Calibration_Module_Variables.Calibration_process_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Ready_state:
        {
            break;
        }

        /*----------------------------------------------------------------------------------------------------------------------*/
        /*----------------------------------------------------------------------------------------------------------------------*/
        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Vdc_state0:
        {

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Vdc_state0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 停止輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_Vdc_state1;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Vdc_state1:
        {
            if ( Main_State_Module.Machine_state != READY_STATE )
            {
                break;
            }

            //
            // 回到READY
            //

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Vdc_state1\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 校正開始
            // 關閉補償及保護重設
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 0;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2 = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max * 1.1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max * 1.1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max * 1.1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = -3000.0;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power * 1.1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 11000.0 );

            Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
            Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

            //
            // 關閉優化
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 0;

            //
            // CLA量測ADC ON
            //
            CPU_to_CLA_Variables.Cla_Control_Flag.bit.Cali_Process = 1;


            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) //5075不支援換檔
            {
                //
                // 手動設定Range Low
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

                //
                // 換檔
                //
                Change_Output_Range_Event();

                //
                // 切換校正值
                //
                Load_HL_Cali_to_Cali_Event();

                //
                // 觸發校正Checksum計算事件
                //
                Update_HL_Cali_CheckSum_Event();
            }

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_Vdc_state;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Vdc_state:
        {


            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Vdc_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            /*---------------------------------------------------------------*/
            //
            // 更新peak offset
            //

            //20220125 測試沒有比較準,先保留不用
            /*
            Global_Variables.Calibration_Variables.p_V_peak_Offset.all = Global_Variables.Calibration_Variables.p_V_peak_Value[CALI_VOLTGE][2];
            Global_Variables.Calibration_Variables.n_V_peak_Offset.all = Global_Variables.Calibration_Variables.n_V_peak_Value[CALI_VOLTGE][2];
            Global_Variables.Calibration_Variables.p_I_peak_Offset.all = Global_Variables.Calibration_Variables.p_I_peak_Value[CALI_VOLTGE][2];
            Global_Variables.Calibration_Variables.n_I_peak_Offset.all = Global_Variables.Calibration_Variables.n_I_peak_Value[CALI_VOLTGE][2];

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " p_V_peak_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.p_V_peak_Value[CALI_VOLTGE][2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " n_V_peak_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.n_V_peak_Value[CALI_VOLTGE][2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " p_I_peak_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.p_I_peak_Value[CALI_VOLTGE][2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " n_I_peak_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.n_I_peak_Value[CALI_VOLTGE][2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif
            */

            /*---------------------------------------------------------------*/
            //
            // 計算電壓量測scale及offset
            //

            //
            //step 0: -Max
            //
            meter_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][0].all * 0.001;
            digital_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_Value[CALI_VOLTGE][0];

            //
            //Step 1: +Max
            //
            meter_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][1].all * 0.001;
            digital_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_Value[CALI_VOLTGE][1];

            //
            //Step 2: 0
            //
            meter_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][2].all * 0.001;
            digital_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_Value[CALI_VOLTGE][2];

            //
            // 計算
            //
            scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 );
            offset = ( meter_value_01 / scale_p ) - digital_value_01;
            scale_n = meter_value_00 / ( digital_value_00 + offset );

            //
            // 更新校正Data Version
            //
            Global_Variables.Calibration_Variables.Data_Version = CALI_DATA_VER;

            //
            // 更新校正值
            //
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all = scale_p;
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all = scale_n;
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all = offset;

            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_P.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_N.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Offset.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;

            CPU_to_CLA_Variables.ADC_to_V_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;
            CPU_to_CLA_Variables.ADC_to_V_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;
            CPU_to_CLA_Variables.ADC_to_V_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;


            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_00: %e\n", CpuTimer2.InterruptCount, meter_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_01: %e\n", CpuTimer2.InterruptCount, meter_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_02: %e\n", CpuTimer2.InterruptCount, meter_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_00: %e\n", CpuTimer2.InterruptCount, digital_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_01: %e\n", CpuTimer2.InterruptCount, digital_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_02: %e\n", CpuTimer2.InterruptCount, digital_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " scale_p: %e\n", CpuTimer2.InterruptCount, scale_p ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " offset = ( meter_value_01 / scale_p ) - digital_value_01\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Offset: %e\n", CpuTimer2.InterruptCount, offset ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " scale_n = meter_value_00 / ( digital_value_00 + offset )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " scale_n: %e\n", CpuTimer2.InterruptCount, scale_n ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEVELOP_SV72
            #else
            /*---------------------------------------------------------------*/
            //
            // 判斷校正值是否合理
            //
            level01 = ADC_TO_V_FB_OFFSET_5150;
            level02 = ADC_TO_V_FB_SCALE_P_5150;

            if ( ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all > ( level01 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all < ( level01 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all < ( level02 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all < ( level02 * 0.5 ) ) )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_ADC_to_V_FB_OVERRANGE;

                //
                // 轉態
                //
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, " ***CALI_ADC_to_V_FB_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 強制轉態
                //
                break;
            }
            #endif

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_DAC_state; //轉態

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_DAC_state:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_DAC_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            /*---------------------------------------------------------------*/
            //
            // 計算更新DAC
            //
            /*---------------------------------------------------------------*/
            //
            // 計算電壓REF DAC scale及offset
            //

            //
            //step 0: -Max
            //
            meter_value_00 = Calibration_Module_Variables.Calibration_process_Variables.DCV_set[CALI_VOLTGE][0];
            digital_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][0].all * 0.001;

            //
            //Step 1: +Max
            //
            meter_value_01 = Calibration_Module_Variables.Calibration_process_Variables.DCV_set[CALI_VOLTGE][1];
            digital_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][1].all * 0.001;

            //
            //Step 2: 0
            //
            meter_value_02 = Calibration_Module_Variables.Calibration_process_Variables.DCV_set[CALI_VOLTGE][2];
            digital_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][2].all * 0.001;

            //
            // +Max到0的Scale以及offset
            //
            scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 );
            offset_p = meter_value_01 - scale_p * digital_value_01;

            //
            // -Max到0的Scale以及offset
            //
            scale_n = ( meter_value_00 - meter_value_02 ) / ( digital_value_00 - digital_value_02 );
            offset_n = meter_value_00 - scale_n * digital_value_00;

            //
            // 取平均
            //
            scale = ( scale_p + scale_n ) * 0.5;        // a'
            offset = ( offset_p + offset_n ) * 0.5;     // b'

            //
            // 更新
            //
            a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
            b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all = a * scale;
            Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all = a * offset + b;

            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Scale.all = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Offset.all = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            //
            // 觸發重新計算VREF DAC箝制的事件
            //
            Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_00: %e\n", CpuTimer2.InterruptCount, meter_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_01: %e\n", CpuTimer2.InterruptCount, meter_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_02: %e\n", CpuTimer2.InterruptCount, meter_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_00: %e\n", CpuTimer2.InterruptCount, digital_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_01: %e\n", CpuTimer2.InterruptCount, digital_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_02: %e\n", CpuTimer2.InterruptCount, digital_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles


            sprintf( SCI_Module_Variables.Debug_Msg, " scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " scale_p: %e\n", CpuTimer2.InterruptCount, scale_p ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " offset_p = meter_value_01 - scale_p * digital_value_01\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " offset_p: %e\n", CpuTimer2.InterruptCount, offset_p ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " scale_n = ( meter_value_00 - meter_value_02 ) / ( digital_value_00 - digital_value_02 )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " scale_p: %e\n", CpuTimer2.InterruptCount, scale_p ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " offset_n = meter_value_00 - scale_n * digital_value_00\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " offset_n: %e\n", CpuTimer2.InterruptCount, offset_n ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " scale = ( scale_p + scale_n ) * 0.5\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " scale: %e\n", CpuTimer2.InterruptCount, scale ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " offset = ( offset_p + offset_n ) * 0.5\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " offset: %e\n", CpuTimer2.InterruptCount, offset ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " a = .VSET_to_DAC_Scale\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " a: %e\n", CpuTimer2.InterruptCount, a ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " b = .VSET_to_DAC_Offset\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " b: %e\n", CpuTimer2.InterruptCount, b ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " VSET_to_DAC_Scale(new) = a * scale\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " VSET_to_DAC_Scale(new): %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " VSET_to_DAC_Offset(new) = a * offset + b\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " VSET_to_DAC_Offset(new): %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            #endif


            //
            #ifdef DEVELOP_SV72
            #else
            /*---------------------------------------------------------------*/
            //
            // 判斷校正值是否合理
            //
            level01 = VSET_to_DAC_OFFSET_5150;
            level02 = VSET_to_DAC_SCALE_5150;

            if ( ( Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all > ( level01 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all < ( level01 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all < ( level02 * 0.5 ) ) )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_VSET_to_DAC_OVERRANGE;

                //
                // 轉態
                //
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, " ***CALI_VSET_to_DAC_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 強制轉態
                //
                break;
            }
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_I_offset_state; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_I_offset_state:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_I_offset_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 更新空載時的電流ADC讀值為offset
            //

            //
            // 拿電壓校正時Step 2 的空載電流讀值就是offset
            //
            offset = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_VOLTGE][2];

            //
            // 更新校正值
            //
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = -offset;
            CPU_to_CLA_Variables.ADC_to_I_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Offset.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;


            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " offset = ADS8353_CHB_Value[CALI_VOLTGE][2]\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Offset: %e\n", CpuTimer2.InterruptCount, offset ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step0_state; //轉態

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 0
        // Freq set = low
        // ACV set = 0
        //
        case Calibration_Freq_step0_state:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step0_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 關閉頻率補償
            //

            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable = 0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Freq_Compensation_Enable = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定FREQ, ACV...等參數, 準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION_FREQ_FSET_LOW;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //
            // 開始計時
            //
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;


            /*---------------------------------------------------------------*/
            //
            // 自動輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step1_state; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 1
        // 更新offset
        //
        case Calibration_Freq_step1_state:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {
                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step1_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                /*---------------------------------------------------------------*/
                //
                // 計算Vrms的offset ( F = Low / ACV = 0V )
                // 要疊代,否則連續校正後會趨近於0
                //
                Global_Variables.Calibration_Variables.V_rms_Offset.all = -Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all + Global_Variables.Calibration_Variables.V_rms_Offset.all;

                Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].V_rms_Offset.all = Global_Variables.Calibration_Variables.V_rms_Offset.all;


                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " V_rms_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.V_rms_Offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step2_state; //轉態
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 2
        // Freq set = low
        // ACV set = Max/2
        //
        case Calibration_Freq_step2_state:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step2_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定FREQ, ACV...等參數, 準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION_FREQ_FSET_LOW;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = CALIBRATION_FREQ_VSET;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //
            // 開始計時
            //
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;

            /*---------------------------------------------------------------*/
            //
            // 輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step3_state; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 3
        // 抓有效值
        //
        case Calibration_Freq_step3_state:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {
                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step3_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                /*---------------------------------------------------------------*/
                //
                // 更新電壓電流有效值
                //
                Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][0] = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all;
                Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][0] = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display.all;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " Vrms_Value[LOW_RANGE][0]: %e\n", CpuTimer2.InterruptCount, Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " Irms_no_Scale_Value[LOW_RANGE][0]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][0] ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step4_state; //轉態
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 4
        // Freq set = mid
        // ACV set = Max/2
        //
        case Calibration_Freq_step4_state:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step4_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定FREQ, ACV...等參數, 準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION_FREQ_FSET_MID;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = CALIBRATION_FREQ_VSET;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //
            // 開始計時
            //
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;

            /*---------------------------------------------------------------*/
            //
            // 輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step5_state; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 5
        // 抓有效值
        //
        case Calibration_Freq_step5_state:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {
                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step5_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                /*---------------------------------------------------------------*/
                //
                // 更新電壓電流有效值
                //

                Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][1] = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all;
                Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][1] = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display.all;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " Vrms_Value[LOW_RANGE][1]: %e\n", CpuTimer2.InterruptCount, Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " Irms_no_Scale_Value[LOW_RANGE][1]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][1] ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step6_state; //轉態
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 6
        // Freq set = high
        // ACV set = Max/2
        //
        case Calibration_Freq_step6_state:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step6_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定FREQ, ACV...等參數, 準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION_FREQ_FSET_HIGH;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = CALIBRATION_FREQ_VSET;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //
            // 開始計時
            //
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;

            /*---------------------------------------------------------------*/
            //
            // 輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step7_state; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 7
        // 抓有效值
        //
        case Calibration_Freq_step7_state:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step7_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                /*---------------------------------------------------------------*/
                //
                // 更新電壓電流有效值
                //
                Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][2] = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all;
                Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][2] = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display.all;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " Vrms_Value[LOW_RANGE][2]: %e\n", CpuTimer2.InterruptCount, Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " Irms_no_Scale_Value[LOW_RANGE][2]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][2] ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                /*---------------------------------------------------------------*/
                //
                // 停止輸出
                //
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                /*---------------------------------------------------------------*/
                //
                // 啟用頻率補償
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable = 1;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " Freq_Compensation_Enable = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step8_state; //轉態
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 8
        // 計算頻率補償校正參數
        //
        case Calibration_Freq_step8_state:
        {
            if ( Main_State_Module.Machine_state != READY_STATE )
            {
                break;
            }

            //
            // 回到READY
            //

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Freq_step8_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 計算頻率補償係數a, b, c
            //

            x[0] = (float)CALIBRATION_FREQ_FSET_LOW * 0.01;
            x[1] = (float)CALIBRATION_FREQ_FSET_MID * 0.01;
            x[2] = (float)CALIBRATION_FREQ_FSET_HIGH * 0.01;

            y[0] = Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][0] / Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][0];
            y[1] = Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][0] / Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][1];
            y[2] = Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][0] / Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][2];

            //
            // 解y=ax^2+bx+c
            //

            a = ( ( y[1] - y[0] ) * ( x[0] - x[2] ) + ( y[2] - y[0] ) * ( x[1] - x[0] ) )
                / ( ( x[0] - x[2] ) * ( x[1] * x[1] - x[0] * x[0] ) + ( x[1] - x[0] ) * ( x[2] * x[2] - x[0] * x[0] ) );

            b = ( ( y[1] - y[0] ) - a * ( x[1] * x[1] - x[0] * x[0] ) ) / ( x[1] - x[0] );

            c = y[0] - a * x[0] * x[0] - b * x[0];


            //
            // 更新校正值
            //

            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all = a;
            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all = b;
            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all = c;

            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_a.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_b.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_c.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " x[0] = %e, x[1] = %e, x[2] = %e\n", CpuTimer2.InterruptCount, x[0], x[1], x[2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " y[0] = %e, y[1] = %e, y[2] = %e\n", CpuTimer2.InterruptCount, y[0], y[1], y[2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " a = ( ( y[1] - y[0] ) * ( x[0] - x[2] ) + ( y[2] - y[0] ) * ( x[1] - x[0] ) )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "     / ( ( x[0] - x[2] ) * ( x[1] * x[1] - x[0] * x[0] ) + ( x[1] - x[0] ) * ( x[2] * x[2] - x[0] * x[0] ) )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Freq_Compensation_coeff_a: %e\n", CpuTimer2.InterruptCount, a ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " b = ( ( y[1] - y[0] ) - a * ( x[1] * x[1] - x[0] * x[0] ) ) / ( x[1] - x[0] )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Freq_Compensation_coeff_b: %e\n", CpuTimer2.InterruptCount, b ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " c = y[0] - a * x[0] * x[0] - b * x[0]\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Freq_Compensation_coeff_c: %e\n", CpuTimer2.InterruptCount, c ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            //
            // 把目前校正參數都印出來
            //

            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " ADC_to_V_FB_Scale_P: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " ADC_to_V_FB_Scale_N: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " ADC_to_V_FB_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " VSET_to_DAC_Scale: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " VSET_to_DAC_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " ADC_to_I_FB_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " V_rms_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.V_rms_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " Freq_Compensation_coeff_a: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Freq_Compensation_coeff_b: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Freq_Compensation_coeff_c: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
            {
                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Vdc_state0_hr;

            }
            else if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5075 )
            {
                //
                // ASR-5075 只計算到這裡
                //

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 觸發校正Checksum計算事件
                //
                Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                Global_Variables.Calibration_State_Flag.all = 1; //finish

                //
                // 校正結束
                // 恢復補償及保護
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 1;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 10500.0 );

                Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
                Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

                //
                // 啟用優化
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 1;

                //
                // CLA量測ADC OFF
                //
                CPU_to_CLA_Variables.Cla_Control_Flag.bit.Cali_Process = 0;

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Vdc_state0_hr:
        {

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Vdc_state0_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 停止輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_Vdc_state1_hr;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Vdc_state1_hr:
        {
            if ( Main_State_Module.Machine_state != READY_STATE )
            {
                break;
            }

            //
            // 回到READY
            //

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Vdc_state1_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) //5075不支援換檔
            {
                //
                // 手動設定Range Low
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                //
                // 換檔
                //
                Change_Output_Range_Event();

                //
                // 切換校正值
                //
                Load_HL_Cali_to_Cali_Event();

                //
                // 觸發校正Checksum計算事件
                //
                Update_HL_Cali_CheckSum_Event();
            }

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_Vdc_state_hr;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Vdc_state_hr:
        {

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Vdc_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 更新peak offset
            //

            //20220125 測試沒有比較準,先保留不用
            /*
            Global_Variables.Calibration_Variables.p_V_peak_Offset.all = Global_Variables.Calibration_Variables.p_V_peak_Value[CALI_VOLTGE][2];
            Global_Variables.Calibration_Variables.n_V_peak_Offset.all = Global_Variables.Calibration_Variables.n_V_peak_Value[CALI_VOLTGE][2];
            Global_Variables.Calibration_Variables.p_I_peak_Offset.all = Global_Variables.Calibration_Variables.p_I_peak_Value[CALI_VOLTGE][2];
            Global_Variables.Calibration_Variables.n_I_peak_Offset.all = Global_Variables.Calibration_Variables.n_I_peak_Value[CALI_VOLTGE][2];

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " p_V_peak_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.p_V_peak_Value[CALI_VOLTGE][2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " n_V_peak_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.n_V_peak_Value[CALI_VOLTGE][2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " p_I_peak_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.p_I_peak_Value[CALI_VOLTGE][2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " n_I_peak_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.n_I_peak_Value[CALI_VOLTGE][2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif
            */

            /*---------------------------------------------------------------*/
            //
            // 計算電壓量測scale及offset
            //

            //
            //step 3: -Max
            //
            meter_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][3].all * 0.001;
            digital_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_Value[CALI_VOLTGE][3];

            //
            //Step 4: +Max
            //
            meter_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][4].all * 0.001;
            digital_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_Value[CALI_VOLTGE][4];

            //
            //Step 5: 0
            //
            meter_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][5].all * 0.001;
            digital_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_Value[CALI_VOLTGE][5];

            //
            // 計算
            //
            scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 );
            offset = ( meter_value_01 / scale_p ) - digital_value_01;
            scale_n = meter_value_00 / ( digital_value_00 + offset );

            //
            // 更新校正Data Version
            //
            Global_Variables.Calibration_Variables.Data_Version = CALI_DATA_VER;

            //
            // 更新校正值
            //

            Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all = scale_p;
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all = scale_n;
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all = offset;

            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Offset.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;

            CPU_to_CLA_Variables.ADC_to_V_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;
            CPU_to_CLA_Variables.ADC_to_V_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;
            CPU_to_CLA_Variables.ADC_to_V_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;


            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_00: %e\n", CpuTimer2.InterruptCount, meter_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_01: %e\n", CpuTimer2.InterruptCount, meter_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Meter_Value_02: %e\n", CpuTimer2.InterruptCount, meter_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_00: %e\n", CpuTimer2.InterruptCount, digital_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_01: %e\n", CpuTimer2.InterruptCount, digital_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Digital_Value_02: %e\n", CpuTimer2.InterruptCount, digital_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " scale_p: %e\n", CpuTimer2.InterruptCount, scale_p ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " offset = ( meter_value_01 / scale_p ) - digital_value_01\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " Offset: %e\n", CpuTimer2.InterruptCount, offset ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, " scale_n = meter_value_00 / ( digital_value_00 + offset )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, " scale_n: %e\n", CpuTimer2.InterruptCount, scale_n ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            //
            #ifdef DEVELOP_SV72
            #else
            /*---------------------------------------------------------------*/
            //
            // 判斷校正值是否合理
            //
            level01 = ADC_TO_V_FB_OFFSET_5151;
            level02 = ADC_TO_V_FB_SCALE_P_5151;

            if ( ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all > ( level01 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all < ( level01 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all < ( level02 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all < ( level02 * 0.5 ) ) )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_ADC_to_V_FB_OVERRANGE;

                //
                // 轉態
                //
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, " ***CALI_ADC_to_V_FB_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, " -----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, " -> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 強制轉態
                //
                break;
            }
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_DAC_state_hr; //轉態

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_DAC_state_hr:
        {
            #ifdef DEBUG_ON
            C28_PRINTF("-------------------- %s ---------------------\n", "Calibration_DAC_state_hr");
            #endif

            /*---------------------------------------------------------------*/
            // 計算更新DAC
            // 計算電壓REF DAC scale及offset

            //
            //step 3: -Max
            meter_value_00 = Calibration_Module_Variables.Calibration_process_Variables.DCV_set[CALI_VOLTGE][3];
            digital_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][3].all * 0.001;

            //
            //Step 4: +Max
            meter_value_01 = Calibration_Module_Variables.Calibration_process_Variables.DCV_set[CALI_VOLTGE][4];
            digital_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][4].all * 0.001;

            //
            //Step 5: 0
            meter_value_02 = Calibration_Module_Variables.Calibration_process_Variables.DCV_set[CALI_VOLTGE][5];
            digital_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][5].all * 0.001;

            //
            // +Max到0的Scale以及offset
            scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 );
            offset_p = meter_value_01 - scale_p * digital_value_01;

            //
            // -Max到0的Scale以及offset
            scale_n = ( meter_value_00 - meter_value_02 ) / ( digital_value_00 - digital_value_02 );
            offset_n = meter_value_00 - scale_n * digital_value_00;

            //
            // 取平均
            scale = ( scale_p + scale_n ) * 0.5;        // a'
            offset = ( offset_p + offset_n ) * 0.5;     // b'

            //
            // 更新
            a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
            b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all = a * scale;
            Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all = a * offset + b;

            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Scale.all = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Offset.all = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            //
            // 觸發重新計算VREF DAC箝制的事件
            Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit = 1;


            //debug msg
            #ifdef DEBUG_ON
            C28_PRINTF("Meter_Value_00: %e\n", meter_value_00 );
            C28_PRINTF("Meter_Value_01: %e\n", meter_value_01 );
            C28_PRINTF("Meter_Value_02: %e\n", meter_value_02 );
            C28_PRINTF("Digital_Value_00: %e\n", digital_value_00 );
            C28_PRINTF("Digital_Value_01: %e\n", digital_value_01 );
            C28_PRINTF("Digital_Value_02: %e\n", digital_value_02 );
            C28_PRINTF("scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 ): %e\n", scale_p );
            C28_PRINTF("offset_p = meter_value_01 - scale_p * digital_value_01: %e\n", offset_p );
            C28_PRINTF("scale_n = ( meter_value_00 - meter_value_02 ) / ( digital_value_00 - digital_value_02 ): %e\n", scale_n );
            C28_PRINTF("offset_n = meter_value_00 - scale_n * digital_value_00: %e\n", offset_n );
            C28_PRINTF("scale = ( scale_p + scale_n ) * 0.5: %e\n", scale );
            C28_PRINTF("offset = ( offset_p + offset_n ) * 0.5: %e\n", offset);
            C28_PRINTF("a = .VSET_to_DAC_Scale: %e\n", a );
            C28_PRINTF("b = .VSET_to_DAC_Offset: %e\n", b );
            C28_PRINTF("VSET_to_DAC_Scale(new): %e\n", Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all );
            C28_PRINTF("VSET_to_DAC_Offset(new): %e\n", Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all );
            #endif

            //
            #ifdef DEVELOP_SV72
            #else
            /*---------------------------------------------------------------*/
            //
            // 判斷校正值是否合理
            //
            level01 = VSET_to_DAC_OFFSET_5151;
            level02 = VSET_to_DAC_SCALE_5151;

            if ( ( Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all > ( level01 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all < ( level01 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all < ( level02 * 0.5 ) ) )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_VSET_to_DAC_OVERRANGE;

                //
                // 轉態
                //
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "***CALI_VSET_to_DAC_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 強制轉態
                //
                break;
            }
            #endif

            Calibration_Module_Variables.Calibration_process_state = Calibration_I_offset_state_hr; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_I_offset_state_hr:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_I_offset_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 更新空載時的電流ADC讀值為offset
            //

            //
            // 拿電壓校正時Step 5 的空載電流讀值就是offset
            //
            offset = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_VOLTGE][5];

            //
            // 更新校正值
            //
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = -offset;
            CPU_to_CLA_Variables.ADC_to_I_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Offset.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "offset = ADS8353_CHB_Value[CALI_VOLTGE][5]\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Offset: %e\n", CpuTimer2.InterruptCount, offset ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step0_state_hr; //轉態

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 0
        // Freq set = low
        // ACV set = 0
        //
        case Calibration_Freq_step0_state_hr:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step0_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 關閉頻率補償
            //

            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable = 0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Freq_Compensation_Enable = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定FREQ, ACV...等參數, 準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION_FREQ_FSET_LOW;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //
            // 開始計時
            //
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;


            /*---------------------------------------------------------------*/
            //
            // 自動輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step1_state_hr; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 1
        // 更新offset
        //
        case Calibration_Freq_step1_state_hr:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {
                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step1_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                /*---------------------------------------------------------------*/
                //
                // 計算Vrms的offset ( F = Low / ACV = 0V )
                // 要疊代,否則連續校正後會趨近於0
                //
                Global_Variables.Calibration_Variables.V_rms_Offset.all = -Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all + Global_Variables.Calibration_Variables.V_rms_Offset.all;

                Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].V_rms_Offset.all = Global_Variables.Calibration_Variables.V_rms_Offset.all;


                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "V_rms_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.V_rms_Offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step2_state_hr; //轉態
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 2
        // Freq set = low
        // ACV set = Max/2
        //
        case Calibration_Freq_step2_state_hr:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step2_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定FREQ, ACV...等參數, 準備輸出
            //

            //設定頻率
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION_FREQ_FSET_LOW;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = CALIBRATION_FREQ_VSET_HR;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //
            // 開始計時
            //
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;

            /*---------------------------------------------------------------*/
            //
            // 輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step3_state_hr; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 3
        // 抓有效值
        //
        case Calibration_Freq_step3_state_hr:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {
                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step3_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                /*---------------------------------------------------------------*/
                //
                // 更新電壓電流有效值
                //
                Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][0] = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all;
                Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][0] = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display.all;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "Vrms_Value[HIGH_RANGE][0]: %e\n", CpuTimer2.InterruptCount, Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "Irms_no_Scale_Value[HIGH_RANGE][0]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][0] ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step4_state_hr; //轉態
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 4
        // Freq set = mid
        // ACV set = Max/2
        //
        case Calibration_Freq_step4_state_hr:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step4_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定FREQ, ACV...等參數, 準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION_FREQ_FSET_MID;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = CALIBRATION_FREQ_VSET_HR;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //
            // 開始計時
            //
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;

            /*---------------------------------------------------------------*/
            //
            // 輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step5_state_hr; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 5
        // 抓有效值
        //
        case Calibration_Freq_step5_state_hr:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {
                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step5_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                /*---------------------------------------------------------------*/
                //
                // 更新電壓電流有效值
                //

                Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][1] = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all;
                Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][1] = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display.all;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "Vrms_Value[HIGH_RANGE][1]: %e\n", CpuTimer2.InterruptCount, Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "Irms_no_Scale_Value[HIGH_RANGE][1]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][1] ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step6_state_hr; //轉態
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 6
        // Freq set = high
        // ACV set = Max/2
        //
        case Calibration_Freq_step6_state_hr:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step6_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定FREQ, ACV...等參數, 準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION_FREQ_FSET_HIGH;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = CALIBRATION_FREQ_VSET_HR;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //
            // 開始計時
            //
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;

            /*---------------------------------------------------------------*/
            //
            // 輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step7_state_hr; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 7
        // 抓有效值
        //
        case Calibration_Freq_step7_state_hr:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step7_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                /*---------------------------------------------------------------*/
                //
                // 更新電壓電流有效值
                //
                Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][2] = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all;
                Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][2] = Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_minus_offset_rms_display.all;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "Vrms_Value[HIGH_RANGE][2]: %e\n", CpuTimer2.InterruptCount, Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "Irms_no_Scale_Value[HIGH_RANGE][2]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][2] ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                /*---------------------------------------------------------------*/
                //
                // 停止輸出
                //
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                /*---------------------------------------------------------------*/
                //
                // 啟用頻率補償
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable = 1;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "Freq_Compensation_Enable = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif



                Calibration_Module_Variables.Calibration_process_state = Calibration_Freq_step8_state_hr; //轉態
                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 頻率補償自動校正 Step 8
        // 計算頻率補償校正參數
        //
        case Calibration_Freq_step8_state_hr:
        {
            if ( Main_State_Module.Machine_state != READY_STATE )
            {
                break;
            }

            //
            // 回到READY
            //

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Freq_step8_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 計算頻率補償係數a, b, c
            //

            x[0] = (float)CALIBRATION_FREQ_FSET_LOW * 0.01;
            x[1] = (float)CALIBRATION_FREQ_FSET_MID * 0.01;
            x[2] = (float)CALIBRATION_FREQ_FSET_HIGH * 0.01;

            y[0] = Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][0] / Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][0];
            y[1] = Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][0] / Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][1];
            y[2] = Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][0] / Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][2];

            //
            // 解y=ax^2+bx+c
            //

            a = ( ( y[1] - y[0] ) * ( x[0] - x[2] ) + ( y[2] - y[0] ) * ( x[1] - x[0] ) )
                / ( ( x[0] - x[2] ) * ( x[1] * x[1] - x[0] * x[0] ) + ( x[1] - x[0] ) * ( x[2] * x[2] - x[0] * x[0] ) );

            b = ( ( y[1] - y[0] ) - a * ( x[1] * x[1] - x[0] * x[0] ) ) / ( x[1] - x[0] );

            c = y[0] - a * x[0] * x[0] - b * x[0];

            //
            // 更新校正值
            //

            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all = a;
            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all = b;
            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all = c;


            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_a.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_b.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_c.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all;


            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "x[0] = %e, x[1] = %e, x[2] = %e\n", CpuTimer2.InterruptCount, x[0], x[1], x[2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "y[0] = %e, y[1] = %e, y[2] = %e\n", CpuTimer2.InterruptCount, y[0], y[1], y[2] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "a = ( ( y[1] - y[0] ) * ( x[0] - x[2] ) + ( y[2] - y[0] ) * ( x[1] - x[0] ) )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "    / ( ( x[0] - x[2] ) * ( x[1] * x[1] - x[0] * x[0] ) + ( x[1] - x[0] ) * ( x[2] * x[2] - x[0] * x[0] ) )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Freq_Compensation_coeff_a: %e\n", CpuTimer2.InterruptCount, a ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "b = ( ( y[1] - y[0] ) - a * ( x[1] * x[1] - x[0] * x[0] ) ) / ( x[1] - x[0] )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Freq_Compensation_coeff_b: %e\n", CpuTimer2.InterruptCount, b ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "c = y[0] - a * x[0] * x[0] - b * x[0]\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Freq_Compensation_coeff_c: %e\n", CpuTimer2.InterruptCount, c ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            //
            // 把目前校正參數都印出來
            //

            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_V_FB_Scale_P: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_V_FB_Scale_N: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_V_FB_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "VSET_to_DAC_Scale: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "VSET_to_DAC_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_I_FB_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "V_rms_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.V_rms_Offset.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Freq_Compensation_coeff_a: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Freq_Compensation_coeff_b: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Freq_Compensation_coeff_c: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 觸發校正Checksum計算事件
            //
            Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

            Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

            Global_Variables.Calibration_State_Flag.all = 1; //finish

            //
            // 校正結束
            // 恢復補償及保護
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 10500.0 );

            Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
            Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

            //
            // 啟用優化
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 1;

            //
            // CLA量測ADC OFF
            //
            CPU_to_CLA_Variables.Cla_Control_Flag.bit.Cali_Process = 0;

            break;
        }


        /*----------------------------------------------------------------------------------------------------------------------*/
        /*----------------------------------------------------------------------------------------------------------------------*/
        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Idc_state0:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Idc_state0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 停止輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_Idc_state1;

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Idc_state1:
        {
            if ( Main_State_Module.Machine_state != READY_STATE )
            {
                break;
            }

            //
            // 回到READY
            //

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Idc_state1\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            //
            // 校正開始
            // 關閉補償及保護重設
            //
            //Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 0;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2 = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max * 1.1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max * 1.1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max * 1.1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = -3000.0;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power * 1.1;
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 11000.0 );

            Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
            Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

            //
            // 關閉優化
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 0;

            //
            // CLA量測ADC ON
            //
            CPU_to_CLA_Variables.Cla_Control_Flag.bit.Cali_Process = 1;


            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) //5075不支援換檔
            {
                //
                // 手動設定Range Low
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

                //
                // 換檔
                //
                Change_Output_Range_Event();

                //
                // 切換校正值
                //
                Load_HL_Cali_to_Cali_Event();

                //
                // 觸發校正Checksum計算事件
                //
                Update_HL_Cali_CheckSum_Event();
            }

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_Idc_state;


            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Idc_state:
        {

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Idc_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            /*---------------------------------------------------------------*/
            //
            // 計算電流量測scale及offset
            //

            //step 0: -Max
            meter_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_CURRENT][0].all * 0.001;
            digital_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_CURRENT][0];

            //Step 1: +Max
            meter_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_CURRENT][1].all * 0.001;
            digital_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_CURRENT][1];

            //Step 2: 0
            // 拿空載時的電流當0點
            //meter_value_02 = 0;
            //digital_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_VOLTGE][2];
            meter_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_CURRENT][2].all * 0.001;
            digital_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_CURRENT][2];


            scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 );

            offset = ( meter_value_01 / scale_p ) - digital_value_01;

            scale_n = meter_value_00 / ( digital_value_00 + offset );

            Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all = scale_p;
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all = scale_n;
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = offset;

            CPU_to_CLA_Variables.ADC_to_I_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;
            CPU_to_CLA_Variables.ADC_to_I_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;
            CPU_to_CLA_Variables.ADC_to_I_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_P.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_N.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Offset.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_00: %e\n", CpuTimer2.InterruptCount, meter_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_01: %e\n", CpuTimer2.InterruptCount, meter_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_02: %e\n", CpuTimer2.InterruptCount, meter_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_00: %e\n", CpuTimer2.InterruptCount, digital_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_01: %e\n", CpuTimer2.InterruptCount, digital_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_02: %e\n", CpuTimer2.InterruptCount, digital_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "scale_p: %e\n", CpuTimer2.InterruptCount, scale_p ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "offset = offset = ( meter_value_01 / scale_p ) - digital_value_01\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Offset: %e\n", CpuTimer2.InterruptCount, offset ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "scale_n = meter_value_00 / ( digital_value_00 + offset )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "scale_n: %e\n", CpuTimer2.InterruptCount, scale_n ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            //
            #ifdef DEVELOP_SV72
            #else
            /*---------------------------------------------------------------*/
            //
            // 判斷校正值是否合理
            //
            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5075 )
            {
                level01 = ADC_TO_I_FB_OFFSET_5075;
                level02 = ADC_TO_I_FB_SCALE_P_5075;
            }
            else if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
            {
                level01 = ADC_TO_I_FB_OFFSET_5150;
                level02 = ADC_TO_I_FB_SCALE_P_5150;
            }

            if ( ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all > ( level01 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all < ( level01 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all < ( level02 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all < ( level02 * 0.5 ) ) )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_ADC_to_I_FB_OVERRANGE;

                //
                // 轉態
                //
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "***CALI_ADC_to_V_FB_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 強制轉態
                //
                break;
            }
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Rs_Cs_state; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Rs_Cs_state:
        {

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Rs_Cs_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 估算Rs時取的範圍是+Max到-Max
            // 計算電流斜率Scal把正跟負取平均
            scale = 0.5 * ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all + Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all );

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "scale = ( ADC_to_I_FB_Scale_P + ADC_to_I_FB_Scale_N ) / 2\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "scale: %e\n", CpuTimer2.InterruptCount, scale ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            /*---------------------------------------------------------------*/
            //
            // 計算Rs
            //

            //step 0: -Max
            meter_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][0].all * 0.001;
            digital_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_VOLTGE][0];

            //Step 1: +Max
            meter_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][1].all * 0.001;
            digital_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_VOLTGE][1];


            Rs = ( meter_value_01 - meter_value_00 ) / ( ( digital_value_01 - digital_value_00 ) * scale );
            inv_Rs = 1 / Rs;

            Global_Variables.Calibration_Variables.inv_Rs.all = inv_Rs;
            CPU_to_CLA_Variables.inv_Rs = Global_Variables.Calibration_Variables.inv_Rs.all;

            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].inv_Rs.all = Global_Variables.Calibration_Variables.inv_Rs.all;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_00: %e\n", CpuTimer2.InterruptCount, meter_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_01: %e\n", CpuTimer2.InterruptCount, meter_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_00: %e\n", CpuTimer2.InterruptCount, digital_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_01: %e\n", CpuTimer2.InterruptCount, digital_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Rs = ( meter_value_01 - meter_value_00 ) / ( ( digital_value_01 - digital_value_00 ) * scale )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Rs: %e\n", CpuTimer2.InterruptCount, Rs ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "inv_Rs: %e\n", CpuTimer2.InterruptCount, inv_Rs ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            /*---------------------------------------------------------------*/
            //
            // 計算電容Cs
            //

            Zs = Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][1] / ( Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][1] * scale );
            Xc = ( Rs * Zs ) / ( Rs - Zs );
            Cs = 1 / ( 2.0 * M_PI * CALIBRATION_FREQ_FSET_MID * 0.01 * Xc );

            Global_Variables.Calibration_Variables.Cs_mpy_360000.all = Cs * 360000.0;
            CPU_to_CLA_Variables.Cs_mpy_360000 = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;

            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Cs_mpy_360000.all = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "scale: %e\n", CpuTimer2.InterruptCount, scale ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Vrms_Value[LOW_RANGE][1]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[LOW_RANGE][1] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Irms_no_Scale_Value[LOW_RANGE][1]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[LOW_RANGE][1] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Zs = Vrms_Value[1] / ( Irms_no_Scale_Value[1] * scale )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Zs: %e\n", CpuTimer2.InterruptCount, Zs ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Xc = ( Rs * Zs ) / ( Rs - Zs )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Xc: %e\n", CpuTimer2.InterruptCount, Xc ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Cs = 1 / ( 2.0 * 3.14159 * CALIBRATION_FREQ_FSET_MID * 0.01 * Xc )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Cs: %e\n", CpuTimer2.InterruptCount, Cs ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            #ifdef DEVELOP_SV72
            #else
            /*---------------------------------------------------------------*/
            //
            // 判斷估算出來的Cs及Rs是否合理
            //
            if ( ( Cs * 360000.0 > CS_MPY_360000_MAX ) || ( inv_Rs > INV_RS_MAX ) )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_CS_RS_OVERRANGE;

                //
                // 轉態
                //
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "***CALI_CS_RS_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 強制轉態
                //
                break;
            }
            #endif

            Calibration_Module_Variables.Calibration_process_state = Calibration_Irms_offset_step0_state; //轉態
            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // 自動校正 Irms offset Step 0
        // Freq set = CALIBRATION__IRMS_OFFSET_FSET
        // ACV set = 0
        //
        case Calibration_Irms_offset_step0_state:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Irms_offset_step0_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定Freq, ACV...等,準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION__IRMS_OFFSET_FSET;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //開始計時
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;

            /*---------------------------------------------------------------*/
            //
            // 輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Irms_offset_step1_state; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 自動校正 Irms offset Step 1
        // 更新offset
        //
        case Calibration_Irms_offset_step1_state:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {
                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Irms_offset_step1_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                /*---------------------------------------------------------------*/
                //
                // 計算電流rms的offset ( F = 50Hz / ACV = 0V )
                // 要疊代計算
                //
                Global_Variables.Calibration_Variables.I_rms_Offset.all = -Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all + Global_Variables.Calibration_Variables.I_rms_Offset.all;

                Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].I_rms_Offset.all = Global_Variables.Calibration_Variables.I_rms_Offset.all;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "I_rms_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.I_rms_Offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                /*---------------------------------------------------------------*/
                //
                // 停止輸出
                //
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif



                /*---------------------------------------------------------------*/

                //把目前校正參數都印出來

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_I_FB_Scale_P: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_I_FB_Scale_N: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_I_FB_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "inv_Rs: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.inv_Rs.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "Cs_mpy_360000: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.Cs_mpy_360000.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "I_rms_Offset: %e\n", CpuTimer2.InterruptCount , Global_Variables.Calibration_Variables.I_rms_Offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif



                if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
                {
                    //
                    // 轉態
                    //
                    Calibration_Module_Variables.Calibration_process_state = Calibration_Idc_state0_hr;

                }
                else if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5075 )
                {
                    //
                    // ASR-5075 只計算到這裡
                    //

                    //debug msg
                    #ifdef DEBUG_ON
                    sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    //
                    // 觸發校正Checksum計算事件
                    //
                    Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

                    Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                    Global_Variables.Calibration_State_Flag.all = 1; //finish

                    //
                    // 校正結束
                    // 恢復補償及保護
                    //
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 1;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 10500.0 );

                    Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
                    Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

                    //
                    // 啟用優化
                    //
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 1;

                    //
                    // CLA量測ADC OFF
                    //
                    CPU_to_CLA_Variables.Cla_Control_Flag.bit.Cali_Process = 0;

                }

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/

        case Calibration_Idc_state0_hr:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Idc_state0_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 停止輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_Idc_state1_hr;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Idc_state1_hr:
        {
            if ( Main_State_Module.Machine_state != READY_STATE )
            {
                break;
            }

            //
            // 回到READY
            //

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Idc_state1_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) //5075不支援換檔
            {
                //
                // 手動設定Range
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                //
                // 換檔
                //
                Change_Output_Range_Event();

                //
                // 切換校正值
                //
                Load_HL_Cali_to_Cali_Event();

                //
                // 觸發校正Checksum計算事件
                //
                Update_HL_Cali_CheckSum_Event();
            }

            //
            // 轉態
            //
            Calibration_Module_Variables.Calibration_process_state = Calibration_Idc_state_hr;


            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Idc_state_hr:
        {

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Idc_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            /*---------------------------------------------------------------*/
            //
            // 計算電流量測scale及offset
            //

            //step 3: -Max
            meter_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_CURRENT][3].all * 0.001;
            digital_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_CURRENT][3];

            //Step 4: +Max
            meter_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_CURRENT][4].all * 0.001;
            digital_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_CURRENT][4];

            //Step 5: 0
            // 拿空載時的電流當0點
            //meter_value_02 = 0;
            //digital_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_VOLTGE][5];
            meter_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_CURRENT][5].all * 0.001;
            digital_value_02 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_CURRENT][5];


            scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 );

            offset = ( meter_value_01 / scale_p ) - digital_value_01;

            scale_n = meter_value_00 / ( digital_value_00 + offset );

            Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all = scale_p;
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all = scale_n;
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = offset;

            CPU_to_CLA_Variables.ADC_to_I_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;
            CPU_to_CLA_Variables.ADC_to_I_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;
            CPU_to_CLA_Variables.ADC_to_I_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Offset.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_00: %e\n", CpuTimer2.InterruptCount, meter_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_01: %e\n", CpuTimer2.InterruptCount, meter_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_02: %e\n", CpuTimer2.InterruptCount, meter_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_00: %e\n", CpuTimer2.InterruptCount, digital_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_01: %e\n", CpuTimer2.InterruptCount, digital_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_02: %e\n", CpuTimer2.InterruptCount, digital_value_02 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "scale_p = ( meter_value_01 - meter_value_02 ) / ( digital_value_01 - digital_value_02 )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "scale_p: %e\n", CpuTimer2.InterruptCount, scale_p ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "offset = offset = ( meter_value_01 / scale_p ) - digital_value_01\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Offset: %e\n", CpuTimer2.InterruptCount, offset ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "scale_n = meter_value_00 / ( digital_value_00 + offset )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "scale_n: %e\n", CpuTimer2.InterruptCount, scale_n ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEVELOP_SV72
            #else
            /*---------------------------------------------------------------*/
            //
            // 判斷校正值是否合理
            //
            level01 = ADC_TO_I_FB_OFFSET_5151;
            level02 = ADC_TO_I_FB_SCALE_P_5151;

            if ( ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all > ( level01 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all < ( level01 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all < ( level02 * 0.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all > ( level02 * 1.5 ) )
                 || ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all < ( level02 * 0.5 ) ) )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_ADC_to_I_FB_OVERRANGE;

                //
                // 轉態
                //
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "***CALI_ADC_to_V_FB_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 強制轉態
                //
                break;
            }
            #endif

            Calibration_Module_Variables.Calibration_process_state = Calibration_Rs_Cs_state_hr; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case Calibration_Rs_Cs_state_hr:
        {

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Rs_Cs_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 估算Rs時取的範圍是+Max到-Max
            // 計算電流斜率Scal把正跟負取平均
            scale = 0.5 * ( Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all + Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all );

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "scale = ( ADC_to_I_FB_Scale_P + ADC_to_I_FB_Scale_N ) / 2\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "scale: %e\n", CpuTimer2.InterruptCount, scale ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            /*---------------------------------------------------------------*/
            //
            // 計算Rs
            //

            //step 3: -Max
            meter_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][3].all * 0.001;
            digital_value_00 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_VOLTGE][3];

            //Step 4: +Max
            meter_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[CALI_VOLTGE][4].all * 0.001;
            digital_value_01 = (float)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[CALI_VOLTGE][4];


            Rs = ( meter_value_01 - meter_value_00 ) / ( ( digital_value_01 - digital_value_00 ) * scale );
            inv_Rs = 1 / Rs;

            Global_Variables.Calibration_Variables.inv_Rs.all = inv_Rs;
            CPU_to_CLA_Variables.inv_Rs = Global_Variables.Calibration_Variables.inv_Rs.all;

            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].inv_Rs.all = Global_Variables.Calibration_Variables.inv_Rs.all;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_00: %e\n", CpuTimer2.InterruptCount, meter_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Meter_Value_01: %e\n", CpuTimer2.InterruptCount, meter_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_00: %e\n", CpuTimer2.InterruptCount, digital_value_00 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Digital_Value_01: %e\n", CpuTimer2.InterruptCount, digital_value_01 ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Rs = ( meter_value_01 - meter_value_00 ) / ( ( digital_value_01 - digital_value_00 ) * scale )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Rs: %e\n", CpuTimer2.InterruptCount, Rs ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "inv_Rs: %e\n", CpuTimer2.InterruptCount, inv_Rs ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            /*---------------------------------------------------------------*/
            //
            // 計算電容Cs
            //

            Zs = Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][1] / ( Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][1] * scale );
            Xc = ( Rs * Zs ) / ( Rs - Zs );
            Cs = 1 / ( 2.0 * M_PI * CALIBRATION_FREQ_FSET_MID * 0.01 * Xc );

            Global_Variables.Calibration_Variables.Cs_mpy_360000.all = Cs * 360000.0;
            CPU_to_CLA_Variables.Cs_mpy_360000 = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;

            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Cs_mpy_360000.all = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "scale: %e\n", CpuTimer2.InterruptCount, scale ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Vrms_Value[LOW_RANGE][1]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Vrms_Value[HIGH_RANGE][1] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Irms_no_Scale_Value[LOW_RANGE][1]: %e\n", CpuTimer2.InterruptCount, Calibration_Module_Variables.Calibration_process_Variables.Irms_no_Scale_Value[HIGH_RANGE][1] ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Zs = Vrms_Value[1] / ( Irms_no_Scale_Value[1] * scale )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Zs: %e\n", CpuTimer2.InterruptCount, Zs ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Xc = ( Rs * Zs ) / ( Rs - Zs )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Xc: %e\n", CpuTimer2.InterruptCount, Xc ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

            sprintf( SCI_Module_Variables.Debug_Msg, "Cs = 1 / ( 2.0 * 3.14159 * CALIBRATION_FREQ_FSET_MID * 0.01 * Xc )\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "Cs: %e\n", CpuTimer2.InterruptCount, Cs ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            #ifdef DEVELOP_SV72
            #else
            /*---------------------------------------------------------------*/
            //
            // 判斷估算出來的Cs及Rs是否合理
            //
            if ( ( Cs * 360000.0 > CS_MPY_360000_MAX ) || ( inv_Rs > INV_RS_MAX ) )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_CS_RS_OVERRANGE;

                //
                // 轉態
                //
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "***CALI_CS_RS_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 強制轉態
                //
                break;
            }
            #endif

            Calibration_Module_Variables.Calibration_process_state = Calibration_Irms_offset_step0_state_hr; //轉態
            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // 自動校正 Irms offset Step 0
        // Freq set = CALIBRATION__IRMS_OFFSET_FSET
        // ACV set = 0
        //
        case Calibration_Irms_offset_step0_state_hr:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Irms_offset_step0_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 設定Freq, ACV...等,準備輸出
            //

            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = CALIBRATION__IRMS_OFFSET_FSET;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = 0;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = 0;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;
            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            //開始計時
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;

            /*---------------------------------------------------------------*/
            //
            // 輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
            Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: ON\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Calibration_Module_Variables.Calibration_process_state = Calibration_Irms_offset_step1_state_hr; //轉態
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 自動校正 Irms offset Step 1
        // 更新offset
        //
        case Calibration_Irms_offset_step1_state_hr:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 機器Busy
                //

                break;
            }

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {
                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Irms_offset_step1_state_hr\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                /*---------------------------------------------------------------*/
                //
                // 計算電流rms的offset ( F = 50Hz / ACV = 0V )
                // 要疊代計算
                //
                Global_Variables.Calibration_Variables.I_rms_Offset.all = -Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all + Global_Variables.Calibration_Variables.I_rms_Offset.all;

                Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].I_rms_Offset.all = Global_Variables.Calibration_Variables.I_rms_Offset.all;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "I_rms_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.I_rms_Offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                /*---------------------------------------------------------------*/
                //
                // 停止輸出
                //
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif



                /*---------------------------------------------------------------*/

                //把目前校正參數都印出來

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_I_FB_Scale_P: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_I_FB_Scale_N: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_I_FB_Offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "inv_Rs: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.inv_Rs.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "Cs_mpy_360000: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.Cs_mpy_360000.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles

                sprintf( SCI_Module_Variables.Debug_Msg, "I_rms_Offset: %e\n", CpuTimer2.InterruptCount , Global_Variables.Calibration_Variables.I_rms_Offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif



                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 觸發校正Checksum計算事件
                //
                Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                Global_Variables.Calibration_State_Flag.all = 1; //finish


                //
                // 校正結束
                // 恢復補償及保護
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 1;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 10500.0 );

                Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
                Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

                //
                // 啟用優化
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 1;

                //
                // CLA量測ADC OFF
                //
                CPU_to_CLA_Variables.Cla_Control_Flag.bit.Cali_Process = 0;

            }

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        // 校正Ext Offset step 0
        // 設定ACDC-INT
        //
        case Calibration_ExtAD_offset_step0_state:
        {
            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_ExtAD_offset_step0_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*---------------------------------------------------------------*/
            //
            // 停止輸出
            //
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "Output: OFF\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_EXT_MODE;

            //開始計時
            Calibration_Module_Variables.Calibration_AutoStart_delay_time = CpuTimer2.InterruptCount;


            Calibration_Module_Variables.Calibration_process_state = Calibration_ExtAD_offset_step1_state; //轉態

            break;

        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 校正Ext Offset Step 1
        // 初始化ExtAD累加變數
        //
        case Calibration_ExtAD_offset_step1_state:
        {
            if ( Main_State_Module.Machine_state != READY_STATE )
            {
                break;
            }

            //
            // 回到READY
            //

            if ( Calc_Elapsed_Time( Calibration_Module_Variables.Calibration_AutoStart_delay_time, CpuTimer2.InterruptCount ) > CALI_DELAY_TIME )
            {

                //等穩定後再開始累加ExtAD

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_ExtAD_offset_step1_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                Calibration_Module_Variables.Calibration_process_Variables.ExtAD_sum = 0;

                Calibration_Module_Variables.Calibration_process_Variables.ExtAD_Total_Sample_Points = 100.0;

                Calibration_Module_Variables.Calibration_process_state = Calibration_ExtAD_offset_step2_state; //轉態


                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_ExtAD_offset_step2_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 校正Ext Offset Step 2
        // 累加ExtAD
        // 累加次數達設定值後, 更新offset
        //
        case Calibration_ExtAD_offset_step2_state:
        {
            Calibration_Module_Variables.Calibration_process_Variables.ExtAD_sum = Calibration_Module_Variables.Calibration_process_Variables.ExtAD_sum + (double)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN;

            Calibration_Module_Variables.Calibration_process_Variables.ExtAD_Total_Sample_Points--;



            if ( Calibration_Module_Variables.Calibration_process_Variables.ExtAD_Total_Sample_Points <= 0 )
            {
                Calibration_Module_Variables.Calibration_process_Variables.ExtAD_Total_Sample_Points = 100.0;
                Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all = (float)(Calibration_Module_Variables.Calibration_process_Variables.ExtAD_sum / Calibration_Module_Variables.Calibration_process_Variables.ExtAD_Total_Sample_Points);

                Calibration_Module_Variables.Calibration_process_Variables.ExtAD_sum = 0;

                Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.all = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
                Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.all = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;

                //
                #ifdef DEVELOP_SV72
                #else
                /*---------------------------------------------------------------*/
                //
                // 判斷ADC_to_EXTIN_L1_offset是否合理
                //
                if ( ( Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all > ADC_to_EXTIN_L1_OFFSET_MAX )
                     || ( Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all < ADC_to_EXTIN_L1_OFFSET_MIN ) )
                {
                    Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                    Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CALI_ADC_to_EXTIN_L1_offset_OVERRANGE;

                    //
                    // 轉態
                    //
                    Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                    error_start_time_reset();

                    //debug msg
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "***CALI_ADC_to_EXTIN_L1_offset_OVERRANGE***\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif
                }
                #endif

                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "ADC_to_EXTIN_L1_offset: %e\n", CpuTimer2.InterruptCount, Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                //debug msg
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "-----------------------------------------\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                sprintf( SCI_Module_Variables.Debug_Msg, "-> Calibration_Ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 觸發校正Checksum計算事件
                //
                Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

                Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state; //轉態

                Global_Variables.Calibration_State_Flag.all = 1; //finish

                //
                // 校正結束
                // 恢復補償及保護
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 1;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power;
                Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 10500.0 );

                Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
                Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

                //
                // 啟用優化
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 1;

                //
                // CLA量測ADC OFF
                //
                CPU_to_CLA_Variables.Cla_Control_Flag.bit.Cali_Process = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;


                break;
            }

            break;
        }

    }
}






//
// End of file
//
