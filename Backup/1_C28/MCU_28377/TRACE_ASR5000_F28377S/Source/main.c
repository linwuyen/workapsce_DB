//###########################################################################
//
// FILE:    main.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210819
//###########################################################################

//需要先設定
//Project -> properties -> Predefined symbols
//Add "_FLASH", "CPU1"


#include "Global_VariableDefs.h"






////////////////////////////////////////////////////////////////////
//
// 主程式
//
void main(void)
{

    /*------------------------------------------------*/
    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2837xS_SysCtrl.c file.
    //
    InitSysCtrl();

    // 配合SPI需要到20MHz
    // 預設LSPCLK=50MHz,這裡設定後LSPCLK=200MHz
    EALLOW;
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0;  //Set LSPCLK equal to SYSCLK. To SCIs, SPIs, McBSPs
    EDIS;

    /*------------------------------------------------*/
    //
    // Step 2. Initialize GPIO:
    // This example function is found in the F2837xS_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    InitGpio();

    /*------------------------------------------------*/
    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    /*------------------------------------------------*/
    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xS_PieCtrl.c file.
    //
    InitPieCtrl();

    /*------------------------------------------------*/
    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    /*------------------------------------------------*/
    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xS_DefaultIsr.c.
    // This function is found in F2837xS_PieVect.c.
    //
    InitPieVectTable();

    /*------------------------------------------------*/
    CpuTimer0.InterruptCount = 0;
    CpuTimer1.InterruptCount = 0;
    CpuTimer2.InterruptCount = 0;

    /*------------------------------------------------*/

    Init_Global_Variables();

    //
    // 載入機種參數
    //
    Model_Type_Change_Event();

    //
    // 載入校正參數初始值
    //
    Set_V_HL_Cali_Variable_default_Event();
    Set_I_HL_Cali_Variable_default_Event();
    Set_EXT_HL_Cali_Variable_default_Event();

    //
    // 觸發校正Checksum計算事件
    //
    Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum = 1;

    Init_SCI_Module_Variables();

    Init_Memory_Module_Variables();

    Init_CPU_Timer_Module_Variables();

    Init_DSP_ADC_16bit_Module_Variables();

    Init_SPI_Module_Variables();

    Init_McBSP_Module_Variables();

    Init_Main_State_Module_Variables();

    Init_Fan_Speed_Control_Variables();

    Init_Event_Module_Variables();

    Init_Meas_Module_Variables();

    Init_Calibration_Variables();

    Init_Output_ACDC_INT_Module_Variables();

    Init_Output_AC_VCA_Module_Variables();

    Init_Output_ACDC_SYNC_Module_Variables();

    Init_Output_ACDC_EXT_Module_Variables();

    Init_Output_ACDC_ADD_Module_Variables();

    Init_Output_SEQ_Module_Variables();

    Init_MS_Output_ACDC_INT_Module_Variables();

    Init_Output_Control_Module_Variables();

    Init_Digital_Filter_Module_Variables();

    Init_Digital_Filter_Module();

    Init_MS_Control_Module_Variables();

    Init_PID_Control_Module_Variables();

    Init_Phase_Sync_Module_Variables();

    Init_EEROM_Module_Variables();

    Init_VREF_WAVE_Process();

    //初始化參數,觸發相關事件
    Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1;
    Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1;


    /*------------------------------------------------*/

    Init_GPIO_Module();

    #ifdef DEBUG_ON
    Init_SCI_A_GPIO();
    Init_SCI_A_Reg();
    #endif

    Init_CPU_Timer_Reg();

    Init_EMIF1_Reg();

    Init_eCAP_Reg();

    Init_EPWM_Module();

    Init_DSP_ADC_16bit_Module();

    Init_SPI_A_GPIO();

    Init_SPI_A_Reg();

    Init_ADS8353();

    Init_McBSP_B_GPIO();

    Init_McBSP_B_Reg();

    Init_DAC_Module();

    /*------------------------------------------------*/
    CLA_configClaMemory();

    CLA_initCpu1Cla1();

    Init_CLA_Module_Variables();


    /*------------------------------------------------*/

    DELAY_US(10000);


    /*------------------------------------------------*/
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    /*------------------------------------------------*/
    //
    // Main loop
    //
    for(;;)
    {

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // 執行週期 => ASAP
        //

        //
        #ifdef DEVELOP_SV01
        /*--------------------Cycle量測 Step2--------------------*/
        cpu_timer_module.Cycle_Meas_Result_03 = CpuTimer0.InterruptCount - cpu_timer_module.Cycle_Meas_Start_03; //8 cycles
        // 對應的執行時間 = ( Cycle_Meas_Result ) x 2.775us

        /*--------------------Cycle量測 Step1--------------------*/
        cpu_timer_module.Cycle_Meas_Start_03 = CpuTimer0.InterruptCount;
        #endif



        //
        // 更新ERR狀態
        //
        Input_Flag_Update_Process();


        //
        // MS控制RS485 TX流程
        //
        SCIC_TXRX_Process();

        //
        // 排程狀態機 01
        //
        switch( Main_State_Module.main_loop_schedule01_state )
        {
            case MAIN_LOOP_SCHEDULE01_01_STATE:
            {
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                // 執行週期 => 1000us
                //
                if ( Calc_Elapsed_Time( cpu_timer_module.Cpu_Timer0_Time_Stamp_01, CpuTimer0.InterruptCount ) > CPU_TIMER0_1ms )
                {
                    cpu_timer_module.Cpu_Timer0_Time_Stamp_01 = CpuTimer0.InterruptCount;

                    //
                    // 外部連線控制模組
                    //
                    MS_Connect_Process();

                    //
                    // 更新輸出IO
                    //
                    Output_Flags_Update(); //13 cycles

                    //
                    // 讀取EXT IO介面
                    //
                    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Ext_IO_Control_Enable == 1 )
                    {
                        Read_EXTIO_Process();
                    }

                    //
                    // 量測頻率更新
                    //
                    if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_EXT_MODE )
                         || ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == ACDC_EXT_MODE ) )
                    {
                        EXT_Freq_Calc_Process();
                    }


                    //
                    // Error Scan
                    //
                    if ( ( Main_State_Module.Machine_state == READY_STATE0 )
                         || ( Main_State_Module.Machine_state == READY_STATE )
                         || ( Main_State_Module.Machine_state == OUTPUT_STATE ) )
                    {

                        Power_Ready_Error_Scan();
                    }

                    //
                    #ifdef DEVELOP_SV01
                    /*--------------------Cycle量測 Step2--------------------*/
                    cpu_timer_module.Power_Ready_Error_Scan_Cycle_Meas_Result = CpuTimer0.InterruptCount - cpu_timer_module.Power_Ready_Error_Scan_Cycle_Meas_Start; //8 cycles
                    // 對應的執行時間 = ( Cycle_Meas_Result ) x 2.775us

                    /*--------------------Cycle量測 Step1--------------------*/
                    cpu_timer_module.Power_Ready_Error_Scan_Cycle_Meas_Start = CpuTimer0.InterruptCount;
                    #endif


                }

                //
                // 轉態
                //
                Main_State_Module.main_loop_schedule01_state = MAIN_LOOP_SCHEDULE01_02_STATE;

                break;
            }

            case MAIN_LOOP_SCHEDULE01_02_STATE:
            {
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                // 執行週期 => ASAP
                //

                //
                // 選擇頻率來源計算量測及補償更新週期, 最高5ms
                //
                Calc_Meas_Period_Process();

                //
                // 轉態
                //
                Main_State_Module.main_loop_schedule01_state = MAIN_LOOP_SCHEDULE01_03_STATE;

                break;
            }

            case MAIN_LOOP_SCHEDULE01_03_STATE:
            {
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                // 執行週期 => 根據Meas_Period去計算執行週期
                //

                if ( Calc_Elapsed_Time( cpu_timer_module.Cpu_Timer0_Time_Stamp_04, CpuTimer0.InterruptCount ) > (Uint32)Meas_Module.Meas_Period )
                {
                    cpu_timer_module.Cpu_Timer0_Time_Stamp_04 = CpuTimer0.InterruptCount;

                    //
                    // 量測計算流程
                    //
                    Measurement_Calc_Process(); //約83000ns

                    //
                    // Calculate the compensate of CV, CC and CP
                    Compensate_Calc_Process();

                    //
                    // SYNC鎖相流程
                    //
                    if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.SYNC == 1 )
                         && ( Phase_Sync_Module.LINE_Phase_Sync_Flags.bit.Phase_Sync_Process == 1 ) )
                    {
                        Phase_Sync_Module.LINE_Phase_Sync_Flags.bit.Phase_Sync_Process = 0;

                        Phase_Sync_Process();
                    }

                    //
                    #ifdef DEVELOP_SV01
                    /*--------------------Cycle量測 Step2--------------------*/
                    cpu_timer_module.Compensate_Calc_Process_Cycle_Meas_Result = CpuTimer0.InterruptCount - cpu_timer_module.Compensate_Calc_Process_Cycle_Meas_Start; //8 cycles

                    // 對應的執行時間 = ( Cycle_Meas_Result ) x 2.775us
                    /*--------------------Cycle量測 Step1--------------------*/
                    cpu_timer_module.Compensate_Calc_Process_Cycle_Meas_Start = CpuTimer0.InterruptCount;
                    #endif


                }

                //
                // 轉態
                //
                Main_State_Module.main_loop_schedule01_state = MAIN_LOOP_SCHEDULE01_01_STATE;

                break;
            }
        }

        //
        // 排程狀態機 02
        //
        switch( Main_State_Module.main_loop_schedule02_state )
        {
            case MAIN_LOOP_SCHEDULE02_01_STATE:
            {
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                // 執行週期 => 10ms
                //

                if ( Calc_Elapsed_Time( cpu_timer_module.Cpu_Timer0_Time_Stamp_02, CpuTimer0.InterruptCount ) > CPU_TIMER0_10ms )
                {

                    cpu_timer_module.Cpu_Timer0_Time_Stamp_02 = CpuTimer0.InterruptCount;

                    Calibration_Process();

                    Main_Process();

                    Event_Scan();

                    Remote_Sense_Limit_Status_Update();

                    Busy_State_Process();

                    Change_Output_Range_Process();
                }

                //
                // 轉態
                //
                Main_State_Module.main_loop_schedule02_state = MAIN_LOOP_SCHEDULE02_02_STATE;

                break;
            }

            case MAIN_LOOP_SCHEDULE02_02_STATE:
            {
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                // 執行週期 => 1s
                //

                if ( Calc_Elapsed_Time( cpu_timer_module.Cpu_Timer0_Time_Stamp_03, CpuTimer0.InterruptCount ) > CPU_TIMER0_1s ) //1s
                {

                    //GpioDataRegs.GPCCLEAR.bit.GPIO94 = 1;

                    cpu_timer_module.Cpu_Timer0_Time_Stamp_03 = CpuTimer0.InterruptCount;

                    Temp_Sensor_Update();

                    Fan_Speed_Command_Update(); //17 cycles

                    Ipk_Hold_Clear_Process();

                    Vpk_Hold_Clear_Process();

                    //GpioDataRegs.GPCSET.bit.GPIO94 = 1;

                    //
                    #ifdef DEVELOP_SV01
                    //
                    // 執行資源監控
                    //
                    cpu_timer_module.Ratio_Check_CPU0_div_CPU2 = (float)CpuTimer0.InterruptCount / (float)CpuTimer2.InterruptCount;
                    cpu_timer_module.Ratio_Check_CLA_div_CPU2 = (float)CLA_Module_Variables.Counter / (float)CpuTimer2.InterruptCount;
                    cpu_timer_module.Ratio_Check_CPU0_div_CLA = (float)CpuTimer0.InterruptCount / (float)CLA_Module_Variables.Counter;
                    #endif
                    //

                }


                //
                // 轉態
                //
                Main_State_Module.main_loop_schedule02_state = MAIN_LOOP_SCHEDULE02_03_STATE;

                break;
            }

            case MAIN_LOOP_SCHEDULE02_03_STATE:
            {
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //
                // 執行週期 => ASAP
                //

                LED_Flick_Process();

                //
                // 轉態
                //
                Main_State_Module.main_loop_schedule02_state = MAIN_LOOP_SCHEDULE02_01_STATE;

                break;
            }

        }

        /*--------------------Cycle量測(ns) Step1--------------------*/
        //cpu_timer_module.Cycle_Meas_Start_03 = EPwm4Regs.TBCTR;

        /*--------------------Cycle量測(ns) Step2--------------------*/
        //cpu_timer_module.Cycle_Meas_Result_03 = EPwm4Regs.TBCTR - cpu_timer_module.Cycle_Meas_Start_03; //8 cycles

        /*--------------------Cycle量測(ns) Step3--------------------*/
        // 量測的cycle = ( Cycle_Meas_Result - 3 ) x 2
        // 對應的執行時間 = ( Cycle_Meas_Result - 3 ) x 2 x 5ns

        asm("       NOP");                    // Wait one cycle
    }

}









