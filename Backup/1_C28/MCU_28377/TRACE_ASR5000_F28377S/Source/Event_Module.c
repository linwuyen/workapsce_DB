//###########################################################################
//
// FILE:    Event_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211105
//###########################################################################



#include "Global_VariableDefs.h"

volatile EVENT_MODULE_VARIABLES_REG Event_Module;






/////////////////////////////////////////////////////////////////////
//
// 模組初始化
//
void Init_Event_Module_Variables(void)
{
    Event_Module.Event_Flag.all = 0;

    Event_Module.Event2_Flag.all = 0;
    Event_Module.Event3_Flag.all = 0;
    Event_Module.Event4_Flag.all = 0;

    Event_Module.Debug_Event_Flag.all = 0;
}


/////////////////////////////////////////////////////////////////////
//
// Event掃描
//
void Event_Scan(void)
{
    //
    // 採用if else寫法, 每次處理一個event
    // 由上而下順位
    //

    /*-----------------------------------------------------------------------------------------*/
    //
    // 從EEPROM載出變數
    //
    if ( Event_Module.Event_Flag.bit.Load_Cali_Variable_from_EEPROM )
    {
        Load_Cali_Variable_from_EEPROM_Event();

        Event_Module.Event_Flag.bit.Load_Cali_Variable_from_EEPROM = 0;

        //觸發校正Checksum計算事件
        Event_Module.Event2_Flag.bit.Update_Cali_CheckSum = 1;

    }
    /*-----------------------------------------------------------------------------------------*/
    //
    // 寫入校正到EEPROM
    //
    else if ( Event_Module.Event_Flag.bit.Save_Cali_Variable_to_EEPROM )
    {
        Save_Cali_Variable_to_EEPROM_Event();

        Event_Module.Event_Flag.bit.Save_Cali_Variable_to_EEPROM = 0;

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

    /*-----------------------------------------------------------------------------------------*/
    //
    // +IREF
    //
    else if ( Event_Module.Event_Flag.bit.Set_DAC7612_CHA )
    {
        Set_DAC7612_CHA_Event();

        Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 0;
    }
    /*-----------------------------------------------------------------------------------------*/
    //
    // -IREF
    //
    else if ( Event_Module.Event_Flag.bit.Set_DAC7612_CHB )
    {
        Set_DAC7612_CHB_Event();

        Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 改機種規格
    //
    else if ( Event_Module.Event_Flag.bit.Model_Type_Change )
    {
        if ( Main_State_Module.Machine_state == READY_STATE )
        {
            //
            // Ready才能改機種
            //

            Model_Type_Change_Event();
            Load_HL_Cali_Variable_from_EEPROM_Event();
            Load_HL_Cali_to_Cali_Event();

            //
            // 觸發校正Checksum計算事件
            //
            Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

            Event_Module.Event_Flag.bit.Model_Type_Change = 0;
        }
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 改變設定VREF DAC Limit事件
    //
    else if ( Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit )
    {
        Calc_VREFDAC_Limit_Event();

        Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit = 0;
    }



    /*-----------------------------------------------------------------------------------------*/
    //
    // 改變設定SlewRate事件
    //
    else if ( Event_Module.Event_Flag.bit.Calc_V_SlewRate_set )
    {
        Calc_V_SlewRate_set_Event();

        Event_Module.Event_Flag.bit.Calc_V_SlewRate_set = 0;
    }


    /*-----------------------------------------------------------------------------------------*/
    //
    // 改變設定Start phase事件
    //
    else if ( Event_Module.Event_Flag.bit.Set_Start_phase )
    {
        Set_Start_phase_Event();

        Event_Module.Event_Flag.bit.Set_Start_phase = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 改變設定Stop phase事件
    //
    else if ( Event_Module.Event_Flag.bit.Set_Stop_phase )
    {
        Set_Stop_phase_Event();

        Event_Module.Event_Flag.bit.Set_Stop_phase = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 還原校正值
    //
    else if ( Event_Module.Event_Flag.bit.Set_Cali_Variable_default )
    {

        Set_V_HL_Cali_Variable_default_Event();
        Set_I_HL_Cali_Variable_default_Event();
        Set_EXT_HL_Cali_Variable_default_Event();
        Load_HL_Cali_to_Cali_Event();
        Save_HL_Cali_Variable_to_EEPROM_Event();


        //
        // 觸發校正Checksum計算事件
        //
        Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;



        Event_Module.Event_Flag.bit.Set_Cali_Variable_default = 0;

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

    /*-----------------------------------------------------------------------------------------*/
    //
    // 設定校正Step
    //
    else if ( Event_Module.Event_Flag.bit.Set_Cali_Step )
    {
        Set_Cali_Step_Event_6step();

        Event_Module.Event_Flag.bit.Set_Cali_Step = 0;

        //
        // 校正開始
        // 關閉補償及保護重設
        //
        if ( Global_Variables.Calibration_Step_Flag.bit.Item == CALI_VOLTGE )
        {
            //
            // 電壓校正關閉補償
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = 0;
        }

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

    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 寫入校正量測值
    //
    else if ( Event_Module.Event_Flag.bit.Set_Cali_Meter_Value )
    {
        Set_Cali_Meter_Value_Event();
        Event_Module.Event_Flag.bit.Set_Cali_Meter_Value = 0;

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

    /*-----------------------------------------------------------------------------------------*/
    //
    // 取消校正
    //
    else if ( Event_Module.Event_Flag.bit.Cancel_Cali_Process )
    {
        Cancel_Cali_Process_Event();
        Event_Module.Event_Flag.bit.Cancel_Cali_Process = 0;

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


    /*-----------------------------------------------------------------------------------------*/
    //
    // Reload WAVE CF
    //
    else if ( Event_Module.Event_Flag.bit.Reload_Wave_CF_set )
    {
        Reload_Wave_CF_set_Event();
        Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 0;

    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // SCIA echo
    //
    else if ( Event_Module.Event_Flag.bit.SCIA_echo )
    {
        SCIA_echo_Event();

        Event_Module.Event_Flag.bit.SCIA_echo = 0;
    }


    /*-----------------------------------------------------------------------------------------*/
    //
    // SYNC改變設定FREQ事件
    //
    /*
    else if ( Event_Module.Event2_Flag.bit.SYNC_Mode_Change_FREQ )
    {
        SYNC_Mode_Change_FREQ_Event();

        Event_Module.Event2_Flag.bit.SYNC_Mode_Change_FREQ = 0;

    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // SYNC Mode改變設定Phase Delay事件
    //
    else if ( Event_Module.Event2_Flag.bit.SYNC_Mode_Change_Phase_Delay )
    {
        SYNC_Mode_Change_Phase_Delay_Event();

        Event_Module.Event2_Flag.bit.SYNC_Mode_Change_Phase_Delay = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 預先載入SQU WAVE_DATA事件
    //
    else if ( Event_Module.Event2_Flag.bit.Preload_SQU_WAVE_DATA )
    {
        //
        // 根據Duty產生SQU WAVE data, 並存入SDRAM
        //
        Generate_Square_wave_data_process();

        Preload_SQU_WAVE_DATA_Event();

        Event_Module.Event2_Flag.bit.Preload_SQU_WAVE_DATA = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 預先載入WAVE_DATA事件
    //
    else if ( Event_Module.Event2_Flag.bit.Preload_WAVE_DATA )
    {
        Preload_WAVE_DATA_Event();

        Event_Module.Event2_Flag.bit.Preload_WAVE_DATA = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // SCI A 接收處理
    //
    else if ( Event_Module.Event2_Flag.bit.SCIA_ask_service )
    {
        SCIA_ask_service_Event();
        Event_Module.Event2_Flag.bit.SCIA_ask_service = 0;

    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // Trig V/I RAW事件
    //

    else if ( Event_Module.Event2_Flag.bit.VI_raw_data_trig )
    {
        //
        // 設定Meas_sampling_state為meas_sampling_ready_state的動作一定要在設定VI_RAW_OP_CODE_Trig為1之前
        // 這是由於主中斷360kHz有機會會隨時中斷執行
        // 如果先設定VI_RAW_OP_CODE_Trig後進入中斷, 擷取RAW狀態機如果本身剛好在ready狀態則轉態開始同步及擷取
        // 然後再去執行設置在meas_sampling_ready_state, 將強制擷取狀態又回到ready並且不再被trig而卡住
        //

        Meas_Module.Meas_sampling_state = meas_sampling_ready_state;

        Meas_Module.Flags.bit.VI_RAW_OP_CODE_Trig = 1;

        Event_Module.Event2_Flag.bit.VI_raw_data_trig = 0;

    }





    /*-----------------------------------------------------------------------------------------*/
    //
    // 計算更新校正Checksum事件
    //
    else if ( Event_Module.Event2_Flag.bit.Update_Cali_CheckSum )
    {

        Update_Cali_CheckSum_Event();

        Event_Module.Event2_Flag.bit.Update_Cali_CheckSum = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 處理觸發Value_Set_User_Change的事件
    //
    // ***確認Event3裡面的事件都執行後才觸發***
    // ***放置到Event掃描的最後順位***
    //
    /*
    else if ( Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change )
    {
        if ( Event_Module.Event3_Flag.all == 0 )
        {
            Trig_Value_Set_User_Change_Event();

            Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change = 0;
        }
    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // 處理SEQ Step Change的事件
    // 為加快速度, 直接放到main.c主迴圈內
    //
    /*
    else if ( Event_Module.Event2_Flag.bit.SEQ_Mode_Change_Step )
    {
        SEQ_Mode_Change_Step_Event();

        Event_Module.Event2_Flag.bit.SEQ_Mode_Change_Step = 0;
    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // SYNC Mode 計算 Zero Phase Count
    //
    /*
    else if ( Event_Module.Event2_Flag.bit.SYNC_Mode_Calc_Zero_Phase_Count )
    {
        SYNC_Mode_Calc_Zero_Phase_Count_Event();

        Event_Module.Event2_Flag.bit.SYNC_Mode_Calc_Zero_Phase_Count = 0;
    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // SEQ Mode 清掉Jump Count
    //
    else if ( Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_Jump_Count )
    {
        SEQ_Mode_Clear_Jump_Count_Event();

        Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_Jump_Count = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // SEQ Mode 初始化Step
    //
    else if ( Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_All_Step )
    {
        SEQ_Mode_Clear_All_Step_Event();

        Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_All_Step = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // SEQ Mode 清掉CheckSum
    //
    else if ( Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_CheckSum )
    {
        SEQ_Mode_Clear_CheckSum_Event();

        Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_CheckSum = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 寫入校正值CheckSum檢查事件
    //
    else if ( Event_Module.Event2_Flag.bit.Check_Cali_CheckSum )
    {
        Check_Cali_CheckSum_Event();

        Event_Module.Event2_Flag.bit.Check_Cali_CheckSum = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 從EEPROM載入HR,LR校正值
    //
    else if ( Event_Module.Event4_Flag.bit.Load_HL_Cali_Variable_from_EEPROM )
    {
        Load_HL_Cali_Variable_from_EEPROM_Event();
        Load_HL_Cali_to_Cali_Event();

        //
        // 觸發校正Checksum計算事件
        //
        Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

        Event_Module.Event4_Flag.bit.Load_HL_Cali_Variable_from_EEPROM = 0;

    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 計算更新校正值HL Checksum事件
    //
    else if ( Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum )
    {

        Update_HL_Cali_CheckSum_Event();

        Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 判斷H/L Range把校正值shadow載入到實際使用的校正值
    //
    else if ( Event_Module.Event4_Flag.bit.Load_HL_Cali_to_Cali )
    {

        Load_HL_Cali_to_Cali_Event();

        Event_Module.Event4_Flag.bit.Load_HL_Cali_to_Cali = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 判斷設定Output Range
    //
    else if ( Event_Module.Event4_Flag.bit.Set_Output_Range_Option )
    {

        Set_Output_Range_Option_Event();

        Event_Module.Event4_Flag.bit.Set_Output_Range_Option = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // Output Range改變
    //
    /*
    else if ( Event_Module.Event4_Flag.bit.Change_Output_Range )
    {

        Change_Output_Range_Event();

        Event_Module.Event4_Flag.bit.Change_Output_Range = 0;
    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // 判斷Output Range
    //
    /*
    else if ( Event_Module.Event4_Flag.bit.Adj_Output_Range )
    {

        Adj_Output_Range_Event();

        Event_Module.Event4_Flag.bit.Adj_Output_Range = 0;
    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // HR,LR校正值存入EEPROM
    //
    else if ( Event_Module.Event4_Flag.bit.Save_HL_Cali_Variable_to_EEPROM )
    {
        Save_HL_Cali_Variable_to_EEPROM_Event();

        Event_Module.Event4_Flag.bit.Save_HL_Cali_Variable_to_EEPROM = 0;

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


    /*-----------------------------------------------------------------------------------------*/
    //
    // Refresh SEQ CheckSum
    //
    else if ( Event_Module.Event4_Flag.bit.SEQ_Mode_Refresh_CheckSum )
    {

        #ifndef DEVELOP_SV78
        SEQ_Mode_Refresh_CheckSum_Event();
        #endif

        Event_Module.Event4_Flag.bit.SEQ_Mode_Refresh_CheckSum = 0;
    }



    /*-----------------------------------------------------------------------------------------*/
    //
    // 改變設定ACDCV電壓事件
    //
    /*
    else if ( Event_Module.Event3_Flag.bit.Change_ACV_DCV )
    {
        Change_ACV_DCV_Event();

        Event_Module.Event3_Flag.bit.Change_ACV_DCV = 0;

        //cpu_timer_module.Event_Trig_Flag.bit.Value_Set_User_Change = 1;

    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // 改變設定DCV電壓事件
    //
    /*
    else if ( Event_Module.Event3_Flag.bit.Change_DCV )
    {
        Change_DCV_Event();

        Event_Module.Event3_Flag.bit.Change_DCV = 0;

        //cpu_timer_module.Event_Trig_Flag.bit.Value_Set_User_Change = 1;

    }
    */
    /*-----------------------------------------------------------------------------------------*/
    //
    // EXT AD改變設定ACV電壓事件
    //
    /*
    else if ( Event_Module.Event3_Flag.bit.VCA_Mode_Change_ACV )
    {
        VCA_Mode_Change_ACV_Event();

        Event_Module.Event3_Flag.bit.VCA_Mode_Change_ACV = 0;

        //cpu_timer_module.Event_Trig_Flag.bit.Value_Set_User_Change = 1;

    }
    */


    /*-----------------------------------------------------------------------------------------*/
    //
    // EXT GAIN改變事件
    //
    /*
    else if ( Event_Module.Event3_Flag.bit.EXT_Mode_Change_GAIN )
    {
        EXT_Mode_Change_GAIN_Event();

        Event_Module.Event3_Flag.bit.EXT_Mode_Change_GAIN = 0;

        //cpu_timer_module.Event_Trig_Flag.bit.Value_Set_User_Change = 1;

    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // ADD Mode 改變ACDC事件
    //
    else if ( Event_Module.Event3_Flag.bit.ADD_Mode_Change_ACV_DCV_GAIN )
    {
        ADD_Mode_Change_ACV_DCV_GAIN_Event();

        Event_Module.Event3_Flag.bit.ADD_Mode_Change_ACV_DCV_GAIN = 0;

        //cpu_timer_module.Event_Trig_Flag.bit.Value_Set_User_Change = 1;

    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // ACDC 轉態 DC 計算VREF 事件
    //
    /*
    else if ( Event_Module.Event3_Flag.bit.Change_ACDC_to_DC_Calc_VREF )
    {
        Change_ACDC_to_DC_Calc_VREF_Event();

        Change_DCV_Event();

        Event_Module.Event3_Flag.bit.Change_ACDC_to_DC_Calc_VREF = 0;

        //cpu_timer_module.Event_Trig_Flag.bit.Value_Set_User_Change = 1;

    }
    */

    /*-----------------------------------------------------------------------------------------*/
    //
    // 改變設定頻率事件
    // (考慮到頻率補償，順位要在ACV之前)
    //
    else if ( Event_Module.Event3_Flag.bit.Change_FREQ )
    {
        Change_FREQ_Event();

        //
        // Reset移動平均累加
        //
        Display_Move_Avg_Reset();

        Event_Module.Event3_Flag.bit.Change_FREQ = 0;

        //
        // 頻率補償計算更新,觸發Change_ACV_DCV事件
        //
        /*
        if (( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable == 1 )
           && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_INT_MODE )
           && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_SIN )
           && ( ( (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all * 0.01 ) >= FREQ_COMPENSATION_LEVEL ))
        {
            Event_Module.Event3_Flag.bit.Change_ACV_DCV = 1;

            Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change = 1;

            // 量測補償OFF
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

        }
        */

    }


    /*-----------------------------------------------------------------------------------------*/
    //
    // 改變波形事件
    //
    else if ( Event_Module.Event3_Flag.bit.Change_Waveform )
    {
        Change_Waveform_Event();

        Event_Module.Event3_Flag.bit.Change_Waveform = 0;

    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // MS Mode 改變ACDC事件
    //
    else if ( Event_Module.Event3_Flag.bit.MS_Mode_Change_ACV_DCV )
    {
        MS_Mode_Change_ACV_DCV_Event();

        Event_Module.Event3_Flag.bit.MS_Mode_Change_ACV_DCV = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // MS Mode 改變相角
    //
    else if ( Event_Module.Event3_Flag.bit.MS_Mode_Change_Phase_Set )
    {
        MS_Mode_Change_Phase_Set_Event();

        Event_Module.Event3_Flag.bit.MS_Mode_Change_Phase_Set = 0;
    }



    /*-----------------------------------------------------------------------------------------*/
    //
    // 使用者設定ON/OFF事件
    //
    // 順位放到後面
    //
    else if ( Event_Module.Event3_Flag.bit.Set_Output_ON_OFF )
    {
        Set_Output_ON_OFF_Event();

        Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 0;
    }

    /*-----------------------------------------------------------------------------------------*/
    //
    // 處理觸發Value_Set_User_Change的事件
    //
    // ***確認Event3裡面的事件都執行後才觸發***
    //
    else if ( Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change )
    {
        if ( Event_Module.Event3_Flag.all == 0 )
        {
            Trig_Value_Set_User_Change_Event();

            Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change = 0;
        }
    }




    /*-----------------------------------------------------------------------------------------*/
    //
    // Debug Event
    // 輸出校正參數
    //
    else if ( Event_Module.Debug_Event_Flag.bit.Print_All_Cali_Variable )
    {
        Print_All_Cali_Variable_Event();

        Event_Module.Debug_Event_Flag.bit.Print_All_Cali_Variable = 0;
    }

    else
    {

    }

}









/////////////////////////////////////////////////////////////////////
void Load_Cali_Variable_from_EEPROM_Event(void)
{

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Load_Cali_Variable_from_EEPROM_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif



    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_V_FB_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    CPU_to_CLA_Variables.ADC_to_V_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;

    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_I_FB_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

    Read_EEPROM_addr_to_Buffer( LR_FREQ_TO_VRMS_A_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_FREQ_TO_VRMS_B_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_FREQ_TO_VRMS_C_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_CS_MPY_360000_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.Cs_mpy_360000.all = EEPROM_Module_Variables.Read_buffer.all;
    CPU_to_CLA_Variables.Cs_mpy_360000 = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;

    Read_EEPROM_addr_to_Buffer( LR_INV_RS_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.inv_Rs.all = EEPROM_Module_Variables.Read_buffer.all;
    CPU_to_CLA_Variables.inv_Rs = Global_Variables.Calibration_Variables.inv_Rs.all;

    Read_EEPROM_addr_to_Buffer( LR_V_RMS_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.V_rms_Offset.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_I_RMS_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.I_rms_Offset.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_P_V_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.p_V_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_N_V_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.n_V_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_P_I_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.p_I_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_N_I_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.n_I_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_V_FB_SCALE_P_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all = EEPROM_Module_Variables.Read_buffer.all;
    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;

    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_V_FB_SCALE_N_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all = EEPROM_Module_Variables.Read_buffer.all;
    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;

    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_I_FB_SCALE_P_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all = EEPROM_Module_Variables.Read_buffer.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;

    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_I_FB_SCALE_N_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all = EEPROM_Module_Variables.Read_buffer.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;

    Read_EEPROM_addr_to_Buffer( LR_VSET_TO_DAC_SCALE_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all = EEPROM_Module_Variables.Read_buffer.all;
    //
    // 觸發重新計算VREF DAC箝制的事件
    //
    Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit = 1;

    Read_EEPROM_addr_to_Buffer( LR_VSET_TO_DAC_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all = EEPROM_Module_Variables.Read_buffer.all;

    Read_EEPROM_addr_to_Buffer( LR_ADC_to_EXTIN_L1_SCALE_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all = EEPROM_Module_Variables.Read_buffer.all;
    Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale = 1 / Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all;


    Read_EEPROM_addr_to_Buffer( LR_ADC_to_EXTIN_L1_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level = (Uint16)Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;

    Read_EEPROM_addr_to_Buffer( CALI_DATA_VER_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.Data_Version = EEPROM_Module_Variables.Read_buffer.MSB;
    //
    // 之後檢查Cali版本如果不一致, 則新加入的校正參數載入初始值
    //

    /*
    if ( Global_Variables.Calibration_Variables.Data_Version > msb )
    {
        //
        // 加在這裡
        //
    }
    */
}



/////////////////////////////////////////////////////////////////////
void Save_Cali_Variable_to_EEPROM_Event(void)
{
    Uint16 write_err_counter;

    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Save_Cali_Variable_to_EEPROM_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    write_err_counter = 0;

    EEPROM_Control_Sequence( EEPROM_EWEN, 0 );




    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_V_FB_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_I_FB_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_FREQ_TO_VRMS_A_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_FREQ_TO_VRMS_B_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_FREQ_TO_VRMS_C_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_CS_MPY_360000_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.inv_Rs.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_INV_RS_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.V_rms_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_V_RMS_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.I_rms_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_I_RMS_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.p_V_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_P_V_PEAK_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.n_V_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_N_V_PEAK_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.p_I_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_P_I_PEAK_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.n_I_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_N_I_PEAK_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_V_FB_SCALE_P_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_V_FB_SCALE_N_EEPROM_ADDR );



    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_I_FB_SCALE_P_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_I_FB_SCALE_N_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_VSET_TO_DAC_SCALE_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_VSET_TO_DAC_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_to_EXTIN_L1_SCALE_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_to_EXTIN_L1_OFFSET_EEPROM_ADDR );


    EEPROM_Module_Variables.Write_buffer.MSB = Global_Variables.Calibration_Variables.Data_Version;
    EEPROM_Module_Variables.Write_buffer.LSB = Global_Variables.Calibration_Variables.Data_Version;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( CALI_DATA_VER_EEPROM_ADDR );



    /*----------------------------------------------------------------------*/
    if ( write_err_counter > 0 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = EEPROM_WRITE_NG;

        //
        // 轉態
        //
        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


        error_start_time_reset();

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Calibration_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        //
        #ifdef DEBUG_ON
        sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Write Error Counter -> %u\n", CpuTimer2.InterruptCount, write_err_counter ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }

    EEPROM_Control_Sequence( EEPROM_EWDS, 0 );
    ACFAIL_start_time_reset();

}


/////////////////////////////////////////////////////////////////////
void Set_DAC7612_CHA_Event(void)
{
    float p_i_peak_level;
    float scale;
    Uint16 dac_value;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_DAC7612_CHA_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable == 1 )
    {
        //
        // IPK Limit ON
        // 設定DAC7612讓硬體SPWM電路去箝制
        //

        p_i_peak_level = (float)Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all * 0.0001;
        scale = Global_Variables.Calibration_Variables.IREF_to_DAC_Scale.all;
        dac_value = (Uint16)( fabs( p_i_peak_level ) * scale );

        //
        // 箝制
        //
        if ( dac_value >= 4095  )
        {
            dac_value = 4095;
        }

    }
    else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable == 0 )
    {
        //
        // IPK Limit OFF
        // 韌體判斷直接停止輸出保護
        //
        dac_value = 4095;
    }

    Write_DAC7612_Sequence( DAC7612_CHA, dac_value ); //2758 cycle


    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DAC7612_CHA -> %d\n", CpuTimer2.InterruptCount, dac_value ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

}


/////////////////////////////////////////////////////////////////////
void Set_DAC7612_CHB_Event(void)
{
    float n_i_peak_level;
    float scale;
    Uint16 dac_value;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_DAC7612_CHB_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable == 1 )
    {
        //
        // IPK Limit ON
        // 設定DAC7612讓硬體SPWM電路去箝制
        //

        n_i_peak_level = (float)Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all * 0.0001;
        scale = Global_Variables.Calibration_Variables.IREF_to_DAC_Scale.all;
        dac_value = (Uint16)( fabs( n_i_peak_level ) * scale );

        //
        // 箝制
        //
        if ( dac_value >= 4095  )
        {
            dac_value = 4095;
        }

    }
    else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable == 0 )
    {
        //
        // IPK Limit OFF
        // 韌體判斷直接停止輸出保護
        //
        dac_value = 4095;
    }

    Write_DAC7612_Sequence( DAC7612_CHB, dac_value ); //2758 cycle


    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DAC7612_CHB -> %d\n", CpuTimer2.InterruptCount, dac_value ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

}


/////////////////////////////////////////////////////////////////////
//
// 根據機種設定初始值以及H/L Range開關
//
void Model_Type_Change_Event(void)
{
    //20211202
    //int32 max_value, min_value;

    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "\nCPU_Timer2: %lu ms > Trig Model_Type_Change_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    switch ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series )
    {
        case ASR_5075:
        {
            //
            // 機器額定參數
            //
            Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout = ASR_5075_MAX_AC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout = ASR_5075_MAX_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout = ASR_5075_MIN_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout = ASR_5075_MAX_I;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak = ASR_5075_MAX_I_PEAK;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power = ASR_5075_MAX_P;

            //
            // 高低檔位
            //
            Global_Variables.POWER_STATUS.bit.V_Range = LOW_RANGE;
            Global_Variables.POWER_STATUS.bit.I_Range = LOW_RANGE;

            Global_Variables.OUTPUT_FLAGS.bit.VM_HI_LO = LOW_RANGE;
            Global_Variables.OUTPUT_FLAGS.bit.IM_HI_LO = LOW_RANGE;

            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.VMONI_LO = LOW_RANGE;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.IMONI_LO = LOW_RANGE;


            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_series -> ASR-5075\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }

        case ASR_5150:
        {
            //
            // 機器額定參數
            //
            Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout = ASR_5150_LR_MAX_AC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout = ASR_5150_LR_MAX_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout = ASR_5150_LR_MIN_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout = ASR_5150_LR_MAX_I;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak = ASR_5150_LR_MAX_I_PEAK;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power = ASR_5150_LR_MAX_P;


            //
            // 高低檔位
            //
            Global_Variables.POWER_STATUS.bit.V_Range = LOW_RANGE;
            Global_Variables.POWER_STATUS.bit.I_Range = HIGH_RANGE;

            Global_Variables.OUTPUT_FLAGS.bit.VM_HI_LO = LOW_RANGE;
            Global_Variables.OUTPUT_FLAGS.bit.IM_HI_LO = HIGH_RANGE;
            Global_Variables.OUTPUT_FLAGS.bit.OUT_HI_LO = LOW_RANGE;

            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.VMONI_LO = LOW_RANGE;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.IMONI_LO = HIGH_RANGE;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_series -> ASR-5150\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;

        }
    }

    //
    // ACV = 額定 x 1.1
    //
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout * 11000.0 );
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_min = 0;

    //
    // DCV = 額定 x 1.1
    //
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 11000.0 );
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_min = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout * 11000.0 );

    //
    // I RMS = 額定 x 1.05
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 10500.0 );


    //
    // IPK max = 額定 x 1.05
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak * 10500.0 );
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max = (int32)( -Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak * 10500.0 );


    //
    // 功率保護
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.OPP_level = ( Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power * 1.1 );
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Power_limit_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Rated_Power;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level = -100.0;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level;


    //
    // IPK保護初始值
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all;

    //
    // OCP及Irms Limit初始值
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2 = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;

    //
    // OVP初始值
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.V_rms_limit_level = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_max * 0.0001;

    //
    // 4倍CF OCP
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES._4CF_OCP_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak;
    CPU_to_CLA_Variables._4CF_OCP_level = Global_Variables.POWER_PROTECT_SETTING_VARIABLES._4CF_OCP_level;

    Global_Variables.Pre_Calc_Variables.inv_Vmax = 1.0 / ( Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 1.01 );

    //
    // 根據機種載入校正初始值
    //
    //Set_V_Cali_Variable_default_Event();
    //Set_I_Cali_Variable_default_Event();
    //Set_EXT_Cali_Variable_default_Event();


}

/////////////////////////////////////////////////////////////////////
//
//
//
void Change_FREQ_Event(void)
{
    float  freq_set;

    float  a, b, c;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Change_FREQ_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    freq_set = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all * 0.01;

    //
    // 箝制最少1Hz
    //
    if ( freq_set <= 1.0 )
    {
        freq_set = 1.0;
    }

    if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable == 1 )
         && ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_INT_MODE )
              || ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == MASTER_AC_INT_MODE )
              || ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == SLAVE_AC_INT_MODE ) )
         && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_SIN )
         && ( freq_set >= FREQ_COMPENSATION_LEVEL ))
    {
        a = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all;
        b = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all;
        c = Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all;

        Output_Control_Module.Freq_Compensation_gain = a * freq_set * freq_set + b * freq_set + c;

    }
    else
    {
        Output_Control_Module.Freq_Compensation_gain = 1.0;
    }


    Global_Variables.FreqStep_shadow = (Uint32)( freq_set * _1HZ );


    //debug msg
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > FREQ_set -> %lu\n", CpuTimer2.InterruptCount, (Uint32)freq_set); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > FreqStep_shadow -> %lu\n", CpuTimer2.InterruptCount, Global_Variables.FreqStep_shadow); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

}



/////////////////////////////////////////////////////////////////////
/*
void SYNC_Mode_Change_FREQ_Event(void)
{
    float  freq_set;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig SYNC_Mode_Change_FREQ_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //Hz
    freq_set = Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all;

    //
    // 箝制最少1Hz
    //
    if ( freq_set <= 1.0 )
    {
        freq_set = 1.0;
    }

    //
    // 計算c1_coeff
    //
    cpu_timer_module.c1_coeff = Global_Variables.Pre_Calc_Variables.CpuTimer0Period_mpy_4k * freq_set;
    //cpu_timer_module.inv_c1_coeff = 1 / cpu_timer_module.c1_coeff;


    // 更新給CLA
    //CPU_to_CLA_Variables.c1_coeff = cpu_timer_module.c1_coeff;

    // 計算c1_final
    //cpu_timer_module.c1_final = (Uint32)( 100000000.0 / freq_set / CPU0_PERIOD );

    //debug msg
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > FREQ_set -> %lu\n", CpuTimer2.InterruptCount, (Uint32)freq_set); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > c1_coeff -> %e\n", CpuTimer2.InterruptCount, cpu_timer_module.c1_coeff); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

}
*/






/////////////////////////////////////////////////////////////////////
//
// 輸出
// Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SoftRamp_Step_set
//
void Calc_V_SlewRate_set_Event(void)
{

    float  float_buf1;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Calc_V_SlewRate_set_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user < 9 )
    {
        //
        // < 9 之後, V_SoftRamp_Step_set會超過1
        // 會造成WAVE的Slew Rate計算發散導致流程NG
        //

        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SoftRamp_Step_set = 1.0;

    }
    else
    {
        float_buf1 = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user;

        //指數斜率使用
        //Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SoftRamp_Step_set = SOFT_RAMP_COEFF * ( CPU0_PERIOD / ( float_buf1 ) );
        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SoftRamp_Step_set = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Soft_Ramp_Coeff * ( CPU0_PERIOD / ( float_buf1 ) );
    }

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > V_SlewRate_set_user -> %u\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > V_SoftRamp_Step_set -> %e\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SoftRamp_Step_set); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

}




/*
/////////////////////////////////////////////////////////////////////
void EXT_Mode_Change_GAIN_Event(void)
{
    int32  ex_gain;
    float  v_peak;
    float  ext_ad_in_max;
    float  vref_dac_value;
    float  a, b;

    float  vmax, vref_dac_value_max;

    ext_ad_in_max = 2.5; //2.5V
    ex_gain = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.all;
    v_peak = (float)ex_gain * ext_ad_in_max;


    a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
    b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

    //
    // 計算VREF
    //
    vref_dac_value = ( v_peak * 0.0001 * a ) + b; //1062 cycles

    //
    // 輸出Vmax計算DAC箝制量
    //
    vmax = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout;
    vref_dac_value_max = ( vmax * a ) + b; //1062 cycles


    //
    // 箝制VREF DAC
    //
    if ( vref_dac_value >= vref_dac_value_max )
    {
        vref_dac_value = vref_dac_value_max;
    }
    else if ( vref_dac_value <= b )
    {
        vref_dac_value = b;
    }

    Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)vref_dac_value;



    //
    // Wave scale, Wave offset
    //
    if ( ex_gain == 0 )
    {
        Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = 0;
        Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = (int32)AD5543_HALF_VALUE;
    }
    else
    {
        Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = 1;
        Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = (int32)AD5543_HALF_VALUE;
    }




    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Change_DCV -> %ld\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Set_exGAIN -> %ld\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.all); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

}
*/

/////////////////////////////////////////////////////////////////////
void ADD_Mode_Change_ACV_DCV_GAIN_Event(void)
{

    int32  acv_set, dcv_set;
    float  cf_value;
    float  acv_peak;

    float  a, b;

    float vmax;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig ADD_Mode_Change_ACV_DCV_GAIN_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    // 計算VREF DAC
    //
    vmax = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 1.01;
    a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
    b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
    Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)( ( vmax * a ) + b );


    //
    // DCV Set
    //
    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 0 )
    {
        dcv_set = 0;
    }
    else
    {
        dcv_set = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCV_set_user -> %ld\n", CpuTimer2.InterruptCount, dcv_set); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }


    //
    // Wave offset
    //
    Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = (int32)( ( -(float)dcv_set / ( vmax * 10000.0 ) ) * AD5543_HALF_VALUE + AD5543_HALF_VALUE );

    //
    // ACV Set
    //
    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 0 )
    {
        acv_peak = 0;
    }
    else
    {
        acv_set = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all;
        cf_value = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user ];
        acv_peak = (float)acv_set * cf_value;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ACV_set_user -> %ld\n", CpuTimer2.InterruptCount, acv_set); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }

    //
    // Wave scale
    //
    Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = -acv_peak / ( vmax * 10000.0 );




}






