//###########################################################################
//
// FILE:    CPU_Timer_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210901
//###########################################################################

#include "Global_VariableDefs.h"

volatile CPU_TIMER_MODULE_REG cpu_timer_module;

static inline INT_Mode_Output_State_Process(void);
static inline Master_INT_Mode_Output_State_Process(void);
static inline Slave_INT_Mode_Output_State_Process(void);
static inline SYNC_Mode_Output_State_Process(void);
static inline EXT_Mode_Output_State_Process(void);
static inline ADD_Mode_Output_State_Process(void);
static inline VCA_Mode_Output_State_Process(void);
static inline SEQ_Mode_Output_State_Process(void);

static inline VI_RAW_Sampling_Process(void);
static inline Calc_SlewRate_Process(void);
static inline Calc_SlewRate_Process_2(void);
static inline SPI_Parser_Process(void);
static inline EXTIO_Update_Process(void);
static inline Error_OFF_Process(void);
static inline Zero_Cross_Process(void);
static inline SYNC_Period_Meas_Process(void);
static inline EXT_Period_Meas_Process(void);
static inline ADD_Calc_DC_Level_Process(void);
static inline EXT_Calc_ACV_Set_Process(void);
static inline MS_SDRAM_Sending_Process(void);
static inline MS_SDRAM_Receving_Process(void);
static inline MS_Balance_Fast_Compensate_Process(void);


static inline CMD_Parser_Process2( volatile Uint16 *rx_buffer, volatile Uint16 *tx_buffer );


////////////////////////////////////////////////////////////////////
void Init_CPU_Timer_Module_Variables(void)
{
    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;


    cpu_timer_module.Value_Set_User_Change = 0;

    cpu_timer_module.EXTAD_TTL_time_stamp = 0;

    cpu_timer_module.EXTAD_state = EXTAD_INIT_STATE;

    cpu_timer_module.EXTAD_Change_Counter = 0;

    cpu_timer_module.EXTAD_TTL_Period = 0;
    cpu_timer_module.EXTAD_TTL_Period_shadow = 0;

    cpu_timer_module.Cpu_Timer2_Time_Stamp_01 = 0;
    cpu_timer_module.Cpu_Timer2_Time_Stamp_02 = 0;
    cpu_timer_module.Cpu_Timer2_Time_Stamp_03 = 0;
    cpu_timer_module.LED_Flick_Interval = 1000;

    cpu_timer_module.I_peak_hold_clear_counter = 2;
    cpu_timer_module.V_peak_hold_clear_counter = 2;

    cpu_timer_module.cpu_timer0_schedule01_state = CPU_TIMER0_SCHEDULE01_01_STATE;

    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;


    cpu_timer_module.Debug01 = 0;
    cpu_timer_module.Debug02 = 0;
    cpu_timer_module.Debug03 = 0;
    cpu_timer_module.Debug04 = 0;

    cpu_timer_module.SEQ_Output_ON_state = SEQ_OUTPUT_ON_STATE0;

    cpu_timer_module.Zero_Cross_State = ZERO_CROSS_INIT_STATE;
    cpu_timer_module.Zero_Cross_Width_Count = 0;
    cpu_timer_module.Zero_Cross_Width_Counter = 0;

    //
    #ifdef DEVELOP_SV01
    cpu_timer_module.ATE_Debug_Meas_Result = 0;
    cpu_timer_module.ATE_Debug_Meas_Start = 0;
    cpu_timer_module.ATE_Debug_Trig = 0;
    cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Max = 0;
    #endif


    cpu_timer_module.Exec_Switch = 0;

}








////////////////////////////////////////////////////////////////////
void Init_CPU_Timer_Reg(void)
{
    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;
    //PieVectTable.TIMER1_INT = &cpu_timer1_isr;
    PieVectTable.TIMER2_INT = &cpu_timer2_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers

    //
    // Initialize the Device Peripheral. This function can be
    // found in F2837xS_CpuTimers.c
    //
    InitCpuTimers();   // For this example, only initialize the Cpu Timers

    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 200MHz CPU Freq, setup Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, CPU_FREQ, CPU0_PERIOD);
    //ConfigCpuTimer(&CpuTimer1, CPU_FREQ, CPU1_PERIOD);
    ConfigCpuTimer(&CpuTimer2, CPU_FREQ, CPU2_PERIOD);

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed in
    // ConfigCpuTimer and InitCpuTimers (in F2837xS_cputimervars.h), the below
    // settings must also be updated.
    //
    CpuTimer0Regs.TCR.all = 0x4000;
    //CpuTimer1Regs.TCR.all = 0x4000;
    CpuTimer2Regs.TCR.all = 0x4000;

    //
    // User specific code, enable interrupts:
    //

    //
    // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
    // which is connected to CPU-Timer 1, and CPU int 14, which is connected
    // to CPU-Timer 2:
    //
    IER |= M_INT1;
    //IER |= M_INT13;
    IER |= M_INT14;

    //
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

}




////////////////////////////////////////////////////////////////////
//
// cpu_timer0_isr - CPU Timer0 ISR with interrupt counter
//
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    //
    #ifdef DEVELOP_SV01
    cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Start = EPwm4Regs.TBCTR;
    //量測cycle
    #endif


    //Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

    switch ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all )
    {
        /*-------------------------------------------------------------------------------------------------------*/
        case DC_INT_MODE: //DC-INT
        case AC_INT_MODE: //AC-INT
        case ACDC_INT_MODE: //ACDC-INT
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            INT_Mode_Output_State_Process();

            VI_RAW_Sampling_Process();


            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();
                Error_OFF_Process();
                Zero_Cross_Process();

                CHECK_THE_CHANGE_SET_USER();


                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;
            }
            else
            {
                //
                // 輸出中判斷轉態處理
                //
                if ( cpu_timer_module.cpu_timer0_state == ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE )
                {
                    //
                    // 停止輸出處理
                    //
                    if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
                    {
                        cpu_timer_module.Value_Set_User_Change = 0;


                        if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 0 )
                        {
                            //
                            // OFF Phase Disable
                            //

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

                        }
                        else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 1 )
                        {
                            //
                            // OFF Phase Enable
                            //

                            //
                            // 等待滿一個週期再轉態
                            //
                            if ( Global_Variables.dds_shadow3.all > Global_Variables.dds.all )
                            {

                                //
                                // 轉態
                                //
                                cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE; //轉態

                            }
                        }
                    }

                    //
                    // 改變設定值處理
                    //
                    if ( cpu_timer_module.Value_Set_User_Change == 1 )
                    {
                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0; //轉態
                    }
                }

                Global_Variables.dds_shadow3.all = Global_Variables.dds.all;


                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process();


            break;
        }

        /*-------------------------------------------------------------------------------------------------------*/
        case SEQ_MODE: //SEQ
        {
            Uint32 wave_addr, wave_addr_1;
            float wave_c2, wave_c2_1;
            //float wave_value;
            Uint32 wave_page;

            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;


            wave_addr = Global_Variables.dds.addr;
            wave_addr_1 = ( Global_Variables.dds.addr + 1 ) & 0x0FFF;

            cpu_timer_module.wave_c2_decimal = (float)Global_Variables.dds.dec1 * 0.003921569;


            if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != WAVE_SQU )
            {
                //
                // 即時從SDRAM載入WAVE
                //
                wave_page = (Uint32)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow << 12 ;
                wave_c2 = (float)*( SDRAM_variables.SDRAMBuf + wave_page + wave_addr );
                wave_c2_1 = (float)*( SDRAM_variables.SDRAMBuf + wave_page + wave_addr_1 );

                //
                // 內差法計算wave value
                //
                cpu_timer_module.wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;
            }
            else if ( (Uint16)Global_Variables.dds.addr <= SEQ_Step_Section5.SEQ_SQU_Duty_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step ] )
            {
                cpu_timer_module.wave_value = 32767.0;
            }
            else if ( (Uint16)Global_Variables.dds.addr > SEQ_Step_Section5.SEQ_SQU_Duty_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step ] )
            {
                cpu_timer_module.wave_value = -32767.0;
            }



            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();
                Error_OFF_Process();
                Zero_Cross_Process();

                CHECK_THE_CHANGE_SET_USER();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;
            }
            else
            {
                SEQ_Mode_Output_State_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;

            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process_2();

            break;
        }
        /*-------------------------------------------------------------------------------------------------------*/
        case AC_SYNC_MODE: //AC-SYNC
        case ACDC_SYNC_MODE: //ACDC-SYNC
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            SYNC_Mode_Output_State_Process();


            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                //
                // 觸發ADC 16bit轉換
                //
                AdcaRegs.ADCSOCFRC1.all = 0x0001; //SOC0

                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();
                Error_OFF_Process();
                Zero_Cross_Process();

                CHECK_THE_CHANGE_SET_USER();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;
            }
            else
            {
                SYNC_Period_Meas_Process();

                //
                // 讀取ADC 16bit
                //
                AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
                DSP_ADC_16bit_Module_Variables.Channel1 = AdcaResultRegs.ADCRESULT0;
                Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN = DSP_ADC_16bit_Module_Variables.Channel1;

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////


            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process();

            break;
        }
        /*-------------------------------------------------------------------------------------------------------*/
        case AC_EXT_MODE: //AC-EXT
        case ACDC_EXT_MODE: //ACDC-EXT
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            EXT_Mode_Output_State_Process();


            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                //
                // 觸發ADC 16bit轉換
                //
                AdcaRegs.ADCSOCFRC1.all = 0x0001; //SOC0

                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();
                Error_OFF_Process();

                //
                // EXT跟DDS沒有同步
                //
                //Zero_Cross_Process();

                CHECK_THE_CHANGE_SET_USER();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;
            }
            else
            {
                EXT_Period_Meas_Process();

                //
                // 讀取ADC 16bit
                //
                AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
                DSP_ADC_16bit_Module_Variables.Channel1 = AdcaResultRegs.ADCRESULT0;
                Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN = DSP_ADC_16bit_Module_Variables.Channel1;

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process_2();

            break;
        }
        /*-------------------------------------------------------------------------------------------------------*/
        case AC_ADD_MODE: //AC-ADD
        case ACDC_ADD_MODE: //ACDC-ADD
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            ADD_Mode_Output_State_Process();


            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                //
                // 觸發ADC 16bit轉換
                //
                AdcaRegs.ADCSOCFRC1.all = 0x0001; //SOC0

                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();
                Error_OFF_Process();
                Zero_Cross_Process();

                CHECK_THE_CHANGE_SET_USER();


                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;
            }
            else
            {
                ADD_Calc_DC_Level_Process();

                //
                // 讀取ADC 16bit
                //
                AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
                DSP_ADC_16bit_Module_Variables.Channel1 = AdcaResultRegs.ADCRESULT0;
                Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN = DSP_ADC_16bit_Module_Variables.Channel1;

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process_2();

            break;
        }
        /*-------------------------------------------------------------------------------------------------------*/
        case AC_VCA_MODE: //AC-VCA
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            VCA_Mode_Output_State_Process();

            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                //
                // 觸發ADC 16bit轉換
                //
                AdcaRegs.ADCSOCFRC1.all = 0x0001; //SOC0

                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();
                Error_OFF_Process();
                Zero_Cross_Process();

                CHECK_THE_CHANGE_SET_USER();


                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;
            }
            else
            {
                EXT_Calc_ACV_Set_Process();

                //
                // 讀取ADC 16bit
                //
                AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
                DSP_ADC_16bit_Module_Variables.Channel1 = AdcaResultRegs.ADCRESULT0;
                Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN = DSP_ADC_16bit_Module_Variables.Channel1;

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process();


            break;
        }
        /*-------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------*/
        case MASTER_DC_INT_MODE:
        case MASTER_AC_INT_MODE:
        case MASTER_ACDC_INT_MODE:
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            //
            // Master給Slave的dds, 領先一個step
            //
            //Global_Variables.dds_to_slave.all = Global_Variables.dds.all + Global_Variables.FreqStep;
            Global_Variables.dds_to_slave.all = Global_Variables.dds.all;

            //
            // 更新
            //
            SPI_Module_Variables.SPIB_Tx_Data_Shadow[0] = ( Global_Variables.dds_to_slave.addr << 4 ) + 1;
            SPI_Module_Variables.SPIB_Tx_Data_Shadow[1] = (Uint16)( CLA_to_CPU_Variables.I_FB_shadow2 * 100.0 + 8000.0 ) & 0xFFFE;
            SPI_Module_Variables.SPIB_Tx_Data_Shadow[2] = ( Global_Variables.dds_to_slave.dec1 << 8 ) + Global_Variables.MS_Function_Variables.Output_Sync_Trig.all;

            //
            // SPI 傳送
            //
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[0];
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[1];
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[2];

            Master_INT_Mode_Output_State_Process();



            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();
                Error_OFF_Process();
                Zero_Cross_Process();

                CHECK_THE_CHANGE_SET_USER();


                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;
            }
            else
            {

                //MS_SDRAM_Sending_Process();

                if( cpu_timer_module.cpu_timer0_state == MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE )
                {
                    //
                    // 停止輸出處理
                    //
                    if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
                    {
                        cpu_timer_module.Value_Set_User_Change = 0;

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0;
                    }

                    //
                    // 改變設定值處理
                    //
                    if ( cpu_timer_module.Value_Set_User_Change == 1 )
                    {

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0; //轉態
                    }
                }

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process_2();



            break;
        }
        /*-------------------------------------------------------------------------------------------------------*/
        /*-------------------------------------------------------------------------------------------------------*/
        case SLAVE_DC_INT_MODE:
        case SLAVE_AC_INT_MODE:
        case SLAVE_ACDC_INT_MODE:
        {
            //Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;


            //
            // 判斷RX
            //
            if ( SpibRegs.SPIFFRX.bit.RXFFINT )
            {
                SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SpibRegs.SPIRXBUF;

                if ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] & 0x01 ) == 0 )
                {
                    SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SpibRegs.SPIRXBUF;

                    if ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] & 0x01 ) == 0 )
                    {
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SpibRegs.SPIRXBUF;
                    }
                }

                SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SpibRegs.SPIRXBUF;
                SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SpibRegs.SPIRXBUF;

#ifdef BYPASS
                //
                // 處理順序錯位
                //
                if ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] & 0x01 ) == 0 )
                {
                    if ( ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] & 0x01 ) == 1 ) )
                    {
                        Uint16 tmp;
                        tmp = SPI_Module_Variables.SPIB_Rx_Data_Shadow[0];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[1];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[2];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = tmp;
                    }
                    else if ( ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] & 0x01 ) == 1 ) )
                    {
                        Uint16 tmp;
                        tmp = SPI_Module_Variables.SPIB_Rx_Data_Shadow[0];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[2];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[1];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = tmp;
                    }
                }
#endif
                SpibRegs.SPIFFRX.bit.RXFFINTCLR=1;  // Clear Interrupt flag
            }


#ifdef BYPASS
            SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SpibRegs.SPIRXBUF;
            SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SpibRegs.SPIRXBUF;
            SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SpibRegs.SPIRXBUF;
            SPI_Module_Variables.SPIB_Rx_Data_Shadow[3] = SpibRegs.SPIRXBUF;
            SPI_Module_Variables.SPIB_Rx_Data_Shadow[4] = SpibRegs.SPIRXBUF;

            SpibRegs.SPIFFRX.bit.RXFIFORESET = 0;
            SpibRegs.SPIFFRX.bit.RXFIFORESET = 1;


            //
            // 處理順序錯位
            //
            if ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] & 0x01 ) == 0 )
            {
                if ( ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] & 0x01 ) == 1 ) )
                {
                    SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[1];
                    SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[2];
                    SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[3];
                }
                else if ( ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] & 0x01 ) == 1 ) )
                {
                    SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[2];
                    SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[3];
                    SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[4];
                }
            }
#endif

#ifdef BYPASS
            if ( SpibRegs.SPIFFRX.bit.RXFFINT )
            {
                SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SpibRegs.SPIRXBUF;
                SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SpibRegs.SPIRXBUF;
                SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SpibRegs.SPIRXBUF;
                SPI_Module_Variables.SPIB_Rx_Data_Shadow[3] = SpibRegs.SPIRXBUF;
                SPI_Module_Variables.SPIB_Rx_Data_Shadow[4] = SpibRegs.SPIRXBUF;

                //
                // 處理順序錯位
                //
                if ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] & 0x01 ) == 0 )
                {
                    if ( ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] & 0x01 ) == 1 ) )
                    {
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[1];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[2];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[3];
                    }
                    else if ( ( ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] & 0x01 ) == 1 ) )
                    {
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[2];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[3];
                        SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] = SPI_Module_Variables.SPIB_Rx_Data_Shadow[4];
                    }
                }

                SpibRegs.SPIFFRX.bit.RXFFINTCLR=1;  // Clear Interrupt flag
            }
#endif

            Slave_INT_Mode_Output_State_Process();

            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();
                Error_OFF_Process();
                Zero_Cross_Process();

                CHECK_THE_CHANGE_SET_USER();


                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;
            }
            else
            {
                MS_Balance_Fast_Compensate_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process_2();


            break;
        }
        /*-------------------------------------------------------------------------------------------------------*/
        case MASTER_DC_INT_SDRAME_SYNC_MODE:
        case MASTER_AC_INT_SDRAME_SYNC_MODE:
        case MASTER_ACDC_INT_SDRAME_SYNC_MODE:
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            //
            // 輸出0
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = WAVE_DAC_ZERO;
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            Output_Control_Module.WAVE_Compensate_Offset2 = 0;



            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 120kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else if ( cpu_timer_module.Exec_Switch == 1 )
            {
                EXTIO_Update_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 2;

            }
            else if ( cpu_timer_module.Exec_Switch == 2 )
            {
                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 3;

            }
            else
            {
                MS_SDRAM_Sending_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process_2();

            break;
        }

        /*-------------------------------------------------------------------------------------------------------*/
        case SLAVE_DC_INT_SDRAME_SYNC_MODE:
        case SLAVE_AC_INT_SDRAME_SYNC_MODE:
        case SLAVE_ACDC_INT_SDRAME_SYNC_MODE:
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            //
            // 輸出0
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = WAVE_DAC_ZERO;
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            Output_Control_Module.WAVE_Compensate_Offset2 = 0;

            //
            // 判斷RX
            //
            if ( SpibRegs.SPIFFRX.bit.RXFFINT )
            {
                SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig = 1;

                SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] = SpibRegs.SPIRXBUF;
                SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] = SpibRegs.SPIRXBUF;

                SpibRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
                SpibRegs.SPIFFRX.bit.RXFFINTCLR=1;  // Clear Interrupt flag
            }

            MS_SDRAM_Receving_Process();


            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 180kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else
            {
                EXTIO_Update_Process();


                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process_2();

            break;
        }


        /*-------------------------------------------------------------------------------------------------------*/
        default:
        {
            Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.FreqStep;

            //
            // !!!非定義的工作模式!!!
            //
            //

            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = WAVE_DAC_ZERO;
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            Output_Control_Module.WAVE_Compensate_Offset2 = 0;

            ////////////////////////////////////////////////////////////////////////////////////////////
            //
            // 180kHz 執行區段
            //
            if ( cpu_timer_module.Exec_Switch == 0 )
            {
                SPI_Parser_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 1;
            }
            else
            {
                EXTIO_Update_Process();
                Error_OFF_Process();

                //
                // 轉態
                //
                cpu_timer_module.Exec_Switch = 0;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////

            //
            // 補償offset更新
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset
                                                                     + (int32)Output_Control_Module.WAVE_Compensate_Offset2;

            Calc_SlewRate_Process();


            break;
        }

    }
    /*--------- switch ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all ) ---------*/


    //
    // 更新到CLA
    //
    CPU_to_CLA_Variables.WAVE_set = Global_Variables.OUTPUT_VARIABLES.WAVE_set;
    CPU_to_CLA_Variables.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set;
    CPU_to_CLA_Variables.FreqStep = Global_Variables.FreqStep;

    CPU_to_CLA_Variables.dds.all = Global_Variables.dds.all;

    //
    // 更新dds_shadow for 360kHz
    //
    Global_Variables.dds_shadow.all = Global_Variables.dds.all;


    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    //
    #ifdef DEVELOP_SV01
    cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Result = EPwm4Regs.TBCTR - cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Start; //8 cycles
    // 量測的cycle = ( Cycle_Meas_Result - 3 ) x 2
    // 對應的執行時間 = ( Cycle_Meas_Result - 3 ) x 2 x 5ns

    //GpioDataRegs.GPCSET.bit.GPIO94 = 1;

    if ( cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Result > cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Max )
    {
        cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Max = cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Result;
    }
    #endif


}


////////////////////////////////////////////////////////////////////
//
// cpu_timer1_isr - CPU Timer1 ISR
//
/*
__interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;

}
*/

////////////////////////////////////////////////////////////////////
//
// cpu_timer2_isr CPU Timer2 ISR
//
__interrupt void cpu_timer2_isr(void)
{
    CpuTimer2.InterruptCount++;

}





////////////////////////////////////////////////////////////////////
//
// 計算剩餘時間,搭配cputimer使用, 有考慮溢味計算
//
// example:
// 1.事先設定CpuTimer2.InterruptCount會累加
// 2.抓取時間
//   time1 = CpuTimer2.InterruptCount;
// 3.之後在程式中呼叫執行就會回傳對比timer經過幾個count
//   Calc_Elapsed_Time( time1, CpuTimer2.InterruptCount )
//
Uint32 Calc_Elapsed_Time(Uint32 start_time, Uint32 current_time)
{
    Uint32 elapsed_time;

    if ( start_time > current_time )
    {
        elapsed_time = 0xFFFFFFFF - start_time + current_time + 1;

        return( elapsed_time );
    }
    else
    {
        return( current_time - start_time );
    }
}