////////////////////////////////////////////////////////////////////
void Input_Flag_Update_Process(void)
{
    //
    // 更新ERR狀態
    //
    if ( ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.SYNC == 1 )
         && ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source == SYNC_LINE ) )
    {
        //
        // SYNC LINE, ERR與ACFAIL的讀取時序要錯開
        //

        if ( Event_Module.Event4_Flag.bit.Input_Flags_Update )
        {

            #ifdef DEVELOP_SV61
            //
            // Bypass NG
            //
            Global_Variables.INPUT_FLAGS.bit.FANFAIL2 = 0;
            Global_Variables.INPUT_FLAGS.bit.FANFAIL1 = 0;
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_OTP = 1;
            Global_Variables.INPUT_FLAGS.bit.FANFAIL = 0;
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP1 = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP1 = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_UVP2 = 1;
            Global_Variables.INPUT_FLAGS.bit.PFC_OVP2 = 1;
            Global_Variables.INPUT_FLAGS.bit.DC_OVP = 1;
            #else
            //
            // 更新ERR
            //
            Input_Flags_Update_Event();
            #endif

            Event_Module.Event4_Flag.bit.Input_Flags_Update = 0;
        }
        else
        {
            Global_Variables.INPUT_FLAGS.bit.ACFAIL = ERR_INPUT;
        }
    }
    else
    {
        //
        // 其他模式下, 依照原本方式持續讀取
        //

        Input_Flags_Update_Event();
    }

    #ifdef BYPASS_ACFAIL
    Global_Variables.INPUT_FLAGS.bit.ACFAIL = 1;
    #endif
}