/////////////////////////////////////////////////////////////////////
//
// MS模式下, Master/Slave輸出, ACV或DCV改變的設定流程
//
void MS_Mode_Change_ACV_DCV_Event(void)
{

    int32  acv_set, dcv_set;
    float  cf_value;
    float  acv_peak;

    float  a, b;

    float vmax;

    Uint16 step, item;
    int32  Step_DCV_Set;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig MS_Mode_Change_ACV_DCV_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    if ( Event_Module.Event2_Flag.bit.Cali_Set_0_DCV == 1 )
    {
        //
        // 校正模式中, DCV = 0V
        //

        step = Global_Variables.Calibration_Step_Flag.bit.Step;
        item = Global_Variables.Calibration_Step_Flag.bit.Item;
        Step_DCV_Set = Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[item][step];

        Event_Module.Event2_Flag.bit.Cali_Set_0_DCV = 0;

        //
        // 計算VREF DAC
        //
        vmax = fabs( (float)Step_DCV_Set * 0.0001 );
        a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
        b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
        Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)( ( vmax * a ) + b );

        //
        // Wave offset
        //
        Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = 0;

        //
        // Wave scale
        //
        Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = 0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Cali_Set_0_DCV\n", CpuTimer2.InterruptCount); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif


    }
    else if ( Event_Module.Event2_Flag.bit.Cali_Set_Max_DCV == 1 )
    {
        //
        // 校正模式中, DCV = +Max
        //

        step = Global_Variables.Calibration_Step_Flag.bit.Step;
        item = Global_Variables.Calibration_Step_Flag.bit.Item;
        Step_DCV_Set = Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[item][step];

        Event_Module.Event2_Flag.bit.Cali_Set_Max_DCV = 0;


        //
        // 計算VREF DAC
        //
        vmax = fabs( (float)Step_DCV_Set * 0.0001 );
        a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
        b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
        Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)( ( vmax * a ) + b );


        //
        // Wave offset
        //
        Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = 0;

        //
        // Wave scale
        //
        Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = 0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Cali_Set_Max_DCV\n", CpuTimer2.InterruptCount); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }
    else if ( Event_Module.Event2_Flag.bit.Cali_Set_Min_DCV == 1 )
    {
        //
        // 校正模式中, DCV = -Max
        //

        step = Global_Variables.Calibration_Step_Flag.bit.Step;
        item = Global_Variables.Calibration_Step_Flag.bit.Item;
        Step_DCV_Set = Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[item][step];

        Event_Module.Event2_Flag.bit.Cali_Set_Min_DCV = 0;

        //
        // 計算VREF DAC
        //
        vmax = fabs( (float)Step_DCV_Set * 0.0001 );
        a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
        b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
        Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)( ( vmax * a ) + b );


        //
        // Wave offset
        //
        Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = (int32)65535;

        //
        // Wave scale
        //
        Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = 0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Cali_Set_Min_DCV\n", CpuTimer2.InterruptCount); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }
    else
    {
        //
        // 非校正模式
        //

        //
        // 計算VREF DAC
        //
        vmax = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 1.01;
        a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
        b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
        Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)( ( vmax * a ) + b );

        //
        // DCV Set
        //
        if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 0 )
        {
            dcv_set = 0;
        }
        else
        {
            dcv_set = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all;

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCV_set_user -> %ld\n", CpuTimer2.InterruptCount, dcv_set); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

        //
        // Wave offset
        //
        Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = (int32)( ( -(float)dcv_set / ( vmax * 10000.0 ) ) * AD5543_HALF_VALUE + AD5543_HALF_VALUE );


        //
        // ACV Set
        //
        if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 0 )
        {
            acv_peak = 0;
        }
        else
        {
            acv_set = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all;

            //
            // 考慮頻率補償
            //
            acv_set = (int32)( (float)acv_set * Output_Control_Module.Freq_Compensation_gain );

            cf_value = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user ];
            acv_peak = (float)acv_set * cf_value;

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ACV_set_user -> %ld\n", CpuTimer2.InterruptCount, acv_set); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

        //
        // Wave scale
        //
        Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = -acv_peak / ( vmax * 10000.0 );




    }






}





