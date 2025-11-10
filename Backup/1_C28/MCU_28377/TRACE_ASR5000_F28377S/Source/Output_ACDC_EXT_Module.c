//###########################################################################
//
// FILE:    Output_ACDC_EXT_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220315
//###########################################################################



#include "Global_VariableDefs.h"


volatile OUTPUT_ACDC_EXT_MODULE_VARIABLES_REG Output_ACDC_EXT_Module;


/////////////////////////////////////////////////////////////////////
//
// 初始化參數
//
void Init_Output_ACDC_EXT_Module_Variables(void)
{
    Output_ACDC_EXT_Module.Output_ACDC_EXT_State = acdc_ext_output_init;
    Output_ACDC_EXT_Module.Output_off_start_time = 0;
    Output_ACDC_EXT_Module.Output_on_start_time = 0;
}


/////////////////////////////////////////////////////////////////////
//
// 輸出流程
//
void ACDC_EXT_Output_Process(void)
{
    int32 int32_buf1, int32_buf2;

    float vmax, a, b;

    switch(Output_ACDC_EXT_Module.Output_ACDC_EXT_State)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // 初始化
        //
        case acdc_ext_output_init:
        {

            Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level = (Uint16)Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;


            //
            // 計算VREF DAC
            //
            vmax = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 1.01;
            a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
            b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
            Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)( ( vmax * a ) + b );

            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

            //輸出Relay ON
            Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 1;

            Output_ACDC_EXT_Module.Output_on_start_time = CpuTimer2.InterruptCount;

            //Slew Rate
            Calc_V_SlewRate_set_Event();


            Output_ACDC_EXT_Module.Output_ACDC_EXT_State = acdc_ext_output_on_delay; //轉態

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_EXT_State -> acdc_ext_output_on_delay\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 輸出前Delay
        //
        case acdc_ext_output_on_delay:
        {
            if ( Calc_Elapsed_Time( Output_ACDC_EXT_Module.Output_on_start_time, CpuTimer2.InterruptCount ) > OUTPUT_ON_DELAY_TIME )
            {

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_EXT_CPU_TIMER0_OUTPUT_ON_STATE; //觸發轉態

                //
                // 設定EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

                // 設定輸出狀態
                //
                Global_Variables.POWER_STATUS.bit.Output_ON = 1;

                //
                // 轉態
                //
                Output_ACDC_EXT_Module.Output_ACDC_EXT_State = acdc_ext_output_on; //轉態

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> ACDC_EXT_CPU_TIMER0_OUTPUT_ON_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_EXT_State -> acdc_ext_output_on\n", CpuTimer2.InterruptCount ); //9530 cycles
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
        case acdc_ext_output_on:
        {
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 ) //停止輸出
            {

                //
                // 轉態
                //
                Output_ACDC_EXT_Module.Output_ACDC_EXT_State = acdc_ext_output_off0; //轉態

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_EXT_State -> acdc_ext_output_off0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            //
            // 檢查SlewRate是否改變
            //
            if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user_shadow
                    != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user )
            {
                Event_Module.Event_Flag.bit.Calc_V_SlewRate_set = 1;
            }
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user;

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
        case acdc_ext_output_off0:
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


                Output_ACDC_EXT_Module.Output_off_start_time = CpuTimer2.InterruptCount;

                //
                // 轉態
                //
                Output_ACDC_EXT_Module.Output_ACDC_EXT_State = acdc_ext_output_off1;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_EXT_State -> acdc_ext_output_off1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case acdc_ext_output_off1:
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

            if ( Calc_Elapsed_Time( Output_ACDC_EXT_Module.Output_off_start_time, CpuTimer2.InterruptCount ) > OUTPUT_OFF_DELAY_TIME0 )
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
                Output_ACDC_EXT_Module.Output_ACDC_EXT_State = acdc_ext_output_off2;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_EXT_State -> acdc_ext_output_off2\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case acdc_ext_output_off2:
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
                Output_ACDC_EXT_Module.Output_ACDC_EXT_State = acdc_ext_output_init;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_EXT_State -> acdc_ext_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
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