////////////////////////////////////////////////////////////////////
void Calc_Meas_Period_Process(void)
{
    float freqset, float_buf;

    switch(Global_Variables.OUTPUT_MODE_FLAG_Shadow.all)
    {
        case ACDC_INT_MODE: //ACDC-INT
        case MASTER_ACDC_INT_MODE:
        case SLAVE_ACDC_INT_MODE:
        {
            if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 0 )
            {
                //freqset = 60.0; //60Hz
                freqset = 1000.0; //1000Hz
            }
            else
            {
                freqset = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all * 0.01;
            }

            break;
        }
        case AC_SYNC_MODE: //AC-SYNC
        case ACDC_SYNC_MODE: //ACDC-SYNC
        case AC_EXT_MODE: //AC-EXT
        case ACDC_EXT_MODE: //ACDC-EXT
        {
            freqset = Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.all;
            break;
        }
        case MASTER_DC_INT_MODE:
        case SLAVE_DC_INT_MODE:
        case DC_INT_MODE: //DC-INT
        {
            //freqset = 60.0; //60Hz
            freqset = 1000.0; //1000Hz
            break;
        }
        case AC_INT_MODE: //AC-INT
        case MASTER_AC_INT_MODE:
        case SLAVE_AC_INT_MODE:
        case AC_VCA_MODE: //AC-VCA
        case AC_ADD_MODE: //AC-ADD
        case ACDC_ADD_MODE: //ACDC-ADD
        {
            freqset = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all * 0.01;
            break;
        }
        case SEQ_MODE: //SEQ
        {
            freqset = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;
            break;
        }
        default:
        {
            break;
        }
    }

    //
    // 頻率計算更新週期
    // 使用cpu timer0 2.775us
    //
    if ( freqset >= 200.0 )
    {
        //
        // 200Hz => 5000us => 1802 count*2.775us
        //
        float_buf =  1802.0;
    }
    else if ( freqset < 1.0 )
    {
        //
        // 60Hz => 16666.67us => 6006 count*2.775us
        //
        float_buf =  6006.0;
    }
    else
    {
        //
        // Example. 360360 / 60Hz = 6006 count*2.775us
        //
        float_buf = 360360.0 / freqset;
    }

    Meas_Module.Meas_Period = float_buf;

}