/////////////////////////////////////////////////////////////////////
//
// MS模式下, 改變輸出相角
//
void MS_Mode_Change_Phase_Set_Event(void)
{
    float phase_set, phase_count;

    //
    // Slave計算3P4W或1P3W落後角度 ( for c2 )
    //
    //

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig MS_Mode_Change_Phase_Set_Event\n", CpuTimer2.InterruptCount); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    if ( ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _3P4W_3UNITS )
         && ( Global_Variables.MS_Function_Variables.MS_Address == 1 ) )
    {
        //
        // 3P4W, L2
        //
        phase_set = (float)Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[1];
    }
    else if ( ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _3P4W_3UNITS )
              && ( Global_Variables.MS_Function_Variables.MS_Address == 2 ) )
    {
        //
        // 3P4W, L3
        //
        phase_set = (float)Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[2];
    }
    else if ( ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _1P3W_2UNITS )
              && ( Global_Variables.MS_Function_Variables.MS_Address == 1 ) )
    {
        //
        // 1P3W, L2
        //
        phase_set = (float)Global_Variables.MS_Function_Variables.MS_1P3W_Phase_set_user[1];
    }
    else
    {
        //
        // 其他
        //
        phase_set = 0;
    }

    //phase_count = phase_set * _4096_div_3600;

    //
    // 1週期 => 360度        => 2^32 count
    //        位移相角 度 => 2^32 x ( 位移相角 / 360 ) count
    //
    phase_count = phase_set * 100.0 * _1HZ;

    Global_Variables.MS_Function_Variables.MS_Output_Phase_Delay_Count = (Uint32)phase_count;

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_Phase_Delay_Count -> %u\n", CpuTimer2.InterruptCount, Global_Variables.MS_Function_Variables.MS_Output_Phase_Delay_Count); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

}












/////////////////////////////////////////////////////////////////////
void Set_Start_phase_Event(void)
{

    float f_buf1;

    f_buf1 = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user * 0.1;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count = (Uint32)( f_buf1 * DEG_STEP );

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Start_Phase_set_user -> %u\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Start_phase_count -> %lu\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


}

/////////////////////////////////////////////////////////////////////
void Set_Stop_phase_Event(void)
{
    float f_buf1;

    f_buf1 = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user * 0.1;

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_phase_count = (Uint32)( f_buf1 * DEG_STEP );


    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Stop_Phase_set_user -> %u\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Stop_phase_count -> %lu\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_phase_count); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


}

/////////////////////////////////////////////////////////////////////
/*
void SYNC_Mode_Calc_Zero_Phase_Count_Event(void)
{
    //float float_buf, float_buf2;
    float float_buf2;

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig SYNC_Mode_Calc_Zero_Phase_Count_Event\n", CpuTimer2.InterruptCount); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    //float_buf = ( Global_Variables.Pre_Calc_Variables.Inv_CpuTimer0Period_mpy_360 / ( Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all * 100.0 ) );

    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source == SYNC_LINE )
    {
        //
        // SYNC LINE
        //

        //
        // Phase 0
        //
        //float_buf2 = float_buf * 1800.0 * 10.0;

        //
        // ASR-5000實際測試, ACFAIL H->L對應0度
        //
        float_buf2 = 0;

    }
    else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source == SYNC_EXT )
    {
        //
        // SYNC EXT
        //
        float_buf2 = 0;
    }

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Zero_phase_count = (Uint32)float_buf2;


    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > SYNC_Zero_phase_count -> %lu\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Zero_phase_count); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //#ifdef DEBUG_ON
    //sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > SYNC_Mode_Calc_Zero_Phase_Count -> %u\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Zero_phase_count); //9530 cycles
    //scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    //#endif

}
*/

















/////////////////////////////////////////////////////////////////////
void SYNC_Mode_Change_Phase_Delay_Event(void)
{

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig SYNC_Mode_Change_Phase_Delay_Event\n", CpuTimer2.InterruptCount); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    //
    // 參考ASR-2000作法
    //
    //Phase_Sync_Module.LINE_Phase_Sync_Variables.Phase_shift = 2048.0 + (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user * 4096.0 / 360.0;
    //先配合實測2048改成0, 之後再研究
    Phase_Sync_Module.LINE_Phase_Sync_Variables.Phase_shift = 0 + (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user * 4096.0 / 3600.0;



    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > SYNC_Phase_Delay_Count -> %lu\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_Count); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif



}


