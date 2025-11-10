//###########################################################################
//
// FILE:    Output_ACDC_INT_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211124
//###########################################################################



#include "Global_VariableDefs.h"


volatile OUTPUT_ACDC_INT_MODULE_VARIABLES_REG Output_ACDC_INT_Module;


/////////////////////////////////////////////////////////////////////
//
// 初始化參數
//
void Init_Output_ACDC_INT_Module_Variables(void)
{
    Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_init;
    Output_ACDC_INT_Module.Output_off_start_time = 0;
    Output_ACDC_INT_Module.Output_on_start_time = 0;

    Output_ACDC_INT_Module.V_Rise_Count = 0;
    Output_ACDC_INT_Module.WAVE_per_Count = 0;
    Output_ACDC_INT_Module.V_Rise_Counter = 0;
    Output_ACDC_INT_Module.DCV_Zero_Stable_Counter = 0;
}




/////////////////////////////////////////////////////////////////////
//
// ACDC INT 輸出流程
//
void ACDC_INT_Output_Process(void)
{
    int32 int32_buf1, int32_buf2;
    Uint32 Uint32_buf1, Uint32_buf2;

    switch(Output_ACDC_INT_Module.Output_ACDC_INT_State)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // 初始化
        //
        case acdc_int_output_init:
        {

            //
            // Preload waveform
            // 根據waveform指定載入的wave_buffer_section
            //
            Change_Waveform_Event();
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;



            //
            // 處理頻率step以及頻率補償
            //
            if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 1 )
            {
                //
                // AC or AC+DC
                //

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 )
                {
                    //
                    // 沒AC輸出
                    //
                    Global_Variables.FreqStep_shadow = DC_FREQ_STEP;
                }
                else
                {
                    Change_FREQ_Event();
                }
            }
            else if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 0 )
                      && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 1 ) )
            {
                //
                // DC
                //
                Global_Variables.FreqStep_shadow = DC_FREQ_STEP;
            }
            Global_Variables.FreqStep = Global_Variables.FreqStep_shadow;

            //
            // Set REF及WAVE DAC
            //
            MS_Mode_Change_ACV_DCV_Event();
            Global_Variables.OUTPUT_VARIABLES.WAVE_offset = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;
            Global_Variables.OUTPUT_VARIABLES.WAVE_scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow;
            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;


            //
            // Start phase
            //
            Set_Start_phase_Event();

            //
            // Stop phase
            //
            Set_Stop_phase_Event();

            //
            // Slew Rate
            //
            Calc_V_SlewRate_set_Event();

            //
            // 處理V_rise_time
            //
            if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == DC_INT_MODE )
                 && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user.all > 0 )
                 && ( Output_ACDC_INT_Module.V_Rise_Bypass_Flag == 0 ) )
            {
                //
                // DC-INT 而且 V_rise_time > 0
                //
                Output_ACDC_INT_Module.V_Rise_Count = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user.all * 1000.0 / CPU0_PERIOD;

                Output_ACDC_INT_Module.WAVE_per_Count = (float)( Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow - 32767 ) / Output_ACDC_INT_Module.V_Rise_Count;

                Output_ACDC_INT_Module.V_Rise_Counter = 0;

                Output_ACDC_INT_Module.DCV_Zero_Stable_Counter = 0;
            }


            //
            // 輸出Relay ON
            //
            Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 1;

            Output_ACDC_INT_Module.Output_on_start_time = CpuTimer2.InterruptCount;

            //
            // 轉態
            //
            Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_on_delay;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_on_delay\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        case acdc_int_output_on_delay:
        {
            if ( Calc_Elapsed_Time( Output_ACDC_INT_Module.Output_on_start_time, CpuTimer2.InterruptCount ) > OUTPUT_ON_DELAY_TIME )
            {

                if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 1 )
                {
                    //
                    // AC or AC+DC
                    //

                    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 )
                    {
                        //
                        // 沒AC輸出
                        //

                        //
                        // ON Phase Disable
                        //
                        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;
                    }
                    else
                    {
                        if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable == 1 )
                        {
                            //
                            // ON Phase Enable
                            //
                            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count;
                        }
                        else
                        {
                            //
                            // ON Phase Disable
                            //
                            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;
                        }
                    }
                }
                else if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 0 )
                          && ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 1 ) )
                {
                    //
                    // DC
                    //

                    //
                    // ON Phase Disable
                    //
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;
                }

                //
                // Reload dds
                //
                Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;

                //
                // 處理V_rise_time
                //
                if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == DC_INT_MODE )
                     && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all != 0 )
                     && ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user.all > 0 )
                     && ( Output_ACDC_INT_Module.V_Rise_Bypass_Flag == 0 ) )
                {
                    //
                    // DC-INT 而且 V_rise_time > 0
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE0;

                    //
                    // 轉態
                    //
                    Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_on_v_rise;

                    //
                    #ifdef DEBUG_ON
                    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    //
                    #ifdef DEBUG_ON
                    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_on_v_rise\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;


                    //
                    // 轉態
                    //
                    Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_on;

                    //
                    #ifdef DEBUG_ON
                    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    //
                    #ifdef DEBUG_ON
                    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_on\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                }

                //
                // 已經Bypass一次, 清掉flag, 讓下次能正常執行
                //
                if ( Output_ACDC_INT_Module.V_Rise_Bypass_Flag == 1 )
                {
                    Output_ACDC_INT_Module.V_Rise_Bypass_Flag = 0;
                }

                // 設定輸出狀態
                //
                Global_Variables.POWER_STATUS.bit.Output_ON = 1;

                //
                // 高頻下補償Delay再去檢查CC/CP
                //
                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all > 20000 )
                {
                    Output_Control_Module.Check_CC_CP_Delay_Counter = CHECK_CC_CP_DELAY_COUNT;
                }

                //
                // 轉態
                // 輸出開始, 觸發Zero Cross狀態機
                //
                if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC != 0 )
                {
                    cpu_timer_module.Zero_Cross_State = ZERO_CROSS_READY_STATE;
                }


                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case acdc_int_output_on_v_rise:
        {
            //
            // DC-INT, V-Rise電壓爬升中
            //

            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) //停止輸出
            {
                //
                // 轉態
                //
                Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_off0;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_off0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                break;
            }

            if ( ( cpu_timer_module.cpu_timer0_state != ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE0 )
                 && ( cpu_timer_module.cpu_timer0_state != ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE1 )
                 && ( cpu_timer_module.cpu_timer0_state != ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE2 ) )
            {
                //
                // 結束V-Rise
                //

                //
                // 轉態
                //
                Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_on;

            }

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_on\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // 輸出中
        //
        case acdc_int_output_on:
        {
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) //停止輸出
            {
                //
                // 轉態
                //
                Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_off0;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_off0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                break;
            }

            if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option == AUTO_RANGE )
            {
                //
                // Auto Range 判斷
                //

                if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow )
                     || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow )
                     || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ) )
                {
                    //
                    // 設定值有改變, 檢查是否要換檔
                    //
                    Adj_Output_Range_Event();
                }
            }

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
                // 備份目前輸出ON/OFF
                //
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user;

                //
                // 換檔後輸出Bypass V-Rise功能
                //
                Output_ACDC_INT_Module.V_Rise_Bypass_Flag = 1;

                //
                // 輸出OFF
                //
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all;
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;
                //
                // 以下會直接break, 所以在此更新shadow
                //

                break;
            }


            if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 1 )
            {
                //
                // AC or AC+DC
                //

                //
                // 判斷波形是否改變
                //
                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user )
                {
                    Event_Module.Event3_Flag.bit.Change_Waveform = 1;

                    //
                    // 波形改變則CF也改變, 所以要重新計算
                    //
                    Event_Module.Event3_Flag.bit.MS_Mode_Change_ACV_DCV = 1;

                    //
                    // 觸發主中斷Change phase
                    //
                    Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change = 1;

                    //
                    // 量測補償OFF
                    //
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

                    //
                    // 高頻下補償Delay再去檢查CC/CP
                    //
                    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all > 20000 )
                    {
                        Output_Control_Module.Check_CC_CP_Delay_Counter = CHECK_CC_CP_DELAY_COUNT;
                    }


                }
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;
            }


            if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 1 )
            {
                //
                // AC or AC+DC
                //

                //
                // 檢查ACV是否改變
                //
                int32_buf1 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow;
                int32_buf2 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all;
                if ( int32_buf1 != int32_buf2 )
                {
                    Event_Module.Event3_Flag.bit.MS_Mode_Change_ACV_DCV = 1;

                    Output_Control_Module.Flag.bit.Change_ACV = 1;

                    //
                    // 觸發主中斷Change phase
                    //
                    Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change = 1;

                    //
                    // 量測補償OFF
                    //
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

                    //
                    // 高頻下補償Delay再去檢查CC/CP
                    //
                    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all > 20000 )
                    {
                        Output_Control_Module.Check_CC_CP_Delay_Counter = CHECK_CC_CP_DELAY_COUNT;
                    }

                    Output_Control_Module.Flag.bit.Change_ACV = 1;

                    if ( ( int32_buf1 == 0 ) && ( int32_buf2 != 0 ) )
                    {
                        //
                        // 設定改變之後開始有AC輸出
                        //

                        Event_Module.Event3_Flag.bit.Change_FREQ = 1;
                    }
                    else if ( ( int32_buf1 != 0 ) && ( int32_buf2 == 0 ) )
                    {
                        //
                        // 設定值改變之後沒有AC
                        //

                        //
                        // Reset Freq Step
                        //
                        Global_Variables.FreqStep_shadow = DC_FREQ_STEP;
                    }
                }
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all;
            }

            if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 1 )
            {
                //
                // DC or AC+DC
                //

                //
                // 檢查DCV是否改變
                //
                int32_buf1 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow;
                int32_buf2 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all;
                if ( int32_buf1 != int32_buf2 )
                {
                    Event_Module.Event3_Flag.bit.MS_Mode_Change_ACV_DCV = 1;

                    Output_Control_Module.Flag.bit.Change_DCV = 1;

                    //
                    // 觸發主中斷Change phase
                    //
                    Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change = 1;

                    //
                    // 量測補償OFF
                    //
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

                    //
                    // 高頻下補償Delay再去檢查CC/CP
                    //
                    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all > 20000 )
                    {
                        Output_Control_Module.Check_CC_CP_Delay_Counter = CHECK_CC_CP_DELAY_COUNT;
                    }

                }
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all;
            }


            if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.AC == 1 )
            {
                //
                // 檢查FREQ是否改變
                //
                Uint32_buf1 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_shadow;
                Uint32_buf2 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all;
                if ( Uint32_buf1 != Uint32_buf2 )
                {
                    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 )
                    {
                        //
                        // 沒AC輸出
                        //

                    }
                    else
                    {
                        //
                        // AC or AC+DC
                        //

                        Event_Module.Event3_Flag.bit.Change_FREQ = 1;


                        Event_Module.Event3_Flag.bit.MS_Mode_Change_ACV_DCV = 1;

                        Output_Control_Module.Flag.bit.Change_ACV = 1;

                        //
                        // 觸發主中斷Change phase
                        //
                        Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change = 1;

                        //
                        // 量測補償OFF
                        //
                        Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

                        //
                        // 高頻下補償Delay再去檢查CC/CP
                        //
                        if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all > 20000 )
                        {
                            Output_Control_Module.Check_CC_CP_Delay_Counter = CHECK_CC_CP_DELAY_COUNT;
                        }

                    }
                }
                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all;
            }

            //
            // 檢查SlewRate是否改變
            //
            Uint32_buf1 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user_shadow;
            Uint32_buf2 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user;
            if ( Uint32_buf1 != Uint32_buf2 )
            {
                Event_Module.Event_Flag.bit.Calc_V_SlewRate_set = 1;
            }
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user;

            //
            // 檢查Start phase是否改變
            //
            Uint32_buf1 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user_shadow;
            Uint32_buf2 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user;
            if ( Uint32_buf1 != Uint32_buf2 )
            {
                Event_Module.Event_Flag.bit.Set_Start_phase = 1;
            }
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user;

            //
            // 檢查Stop phase是否改變
            //
            Uint32_buf1 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user_shadow;
            Uint32_buf2 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user;
            if ( Uint32_buf1 != Uint32_buf2 )
            {
                Event_Module.Event_Flag.bit.Set_Stop_phase = 1;
            }
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user;

            //
            // 檢查I_rms_limit是否改變
            //
            int32_buf1 = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow;
            int32_buf2 = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;
            if ( int32_buf1 != int32_buf2 )
            {
                if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable == 1 )
                {
                    Output_Control_Module.Flag.bit.Change_I_rms_limit = 1;
                }
            }
            Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;

            break;

        }


        /*-----------------------------------------------------------------------------------------*/
        case acdc_int_output_off0:
        {
            //
            // 等待CPU主中斷WAVE DAC降到zero
            //

            if ( cpu_timer_module.cpu_timer0_state == CPU_TIMER0_OUTPUT_OFF_STATE1 )
            {
                //
                // Bypass電流OCP
                //
                Output_Control_Module.Flag.bit.Bypass_OCP_Flag = 1;


                Output_ACDC_INT_Module.Output_off_start_time = CpuTimer2.InterruptCount;

                //
                // 轉態
                //
                Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_off1;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_off1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case acdc_int_output_off1:
        {
            //
            // Delay for 逆電流卸載補償執行
            //

            //
            // 指定CPU主中斷在逆電流卸載補償狀態
            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE2;

            if ( Calc_Elapsed_Time( Output_ACDC_INT_Module.Output_off_start_time, CpuTimer2.InterruptCount ) > OUTPUT_OFF_DELAY_TIME0 )
            {
                //
                // 指定CPU主中斷離開逆電流卸載補償狀態
                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;


                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> CPU_TIMER0_OUTPUT_OFF_STATE3\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_off2;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_off2\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case acdc_int_output_off2:
        {
            //
            // 等待CPU主中斷VREF DAC降到zero
            //

            if ( cpu_timer_module.cpu_timer0_state == CPU_TIMER0_OUTPUT_OFF_STATE4 )
            {
                //
                // 恢復電流OCP
                //
                Output_Control_Module.Flag.bit.Bypass_OCP_Flag = 0;

                //
                // 指定CPU主中斷回Ready
                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> CPU_TIMER0_OUTPUT_READY_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                // 轉態
                //
                Main_State_Module.Machine_state = READY_STATE0;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> READY_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                //
                // 轉態
                //
                Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_init;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }
    }
}

















//
// End of file
//
