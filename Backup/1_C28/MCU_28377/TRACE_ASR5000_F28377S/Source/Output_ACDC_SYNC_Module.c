//###########################################################################
//
// FILE:    Output_ACDC_SYNC_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220308
//###########################################################################



#include "Global_VariableDefs.h"


volatile OUTPUT_ACDC_SYNC_MODULE_VARIABLES_REG Output_ACDC_SYNC_Module;


/////////////////////////////////////////////////////////////////////
//
// 初始化參數
//
void Init_Output_ACDC_SYNC_Module_Variables(void)
{
    Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State = acdc_sync_output_init;
    Output_ACDC_SYNC_Module.Output_off_start_time = 0;
    Output_ACDC_SYNC_Module.Output_on_start_time = 0;

}




/////////////////////////////////////////////////////////////////////
//
// ACDC SYNC 輸出流程
//
void ACDC_SYNC_Output_Process(void)
{
    int32 int32_buf1, int32_buf2;
    Uint32 Uint32_buf1, Uint32_buf2;
    float float_buf1, float_buf2;

    switch(Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // 初始化
        //
        case acdc_sync_output_init:
        {

            //
            // Preload waveform
            // 根據waveform指定載入的wave_buffer_section
            //
            Change_Waveform_Event();
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;


            //
            // Set REF及WAVE DAC
            //
            MS_Mode_Change_ACV_DCV_Event();
            Global_Variables.OUTPUT_VARIABLES.WAVE_offset = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;
            Global_Variables.OUTPUT_VARIABLES.WAVE_scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow;
            //Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;
            //
            // 20230217
            // 進入主中斷會有一段時間等待c1與SYNC同步
            // 如果這裡就開VREF, ACDC-SYNC輸出時, 波形會先墊高DC然後再有AC
            // 所以這裡不開, 等進入主中斷ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE0
            // 再去判斷
            //


            //
            // SYNC Phase Delay
            //
            SYNC_Mode_Change_Phase_Delay_Event();

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
            // 輸出Relay ON
            //
            Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 1;

            Output_ACDC_SYNC_Module.Output_on_start_time = CpuTimer2.InterruptCount;


            //
            // 轉態
            //
            Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State = acdc_sync_output_on_delay;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_SYNC_State -> acdc_sync_output_on_delay\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 輸出前Delay
        //
        case acdc_sync_output_on_delay:
        {

            if ( Calc_Elapsed_Time( Output_ACDC_SYNC_Module.Output_on_start_time, CpuTimer2.InterruptCount ) > OUTPUT_ON_DELAY_TIME )
            {

                if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 )
                {
                    //
                    // 沒AC輸出
                    //


                    //
                    // ON Phase Disable
                    //

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;

                    //
                    // 量測補償ON
                    //
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 1;
                    Output_Control_Module.DCV_Compensate_State = DCV_NO_COMPENSATE;
                    Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                    //
                    // Reset WAVE以及VREF補償量
                    //
                    Output_Control_Module.WAVE_Compensate_Offset = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                    //
                    // 觸發轉態
                    //
                    cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;

                }
                else
                {
                    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable == 1 )
                    {
                        //
                        // ON Phase Enable
                        //
                        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count;

                        //
                        // 觸發轉態
                        //
                        cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;

                    }
                    else
                    {
                        //
                        // ON Phase Disable
                        //

                        Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;

                        //
                        // 觸發轉態
                        //
                        cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;

                    }
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

                //
                // 轉態
                //
                Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State = acdc_sync_output_on;


                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_SYNC_State -> acdc_sync_output_on\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 輸出中
        //
        case acdc_sync_output_on:
        {
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) //停止輸出
            {
                ACFAIL_start_time_reset();

                //
                // 轉態
                //
                Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State = acdc_sync_output_off0;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_SYNC_State -> acdc_sync_output_off0\n", CpuTimer2.InterruptCount ); //9530 cycles
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

            }
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all;


            if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.DC == 1 )
            {
                //
                // AC+DC
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


            //
            // 檢查Phase Delay是否改變
            //
            float_buf1 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user_shadow;
            float_buf2 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user;
            if ( float_buf1 != float_buf2 )
            {
                Event_Module.Event2_Flag.bit.SYNC_Mode_Change_Phase_Delay = 1;
            }
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user;


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
        case acdc_sync_output_off0:
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


                Output_ACDC_SYNC_Module.Output_off_start_time = CpuTimer2.InterruptCount;

                ACFAIL_start_time_reset();

                //
                // 轉態
                //
                Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State = acdc_sync_output_off1;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_SYNC_State -> acdc_sync_output_off1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case acdc_sync_output_off1:
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

            if ( Calc_Elapsed_Time( Output_ACDC_SYNC_Module.Output_off_start_time, CpuTimer2.InterruptCount ) > OUTPUT_OFF_DELAY_TIME0 )
            {
                ACFAIL_start_time_reset();

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
                Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State = acdc_sync_output_off2;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_SYNC_State -> acdc_sync_output_off2\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case acdc_sync_output_off2:
        {
            //
            // 等待CPU主中斷VREF DAC降到zero
            //

            if ( cpu_timer_module.cpu_timer0_state == CPU_TIMER0_OUTPUT_OFF_STATE4 )
            {
                ACFAIL_start_time_reset();

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
                Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State = acdc_sync_output_init;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_SYNC_State -> acdc_sync_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
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