/////////////////////////////////////////////////////////////////////
void Preload_SQU_WAVE_DATA_Event(void)
{
    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Preload_SQU_WAVE_DATA_Event\n", CpuTimer2.InterruptCount); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    //
    // 把SQU SDRAM的資料載入WAVE
    //
    if ( Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_SQU, 1 ) == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = LOAD_WAVE_SQU_TO_LOCAL_CHECKSUM_NG;

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

        error_start_time_reset();

        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        return;
    }

}

/////////////////////////////////////////////////////////////////////
void Preload_WAVE_DATA_Event(void)
{
    //固定
    //Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_SIN, 0 );
    //Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_SQU, 1 );
    //Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_TRI, 2 );

    //可變
    //Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_ARB1, 3 );
    //Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_ARB2, 4 );

    //
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Preload_WAVE_DATA_Event\n", CpuTimer2.InterruptCount); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    // 把SIN SDRAM的資料載入WAVE
    //
    if ( Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_SIN, 0 ) == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = LOAD_WAVE_SIN_TO_LOCAL_CHECKSUM_NG;

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

        error_start_time_reset();

        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        return;
    }

    //
    // 把SQU SDRAM的資料載入WAVE
    //
    if ( Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_SQU, 1 ) == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = LOAD_WAVE_SQU_TO_LOCAL_CHECKSUM_NG;

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

        error_start_time_reset();

        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        return;
    }


    //
    // 把TRI SDRAM的資料載入WAVE
    //
    if ( Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_TRI, 2 ) == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = LOAD_WAVE_TRI_TO_LOCAL_CHECKSUM_NG;

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

        error_start_time_reset();

        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        return;
    }

    //
    // 把ARB1 SDRAM的資料載入WAVE
    //
    if ( Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_ARB1, 3 ) == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = LOAD_WAVE_ARB1_TO_LOCAL_CHECKSUM_NG;

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

        error_start_time_reset();

        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        return;
    }

    Memory_Module.wave_buffer_section_3 = WAVE_ARB1;


    //
    // 把ARB2 SDRAM的資料載入WAVE
    //
    if ( Load_WAVE_SDRAM_to_LOCAL_RAM_Process( WAVE_ARB2, 4 ) == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = LOAD_WAVE_ARB2_TO_LOCAL_CHECKSUM_NG;

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

        error_start_time_reset();

        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        return;
    }

    Memory_Module.wave_buffer_section_4 = WAVE_ARB2;


}








/////////////////////////////////////////////////////////////////////
//
// 電壓校正前參數初始化
// 只針對電壓校正時的校正參數初始化
//
void Set_V_Cali_Variable_default_Event(void)
{
    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_V_Cali_Variable_default_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    switch ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series )
    {
        case ASR_5075:
        {
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all = ADC_TO_V_FB_OFFSET_5075;
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all = ADC_TO_V_FB_SCALE_P_5075;
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all = ADC_TO_V_FB_SCALE_N_5075;

            Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all = VSET_to_DAC_SCALE_5075;
            Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all = VSET_to_DAC_OFFSET_5075;

            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all = FREQ_TO_VRMS_A_5075;
            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all = FREQ_TO_VRMS_B_5075;
            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all = FREQ_TO_VRMS_C_5075;

            //
            // 電壓校正時會去更新電流offset
            //
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = ADC_TO_I_FB_OFFSET_5075;

            break;
        }

        case ASR_5150:
        {
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all = ADC_TO_V_FB_OFFSET_5150;
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all = ADC_TO_V_FB_SCALE_P_5150;
            Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all = ADC_TO_V_FB_SCALE_N_5150;

            Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all = VSET_to_DAC_SCALE_5150;
            Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all = VSET_to_DAC_OFFSET_5150;

            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all = FREQ_TO_VRMS_A_5150;
            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all = FREQ_TO_VRMS_B_5150;
            Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all = FREQ_TO_VRMS_C_5150;

            //
            // 電壓校正時會去更新電流offset
            //
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = ADC_TO_I_FB_OFFSET_5150;

            break;
        }
    }

    //
    // 觸發重新計算VREF DAC箝制的事件
    //
    Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit = 1;


    Global_Variables.Calibration_Variables.Data_Version = CALI_DATA_VER;

    CPU_to_CLA_Variables.ADC_to_V_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;

    CPU_to_CLA_Variables.ADC_to_I_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;

    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;
    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;


    Global_Variables.Calibration_Variables.V_rms_Offset.all = V_RMS_OFFSET;

    Global_Variables.Calibration_Variables.p_V_peak_Offset.all = P_V_PEAK_OFFSET;
    Global_Variables.Calibration_Variables.n_V_peak_Offset.all = N_V_PEAK_OFFSET;

}


/////////////////////////////////////////////////////////////////////
//
// 電流校正前參數初始化
// 只針對電流校正時的校正參數初始化
//
void Set_I_Cali_Variable_default_Event(void)
{
    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_I_Cali_Variable_default_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    switch ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series )
    {
        case ASR_5075:
        {

            Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = ADC_TO_I_FB_OFFSET_5075;
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all = ADC_TO_I_FB_SCALE_P_5075;
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all = ADC_TO_I_FB_SCALE_N_5075;

            Global_Variables.Calibration_Variables.IREF_to_DAC_Scale.all = IREF_TO_DAC_SCALE_5075;
            Global_Variables.Calibration_Variables.IREF_to_DAC_Offset.all = IREF_TO_DAC_OFFSET_5075;

            break;
        }

        case ASR_5150:
        {
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = ADC_TO_I_FB_OFFSET_5150;
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all = ADC_TO_I_FB_SCALE_P_5150;
            Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all = ADC_TO_I_FB_SCALE_N_5150;

            Global_Variables.Calibration_Variables.IREF_to_DAC_Scale.all = IREF_TO_DAC_SCALE_5150;
            Global_Variables.Calibration_Variables.IREF_to_DAC_Offset.all = IREF_TO_DAC_OFFSET_5150;

            break;
        }
    }

    Global_Variables.Calibration_Variables.inv_Rs.all = INV_RS_REF;
    CPU_to_CLA_Variables.inv_Rs = Global_Variables.Calibration_Variables.inv_Rs.all;

    Global_Variables.Calibration_Variables.Cs_mpy_360000.all = CS_MPY_360000_REF;
    CPU_to_CLA_Variables.Cs_mpy_360000 = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;



    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;


    Global_Variables.Calibration_Variables.I_rms_Offset.all = I_RMS_OFFSET;

    Global_Variables.Calibration_Variables.p_I_peak_Offset.all = P_I_PEAK_OFFSET;
    Global_Variables.Calibration_Variables.n_I_peak_Offset.all = N_I_PEAK_OFFSET;
}



/////////////////////////////////////////////////////////////////////
//
// EXT校正前參數初始化
// 只針對EXT校正時的校正參數初始化
//
void Set_EXT_Cali_Variable_default_Event(void)
{
    float32 scale, offset;

    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_EXT_Cali_Variable_default_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all = ADC_to_EXTIN_L1_SCALE_REF;
    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all = ADC_to_EXTIN_L1_OFFSET_REF;

    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level = (Uint16)Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;


    Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale = 1 / Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all;


    scale = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all;
    offset = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;

    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN_TTL_Level_L = (Uint16)(( TTL_LEVEL_L * scale ) + offset );
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN_TTL_Level_H = (Uint16)(( TTL_LEVEL_H * scale ) + offset );


}



/////////////////////////////////////////////////////////////////////
void Set_Cali_Step_Event_6step(void)
{
    Uint16 step, item;
    int32  Step_DCV_Set;

    step = Global_Variables.Calibration_Step_Flag.bit.Step;
    item = Global_Variables.Calibration_Step_Flag.bit.Item;

    Step_DCV_Set = Calibration_Module_Variables.Calibration_process_Variables.Step_DCV_Set[item][step];

    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_Cali_Step_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif



    switch ( item )
    {
        /*---------------------------------------------------*/
        case CALI_VOLTGE:
        {
            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration item -> CALI_VOLTGE\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration step -> %u\n", CpuTimer2.InterruptCount, step ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            if ( step == 0 )
            {
                if ( Main_State_Module.Machine_state == READY_STATE )
                {
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
                    // 校正參數初始化
                    //
                    Set_V_HL_Cali_Variable_default_Event();

                    //
                    // 觸發校正Checksum計算事件
                    //
                    Update_HL_Cali_CheckSum_Event();


                    //
                    // DC輸出 -Vmax x ratio
                    //
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                    Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

                    //
                    // 觸發設定DCV輸出 -Max
                    //
                    Event_Module.Event2_Flag.bit.Cali_Set_Min_DCV = 1;

                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                }
                else if ( Main_State_Module.Machine_state != READY_STATE )
                {
                    //
                    // 不在READY或其他狀態, 手動換檔切Relay有危險
                    //

                    Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                    Global_Variables.POWER_FUNCTIONS_ERROR_CODE = SET_CALI_STEP_0_ERROR;

                    //
                    // 轉態
                    //
                    Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                    error_start_time_reset();

                    //
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Calibration_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    //
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                }
            }
            else if ( step == 1 )
            {
                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }


                //
                // DC輸出 +Vmax x ratio
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

                //
                // 觸發設定DCV輸出 Max
                //
                Event_Module.Event2_Flag.bit.Cali_Set_Max_DCV = 1;

            }
            else if ( step == 2 )
            {
                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }

                //
                // DC輸出 0
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

                //
                // 觸發設定DCV輸出  0
                //
                Event_Module.Event2_Flag.bit.Cali_Set_0_DCV = 1;

            }
            else if ( ( step == 3 ) && ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) )
            {
                //
                // ASR-5075 Bypass
                //

                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }

                //
                // DC輸出 -Vmax x ratio
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

                //
                // 觸發設定DCV輸出 -Max
                //
                Event_Module.Event2_Flag.bit.Cali_Set_Min_DCV = 1;

            }
            else if ( ( step == 4 ) && ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) )
            {
                //
                // ASR-5075 Bypass
                //

                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }

                //
                // DC輸出 +Vmax x ratio
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

                //
                // 觸發設定DCV輸出 Max
                //
                Event_Module.Event2_Flag.bit.Cali_Set_Max_DCV = 1;

            }
            else if ( ( step == 5 ) && ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) )
            {
                //
                // ASR-5075 Bypass
                //

                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }


                //
                // DC輸出 0
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

                //
                // 觸發設定DCV輸出  0
                //
                Event_Module.Event2_Flag.bit.Cali_Set_0_DCV = 1;

            }

            Global_Variables.Calibration_State_Flag.all = 1; //finish

            break;
        }

        /*---------------------------------------------------*/
        case CALI_CURRENT:
        {
            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration item -> CALI_CURRENT\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration step -> %u\n", CpuTimer2.InterruptCount, step ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            if ( step == 0 )
            {
                if ( Main_State_Module.Machine_state == READY_STATE )
                {
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
                    // 校正參數初始化
                    //
                    Set_I_HL_Cali_Variable_default_Event();

                    //
                    // 觸發校正Checksum計算事件
                    //
                    Update_HL_Cali_CheckSum_Event();


                    //
                    // 輸出DC -100V
                    // 靠負載設定, 讓輸出電流到-Max
                    //
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                    Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                }
                else if ( Main_State_Module.Machine_state != READY_STATE )
                {
                    //
                    // 不在READY或其他狀態, 手動換檔切Relay有危險
                    //

                    Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
                    Global_Variables.POWER_FUNCTIONS_ERROR_CODE = SET_CALI_STEP_0_ERROR;

                    //
                    // 轉態
                    //
                    Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                    error_start_time_reset();

                    //
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Calibration_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    //
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                }
            }
            else if ( step == 1 )
            {
                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }


                //
                // 輸出DC 100V
                // 靠負載設定, 讓輸出電流到Max
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            }
            else if ( step == 2 )
            {
                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_LOW_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }

                //
                // DC輸出 0
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            }
            else if ( ( step == 3 ) && ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) )
            {
                //
                // ASR-5075 Bypass
                //

                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }

                //
                // 輸出DC -200V
                // 靠負載設定, 讓輸出電流到-Max
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            }
            else if ( ( step == 4 ) && ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) )
            {
                //
                // ASR-5075 Bypass
                //

                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }

                //
                // 輸出DC 200V
                // 靠負載設定, 讓輸出電流到Max
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            }
            else if ( ( step == 5 ) && ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) )
            {
                //
                // ASR-5075 Bypass
                //

                //
                // 設定檔位
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_HIGH_RANGE;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致 => 換檔
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //
                    // 換檔後持續輸出
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 1;

                    //
                    // 輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                }
                else
                {
                    //
                    // 輸出ON
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 1;
                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }

                //
                // DC輸出 0
                //
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all = Step_DCV_Set;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all = 0;

                Global_Variables.OUTPUT_MODE_FLAG.all = ACDC_INT_MODE;

            }

            Global_Variables.Calibration_State_Flag.all = 1; //finish

            break;
        }

        /*---------------------------------------------------*/
        case CALI_EXT:
        {
            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration item -> CALI_EXT\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            //校正參數初始化
            Set_EXT_Cali_Variable_default_Event();


            //校正流程轉態
            Calibration_Module_Variables.Calibration_process_state = Calibration_ExtAD_offset_step0_state;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration_process_state -> Calibration_ExtAD_offset_step0_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            break;
        }
    }

}