////////////////////////////////////////////////////////////////////
//
//
//
static inline INT_Mode_Output_State_Process(void)
{
    Uint16 wave_addr, wave_addr_1;
    float wave_c2, wave_c2_1;
    float wave_value;


    wave_addr = (Uint16)Global_Variables.dds.addr;
    wave_addr_1 = ( (Uint16)Global_Variables.dds.addr + 1 ) & 0x0FFF;

    cpu_timer_module.wave_c2_decimal = (float)Global_Variables.dds.dec1 * 0.003921569;

    //
    // DAC輸出
    //
    //DacaRegs.DACVALS.all = (Uint16)( CLA_to_CPU_Variables.V_FB * 7.447273 );

    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != WAVE_SQU )
    {
        //
        // 從WAVE buffer載入WAVE
        //
        wave_c2 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr ];
        wave_c2_1 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr_1 ];

        //
        // 內差法計算wave value
        //
        wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;
    }
    else if ( Global_Variables.dds.addr <= Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = 32767.0;
    }
    else if ( Global_Variables.dds.addr > Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = -32767.0;
    }

    //
    // 輸出主中斷流程
    //
    switch ( cpu_timer_module.cpu_timer0_state )
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // Ready
        //
        case CPU_TIMER0_OUTPUT_READY_STATE:
        {
            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE0:
        {
            //
            // 開補償讓輸出從0V開始
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow = 0;

            //
            // 量測補償ON
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 1;

            //
            // 計時等100ms, 讓輸出追到0V再轉態
            //

            Output_ACDC_INT_Module.DCV_Zero_Stable_Counter ++;

            if ( Output_ACDC_INT_Module.DCV_Zero_Stable_Counter > 108108 ) // 至少要讓補償執行
            {
                //
                // 計數約1000ms ( => 1000000u / 2.775u = 360360 )
                //

                Output_ACDC_INT_Module.DCV_Zero_Stable_Counter = 0;

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all;

                //
                // 量測補償OFF
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

                //
                // 設定EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE1;

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE1:
        {

            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( (float)Output_ACDC_INT_Module.V_Rise_Counter * Output_ACDC_INT_Module.WAVE_per_Count ) + 32767;

            Output_ACDC_INT_Module.V_Rise_Counter ++;

            if ( (float)Output_ACDC_INT_Module.V_Rise_Counter > Output_ACDC_INT_Module.V_Rise_Count )
            {

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE2;
            }

            //
            // 停止輸出處理
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {
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
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE2:
        {
            //
            // 快到達目標值
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow )
                    <= Global_Variables.OUTPUT_VARIABLES.WAVE_set_diff_level )
            {
                //
                // 量測補償ON
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 1;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE;

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-INT
        // ON_Phase 0
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            //
            // 設定EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-INT
        // ON_Phase 1
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1:
        {
            //
            // 快到達目標值
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow )
                    <= Global_Variables.OUTPUT_VARIABLES.WAVE_set_diff_level )
            {
                if ( Memory_Module.wave_buffer_section == 0 )
                {
                    //
                    // SIN, Slew Rate => OFF
                    //
                    cpu_timer_module.slew_rate_state = NO_SLEWRATE_STATE;
                }
                else
                {
                    //
                    // Slew Rate => ON
                    //
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;
                }

                //
                // 載入start phase到dds
                //
                Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;

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
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE;

                break;
            }


            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-INT
        // Output_ON
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

#ifdef BYPASS
            //
            // 停止輸出處理
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {
                cpu_timer_module.Value_Set_User_Change = 0;


                if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 0 )
                {
                    //
                    // OFF Phase Disable
                    //

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

                    break;
                }
                else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 1 )
                {
                    //
                    // OFF Phase Enable
                    //

                    //
                    // 等待滿一個週期再轉態
                    //
                    if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                    {

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE; //轉態

                        break;
                    }
                }
            }


            //
            // 改變設定值處理
            //
            if ( cpu_timer_module.Value_Set_User_Change == 1 )
            {
                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0; //轉態

                break;
            }
#endif
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-INT
        // Change_Phase 0
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0:
        {
            float scale, offset;
            //Uint32 sync_change_phase;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );


            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Zero_Change_Phase_Enable == 1 )
            {
                //
                // Zero Change Enable
                //

                if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                {
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                    //
                    // Reset WAVE以及VREF補償量
                    //
                    Output_Control_Module.WAVE_Compensate_Offset = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                    break;
                }

            }
            else
            {
                //
                // Zero Change Disable
                //

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                //
                // Reset WAVE以及VREF補償量
                //
                Output_Control_Module.WAVE_Compensate_Offset = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-INT
        // Change_Phase 1
        //
        /*
        case ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1:
        {

            break;
        }
        */
        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-INT
        // Change_Phase 2
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2:
        {
            cpu_timer_module.Value_Set_User_Change = 0;

            //
            // 載入頻率Step
            //
            Global_Variables.FreqStep = Global_Variables.FreqStep_shadow;

            //
            // 載入wave_buffer_section, 改變輸出波形
            //
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;


            //
            // 更新WAVE參數
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;

            Global_Variables.OUTPUT_VARIABLES.WAVE_offset = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;
            Global_Variables.OUTPUT_VARIABLES.WAVE_scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow;



            // 更新VREF
            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

            //
            // 設定值改變後的起始相角
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;

            //
            // Reload dds
            //
            Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;



            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;

            Output_Control_Module.DCV_Compensate_State = DCV_NO_COMPENSATE;
            Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;


            //
            // Reset WAVE以及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-INT
        // Stop_Phase
        //
        case ACDC_INT_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE:
        {
            float scale, offset;
            Uint32 stop_phase_count_shadow;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            stop_phase_count_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_phase_count;


            //
            // 正常停止
            //
            if ( Global_Variables.dds.all >= stop_phase_count_shadow )
            {
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

                break;

            }

            break;
        }

//int
        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 0
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            //
            // 量測補償OFF
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

            //
            // 判斷是否降到0
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow ) <= 32.767 )
            {
                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE1;
                }
                else
                {
                    //
                    // 機器沒有輸出, 或有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;
                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 1
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            //
            // 逆電流卸載PID 初始化
            //

            //
            // 初始化補償器
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 2
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE2:
        {
            //
            // 逆電流卸載PID Loop
            //

            //
            // 補償計算
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB;

            // Compute the error
            // 計算Err
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref - PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb;

            // Compute the proportional output
            // 計算比例Up
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err;

            // Compute the integral output
            // 計算積分Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui + ( PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up );

            // Compute the pre-saturated output
            // 計算Up+Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up + PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui;

            // Saturate the output
            if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat > PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax;
            }
            else if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat < PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin;
            }
            else
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat;
            }

            //
            // Update到CLA模組去補償WAVE DAC
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset2 = -PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 3
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE3:
        {
            float vref_zero;
            float step;

            vref_zero = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            step = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step;

            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - vref_zero ) <= step )
            {
                //
                // 靠近 vref_zero
                //

                //
                // DAC降到zero
                //
                Init_VREF_WAVE_Process();

                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE4;
                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;
                }
            }
            else
            {
                //
                // 每次遞減
                //

                Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - step );
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 4
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE4:
        {
            //
            // 等待被觸發轉態
            //

            break;
        }
    }
    /*--------- switch ( cpu_timer_module.cpu_timer0_state ) ---------*/

}





////////////////////////////////////////////////////////////////////
//
//
//
static inline VI_RAW_Sampling_Process(void)
{
    //
    // THD分析使用, 只有AC INT
    // ( 20230818 實測只能放在360kHz )
    //
    #ifdef DEVELOP_SV69
    #else
    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_INT_MODE )
    #endif
    {
        /*---------------------------------------------------------------------------------------------------*/
        //
        // 取樣狀態機
        //
        switch ( Meas_Module.Meas_sampling_state )
        {
            /*------------------------------------------------------*/
            case meas_sampling_ready_state:
            {
                if ( Meas_Module.Flags.bit.VI_RAW_OP_CODE_Trig == 1 )
                {
                    Meas_Module.Flags.bit.VI_RAW_OP_CODE_Trig = 0;

                    //
                    // 轉態
                    //
                    Meas_Module.Meas_sampling_state = wait_period_start_state;

                    break;
                }

                break;
            }

            /*------------------------------------------------------*/
            case wait_period_start_state:
            {
                if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                {
                    Meas_Module.Sampling_index = 0;

                    if ( Global_Variables.VI_RAW_OP_CODE.bit.raw_data_4k_1k == 1 )
                    {
                        //
                        // 轉態
                        //
                        Meas_Module.Meas_sampling_state = sampling_state_1k;
                    }
                    else
                    {
                        //
                        // 轉態
                        //
                        Meas_Module.Meas_sampling_state = sampling_state_4k;

                    }

                    break;
                }

                break;
            }

            /*------------------------------------------------------*/
            case sampling_state_4k:
            {
                if ( Global_Variables.dds_shadow.addr != Global_Variables.dds.addr )
                {
                    #ifdef DEVELOP_SV69
                    //
                    // Debug模式
                    //

                    V_RAW.local_ram[Meas_Module.Sampling_index] = cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Result;

                    //I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHB;
                    //I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHA_sum_shadow2;
                    //I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHA_Dummy;
                    I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.Cla1Task1_Cycle_Meas_Result;

                    #else
                    //
                    // 正常模式
                    //
                    V_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHA;
                    I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHB;
                    #endif

                    Meas_Module.Sampling_index++;
                }

                if ( Meas_Module.Sampling_index == 4096 )
                {
                    Global_Variables.POWER_DISPLAY_VARIABLES.VI_Raw_Data_Counter = Meas_Module.Sampling_index;

                    //
                    // 轉態
                    //
                    Meas_Module.Meas_sampling_state = meas_sampling_ready_state;

                }
                break;
            }

            /*------------------------------------------------------*/
            case sampling_state_1k:
            {
                if ( Global_Variables.dds_shadow.addr_1k != Global_Variables.dds.addr_1k )
                {
                    #ifdef DEVELOP_SV69
                    //
                    // Debug模式
                    //
                    //V_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHA;
                    V_RAW.local_ram[Meas_Module.Sampling_index] = Global_Variables.INPUT_FLAGS.bit.ACFAIL;

                    //I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHB;
                    //I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHA_sum_shadow2;
                    //I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHA_Dummy;
                    //I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.Cla1Task1_Cycle_Meas_Result;
                    I_RAW.local_ram[Meas_Module.Sampling_index] = Global_Variables.INPUT_FLAGS.bit.ACFAIL_shadow3;


                    #else
                    //
                    // 正常模式
                    //
                    V_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHA;
                    I_RAW.local_ram[Meas_Module.Sampling_index] = CLA_to_CPU_Variables.ADS8353_CHB;
                    #endif


                    Meas_Module.Sampling_index++;
                }

                if ( Meas_Module.Sampling_index == 1024 )
                {
                    Global_Variables.POWER_DISPLAY_VARIABLES.VI_Raw_Data_Counter = Meas_Module.Sampling_index;

                    //
                    // 轉態
                    //
                    Meas_Module.Meas_sampling_state = meas_sampling_ready_state;

                }
                break;
            }

        }
        /*--------- switch ( Meas_Module.Meas_sampling_state ) ---------*/
    }

}



