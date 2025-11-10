//###########################################################################
//
// FILE:    Global_VariableDefs.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210819
//###########################################################################



#include "Global_VariableDefs.h"

volatile GLOBAL_VARIABLE_REG Global_Variables;




/////////////////////////////////////////////////////////////////////
//
//   最基本讓DSP不會出錯的參數,只在程式開始時執行一次
//
void Init_Global_Variables(void)
{
    Uint16 i = 0;

    //----------------------------------------------------
    Global_Variables.EXT_INPUT_FLAG.all = 0;

    //----------------------------------------------------
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN = 0;

    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN_TTL_Level_L = 0;
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN_TTL_Level_H = 0;

    Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all = 0;
    Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq_shadow = 0;

    Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set = 0;
    Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set_shadow = 0;

    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level = 0;








    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_LPF_Out = 0;

    //----------------------------------------------------
    Global_Variables.EXT_OUTPUT_FLAG.all = 0;

    //----------------------------------------------------
    Global_Variables.INPUT_FLAGS.all = 0;

    //----------------------------------------------------
    Global_Variables.INPUT_VARIABLES.I_FB = 0;
    Global_Variables.INPUT_VARIABLES.Temp_Sensor = 0;
    Global_Variables.INPUT_VARIABLES.V_FB = 0;

    //----------------------------------------------------

    //
    // 預設ASR-5075
    // ASR5000系列： 0為 ASR5075、1為 ASR5150、2為ASR5151
    //
    #ifdef DEFAULT_ASR_5075
    Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series = ASR_5075;
    #endif

    #ifdef DEFAULT_ASR_5150
    Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series = ASR_5150;
    #endif


    //
    // 機器參數
    //
    switch ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series )
    {
        case ASR_5075:
        {
            Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout = ASR_5075_MAX_AC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout = ASR_5075_MAX_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout = ASR_5075_MIN_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout = ASR_5075_MAX_I;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak = ASR_5075_MAX_I_PEAK;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power = ASR_5075_MAX_P;

            break;
        }
        case ASR_5150:
        {
            Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout = ASR_5150_LR_MAX_AC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout = ASR_5150_LR_MAX_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout = ASR_5150_LR_MIN_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout = ASR_5150_LR_MAX_I;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak = ASR_5150_LR_MAX_I_PEAK;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power = ASR_5150_LR_MAX_P;

            break;
        }
    }

    //----------------------------------------------------
    Global_Variables.OUTPUT_FLAGS.all = 0;

    //----------------------------------------------------
    Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
    Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

    Global_Variables.OUTPUT_VARIABLES.WAVE_set = WAVE_DAC_ZERO;
    Global_Variables.OUTPUT_VARIABLES.WAVE_offset = 0;
    Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = 0;
    Global_Variables.OUTPUT_VARIABLES.WAVE_scale = 1;
    Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = 1;

    Global_Variables.OUTPUT_VARIABLES.WAVE_set_diff_level = 100.0;

    Global_Variables.OUTPUT_VARIABLES.ext_WAVE_scale = 1;
    Global_Variables.OUTPUT_VARIABLES.ext_WAVE_scale_shadow = 1;


    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = WAVE_DAC_ZERO;

    Global_Variables.OUTPUT_VARIABLES.n_IREF_set = 0;
    Global_Variables.OUTPUT_VARIABLES.p_IREF_set = 0;

    //----------------------------------------------------
    Global_Variables.POWER_CONTROL.all = 0;

    //----------------------------------------------------
    Global_Variables.POWER_DISPLAY_VARIABLES.FREQ_display.all = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display_shadow = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display.all = 0;
    //Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display_shadow = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display_shadow = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout;

    Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display.all = 0;
    //Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display_shadow = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display_shadow = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout;


    Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display_shadow = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display.all = 0;
    //Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display_shadow = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display_shadow = -Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak;

    Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display.all = 0;
    //Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display_shadow = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display_shadow = Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak;

    Global_Variables.POWER_DISPLAY_VARIABLES.P_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow.all = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.S_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.S_display_shadow = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.Q_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.Q_display_shadow = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.PF_display = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.CF_display = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.V_peak_hold_display.all = 0;

    // 20220719
    // 配合面板需求, 初始改成1
    Global_Variables.POWER_DISPLAY_VARIABLES.VI_Raw_Data_Counter = 0xA5A5;

    Global_Variables.POWER_DISPLAY_VARIABLES.Short_OCP_V_FB.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.Short_OCP_c2 = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.Short_OCP_VREF_set = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.Short_OCP_WAVE_set = 0;

    Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = 0;

    //----------------------------------------------------
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.all = 0;

    //預設開啟I rms Limit
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable = 1;

    //預設開啟I peak Limit
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable = 1;

    //----------------------------------------------------
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.all = 0;

    //預設開啟頻率補償
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable = 1;


    //預設穩態補償 ON
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 1;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

    //
    // 預設Output Relay Enable ON
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Output_Relay_Enable = 1;

    //
    // 預設0度改變 Enable
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Zero_Change_Phase_Enable = 1;

    //Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.WAVE_DAC_Slew_Rate_Enable = 1;
    //Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.VREF_DAC_Slew_Rate_Enable = 1;

    #ifdef DEVELOP_SV41
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Balance_Compensate_Enable = 0;
    #else
    // 預設開啟MS均流補償
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Balance_Compensate_Enable = 1;
    #endif


    #ifdef DEVELOP_SV42
    //
    // 預設本機為Master
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Master = 1;
    #else
    //
    // 預設本機為Slave
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Master = 0;
    #endif

    //
    // 預設啟用優化
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 1;

    //----------------------------------------------------
    Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all = 0;
    Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all = 0;

    Global_Variables.POWER_FUNCTIONS_ERROR_CODE = NO_ERROR_CODE;

    //----------------------------------------------------
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_max = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_min = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_LSB_buf = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_max = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_min = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_LSB_buf = 0;

    //
    // 預設指數逼近模式, 100us (實測)
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.V_SlewRate_Mode = SOFT_RAMP;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user = 100;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user_shadow = 100;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SoftRamp_Step_set = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step = 1000.0;

    //Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Soft_Ramp_Coeff = 3.0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Soft_Ramp_Coeff = 2.4;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all = 100; //至少1Hz
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_shadow = 100;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_LSB_buf = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user_shadow = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user_shadow = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_phase_count = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user_shadow = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_Count = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Zero_phase_count = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.all = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user_max = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user_min = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user_LSB_buf = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user_shadow = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = WAVE_SIN;   //預設sin
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = WAVE_SIN;   //預設sin

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.I_peak_hold_start_time = 1;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_peak_hold_start_time = 1;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.all = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user_max = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user_min = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user_LSB_buf = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user_shadow = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user.all = 0;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user_LSB_buf = 0;

    for ( i = 0; i < 20; i++ )
    {
        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[i].all = 0;

        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[i] = 0;
    }

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page = 0;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FanSpeed_set_user = 1;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = LOW_RANGE;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_LOW_RANGE;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_LOW_RANGE;

    //----------------------------------------------------
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_ocp_start_time = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_ocp_start_time = 0;

    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_short_delay_time = 0;
    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_short_level = 0;
    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_short_ocp_start_time = 0;

    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_short_delay_time = 0;
    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_short_level = 0;
    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_short_ocp_start_time = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2 = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_ocp_start_time = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_limit_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_limit_level = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES._4CF_OCP_level = 0;