/////////////////////////////////////////////////////////////////////
//
// 根據step及測試項去切換校正流程
//
void Set_Cali_Meter_Value_Event(void)
{
    Uint16 step, item;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_Cali_Meter_Value_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    step = Global_Variables.Calibration_Step_Flag.bit.Step;
    item = Global_Variables.Calibration_Step_Flag.bit.Item;

    switch ( item )
    {
        /*---------------------------------------------------*/
        case CALI_VOLTGE:
        {
            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration item -> CALI_VOLTGE\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration step -> %u\n", CpuTimer2.InterruptCount, step ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            if ( step == 5 )
            {
                //校正流程轉態
                Calibration_Module_Variables.Calibration_process_state = Calibration_Vdc_state0;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration_process_state -> Calibration_Vdc_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
            else
            {
                Global_Variables.Calibration_State_Flag.all = 1; //finish
            }

            break;
        }

        /*---------------------------------------------------*/
        case CALI_CURRENT:
        {
            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration item -> CALI_CURRENT\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration step -> %u\n", CpuTimer2.InterruptCount, step ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            if ( step == 5 )
            {
                //校正流程轉態
                Calibration_Module_Variables.Calibration_process_state = Calibration_Idc_state0;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration_process_state -> Calibration_Idc_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
            else
            {
                Global_Variables.Calibration_State_Flag.all = 1; //finish
            }

            break;
        }

        /*---------------------------------------------------*/
        case CALI_EXT:
        {
            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Calibration item -> CALI_EXT\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }

    }

}

/////////////////////////////////////////////////////////////////////
void Cancel_Cali_Process_Event(void)
{
    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Cancel_Cali_Process_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

    Load_HL_Cali_Variable_from_EEPROM_Event();
    Load_HL_Cali_to_Cali_Event();


    //
    // 觸發校正Checksum計算事件
    //
    Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

    //
    // 觸發轉態
    //
    Calibration_Module_Variables.Calibration_process_state = Calibration_Ready_state;


    Global_Variables.Calibration_Step_Flag.all = 0;


}


/////////////////////////////////////////////////////////////////////
void Print_All_Cali_Variable_Event(void)
{
    //把目前校正參數都印出來

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Print all calibration variable:\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    /*--------------------------------------------------------------------------------------------------------------------*/
    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > VSET_to_DAC_Scale: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > VSET_to_DAC_Offset: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    /*--------------------------------------------------------------------------------------------------------------------*/
    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ADC_to_V_FB_Scale_P: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ADC_to_V_FB_Scale_N: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ADC_to_V_FB_Offset: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    /*--------------------------------------------------------------------------------------------------------------------*/
    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > V_rms_Offset: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.V_rms_Offset.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    /*--------------------------------------------------------------------------------------------------------------------*/
    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ADC_to_I_FB_Scale_P: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ADC_to_I_FB_Scale_N: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ADC_to_I_FB_Offset: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    /*--------------------------------------------------------------------------------------------------------------------*/
    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > I_rms_Offset: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.I_rms_Offset.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    /*--------------------------------------------------------------------------------------------------------------------*/
    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Freq_Compensation_coeff_a: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Freq_Compensation_coeff_b: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Freq_Compensation_coeff_c: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    /*--------------------------------------------------------------------------------------------------------------------*/
    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > inv_Rs: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.inv_Rs.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    //#ifdef DEBUG_ON
    //sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Cs: %e\n", CpuTimer2.InterruptCount
    //         , Global_Variables.Calibration_Variables.Cs.all ); //9530 cycles
    //scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    //#endif


    /*--------------------------------------------------------------------------------------------------------------------*/
    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ADC_to_EXTIN_L1_Scale: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ADC_to_EXTIN_L1_offset: %e\n", CpuTimer2.InterruptCount
             , Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif



}





/////////////////////////////////////////////////////////////////////
//
// 改變波形事件
//
void Change_Waveform_Event(void)
{
    Uint16 wave_form;

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Change_Waveform_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    // Preload waveform
    // 根據waveform指定載入的wave_buffer_section
    //
    switch ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user )
    {
        case WAVE_SIN:
        {
            //Memory_Module.wave_buffer_section = 0;
            wave_form = 0;

            break;
        }
        case WAVE_SQU:
        {
            //Memory_Module.wave_buffer_section = 1;
            wave_form = 1;

            break;
        }
        case WAVE_TRI:
        {
            //Memory_Module.wave_buffer_section = 2;
            wave_form = 2;

            break;
        }
        case WAVE_ARB1:
        case WAVE_ARB2:
        case WAVE_ARB3:
        case WAVE_ARB4:
        case WAVE_ARB5:
        case WAVE_ARB6:
        case WAVE_ARB7:
        case WAVE_ARB8:
        case WAVE_ARB9:
        case WAVE_ARB10:
        case WAVE_ARB11:
        case WAVE_ARB12:
        case WAVE_ARB13:
        case WAVE_ARB14:
        case WAVE_ARB15:
        case WAVE_ARB16:
        {
            if ( Memory_Module.wave_buffer_section_3 == Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user )
            {
                //Memory_Module.wave_buffer_section = 3;
                wave_form = 3;
            }
            else if ( Memory_Module.wave_buffer_section_4 == Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user )
            {
                //Memory_Module.wave_buffer_section = 4;
                wave_form = 4;
            }
            else
            {
                if ( Memory_Module.wave_buffer_section <= 3 )
                {
                    //
                    // 把SDRAM裡面ARB的資料載入WAVE buffer #4
                    //
                    if ( Load_WAVE_SDRAM_to_LOCAL_RAM_Process( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user, 4 ) == 1 )
                    {
                        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1;
                        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = LOAD_WAVE_TO_LOCAL_CHECKSUM_NG;

                        //debug msg
                        #ifdef DEBUG_ON
                        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                        #endif

                        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                        error_start_time_reset();

                        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

                        //
                        #ifdef DEBUG_ON
                        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                        #endif

                        return;
                    }

                    //Memory_Module.wave_buffer_section = 4;
                    wave_form = 4;

                    //
                    // 表示WAVE buffer #4 裡面存放的是目前設定的WAVE_form
                    //
                    Memory_Module.wave_buffer_section_4 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;
                }
                else if ( Memory_Module.wave_buffer_section == 4 )
                {
                    //
                    // 把SDRAM裡面ARB的資料載入WAVE buffer #3
                    //
                    if ( Load_WAVE_SDRAM_to_LOCAL_RAM_Process( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user, 3 ) == 1 )
                    {
                        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1;
                        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = LOAD_WAVE_TO_LOCAL_CHECKSUM_NG;

                        //debug msg
                        #ifdef DEBUG_ON
                        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                        #endif

                        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                        error_start_time_reset();

                        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

                        //
                        #ifdef DEBUG_ON
                        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                        #endif

                        return;
                    }

                    //Memory_Module.wave_buffer_section = 3;
                    wave_form = 3;

                    //
                    // 表示WAVE buffer #3 裡面存放的是目前設定的WAVE_form
                    //
                    Memory_Module.wave_buffer_section_3 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;
                }
            }

            break;
        }
    }

    //
    // 更新wave_buffer_section_shadow
    //
    Memory_Module.wave_buffer_section_shadow = wave_form;
}


/////////////////////////////////////////////////////////////////////
//
// 計算更新校正參數CheckSum
//
void Update_Cali_CheckSum_Event(void)
{
    Uint16 Checksum;

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Update_Cali_CheckSum_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    Checksum = 0;

    Checksum = Checksum + Global_Variables.Calibration_Variables.Data_Version;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.Cs_mpy_360000.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.Cs_mpy_360000.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.inv_Rs.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.inv_Rs.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.V_rms_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.V_rms_Offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.I_rms_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.I_rms_Offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.p_V_peak_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.p_V_peak_Offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.n_V_peak_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.n_V_peak_Offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.p_I_peak_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.p_I_peak_Offset.MSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.n_I_peak_Offset.LSB;
    Checksum = Checksum + Global_Variables.Calibration_Variables.n_I_peak_Offset.MSB;

    Global_Variables.Calibration_Variables.CheckSum = Checksum;
}


/////////////////////////////////////////////////////////////////////
//
// 處理觸發cpu_timer0輸出中改變設定值flag => Value_Set_User_Change
//
void Trig_Value_Set_User_Change_Event(void)
{

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Trig_Value_Set_User_Change_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    cpu_timer_module.Value_Set_User_Change = 1;


}




/////////////////////////////////////////////////////////////////////
//
// 清空Jump Count
//
void SEQ_Mode_Clear_Jump_Count_Event(void)
{
    Uint16 i;

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig SEQ_Mode_Clear_Jump_Count_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    for ( i = 0; i < SEQ_STEP_MAX; i ++ )
    {
        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Jump_Count[i] = 0;

        //
        // 避免逾時誤判
        //
        ACFAIL_start_time_reset();
    }
}

/////////////////////////////////////////////////////////////////////
//
// Step 初始化
//
void SEQ_Mode_Clear_All_Step_Event(void)
{
    Uint32 i;

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig SEQ_Mode_Clear_All_Step_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif


    //
    // Clear SEQ Step Memory
    //

    for ( i = 0; i < SEQ_STEP_MAX; i ++ )
    {
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].all = 0;
        SEQ_Step_Section1.SEQ_Config_State2_set_user[i].all = 0;

        SEQ_Step_Section1.SEQ_Config_State2_set_user[i].bit.WAVE_Form = WAVE_SIN;
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].bit.ACV_Const = 1;
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].bit.DCV_Const = 1;
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].bit.FREQ_Const = 1;
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].bit.Step_Continuous = 1;

        SEQ_Step_Section2.SEQ_Branch1_set_user[i] = 0;
        SEQ_Step_Section2.SEQ_Branch2_set_user[i] = 0;

        SEQ_Step_Section2.SEQ_Jump_set_user[i] = 0;
        SEQ_Step_Section2.SEQ_Jump_Count_set_user[i] = 1;

        SEQ_Step_Section1.SEQ_Start_Phase_set_user[i] = 0;
        SEQ_Step_Section1.SEQ_Stop_Phase_set_user[i] = 0;

        SEQ_Step_Section3.SEQ_Freq_set_user[i].all = 5000;
        SEQ_Step_Section4.SEQ_DCV_set_user[i].all = 0;
        SEQ_Step_Section4.SEQ_ACV_set_user[i].all = 0;

        SEQ_Step_Section5.SEQ_SQU_Duty_set_user[i] = 2048;

        SEQ_Step_Section3.SEQ_Step_Time_set_user[i].all = 1000;
    }

}


/////////////////////////////////////////////////////////////////////
//
// Clear SEQ CheckSum
//
void SEQ_Mode_Clear_CheckSum_Event(void)
{

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig SEQ_Mode_Clear_CheckSum_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum = 0;

}




/////////////////////////////////////////////////////////////////////
//
// 判斷ACV Unit, 重新載入WAVE CF
//
void Reload_Wave_CF_set_Event(void)
{
    Uint16 i;

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Reload_Wave_CF_set_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit == 0 )
    {
        //
        // 單位 RMS
        //

        for ( i = 0; i < 20; i++ )
        {
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ i ] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[ i ].all;
        }

    }
    else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit == 1 )
    {
        //
        // 單位 Peak to Peak
        //

        //
        // ARB1-16以及TRI的CF固定為0.5
        //
        for ( i = 1; i < 17; i++ )
        {
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ i ] = 0.5;
        }

        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ WAVE_TRI ] = 0.5;

        //
        // 預先計算 TRI CF的倒數
        // 補償控制使用
        //
        Global_Variables.Pre_Calc_Variables.inv_Wave_TRI_CF_set_user = 1.0 / Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[ WAVE_TRI ].all;


        //
        // SIN及SQU 不動
        //
        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ WAVE_SIN ] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[ WAVE_SIN ].all;
        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ WAVE_SQU ] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[ WAVE_SQU ].all;


    }

}







/////////////////////////////////////////////////////////////////////
//
// User設定輸出ON/OFF事件
//
void Set_Output_ON_OFF_Event(void)
{

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_Output_ON_OFF_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user;


}


/////////////////////////////////////////////////////////////////////
//
// 寫入校正參數區, 檢查CheckSum是否正確
//
void Check_Cali_CheckSum_Event(void)
{
    Uint16 Checksum, i;

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Check_Cali_CheckSum_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    Checksum = 0;

    Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[0].Data_Version;

    for ( i = 0; i < 2; i ++ )
    {
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Scale_P.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Scale_P.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Scale_N.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Scale_N.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Scale_P.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Scale_P.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Scale_N.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Scale_N.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].Cs_mpy_360000.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].Cs_mpy_360000.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].inv_Rs.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].inv_Rs.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].VSET_to_DAC_Scale.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].VSET_to_DAC_Scale.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].VSET_to_DAC_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].VSET_to_DAC_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_a.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_a.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_b.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_b.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_c.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_c.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].V_rms_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].V_rms_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].I_rms_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].I_rms_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_EXTIN_L1_Scale.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_EXTIN_L1_Scale.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_EXTIN_L1_offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].ADC_to_EXTIN_L1_offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].p_V_peak_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].p_V_peak_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].n_V_peak_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].n_V_peak_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].p_I_peak_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].p_I_peak_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].n_I_peak_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow[i].n_I_peak_Offset.MSB;
    }


    if ( Global_Variables.Calibration_Variables_shadow[0].CheckSum == Checksum )
    {
        //
        // 檢查CheckSum PASS
        //

        Global_Variables.Calibration_Variables.CheckSum = Global_Variables.Calibration_Variables_shadow[0].CheckSum;
        Global_Variables.Calibration_Variables.Data_Version = Global_Variables.Calibration_Variables_shadow[0].Data_Version;

        //
        // 把shadow更新到校正值
        //
        for ( i = 0; i < 2; i ++ )
        {

            Global_Variables.Calibration_Variables_shadow2[i].Data_Version = Global_Variables.Calibration_Variables_shadow[i].Data_Version;
            Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Scale_P.all = Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Scale_P.all;
            Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Scale_N.all = Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Scale_N.all;
            Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Offset.all = Global_Variables.Calibration_Variables_shadow[i].ADC_to_V_FB_Offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Scale_P.all = Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Scale_P.all;
            Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Scale_N.all = Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Scale_N.all;
            Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Offset.all = Global_Variables.Calibration_Variables_shadow[i].ADC_to_I_FB_Offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].Cs_mpy_360000.all = Global_Variables.Calibration_Variables_shadow[i].Cs_mpy_360000.all;
            Global_Variables.Calibration_Variables_shadow2[i].inv_Rs.all = Global_Variables.Calibration_Variables_shadow[i].inv_Rs.all;
            Global_Variables.Calibration_Variables_shadow2[i].VSET_to_DAC_Scale.all = Global_Variables.Calibration_Variables_shadow[i].VSET_to_DAC_Scale.all;
            Global_Variables.Calibration_Variables_shadow2[i].VSET_to_DAC_Offset.all = Global_Variables.Calibration_Variables_shadow[i].VSET_to_DAC_Offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_a.all = Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_a.all;
            Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_b.all = Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_b.all;
            Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_c.all = Global_Variables.Calibration_Variables_shadow[i].Freq_Compensation_coeff_c.all;
            Global_Variables.Calibration_Variables_shadow2[i].V_rms_Offset.all = Global_Variables.Calibration_Variables_shadow[i].V_rms_Offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].I_rms_Offset.all = Global_Variables.Calibration_Variables_shadow[i].I_rms_Offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].ADC_to_EXTIN_L1_Scale.all = Global_Variables.Calibration_Variables_shadow[i].ADC_to_EXTIN_L1_Scale.all;
            Global_Variables.Calibration_Variables_shadow2[i].ADC_to_EXTIN_L1_offset.all = Global_Variables.Calibration_Variables_shadow[i].ADC_to_EXTIN_L1_offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].p_V_peak_Offset.all = Global_Variables.Calibration_Variables_shadow[i].p_V_peak_Offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].n_V_peak_Offset.all = Global_Variables.Calibration_Variables_shadow[i].n_V_peak_Offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].p_I_peak_Offset.all = Global_Variables.Calibration_Variables_shadow[i].p_I_peak_Offset.all;
            Global_Variables.Calibration_Variables_shadow2[i].n_I_peak_Offset.all = Global_Variables.Calibration_Variables_shadow[i].n_I_peak_Offset.all;

        }

        //
        // 切換校正值
        //
        Load_HL_Cali_to_Cali_Event();

        //
        // 觸發重新計算VREF DAC箝制的事件
        //
        Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit = 1;

    }
    else
    {
        //
        // NG
        //

        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WRITE_CALI_VARIABLE_CHECKSUM_NG;

        //
        // 轉態
        //
        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


        error_start_time_reset();

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Calibration_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }

}