////////////////////////////////////////////////////////////////////
//
//
//
static inline SEQ_Mode_Output_State_Process(void)
{

    //
    // 輸出主中斷
    //
    switch(cpu_timer_module.cpu_timer0_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // Ready
        //
        case CPU_TIMER0_OUTPUT_READY_STATE:
        {
            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Ready
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_READY_STATE:
        {
            float inv_vmax;
            float dcv_set, acv_set, cf, freq_set;

            //
            // Freq set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ 0 ].all * 0.01;
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;

            //
            // DCV set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ 0 ].all * 0.0001;
            dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;

            //
            // ACV set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ 0 ].all * 0.0001;
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // Wave set user
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = SEQ_Step_Section1.SEQ_Config_State2_set_user[ 0 ].bit.WAVE_Form;



            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );

            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_out_on;


            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Step init
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE:
        {
            Uint16 run_step;
            Uint32 step_count;
            float  inv_step_count;
            float  begin, end;
            float  phase_set;

            float freq_set;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;


            //
            // 計算總step count
            //
            if ( SEQ_Step_Section3.SEQ_Step_Time_set_user[ run_step ].all == 0 )
            {
                step_count = 1;
            }
            else
            {
                step_count = (Uint32)( (float)SEQ_Step_Section3.SEQ_Step_Time_set_user[ run_step ].all * _100uSEC_DIV_SEQ_RUN_PERIOD );
            }

            inv_step_count = 1.0 / (float)step_count;

            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count = step_count;
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count = inv_step_count;


            //
            // 清掉累加值
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu = 0;
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu = 0;
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu = 0;

            //
            // 處理Freq
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.FREQ_Const )
            {
                //
                // Freq Const
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ run_step ].all * 0.01;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.FREQ_Sweep )
            {
                //
                // Freq Sweep
                //

                begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;
                end = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ run_step ].all * 0.01;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu = ( end - begin ) * inv_step_count;
            }


            //
            // 計算更新Freq Step
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );

            //
            // 處理 DCV
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.DCV_Const )
            {
                //
                // DCV Const
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ run_step ].all * 0.0001;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.DCV_Sweep )
            {
                //
                // DCV Sweep
                //

                begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;
                end = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ run_step ].all * 0.0001;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu = ( end - begin ) * inv_step_count;
            }

            //
            // 處理ACV
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.ACV_Const )
            {
                //
                // ACV Const
                //
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ run_step ].all * 0.0001;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.ACV_Sweep )
            {
                //
                // ACV Sweep
                //
                begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                end = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ run_step ].all * 0.0001;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu = ( end - begin ) * inv_step_count;
            }

            //
            // 處理Start Phase
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Start_Phase_Enable == 1 )
            {
                //
                // Start phase Enable
                //

                //
                // 計算Start phase count
                //
                phase_set = (float)SEQ_Step_Section1.SEQ_Start_Phase_set_user[ run_step ] * 0.1;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Start_Phase_Count = (Uint32)( phase_set * DEG_STEP );

                //
                // Reload dds
                //
                Global_Variables.dds.all = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Start_Phase_Count;
            }


            //
            // 載入波形Wave form set
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.WAVE_Form;

            //
            // 設定SYNC Code 0/1 => EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC0 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Sync_Code_0;
            Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC1 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Sync_Code_1;


            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_ON_STATE;

            //
            // 轉態
            //
            cpu_timer_module.SEQ_Output_ON_state = SEQ_ACDC_OUTPUT_ON_STATE0;


            //
            #ifdef DEVELOP_SV63
            Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 1;
            #endif

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Output ON
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            //
            // SEQ 輸出ON的排程
            //
            switch(cpu_timer_module.SEQ_Output_ON_state)
            {
                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 0
                //
                case SEQ_ACDC_OUTPUT_ON_STATE0:
                {
                    Uint16 run_step;

                    float inv_vmax;
                    float dcv_set, acv_set;
                    float cf;
                    float dcv_accu;

                    float begin, end;
                    float inv_step_count;

                    //
                    // 目前執行的Step編號
                    //
                    run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

                    //
                    // step count遞減
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count - 1;

                    //
                    // 計算inv step count
                    //
                    if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count == 0 )
                    {
                        //
                        // 維持不變
                        //
                        inv_step_count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count;
                    }
                    else
                    {
                        inv_step_count = 1.0 / (float)Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count = inv_step_count;
                    }


                    //
                    // 載入目前設定值
                    //
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                    dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


                    //
                    // DCV Sweep
                    //
                    if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.DCV_Sweep )
                    {
                        begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;
                        end = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ run_step ].all * 0.0001;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu = ( end - begin ) * inv_step_count;
                    }


                    //
                    // 處理DCV set 累加
                    //
                    dcv_accu = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu;
                    dcv_set = dcv_set + dcv_accu;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = dcv_set;


                    //
                    // 計算更新 Wave set goal
                    //
                    cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_ACDC_OUTPUT_ON_STATE1;

                    break;
                }



                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 1
                //
                case SEQ_ACDC_OUTPUT_ON_STATE1:
                {

                    Uint16 run_step;

                    float inv_vmax;
                    float dcv_set, acv_set;
                    float cf;
                    float acv_accu;

                    float begin, end;
                    float inv_step_count;

                    //
                    // SEQ 狀態更新
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;

                    //
                    // 目前執行的Step編號
                    //
                    run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

                    //
                    // inv step count
                    //
                    inv_step_count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count;


                    //
                    // 載入目前設定值
                    //
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                    dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


                    //
                    // ACV Sweep
                    //
                    if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.ACV_Sweep )
                    {
                        begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                        end = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ run_step ].all * 0.0001;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu = ( end - begin ) * inv_step_count;
                    }

                    //
                    // 處理ACV set 累加
                    //
                    acv_accu = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu;
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set + acv_accu;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = acv_set;


                    //
                    // 計算更新 Wave set goal
                    //
                    cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );


                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_ACDC_OUTPUT_ON_STATE2;

                    break;
                }



                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 2
                //
                case SEQ_ACDC_OUTPUT_ON_STATE2:
                {

                    Uint16 run_step;

                    float inv_vmax;
                    float dcv_set, acv_set, freq_set;
                    float cf;
                    float freq_accu;

                    float begin, end;
                    float inv_step_count;

                    //
                    // 目前執行的Step編號
                    //
                    run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

                    //
                    // inv step count
                    //
                    inv_step_count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count;


                    //
                    // 載入目前設定值
                    //
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                    dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;
                    freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;


                    //
                    // Freq Sweep
                    //
                    if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.FREQ_Sweep )
                    {
                        begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;
                        end = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ run_step ].all * 0.01;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu = ( end - begin ) * inv_step_count;
                    }

                    //
                    // 處理頻率freq set 累加
                    //
                    freq_accu = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu;
                    freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set + freq_accu;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = freq_set;


                    //
                    // 計算更新 Wave set goal
                    //
                    cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );


                    if ( Global_Variables.dds_shadow3.all > Global_Variables.dds.all ) //等待一個周期再更新
                    {
                        //
                        // 計算更新Freq Step
                        //
                        Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );
                    }


                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_ACDC_OUTPUT_ON_STATE3;

                    break;

                }
                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 3
                //
                case SEQ_ACDC_OUTPUT_ON_STATE3:
                {
                    float inv_vmax;
                    float dcv_set, acv_set;
                    float cf;

                    //
                    // SEQ 狀態更新
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;


                    //
                    // 載入目前設定值
                    //
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                    dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


                    //
                    // 計算更新 Wave set goal
                    //
                    cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );


                    //
                    // Step count < 0, 到達Step Count
                    //
                    if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count == 0 )
                    {
                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE;

                    }



                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_ACDC_OUTPUT_ON_STATE0;

                    break;
                }
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE:
        {
            Uint16 run_step;

            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

            //
            // Freq
            // 計時時間到且設定Sweep時, 讓設定值相等
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.FREQ_Sweep )
            {
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ run_step ].all * 0.01;
            }

            //
            // ACV
            // 計時時間到且設定Sweep時, 讓設定值相等
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.ACV_Sweep )
            {
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ run_step ].all * 0.0001;

            }

            //
            // DCV
            // 計時時間到且設定Sweep時, 讓設定值相等
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.DCV_Sweep )
            {
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ run_step ].all * 0.0001;

            }


            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Stop_Phase_Enable == 1 )
            {
                //
                // Stop phase Enable
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0;

            }
            else
            {
                //
                // Disable Stop phase
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2;

            }

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Hold
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_HOLD_STATE:
        {
            float inv_vmax;
            float dcv_set, acv_set, cf, freq_set;

            //
            // 處理DCV set
            //
            dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;

            //
            // 處理ACV set
            //
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // 處理頻率freq set
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;



            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );


            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_hold;

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 0
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0:
        {
            float inv_vmax;
            float dcv_set, acv_set, cf, freq_set;
            float phase_set;

            Uint16 run_step;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;


            //
            // 處理DCV set
            //
            dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;

            //
            // 處理ACV set
            //
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // 處理頻率freq set
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;


            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );

            //
            // 等待一個周期再轉態
            //
            if ( Global_Variables.dds_shadow3.all > Global_Variables.dds.all )
            {

                //
                // 計算Stop phase count
                //
                phase_set = (float)SEQ_Step_Section1.SEQ_Stop_Phase_set_user[ run_step ] * 0.1;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Stop_Phase_Count = (Uint32)( phase_set * DEG_STEP );

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1;

                break;

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 1
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1:
        {
            float inv_vmax;
            float dcv_set, acv_set, cf, freq_set;

            //
            // 處理DCV set
            //
            dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;

            //
            // 處理ACV set
            //
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // 處理頻率freq set
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;


            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );


            if ( Global_Variables.dds.all >= Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Stop_Phase_Count )
            {
                //
                // dds滿足OFF phase count
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2; //轉態

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 2
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2:
        {
            Uint16 run_step;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

            //
            #ifdef DEVELOP_SV63
            Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
            #endif


            if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step == 0 )
            {
                //
                // Run step = 0 回Ready
                //

                //
                // 觸發清空Jump Count事件
                //
                Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_Jump_Count = 1;

                //
                // 設定SYNC Code 0/1 => EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC0 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ 0 ].bit.Sync_Code_0;
                Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC1 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ 0 ].bit.Sync_Code_1;

                //
                // 轉態Ready前清掉SEQ控制旗標,避免誤動作
                //
                Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_READY_STATE;

                //
                #ifdef DEVELOP_SV63
                Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
                #endif

                break;
            }
            else if ( ( SEQ_Step_Section2.SEQ_Jump_Count_set_user[ run_step ] == 0 )
                      && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Jump_Enable == 1 ) )
            {
                //
                // Jump Count infinite
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Jump_set_user[ run_step ];

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Jump_Count[ run_step ] < SEQ_Step_Section2.SEQ_Jump_Count_set_user[ run_step ] )
                      && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Jump_Enable == 1 ) )
            {
                //
                // Jump Count還未達指定次數
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Jump_Count[ run_step ] ++;

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Jump_set_user[ run_step ];

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Step_Stop == 1 )
            {
                //
                // Term = Stop
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Step_Hold == 1 )
            {
                //
                // Term = Hold
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_END_HOLD_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Step_Continuous == 1 )
            {
                //
                // Term = Continuous
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step ++;

                if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step > 999 )
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;
                }

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Hold(End)
        //
        case SEQ_ACDC_CPU_TIMER0_OUTPUT_END_HOLD_STATE:
        {
            float inv_vmax;
            float dcv_set, acv_set, cf, freq_set;

            //
            // 處理DCV set
            //
            dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;

            //
            // 處理ACV set
            //
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // 處理頻率freq set
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;



            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );

            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_hold;


            break;

        }



        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Ready
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_READY_STATE:
        {
            float inv_vmax;
            float acv_set, cf, freq_set;

            //
            // Freq set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ 0 ].all * 0.01;
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;

            //
            // DCV set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = 0;

            //
            // ACV set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ 0 ].all * 0.0001;
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // Wave set user
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = SEQ_Step_Section1.SEQ_Config_State2_set_user[ 0 ].bit.WAVE_Form;



            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );

            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_out_on;


            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Step init
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE:
        {
            Uint16 run_step;
            Uint32 step_count;
            float  inv_step_count;
            float  begin, end;
            float  phase_set;

            float freq_set;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;


            //
            // 計算總step count
            //
            if ( SEQ_Step_Section3.SEQ_Step_Time_set_user[ run_step ].all == 0 )
            {
                step_count = 1;
            }
            else
            {
                step_count = (Uint32)( (float)SEQ_Step_Section3.SEQ_Step_Time_set_user[ run_step ].all * _100uSEC_DIV_SEQ_RUN_PERIOD );
            }

            inv_step_count = 1.0 / (float)step_count;

            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count = step_count;
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count = inv_step_count;


            //
            // 清掉累加值
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu = 0;
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu = 0;

            //
            // 處理Freq
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.FREQ_Const )
            {
                //
                // Freq Const
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ run_step ].all * 0.01;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.FREQ_Sweep )
            {
                //
                // Freq Sweep
                //

                begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;
                end = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ run_step ].all * 0.01;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu = ( end - begin ) * inv_step_count;
            }


            //
            // 計算更新Freq Step
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );


            //
            // 處理ACV
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.ACV_Const )
            {
                //
                // ACV Const
                //
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ run_step ].all * 0.0001;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.ACV_Sweep )
            {
                //
                // ACV Sweep
                //
                begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                end = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ run_step ].all * 0.0001;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu = ( end - begin ) * inv_step_count;
            }

            //
            // 處理Start Phase
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Start_Phase_Enable == 1 )
            {
                //
                // Start phase Enable
                //

                //
                // 計算Start phase count
                //
                phase_set = (float)SEQ_Step_Section1.SEQ_Start_Phase_set_user[ run_step ] * 0.1;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Start_Phase_Count = (Uint32)( phase_set * DEG_STEP );

                //
                // Reload dds
                //
                Global_Variables.dds.all = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Start_Phase_Count;
            }


            //
            // 載入波形Wave form set
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.WAVE_Form;

            //
            // 設定SYNC Code 0/1 => EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC0 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Sync_Code_0;
            Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC1 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Sync_Code_1;


            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_ON_STATE;

            //
            // 轉態
            //
            cpu_timer_module.SEQ_Output_ON_state = SEQ_AC_OUTPUT_ON_STATE0;


            //
            #ifdef DEVELOP_SV63
            Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 1;
            #endif

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Output ON
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            //
            // SEQ 輸出ON的排程
            //
            switch(cpu_timer_module.SEQ_Output_ON_state)
            {
                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 0
                //
                case SEQ_AC_OUTPUT_ON_STATE0:
                {

                    float inv_vmax;
                    float acv_set;
                    float cf;

                    float inv_step_count;


                    //
                    // step count遞減
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count - 1;

                    //
                    // 計算inv step count
                    //
                    if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count == 0 )
                    {
                        //
                        // 維持不變
                        //
                        inv_step_count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count;
                    }
                    else
                    {
                        inv_step_count = 1.0 / (float)Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count = inv_step_count;
                    }


                    //
                    // 載入目前設定值
                    //
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;



                    //
                    // 計算更新 Wave set goal
                    //
                    cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_AC_OUTPUT_ON_STATE1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 1
                //
                case SEQ_AC_OUTPUT_ON_STATE1:
                {

                    Uint16 run_step;

                    float inv_vmax;
                    float acv_set;
                    float cf;
                    float acv_accu;

                    float begin, end;
                    float inv_step_count;

                    //
                    // SEQ 狀態更新
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;

                    //
                    // 目前執行的Step編號
                    //
                    run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

                    //
                    // inv step count
                    //
                    inv_step_count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count;


                    //
                    // 載入目前設定值
                    //
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;


                    //
                    // ACV Sweep
                    //
                    if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.ACV_Sweep )
                    {
                        begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                        end = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ run_step ].all * 0.0001;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu = ( end - begin ) * inv_step_count;
                    }

                    //
                    // 處理ACV set 累加
                    //
                    acv_accu = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set_accu;
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set + acv_accu;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = acv_set;



                    //
                    // 計算更新 Wave set goal
                    //
                    cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );


                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_AC_OUTPUT_ON_STATE2;

                    break;
                }



                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 2
                //
                case SEQ_AC_OUTPUT_ON_STATE2:
                {

                    Uint16 run_step;

                    float inv_vmax;
                    float acv_set, freq_set;
                    float cf;
                    float freq_accu;

                    float begin, end;
                    float inv_step_count;

                    //
                    // 目前執行的Step編號
                    //
                    run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

                    //
                    // inv step count
                    //
                    inv_step_count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count;


                    //
                    // 載入目前設定值
                    //
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;
                    freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;


                    //
                    // Freq Sweep
                    //
                    if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.FREQ_Sweep )
                    {
                        begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;
                        end = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ run_step ].all * 0.01;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu = ( end - begin ) * inv_step_count;
                    }

                    //
                    // 處理頻率freq set 累加
                    //
                    freq_accu = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set_accu;
                    freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set + freq_accu;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = freq_set;


                    //
                    // 計算更新 Wave set goal
                    //
                    cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );


                    if ( Global_Variables.dds_shadow3.all > Global_Variables.dds.all ) //等待一個周期再更新
                    {
                        //
                        // 計算更新Freq Step
                        //
                        Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );
                    }


                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_AC_OUTPUT_ON_STATE3;

                    break;

                }
                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 3
                //
                case SEQ_AC_OUTPUT_ON_STATE3:
                {
                    float inv_vmax;
                    float acv_set;
                    float cf;

                    //
                    // SEQ 狀態更新
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;


                    //
                    // 載入目前設定值
                    //
                    acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;


                    //
                    // 計算更新 Wave set goal
                    //
                    cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );


                    //
                    // Step count < 0, 到達Step Count
                    //
                    if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count == 0 )
                    {
                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE;

                    }



                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_AC_OUTPUT_ON_STATE0;

                    break;
                }
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE:
        {
            Uint16 run_step;

            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;


            //
            // Freq
            // 計時時間到且設定Sweep時, 讓設定值相等
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.FREQ_Sweep )
            {
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = (float)SEQ_Step_Section3.SEQ_Freq_set_user[ run_step ].all * 0.01;
            }

            //
            // ACV
            // 計時時間到且設定Sweep時, 讓設定值相等
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.ACV_Sweep )
            {
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = (float)SEQ_Step_Section4.SEQ_ACV_set_user[ run_step ].all * 0.0001;

            }


            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Stop_Phase_Enable == 1 )
            {
                //
                // Stop phase Enable
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0;

            }
            else
            {
                //
                // Disable Stop phase
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2;

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Hold
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_HOLD_STATE:
        {
            float inv_vmax;
            float acv_set, cf, freq_set;

            //
            // 處理ACV set
            //
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // 處理頻率freq set
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;



            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );


            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_hold;

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 0
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0:
        {
            float inv_vmax;
            float acv_set, cf, freq_set;
            float phase_set;

            Uint16 run_step;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;


            //
            // 處理ACV set
            //
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // 處理頻率freq set
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;


            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );

            //
            // 等待一個周期再轉態
            //
            if ( Global_Variables.dds_shadow3.all > Global_Variables.dds.all )
            {

                //
                // 計算Stop phase count
                //
                phase_set = (float)SEQ_Step_Section1.SEQ_Stop_Phase_set_user[ run_step ] * 0.1;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Stop_Phase_Count = (Uint32)( phase_set * DEG_STEP );

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1;

                break;

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 1
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1:
        {
            float inv_vmax;
            float acv_set, cf, freq_set;


            //
            // 處理ACV set
            //
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // 處理頻率freq set
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;


            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );


            if ( Global_Variables.dds.all >= Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Stop_Phase_Count )
            {
                //
                // dds滿足OFF phase count
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2; //轉態

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 2
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2:
        {
            Uint16 run_step;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

            //
            #ifdef DEVELOP_SV63
            Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
            #endif


            if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step == 0 )
            {
                //
                // Run step = 0 回Ready
                //

                //
                // 觸發清空Jump Count事件
                //
                Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_Jump_Count = 1;

                //
                // 設定SYNC Code 0/1 => EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC0 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ 0 ].bit.Sync_Code_0;
                Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC1 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ 0 ].bit.Sync_Code_1;

                //
                // 轉態Ready前清掉SEQ控制旗標,避免誤動作
                //
                Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_READY_STATE;

                //
                #ifdef DEVELOP_SV63
                Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
                #endif

                break;
            }
            else if ( ( SEQ_Step_Section2.SEQ_Jump_Count_set_user[ run_step ] == 0 )
                      && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Jump_Enable == 1 ) )
            {
                //
                // Jump Count infinite
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Jump_set_user[ run_step ];

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Jump_Count[ run_step ] < SEQ_Step_Section2.SEQ_Jump_Count_set_user[ run_step ] )
                      && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Jump_Enable == 1 ) )
            {
                //
                // Jump Count還未達指定次數
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Jump_Count[ run_step ] ++;

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Jump_set_user[ run_step ];

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Step_Stop == 1 )
            {
                //
                // Term = Stop
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Step_Hold == 1 )
            {
                //
                // Term = Hold
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_END_HOLD_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Step_Continuous == 1 )
            {
                //
                // Term = Continuous
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step ++;

                if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step > 999 )
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;
                }

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Hold(End)
        //
        case SEQ_AC_CPU_TIMER0_OUTPUT_END_HOLD_STATE:
        {
            float inv_vmax;
            float acv_set, cf, freq_set;


            //
            // 處理ACV set
            //
            acv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set;

            //
            // 處理頻率freq set
            //
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;


            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * cpu_timer_module.wave_value * inv_vmax );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );

            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_hold;


            break;

        }




        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Ready
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_READY_STATE:
        {
            float inv_vmax;
            float dcv_set, freq_set;

            //
            // Freq set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set = 1000.0;
            freq_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Freq_Set;

            //
            // ACV set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_ACV_Set = 0;

            //
            // DCV set
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ 0 ].all * 0.0001;
            dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


            //
            // 計算更新 Wave set goal
            //
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 );

            //
            // 計算更新Freq Step
            //
            Global_Variables.FreqStep = (Uint32)( freq_set * _1HZ );

            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_out_on;


            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Step init
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE:
        {
            Uint16 run_step;
            Uint32 step_count;
            float  inv_step_count;
            float  begin, end;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;


            //
            // 計算總step count
            //
            if ( SEQ_Step_Section3.SEQ_Step_Time_set_user[ run_step ].all == 0 )
            {
                step_count = 1;
            }
            else
            {
                step_count = (Uint32)( (float)SEQ_Step_Section3.SEQ_Step_Time_set_user[ run_step ].all * _100uSEC_DIV_SEQ_RUN_PERIOD );
            }

            inv_step_count = 1.0 / (float)step_count;

            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count = step_count;
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count = inv_step_count;


            //
            // 清掉累加值
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu = 0;




            //
            // 處理 DCV
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.DCV_Const )
            {
                //
                // DCV Const
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ run_step ].all * 0.0001;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.DCV_Sweep )
            {
                //
                // DCV Sweep
                //

                begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;
                end = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ run_step ].all * 0.0001;
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu = ( end - begin ) * inv_step_count;
            }


            //
            // 設定SYNC Code 0/1 => EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC0 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Sync_Code_0;
            Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC1 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Sync_Code_1;


            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_ON_STATE;

            //
            // 轉態
            //
            cpu_timer_module.SEQ_Output_ON_state = SEQ_DC_OUTPUT_ON_STATE0;


            //
            #ifdef DEVELOP_SV63
            Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 1;
            #endif

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Output ON
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            //
            // SEQ 輸出ON的排程
            //
            switch(cpu_timer_module.SEQ_Output_ON_state)
            {
                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 0
                //
                case SEQ_DC_OUTPUT_ON_STATE0:
                {
                    Uint16 run_step;

                    float inv_vmax;
                    float dcv_set;
                    float dcv_accu;

                    float begin, end;
                    float inv_step_count;

                    //
                    // 目前執行的Step編號
                    //
                    run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

                    //
                    // step count遞減
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count - 1;

                    //
                    // 計算inv step count
                    //
                    if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count == 0 )
                    {
                        //
                        // 維持不變
                        //
                        inv_step_count = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count;
                    }
                    else
                    {
                        inv_step_count = 1.0 / (float)Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_inv_Step_Count = inv_step_count;
                    }


                    //
                    // 載入目前設定值
                    //
                    dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


                    //
                    // DCV Sweep
                    //
                    if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.DCV_Sweep )
                    {
                        begin = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;
                        end = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ run_step ].all * 0.0001;
                        Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu = ( end - begin ) * inv_step_count;
                    }


                    //
                    // 處理DCV set 累加
                    //
                    dcv_accu = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set_accu;
                    dcv_set = dcv_set + dcv_accu;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = dcv_set;




                    //
                    // 計算更新 Wave set goal
                    //
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 );

                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_DC_OUTPUT_ON_STATE1;

                    break;
                }



                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 1
                //
                case SEQ_DC_OUTPUT_ON_STATE1:
                {

                    float inv_vmax;
                    float dcv_set;


                    //
                    // SEQ 狀態更新
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;


                    //
                    // 載入目前設定值
                    //
                    dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


                    //
                    // 計算更新 Wave set goal
                    //
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 );


                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_DC_OUTPUT_ON_STATE2;

                    break;
                }



                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 2
                //
                case SEQ_DC_OUTPUT_ON_STATE2:
                {

                    float inv_vmax;
                    float dcv_set;


                    //
                    // SEQ 狀態更新
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;


                    //
                    // 載入目前設定值
                    //
                    dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


                    //
                    // 計算更新 Wave set goal
                    //
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 );


                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_DC_OUTPUT_ON_STATE3;

                    break;

                }
                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Output ON - 3
                //
                case SEQ_DC_OUTPUT_ON_STATE3:
                {

                    float inv_vmax;
                    float dcv_set;

                    //
                    // SEQ 狀態更新
                    //
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;


                    //
                    // 載入目前設定值
                    //
                    dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;



                    //
                    // 計算更新 Wave set goal
                    //
                    inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 );


                    //
                    // Step count < 0, 到達Step Count
                    //
                    if ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Step_Count == 0 )
                    {
                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE;

                    }


                    //
                    // 轉態
                    //
                    cpu_timer_module.SEQ_Output_ON_state = SEQ_DC_OUTPUT_ON_STATE0;

                    break;
                }
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE:
        {
            Uint16 run_step;

            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_run;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

            //
            // DCV
            // 計時時間到且設定Sweep時, 讓設定值相等
            //
            if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.DCV_Sweep )
            {
                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set = (float)SEQ_Step_Section4.SEQ_DCV_set_user[ run_step ].all * 0.0001;

            }

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Hold
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_HOLD_STATE:
        {
            float inv_vmax;
            float dcv_set;

            //
            // 處理DCV set
            //
            dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


            //
            // 計算更新 Wave set goal
            //
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 );


            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_hold;

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 0
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0:
        {


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 1
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1:
        {

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Stop phase 2
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2:
        {
            Uint16 run_step;

            //
            // 目前執行的Step編號
            //
            run_step = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;

            //
            #ifdef DEVELOP_SV63
            Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
            #endif


            if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step == 0 )
            {
                //
                // Run step = 0 回Ready
                //

                //
                // 觸發清空Jump Count事件
                //
                Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_Jump_Count = 1;

                //
                // 設定SYNC Code 0/1 => EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC0 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ 0 ].bit.Sync_Code_0;
                Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC1 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ 0 ].bit.Sync_Code_1;

                //
                // 轉態Ready前清掉SEQ控制旗標,避免誤動作
                //
                Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.all = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_READY_STATE;

                //
                #ifdef DEVELOP_SV63
                Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
                #endif

                break;
            }
            else if ( ( SEQ_Step_Section2.SEQ_Jump_Count_set_user[ run_step ] == 0 )
                      && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Jump_Enable == 1 ) )
            {
                //
                // Jump Count infinite
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Jump_set_user[ run_step ];

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( ( Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Jump_Count[ run_step ] < SEQ_Step_Section2.SEQ_Jump_Count_set_user[ run_step ] )
                      && ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Jump_Enable == 1 ) )
            {
                //
                // Jump Count還未達指定次數
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.Current_Jump_Count[ run_step ] ++;

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = SEQ_Step_Section2.SEQ_Jump_set_user[ run_step ];

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State2_set_user[ run_step ].bit.Step_Stop == 1 )
            {
                //
                // Term = Stop
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Step_Hold == 1 )
            {
                //
                // Term = Hold
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_END_HOLD_STATE;

                break;
            }
            else if ( SEQ_Step_Section1.SEQ_Config_State1_set_user[ run_step ].bit.Step_Continuous == 1 )
            {
                //
                // Term = Continuous
                //

                Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step ++;

                if ( Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step > 999 )
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step = 0;
                }

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE;

                break;
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // SEQ Hold(End)
        //
        case SEQ_DC_CPU_TIMER0_OUTPUT_END_HOLD_STATE:
        {
            float inv_vmax;
            float dcv_set;

            //
            // 處理DCV set
            //
            dcv_set = Global_Variables.SEQ_FUNCTION_VARIABLES.Current_DCV_Set;


            //
            // 計算更新 Wave set goal
            //
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( -dcv_set * inv_vmax + 1.0 ) * 32768.0 );


            //
            // SEQ 狀態更新
            //
            Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_hold;


            break;

        }


//seq
        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 0
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            //
            // 量測補償OFF
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

            //
            // 判斷是否降到0
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow ) <= 32.767 )
            {
                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE1;
                }
                else
                {
                    //
                    // 機器沒有輸出, 或有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;
                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 1
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            //
            // 逆電流卸載PID 初始化
            //

            //
            // 初始化補償器
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 2
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE2:
        {
            //
            // 逆電流卸載PID Loop
            //

            //
            // 補償計算
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB;

            // Compute the error
            // 計算Err
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref - PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb;

            // Compute the proportional output
            // 計算比例Up
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err;

            // Compute the integral output
            // 計算積分Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui + ( PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up );

            // Compute the pre-saturated output
            // 計算Up+Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up + PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui;

            // Saturate the output
            if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat > PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax;
            }
            else if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat < PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin;
            }
            else
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat;
            }

            //
            // Update到CLA模組去補償WAVE DAC
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset2 = -PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 3
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE3:
        {
            float vref_zero;
            float step;

            vref_zero = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            step = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step;

            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - vref_zero ) <= step )
            {
                //
                // 靠近 vref_zero
                //

                //
                // DAC降到zero
                //
                Init_VREF_WAVE_Process();

                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE4;
                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;
                }
            }
            else
            {
                //
                // 每次遞減
                //

                Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - step );
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 4
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE4:
        {
            //
            // 等待被觸發轉態
            //

            break;
        }

    }
    /*--------- switch ( cpu_timer_module.cpu_timer0_state ) ---------*/

    Global_Variables.dds_shadow3.all = Global_Variables.dds.all;

}



////////////////////////////////////////////////////////////////////
//
//
//
static inline SYNC_Mode_Output_State_Process(void)
{
    Uint16 wave_addr, wave_addr_1;
    float wave_c2, wave_c2_1;
    float wave_value;


    wave_addr = (Uint16)Global_Variables.dds.addr;
    wave_addr_1 = ( (Uint16)Global_Variables.dds.addr + 1 ) & 0x0FFF;

    cpu_timer_module.wave_c2_decimal = (float)Global_Variables.dds.dec1 * 0.003921569;


    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != WAVE_SQU )
    {
        //
        // 從WAVE buffer載入WAVE
        //
        wave_c2 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr ];
        wave_c2_1 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr_1 ];

        //
        // 內差法計算wave value
        //
        wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;
    }
    else if ( Global_Variables.dds.addr <= Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = 32767.0;
    }
    else if ( Global_Variables.dds.addr > Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = -32767.0;
    }


    //
    // 輸出主中斷
    //
    switch(cpu_timer_module.cpu_timer0_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // Ready
        //
        case CPU_TIMER0_OUTPUT_READY_STATE:
        {
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-SYNC
        // ON_Phase
        //
        case ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE0:
        {
            if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
            {
                //
                // 等周期的開始
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-SYNC
        // ON_Phase
        //
        case ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE1:
        {
            //
            // 等待同步到 Start phase Count
            //
            if ( Global_Variables.dds.all >= Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow )
            {
                if ( Memory_Module.wave_buffer_section == 0 )
                {
                    //
                    // SIN, Slew Rate => OFF
                    //
                    cpu_timer_module.slew_rate_state = NO_SLEWRATE_STATE;
                }
                else
                {
                    //
                    // Slew Rate => ON
                    //
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;
                }


                //
                // SYNC模式下
                // 等待相角到達Start phase count shadow再去開VREF DAC
                //
                Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

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
                // 設定EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_ON_STATE;

                break;
            }



            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-SYNC
        // Output_ON
        //
        case ACDC_SYNC_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );


            //
            // 停止輸出處理
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {
                cpu_timer_module.Value_Set_User_Change = 0;


                if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 0 )
                {
                    //
                    // OFF Phase Disable
                    //

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

                    break;
                }
                else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 1 )
                {
                    //
                    // OFF Phase Enable
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0; //轉態

                    break;
                }
            }

            //
            // 改變設定值處理
            //
            if ( cpu_timer_module.Value_Set_User_Change == 1 )
            {
                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0; //轉態

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-SYNC
        // Change_Phase 0
        //
        case ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
            {
                //
                // 等周期的開始
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1;
            }

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-SYNC
        // Change_Phase 0
        //
        case ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1:
        {
            float scale, offset;
            //Uint32 sync_change_phase;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );


            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Zero_Change_Phase_Enable == 1 )
            {
                //
                // Zero Change Enable
                //

                if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                {
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                    //
                    // Reset WAVE以及VREF補償量
                    //
                    Output_Control_Module.WAVE_Compensate_Offset = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                    break;
                }
            }
            else
            {
                //
                // Zero Change Disable
                //

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                //
                // Reset WAVE以及VREF補償量
                //
                Output_Control_Module.WAVE_Compensate_Offset = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-SYNC
        // Change_Phase 2
        //
        case ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2:
        {
            cpu_timer_module.Value_Set_User_Change = 0;

            //
            // 載入wave_buffer_section, 改變輸出波形
            //
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;

            //
            // 更新WAVE參數
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;

            Global_Variables.OUTPUT_VARIABLES.WAVE_offset = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;
            Global_Variables.OUTPUT_VARIABLES.WAVE_scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow;


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
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_ON_STATE;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-SYNC
        // OFF_Phase
        //
        case ACDC_SYNC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
            {
                //
                // 等周期的開始
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_SYNC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC+DC-SYNC
        // Stop_Phase
        //
        case ACDC_SYNC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            //
            // 正常停止
            //
            if ( Global_Variables.dds.all >= Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_phase_count )
            {
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

                break;

            }


            break;
        }
//sync
        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 0
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            //
            // 量測補償OFF
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

            //
            // 判斷是否降到0
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow ) <= 32.767 )
            {
                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE1;
                }
                else
                {
                    //
                    // 機器沒有輸出, 或有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;
                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 1
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            //
            // 逆電流卸載PID 初始化
            //

            //
            // 初始化補償器
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 2
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE2:
        {
            //
            // 逆電流卸載PID Loop
            //

            //
            // 補償計算
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB;

            // Compute the error
            // 計算Err
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref - PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb;

            // Compute the proportional output
            // 計算比例Up
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err;

            // Compute the integral output
            // 計算積分Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui + ( PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up );

            // Compute the pre-saturated output
            // 計算Up+Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up + PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui;

            // Saturate the output
            if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat > PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax;
            }
            else if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat < PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin;
            }
            else
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat;
            }

            //
            // Update到CLA模組去補償WAVE DAC
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset2 = -PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 3
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE3:
        {
            float vref_zero;
            float step;

            vref_zero = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            step = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step;

            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - vref_zero ) <= step )
            {
                //
                // 靠近 vref_zero
                //

                //
                // DAC降到zero
                //
                Init_VREF_WAVE_Process();

                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE4;
                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;
                }
            }
            else
            {
                //
                // 每次遞減
                //

                Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - step );
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 4
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE4:
        {
            //
            // 等待被觸發轉態
            //

            break;
        }
    }
    /*--------- switch(cpu_timer_module.cpu_timer0_state) ---------*/

}