//    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_V_peak_limit_delay_time = 0;
//    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_V_peak_limit_level = 0;

//    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_V_peak_limit_delay_time = 0;
//    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_V_peak_limit_level = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.V_rms_limit_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.V_rms_limit_level = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.V_rms_ovp_start_time = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.OPP_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.OPP_level = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.opp_start_time = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.remote_OVP_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.remote_OVP_level = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.remote_OVP_start_time = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL1_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL2_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OTP_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.ACFAIL_delay_time = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL_delay_time = 0; //for 全寬機種
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP1_delay_time = 0; //for 全寬機種
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP1_delay_time = 0; //for 全寬機種
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP2_delay_time = 0; //for 全寬機種
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP2_delay_time = 0; //for 全寬機種

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DC_OVP_delay_time = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DCAC_OTP_delay_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DCAC_OTP_level = 0;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL1_start_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL2_start_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OTP_start_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP_start_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP_start_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.ACFAIL_start_time = 0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DCAC_OTP_start_time = 0;


    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL_start_time = 0; //for 全寬機種
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP1_start_time = 0; //for 全寬機種
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP1_start_time = 0; //for 全寬機種
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP2_start_time = 0; //for 全寬機種
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP2_start_time = 0; //for 全寬機種

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DC_OVP_start_time = 0;

    //----------------------------------------------------
    Global_Variables.POWER_STATUS.all = 0;

    //----------------------------------------------------
    Global_Variables.VI_RAW_OP_CODE.all = 0;

    Global_Variables.Pre_Calc_Variables.inv_Wave_TRI_CF_set_user = 0;

    Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale = 0;

    Global_Variables.Pre_Calc_Variables.inv_Vmax = 1.0 / ( Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 1.01 );


    //----------------------------------------------------

    Global_Variables.Calibration_Variables.CheckSum = 0;

    Global_Variables.Calibration_Variables.Data_Version = CALI_DATA_VER;

    //Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale.all = 0;
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all = 0;

    //Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale.all = 0;
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = 0;

    Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all = 0;
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all = 0;
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all = 0;
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all = 0;


    Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all = 0;
    Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all = 0;

    Global_Variables.Calibration_Variables.IREF_to_DAC_Scale.all = 0;
    Global_Variables.Calibration_Variables.IREF_to_DAC_Offset.all = 0;


    Global_Variables.Calibration_Step_Flag.all = 0;


    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all = 0;
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all = 0;
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all = 0;


    Global_Variables.Calibration_Variables.Cs_mpy_360000.all = 0;
    Global_Variables.Calibration_Variables.inv_Rs.all = 0;


    Global_Variables.Calibration_Variables.V_rms_Offset.all = 0;
    Global_Variables.Calibration_Variables.I_rms_Offset.all = 0;
    Global_Variables.Calibration_Variables.p_V_peak_Offset.all = 0;
    Global_Variables.Calibration_Variables.n_V_peak_Offset.all = 0;
    Global_Variables.Calibration_Variables.p_I_peak_Offset.all = 0;
    Global_Variables.Calibration_Variables.n_I_peak_Offset.all = 0;


    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all = 0;
    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all = 0;


    Global_Variables.Calibration_State_Flag.all = 0x01; //預設完成

    //----------------------------------------------------
    Global_Variables.OUTPUT_MODE_set_user = ACDC_INT_MODE;
    Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

    Global_Variables.OUTPUT_MODE_FLAG_Shadow.all = Global_Variables.OUTPUT_MODE_FLAG.all;


    //----------------------------------------------------
    Global_Variables.StartUp_State_Flag.all = 0;

    Global_Variables.CPU_State_Flag.all = 0;


    //----------------------------------------------------

    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = 0;

    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Step_Time_set_user_LSB_buf = 0;

    Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

    Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user.all = SEQ_ACDC_MODE;


    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Freq_set_user_LSB_buf = 0;

    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_DCV_set_user_LSB_buf = 0;

    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_ACV_set_user_LSB_buf = 0;

    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum = 0;

    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Start_Phase_Count = 0;
    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Stop_Phase_Count = 0;


    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Width_Set = 1;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Option.all = 0;

    //
    // 方波Duty, 預設2048 -> 50%
    //
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user = 2048;

    #ifdef DEVELOP_SV43
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable = 1; // 預設啟用MS_Enable
    Global_Variables.MS_Function_Variables.MS_Output_Phase = _1P2W_2UNITS; // 預設 1P2W
    #else
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable = 0; // 預設啟用MS_Enable
    Global_Variables.MS_Function_Variables.MS_Output_Phase = _1P2W_2UNITS; // 預設 1P2W
    #endif

    Global_Variables.MS_Function_Variables.MS_Address = 0xFF;
    Global_Variables.MS_Function_Variables.MS_Total_Units = 0;

    Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[0] = 0;
    Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[1] = 1200;
    Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[2] = 2400;
    //Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set = 0;

    Global_Variables.MS_Function_Variables.MS_1P3W_Phase_set_user[0] = 0;
    Global_Variables.MS_Function_Variables.MS_1P3W_Phase_set_user[1] = 1800;
    //Global_Variables.MS_Function_Variables.MS_1P3W_Phase_set = 0;

    Global_Variables.MS_Function_Variables.MS_Output_Phase_Delay_Count = 0;


    //Global_Variables.MS_Function_Variables.MS_L1_POWER_FUNCTIONS_ERROR_FLAGS.all = 0;
    //Global_Variables.MS_Function_Variables.MS_L1_POWER_FUNCTIONS_ERROR_FLAGS2.all = 0;

    //Global_Variables.MS_Function_Variables.MS_L2_POWER_FUNCTIONS_ERROR_FLAGS.all = 0;
    //Global_Variables.MS_Function_Variables.MS_L2_POWER_FUNCTIONS_ERROR_FLAGS2.all = 0;

    //Global_Variables.MS_Function_Variables.MS_L3_POWER_FUNCTIONS_ERROR_FLAGS.all = 0;
    //Global_Variables.MS_Function_Variables.MS_L3_POWER_FUNCTIONS_ERROR_FLAGS2.all = 0;

    //Global_Variables.MS_Function_Variables.MS_L1_Power_Status.all = 0;
    //Global_Variables.MS_Function_Variables.MS_L2_Power_Status.all = 0;
    //Global_Variables.MS_Function_Variables.MS_L3_Power_Status.all = 0;

    for ( i = 0; i < 3; i ++ )
    {
        Global_Variables.MS_Function_Variables.MS_Power_Status[i].all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_FUNCTIONS_ERROR_FLAGS[i].all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_FUNCTIONS_ERROR_FLAGS2[i].all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].AC_V_rms_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].AC_V_rms_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].CF_display = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].FREQ_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].I_avg_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].I_avg_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].I_peak_hold_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].I_rms_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].I_rms_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].PF_display = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].P_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].P_display_shadow.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].p_I_peak_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].p_I_peak_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].p_V_peak_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].p_V_peak_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].Q_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].Q_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].S_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].S_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].Short_OCP_VREF_set = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].Short_OCP_V_FB.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].Short_OCP_WAVE_set = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].Short_OCP_c2 = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].Temp_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].VI_Raw_Data_Counter = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].V_avg_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].V_avg_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].V_peak_hold_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].V_rms_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].V_rms_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].n_I_peak_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].n_I_peak_display_shadow = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].n_V_peak_display.all = 0;
        Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[i].n_V_peak_display_shadow = 0;

        Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[i].all = 0;
    }

    //Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags.all = 0;
    //Global_Variables.MS_Function_Variables.MS_L2_Sync_Status_Flags.all = 0;
    //Global_Variables.MS_Function_Variables.MS_L3_Sync_Status_Flags.all = 0;

    Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = 0;

    Global_Variables.MS_Function_Variables.Master_IFB = 0;

    Global_Variables.dds.all = 0;
    Global_Variables.FreqStep = 0;

}







//
// End of file
//