/////////////////////////////////////////////////////////////////////
//
// 計算VREF DAC箝制值
//
void Calc_VREFDAC_Limit_Event(void)
{
    float a, b;
    float vref_dac_value;
    float Vmax_limit;

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Calc_VREFDAC_Limit_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
    b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

    //
    // 輸出Vmax計算DAC箝制量
    //
    Vmax_limit = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_max * 0.0001;
    vref_dac_value = ( Vmax_limit * a ) + b;

    //
    // 更新到CLA
    //
    CPU_to_CLA_Variables.VREF_set_limit_High = vref_dac_value;
    CPU_to_CLA_Variables.VREF_set_limit_Low = b;

}

/////////////////////////////////////////////////////////////////////
//
// SCIA echo
//
void SCIA_echo_Event(void)
{
    //
    // echo
    //
    scia_TXwrite_char( SCI_Module_Variables.SCIA_echo_Data , 1 ); //4,575,499 cycles

}

/////////////////////////////////////////////////////////////////////
//
// 接收SCIA命令處理
//
void SCIA_ask_service_Event(void)
{
    Uint16 i;

    //
    // echo
    //
    SCI_Module_Variables.SCIA_echo_Data[0] = '\r';
    scia_TXwrite_char( SCI_Module_Variables.SCIA_echo_Data , 1 ); //4,575,499 cycles
    SCI_Module_Variables.SCIA_echo_Data[0] = '>';
    scia_TXwrite_char( SCI_Module_Variables.SCIA_echo_Data , 1 ); //4,575,499 cycles
    scia_TXwrite_char( SCI_Module_Variables.SCIA_Rx_Data , SCI_Module_Variables.SCIA_Rx_Data_Count ); //4,575,499 cycles

    //
    // 箝制
    //
    if ( SCI_Module_Variables.SCIA_Rx_Data_Count > 100 )
    {
        SCI_Module_Variables.SCIA_Rx_Data_Count = 100;
    }

    //
    // 擷取Enter之前的字串
    //
    for ( i = 0; i < SCI_Module_Variables.SCIA_Rx_Data_Count; i ++ )
    {
        SCI_Module_Variables.SCIA_Rx_Data_shadow[i] = SCI_Module_Variables.SCIA_Rx_Data[i];
    }

    //
    // 清掉其餘字串
    //
    for ( i = SCI_Module_Variables.SCIA_Rx_Data_Count + 1; i < 100; i ++ )
    {
        SCI_Module_Variables.SCIA_Rx_Data_shadow[i] = 0;
    }



    //
    // Debug協議
    //

    //
    // 測試用
    //
    if ( strcmp( SCI_Module_Variables.SCIA_Rx_Data_shadow, "help\r" ) == 0 )
    {
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > help\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    }

    //
    // 解析SPI
    //
    if ( strncmp( SCI_Module_Variables.SCIA_Rx_Data_shadow, "0x", 2 ) == 0 )
    {
        //
        // 接收到的ASCII字元轉成Uint16
        //
        SCI_Module_Variables.SCIA_Rx_Data2[0] = hex_ascii_to_uint16( SCI_Module_Variables.SCIA_Rx_Data_shadow[2] ) * 4096;
        SCI_Module_Variables.SCIA_Rx_Data2[0] += hex_ascii_to_uint16( SCI_Module_Variables.SCIA_Rx_Data_shadow[3] ) * 256;
        SCI_Module_Variables.SCIA_Rx_Data2[0] += hex_ascii_to_uint16( SCI_Module_Variables.SCIA_Rx_Data_shadow[4] ) * 16;
        SCI_Module_Variables.SCIA_Rx_Data2[0] += hex_ascii_to_uint16( SCI_Module_Variables.SCIA_Rx_Data_shadow[5] ) * 1;

        SCI_Module_Variables.SCIA_Rx_Data2[1] = hex_ascii_to_uint16( SCI_Module_Variables.SCIA_Rx_Data_shadow[6] ) * 4096;
        SCI_Module_Variables.SCIA_Rx_Data2[1] += hex_ascii_to_uint16( SCI_Module_Variables.SCIA_Rx_Data_shadow[7] ) * 256;
        SCI_Module_Variables.SCIA_Rx_Data2[1] += hex_ascii_to_uint16( SCI_Module_Variables.SCIA_Rx_Data_shadow[8] ) * 16;
        SCI_Module_Variables.SCIA_Rx_Data2[1] += hex_ascii_to_uint16( SCI_Module_Variables.SCIA_Rx_Data_shadow[9] ) * 1;

        //
        // SPI指令解析
        //
        CMD_Parser_Process( SCI_Module_Variables.SCIA_Rx_Data2, SCI_Module_Variables.SCIA_Tx_Data2 );

        //
        // 回傳的Uint16轉成ASCII
        //
        SCI_Module_Variables.SCIA_Tx_Data[0] = '<';
        SCI_Module_Variables.SCIA_Tx_Data[1] = '0';
        SCI_Module_Variables.SCIA_Tx_Data[2] = 'x';

        SCI_Module_Variables.SCIA_Tx_Data[3] = hex_uint16_to_ascii( ( SCI_Module_Variables.SCIA_Tx_Data2[0] >> 12 ) & 0x0F );
        SCI_Module_Variables.SCIA_Tx_Data[4] = hex_uint16_to_ascii( ( SCI_Module_Variables.SCIA_Tx_Data2[0] >> 8 ) & 0x0F );
        SCI_Module_Variables.SCIA_Tx_Data[5] = hex_uint16_to_ascii( ( SCI_Module_Variables.SCIA_Tx_Data2[0] >> 4 ) & 0x0F );
        SCI_Module_Variables.SCIA_Tx_Data[6] = hex_uint16_to_ascii( SCI_Module_Variables.SCIA_Tx_Data2[0] & 0x0F);

        SCI_Module_Variables.SCIA_Tx_Data[7] = hex_uint16_to_ascii( ( SCI_Module_Variables.SCIA_Tx_Data2[1] >> 12 ) & 0x0F );
        SCI_Module_Variables.SCIA_Tx_Data[8] = hex_uint16_to_ascii( ( SCI_Module_Variables.SCIA_Tx_Data2[1] >> 8 ) & 0x0F );
        SCI_Module_Variables.SCIA_Tx_Data[9] = hex_uint16_to_ascii( ( SCI_Module_Variables.SCIA_Tx_Data2[1] >> 4 ) & 0x0F );
        SCI_Module_Variables.SCIA_Tx_Data[10] = hex_uint16_to_ascii( SCI_Module_Variables.SCIA_Tx_Data2[1] & 0x0F );

        SCI_Module_Variables.SCIA_Tx_Data[11] = '\n';

        scia_TXwrite_char( SCI_Module_Variables.SCIA_Tx_Data , 12 ); //4,575,499 cycles

    }


    //
    // Debug協議
    //


    SCI_Module_Variables.SCIA_Rx_Data_Count = 0;

}



////////////////////////////////////////////////////////////////////
//
// 轉換16進制 char to Uint16
//
Uint16 hex_ascii_to_uint16( char char_value )
{
    Uint16 uint16_value;

    switch( (Uint16)char_value )
    {
        /*-----------------------------------------------------------------------------------------*/
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        {
            uint16_value = (Uint16)char_value - 48;
            break;
        }
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        case 70:
        {
            uint16_value = (Uint16)char_value - 55;
            break;
        }
        case 97:
        case 98:
        case 99:
        case 100:
        case 101:
        case 102:
        {
            uint16_value = (Uint16)char_value - 87;
            break;
        }
        default:
        {
            uint16_value = 0;
            break;
        }
    }

    return uint16_value;
}


////////////////////////////////////////////////////////////////////
//
// 轉換16進制  Uint16 to char
//
char hex_uint16_to_ascii( Uint16 uint16_value )
{
    char char_value;

    switch( uint16_value )
    {
        /*-----------------------------------------------------------------------------------------*/
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        {
            char_value = (char)( uint16_value + 48 );
            break;
        }
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        {
            char_value = (char)( uint16_value + 55 );
            break;
        }
        default:
        {
            char_value = (char)( 63 );
            break;
        }
    }

    return char_value;
}



////////////////////////////////////////////////////////////////////
void Input_Flags_Update_Event(void)
{
    Uint16 buf;


    buf = Read_74HCT165_Sequence(); //2633 cycle


    switch ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series )
    {
        case ASR_5075:
        {

            buf = buf >> 2;


            #ifdef DEVELOP_SV52
            Global_Variables.INPUT_FLAGS.bit.DC_OVP = 1;
            #else
            //
            // got DC_OVP
            //
            Global_Variables.INPUT_FLAGS.bit.DC_OVP = 0x01 & buf;
            #endif

            buf = buf >> 1;

            //
            // got FANFAIL
            //
            Global_Variables.INPUT_FLAGS.bit.FANFAIL = 0x01 & buf;

            buf = buf >> 1;


            //
            // got PFC_UVP
            //
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP = 0x01 & buf;

            buf = buf >> 1;

            //
            // got PFC_OVP
            //
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP = 0x01 & buf;

            buf = buf >> 1;


            //
            // got ACFAIL
            //
#ifdef DEVELOP_SV59
            Global_Variables.INPUT_FLAGS.bit.ACFAIL = 0x01 & buf;
            buf = buf >> 1;
#else
            //
            // buf存入shadow
            //
            Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow3 = Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow2;
            Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow2 = Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow1;
            Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow1 = 0x01 & buf;

            //
            // 狀態一致才更新ACFAIL
            //
            if ( ( Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow1 == Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow2 )
                 && ( Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow2 == Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow3 ) )
            {
                Global_Variables.INPUT_FLAGS.bit.ACFAIL = Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow3;
            }
            buf = buf >> 1;

#endif


            //
            // got PFC_OTP
            //
            Global_Variables.INPUT_FLAGS.bit.PFC_OTP = 0x01 & buf;

            // 對半寬而言以下電路上是浮接,需要初始化
            Global_Variables.INPUT_FLAGS.bit.FANFAIL1 = 0;
            Global_Variables.INPUT_FLAGS.bit.FANFAIL2 = 0;
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP1 = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP1 = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP2 = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP2 = 1;

            break;
        }
        case ASR_5150:
        {
            //
            // got PFC_UVP2
            //
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP2 = 0x01 & buf;

            buf = buf >> 1;

            //
            // got PFC_OVP2
            //
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP2 = 0x01 & buf;

            buf = buf >> 1;


            #ifdef DEVELOP_SV52
            Global_Variables.INPUT_FLAGS.bit.DC_OVP = 1;
            #else
            //
            // got DC_OVP
            //
            Global_Variables.INPUT_FLAGS.bit.DC_OVP = 0x01 & buf;
            #endif

            buf = buf >> 1;

            //
            // got FANFAIL
            //
            Global_Variables.INPUT_FLAGS.bit.FANFAIL = 0x01 & buf;

            buf = buf >> 1;

            //
            // got PFC_UVP1
            //
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP1 = 0x01 & buf;

            buf = buf >> 1;

            //
            // got PFC_OVP1
            //
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP1 = 0x01 & buf;

            buf = buf >> 1;

            //
            // got ACFAIL
            //

#ifdef DEVELOP_SV59
            Global_Variables.INPUT_FLAGS.bit.ACFAIL = 0x01 & buf;
            buf = buf >> 1;
#else
            //
            // buf存入shadow
            //
            Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow3 = Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow2;
            Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow2 = Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow1;
            Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow1 = 0x01 & buf;

            //
            // 狀態一致才更新ACFAIL
            //
            if ( ( Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow1 == Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow2 )
                 && ( Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow2 == Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow3 ) )
            {
                Global_Variables.INPUT_FLAGS.bit.ACFAIL = Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow3;
            }
            buf = buf >> 1;

#endif

            //
            // got PFC_OTP
            //
            Global_Variables.INPUT_FLAGS.bit.PFC_OTP = 0x01 & buf;

            // 對全寬而言以下電路上是浮接,需要初始化
            Global_Variables.INPUT_FLAGS.bit.FANFAIL1 = 0;
            Global_Variables.INPUT_FLAGS.bit.FANFAIL2 = 0;
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP = 1;


            break;
        }

    }
}