////////////////////////////////////////////////////////////////////
//
//
//
static inline EXT_Mode_Output_State_Process(void)
{
    //
    // 輸出主中斷
    //
    switch(cpu_timer_module.cpu_timer0_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // Ready
        //
        case CPU_TIMER0_OUTPUT_READY_STATE:
        {
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ACDC-EXT
        // Output ON
        //
        case ACDC_EXT_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            float ex_adc_value;
            float ex_gain;
            float inv_ext_scale;
            float ext_offset;
            float inv_vmax;
            float k1;


            ex_gain = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.all * 0.0001;
            inv_ext_scale = Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale;
            ext_offset = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;

            k1 = -ex_gain * 32768.0 * inv_ext_scale * inv_vmax;

            ex_adc_value = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN - ( (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level - ext_offset );


            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( ( ex_adc_value - ext_offset ) * k1 + 32768.0 );


            //
            // 停止輸出處理
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE0;

                Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = WAVE_DAC_ZERO;

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                break;
            }

            break;
        }

//ext
        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 0
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            //
            // 量測補償OFF
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

            //
            // 判斷是否降到0
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow ) <= 32.767 )
            {
                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE1;
                }
                else
                {
                    //
                    // 機器沒有輸出, 或有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;
                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 1
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            //
            // 逆電流卸載PID 初始化
            //

            //
            // 初始化補償器
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 2
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE2:
        {
            //
            // 逆電流卸載PID Loop
            //

            //
            // 補償計算
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB;

            // Compute the error
            // 計算Err
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref - PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb;

            // Compute the proportional output
            // 計算比例Up
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err;

            // Compute the integral output
            // 計算積分Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui + ( PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up );

            // Compute the pre-saturated output
            // 計算Up+Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up + PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui;

            // Saturate the output
            if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat > PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax;
            }
            else if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat < PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin;
            }
            else
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat;
            }

            //
            // Update到CLA模組去補償WAVE DAC
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset2 = -PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 3
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE3:
        {
            float vref_zero;
            float step;

            vref_zero = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            step = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step;

            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - vref_zero ) <= step )
            {
                //
                // 靠近 vref_zero
                //

                //
                // DAC降到zero
                //
                Init_VREF_WAVE_Process();

                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE4;
                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;
                }
            }
            else
            {
                //
                // 每次遞減
                //

                Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - step );
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 4
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE4:
        {
            //
            // 等待被觸發轉態
            //

            break;
        }

    }
    /*--------- switch(cpu_timer_module.cpu_timer0_state) ---------*/


}


////////////////////////////////////////////////////////////////////
//
//
//
static inline ADD_Mode_Output_State_Process(void)
{
    Uint16 wave_addr, wave_addr_1;
    float wave_c2, wave_c2_1;
    float wave_value;


    wave_addr = (Uint16)Global_Variables.dds.addr;
    wave_addr_1 = ( (Uint16)Global_Variables.dds.addr + 1 ) & 0x0FFF;

    cpu_timer_module.wave_c2_decimal = (float)Global_Variables.dds.dec1 * 0.003921569;


    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != WAVE_SQU )
    {
        //
        // 從WAVE buffer載入WAVE
        //
        wave_c2 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr ];
        wave_c2_1 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr_1 ];

        //
        // 內差法計算wave value
        //
        wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;
    }
    else if ( Global_Variables.dds.addr <= Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = 32767.0;
    }
    else if ( Global_Variables.dds.addr > Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = -32767.0;
    }


    //
    // 輸出主中斷
    //
    switch(cpu_timer_module.cpu_timer0_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // Ready
        //
        case CPU_TIMER0_OUTPUT_READY_STATE:
        {
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ACDC-ADD
        // Start Phase 0
        //
        case ACDC_ADD_CPU_TIMER0_OUTPUT_START_PHASE_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            // ADD的Start phase的WAVE_set_goal不考慮ext訊號
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            //
            // 設定EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = ACDC_ADD_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;


            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ACDC-ADD
        // Start Phase 1
        //
        case ACDC_ADD_CPU_TIMER0_OUTPUT_START_PHASE_STATE1:
        {
            //
            // 快到達目標值
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow )
                    <= Global_Variables.OUTPUT_VARIABLES.WAVE_set_diff_level )
            {
                if ( Memory_Module.wave_buffer_section == 0 )
                {
                    //
                    // SIN, Slew Rate OFF
                    //
                    cpu_timer_module.slew_rate_state = NO_SLEWRATE_STATE;
                }
                else
                {
                    //
                    // 其他波形, Slew Rate ON
                    //
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;
                }

                //
                // 載入start phase到dds
                //
                Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_ADD_CPU_TIMER0_OUTPUT_ON_STATE;

                break;
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ACDC-ADD
        // Output ON
        //
        case ACDC_ADD_CPU_TIMER0_OUTPUT_ON_STATE:
        {

            float scale, offset;

            float ex_adc_value;
            float ex_gain;
            float inv_ext_scale;
            float ext_offset;
            float inv_vmax;
            float k1;

            float wave_int, wave_ext;

            //
            // 處理Ext WAVE
            //
            ex_gain = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.all * 0.0001;
            inv_ext_scale = Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale;
            ext_offset = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            k1 = -ex_gain * 32768.0 * inv_ext_scale * inv_vmax;
            ex_adc_value = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN - ( (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level - ext_offset );
            wave_ext = ( ex_adc_value - ext_offset ) * k1;


            //
            // 處理INT WAVE
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            wave_int = wave_value * scale + offset;

            //
            // 加總
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_int + wave_ext );


            //
            // 停止輸出處理
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {
                cpu_timer_module.Value_Set_User_Change = 0;

                //
                // 沒有截止相角Enable
                //
                if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 0 )
                {
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

                    break;
                }
                //
                // 有截止相角Enable
                //
                else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 1 )
                {
                    //
                    // OFF Phase Enable
                    //

                    //
                    // 等待滿一個週期再轉態
                    //
                    if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                    {

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = ACDC_ADD_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE;

                        break;
                    }
                }
            }

            //
            // 改變設定值處理
            //
            if ( cpu_timer_module.Value_Set_User_Change == 1 )
            {
                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0;

                break;
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ACDC-ADD
        // Change Phase 0
        //
        case ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0:
        {
            float scale, offset;

            float ex_adc_value;
            float ex_gain;
            float inv_ext_scale;
            float ext_offset;
            float inv_vmax;
            float k1;

            float wave_int, wave_ext;

            //
            // 處理Ext WAVE
            //
            ex_gain = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.all * 0.0001;
            inv_ext_scale = Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale;
            ext_offset = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            k1 = -ex_gain * 32768.0 * inv_ext_scale * inv_vmax;
            ex_adc_value = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN - ( (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level - ext_offset );
            wave_ext = ( ex_adc_value - ext_offset ) * k1;


            //
            // 處理INT WAVE
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            wave_int = wave_value * scale + offset;

            //
            // 加總
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_int + wave_ext );

            //
            // 判斷是否0度改變
            //
            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Zero_Change_Phase_Enable == 1 )
            {
                //
                // Zero_Change_Phase_Enable == 1
                //

                if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                    break;
                }
            }
            else
            {
                //
                // Zero_Change_Phase_Enable == 0
                //

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                break;
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        //
        // ACDC-ADD
        // Change Phase 1
        //
        /*
        case ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1:
        {


            break;
        }
        */
        /*-----------------------------------------------------------------------------------------*/
        //
        // ACDC-ADD
        // Change Phase 2
        //
        case ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2:
        {
            cpu_timer_module.Value_Set_User_Change = 0;

            //
            // 載入頻率Step
            //
            Global_Variables.FreqStep = Global_Variables.FreqStep_shadow;

            //
            // 載入wave_buffer_section, 改變輸出波形
            //
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;

            //
            // 更新WAVE
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;

            Global_Variables.OUTPUT_VARIABLES.WAVE_offset = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;
            Global_Variables.OUTPUT_VARIABLES.WAVE_scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow;

            Global_Variables.OUTPUT_VARIABLES.ext_WAVE_scale = Global_Variables.OUTPUT_VARIABLES.ext_WAVE_scale_shadow;

            //
            // 更新VREF
            //
            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

            //
            // 設定值改變後的起始相角
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;

            //
            // Reload dds
            //
            Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = ACDC_ADD_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;

            break;


        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ACDC-ADD
        // Stop Phase
        //
        case ACDC_ADD_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE:
        {
            float scale, offset;

            float ex_adc_value;
            float ex_gain;
            float inv_ext_scale;
            float ext_offset;
            float inv_vmax;
            float k1;

            float wave_int, wave_ext;

            Uint32 stop_phase_count_shadow;

            //
            // 處理Ext WAVE
            //
            ex_gain = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.all * 0.0001;
            inv_ext_scale = Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale;
            ext_offset = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            k1 = -ex_gain * 32768.0 * inv_ext_scale * inv_vmax;
            ex_adc_value = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN - ( (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level - ext_offset );
            wave_ext = ( ex_adc_value - ext_offset ) * k1;


            //
            // 處理INT WAVE
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            wave_int = wave_value * scale + offset;

            //
            // 加總
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_int + wave_ext );

            stop_phase_count_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_phase_count;

            //
            // 正常停止
            //
            if ( Global_Variables.dds.all >= stop_phase_count_shadow )
            {
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

                break;
            }

            break;
        }

//add
        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 0
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            //
            // 量測補償OFF
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

            //
            // 判斷是否降到0
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow ) <= 32.767 )
            {
                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE1;
                }
                else
                {
                    //
                    // 機器沒有輸出, 或有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;
                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 1
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            //
            // 逆電流卸載PID 初始化
            //

            //
            // 初始化補償器
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 2
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE2:
        {
            //
            // 逆電流卸載PID Loop
            //

            //
            // 補償計算
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB;

            // Compute the error
            // 計算Err
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref - PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb;

            // Compute the proportional output
            // 計算比例Up
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err;

            // Compute the integral output
            // 計算積分Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui + ( PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up );

            // Compute the pre-saturated output
            // 計算Up+Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up + PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui;

            // Saturate the output
            if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat > PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax;
            }
            else if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat < PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin;
            }
            else
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat;
            }

            //
            // Update到CLA模組去補償WAVE DAC
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset2 = -PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 3
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE3:
        {
            float vref_zero;
            float step;

            vref_zero = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            step = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step;

            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - vref_zero ) <= step )
            {
                //
                // 靠近 vref_zero
                //

                //
                // DAC降到zero
                //
                Init_VREF_WAVE_Process();

                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE4;
                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;
                }
            }
            else
            {
                //
                // 每次遞減
                //

                Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - step );
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 4
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE4:
        {
            //
            // 等待被觸發轉態
            //

            break;
        }
    }
    /*--------- switch(cpu_timer_module.cpu_timer0_state) ---------*/

}



////////////////////////////////////////////////////////////////////
//
//
//
static inline VCA_Mode_Output_State_Process(void)
{
    Uint16 wave_addr, wave_addr_1;
    float wave_c2, wave_c2_1;
    float wave_value;


    wave_addr = (Uint16)Global_Variables.dds.addr;
    wave_addr_1 = ( (Uint16)Global_Variables.dds.addr + 1 ) & 0x0FFF;

    cpu_timer_module.wave_c2_decimal = (float)Global_Variables.dds.dec1 * 0.003921569;



    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != WAVE_SQU )
    {
        //
        // 從WAVE buffer載入WAVE
        //
        wave_c2 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr ];
        wave_c2_1 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr_1 ];

        //
        // 內差法計算wave value
        //
        wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;
    }
    else if ( Global_Variables.dds.addr <= Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = 32767.0;
    }
    else if ( Global_Variables.dds.addr > Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = -32767.0;
    }

    //
    // 輸出主中斷
    //
    switch(cpu_timer_module.cpu_timer0_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // Ready
        //
        case CPU_TIMER0_OUTPUT_READY_STATE:
        {
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC-VCA
        // ON_Phase 0
        //
        case AC_VCA_CPU_TIMER0_OUTPUT_START_PHASE_STATE0:
        {
            float inv_vmax;
            float acv_set, cf;


            acv_set = Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set;

            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * wave_value * inv_vmax );

            //
            // 設定EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = AC_VCA_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;

            break;


        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC-VCA
        // ON_Phase 1
        //
        case AC_VCA_CPU_TIMER0_OUTPUT_START_PHASE_STATE1:
        {
            //
            // 快到達目標值
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow )
                    <= Global_Variables.OUTPUT_VARIABLES.WAVE_set_diff_level )
            {
                //
                // 載入start phase到dds
                //
                Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;


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
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = AC_VCA_CPU_TIMER0_OUTPUT_ON_STATE;

                break;
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC-VCA
        // Output_ON
        //
        case AC_VCA_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            float inv_vmax;
            float acv_set, cf;


            acv_set = Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set;


            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * wave_value * inv_vmax );


            //
            // 停止輸出處理
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {
                cpu_timer_module.Value_Set_User_Change = 0;


                if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 0 )
                {
                    //
                    // OFF Phase Disable
                    //

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

                    break;
                }
                else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 1 )
                {
                    //
                    // OFF Phase Enable
                    //

                    //
                    // 等待滿一個週期再轉態
                    //
                    if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                    {

                        //
                        // 轉態
                        //
                        cpu_timer_module.cpu_timer0_state = AC_VCA_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE; //轉態

                        break;
                    }
                }
            }

            //
            // 改變設定值處理
            //
            if ( cpu_timer_module.Value_Set_User_Change == 1 )
            {
                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = AC_VCA_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0;

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC-VCA
        // Change_Phase 0
        //
        case AC_VCA_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0:
        {
            float inv_vmax;
            float acv_set, cf;


            acv_set = Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set;


            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * wave_value * inv_vmax );


            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Zero_Change_Phase_Enable == 1 )
            {
                //
                // Zero Change Enable
                //

                if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                {
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                    //
                    // Reset WAVE以及VREF補償量
                    //
                    Output_Control_Module.WAVE_Compensate_Offset = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = AC_VCA_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1;

                    break;
                }

            }
            else
            {
                //
                // Zero Change Disable
                //

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                //
                // Reset WAVE以及VREF補償量
                //
                Output_Control_Module.WAVE_Compensate_Offset = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = AC_VCA_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1;

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // AC-VCA
        // Change_Phase 1
        //
        case AC_VCA_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1:
        {
            cpu_timer_module.Value_Set_User_Change = 0;

            //
            // 載入頻率Step
            //
            Global_Variables.FreqStep = Global_Variables.FreqStep_shadow;

            //
            // 載入wave_buffer_section, 改變輸出波形
            //
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;

            //
            // 設定值改變後的起始相角
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;

            //
            // Reload dds
            //
            Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;


            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = AC_VCA_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;

            Output_Control_Module.DCV_Compensate_State = DCV_NO_COMPENSATE;
            Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

            //
            // Reset WAVE以及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

            break;


        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // AC-VCA
        // Stop_Phase
        //
        case AC_VCA_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE:
        {
            Uint32 stop_phase_count_shadow;

            float inv_vmax;
            float acv_set, cf;

            acv_set = Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set;


            //
            // 計算更新 Wave set goal
            //
            cf = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set[ Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ];
            inv_vmax = Global_Variables.Pre_Calc_Variables.inv_Vmax;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( 32768.0 - acv_set * cf * wave_value * inv_vmax );


            stop_phase_count_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_phase_count;


            //
            // 正常停止
            //
            if ( Global_Variables.dds.all >= stop_phase_count_shadow )
            {
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

                break;

            }

            break;
        }

//vca
        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 0
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            //
            // 量測補償OFF
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

            //
            // 判斷是否降到0
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow ) <= 32.767 )
            {
                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE1;
                }
                else
                {
                    //
                    // 機器沒有輸出, 或有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;
                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 1
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            //
            // 逆電流卸載PID 初始化
            //

            //
            // 初始化補償器
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 2
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE2:
        {
            //
            // 逆電流卸載PID Loop
            //

            //
            // 補償計算
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB;

            // Compute the error
            // 計算Err
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref - PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb;

            // Compute the proportional output
            // 計算比例Up
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err;

            // Compute the integral output
            // 計算積分Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui + ( PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up );

            // Compute the pre-saturated output
            // 計算Up+Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up + PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui;

            // Saturate the output
            if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat > PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax;
            }
            else if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat < PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin;
            }
            else
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat;
            }

            //
            // Update到CLA模組去補償WAVE DAC
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset2 = -PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 3
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE3:
        {
            float vref_zero;
            float step;

            vref_zero = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            step = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step;

            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - vref_zero ) <= step )
            {
                //
                // 靠近 vref_zero
                //

                //
                // DAC降到zero
                //
                Init_VREF_WAVE_Process();

                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE4;
                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;
                }
            }
            else
            {
                //
                // 每次遞減
                //

                Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - step );
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 4
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE4:
        {
            //
            // 等待被觸發轉態
            //

            break;
        }

    }
    /*--------- switch(cpu_timer_module.cpu_timer0_state) ---------*/

}


////////////////////////////////////////////////////////////////////
//
//
//
static inline Master_INT_Mode_Output_State_Process(void)
{

    Uint16 wave_addr, wave_addr_1;
    float wave_c2, wave_c2_1;
    float wave_value;


    wave_addr = (Uint16)Global_Variables.dds.addr;
    wave_addr_1 = ( (Uint16)Global_Variables.dds.addr + 1 ) & 0x0FFF;

    cpu_timer_module.wave_c2_decimal = (float)Global_Variables.dds.dec1 * 0.003921569;


    //
    // 從WAVE buffer載入WAVE
    //
    wave_c2 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr ];
    wave_c2_1 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr_1 ];

    //
    // 內差法計算wave value
    //
    wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;

    //
    // MS模式先不加Duty可調
    //
#ifdef BYPASS
    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != WAVE_SQU )
    {
        //
        // 從WAVE buffer載入WAVE
        //
        wave_c2 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr ];
        wave_c2_1 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr_1 ];

        //
        // 內差法計算wave value
        //
        wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;
    }
    else if ( Global_Variables.dds.addr <= Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = 32767.0;
    }
    else if ( Global_Variables.dds.addr > Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = -32767.0;
    }