////////////////////////////////////////////////////////////////////
void Remote_Sense_Limit_Status_Update(void)
{
    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Remote_Sense_Enable == 1 )
    {
        Global_Variables.POWER_STATUS.bit.Remote_Sense_Limit = CLA_to_CPU_Variables.DAC_Clamp_Status.bit.VREF_Limit_High_Clamp;
    }
    else
    {
        Global_Variables.POWER_STATUS.bit.Remote_Sense_Limit = 0;
    }

}




////////////////////////////////////////////////////////////////////
void Busy_State_Process(void)
{
    //
    // Busy 狀態機
    //
    switch( Main_State_Module.Machine_Busy_State )
    {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MACHINE_BUSY_STATE_0:
        {
            if ( Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy == 1 )
            {
                //
                // 有事件處理, 觸發Busy
                //

                //
                // 轉態
                //
                Main_State_Module.Machine_Busy_State = MACHINE_BUSY_STATE_1;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MACHINE_BUSY_STATE_1:
        {
            //
            // 根據事件種類, 判斷是否解除Busy
            //
            if (    ( Event_Module.Event_Flag.bit.Load_Cali_Variable_from_EEPROM == 0 )
                 && ( Event_Module.Event_Flag.bit.Save_Cali_Variable_to_EEPROM == 0 )
                 && ( Event_Module.Event_Flag.bit.Set_DAC7612_CHA == 0 )
                 && ( Event_Module.Event_Flag.bit.Set_DAC7612_CHB == 0 )
                 && ( Event_Module.Event_Flag.bit.Model_Type_Change == 0 )
                 && ( Event_Module.Event_Flag.bit.Calc_VREFDAC_Limit == 0 )
                 && ( Event_Module.Event_Flag.bit.Calc_V_SlewRate_set == 0 )
                 && ( Event_Module.Event_Flag.bit.Set_Start_phase == 0 )
                 && ( Event_Module.Event_Flag.bit.Set_Stop_phase == 0 )
                 && ( Event_Module.Event_Flag.bit.Set_Cali_Variable_default == 0 )
                 && ( Event_Module.Event_Flag.bit.Set_Cali_Step == 0 )
                 && ( Event_Module.Event_Flag.bit.Set_Cali_Meter_Value == 0 )
                 && ( Event_Module.Event_Flag.bit.Cancel_Cali_Process == 0 )
                 && ( Event_Module.Event_Flag.bit.Reload_Wave_CF_set == 0 )
                 //&& ( Event_Module.Event_Flag.bit.SCIA_echo == 0 )
                 && ( Event_Module.Event_Flag.bit.MS_Sync_SDRAM_Process == 0 )

                 && ( Event_Module.Event2_Flag.bit.SYNC_Mode_Change_Phase_Delay == 0 )
                 && ( Event_Module.Event2_Flag.bit.Preload_SQU_WAVE_DATA == 0 )
                 && ( Event_Module.Event2_Flag.bit.Preload_WAVE_DATA == 0 )
                 //&& ( Event_Module.Event2_Flag.bit.SCIA_ask_service == 0 )
                 && ( Event_Module.Event2_Flag.bit.VI_raw_data_trig == 0 )
                 && ( Event_Module.Event2_Flag.bit.Update_Cali_CheckSum == 0 )
                 && ( Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change == 0 )
                 && ( Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_Jump_Count == 0 )
                 && ( Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_All_Step == 0 )
                 && ( Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_CheckSum == 0 )
                 && ( Event_Module.Event2_Flag.bit.Cali_Set_Max_DCV == 0 )
                 && ( Event_Module.Event2_Flag.bit.Cali_Set_Min_DCV == 0 )
                 && ( Event_Module.Event2_Flag.bit.Cali_Set_0_DCV == 0 )
                 && ( Event_Module.Event2_Flag.bit.Check_Cali_CheckSum == 0 )

                 && ( Event_Module.Event3_Flag.bit.ADD_Mode_Change_ACV_DCV_GAIN == 0 )
                 && ( Event_Module.Event3_Flag.bit.Change_FREQ == 0 )
                 && ( Event_Module.Event3_Flag.bit.Change_Waveform == 0 )
                 && ( Event_Module.Event3_Flag.bit.MS_Mode_Change_ACV_DCV == 0 )
                 && ( Event_Module.Event3_Flag.bit.MS_Mode_Change_Phase_Set == 0 )
                 && ( Event_Module.Event3_Flag.bit.Set_Output_ON_OFF == 0 )

                 //&& ( Event_Module.Event4_Flag.bit.Input_Flags_Update == 0 )
                 && ( Event_Module.Event4_Flag.bit.Load_HL_Cali_Variable_from_EEPROM == 0 )
                 && ( Event_Module.Event4_Flag.bit.Update_HL_Cali_CheckSum == 0 )
                 && ( Event_Module.Event4_Flag.bit.Load_HL_Cali_to_Cali == 0 )
                 && ( Event_Module.Event4_Flag.bit.Set_Output_Range_Option == 0 )
                 && ( Event_Module.Event4_Flag.bit.Change_Output_Range == 0 )
                 && ( Event_Module.Event4_Flag.bit.Adj_Output_Range == 0 )
                 && ( Event_Module.Event4_Flag.bit.Save_HL_Cali_Variable_to_EEPROM == 0 ) )
            {

                if ( ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
                     && ( Global_Variables.POWER_STATUS.bit.MS_Master == 1 ) )
                {
                    //
                    // MS Master
                    //

                    if ( MS_Control_Module_Variables.Master_SCI_Write_CMD.all == 0 )
                    {
                        // 解除Busy狀態
                        //
                        Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 0;
                    }
                }
                else
                {
                    //
                    // 單機狀態
                    //

                    // 解除Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 0;
                }


                //
                // 轉態
                //
                Main_State_Module.Machine_Busy_State = MACHINE_BUSY_STATE_0;
            }

            break;
        }

    }

}



////////////////////////////////////////////////////////////////////
void Vpk_Hold_Clear_Process(void)
{
    //
    // VPK hold clear
    // 設定CPU_to_CLA_Variables.VPK_Hold_Clear = 1, 持續約2sec, 然後清掉
    //
    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.V_peak_hold_clear == 1 )
    {
        CPU_to_CLA_Variables.VPK_Hold_Clear = 1;

        cpu_timer_module.V_peak_hold_clear_counter --;

        if ( cpu_timer_module.V_peak_hold_clear_counter == 0 )
        {
            cpu_timer_module.V_peak_hold_clear_counter = 2;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.V_peak_hold_clear = 0;
            CPU_to_CLA_Variables.VPK_Hold_Clear = 0;
        }
    }

}





////////////////////////////////////////////////////////////////////
void Ipk_Hold_Clear_Process(void)
{
    //
    // IPK hold clear
    // 設定CPU_to_CLA_Variables.IPK_Hold_Clear = 1, 持續約2sec, 然後清掉
    //
    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_hold_clear == 1 )
    {
        CPU_to_CLA_Variables.IPK_Hold_Clear = 1;

        cpu_timer_module.I_peak_hold_clear_counter --;

        if ( cpu_timer_module.I_peak_hold_clear_counter == 0 )
        {
            cpu_timer_module.I_peak_hold_clear_counter = 2;
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_hold_clear = 0;
            CPU_to_CLA_Variables.IPK_Hold_Clear = 0;
        }
    }

}