/////////////////////////////////////////////////////////////////////
void Load_HL_Cali_Variable_from_EEPROM_Event(void)
{

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Load_HL_Cali_Variable_from_EEPROM_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    // 校正值版本
    //
    Read_EEPROM_addr_to_Buffer( CALI_DATA_VER_EEPROM_ADDR );
    Global_Variables.Calibration_Variables.Data_Version = EEPROM_Module_Variables.Read_buffer.MSB;

    //
    // LR, Low Range Section
    //
    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_V_FB_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_I_FB_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_FREQ_TO_VRMS_A_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_a.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_FREQ_TO_VRMS_B_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_b.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_FREQ_TO_VRMS_C_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_c.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_CS_MPY_360000_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Cs_mpy_360000.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_INV_RS_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].inv_Rs.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_V_RMS_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].V_rms_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_I_RMS_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].I_rms_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_P_V_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].p_V_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_N_V_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].n_V_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_P_I_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].p_I_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_N_I_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].n_I_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_V_FB_SCALE_P_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_P.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_V_FB_SCALE_N_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_N.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_I_FB_SCALE_P_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_P.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_ADC_TO_I_FB_SCALE_N_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_N.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_VSET_TO_DAC_SCALE_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Scale.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_VSET_TO_DAC_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_ADC_to_EXTIN_L1_SCALE_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_Scale.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( LR_ADC_to_EXTIN_L1_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.all = EEPROM_Module_Variables.Read_buffer.all;

    //
    // HR, High Range Section
    //
    Read_EEPROM_addr_to_Buffer( HR_ADC_TO_V_FB_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_ADC_TO_I_FB_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_FREQ_TO_VRMS_A_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_a.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_FREQ_TO_VRMS_B_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_b.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_FREQ_TO_VRMS_C_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_c.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_CS_MPY_360000_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Cs_mpy_360000.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_INV_RS_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].inv_Rs.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_V_RMS_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].V_rms_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_I_RMS_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].I_rms_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_P_V_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].p_V_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_N_V_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].n_V_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_P_I_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].p_I_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_N_I_PEAK_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].n_I_peak_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_ADC_TO_V_FB_SCALE_P_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_ADC_TO_V_FB_SCALE_N_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_ADC_TO_I_FB_SCALE_P_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_ADC_TO_I_FB_SCALE_N_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_VSET_TO_DAC_SCALE_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Scale.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_VSET_TO_DAC_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Offset.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_ADC_to_EXTIN_L1_SCALE_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_Scale.all = EEPROM_Module_Variables.Read_buffer.all;
    Read_EEPROM_addr_to_Buffer( HR_ADC_to_EXTIN_L1_OFFSET_EEPROM_ADDR );
    Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.all = EEPROM_Module_Variables.Read_buffer.all;

    //
    // 之後檢查Cali版本如果不一致, 則新加入的校正參數載入初始值
    //
    /*
    if ( Global_Variables.Calibration_Variables.Data_Version > msb )
    {
        //
        // 加在這裡
        //
    }
    */

}


/////////////////////////////////////////////////////////////////////
//
// HL校正值CheckSum計算
//
void Update_HL_Cali_CheckSum_Event(void)
{
    Uint16 Checksum;
    Uint16 i;

    //debug msg
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Update_HL_Cali_CheckSum_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    Checksum = 0;

    Checksum = Checksum + Global_Variables.Calibration_Variables.Data_Version;

    for ( i = 0; i < 2; i ++ )
    {
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Scale_P.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Scale_P.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Scale_N.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Scale_N.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Scale_P.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Scale_P.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Scale_N.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Scale_N.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].Cs_mpy_360000.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].Cs_mpy_360000.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].inv_Rs.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].inv_Rs.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].VSET_to_DAC_Scale.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].VSET_to_DAC_Scale.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].VSET_to_DAC_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].VSET_to_DAC_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_a.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_a.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_b.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_b.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_c.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_c.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].V_rms_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].V_rms_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].I_rms_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].I_rms_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_EXTIN_L1_Scale.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_EXTIN_L1_Scale.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_EXTIN_L1_offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].ADC_to_EXTIN_L1_offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].p_V_peak_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].p_V_peak_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].n_V_peak_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].n_V_peak_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].p_I_peak_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].p_I_peak_Offset.MSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].n_I_peak_Offset.LSB;
        Checksum = Checksum + Global_Variables.Calibration_Variables_shadow2[i].n_I_peak_Offset.MSB;
    }


    Global_Variables.Calibration_Variables.CheckSum = Checksum;
}


/////////////////////////////////////////////////////////////////////
//
// 判斷H/L Range把校正值shadow載入到實際使用的校正值
//
void Load_HL_Cali_to_Cali_Event(void)
{
    Uint16 range;

    range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range;

    Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_V_FB_Scale_P.all;
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_V_FB_Scale_N.all;
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_V_FB_Offset.all;
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_I_FB_Scale_P.all;
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_I_FB_Scale_N.all;
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_I_FB_Offset.all;
    Global_Variables.Calibration_Variables.Cs_mpy_360000.all = Global_Variables.Calibration_Variables_shadow2[ range ].Cs_mpy_360000.all;
    Global_Variables.Calibration_Variables.inv_Rs.all = Global_Variables.Calibration_Variables_shadow2[ range ].inv_Rs.all;
    Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all = Global_Variables.Calibration_Variables_shadow2[ range ].VSET_to_DAC_Scale.all;
    Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].VSET_to_DAC_Offset.all;
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all = Global_Variables.Calibration_Variables_shadow2[ range ].Freq_Compensation_coeff_a.all;
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all = Global_Variables.Calibration_Variables_shadow2[ range ].Freq_Compensation_coeff_b.all;
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all = Global_Variables.Calibration_Variables_shadow2[ range ].Freq_Compensation_coeff_c.all;
    Global_Variables.Calibration_Variables.V_rms_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].V_rms_Offset.all;
    Global_Variables.Calibration_Variables.I_rms_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].I_rms_Offset.all;
    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_EXTIN_L1_Scale.all;
    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_EXTIN_L1_offset.all;
    Global_Variables.Calibration_Variables.p_V_peak_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].p_V_peak_Offset.all;
    Global_Variables.Calibration_Variables.n_V_peak_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].n_V_peak_Offset.all;
    Global_Variables.Calibration_Variables.p_I_peak_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].p_I_peak_Offset.all;
    Global_Variables.Calibration_Variables.n_I_peak_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].n_I_peak_Offset.all;

    //
    // 更新到CLA
    //
    CPU_to_CLA_Variables.ADC_to_V_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;
    CPU_to_CLA_Variables.Cs_mpy_360000 = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;
    CPU_to_CLA_Variables.inv_Rs = Global_Variables.Calibration_Variables.inv_Rs.all;
    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;
    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;

    //
    // 觸發重新計算VREF DAC箝制的事件
    //
    Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit = 1;

    //
    // 其他更新
    //
    Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale = 1 / Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all;
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level = (Uint16)Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;


}


/////////////////////////////////////////////////////////////////////
void Set_Output_Range_Option_Event(void)
{
    switch ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series )
    {
        case ASR_5075:
        {
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = FIX_LOW_RANGE;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = FIX_LOW_RANGE;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = LOW_RANGE;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

            break;
        }
        case ASR_5150:
        {
            if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option )
            {
                //
                // 設定值不一樣才處理
                //

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user;

                //
                // 設定Output_Range_shadow
                //
                switch ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option )
                {
                    case FIX_LOW_RANGE:
                    {
                        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;

                        break;
                    }
                    case FIX_HIGH_RANGE:
                    {
                        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;

                        break;
                    }
                    case AUTO_RANGE:
                    {
                        //
                        // 判斷 Range 事件
                        //
                        Adj_Output_Range_Event();

                        break;
                    }
                }


                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
                {
                    //
                    // Output_Range_shadow與目前設定不一致
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow;

                    //
                    // Range改變, 觸發事件
                    //
                    Event_Module.Event4_Flag.bit.Change_Output_Range = 1;
                }
            }

            break;
        }
    }
}


/////////////////////////////////////////////////////////////////////
void Change_Output_Range_Event(void)
{
    //
    // !!!要確保機器沒有輸出才能切檔位!!!
    //

    switch ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range )
    {
        case LOW_RANGE: // ( 對應原本5150 )
        {
            //
            // 機器額定參數
            //
            Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout = ASR_5150_LR_MAX_AC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout = ASR_5150_LR_MAX_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout = ASR_5150_LR_MIN_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout = ASR_5150_LR_MAX_I;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak = ASR_5150_LR_MAX_I_PEAK;


            Global_Variables.POWER_STATUS.bit.V_Range = LOW_RANGE;
            Global_Variables.POWER_STATUS.bit.I_Range = HIGH_RANGE;

            Global_Variables.OUTPUT_FLAGS.bit.VM_HI_LO = LOW_RANGE;
            Global_Variables.OUTPUT_FLAGS.bit.IM_HI_LO = HIGH_RANGE;
            Global_Variables.OUTPUT_FLAGS.bit.OUT_HI_LO = LOW_RANGE;

            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.VMONI_LO = LOW_RANGE;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.IMONI_LO = HIGH_RANGE;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Change_Output_Range_Event -> LOW\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }

        case HIGH_RANGE: // ( 對應原本5151 )
        {
            //
            // 機器額定參數
            //
            Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout = ASR_5150_HR_MAX_AC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout = ASR_5150_HR_MAX_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout = ASR_5150_HR_MIN_DC_V;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout = ASR_5150_HR_MAX_I;
            Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak = ASR_5150_HR_MAX_I_PEAK;

            Global_Variables.POWER_STATUS.bit.V_Range = HIGH_RANGE;
            Global_Variables.POWER_STATUS.bit.I_Range = LOW_RANGE;

            Global_Variables.OUTPUT_FLAGS.bit.VM_HI_LO = HIGH_RANGE;
            Global_Variables.OUTPUT_FLAGS.bit.IM_HI_LO = LOW_RANGE;
            Global_Variables.OUTPUT_FLAGS.bit.OUT_HI_LO = HIGH_RANGE;

            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.VMONI_LO = HIGH_RANGE;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.IMONI_LO = LOW_RANGE;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Change_Output_Range_Event -> HIGH\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            break;
        }
    }

    //
    // ACV = 額定 x 1.1
    //
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout * 11000.0 );
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_min = 0;

    //
    // DCV = 額定 x 1.1
    //
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 11000.0 );
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_min = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Minimum_Vout * 11000.0 );

    //
    // I RMS = 額定 x 1.05
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout * 10500.0 );

    //
    // 換檔後, Irms Limit設定
    // 參考ASR-2000
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max;
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2 = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;

    //
    // IPK max = 額定 x 1.05
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max = (int32)( Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak * 10500.0 );
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max = (int32)( -Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak * 10500.0 );

    //
    // 換檔後, Ipk Limit設定
    // 參考ASR-2000
    //
    if ( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all > Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max )
    {
        Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max;
        Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_max;
    }

    if ( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all < Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max )
    {
        Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max;
        Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_max;
    }

    //
    // OVP更新
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.V_rms_limit_level = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_max * 0.0001;

    //
    // 4倍CF OCP
    //
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES._4CF_OCP_level = Global_Variables.MACHINE_SERIES_IMFORMATION.Maximum_Iout_peak;
    CPU_to_CLA_Variables._4CF_OCP_level = Global_Variables.POWER_PROTECT_SETTING_VARIABLES._4CF_OCP_level;

    Global_Variables.Pre_Calc_Variables.inv_Vmax = 1.0 / ( Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 1.01 );

    //
    // 切換校正值
    //

    //Calc_VREFDAC_Limit_Event

}



/////////////////////////////////////////////////////////////////////
void Adj_Output_Range_Event(void)
{

    //
    // 指定OUTPUT_MODE輸出流程進入初始化狀態
    //
    switch(Global_Variables.OUTPUT_MODE_FLAG_Shadow.all)
    {
        case DC_INT_MODE: //DC-INT
        case MASTER_DC_INT_MODE:
        case SLAVE_DC_INT_MODE:
        {
            //
            // DC
            //

            float dcv_set;
            float p_Vpk, n_Vpk;
            float p_Vpk_level, n_Vpk_level;

            dcv_set = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all * 0.0001;

            p_Vpk = dcv_set;
            n_Vpk = dcv_set;

            p_Vpk_level = ASR_5150_LR_MAX_DC_V + 0.1;
            n_Vpk_level = ASR_5150_LR_MIN_DC_V - 0.1;


            if ( ( p_Vpk >= p_Vpk_level ) || ( n_Vpk <= n_Vpk_level ) )
            {
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;
            }
            else
            {
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;
            }

            break;
        }
        case AC_INT_MODE: //AC-INT
        case AC_ADD_MODE: //AC-ADD
        case AC_SYNC_MODE: //AC-SYNC
        case AC_VCA_MODE: //AC-VCA
        case MASTER_AC_INT_MODE:
        case SLAVE_AC_INT_MODE:
        {
            //
            // AC
            //

            float acv_set, cf_value, acv_peak;
            float p_Vpk, n_Vpk;
            float p_Vpk_level, n_Vpk_level;

            acv_set = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all * 0.0001;
            cf_value = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user ];
            acv_peak = (float)acv_set * cf_value;

            if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_SQU )
                 && ( acv_peak > ASR_5150_LR_MAX_AC_V ) )
            {
                //
                // 參照ASR-2000, 與其作法一致
                //

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;
            }
            else
            {
                p_Vpk = +acv_peak;
                n_Vpk = -acv_peak;

                p_Vpk_level = ASR_5150_LR_MAX_DC_V + 0.1;
                n_Vpk_level = ASR_5150_LR_MIN_DC_V - 0.1;


                if ( ( p_Vpk >= p_Vpk_level ) || ( n_Vpk <= n_Vpk_level ) )
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;
                }
                else
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;
                }
            }

            break;
        }
        case ACDC_INT_MODE: //ACDC-INT
        case ACDC_ADD_MODE: //ACDC-ADD
        case ACDC_SYNC_MODE: //ACDC-SYNC
        case MASTER_ACDC_INT_MODE:
        case SLAVE_ACDC_INT_MODE:
        {
            //
            // AC+DC
            //

            float acv_set, cf_value, acv_peak;
            float dcv_set;
            float p_Vpk, n_Vpk;
            float p_Vpk_level, n_Vpk_level;

            acv_set = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all * 0.0001;
            cf_value = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user ];
            acv_peak = (float)acv_set * cf_value;

            dcv_set = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all * 0.0001;

            if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user == WAVE_SQU )
                 && ( acv_peak > ASR_5150_LR_MAX_AC_V ) )
            {
                //
                // 參照ASR-2000, 與其作法一致
                //

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;
            }
            else
            {
                p_Vpk = dcv_set + acv_peak;
                n_Vpk = dcv_set - acv_peak;

                p_Vpk_level = ASR_5150_LR_MAX_DC_V + 0.1;
                n_Vpk_level = ASR_5150_LR_MIN_DC_V - 0.1;


                if ( ( p_Vpk >= p_Vpk_level ) || ( n_Vpk <= n_Vpk_level ) )
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = HIGH_RANGE;
                }
                else
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_shadow = LOW_RANGE;
                }
            }

            break;
        }
        case SEQ_MODE: //SEQ
        case AC_EXT_MODE: //AC-EXT
        case ACDC_EXT_MODE: //ACDC-EXT
        default:
        {
            break;
        }
    }

}