#endif

    //
    // 輸出主中斷
    //
    switch(cpu_timer_module.cpu_timer0_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // Ready
        //
        case CPU_TIMER0_OUTPUT_READY_STATE:
        {
            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            //
            // 設定EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1:
        {
            //
            // 快到達目標值
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow )
                    <= Global_Variables.OUTPUT_VARIABLES.WAVE_set_diff_level )
            {
                if ( Memory_Module.wave_buffer_section == 0 )
                {
                    //
                    // SIN, Slew Rate => OFF
                    //
                    cpu_timer_module.slew_rate_state = NO_SLEWRATE_STATE;
                }
                else
                {
                    //
                    // Slew Rate => ON
                    //
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;
                }

                //
                // 載入start phase到dds
                //
                Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;

                //
                // 量測補償ON
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 1;
                Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;
                Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;


                //
                // Reset WAVE以及VREF補償量
                //
                Output_Control_Module.WAVE_Compensate_Offset = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;


                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE;

                break;
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );



            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            if ( ( ( Global_Variables.MS_Function_Variables.MS_Total_Units == 2 )
                   && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[1].bit.Slave_Wait_Change_Sync == 1 ) )
                 || ( ( Global_Variables.MS_Function_Variables.MS_Total_Units == 3 )
                      && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[1].bit.Slave_Wait_Change_Sync == 1 )
                      && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[2].bit.Slave_Wait_Change_Sync == 1 ) ) )
            {

                //
                // SPI傳送同步訊號
                //
                Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.Change_Sync_Trig_0 = 1;

                //
                // 開始計時
                //
                cpu_timer_module.MS_Sync_0_Singal_Start_Time = CpuTimer0.InterruptCount;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0_Shadow; //轉態

            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 由於 MS SPI同步週期 2.775u x 2
        // 所以先轉態多等待一個cycle
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0_Shadow:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1; //轉態

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );


            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Zero_Change_Phase_Enable == 1 )
            {
                //
                // Zero Change Enable
                //

                //
                // 等待滿一個週期再轉態
                //
                if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                {
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                    //
                    // Reset WAVE以及VREF補償量
                    //
                    Output_Control_Module.WAVE_Compensate_Offset = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                    CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                    //
                    // SPI傳送同步訊號
                    //
                    Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.Change_Sync_Trig_1 = 1;

                    //
                    // 開始計時
                    //
                    cpu_timer_module.MS_Sync_0_Singal_Start_Time = CpuTimer0.InterruptCount;

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                    break;
                }

            }
            else
            {
                //
                // Zero Change Disable
                //

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                //
                // Reset WAVE以及VREF補償量
                //
                Output_Control_Module.WAVE_Compensate_Offset = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                //
                // SPI傳送同步訊號
                //
                Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.Change_Sync_Trig_1 = 1;

                //
                // 開始計時
                //
                cpu_timer_module.MS_Sync_0_Singal_Start_Time = CpuTimer0.InterruptCount;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                break;
            }

            break;

        }

        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2:
        {
            cpu_timer_module.Value_Set_User_Change = 0;

            //
            // 載入頻率Step
            //
            Global_Variables.FreqStep = Global_Variables.FreqStep_shadow;

            //
            // 載入wave_buffer_section, 改變輸出波形
            //
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;


            //
            // 更新WAVE參數
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;

            Global_Variables.OUTPUT_VARIABLES.WAVE_offset = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;
            Global_Variables.OUTPUT_VARIABLES.WAVE_scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow;


            //
            // 設定值改變後為AC+DC輸出
            //

            // 更新VREF
            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

            //
            // 設定值改變後的起始相角
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;

            //
            // Reload dds
            //
            Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;


            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0;

            Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;
            Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

            //
            // Reset WAVE以及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

            break;

        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            if ( ( ( Global_Variables.MS_Function_Variables.MS_Total_Units == 2 )
                   && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[1].bit.Slave_Wait_OFF_Sync == 1 ) )
                 || ( ( Global_Variables.MS_Function_Variables.MS_Total_Units == 3 )
                      && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[1].bit.Slave_Wait_OFF_Sync == 1 )
                      && ( Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[2].bit.Slave_Wait_OFF_Sync == 1 ) ) )
            {
                //
                // 等待滿一個週期再轉態
                //
                if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                {

                    //
                    // SPI傳送同步訊號
                    //
                    Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.OFF_Sync_Trig_0 = 1;;

                    //
                    // 開始計時
                    //
                    cpu_timer_module.MS_Sync_0_Singal_Start_Time = CpuTimer0.InterruptCount;

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0_Shadow; //轉態

                    break;
                }
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // 由於 MS SPI同步週期 2.775u x 2
        // 所以先轉態多等待一個cycle
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0_Shadow:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE1; //轉態

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 0 )
            {
                //
                // OFF Phase Disable
                //

                //
                // SPI傳送同步訊號
                //
                Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.OFF_Sync_Trig_1 = 1;

                //
                // 開始計時
                //
                cpu_timer_module.MS_Sync_0_Singal_Start_Time = CpuTimer0.InterruptCount;

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

                break;
            }
            else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable == 1 )
            {
                //
                // OFF Phase Enable
                //

                //
                // 等待滿一個週期再轉態
                //
                if ( Global_Variables.dds_shadow.all > Global_Variables.dds.all )
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE; //轉態

                    break;
                }
            }

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE:
        {
            float scale, offset;
            Uint32 stop_phase_count_shadow;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            stop_phase_count_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_phase_count;


            //
            // 正常停止
            //
            if ( Global_Variables.dds.all >= stop_phase_count_shadow )
            {
                //
                // SPI傳送同步訊號
                //
                Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.OFF_Sync_Trig_1 = 1;

                //
                // 開始計時
                //
                cpu_timer_module.MS_Sync_0_Singal_Start_Time = CpuTimer0.InterruptCount;

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

                break;

            }

            break;
        }

//master
        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 0
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            //
            // 量測補償OFF
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

            //
            // 判斷是否降到0
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow ) <= 32.767 )
            {
                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE1;
                }
                else
                {
                    //
                    // 機器沒有輸出, 或有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;
                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 1
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            //
            // 逆電流卸載PID 初始化
            //

            //
            // 初始化補償器
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 2
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE2:
        {
            //
            // 逆電流卸載PID Loop
            //

            //
            // 補償計算
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB;

            // Compute the error
            // 計算Err
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref - PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb;

            // Compute the proportional output
            // 計算比例Up
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err;

            // Compute the integral output
            // 計算積分Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui + ( PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up );

            // Compute the pre-saturated output
            // 計算Up+Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up + PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui;

            // Saturate the output
            if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat > PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax;
            }
            else if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat < PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin;
            }
            else
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat;
            }

            //
            // Update到CLA模組去補償WAVE DAC
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset2 = -PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 3
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE3:
        {
            float vref_zero;
            float step;

            vref_zero = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            step = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step;

            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - vref_zero ) <= step )
            {
                //
                // 靠近 vref_zero
                //

                //
                // DAC降到zero
                //
                Init_VREF_WAVE_Process();

                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE4;

                    //
                    // 輸出Relay OFF
                    //
                    Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 0;
                    CPU_to_CLA_Variables.OUTPUT_FLAGS_all = Global_Variables.OUTPUT_FLAGS.all;

                    //
                    // SPI傳送同步訊號
                    //
                    Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.OFF_Sync_Trig_2 = 1;
                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;
                }
            }
            else
            {
                //
                // 每次遞減
                //

                Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - step );
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 4
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE4:
        {
            //
            // 等待被觸發轉態
            //

            break;
        }

    }
    /*--------- switch(cpu_timer_module.cpu_timer0_state) ---------*/


}





////////////////////////////////////////////////////////////////////
//
//
//
static inline Slave_INT_Mode_Output_State_Process(void)
{
    Uint16 wave_addr, wave_addr_1;
    float wave_c2, wave_c2_1;
    float wave_value;

    //
    // 更新dds以及同步指令
    //

    Global_Variables.dds.addr = SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] >> 4;
    Global_Variables.dds.dec1 = SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] >> 8;

    Global_Variables.dds.all = Global_Variables.dds.all + Global_Variables.MS_Function_Variables.MS_Output_Phase_Delay_Count;


    Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = SPI_Module_Variables.SPIB_Rx_Data_Shadow[2] & 0xFF;

    wave_addr = (Uint16)Global_Variables.dds.addr;
    wave_addr_1 = ( (Uint16)Global_Variables.dds.addr + 1 ) & 0x0FFF;

    cpu_timer_module.wave_c2_decimal = (float)Global_Variables.dds.dec1 * 0.003921569;

    //
    // 從WAVE buffer載入WAVE
    //
    wave_c2 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr ];
    wave_c2_1 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr_1 ];

    //
    // 內差法計算wave value
    //
    wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;

    //
    // MS模式先不加Duty可調
    //
#ifdef BYPASS
    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow != WAVE_SQU )
    {
        //
        // 從WAVE buffer載入WAVE
        //
        wave_c2 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr ];
        wave_c2_1 = (float)WAVE_DATA.local_ram[ Memory_Module.wave_buffer_section ][ wave_addr_1 ];

        //
        // 內差法計算wave value
        //
        wave_value = wave_c2 + ( wave_c2_1 - wave_c2 ) * cpu_timer_module.wave_c2_decimal;
    }
    else if ( Global_Variables.dds.addr <= Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = 32767.0;
    }
    else if ( Global_Variables.dds.addr > Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user )
    {
        //
        // SQU Duty可調
        //

        wave_value = -32767.0;
    }
#endif


    //
    // 輸出主中斷
    //
    switch(cpu_timer_module.cpu_timer0_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        //
        // Ready
        //
        case CPU_TIMER0_OUTPUT_READY_STATE:
        {
            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0:
        {
            if ( Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.ON_Sync_Trig )
            {
                //
                // 收到Master ON_Sync_Trig 同步觸發
                //

                Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[ Global_Variables.MS_Function_Variables.MS_Address ].bit.Slave_Wait_ON_Sync = 0;


                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            //
            // 設定EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 1;

            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE2;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE2:
        {
            //
            // 快到達目標值
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow )
                    <= Global_Variables.OUTPUT_VARIABLES.WAVE_set_diff_level )
            {
                if ( Memory_Module.wave_buffer_section == 0 )
                {
                    //
                    // SIN, Slew Rate => OFF
                    //
                    cpu_timer_module.slew_rate_state = NO_SLEWRATE_STATE;
                }
                else
                {
                    //
                    // Slew Rate => ON
                    //
                    cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;
                }

                //
                // 載入start phase到dds
                //
                Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;

                //
                // 量測補償ON
                //
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 1;
                Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;
                Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

                //
                // Reset WAVE以及VREF補償量
                //
                Output_Control_Module.WAVE_Compensate_Offset = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE;

                break;
            }


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );


            //
            // 停止輸出處理
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {
                cpu_timer_module.Value_Set_User_Change = 0;

                Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[ Global_Variables.MS_Function_Variables.MS_Address ].bit.Slave_Wait_OFF_Sync = 1;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0;

                break;
            }


            //
            // 改變設定值處理
            //
            if ( cpu_timer_module.Value_Set_User_Change == 1 )
            {
                Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[ Global_Variables.MS_Function_Variables.MS_Address ].bit.Slave_Wait_Change_Sync = 1;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0; //轉態

                break;
            }

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );

            if ( Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.Change_Sync_Trig_0 )
            {
                //
                // 收到Master Change_Sync_Trig_0 同步觸發
                //


                Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[ Global_Variables.MS_Function_Variables.MS_Address ].bit.Slave_Wait_Change_Sync = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );


            if ( Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.Change_Sync_Trig_1 )
            {
                //
                // 收到Master Change_Sync_Trig_1 同步觸發
                //

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset;

                //
                // Reset WAVE以及VREF補償量
                //
                Output_Control_Module.WAVE_Compensate_Offset = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
                CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2;

                break;

            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2:
        {
            cpu_timer_module.Value_Set_User_Change = 0;

            //
            // 載入頻率step
            //
            Global_Variables.FreqStep = Global_Variables.FreqStep_shadow;

            //
            // 載入wave_buffer_section, 改變輸出波形
            //
            Memory_Module.wave_buffer_section = Memory_Module.wave_buffer_section_shadow;


            //
            // 更新WAVE參數
            //
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;

            Global_Variables.OUTPUT_VARIABLES.WAVE_offset = Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow;
            Global_Variables.OUTPUT_VARIABLES.WAVE_scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow;


            //
            // 設定值改變後為AC+DC輸出
            //

            // 更新VREF
            Global_Variables.OUTPUT_VARIABLES.VREF_set = Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow;

            //
            // 設定值改變後的起始相角
            //
            Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow = 0;

            // Reload dds
            Global_Variables.dds.all = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_phase_count_shadow;


            //
            // 轉態
            //
            cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1;

            Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;
            Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

            //
            // Reset WAVE以及VREF補償量
            //
            Output_Control_Module.WAVE_Compensate_Offset = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
            CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;

            break;
        }



        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );


            if ( Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.OFF_Sync_Trig_0 )
            {
                //
                // 收到Master OFF_Sync_Trig_0 同步觸發
                //


                Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[ Global_Variables.MS_Function_Variables.MS_Address ].bit.Slave_Wait_OFF_Sync = 0;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE1;
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        //
        //
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            float scale, offset;

            //
            // 轉成WAVE_set_goal
            //
            scale = Global_Variables.OUTPUT_VARIABLES.WAVE_scale;
            offset = (float)Global_Variables.OUTPUT_VARIABLES.WAVE_offset;
            Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = (int32)( wave_value * scale + offset );


            if ( Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.OFF_Sync_Trig_1 )
            {
                //
                // 收到Master OFF_Sync_Trig_1 同步觸發
                //

                Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = WAVE_DAC_ZERO;

                cpu_timer_module.slew_rate_state = WAVE_SLEWRATE_STATE;

                //
                // 轉態
                // 沒有輸出時, Zero Cross Trig不動作
                //
                cpu_timer_module.Zero_Cross_State = ZERO_CROSS_INIT_STATE;

                //
                // 轉態
                //
                cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE0;

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 0
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            //
            // 量測補償OFF
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

            //
            // 判斷是否降到0
            //
            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow ) <= 32.767 )
            {
                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE1;
                }
                else
                {
                    //
                    // 機器沒有輸出, 或有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE3;
                }
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 1
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            //
            // 逆電流卸載PID 初始化
            //

            //
            // 初始化補償器
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 2
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE2:
        {
            //
            // 逆電流卸載PID Loop
            //

            //
            // 補償計算
            //
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB;

            // Compute the error
            // 計算Err
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref - PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb;

            // Compute the proportional output
            // 計算比例Up
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err;

            // Compute the integral output
            // 計算積分Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui + ( PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki * PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up );

            // Compute the pre-saturated output
            // 計算Up+Ui
            PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up + PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui;

            // Saturate the output
            if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat > PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax;
            }
            else if (PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat < PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin)
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out =  PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin;
            }
            else
            {
                PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat;
            }

            //
            // Update到CLA模組去補償WAVE DAC
            // !!! 要注意這裡的補償正負號, 實測過要帶負號才會收斂 !!!
            //
            Output_Control_Module.WAVE_Compensate_Offset2 = -PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out;


            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 3
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE3:
        {
            float vref_zero;
            float step;

            vref_zero = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

            step = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_OFF_Vref_SlewRate_step;

            if ( fabs( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - vref_zero ) <= step )
            {
                //
                // 靠近 vref_zero
                //

                //
                // DAC降到zero
                //
                Init_VREF_WAVE_Process();

                if ( ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all == 0 )
                     && ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all == 0 ) )
                {
                    //
                    // 機器輸出中, 且 沒有異常發生
                    //

                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_OFF_STATE4;
                }
                else
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.cpu_timer0_state = CPU_TIMER0_OUTPUT_READY_STATE;
                }
            }
            else
            {
                //
                // 每次遞減
                //

                Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)( (float)Global_Variables.OUTPUT_VARIABLES.VREF_set - step );
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        //
        // ALL
        // Output OFF 4
        //
        case CPU_TIMER0_OUTPUT_OFF_STATE4:
        {
            //
            // 等待被觸發轉態
            //

            if ( Global_Variables.MS_Function_Variables.Output_Sync_Trig.bit.OFF_Sync_Trig_2 )
            {
                //
                // 輸出Relay OFF
                //
                Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 0;
                CPU_to_CLA_Variables.OUTPUT_FLAGS_all = Global_Variables.OUTPUT_FLAGS.all;


            }


            break;
        }

    }
    /*--------- switch(cpu_timer_module.cpu_timer0_state) ---------*/

}








////////////////////////////////////////////////////////////////////
//
//
//
static inline SPI_Parser_Process(void)
{
    //
    // SPI指令解析
    //
    if ( CLA_Module_Variables.cla_spi_state == CLA_SPI_PARSING_STATE )
    {
        Uint16 checksum;

        //
        // SPI指令解析
        //
        CMD_Parser_Process2( CLA_to_CPU_Variables.RXBUFFER, CPU_to_CLA_Variables.TXBUFFER );

        //
        // 加上CheckSum
        //
        checksum = ( CPU_to_CLA_Variables.TXBUFFER[1] & 0xFF ) + ( ( CPU_to_CLA_Variables.TXBUFFER[1] >> 8) & 0xFF );
        CPU_to_CLA_Variables.TXBUFFER[0] = CPU_to_CLA_Variables.TXBUFFER[0] + checksum;

        //
        // 資料塞到SPI等著被收走
        //
        McbspbRegs.DXR2.all = CPU_to_CLA_Variables.TXBUFFER[0];
        McbspbRegs.DXR1.all = CPU_to_CLA_Variables.TXBUFFER[1];

        //
        // 轉態
        //
        CLA_Module_Variables.cla_spi_state = CLA_SPI_REPLY_STATE;
    }

}


////////////////////////////////////////////////////////////////////
//
//
//
static inline EXTIO_Update_Process(void)
{
    //
    // 寫入EXT IO介面
    //
    long *addr;
    Uint16 data;

    addr = (long *)EXTIO_OUT_ADDR;
    Global_Variables.EXT_OUTPUT_FLAG.bit.SOFTWARE_BUSY = Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy;
    data = Global_Variables.EXT_OUTPUT_FLAG.all;
    memcpy_fast( addr, &data, 1 );

}



////////////////////////////////////////////////////////////////////
//
//
//
static inline Error_OFF_Process(void)
{
    //
    // 只要異常發生, 觸發轉態
    //
    if ( ( Main_State_Module.Machine_state != ERROR_STATE )
         &&( ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all != 0 )
             || ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all != 0 )
             || ( CLA_to_CPU_Variables._4CF_OCP == 1 )
             || ( CLA_to_CPU_Variables.Short_OCP == 1 ) ) )
    {
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
    }


}


////////////////////////////////////////////////////////////////////
//
//
//
static inline Zero_Cross_Process(void)
{
    //
    // Zero Cross 處理
    //
    switch( cpu_timer_module.Zero_Cross_State )
    {
        /*----------------------------------*/
        case ZERO_CROSS_INIT_STATE:
        {
            Global_Variables.EXT_OUTPUT_FLAG.bit.ZERO_CROSS_TRIG_OUT = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Option.bit.H_L_Level;

            break;
        }

        /*----------------------------------*/
        case ZERO_CROSS_READY_STATE:
        {
            if ( Global_Variables.dds_shadow2.all > Global_Variables.dds.all )
            {
                //
                // 0度觸發
                //

                Global_Variables.EXT_OUTPUT_FLAG.bit.ZERO_CROSS_TRIG_OUT = !Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Option.bit.H_L_Level;

                //
                // Reload Count
                //
                cpu_timer_module.Zero_Cross_Width_Counter = cpu_timer_module.Zero_Cross_Width_Count;

                //
                // 轉態
                //
                cpu_timer_module.Zero_Cross_State = ZERO_CROSS_TRIG_HOLD_STATE;
            }

            break;
        }

        /*----------------------------------*/
        case ZERO_CROSS_TRIG_HOLD_STATE:
        {
            cpu_timer_module.Zero_Cross_Width_Counter --;

            if ( cpu_timer_module.Zero_Cross_Width_Counter == 0 )
            {
                Global_Variables.EXT_OUTPUT_FLAG.bit.ZERO_CROSS_TRIG_OUT = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Option.bit.H_L_Level;

                //
                // 轉態
                //
                cpu_timer_module.Zero_Cross_State = ZERO_CROSS_READY_STATE;
            }

            break;
        }
    }

    //
    // 更新dds_shadow2
    //
    Global_Variables.dds_shadow2.all = Global_Variables.dds.all;


}


////////////////////////////////////////////////////////////////////
//
//
//
static inline Calc_SlewRate_Process(void)
{
    //
    // SlewRate判斷
    //
    if ( cpu_timer_module.slew_rate_state == WAVE_SLEWRATE_STATE )
    {
        float step;

        //
        // SOFT_RAMP
        //

        step = ( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set ) * Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SoftRamp_Step_set;

        if ( ( 0 < step ) && ( step < 1.0 ) )
        {
            step = 1.0;
        }
        else if ( ( -1.0 < step ) && ( step < 0 ) )
        {
            step = -1.0;
        }

        Global_Variables.OUTPUT_VARIABLES.WAVE_set = (int32)( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set + step );

    }
    else if ( cpu_timer_module.slew_rate_state == NO_SLEWRATE_STATE )
    {
        Global_Variables.OUTPUT_VARIABLES.WAVE_set = Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow;
    }

}

