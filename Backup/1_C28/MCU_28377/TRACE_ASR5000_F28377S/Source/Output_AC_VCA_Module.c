//###########################################################################
//
// FILE:    Output_AC_VCA_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220308
//###########################################################################



#include "Global_VariableDefs.h"


volatile OUTPUT_AC_VCA_MODULE_VARIABLES_REG Output_AC_VCA_Module;


/////////////////////////////////////////////////////////////////////
//
// 初始化參數
//
void Init_Output_AC_VCA_Module_Variables(void)
{
    Output_AC_VCA_Module.Output_AC_VCA_State = ac_vca_output_init;
    Output_AC_VCA_Module.Output_off_start_time = 0;
    Output_AC_VCA_Module.Output_on_start_time = 0;
}




/////////////////////////////////////////////////////////////////////
//
// AC VCA 輸出流程
//
void AC_VCA_Output_Process(void)
{
    int32 int32_buf1, int32_buf2;
    Uint32 Uint32_buf1, Uint32_buf2;

    switch(Output_AC_VCA_Module.Output_AC_VCA_State)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // 初始化
        //
        case ac_vca_output_init:
        {
            float vmax, a, b;

            //
            // Preload waveform
            // 根據waveform指定載入的wave_buffer_section
            //
            Change_Waveform_Event();
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;


            //
            // 計算VREF DAC
            //
            vmax = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 1.01;
            a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
            b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
            Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)( ( vmax * a ) + b );


            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

            //
            // FREQ
            //
            Change_FREQ_Event();
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
            // Slew Rate
            //
            Calc_V_SlewRate_set_Event();

            //
            // 輸出Relay ON
            //
            Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 1;

            Output_AC_VCA_Module.Output_on_start_time = CpuTimer2.InterruptCount;

            //
            // 轉態
            //
            Output_AC_VCA_Module.Output_AC_VCA_State = ac_vca_output_on_delay;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_AC_VCA_State -> ac_vca_output_on_delay\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ac_vca_output_on_delay:
        {

            if ( Calc_Elapsed_Time( Output_AC_VCA_Module.Output_on_start_time, CpuTimer2.InterruptCount ) > OUTPUT_ON_DELAY_TIME )
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

                //
                // Reload dds
                //
                Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;


                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = AC_VCA_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;

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
                Output_AC_VCA_Module.Output_AC_VCA_State = ac_vca_output_on;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> AC_VCA_CPU_TIMER0_OUTPUT_START_PHASE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_AC_VCA_State -> ac_vca_output_on\n", CpuTimer2.InterruptCount ); //9530 cycles
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
        case ac_vca_output_on:
        {
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) //停止輸出
            {

                //
                // 轉態
                //
                Output_AC_VCA_Module.Output_AC_VCA_State = ac_vca_output_off0; //轉態

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg,
                        "CPU_Timer2: %lu ms > Output_AC_VCA_State -> ac_vca_output_off0\n",
                        CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                break;
            }

            //
            // 判斷波形是否改變
            //
            if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user )
            {

                Event_Module.Event3_Flag.bit.Change_Waveform = 1;

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
            // 檢查FREQ是否改變
            //
            Uint32_buf1 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_shadow;
            Uint32_buf2 = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all;
            if ( Uint32_buf1 != Uint32_buf2 )
            {
                Event_Module.Event3_Flag.bit.Change_FREQ = 1;

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
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all;

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
        case ac_vca_output_off0:
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


                Output_AC_VCA_Module.Output_off_start_time = CpuTimer2.InterruptCount;

                //
                // 轉態
                //
                Output_AC_VCA_Module.Output_AC_VCA_State = ac_vca_output_off1;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_AC_VCA_State -> ac_vca_output_off1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case ac_vca_output_off1:
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

            if ( Calc_Elapsed_Time( Output_AC_VCA_Module.Output_off_start_time, CpuTimer2.InterruptCount ) > OUTPUT_OFF_DELAY_TIME0 )
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
                Output_AC_VCA_Module.Output_AC_VCA_State = ac_vca_output_off2;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_AC_VCA_State -> ac_vca_output_off2\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;

        }

        /*-----------------------------------------------------------------------------------------*/
        case ac_vca_output_off2:
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
                Output_AC_VCA_Module.Output_AC_VCA_State = ac_vca_output_init;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_AC_VCA_State -> ac_vca_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
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