/////////////////////////////////////////////////////////////////////
void Save_HL_Cali_Variable_to_EEPROM_Event(void)
{
    Uint16 write_err_counter;

    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Save_HL_Cali_Variable_to_EEPROM_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    write_err_counter = 0;

    EEPROM_Control_Sequence( EEPROM_EWEN, 0 );

    //
    // 較正值版本
    //
    EEPROM_Module_Variables.Write_buffer.MSB = Global_Variables.Calibration_Variables.Data_Version;
    EEPROM_Module_Variables.Write_buffer.LSB = Global_Variables.Calibration_Variables.Data_Version;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( CALI_DATA_VER_EEPROM_ADDR );

    //
    // LR, Low Range Section
    //
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_V_FB_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_I_FB_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_a.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_FREQ_TO_VRMS_A_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_b.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_FREQ_TO_VRMS_B_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_c.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_FREQ_TO_VRMS_C_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Cs_mpy_360000.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_CS_MPY_360000_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].inv_Rs.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_INV_RS_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].V_rms_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_V_RMS_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].I_rms_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_I_RMS_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].p_V_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_P_V_PEAK_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].n_V_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_N_V_PEAK_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].p_I_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_P_I_PEAK_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].n_I_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_N_I_PEAK_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_P.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_V_FB_SCALE_P_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_N.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_V_FB_SCALE_N_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_P.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_I_FB_SCALE_P_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_N.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_TO_I_FB_SCALE_N_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Scale.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_VSET_TO_DAC_SCALE_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_VSET_TO_DAC_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_Scale.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_to_EXTIN_L1_SCALE_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( LR_ADC_to_EXTIN_L1_OFFSET_EEPROM_ADDR );


    //
    // HR, High Range Section
    //
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_ADC_TO_V_FB_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_ADC_TO_I_FB_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_a.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_FREQ_TO_VRMS_A_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_b.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_FREQ_TO_VRMS_B_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_c.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_FREQ_TO_VRMS_C_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Cs_mpy_360000.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_CS_MPY_360000_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].inv_Rs.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_INV_RS_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].V_rms_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_V_RMS_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].I_rms_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_I_RMS_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].p_V_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_P_V_PEAK_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].n_V_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_N_V_PEAK_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].p_I_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_P_I_PEAK_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].n_I_peak_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_N_I_PEAK_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_ADC_TO_V_FB_SCALE_P_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_ADC_TO_V_FB_SCALE_N_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_ADC_TO_I_FB_SCALE_P_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_ADC_TO_I_FB_SCALE_N_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Scale.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_VSET_TO_DAC_SCALE_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_VSET_TO_DAC_OFFSET_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_Scale.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_ADC_to_EXTIN_L1_SCALE_EEPROM_ADDR );
    EEPROM_Module_Variables.Write_buffer.all = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.all;
    write_err_counter = write_err_counter + Write_Buffer_to_EEPROM_addr( HR_ADC_to_EXTIN_L1_OFFSET_EEPROM_ADDR );



    /*----------------------------------------------------------------------*/
    if ( write_err_counter > 0 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = EEPROM_WRITE_NG;

        //
        // 轉態
        //
        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


        error_start_time_reset();

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Calibration_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        //
        #ifdef DEBUG_ON
        sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Write Error Counter -> %u\n", CpuTimer2.InterruptCount, write_err_counter ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }

    EEPROM_Control_Sequence( EEPROM_EWDS, 0 );
    ACFAIL_start_time_reset();

}


/////////////////////////////////////////////////////////////////////
//
// 電壓校正前參數初始化
// 只針對電壓校正時的校正參數初始化
//
void Set_V_HL_Cali_Variable_default_Event(void)
{
    Uint16 i;
    Uint16 range;
    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_V_HL_Cali_Variable_default_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    // 較正值版本
    //
    Global_Variables.Calibration_Variables.Data_Version = CALI_DATA_VER;

    //
    // 分機種
    //
    switch ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series )
    {
        case ASR_5075:
        {
            for ( i = 0; i < 2; i ++ )
            {
                Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Offset.all = ADC_TO_V_FB_OFFSET_5075;
                Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Scale_P.all = ADC_TO_V_FB_SCALE_P_5075;
                Global_Variables.Calibration_Variables_shadow2[i].ADC_to_V_FB_Scale_N.all = ADC_TO_V_FB_SCALE_N_5075;
                Global_Variables.Calibration_Variables_shadow2[i].VSET_to_DAC_Scale.all = VSET_to_DAC_SCALE_5075;
                Global_Variables.Calibration_Variables_shadow2[i].VSET_to_DAC_Offset.all = VSET_to_DAC_OFFSET_5075;
                Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_a.all = FREQ_TO_VRMS_A_5075;
                Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_b.all = FREQ_TO_VRMS_B_5075;
                Global_Variables.Calibration_Variables_shadow2[i].Freq_Compensation_coeff_c.all = FREQ_TO_VRMS_C_5075;
            }

            break;
        }

        case ASR_5150:
        {
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Offset.all = ADC_TO_V_FB_OFFSET_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_P.all = ADC_TO_V_FB_SCALE_P_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_N.all = ADC_TO_V_FB_SCALE_N_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Scale.all = VSET_to_DAC_SCALE_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Offset.all = VSET_to_DAC_OFFSET_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_a.all = FREQ_TO_VRMS_A_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_b.all = FREQ_TO_VRMS_B_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_c.all = FREQ_TO_VRMS_C_5150;

            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Offset.all = ADC_TO_V_FB_OFFSET_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.all = ADC_TO_V_FB_SCALE_P_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.all = ADC_TO_V_FB_SCALE_N_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Scale.all = VSET_to_DAC_SCALE_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Offset.all = VSET_to_DAC_OFFSET_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_a.all = FREQ_TO_VRMS_A_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_b.all = FREQ_TO_VRMS_B_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_c.all = FREQ_TO_VRMS_C_5151;

            break;
        }
    }

    //
    // 共用區
    //
    for ( i = 0; i < 2; i ++ )
    {
        Global_Variables.Calibration_Variables_shadow2[i].V_rms_Offset.all = V_RMS_OFFSET;
        Global_Variables.Calibration_Variables_shadow2[i].p_V_peak_Offset.all = P_V_PEAK_OFFSET;
        Global_Variables.Calibration_Variables_shadow2[i].n_V_peak_Offset.all = N_V_PEAK_OFFSET;
    }


    //
    // 判斷range載入
    //
    range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range;

    Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_V_FB_Offset.all;
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_V_FB_Scale_P.all;
    Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_V_FB_Scale_N.all;
    Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all = Global_Variables.Calibration_Variables_shadow2[ range ].VSET_to_DAC_Scale.all;
    Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].VSET_to_DAC_Offset.all;
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_a.all = Global_Variables.Calibration_Variables_shadow2[ range ].Freq_Compensation_coeff_a.all;
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_b.all = Global_Variables.Calibration_Variables_shadow2[ range ].Freq_Compensation_coeff_b.all;
    Global_Variables.Calibration_Variables.Freq_Compensation_coeff_c.all = Global_Variables.Calibration_Variables_shadow2[ range ].Freq_Compensation_coeff_c.all;
    Global_Variables.Calibration_Variables.V_rms_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].V_rms_Offset.all;
    Global_Variables.Calibration_Variables.p_V_peak_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].p_V_peak_Offset.all;
    Global_Variables.Calibration_Variables.n_V_peak_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].n_V_peak_Offset.all;

    //
    // 更新到CLA
    //
    CPU_to_CLA_Variables.ADC_to_V_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_V_FB_Offset.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Offset = Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all;
    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_P.all;
    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_V_FB_Scale_N.all;

    //
    // 觸發重新計算VREF DAC箝制的事件
    //
    Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit = 1;

}


/////////////////////////////////////////////////////////////////////
//
// 電流校正前參數初始化
// 只針對電流校正時的校正參數初始化
//
void Set_I_HL_Cali_Variable_default_Event(void)
{
    Uint16 i;
    Uint16 range;

    //
    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_I_HL_Cali_Variable_default_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    // 分機種
    //
    switch ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series )
    {
        case ASR_5075:
        {
            for ( i = 0; i < 2; i ++ )
            {
                Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Offset.all = ADC_TO_I_FB_OFFSET_5075;
                Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Scale_P.all = ADC_TO_I_FB_SCALE_P_5075;
                Global_Variables.Calibration_Variables_shadow2[i].ADC_to_I_FB_Scale_N.all = ADC_TO_I_FB_SCALE_N_5075;
                Global_Variables.Calibration_Variables_shadow2[i].IREF_to_DAC_Scale.all = IREF_TO_DAC_SCALE_5075;
                Global_Variables.Calibration_Variables_shadow2[i].IREF_to_DAC_Offset.all = IREF_TO_DAC_OFFSET_5075;
            }

            break;
        }

        case ASR_5150:
        {
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Offset.all = ADC_TO_I_FB_OFFSET_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_P.all = ADC_TO_I_FB_SCALE_P_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_N.all = ADC_TO_I_FB_SCALE_N_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].IREF_to_DAC_Scale.all = IREF_TO_DAC_SCALE_5150;
            Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].IREF_to_DAC_Offset.all = IREF_TO_DAC_OFFSET_5150;

            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Offset.all = ADC_TO_I_FB_OFFSET_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.all = ADC_TO_I_FB_SCALE_P_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.all = ADC_TO_I_FB_SCALE_N_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].IREF_to_DAC_Scale.all = IREF_TO_DAC_SCALE_5151;
            Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].IREF_to_DAC_Offset.all = IREF_TO_DAC_OFFSET_5151;

            break;
        }
    }

    //
    // 共用
    //
    for ( i = 0; i < 2; i ++ )
    {
        Global_Variables.Calibration_Variables_shadow2[i].inv_Rs.all = INV_RS_REF;
        Global_Variables.Calibration_Variables_shadow2[i].Cs_mpy_360000.all = CS_MPY_360000_REF;
        Global_Variables.Calibration_Variables_shadow2[i].I_rms_Offset.all = I_RMS_OFFSET;
        Global_Variables.Calibration_Variables_shadow2[i].p_I_peak_Offset.all = P_I_PEAK_OFFSET;
        Global_Variables.Calibration_Variables_shadow2[i].n_I_peak_Offset.all = N_I_PEAK_OFFSET;
    }

    //
    // 判斷range載入
    //
    range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range;

    Global_Variables.Calibration_Variables.ADC_to_I_FB_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_I_FB_Offset.all;
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_I_FB_Scale_P.all;
    Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_I_FB_Scale_N.all;
    Global_Variables.Calibration_Variables.IREF_to_DAC_Scale.all = Global_Variables.Calibration_Variables_shadow2[ range ].IREF_to_DAC_Scale.all;
    Global_Variables.Calibration_Variables.IREF_to_DAC_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].IREF_to_DAC_Offset.all;
    Global_Variables.Calibration_Variables.inv_Rs.all = Global_Variables.Calibration_Variables_shadow2[ range ].inv_Rs.all;
    Global_Variables.Calibration_Variables.Cs_mpy_360000.all = Global_Variables.Calibration_Variables_shadow2[ range ].Cs_mpy_360000.all;
    Global_Variables.Calibration_Variables.I_rms_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].I_rms_Offset.all;
    Global_Variables.Calibration_Variables.p_I_peak_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].p_I_peak_Offset.all;
    Global_Variables.Calibration_Variables.n_I_peak_Offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].n_I_peak_Offset.all;


    //
    // 更新到CLA
    //
    CPU_to_CLA_Variables.inv_Rs = Global_Variables.Calibration_Variables.inv_Rs.all;
    CPU_to_CLA_Variables.Cs_mpy_360000 = Global_Variables.Calibration_Variables.Cs_mpy_360000.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_P = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_P.all;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_N = Global_Variables.Calibration_Variables.ADC_to_I_FB_Scale_N.all;

}



/////////////////////////////////////////////////////////////////////
//
// EXT校正前參數初始化
// 只針對EXT校正時的校正參數初始化
//
void Set_EXT_HL_Cali_Variable_default_Event(void)
{
    Uint16 i;
    Uint16 range;
    float32 scale, offset;

    #ifdef DEBUG_ON
    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Trig Set_EXT_HL_Cali_Variable_default_Event\n", CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    //
    // 共用
    //
    for ( i = 0; i < 2; i ++ )
    {
        Global_Variables.Calibration_Variables_shadow2[i].ADC_to_EXTIN_L1_Scale.all = ADC_to_EXTIN_L1_SCALE_REF;
        Global_Variables.Calibration_Variables_shadow2[i].ADC_to_EXTIN_L1_offset.all = ADC_to_EXTIN_L1_OFFSET_REF;
    }

    //
    // 判斷range載入
    //
    range = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range;

    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_EXTIN_L1_Scale.all;
    Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all = Global_Variables.Calibration_Variables_shadow2[ range ].ADC_to_EXTIN_L1_offset.all;


    //
    // 更新出去
    //
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level = (Uint16)Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
    Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale = 1 / Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all;

    scale = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_Scale.all;
    offset = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN_TTL_Level_L = (Uint16)(( TTL_LEVEL_L * scale ) + offset );
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN_TTL_Level_H = (Uint16)(( TTL_LEVEL_H * scale ) + offset );


}


/////////////////////////////////////////////////////////////////////
void SEQ_Mode_Refresh_CheckSum_Event(void)
{
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum = 0;

    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section1.SEQ_Config_State1_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].all;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section1.SEQ_Config_State2_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].all;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section1.SEQ_Start_Phase_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section1.SEQ_Stop_Phase_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section2.SEQ_Jump_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section2.SEQ_Jump_Count_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section2.SEQ_Branch1_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section2.SEQ_Branch2_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section3.SEQ_Step_Time_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section3.SEQ_Step_Time_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].LSB;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section5.SEQ_SQU_Duty_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section3.SEQ_Freq_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section3.SEQ_Freq_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].LSB;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section4.SEQ_DCV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section4.SEQ_DCV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].LSB;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section4.SEQ_ACV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB;
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += SEQ_Step_Section4.SEQ_ACV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].LSB;

}






//
// End of file
//