////////////////////////////////////////////////////////////////////
//
//
//
static inline Calc_SlewRate_Process_2(void)
{
    //
    // 固定使用SlewRate
    //

    float step;

    //
    // SOFT_RAMP
    //

    step = ( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal_shadow - (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set ) * Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SoftRamp_Step_set;

    if ( ( 0 < step ) && ( step < 1.0 ) )
    {
        step = 1.0;
    }
    else if ( ( -1.0 < step ) && ( step < 0 ) )
    {
        step = -1.0;
    }

    Global_Variables.OUTPUT_VARIABLES.WAVE_set = (int32)( (float)Global_Variables.OUTPUT_VARIABLES.WAVE_set + step );

}



////////////////////////////////////////////////////////////////////
//
//
//
static inline SYNC_Period_Meas_Process(void)
{
    //
    // 處理SYNC模式累加週期流程
    //
    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source == SYNC_EXT )
    {
        //
        // 計算週期
        //
        cpu_timer_module.EXTAD_TTL_Period_shadow = cpu_timer_module.EXTAD_TTL_Period_shadow + 1.0;

        switch(cpu_timer_module.EXTAD_state)
        {
            /*-----------------------------------------------------------------------------------------*/
            case EXTAD_INIT_STATE:
            {
                //
                // 直接從L開始
                //

                //
                // 轉態
                //
                cpu_timer_module.EXTAD_state = EXTAD_L_STATE;

                cpu_timer_module.EXTAD_Change_Counter = 0;

                cpu_timer_module.EXTAD_TTL_Period = 0;
                cpu_timer_module.EXTAD_TTL_Period_shadow = 0;

                break;
            }

            /*-----------------------------------------------------------------------------------------*/
            case EXTAD_L_STATE:
            {
                //
                #ifdef DEVELOP_SV58
                Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
                #endif

                if ( Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN >= Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN_TTL_Level_H )
                {

                    cpu_timer_module.EXTAD_Change_Counter ++;

                    //
                    // EXT同步, 頻率範圍有到2000Hz
                    // 平均次數越高, 則同步的速度會較慢
                    // 但平均次數不能太低會影響量測頻率值, 進而影響鎖相穩定
                    // 實際測試取100次平均ok
                    //
                    if ( cpu_timer_module.EXTAD_Change_Counter >= 100 )
                    {
                        cpu_timer_module.EXTAD_Change_Counter = 0;

                        cpu_timer_module.EXTAD_TTL_Period = cpu_timer_module.EXTAD_TTL_Period_shadow * 0.01;

                        cpu_timer_module.EXTAD_TTL_Period_shadow = 0;

                        Phase_Sync_Module.LINE_Phase_Sync_Variables.dds_addr = (float)Global_Variables.dds.addr;

                        if ( Phase_Sync_Module.LINE_Phase_Sync_Flags.bit.Phase_Sync_Process == 0 )
                        {
                            //
                            // 觸發鎖相計算
                            //
                            Phase_Sync_Module.LINE_Phase_Sync_Flags.bit.Phase_Sync_Process = 1;
                        }

                    }

                    //
                    // 轉態
                    //
                    cpu_timer_module.EXTAD_state = EXTAD_H_STATE;

                }

                break;
            }

            /*-----------------------------------------------------------------------------------------*/
            case EXTAD_H_STATE:
            {
                //
                #ifdef DEVELOP_SV58
                Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 1;
                #endif

                if ( Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN <= Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN_TTL_Level_L )
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.EXTAD_state = EXTAD_L_STATE;
                }

                break;
            }
        }
        /*--------- switch(cpu_timer_module.EXTAD_state) ---------*/

    }
    else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source == SYNC_LINE )
    {
        //
        // 計算週期
        //
        cpu_timer_module.EXTAD_TTL_Period_shadow = cpu_timer_module.EXTAD_TTL_Period_shadow + 1.0;

        switch(cpu_timer_module.EXTAD_state)
        {
            /*-----------------------------------------------------------------------------------------*/
            case EXTAD_INIT_STATE:
            {
                //
                // 直接從H開始
                //

                //
                // 轉態
                //
                cpu_timer_module.EXTAD_state = EXTAD_H_STATE;

                cpu_timer_module.EXTAD_Change_Counter = 0;

                cpu_timer_module.EXTAD_TTL_Period = 0;
                cpu_timer_module.EXTAD_TTL_Period_shadow = 0;

                break;
            }

            /*-----------------------------------------------------------------------------------------*/
            case EXTAD_H_STATE:
            {
                //
                #ifdef DEVELOP_SV58
                Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 1;
                #endif

                if ( ERR_INPUT == 0 )
                {
                    cpu_timer_module.EXTAD_Change_Counter ++;

                    //
                    // LINE同步, 頻率範圍大約100Hz以內
                    // 平均次數越高, 則同步的速度會較慢
                    // 實際交流電50/60Hz會稍微飄動
                    // 實際測試取10次平均ok
                    //
                    if ( cpu_timer_module.EXTAD_Change_Counter >= 10 )
                    {
                        cpu_timer_module.EXTAD_Change_Counter = 0;

                        cpu_timer_module.EXTAD_TTL_Period = cpu_timer_module.EXTAD_TTL_Period_shadow * 0.1;

                        cpu_timer_module.EXTAD_TTL_Period_shadow = 0;

                        Phase_Sync_Module.LINE_Phase_Sync_Variables.dds_addr = (float)Global_Variables.dds.addr;

                        if ( Phase_Sync_Module.LINE_Phase_Sync_Flags.bit.Phase_Sync_Process == 0 )
                        {
                            //
                            // 觸發鎖相計算
                            //
                            Phase_Sync_Module.LINE_Phase_Sync_Flags.bit.Phase_Sync_Process = 1;
                        }

                    }

                    //
                    // 觸發主迴圈更新ERR
                    //
                    Event_Module.Event4_Flag.bit.Input_Flags_Update = 1;

                    //
                    // 轉態
                    //
                    cpu_timer_module.EXTAD_state = EXTAD_L_STATE;
                }

                break;
            }

            /*-----------------------------------------------------------------------------------------*/
            case EXTAD_L_STATE:
            {
                //
                #ifdef DEVELOP_SV58
                Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
                #endif

                if ( ( ERR_INPUT == 1 ) && ( Event_Module.Event4_Flag.bit.Input_Flags_Update == 0 ) )
                {

                    //
                    // 轉態
                    //
                    cpu_timer_module.EXTAD_state = EXTAD_H_STATE;
                }

                break;
            }
        }
        /*--------- switch(cpu_timer_module.EXTAD_state) ---------*/
    }


}




////////////////////////////////////////////////////////////////////
//
//
//
static inline EXT_Period_Meas_Process(void)
{
    //
    // 濾除交流成分
    // 數位濾波器計算值流值
    //
    EXT_AD_DC_FILTER_LPF.x_n = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN;
    EXT_AD_DC_FILTER_LPF.y_n = EXT_AD_DC_FILTER_LPF.C1 * ( EXT_AD_DC_FILTER_LPF.x_n + EXT_AD_DC_FILTER_LPF.x_n_1 ) + EXT_AD_DC_FILTER_LPF.D1 * EXT_AD_DC_FILTER_LPF.y_n;
    EXT_AD_DC_FILTER_LPF.x_n_1 = EXT_AD_DC_FILTER_LPF.x_n;
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_LPF_Out = (Uint16)EXT_AD_DC_FILTER_LPF.y_n;
    //24 cycles

    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_EXT_MODE )
    {
        Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level = Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_LPF_Out;
    }


    //
    // 濾除高頻成分
    //
    EXT_AD_LPF.x_n = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN;
    EXT_AD_LPF.y_n = EXT_AD_LPF.C1 * ( EXT_AD_LPF.x_n + EXT_AD_LPF.x_n_1 ) + EXT_AD_LPF.D1 * EXT_AD_LPF.y_n;
    EXT_AD_LPF.x_n_1 = EXT_AD_LPF.x_n;
    //24 cycles

    //
    // 計算週期
    //

    cpu_timer_module.EXTAD_TTL_Period_shadow = cpu_timer_module.EXTAD_TTL_Period_shadow + 1.0;

    switch(cpu_timer_module.EXTAD_state)
    {

        /*-----------------------------------------------------------------------------------------*/
        case EXTAD_INIT_STATE:
        {
            //
            //直接從L開始
            //

            //
            // 轉態
            //
            cpu_timer_module.EXTAD_state = EXTAD_L_STATE;

            cpu_timer_module.EXTAD_Change_Counter = 0;

            cpu_timer_module.EXTAD_TTL_Period = 0;
            cpu_timer_module.EXTAD_TTL_Period_shadow = 0;

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case EXTAD_L_STATE:
        {
            //
            #ifdef DEVELOP_SV58
            Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
            #endif

            if ( (Uint16)EXT_AD_LPF.y_n >= Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_LPF_Out )
            {

                cpu_timer_module.EXTAD_Change_Counter ++;

                if ( cpu_timer_module.EXTAD_Change_Counter == 10 )
                {
                    cpu_timer_module.EXTAD_Change_Counter = 0;
                    cpu_timer_module.EXTAD_TTL_Period = cpu_timer_module.EXTAD_TTL_Period_shadow * 0.1;
                    cpu_timer_module.EXTAD_TTL_Period_shadow = 0;
                }

                //
                // 轉態
                //
                cpu_timer_module.EXTAD_state = EXTAD_H_STATE;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case EXTAD_H_STATE:
        {
            //
            #ifdef DEVELOP_SV58
            Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 1;
            #endif

            if ( (Uint16)EXT_AD_LPF.y_n <= Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_LPF_Out )
            {
                //
                // 轉態
                //
                cpu_timer_module.EXTAD_state = EXTAD_L_STATE;
            }

            break;
        }

    }
    /*--------- switch(cpu_timer_module.EXTAD_state) ---------*/

}







////////////////////////////////////////////////////////////////////
//
//
//
static inline ADD_Calc_DC_Level_Process(void)
{
    //
    // 濾除交流成分
    // 數位濾波器計算值流值
    //
    EXT_AD_DC_FILTER_LPF.x_n = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN;
    EXT_AD_DC_FILTER_LPF.y_n = EXT_AD_DC_FILTER_LPF.C1 * ( EXT_AD_DC_FILTER_LPF.x_n + EXT_AD_DC_FILTER_LPF.x_n_1 ) + EXT_AD_DC_FILTER_LPF.D1 * EXT_AD_DC_FILTER_LPF.y_n;
    EXT_AD_DC_FILTER_LPF.x_n_1 = EXT_AD_DC_FILTER_LPF.x_n;
    Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_LPF_Out = (Uint16)EXT_AD_DC_FILTER_LPF.y_n;
    //24 cycles

    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all == AC_ADD_MODE )
    {
        Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_DC_Level = Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_DC_Filter_LPF_Out;
    }

}





////////////////////////////////////////////////////////////////////
//
//
//
static inline EXT_Calc_ACV_Set_Process(void)
{
    float scale, offset;
    float ext_v;
    float ext_acv_set;

    //
    // 濾除高頻成分
    //
    //EXT_AD_LPF.x_n = (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN;
    //EXT_AD_LPF.y_n = EXT_AD_LPF.C1 * ( EXT_AD_LPF.x_n + EXT_AD_LPF.x_n_1 ) + EXT_AD_LPF.D1 * EXT_AD_LPF.y_n;
    //EXT_AD_LPF.x_n_1 = EXT_AD_LPF.x_n;

    //
    // 校正找出 EXT Vin
    //
    scale = Global_Variables.Pre_Calc_Variables.inv_ADC_to_EXTIN_L1_Scale;
    offset = Global_Variables.Calibration_Variables.ADC_to_EXTIN_L1_offset.all;
    //ext_v = ( EXT_AD_LPF.y_n - offset ) * scale;


    ext_v = ( (float)Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN - offset ) * scale;

    ext_acv_set = ext_v * (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.all * 0.0001;

    //
    // 箝制處理
    //
    if ( ext_acv_set >= Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout )
    {
        Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set = Global_Variables.MACHINE_SERIES_IMFORMATION.AC_Rated_Vout;
    }
    else if ( ext_acv_set <= 0 )
    {
        Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set = 0;
    }
    else
    {
        Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set = ext_acv_set;
    }

}




////////////////////////////////////////////////////////////////////
//
//
//
static inline MS_SDRAM_Sending_Process(void)
{
    switch( cpu_timer_module.MS_SDRAM_Sending_State )
    {
        case MS_SDRAM_Sending_Ready:
        {

            break;
        }
        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process0:
        {
            Uint32 SyncWavePage, SyncWaveAddr;
            Uint16 SyncWaveData = 0;

            if ( MS_Control_Module_Variables.Sync_Wave_Addr_Counter < 4096 )
            {
                //
                // 未滿4096
                //

                //
                // Master取出SDRAM Data
                //
                SyncWavePage = (Uint32)MS_Control_Module_Variables.Sync_Wave_Page_Counter << 12;
                SyncWaveAddr = (Uint32)MS_Control_Module_Variables.Sync_Wave_Addr_Counter;
                SyncWaveData = *( SDRAM_variables.SDRAMBuf + SyncWavePage + SyncWaveAddr );

                //
                // 累加CheckSum
                //
                MS_Control_Module_Variables.Sync_Wave_data_CheckSum = MS_Control_Module_Variables.Sync_Wave_data_CheckSum + SyncWaveData;

                //SPI_Module_Variables.SPIB_Tx_Data_Shadow[0] = MS_Control_Module_Variables.Sync_Wave_Addr_Counter;
                //SPI_Module_Variables.SPIB_Tx_Data_Shadow[1] = SyncWaveData;
                SPI_Module_Variables.SPIB_Tx_Data_Shadow[0] = SyncWaveData;
                SPI_Module_Variables.SPIB_Tx_Data_Shadow[1] = SyncWaveData;

                //
                // SPI 傳送
                //
                SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[0];
                SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[1];


                MS_Control_Module_Variables.Sync_Wave_Addr_Counter ++;

                //
                // 轉態
                //
                cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process0a;

            }
            else if ( MS_Control_Module_Variables.Sync_Wave_Addr_Counter >= 4096 )
            {
                //
                // 滿4096
                //

                //
                // Reset Counter
                //
                MS_Control_Module_Variables.Sync_Wave_Addr_Counter = 0;

                //
                // 轉態
                //
                cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process0c;

            }

            break;

        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process0a:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process0b;

            break;

        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process0b:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process0;

            break;

        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process0c:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process0d;

            break;

        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process0d:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process1;

            break;

        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process1:
        {
            //
            // 傳送CF MSB
            //

            Uint16 wave_set;
            Uint16 cf_value;

            wave_set = MS_Control_Module_Variables.Sync_Wave_Page_Counter;
            cf_value = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[wave_set].MSB;

            SPI_Module_Variables.SPIB_Tx_Data_Shadow[0] = cf_value;
            SPI_Module_Variables.SPIB_Tx_Data_Shadow[1] = cf_value;

            //
            // 累加CheckSum
            //
            MS_Control_Module_Variables.Sync_Wave_data_CheckSum = MS_Control_Module_Variables.Sync_Wave_data_CheckSum + cf_value;

            //
            // SPI 傳送
            //
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[0];
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[1];

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process1a;

            break;
        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process1a:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process1b;

            break;

        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process1b:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process2;

            break;

        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process2:
        {
            //
            // 傳送CF LSB
            //

            Uint16 wave_set;
            Uint16 cf_value;

            wave_set = MS_Control_Module_Variables.Sync_Wave_Page_Counter;
            cf_value = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[wave_set].LSB;

            SPI_Module_Variables.SPIB_Tx_Data_Shadow[0] = cf_value;
            SPI_Module_Variables.SPIB_Tx_Data_Shadow[1] = cf_value;

            //
            // 累加CheckSum
            //
            MS_Control_Module_Variables.Sync_Wave_data_CheckSum = MS_Control_Module_Variables.Sync_Wave_data_CheckSum + cf_value;

            //
            // SPI 傳送
            //
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[0];
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[1];

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process2a;

            break;
        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process2a:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process2b;

            break;

        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process2b:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process3;

            break;

        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process3:
        {
            //
            // 傳送CheckSum
            //

            SPI_Module_Variables.SPIB_Tx_Data_Shadow[0] = MS_Control_Module_Variables.Sync_Wave_data_CheckSum;
            SPI_Module_Variables.SPIB_Tx_Data_Shadow[1] = MS_Control_Module_Variables.Sync_Wave_data_CheckSum;

            //
            // SPI 傳送
            //
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[0];
            SpibRegs.SPITXBUF = SPI_Module_Variables.SPIB_Tx_Data_Shadow[1];

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process3a;

            break;
        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process3a:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process3b;

            break;

        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process3b:
        {
            //
            // Wait 1 cycle
            //

            //
            // 轉態
            //
            cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process4;

            break;

        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Sending_Process4:
        {

            //
            // 等待觸發轉態
            //

            break;
        }

    }
}






////////////////////////////////////////////////////////////////////
//
//
//
static inline MS_SDRAM_Receving_Process(void)
{
    switch( cpu_timer_module.MS_SDRAM_Receving_State )
    {
        case MS_SDRAM_Receving_Ready:
        {

            break;
        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Receving_Process0:
        {
            //
            // 接收Wave data x 4096
            //

            if ( ( SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig == 1 )
                 && ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] == SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] ) )
            {
                //
                // SPI收到資料
                //

                Uint16 SyncWaveAddr;
                Uint16 SyncWaveData;

                SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig = 0;


                SyncWaveAddr = MS_Control_Module_Variables.Sync_Wave_Addr_Counter;
                SyncWaveData = SPI_Module_Variables.SPIB_Rx_Data_Shadow[1];

                //
                // 存到RAM
                //
                WAVE_DATA.local_ram[5][SyncWaveAddr] = (int16)SyncWaveData;

                //
                // 累加CheckSum
                //
                MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow = MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow + SyncWaveData;

                //
                // 累加counter
                //
                MS_Control_Module_Variables.Sync_Wave_Addr_Counter ++;

                if ( MS_Control_Module_Variables.Sync_Wave_Addr_Counter == 4096 )
                {
                    //
                    // 轉態
                    //
                    cpu_timer_module.MS_SDRAM_Receving_State = MS_SDRAM_Receving_Process1;

                }
            }

            break;
        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Receving_Process1:
        {
            //
            // 接收 CF MSB
            //

            if ( ( SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig == 1 )
                 && ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] == SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] ) )
            {
                //
                // SPI收到資料
                //

                Uint16 wave_set;
                Uint16 cf_value;

                SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig = 0;

                wave_set = MS_Control_Module_Variables.Sync_Wave_Page_Counter;
                cf_value = SPI_Module_Variables.SPIB_Rx_Data_Shadow[0];

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[wave_set].MSB = cf_value;


                //
                // 累加CheckSum
                //
                MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow = MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow + cf_value;


                //
                // 轉態
                //
                cpu_timer_module.MS_SDRAM_Receving_State = MS_SDRAM_Receving_Process2;

            }

            break;
        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Receving_Process2:
        {
            //
            // 接收 CF LSB
            //

            if ( ( SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig == 1 )
                 && ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] == SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] ) )
            {
                //
                // SPI收到資料
                //

                Uint16 wave_set;
                Uint16 cf_value;

                SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig = 0;

                wave_set = MS_Control_Module_Variables.Sync_Wave_Page_Counter;
                cf_value = SPI_Module_Variables.SPIB_Rx_Data_Shadow[0];

                Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[wave_set].LSB = cf_value;


                //
                // 累加CheckSum
                //
                MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow = MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow + cf_value;


                //
                // 轉態
                //
                cpu_timer_module.MS_SDRAM_Receving_State = MS_SDRAM_Receving_Process3;

            }

            break;
        }


        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Receving_Process3:
        {
            //
            // 接收 CheckSum
            //

            if ( ( SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig == 1 )
                 && ( SPI_Module_Variables.SPIB_Rx_Data_Shadow[0] == SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] ) )
            {
                //
                // SPI收到資料
                //
                SPI_Module_Variables.SPI_Module_Control_flag.bit.SPIB_RX_Trig = 0;


                MS_Control_Module_Variables.Sync_Wave_data_CheckSum = SPI_Module_Variables.SPIB_Rx_Data_Shadow[0];

                //
                // 轉態
                //
                cpu_timer_module.MS_SDRAM_Receving_State = MS_SDRAM_Receving_Process4;

            }

            break;
        }

        ////////////////////////////////////////////////////////////
        //
        // (只有MS同步SDRAM使用)
        //
        case MS_SDRAM_Receving_Process4:
        {

            //
            // 等待觸發轉態
            //

            break;
        }



    }
}






////////////////////////////////////////////////////////////////////
//
//
//
static inline MS_Balance_Fast_Compensate_Process(void)
{
    //
    // 處理MS暫態補償
    //

    Global_Variables.MS_Function_Variables.Master_IFB = (float)SPI_Module_Variables.SPIB_Rx_Data_Shadow[1] * 0.01 - 80.0;

    //
    // 補償計算
    //
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ref = Global_Variables.MS_Function_Variables.Master_IFB;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Fdb = CLA_to_CPU_Variables.I_FB_shadow2;

    // Compute the error
    //計算Err
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Err = PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ref - PID_Control_Module.MS_Balance_Fast_Compensate_loop.Fdb;

    // Compute the proportional output
    //計算比例Up
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Up = PID_Control_Module.MS_Balance_Fast_Compensate_loop.Kp * PID_Control_Module.MS_Balance_Fast_Compensate_loop.Err;

    // Compute the integral output
    //計算積分Ui
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ui = PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ui + ( PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ki * PID_Control_Module.MS_Balance_Fast_Compensate_loop.Up );

    // Compute the pre-saturated output
    //計算Up+Ui+Ud
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutPreSat = PID_Control_Module.MS_Balance_Fast_Compensate_loop.Up + PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ui;

    // Saturate the output
    if (PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutPreSat > PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutMax)
    {
        PID_Control_Module.MS_Balance_Fast_Compensate_loop.Out =  PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutMax;
    }
    else if (PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutPreSat < PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutMin)
    {
        PID_Control_Module.MS_Balance_Fast_Compensate_loop.Out =  PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutMin;
    }
    else
    {
        PID_Control_Module.MS_Balance_Fast_Compensate_loop.Out = PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutPreSat;
    }

    #ifdef DEVELOP_SV41
    CPU_to_CLA_Variables.VREF_Compensate_Offset_2 = 0;
    #else
    //
    // Update到CLA模組去補償VREF DAC
    //
    CPU_to_CLA_Variables.VREF_Compensate_Offset_2 = PID_Control_Module.MS_Balance_Fast_Compensate_loop.Out;
    #endif


}


