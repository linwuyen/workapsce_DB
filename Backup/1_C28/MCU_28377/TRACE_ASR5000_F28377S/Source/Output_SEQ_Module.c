//###########################################################################
//
// FILE:    Output_SEQ_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220824
//###########################################################################



#include "Global_VariableDefs.h"


volatile OUTPUT_SEQ_MODULE_VARIABLES_REG Output_SEQ_Module;


/////////////////////////////////////////////////////////////////////
//
// 初始化參數
//
void Init_Output_SEQ_Module_Variables(void)
{
    Output_SEQ_Module.Output_SEQ_State = seq_output_init;
    Output_SEQ_Module.Output_off_start_time = 0;
    Output_SEQ_Module.Output_on_start_time = 0;
}




/////////////////////////////////////////////////////////////////////
//
// SEQ 輸出流程
//
void SEQ_Output_Process(void)
{

    switch(Output_SEQ_Module.Output_SEQ_State)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // 初始化
        //
        case seq_output_init:
        {
            //Uint16 run_step;
            float vmax, a, b;


            //
            // 計算VREF DAC
            //
            vmax = Global_Variables.MACHINE_SERIES_IMFORMATION.DC_Maximum_Vout * 1.01;
            a = Global_Variables.Calibration_Variables.VSET_to_DAC_Scale.all;
            b = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
            Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)( ( vmax * a ) + b );

            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

            //
            // 載入SEQ輸出Mode
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user_shadow.all = Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user.all;


            //
            // Slew Rate
            //
            Calc_V_SlewRate_set_Event();

            //
            // 清掉Jump Counter
            //
            SEQ_Mode_Clear_Jump_Count_Event();

            //
            // 輸出Relay ON
            //
            Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 1;

            Output_SEQ_Module.Output_on_start_time = CpuTimer2.InterruptCount;

            //
            // 轉態
            //
            Output_SEQ_Module.Output_SEQ_State = seq_output_on_delay;

            //
            #ifdef DEBUG_ON
            sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_SEQ_State -> seq_output_on_delay\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case seq_output_on_delay:
        {
            if ( Calc_Elapsed_Time( Output_SEQ_Module.Output_on_start_time, CpuTimer2.InterruptCount ) > OUTPUT_ON_DELAY_TIME )
            {

                //
                // 轉態
                //
                Output_SEQ_Module.Output_SEQ_State = seq_output_on;

                //
                // 轉態Ready前清掉SEQ控制旗標, 避免誤動作
                //
                Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                //
                // Ready下, Run step = 0
                //
                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;

                //
                // 設定EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

                // 設定輸出狀態
                //
                Global_Variables.POWER_STATUS.bit.Output_ON = 1;


                //
                // 判斷SEQ Mode
                //
                if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user_shadow.all == SEQ_AC_MODE )
                {
                    //
                    // 轉態
                    // 輸出開始, 觸發Zero Cross狀態機
                    //
                    cpu_timer_module.Zero_Cross_State = ZERO_CROSS_READY_STATE;

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_READY_STATE;
                }
                else if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user_shadow.all == SEQ_DC_MODE )
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_READY_STATE;
                }
                else if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user_shadow.all == SEQ_ACDC_MODE )
                {
                    //
                    // 轉態
                    // 輸出開始, 觸發Zero Cross狀態機
                    //
                    cpu_timer_module.Zero_Cross_State = ZERO_CROSS_READY_STATE;

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_READY_STATE;
                }


                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> SEQ_CPU_TIMER0_OUTPUT_READY_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_SEQ_State -> seq_output_on\n", CpuTimer2.InterruptCount ); //9530 cycles
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
        case seq_output_on:
        {
            Uint32 Uint32_buf1, Uint32_buf2;

            Uint16 run_step;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;


            //
            // 停止輸出 Output OFF
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {

                //
                // 觸發清空Jump Count事件
                //
                Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_Jump_Count = 1;

                //
                // 設定Run Step = 0
                //
                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;

                //
                // 轉態
                // 沒有輸出時, Zero Cross Trig不動作
                //
                cpu_timer_module.Zero_Cross_State = ZERO_CROSS_INIT_STATE;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE0;

                Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = WAVE_DAC_ZERO;

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;


                //
                // 轉態
                //
                Output_SEQ_Module.Output_SEQ_State = seq_output_off0; //轉態


                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > cpu_timer0_state -> CPU_TIMER0_OUTPUT_OFF_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_SEQ_State -> seq_output_off0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                break;
            }


            switch( Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user_shadow.all )
            {

                /*-----------------------------------------------------------------------------------------*/
                case SEQ_AC_MODE:
                {
                    //
                    // SEQ Start處理
                    //
                    if ( ( cpu_timer_module.cpu_timer0_state == SEQ_AC_CPU_TIMER0_OUTPUT_READY_STATE )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // 轉態前清掉SEQ控制旗標避免轉態後誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        //
                        // Run Step = 1, 從 1 開始
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 1;


                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( ( cpu_timer_module.cpu_timer0_state == SEQ_AC_CPU_TIMER0_OUTPUT_HOLD_STATE )
                              && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // Resume
                        //

                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_ON_STATE;

                        break;
                    }
                    else if ( ( cpu_timer_module.cpu_timer0_state == SEQ_AC_CPU_TIMER0_OUTPUT_END_HOLD_STATE )
                              && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // Resume
                        //

                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;

                        //
                        // Term = Continuous
                        //


                        if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step > 998 )
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;
                        }
                        else
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step ++;
                        }

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else
                    {
                        //
                        // 其他條件觸發Start, 則直接清除
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;
                    }


                    //
                    // SEQ Stop處理
                    // 當SEQ狀態 = ON / Hold / Hold(end), 命令才有效
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Stop == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step != 0 )
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;

                            //
                            // 轉態
                            //
                            cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                            break;
                        }
                    }



                    //
                    // SEQ Hold處理
                    // SEQ Output ON時, Hold才有效
                    //
                    if ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Hold == 1 ) )
                    {

                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_HOLD_STATE;

                        break;
                    }



                    //
                    // SEQ Branch1 處理
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1 == 1 )
                         && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch1_Enable == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Branch1_set_user[ run_step ];

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch1_Enable == 0 )
                    {
                        //
                        // Branch1沒有Enable, 命令無效
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1 = 0;

                        break;
                    }


                    //
                    // SEQ Branch2 處理
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2 == 1 )
                         && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch2_Enable == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Branch2_set_user[ run_step ];

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch2_Enable == 0 )
                    {
                        //
                        // Branch2沒有Enable, 命令無效
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2 = 0;
                    }

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case SEQ_DC_MODE:
                {
                    //
                    // SEQ Start處理
                    //
                    if ( ( cpu_timer_module.cpu_timer0_state == SEQ_DC_CPU_TIMER0_OUTPUT_READY_STATE )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // 轉態前清掉SEQ控制旗標避免轉態後誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        //
                        // Run Step = 1, 從 1 開始
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 1;


                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( ( cpu_timer_module.cpu_timer0_state == SEQ_DC_CPU_TIMER0_OUTPUT_HOLD_STATE )
                              && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // Resume
                        //

                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_ON_STATE;

                        break;
                    }
                    else if ( ( cpu_timer_module.cpu_timer0_state == SEQ_DC_CPU_TIMER0_OUTPUT_END_HOLD_STATE )
                              && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // Resume
                        //

                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;

                        //
                        // Term = Continuous
                        //


                        if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step > 998 )
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;
                        }
                        else
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step ++;
                        }

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else
                    {
                        //
                        // 其他條件觸發Start, 則直接清除
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;
                    }


                    //
                    // SEQ Stop處理
                    // 當SEQ狀態 = ON / Hold / Hold(end), 命令才有效
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Stop == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step != 0 )
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;

                            //
                            // 轉態
                            //
                            cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                            break;
                        }
                    }



                    //
                    // SEQ Hold處理
                    // SEQ Output ON時, Hold才有效
                    //
                    if ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Hold == 1 ) )
                    {

                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_HOLD_STATE;

                        break;
                    }



                    //
                    // SEQ Branch1 處理
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1 == 1 )
                         && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch1_Enable == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Branch1_set_user[ run_step ];

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch1_Enable == 0 )
                    {
                        //
                        // Branch1沒有Enable, 命令無效
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1 = 0;

                        break;
                    }


                    //
                    // SEQ Branch2 處理
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2 == 1 )
                         && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch2_Enable == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Branch2_set_user[ run_step ];

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch2_Enable == 0 )
                    {
                        //
                        // Branch2沒有Enable, 命令無效
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2 = 0;
                    }


                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case SEQ_ACDC_MODE:
                {

                    //
                    // SEQ Start處理
                    //
                    if ( ( cpu_timer_module.cpu_timer0_state == SEQ_ACDC_CPU_TIMER0_OUTPUT_READY_STATE )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // 轉態前清掉SEQ控制旗標避免轉態後誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        //
                        // Run Step = 1, 從 1 開始
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 1;


                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( ( cpu_timer_module.cpu_timer0_state == SEQ_ACDC_CPU_TIMER0_OUTPUT_HOLD_STATE )
                              && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // Resume
                        //

                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_ON_STATE;

                        break;
                    }
                    else if ( ( cpu_timer_module.cpu_timer0_state == SEQ_ACDC_CPU_TIMER0_OUTPUT_END_HOLD_STATE )
                              && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start == 1 ) )
                    {
                        //
                        // Resume
                        //

                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;

                        //
                        // Term = Continuous
                        //


                        if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step > 998 )
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;
                        }
                        else
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step ++;
                        }

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else
                    {
                        //
                        // 其他條件觸發Start, 則直接清除
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = 0;
                    }


                    //
                    // SEQ Stop處理
                    // 當SEQ狀態 = ON / Hold / Hold(end), 命令才有效
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Stop == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step != 0 )
                        {
                            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;

                            //
                            // 轉態
                            //
                            cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                            break;
                        }
                    }



                    //
                    // SEQ Hold處理
                    // SEQ Output ON時, Hold才有效
                    //
                    if ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Hold == 1 ) )
                    {

                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_HOLD_STATE;

                        break;
                    }



                    //
                    // SEQ Branch1 處理
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1 == 1 )
                         && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch1_Enable == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Branch1_set_user[ run_step ];

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch1_Enable == 0 )
                    {
                        //
                        // Branch1沒有Enable, 命令無效
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1 = 0;

                        break;
                    }


                    //
                    // SEQ Branch2 處理
                    //
                    if ( ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_run )
                           || ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State == _Sequence_hold ) )
                         && ( Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2 == 1 )
                         && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch2_Enable == 1 ) )
                    {
                        //
                        // 清掉SEQ控制旗標避免誤動作
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Branch2_set_user[ run_step ];

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                        break;
                    }
                    else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Branch2_Enable == 0 )
                    {
                        //
                        // Branch2沒有Enable, 命令無效
                        //
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2 = 0;
                    }


                    break;
                }
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

            break;

        }


        /*-----------------------------------------------------------------------------------------*/
        case seq_output_off0:
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


                Output_SEQ_Module.Output_off_start_time = CpuTimer2.InterruptCount;

                //
                // 轉態
                //
                Output_SEQ_Module.Output_SEQ_State = seq_output_off1;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_SEQ_State -> seq_output_off1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case seq_output_off1:
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

            if ( Calc_Elapsed_Time( Output_SEQ_Module.Output_off_start_time, CpuTimer2.InterruptCount ) > OUTPUT_OFF_DELAY_TIME0 )
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
                Output_SEQ_Module.Output_SEQ_State = seq_output_off2;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_SEQ_State -> seq_output_off2\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case seq_output_off2:
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
                Output_SEQ_Module.Output_SEQ_State = seq_output_init;

                //
                #ifdef DEBUG_ON
                sprintf( SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_SEQ_State -> seq_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
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
