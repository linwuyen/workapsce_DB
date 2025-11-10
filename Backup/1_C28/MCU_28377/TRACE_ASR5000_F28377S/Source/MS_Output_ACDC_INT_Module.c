//###########################################################################
//
// FILE:    MS_Output_ACDC_INT_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20221214
//###########################################################################



#include "Global_VariableDefs.h"


volatile MS_OUTPUT_ACDC_INT_MODULE_VARIABLES_REG MS_Output_ACDC_INT_Module;


/////////////////////////////////////////////////////////////////////
//
// 初始化參數
//
void Init_MS_Output_ACDC_INT_Module_Variables(void)
{
    MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_init;
    MS_Output_ACDC_INT_Module.Output_off_start_time = 0;
    MS_Output_ACDC_INT_Module.Output_on_start_time = 0;
}




/////////////////////////////////////////////////////////////////////
//
// MS ACDC INT 輸出流程
//
void MS_ACDC_INT_Output_Process(void)
{
    int32 int32_buf1, int32_buf2;
    Uint32 Uint32_buf1, Uint32_buf2;

    switch(MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // 初始化
        //
        case MS_acdc_int_output_init:
        {
            //
            // 清掉Sync Trig
            //
            Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = 0;

            //
            // 根據Duty產生SQU WAVE data, 並存入SDRAM
            //
            //Generate_Square_wave_data_process();
            //Preload_SQU_WAVE_DATA_Event();

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
            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

            //
            // 處理 頻率step
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
            // Start phase
            //
            Set_Start_phase_Event();

            //
            // Stop phase
            //
            Set_Stop_phase_Event();

            //
            // 3P4W/1P3W Phase
            //
            MS_Mode_Change_Phase_Set_Event();

            //
            // Slew Rate
            //
            Calc_V_SlewRate_set_Event();

            //
            // 輸出Relay ON
            //
            Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 1;

            MS_Output_ACDC_INT_Module.Output_on_start_time = CpuTimer2.InterruptCount;


            //
            // 輸出前初始化暫態補償器
            //
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.Err = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.Fdb = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.Out = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ref = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.SatErr = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ud = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ui = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.Up = 0;
            PID_Control_Module.MS_Balance_Fast_Compensate_loop.Up1 = 0;

            //
            // Reset VREF offset 2
            //
            CPU_to_CLA_Variables.VREF_Compensate_Offset_2 = 0;

            //
            // 轉態
            //
            MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_on_delay_0;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_on_delay_0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case MS_acdc_int_output_on_delay_0:
        {

            if ( Calc_Elapsed_Time( MS_Output_ACDC_INT_Module.Output_on_start_time, CpuTimer2.InterruptCount ) > OUTPUT_ON_DELAY_TIME )
            {

                if ( Global_Variables.POWER_STATUS.bit.MS_Master == 1 )
                {
                    //
                    // Master
                    //

                    //
                    // Reset Timer
                    //
                    MS_Output_ACDC_INT_Module.Master_Wait_Slave_ON_Start_Time = CpuTimer2.InterruptCount;

                    //
                    // 轉態
                    //
                    MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_on_delay_Master0;

                    //
                    #ifdef DEBUG_ON
                    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_on_delay_Master0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                }
                else if ( Global_Variables.POWER_STATUS.bit.MS_Master == 0 )
                {
                    //
                    // Slave
                    //

                    //
                    // 轉態
                    //
                    MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_on_delay_Slave0;

                    //
                    #ifdef DEBUG_ON
                    sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_on_delay_Slave0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case MS_acdc_int_output_on_delay_Master0:
        {
            //
            // 等待Slave都先轉態後, 再送出同步觸發
            //

            if ( ( ( Global_Variables.MS_Function_Variables.MS_Total_Units == 2 )
                   && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[1].bit.Slave_Wait_ON_Sync == 1 ) )
                 || ( ( Global_Variables.MS_Function_Variables.MS_Total_Units == 3 )
                      && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[1].bit.Slave_Wait_ON_Sync == 1 )
                      && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[2].bit.Slave_Wait_ON_Sync == 1 ) ) )
            {
                //
                // Reset Timer
                //
                MS_Output_ACDC_INT_Module.Master_Wait_Slave_ON_Start_Time = CpuTimer2.InterruptCount;

                //
                // SPI傳送同步訊號
                //
                Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.ON_Sync_Trig = 1;

                //
                // 開始計時
                //
                cpu_timer_module.MS_Sync_0_Singal_Start_Time = CpuTimer0.InterruptCount;


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
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;


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
                MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_on;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_on\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }


            if ( Calc_Elapsed_Time( MS_Output_ACDC_INT_Module.Master_Wait_Slave_ON_Start_Time, CpuTimer2.InterruptCount ) > MASTER_WAIT_SLAVE_ON_OVER_TIME )
            {
                //
                // 等待同步逾時, NG
                //
                //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Sync_Output_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_SLAVE_WAIT_ON_SYNC_OVERTIME;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;


                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***MS_CONNECT_OK_STATE_Error***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Connect_State -> MS_CONNECT_ERROR_HANDLE_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case MS_acdc_int_output_on_delay_Slave0:
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
            // 送出狀態給Master, 等待Master同步
            //
            Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[ Global_Variables.MS_Function_Variables.MS_Address ].bit.Slave_Wait_ON_Sync = 1;

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;

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
            MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_on;

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_on\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 輸出中
        //
        case MS_acdc_int_output_on:
        {
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) //停止輸出
            {

                //
                // 轉態
                //
                MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_off0;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_off0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

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
        case MS_acdc_int_output_off0:
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


                MS_Output_ACDC_INT_Module.Output_off_start_time = CpuTimer2.InterruptCount;

                //
                // 轉態
                //
                MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_off1;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_off1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEVELOP_SV54
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ( DEVELOP_SV54: Bypass Reverse_Current_Unload_Compensate_loop )\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
                #endif

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case MS_acdc_int_output_off1:
        {
            //
            // Delay for 逆電流卸載補償執行
            //

            //
            // 指定CPU主中斷在逆電流卸載補償狀態
            //
            // 轉態
            //
            #ifdef DEVELOP_SV54
            //
            // Bypass Reverse_Current_Unload_Compensate_loop
            //
            #else
            cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE2;
            #endif

            if ( Calc_Elapsed_Time( MS_Output_ACDC_INT_Module.Output_off_start_time, CpuTimer2.InterruptCount ) > OUTPUT_OFF_DELAY_TIME0 )
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
                MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_off2;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_off2\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case MS_acdc_int_output_off2:
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
                MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_init;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
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