////////////////////////////////////////////////////////////////////
void Temp_Sensor_Update(void)
{
    Global_Variables.INPUT_VARIABLES.Temp_Sensor = (float)( Read_MCP3301_Sequence() & 0x1FFF ) * ADC_TO_TEMP_C; //3527 cycle

    #ifdef DEVELOP_SV39
    //Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = (float)SPI_Module_Variables.MS_SPI_RX_NG_Count / 100.0;
    Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = cpu_timer_module.Ratio_Check_CPU0_div_CPU2;
    //Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = (float)Global_Variables.MS_Function_Variables.MS_Address;
    //Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = (float)MS_Control_Module_Variables.MS_Connect_State;
    //Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = CLA_to_CPU_Variables.I_FB;
    //Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = (float)Global_Variables.POWER_FUNCTIONS_ERROR_CODE;
    //Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = (float)cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Max;
    #else
    Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.all = Global_Variables.INPUT_VARIABLES.Temp_Sensor;
    #endif

}





////////////////////////////////////////////////////////////////////
void LED_Flick_Process(void)
{
    //
    // GPIO94 LED 閃爍控制 (x 1ms)
    //

    //
    // 機器狀態切換閃爍週期
    //
    switch(Main_State_Module.Machine_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        case POWER_UP_STATE:
        case SELF_TEST_STATE:
        {
            cpu_timer_module.LED_Flick_Interval = 500;
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case READY_STATE0:
        case READY_STATE:
        case OUTPUT_STATE:
        {
            cpu_timer_module.LED_Flick_Interval = 1000;
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ERROR_HANDLE_STATE0:
        case ERROR_HANDLE_STATE1:
        case ERROR_HANDLE_STATE2:
        case ERROR_STATE:
        {
            cpu_timer_module.LED_Flick_Interval = 100;
            break;
        }
    }

    if ( Calc_Elapsed_Time( cpu_timer_module.Cpu_Timer2_Time_Stamp_01, CpuTimer2.InterruptCount ) > cpu_timer_module.LED_Flick_Interval )
    {
        cpu_timer_module.Cpu_Timer2_Time_Stamp_01 = CpuTimer2.InterruptCount;

        GpioDataRegs.GPCTOGGLE.bit.GPIO94 = 1;
    }

}


////////////////////////////////////////////////////////////////////
void Change_Output_Range_Process(void)
{
    switch( Main_State_Module.Change_Output_Range_State )
    {

        /*-----------------------------------------------------------------------------------------*/
        case CHANGE_OUTPUT_RANGE_STATE_0:
        {
            if ( Event_Module.Event4_Flag.bit.Change_Output_Range )
            {

                //
                // 轉態
                //
                Main_State_Module.Change_Output_Range_State = CHANGE_OUTPUT_RANGE_STATE_1;
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case CHANGE_OUTPUT_RANGE_STATE_1:
        {
            if ( Main_State_Module.Machine_state == READY_STATE )
            {
                //
                // 等機器回到Ready
                //

                //
                // 轉態
                //
                Main_State_Module.Change_Output_Range_State = CHANGE_OUTPUT_RANGE_STATE_2;
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case CHANGE_OUTPUT_RANGE_STATE_2:
        {

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Change Output_Range -> %u\n", CpuTimer2.InterruptCount, Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

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

            //
            // 轉態
            //
            Main_State_Module.Change_Output_Range_State = CHANGE_OUTPUT_RANGE_STATE_3;


            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case CHANGE_OUTPUT_RANGE_STATE_3:
        {
            //
            // 判斷是否繼續輸出
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow == 1 )
            {
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow;

                Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;

                // 設定Busy狀態
                //
                Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            }

            Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user_shadow = 0;

            //
            // 清掉事件
            //
            Event_Module.Event4_Flag.bit.Change_Output_Range = 0;

            //
            // 轉態
            //
            Main_State_Module.Change_Output_Range_State = CHANGE_OUTPUT_RANGE_STATE_0;

            break;
        }
    }
}



//
// End of file
//