////////////////////////////////////////////////////////////////////
//
//
//
static inline CMD_Parser_Process2( volatile Uint16 *rx_buffer, volatile Uint16 *tx_buffer )
{
    Uint16 spi_addr_section;

    spi_addr_section = rx_buffer[0] >> 8;


    //TX[0]
    tx_buffer[0] = rx_buffer[0];

    switch( spi_addr_section )
    {
        /*-----------------------------------------------------------------------------------------*/
        case 0x04:
        {
            switch( rx_buffer[0] ) //取前面位址
            {
                /*-----------------------------------------------------------------------------------------*/
                case C2000_Version_spi_addr: //C2000 Version
                {
                    tx_buffer[1] = DSP_FW_Version_Code;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Machine_Status_spi_addr: //Machine Status
                {
                    tx_buffer[1] = Global_Variables.POWER_STATUS.all;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Vrms_MSB_spi_addr: //Vrms MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.MSB;
                    break;
                }

                case Vrms_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Irms_MSB_spi_addr: //Irms MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.MSB;
                    break;
                }

                case Irms_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Ipeak_max_MSB_spi_addr: //Ipeak max MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display.MSB;
                    break;
                }

                case Ipeak_max_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case W_MSB_spi_addr: //W MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.P_display.MSB;
                    break;
                }

                case W_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.P_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Ipeak_min_MSB_spi_addr: //Ipeak min MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display.MSB;
                    break;
                }

                case Ipeak_min_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Vpeak_max_MSB_spi_addr: //Vpeak max MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display.MSB;
                    break;
                }

                case Vpeak_max_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Vpeak_min_MSB_spi_addr: //Vpeak min MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display.MSB;
                    break;
                }

                case Vpeak_min_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Vdc_avg_MSB_spi_addr: //Vdc avg MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.MSB;
                    break;
                }

                case Vdc_avg_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.LSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Idc_avg_MSB_spi_addr: //Idc avg MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.MSB;
                    break;
                }

                case Idc_avg_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case VA_MSB_spi_addr: //VA MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.S_display.MSB;
                    break;
                }

                case VA_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.S_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case VAR_MSB_spi_addr: //VAR MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.Q_display.MSB;
                    break;
                }

                case VAR_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.Q_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case PF_spi_addr: //PF, 單位0.001
                {
                    tx_buffer[1] = (Uint16)Global_Variables.POWER_DISPLAY_VARIABLES.PF_display;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case CF_spi_addr: //CF, 單位0.01
                {
                    tx_buffer[1] = (Uint16)Global_Variables.POWER_DISPLAY_VARIABLES.CF_display;
                    break;
                }



                /*-----------------------------------------------------------------------------------------*/
                case Ipeak_hold_MSB_spi_addr: //Ipeak hold MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.MSB;
                    break;
                }

                case Ipeak_hold_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.LSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Calibration_State_spi_addr: //Calibration State
                {
                    tx_buffer[1] = Global_Variables.Calibration_State_Flag.all;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Startup_State_spi_addr: //Startup State (new)
                {
                    tx_buffer[1] = Global_Variables.StartUp_State_Flag.all;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Frequency_MSB_spi_addr: //Frequency MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.FREQ_display.MSB;
                    break;
                }

                case Frequency_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.FREQ_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_Step_CheckSum_spi_addr: //Sequence CheckSum
                {
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Sequence_Run_Step_spi_addr: //Sequence Run Step
                {
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Run_Step;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_State_spi_addr: //Sequence State
                {
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_Set_Step_spi_addr: //Sequence Set Step
                {
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case C2000_Alarm_Status_spi_addr: //C2000 Alarm Status
                {
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case C2000_Alarm_Status_2_spi_addr: //C2000 Alarm Status2
                {
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Temperature_MSB_spi_addr: //Temp MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.MSB;
                    break;
                }

                case Temperature_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.Temp_display.LSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Vpeak_hold_MSB_spi_addr: //Vpeak hold MSB
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.V_peak_hold_display.MSB;
                    break;
                }

                case Vpeak_hold_LSB_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.V_peak_hold_display.LSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Calibration_CheckSum_spi_addr: //Calibration Check sum
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables.CheckSum;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Calibration_Data_Version_spi_addr: //Calibration Data Version
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables.Data_Version;
                    break;
                }

        //20230809 01
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Scale_P_LSB_spi_addr_lr: //ADC_to_V_FB_Scale_P LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_P.LSB;
                    break;
                }

                case ADC_to_V_FB_Scale_P_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_P.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Scale_P_LSB_spi_addr_hr: //ADC_to_V_FB_Scale_P LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.LSB;
                    break;
                }

                case ADC_to_V_FB_Scale_P_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Scale_N_LSB_spi_addr_lr: //ADC_to_V_FB_Scale_N LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_N.LSB;
                    break;
                }

                case ADC_to_V_FB_Scale_N_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Scale_N.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Scale_N_LSB_spi_addr_hr: //ADC_to_V_FB_Scale_N LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.LSB;
                    break;
                }

                case ADC_to_V_FB_Scale_N_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Offset_LSB_spi_addr_lr: //ADC_to_V_FB_Offset LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Offset.LSB;
                    break;
                }

                case ADC_to_V_FB_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_V_FB_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Offset_LSB_spi_addr_hr: //ADC_to_V_FB_Offset LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Offset.LSB;
                    break;
                }

                case ADC_to_V_FB_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_V_FB_Offset.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Scale_P_LSB_spi_addr_lr: //ADC_to_I_FB_Scale_P LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_P.LSB;
                    break;
                }

                case ADC_to_I_FB_Scale_P_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_P.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Scale_P_LSB_spi_addr_hr: //ADC_to_I_FB_Scale_P LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.LSB;
                    break;
                }

                case ADC_to_I_FB_Scale_P_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Scale_N_LSB_spi_addr_lr: //ADC_to_I_FB_Scale_N LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_N.LSB;
                    break;
                }

                case ADC_to_I_FB_Scale_N_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Scale_N.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Scale_N_LSB_spi_addr_hr: //ADC_to_I_FB_Scale_N LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.LSB;
                    break;
                }

                case ADC_to_I_FB_Scale_N_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Offset_LSB_spi_addr_lr: //ADC_to_I_FB_Offset LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Offset.LSB;
                    break;
                }

                case ADC_to_I_FB_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_I_FB_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Offset_LSB_spi_addr_hr: //ADC_to_I_FB_Offset LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Offset.LSB;
                    break;
                }

                case ADC_to_I_FB_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_I_FB_Offset.MSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Cs_mpy_360000_LSB_spi_addr_lr: //Cs_mpy_360000 LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Cs_mpy_360000.LSB;
                    break;
                }

                case Cs_mpy_360000_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Cs_mpy_360000.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case Cs_mpy_360000_LSB_spi_addr_hr: //Cs_mpy_360000 LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Cs_mpy_360000.LSB;
                    break;
                }

                case Cs_mpy_360000_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Cs_mpy_360000.MSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case inv_Rs_LSB_spi_addr_lr: //inv_Rs LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].inv_Rs.LSB;
                    break;
                }

                case inv_Rs_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].inv_Rs.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case inv_Rs_LSB_spi_addr_hr: //inv_Rs LSB
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].inv_Rs.LSB;
                    break;
                }

                case inv_Rs_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].inv_Rs.MSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case VSET_to_DAC_Scale_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Scale.LSB;
                    break;
                }

                case VSET_to_DAC_Scale_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Scale.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case VSET_to_DAC_Scale_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Scale.LSB;
                    break;
                }

                case VSET_to_DAC_Scale_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Scale.MSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case VSET_to_DAC_Offset_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Offset.LSB;
                    break;
                }

                case VSET_to_DAC_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].VSET_to_DAC_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case VSET_to_DAC_Offset_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Offset.LSB;
                    break;
                }

                case VSET_to_DAC_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].VSET_to_DAC_Offset.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_a_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_a.LSB;
                    break;
                }

                case Freq_Compensation_coeff_a_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_a.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_a_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_a.LSB;
                    break;
                }

                case Freq_Compensation_coeff_a_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_a.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_b_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_b.LSB;
                    break;
                }

                case Freq_Compensation_coeff_b_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_b.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_b_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_b.LSB;
                    break;
                }

                case Freq_Compensation_coeff_b_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_b.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_c_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_c.LSB;
                    break;
                }

                case Freq_Compensation_coeff_c_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].Freq_Compensation_coeff_c.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_c_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_c.LSB;
                    break;
                }

                case Freq_Compensation_coeff_c_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].Freq_Compensation_coeff_c.MSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case V_rms_Offset_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].V_rms_Offset.LSB;
                    break;
                }

                case V_rms_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].V_rms_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case V_rms_Offset_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].V_rms_Offset.LSB;
                    break;
                }

                case V_rms_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].V_rms_Offset.MSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case I_rms_Offset_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].I_rms_Offset.LSB;
                    break;
                }

                case I_rms_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].I_rms_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case I_rms_Offset_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].I_rms_Offset.LSB;
                    break;
                }

                case I_rms_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].I_rms_Offset.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_EXTIN_L1_Scale_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_Scale.LSB;
                    break;
                }

                case ADC_to_EXTIN_L1_Scale_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_Scale.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_EXTIN_L1_Scale_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_Scale.LSB;
                    break;
                }

                case ADC_to_EXTIN_L1_Scale_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_Scale.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_EXTIN_L1_offset_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.LSB;
                    break;
                }

                case ADC_to_EXTIN_L1_offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_EXTIN_L1_offset_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.LSB;
                    break;
                }

                case ADC_to_EXTIN_L1_offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case p_V_peak_Offset_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].p_V_peak_Offset.LSB;
                    break;
                }

                case p_V_peak_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].p_V_peak_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case p_V_peak_Offset_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].p_V_peak_Offset.LSB;
                    break;
                }

                case p_V_peak_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].p_V_peak_Offset.MSB;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case n_V_peak_Offset_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].n_V_peak_Offset.LSB;
                    break;
                }

                case n_V_peak_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].n_V_peak_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case n_V_peak_Offset_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].n_V_peak_Offset.LSB;
                    break;
                }

                case n_V_peak_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].n_V_peak_Offset.MSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case p_I_peak_Offset_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].p_I_peak_Offset.LSB;
                    break;
                }

                case p_I_peak_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].p_I_peak_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case p_I_peak_Offset_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].p_I_peak_Offset.LSB;
                    break;
                }

                case p_I_peak_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].p_I_peak_Offset.MSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case n_I_peak_Offset_LSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].n_I_peak_Offset.LSB;
                    break;
                }

                case n_I_peak_Offset_MSB_spi_addr_lr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ LOW_RANGE ].n_I_peak_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case n_I_peak_Offset_LSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].n_I_peak_Offset.LSB;
                    break;
                }

                case n_I_peak_Offset_MSB_spi_addr_hr:
                {
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow2[ HIGH_RANGE ].n_I_peak_Offset.MSB;
                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                default:
                {
                    tx_buffer[1] = 0;
                }
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case 0x07:
        {
            switch( rx_buffer[0] ) //取前面位址
            {

                /*-----------------------------------------------------------------------------------------*/
                case V_and_I_Raw_Data_Counter_spi_addr: //V and I Raw Data Counter
                {
                    tx_buffer[1] = Global_Variables.POWER_DISPLAY_VARIABLES.VI_Raw_Data_Counter;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case External_Input_spi_addr: //External Input
                {
                    tx_buffer[1] = Global_Variables.EXT_INPUT_FLAG.all;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case External_Output_spi_addr: //External Output
                {
                    tx_buffer[1] = Global_Variables.EXT_OUTPUT_FLAG.all;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case External_AD_spi_addr: //External AD
                {
                    tx_buffer[1] = Global_Variables.EXT_INPUT_VARIABLES.EXT_AD_IN;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Wave_Data_Address_Page_spi_addr: //Wave Data Address Page
                {
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case External_Frequency_H_spi_addr: //External Frequency H
                {
                    tx_buffer[1] = Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.MSB;
                    break;
                }

                case External_Frequency_L_spi_addr:
                {
                    tx_buffer[1] = Global_Variables.EXT_INPUT_VARIABLES.EXT_Freq.LSB;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                default:
                {
                    tx_buffer[1] = rx_buffer[0];
                }
            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case 0x08:
        {
            switch( rx_buffer[0] ) //取前面位址
            {

                /*-----------------------------------------------------------------------------------------*/
                case Update_C2000_Setting_spi_addr: //Update C2000 Setting
                {
                    tx_buffer[1] = rx_buffer[0];
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case V_and_I_Raw_Data_Operate_spi_addr: //V and I Raw Data Operate
                {
                    Global_Variables.VI_RAW_OP_CODE.all = rx_buffer[1] & 0x0F;
                    tx_buffer[1] = Global_Variables.VI_RAW_OP_CODE.all & 0x0F;

                    if ( Global_Variables.VI_RAW_OP_CODE.bit.Trig == 1 )
                    {
                        Global_Variables.VI_RAW_OP_CODE.bit.Trig = 0;

                        Event_Module.Event2_Flag.bit.VI_raw_data_trig = 1;
                        Global_Variables.POWER_DISPLAY_VARIABLES.VI_Raw_Data_Counter = 0;

                        // 設定Busy狀態
                        //
                        Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    }

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Print_All_Cali_Variable_spi_addr: //Print_All_Cali_Variable
                {
                    Event_Module.Debug_Event_Flag.bit.Print_All_Cali_Variable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Event_Module.Debug_Event_Flag.bit.Print_All_Cali_Variable & 0x01;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                default:
                {
                    tx_buffer[1] = rx_buffer[0];
                }

            }

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case 0x09:
        case 0x0A:
        {
            Uint16 step, item;

            switch( rx_buffer[0] ) //取前面位址
            {
                /*-----------------------------------------------------------------------------------------*/
                case Output_ON_OFF_spi_addr: //Output ON/OFF
                {
                    //Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = rx_buffer[1] & 0x01;
                    //tx_buffer[1] = Global_Variables.POWER_CONTROL.bit.OUTPUT_ON & 0x01;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user & 0x01;

                    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Fan_Speed_spi_addr: //Fan Speed
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FanSpeed_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FanSpeed_set_user;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Output_Waveform_Set_spi_addr: //Output Waveform Set
                {
                    //2021101 修改為不箝制
                    //if ( ( rx_buffer[1] >= 1 ) && ( rx_buffer[1] <= 19 ) )
                    //{
                    //    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = rx_buffer[1];
                    //}
                    //
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Output_Mode_Set_spi_addr: //Output Mode Set
                {

                    //Global_Variables.OUTPUT_MODE_FLAG.all = rx_buffer[1];
                    //tx_buffer[1] = Global_Variables.OUTPUT_MODE_FLAG.all;

                    Global_Variables.OUTPUT_MODE_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.OUTPUT_MODE_set_user;

                    //
                    // 使用者設定的輸出模式更新進來
                    //
                    Global_Variables.OUTPUT_MODE_FLAG.all = ( Global_Variables.OUTPUT_MODE_FLAG.all & 0x0180 ) + ( Global_Variables.OUTPUT_MODE_set_user & 0x007F );

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Output_Range_Set_spi_addr:
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option_set_user;

                    Event_Module.Event4_Flag.bit.Set_Output_Range_Option = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Ipeak_hold_Clear_spi_addr: //Ipeak hold Clear
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_hold_clear = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_hold_clear & 0x01;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case T_Ipeak_hold_spi_addr: //T Ipeak hold, 0.001sec
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.I_peak_hold_start_time = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.I_peak_hold_start_time;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case I_RMS_Limiter_ON_OFF_spi_addr: //I RMS Limiter ON/OFF
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable & 0x01;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case I_Peak_Limiter_ON_OFF_spi_addr: //I Peak Limiter ON/OFF
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable & 0x01;

                    Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
                    Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Frequency_Set_MSB_spi_addr: //Frequency Set MSB, 0.01Hz
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.MSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.LSB = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_LSB_buf;
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_LSB_buf = 0;


                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.MSB;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_81_Tx_Trig = 1;

                    break;
                }

                case Frequency_Set_LSB_spi_addr: //Frequency Set LSB
                {
                    //Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.LSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_LSB_buf;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Vpeak_hold_Clear_spi_addr: //Vpeak hold Clear
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.V_peak_hold_clear = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.V_peak_hold_clear & 0x01;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case T_Vpeak_hold_spi_addr: //T Vpeak hold, 0.001sec
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_peak_hold_start_time = rx_buffer[1];

                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_peak_hold_start_time;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case External_IO_Control_ON_OFF_spi_addr: //External I/O Control ON/OFF
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Ext_IO_Control_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Ext_IO_Control_Enable & 0x01;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Zero_Change_Phase_ON_OFF_spi_addr: //Zero Change Phase ON/OFF
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Zero_Change_Phase_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Zero_Change_Phase_Enable & 0x01;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Start_Phase_Set_spi_addr: //Start Phase Set, 0.1
                {

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Start_Phase_ON_OFF_spi_addr: //Start Phase ON/OFF
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable & 0x01;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Stop_Phase_Set_spi_addr: //Stop Phase Set, 0.1
                {

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Stop_Phase_ON_OFF_spi_addr: //Stop Phase ON/OFF
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable & 0x01;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Output_relay_Enable_Disable_spi_addr:
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Output_Relay_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Output_Relay_Enable & 0x01;

                    //
                    // 觸發輸出OFF
                    //
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
                    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case V_Measure_Compensation_ON_OFF_spi_addr: //CV_Compensation_Enable
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable & 0x01;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case V_Slew_Rate_spi_addr: //V Slew Rate
                {

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Frequency_Compensation_ON_OFF_spi_addr: //Freq_Compensation_Enable
                {

                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable & 0x01;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case V_Slew_Rate_mode_spi_addr: //V Slew Rate mode
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.V_SlewRate_Mode = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.V_SlewRate_Mode & 0x01;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case SYNC_Frequency_Source_spi_addr: //SYNC Frequency Source
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Sync_freq_Source & 0x01;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Remote_Sense_ON_OFF_spi_addr: //Remote Sense ON/OFF
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Remote_Sense_Enable = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Remote_Sense_Enable & 0x01;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case CPU_State_spi_addr: //CPU State
                {
                    Global_Variables.CPU_State_Flag.all = rx_buffer[1] & 0x03;
                    tx_buffer[1] = Global_Variables.CPU_State_Flag.all & 0x03;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case I_RMS_Limiter_Set_MSB_spi_addr: //I RMS Limiter Set MSB, 0.0001
                {
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.MSB = rx_buffer[1];
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.LSB = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_LSB_buf;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_LSB_buf = 0;

                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow2 = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;

                    tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.MSB;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_82_Tx_Trig = 1;

                    break;
                }

                case I_RMS_Limiter_Set_LSB_spi_addr: //I RMS Limiter Set LSB
                {
                    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.LSB = rx_buffer[1];
                    //tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.LSB;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_LSB_buf;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case p_I_Peak_Limiter_Set_MSB_spi_addr: //I Peak + Limiter Set MSB, 0.0001
                {
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.MSB = rx_buffer[1];
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.LSB = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_LSB_buf;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_LSB_buf = 0;


                    tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.MSB;

                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all;

                    Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_82_Tx_Trig = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;


                    break;
                }

                case p_I_Peak_Limiter_Set_LSB_spi_addr: //I Peak + Limiter Set LSB
                {
                    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.LSB = rx_buffer[1];
                    //tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.LSB;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level_LSB_buf;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case n_I_Peak_Limiter_Set_MSB_spi_addr: //I Peak - Limiter Set MSB, 0.0001
                {
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.MSB = rx_buffer[1];
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.LSB = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_LSB_buf;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_LSB_buf = 0;


                    tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.MSB;

                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all;

                    Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_82_Tx_Trig = 1;

                    break;
                }

                case n_I_Peak_Limiter_Set_LSB_spi_addr: //I Peak - Limiter Set LSB
                {
                    //Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.LSB = rx_buffer[1];
                    //tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.LSB;
                    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level_LSB_buf;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Ext_Gain_Set_MSB_spi_addr: //Ext Gain Set MSB, 0.0001
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.MSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.LSB = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user_LSB_buf;
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user_LSB_buf = 0;


                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.MSB;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_83_Tx_Trig = 1;

                    break;
                }

                case Ext_Gain_Set_LSB_spi_addr: //Ext Gain Set LSB
                {
                    //Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.LSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user_LSB_buf;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case DC_Voltage_Set_MSB_spi_addr: //DC Voltage Set MSB, 0.0001
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.MSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.LSB = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_LSB_buf;
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_LSB_buf = 0;


                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.MSB;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_81_Tx_Trig = 1;

                    break;
                }

                case DC_Voltage_Set_LSB_spi_addr: //DC Voltage Set LSB
                {
                    //Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.LSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_LSB_buf;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case AC_Voltage_Set_MSB_spi_addr: //AC Voltage Set MSB, 0.0001
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.MSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.LSB = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_LSB_buf;
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_LSB_buf = 0;


                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.MSB;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_81_Tx_Trig = 1;

                    break;
                }

                case AC_Voltage_Set_LSB_spi_addr: //AC Voltage Set LSB
                {
                    //Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.LSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_LSB_buf;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case VCA_Gain_Set_MSB_spi_addr: //VCA Gain Set MSB, 0.0001
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.MSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.LSB = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user_LSB_buf;
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user_LSB_buf = 0;

                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.MSB;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_83_Tx_Trig = 1;

                    break;
                }

                case VCA_Gain_Set_LSB_spi_addr: //VCA Gain Set LSB
                {
                    //Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.LSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user_LSB_buf;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case SYNC_Mode_Phase_Delay_spi_addr: //SYNC Phase Delay Set, 0.1
                {

                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user;

                    Event_Module.Event2_Flag.bit.SYNC_Mode_Change_Phase_Delay = 1;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_83_Tx_Trig = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Zero_Cross_Width_Set_spi_addr: //Zero_Cross_Width_Set 100us
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Width_Set = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Width_Set;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Zero_Cross_Option_spi_addr:
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Option.all = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Option.all;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case ACV_Unit_spi_addr: //ACV Unit
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit & 0x01;

                    //
                    // 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    //MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Calibration_Step_spi_addr: //Calibration Step
                {
                    Global_Variables.Calibration_Step_Flag.all = rx_buffer[1] & 0x03FF;
                    tx_buffer[1] = Global_Variables.Calibration_Step_Flag.all & 0x03FF;

                    Global_Variables.Calibration_State_Flag.all = 0; //Running

                    Event_Module.Event_Flag.bit.Set_Cali_Step = 1; //觸發事件

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Calibration_Value_to_C2000_MSB_spi_addr: //Calibration Value to C2000 MSB
                {
                    step = Global_Variables.Calibration_Step_Flag.bit.Step;
                    item = Global_Variables.Calibration_Step_Flag.bit.Item;

                    Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[item][step].MSB = rx_buffer[1];
                    Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[item][step].LSB = Calibration_Module_Variables.Calibration_process_Variables.Meter_Value_LSB_buf[item][step];
                    Calibration_Module_Variables.Calibration_process_Variables.Meter_Value_LSB_buf[item][step] = 0;

                    tx_buffer[1] = Calibration_Module_Variables.Calibration_process_Variables.Meter_Value[item][step].MSB;

                    //抓AD值
                    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_Value[item][step] = (Uint16)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHA_avg_display.all;
                    Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_Value[item][step] = (Uint16)Calibration_Module_Variables.Calibration_process_Variables.ADS8353_CHB_avg_display.all;
                    //Global_Variables.Calibration_Variables.ADS8353_CHA_Value[item][step] = CLA_to_CPU_Variables.ADS8353_CHA;
                    //Global_Variables.Calibration_Variables.ADS8353_CHB_Value[item][step] = CLA_to_CPU_Variables.ADS8353_CHB;

                    //抓DA值
                    //Global_Variables.Calibration_Variables.REF_DAC_Value[item][step] = Global_Variables.OUTPUT_VARIABLES.VREF_set;
                    Calibration_Module_Variables.Calibration_process_Variables.DCV_set[item][step] = (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all * 0.0001;


                    //更新peak值
                    Calibration_Module_Variables.Calibration_process_Variables.p_V_peak_Value[item][step] = -Global_Variables.POWER_DISPLAY_VARIABLES.p_V_peak_display.all;
                    Calibration_Module_Variables.Calibration_process_Variables.n_V_peak_Value[item][step] = -Global_Variables.POWER_DISPLAY_VARIABLES.n_V_peak_display.all;
                    Calibration_Module_Variables.Calibration_process_Variables.p_I_peak_Value[item][step] = -Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display.all;
                    Calibration_Module_Variables.Calibration_process_Variables.n_I_peak_Value[item][step] = -Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display.all;


                    //儲存DAC值
                    //儲存輸出設定值
                    //TBD

                    Global_Variables.Calibration_State_Flag.all = 0; //Running

                    Event_Module.Event_Flag.bit.Set_Cali_Meter_Value = 1; //觸發事件

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;


                    break;
                }

                case Calibration_Value_to_C2000_LSB_spi_addr: //Calibration Value to C2000 LSB
                {
                    step = Global_Variables.Calibration_Step_Flag.bit.Step;
                    item = Global_Variables.Calibration_Step_Flag.bit.Item;

                    //Global_Variables.Calibration_Variables.Meter_Value[item][step].LSB = rx_buffer[1];
                    //tx_buffer[1] = Global_Variables.Calibration_Variables.Meter_Value[item][step].LSB;
                    Calibration_Module_Variables.Calibration_process_Variables.Meter_Value_LSB_buf[item][step] = rx_buffer[1];
                    tx_buffer[1] = Calibration_Module_Variables.Calibration_process_Variables.Meter_Value_LSB_buf[item][step];

                    Global_Variables.Calibration_State_Flag.all = 0; //Running

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Calibration_Recall_default_spi_addr: //Calibration Recall default
                {
                    Event_Module.Event_Flag.bit.Set_Cali_Variable_default = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Event_Module.Event_Flag.bit.Set_Cali_Variable_default & 0x01;

                    //直接設定觸發事件

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Alarm_Clear_spi_addr: //Alarm Clear
                {
                    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset & 0x01;

                    MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Reset_Trig = 1;
                    MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Model_Type_spi_addr: //Model Type
                {
                    // 改變機種則觸發事件
                    if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series != rx_buffer[1] )
                    {
                        Event_Module.Event_Flag.bit.Model_Type_Change = 1;

                        // 設定Busy狀態
                        //
                        Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    }

                    Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series = rx_buffer[1];

                    tx_buffer[1] = Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Calibration_Cancel_spi_addr: //Calibration Cancel
                {
                    Event_Module.Event_Flag.bit.Cancel_Cali_Process = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Event_Module.Event_Flag.bit.Cancel_Cali_Process & 0x01;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //直接設定觸發事件

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Calibration_Save_spi_addr: //Calibration Save
                {
                    Event_Module.Event4_Flag.bit.Save_HL_Cali_Variable_to_EEPROM = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Event_Module.Event4_Flag.bit.Save_HL_Cali_Variable_to_EEPROM & 0x01;


                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    //直接設定觸發事件

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Wave_Data_Address_Page_spi_addr_w: //Wave Data Address Page
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_Step_spi_addr: //SEQ_Current_Set_Step
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_Config_State_1_spi_addr: //SEQ Step Config1
                {
                    SEQ_Step_Section1.SEQ_Config_State1_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].all = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section1.SEQ_Config_State1_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].all;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_Config_State_2_spi_addr: //SEQ Step Config2
                {
                    SEQ_Step_Section1.SEQ_Config_State2_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].all = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section1.SEQ_Config_State2_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].all;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Sequence_star_phase_spi_addr: // SEQ Step Start Phase
                {
                    SEQ_Step_Section1.SEQ_Start_Phase_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ] = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section1.SEQ_Start_Phase_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_stop_phase_spi_addr: // SEQ Step Stop Phase
                {
                    SEQ_Step_Section1.SEQ_Stop_Phase_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ] = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section1.SEQ_Stop_Phase_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_jump_spi_addr: // SEQ Step Jump to
                {
                    SEQ_Step_Section2.SEQ_Jump_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ] = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section2.SEQ_Jump_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_jump_count_spi_addr: // SEQ Step Jump loop count
                {
                    SEQ_Step_Section2.SEQ_Jump_Count_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ] = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section2.SEQ_Jump_Count_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Sequence_branch_step_1_spi_addr: // SEQ Step Branch1
                {
                    SEQ_Step_Section2.SEQ_Branch1_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ] = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section2.SEQ_Branch1_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_branch_step_2_spi_addr: // SEQ Step Branch2
                {
                    SEQ_Step_Section2.SEQ_Branch2_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ] = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section2.SEQ_Branch2_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Step duration time
                //
                case Sequence_step_time_MSB_spi_addr: //MSB
                {
                    SEQ_Step_Section3.SEQ_Step_Time_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB = rx_buffer[1];
                    SEQ_Step_Section3.SEQ_Step_Time_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].LSB = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Step_Time_set_user_LSB_buf;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Step_Time_set_user_LSB_buf = 0;

                    tx_buffer[1] = SEQ_Step_Section3.SEQ_Step_Time_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                case Sequence_step_time_LSB_spi_addr: //LSB
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Step_Time_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Step_Time_set_user_LSB_buf;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_start_spi_addr: // SEQ Start
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Start;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_stop_spi_addr: // SEQ Stop
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Stop = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Stop;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Sequence_hold_spi_addr: // SEQ Hold
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Hold = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Hold;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Sequence_branch_1_spi_addr: // SEQ Branch1
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1 = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch1;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_branch_2_spi_addr: // SEQ Branch2
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2 = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.Control_flags.bit.Branch2;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_Mode_spi_addr: // SEQ Output Mode
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user.all = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.Output_Mode_set_user.all;
                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Clear_Sequence_CheckSum_spi_addr: //Clear SEQ CheckSum
                {
                    //Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_CheckSum = rx_buffer[1] & 0x01;
                    //tx_buffer[1] = Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_CheckSum;

                    if ( rx_buffer[1] & 0x01 )
                    {
                        Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum = 0;

                        tx_buffer[1] = 1;
                    }

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case SEQ_CheckSum_Refresh_spi_addr: // SEQ CheckSum Refresh
                {
                    Event_Module.Event4_Flag.bit.SEQ_Mode_Refresh_CheckSum = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Event_Module.Event4_Flag.bit.SEQ_Mode_Refresh_CheckSum;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Sequence_SQU_Duty_spi_addr: // SEQ SQU Duty
                {
                    SEQ_Step_Section5.SEQ_SQU_Duty_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ] = rx_buffer[1];
                    tx_buffer[1] = SEQ_Step_Section5.SEQ_SQU_Duty_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ];

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Sequence_Step_Clear_spi_addr: // SEQ Clear all step
                {
                    Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_All_Step = rx_buffer[1] & 0x01;
                    tx_buffer[1] = Event_Module.Event2_Flag.bit.SEQ_Mode_Clear_All_Step;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Step Freq
                //
                case Sequence_Freq_Set_MSB_spi_addr: //MSB
                {
                    SEQ_Step_Section3.SEQ_Freq_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB = rx_buffer[1];
                    SEQ_Step_Section3.SEQ_Freq_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].LSB = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Freq_set_user_LSB_buf;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Freq_set_user_LSB_buf = 0;

                    tx_buffer[1] = SEQ_Step_Section3.SEQ_Freq_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                case Sequence_Freq_Set_LSB_spi_addr: //LSB
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Freq_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Freq_set_user_LSB_buf;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }



                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Step DCV
                //
                case Sequence_dc_value_MSB_spi_addr: //MSB
                {
                    SEQ_Step_Section4.SEQ_DCV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB = rx_buffer[1];
                    SEQ_Step_Section4.SEQ_DCV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].LSB = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_DCV_set_user_LSB_buf;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_DCV_set_user_LSB_buf = 0;

                    tx_buffer[1] = SEQ_Step_Section4.SEQ_DCV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                case Sequence_dc_value_LSB_spi_addr: //LSB
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_DCV_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_DCV_set_user_LSB_buf;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                //
                // SEQ Step ACV
                //
                case Sequence_ac_value_MSB_spi_addr: //MSB
                {
                    SEQ_Step_Section4.SEQ_ACV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB = rx_buffer[1];
                    SEQ_Step_Section4.SEQ_ACV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].LSB = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_ACV_set_user_LSB_buf;
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_ACV_set_user_LSB_buf = 0;

                    tx_buffer[1] = SEQ_Step_Section4.SEQ_ACV_set_user[ Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_Set_Step ].MSB;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                case Sequence_ac_value_LSB_spi_addr: //LSB
                {
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_ACV_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_ACV_set_user_LSB_buf;

                    //
                    // SEQ CheckSum
                    //
                    #ifdef DEVELOP_SV78
                    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_CheckSum += rx_buffer[1];
                    #endif

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case SQU_Duty_Set_spi_addr:
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case V_Rise_Time_Set_MSB_spi_addr:
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user.MSB = rx_buffer[1];
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user.LSB = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user_LSB_buf;
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user_LSB_buf = 0;

                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user.MSB;


                    break;
                }

                case V_Rise_Time_Set_LSB_spi_addr:
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user_LSB_buf = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_Rise_Time_set_user_LSB_buf;
                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB1_spi_addr: //Waveform CF Set LSB [ARB1]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB1].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB1].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB1_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB1].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB1].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB2_spi_addr: //Waveform CF Set LSB [ARB2]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB2].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB2].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB2_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB2].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB2].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB3_spi_addr: //Waveform CF Set LSB [ARB3]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB3].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB3].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB3_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB3].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB3].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB4_spi_addr: //Waveform CF Set LSB [ARB4]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB4].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB4].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB4_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB4].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB4].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB5_spi_addr: //Waveform CF Set LSB [ARB5]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB5].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB5].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB5_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB5].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB5].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB6_spi_addr: //Waveform CF Set LSB [ARB6]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB6].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB6].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB6_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB6].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB6].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB7_spi_addr: //Waveform CF Set LSB [ARB7]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB7].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB7].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB7_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB7].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB7].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB8_spi_addr: //Waveform CF Set LSB [ARB8]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB8].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB8].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB8_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB8].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB8].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB9_spi_addr: //Waveform CF Set LSB [ARB9]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB9].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB9].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB9_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB9].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB9].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB10_spi_addr: //Waveform CF Set LSB [ARB10]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB10].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB10].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB10_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB10].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB10].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB11_spi_addr: //Waveform CF Set LSB [ARB11]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB11].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB11].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB11_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB11].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB11].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB12_spi_addr: //Waveform CF Set LSB [ARB12]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB12].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB12].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB12_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB12].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB12].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB13_spi_addr: //Waveform CF Set LSB [ARB13]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB13].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB13].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB13_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB13].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB13].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB14_spi_addr: //Waveform CF Set LSB [ARB14]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB14].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB14].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB14_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB14].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB14].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB15_spi_addr: //Waveform CF Set LSB [ARB15]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB15].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB15].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB15_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB15].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB15].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_ARB16_spi_addr: //Waveform CF Set LSB [ARB16]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB16].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB16].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_ARB16_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB16].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_ARB16].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_SIN_spi_addr: //Waveform CF Set LSB [SIN]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SIN].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SIN].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_SIN_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SIN].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SIN].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_SQU_spi_addr: //Waveform CF Set LSB [SQU]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SQU].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SQU].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_SQU_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SQU].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SQU].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Waveform_CF_Set_LSB_TRI_spi_addr: //Waveform CF Set LSB [TRI]
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_TRI].LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_TRI].LSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }

                case Waveform_CF_Set_MSB_TRI_spi_addr: //MSB
                {
                    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_TRI].MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_TRI].MSB;

                    //
                    // 改變CF值, 觸發事件
                    //
                    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

                    break;
                }



        //20230809 03

                /*-----------------------------------------------------------------------------------------*/
                case Calibration_CheckSum_spi_addr_w: //Calibration Check sum
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].CheckSum = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].CheckSum;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Calibration_Data_Version_spi_addr_w: //Calibration Data Version
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Data_Version = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Data_Version;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Scale_P_LSB_spi_addr_w_lr: //ADC_to_V_FB_Scale_P LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Scale_P.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Scale_P.LSB;

                    break;
                }

                case ADC_to_V_FB_Scale_P_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Scale_P.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Scale_P.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Scale_P_LSB_spi_addr_w_hr: //ADC_to_V_FB_Scale_P LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.LSB;

                    break;
                }

                case ADC_to_V_FB_Scale_P_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Scale_P.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Scale_N_LSB_spi_addr_w_lr: //ADC_to_V_FB_Scale_N LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Scale_N.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Scale_N.LSB;

                    break;
                }

                case ADC_to_V_FB_Scale_N_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Scale_N.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Scale_N.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Scale_N_LSB_spi_addr_w_hr: //ADC_to_V_FB_Scale_N LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.LSB;

                    break;
                }

                case ADC_to_V_FB_Scale_N_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Scale_N.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Offset_LSB_spi_addr_w_lr: //ADC_to_V_FB_Offset LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Offset.LSB;

                    break;
                }

                case ADC_to_V_FB_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_V_FB_Offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_V_FB_Offset_LSB_spi_addr_w_hr: //ADC_to_V_FB_Offset LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Offset.LSB;

                    break;
                }

                case ADC_to_V_FB_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_V_FB_Offset.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Scale_P_LSB_spi_addr_w_lr: //ADC_to_I_FB_Scale_P LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Scale_P.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Scale_P.LSB;

                    break;
                }

                case ADC_to_I_FB_Scale_P_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Scale_P.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Scale_P.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Scale_P_LSB_spi_addr_w_hr: //ADC_to_I_FB_Scale_P LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.LSB;

                    break;
                }

                case ADC_to_I_FB_Scale_P_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Scale_P.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Scale_N_LSB_spi_addr_w_lr: //ADC_to_I_FB_Scale_N LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Scale_N.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Scale_N.LSB;

                    break;
                }

                case ADC_to_I_FB_Scale_N_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Scale_N.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Scale_N.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Scale_N_LSB_spi_addr_w_hr: //ADC_to_I_FB_Scale_N LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.LSB;

                    break;
                }

                case ADC_to_I_FB_Scale_N_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Scale_N.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Offset_LSB_spi_addr_w_lr: //ADC_to_I_FB_Offset LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Offset.LSB;

                    break;
                }

                case ADC_to_I_FB_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_I_FB_Offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_I_FB_Offset_LSB_spi_addr_w_hr: //ADC_to_I_FB_Offset LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Offset.LSB;

                    break;
                }

                case ADC_to_I_FB_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_I_FB_Offset.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case Cs_mpy_360000_LSB_spi_addr_w_lr: //Cs_mpy_360000 LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Cs_mpy_360000.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Cs_mpy_360000.LSB;

                    break;
                }

                case Cs_mpy_360000_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Cs_mpy_360000.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Cs_mpy_360000.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case Cs_mpy_360000_LSB_spi_addr_w_hr: //Cs_mpy_360000 LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Cs_mpy_360000.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Cs_mpy_360000.LSB;

                    break;
                }

                case Cs_mpy_360000_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Cs_mpy_360000.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Cs_mpy_360000.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case inv_Rs_LSB_spi_addr_w_lr: //inv_Rs LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].inv_Rs.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].inv_Rs.LSB;

                    break;
                }

                case inv_Rs_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].inv_Rs.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].inv_Rs.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case inv_Rs_LSB_spi_addr_w_hr: //inv_Rs LSB
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].inv_Rs.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].inv_Rs.LSB;

                    break;
                }

                case inv_Rs_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].inv_Rs.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].inv_Rs.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case VSET_to_DAC_Scale_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].VSET_to_DAC_Scale.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].VSET_to_DAC_Scale.LSB;

                    break;
                }

                case VSET_to_DAC_Scale_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].VSET_to_DAC_Scale.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].VSET_to_DAC_Scale.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case VSET_to_DAC_Scale_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].VSET_to_DAC_Scale.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].VSET_to_DAC_Scale.LSB;

                    break;
                }

                case VSET_to_DAC_Scale_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].VSET_to_DAC_Scale.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].VSET_to_DAC_Scale.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case VSET_to_DAC_Offset_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].VSET_to_DAC_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].VSET_to_DAC_Offset.LSB;

                    break;
                }

                case VSET_to_DAC_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].VSET_to_DAC_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].VSET_to_DAC_Offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case VSET_to_DAC_Offset_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].VSET_to_DAC_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].VSET_to_DAC_Offset.LSB;

                    break;
                }

                case VSET_to_DAC_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].VSET_to_DAC_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].VSET_to_DAC_Offset.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_a_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_a.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_a.LSB;

                    break;
                }

                case Freq_Compensation_coeff_a_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_a.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_a.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_a_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_a.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_a.LSB;

                    break;
                }

                case Freq_Compensation_coeff_a_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_a.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_a.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_b_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_b.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_b.LSB;

                    break;
                }

                case Freq_Compensation_coeff_b_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_b.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_b.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_b_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_b.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_b.LSB;

                    break;
                }

                case Freq_Compensation_coeff_b_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_b.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_b.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_c_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_c.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_c.LSB;

                    break;
                }

                case Freq_Compensation_coeff_c_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_c.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].Freq_Compensation_coeff_c.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case Freq_Compensation_coeff_c_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_c.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_c.LSB;

                    break;
                }

                case Freq_Compensation_coeff_c_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_c.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].Freq_Compensation_coeff_c.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case V_rms_Offset_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].V_rms_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].V_rms_Offset.LSB;

                    break;
                }

                case V_rms_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].V_rms_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].V_rms_Offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case V_rms_Offset_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].V_rms_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].V_rms_Offset.LSB;

                    break;
                }

                case V_rms_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].V_rms_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].V_rms_Offset.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case I_rms_Offset_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].I_rms_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].I_rms_Offset.LSB;

                    break;
                }

                case I_rms_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].I_rms_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].I_rms_Offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case I_rms_Offset_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].I_rms_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].I_rms_Offset.LSB;

                    break;
                }

                case I_rms_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].I_rms_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].I_rms_Offset.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_EXTIN_L1_Scale_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_EXTIN_L1_Scale.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_EXTIN_L1_Scale.LSB;

                    break;
                }

                case ADC_to_EXTIN_L1_Scale_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_EXTIN_L1_Scale.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_EXTIN_L1_Scale.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_EXTIN_L1_Scale_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_EXTIN_L1_Scale.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_EXTIN_L1_Scale.LSB;

                    break;
                }

                case ADC_to_EXTIN_L1_Scale_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_EXTIN_L1_Scale.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_EXTIN_L1_Scale.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_EXTIN_L1_offset_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.LSB;

                    break;
                }

                case ADC_to_EXTIN_L1_offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].ADC_to_EXTIN_L1_offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case ADC_to_EXTIN_L1_offset_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.LSB;

                    break;
                }

                case ADC_to_EXTIN_L1_offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].ADC_to_EXTIN_L1_offset.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case p_V_peak_Offset_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].p_V_peak_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].p_V_peak_Offset.LSB;

                    break;
                }

                case p_V_peak_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].p_V_peak_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].p_V_peak_Offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case p_V_peak_Offset_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].p_V_peak_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].p_V_peak_Offset.LSB;

                    break;
                }

                case p_V_peak_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].p_V_peak_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].p_V_peak_Offset.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case n_V_peak_Offset_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].n_V_peak_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].n_V_peak_Offset.LSB;

                    break;
                }

                case n_V_peak_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].n_V_peak_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].n_V_peak_Offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case n_V_peak_Offset_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].n_V_peak_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].n_V_peak_Offset.LSB;

                    break;
                }

                case n_V_peak_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].n_V_peak_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].n_V_peak_Offset.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case p_I_peak_Offset_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].p_I_peak_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].p_I_peak_Offset.LSB;

                    break;
                }

                case p_I_peak_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].p_I_peak_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].p_I_peak_Offset.MSB;

                    break;
                }
                /*-----------------------------------------------------------------------------------------*/
                case p_I_peak_Offset_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].p_I_peak_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].p_I_peak_Offset.LSB;

                    break;
                }

                case p_I_peak_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].p_I_peak_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].p_I_peak_Offset.MSB;

                    break;
                }


                /*-----------------------------------------------------------------------------------------*/
                case n_I_peak_Offset_LSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].n_I_peak_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].n_I_peak_Offset.LSB;

                    break;
                }

                case n_I_peak_Offset_MSB_spi_addr_w_lr:
                {
                    Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].n_I_peak_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ LOW_RANGE ].n_I_peak_Offset.MSB;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                case n_I_peak_Offset_LSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].n_I_peak_Offset.LSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].n_I_peak_Offset.LSB;

                    break;
                }

                case n_I_peak_Offset_MSB_spi_addr_w_hr:
                {
                    Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].n_I_peak_Offset.MSB = rx_buffer[1];
                    tx_buffer[1] = Global_Variables.Calibration_Variables_shadow[ HIGH_RANGE ].n_I_peak_Offset.MSB;

                    //
                    // 觸發檢查Checksum, 檢查ok才會真的寫入
                    //
                    Event_Module.Event2_Flag.bit.Check_Cali_CheckSum = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

                    break;
                }

                /*-----------------------------------------------------------------------------------------*/
                default:
                {
                    tx_buffer[1] = rx_buffer[1];
                }

            }

            // 設定Busy狀態
            //
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;


            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        {
            Uint16 tmp;

            tmp = rx_buffer[0] - V_Raw_Data_spi_addr;
            tx_buffer[1] = V_RAW.local_ram[tmp];
            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        {
            Uint16 tmp;

            tmp = rx_buffer[0] - I_Raw_Data_spi_addr;
            tx_buffer[1] = I_RAW.local_ram[tmp];
            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F:
        {
            Uint32 offset, page, CheckSum_page;

            //計算offset
            offset = (Uint32)rx_buffer[0] - Wave_Data_spi_addr;

            //載入wave page
            page = (Uint32)( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page + 1 ) << 12;
            CheckSum_page = page + 0x14000;

            *( SDRAM_variables.SDRAMBuf + page + offset ) = rx_buffer[1]; //7 cycles

            //Checksum
            *( SDRAM_variables.SDRAMBuf + CheckSum_page + offset ) = ( (int16)rx_buffer[1] >> 8 ) + (int16)rx_buffer[1]; // 11cycles

            tx_buffer[1] = *( SDRAM_variables.SDRAMBuf + page + offset );

            //
            // 判斷最後一筆資料, 觸發把SDRAM預載入RAM的事件
            //
            if ( ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page == 18 ) && ( rx_buffer[0] == 0x3FFF ) )
            {
                Event_Module.Event2_Flag.bit.Preload_SQU_WAVE_DATA = 1;
                Event_Module.Event2_Flag.bit.Preload_WAVE_DATA = 1;

                // 設定Busy狀態
                //
                Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            }
            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        default:
        {
            //
            // 保留此寫法, 方便有問題的時候才好查找
            //
            tx_buffer[1] = rx_buffer[0];
        }
    }
}










//
// End of file
//
