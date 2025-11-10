//###########################################################################
//
// FILE:    Main_State_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211014
//###########################################################################



#include "Global_VariableDefs.h"


volatile MAIN_STATE_MODULE_VARIABLES_REG Main_State_Module;



static inline Power_Up_Process(void);
static inline Self_test_Process(void);
static inline Ready0_Process(void);
static inline Ready_Process(void);
static inline Output0_Process(void);
static inline Output_Process(void);
static inline Error_Handle0_Process(void);
static inline Error_Handle1_Process(void);
static inline Error_Handle2_Process(void);
static inline Error_Process(void);




/////////////////////////////////////////////////////////////////////
//
// 變數初始化
//
void Init_Main_State_Module_Variables(void)
{
    Main_State_Module.Machine_state = POWER_UP_STATE;
    Main_State_Module.Power_up_state = start_state;
    Main_State_Module.Self_test_state = sd_ram_check_state;

    Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.all = 0;

    Main_State_Module.Power_Up_start_time = CpuTimer2.InterruptCount;

    Main_State_Module.fan_Check_start_time = 0;
    Main_State_Module.ac_on_Check_start_time = 0;
    Main_State_Module.ac_on_Check2_start_time = 0;
    Main_State_Module.pfc_Check_start_time = 0;
    Main_State_Module.pfc_Check2_start_time = 0;
    Main_State_Module.dcac_Check_start_time = 0;
    Main_State_Module.dcdc_Check_start_time = 0;
    Main_State_Module.error_start_time = 0;
    Main_State_Module.output_start_time = 0;
    Main_State_Module.output_stop_time = 0;

    Main_State_Module.Self_Test_AutoReset_Counter = SELF_TEST_AUTO_RESET_COUNT;

    Main_State_Module.main_loop_schedule01_state = MAIN_LOOP_SCHEDULE01_01_STATE;
    Main_State_Module.main_loop_schedule02_state = MAIN_LOOP_SCHEDULE02_01_STATE;

    Main_State_Module.Machine_Busy_State = MACHINE_BUSY_STATE_0;

    Main_State_Module.Change_Output_Range_State = CHANGE_OUTPUT_RANGE_STATE_0;

}


/////////////////////////////////////////////////////////////////////
//
// 輸出VREF及WAVE初始化
//
void Init_VREF_WAVE_Process(void)
{
    Global_Variables.OUTPUT_VARIABLES.VREF_set = (Uint16)Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
    Global_Variables.OUTPUT_VARIABLES.VREF_set_shadow = (Uint16)Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;

    Global_Variables.OUTPUT_VARIABLES.WAVE_set = WAVE_DAC_ZERO;
    Global_Variables.OUTPUT_VARIABLES.WAVE_offset = 0;
    Global_Variables.OUTPUT_VARIABLES.WAVE_offset_shadow = 0;
    Global_Variables.OUTPUT_VARIABLES.WAVE_scale = 1;
    Global_Variables.OUTPUT_VARIABLES.WAVE_scale_shadow = 1;
    Global_Variables.OUTPUT_VARIABLES.WAVE_set_goal = WAVE_DAC_ZERO;

    Global_Variables.OUTPUT_VARIABLES.ext_WAVE_scale = 1;
    Global_Variables.OUTPUT_VARIABLES.ext_WAVE_scale_shadow = 1;

}





/////////////////////////////////////////////////////////////////////
//
// 主流程
//
void Main_Process(void)
{

    switch(Main_State_Module.Machine_state)
    {

        /*-----------------------------------------------------------------------------------------*/
        case POWER_UP_STATE:
        {

            Power_Up_Error_Scan();

            Power_Up_Process();

            if ( Main_State_Module.Power_up_state == power_up_ready_state )
            {
                FANFAIL1_start_time_reset();
                FANFAIL2_start_time_reset();
                PFC_UVP_start_time_reset();
                PFC_OVP_start_time_reset();
                PFC_OTP_start_time_reset();
                ACFAIL_start_time_reset();
                DCAC_OTP_start_time_reset();
                p_I_peak_ocp_start_time_reset();
                n_I_peak_ocp_start_time_reset();
                I_rms_ocp_start_time_reset();
                V_rms_ovp_start_time_reset();
                opp_start_time_reset();
                remote_OVP_start_time_reset();

                FANFAIL_start_time_reset(); //for 全寬機種
                PFC_UVP1_start_time_reset(); //for 全寬機種
                PFC_OVP1_start_time_reset(); //for 全寬機種
                PFC_UVP2_start_time_reset(); //for 全寬機種
                PFC_OVP2_start_time_reset(); //for 全寬機種

                DC_OVP_start_time_reset();

                //
                // 轉態
                //
                Main_State_Module.Machine_state = SELF_TEST_STATE;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> SELF_TEST_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case SELF_TEST_STATE:
        {
            Self_test_Process();

            if ( Main_State_Module.Self_test_state == self_test_ready )
            {
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;
                Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;

                FANFAIL1_start_time_reset();
                FANFAIL2_start_time_reset();
                PFC_UVP_start_time_reset();
                PFC_OVP_start_time_reset();
                PFC_OTP_start_time_reset();
                ACFAIL_start_time_reset();
                DCAC_OTP_start_time_reset();
                p_I_peak_ocp_start_time_reset();
                n_I_peak_ocp_start_time_reset();
                I_rms_ocp_start_time_reset();
                V_rms_ovp_start_time_reset();
                opp_start_time_reset();
                remote_OVP_start_time_reset();

                FANFAIL_start_time_reset(); //for 全寬機種
                PFC_UVP1_start_time_reset(); //for 全寬機種
                PFC_OVP1_start_time_reset(); //for 全寬機種
                PFC_UVP2_start_time_reset(); //for 全寬機種
                PFC_OVP2_start_time_reset(); //for 全寬機種

                DC_OVP_start_time_reset();

                //
                // 轉態
                //
                Main_State_Module.Machine_state = READY_STATE0;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> READY_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        //
        // 進 READY_STATE 前的初始化
        //
        case READY_STATE0:
        {
            Ready0_Process();

            //
            // 轉態
            //
            Main_State_Module.Machine_state = READY_STATE;

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> READY_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case READY_STATE:
        {
            Ready_Process();

            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 1 )
            {
                //
                // 轉態
                //
                Main_State_Module.Machine_state = OUTPUT_STATE0;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> OUTPUT_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case OUTPUT_STATE0:
        {
            Output0_Process();

            //
            // 轉態
            //
            Main_State_Module.Machine_state = OUTPUT_STATE; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> OUTPUT_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
        /*-----------------------------------------------------------------------------------------*/
        case OUTPUT_STATE:
        {
            Output_Process();

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ERROR_HANDLE_STATE0:
        {
            Error_Handle0_Process();

            //
            // 轉態
            //
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE1;

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE1\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ERROR_HANDLE_STATE1:
        {
            Error_Handle1_Process();

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ERROR_HANDLE_STATE2:
        {
            Error_Handle2_Process();

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ERROR_STATE:
        {
            Error_Process();

            break;
        }
    }
}


/////////////////////////////////////////////////////////////////////
//
// 自我測試流程
//
static inline Self_test_Process(void)
{
    switch(Main_State_Module.Self_test_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        case sd_ram_check_state:
        {
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 0;

            Global_Variables.StartUp_State_Flag.bit.SDRAM_Check = 0;
            Global_Variables.StartUp_State_Flag.bit.EEPROM_Check = 0;

            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Self_test_state -> sd_ram_check_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            /*------------------------------------------------*/
            //
            // SDRAM Self-Test and Clear
            //

            if (SDRAM_Self_Test_V2()) //1,847,863,356 cycles 差不多10秒
            {

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 1; //NG

                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = SDRAM_SELF_TEST_V2_NG;

                Global_Variables.StartUp_State_Flag.bit.SDRAM_Check = 2;

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***SDRAM_Error_SW***, SDRAM_SELF_TEST_V2_NG\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                //轉態
                Main_State_Module.Self_test_state = self_test_error_state;

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Self_test_state -> self_test_error_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
            else
            {

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 0; //PASS

                Global_Variables.StartUp_State_Flag.bit.SDRAM_Check = 1;


                #ifdef BYPASS_LOAD_AND_CHECK_EEPROM
                //
                // Bypass EEPROM Load and check
                //
                Global_Variables.StartUp_State_Flag.bit.EEPROM_Check = 1;

                //轉態
                Main_State_Module.Self_test_state = self_test_ready;

                #else
                //轉態
                Main_State_Module.Self_test_state = eeprom_load_state;
                #endif

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Self_test_state -> eeprom_load_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            Local_RAM_and_SDRAM_Clear();



            #ifdef BYPASS_GEN_WAVE_TO_SDRAM
            Generate_Tri_wave_data_process();
            //Generate_Square_wave_data_process();
            Generate_Sine_wave_data_process();
            Event_Module.Event2_Flag.bit.Preload_SQU_WAVE_DATA = 1;
            Event_Module.Event2_Flag.bit.Preload_WAVE_DATA = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Generate_Tri_wave_data_process()\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Generate_Square_wave_data_process()\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Generate_Sine_wave_data_process()\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            #endif



            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case eeprom_load_state:
        {
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 0;


            Load_HL_Cali_Variable_from_EEPROM_Event();
            Load_HL_Cali_to_Cali_Event();

            //
            // 觸發校正Checksum計算事件
            //
            Update_HL_Cali_CheckSum_Event();


            if ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW == 1 )
            {
                Global_Variables.StartUp_State_Flag.bit.EEPROM_Check = 2;

                //轉態
                Main_State_Module.Self_test_state = self_test_error_state;

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Self_test_state -> self_test_error_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
            else
            {
                Global_Variables.StartUp_State_Flag.bit.EEPROM_Check = 1;


                //轉態
                Main_State_Module.Self_test_state = self_test_ready;

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Self_test_state -> self_test_ready\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case self_test_ready:
        {

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case self_test_error_state:
        {
            if ( Main_State_Module.Self_Test_AutoReset_Counter > 0 )
            {
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 1;

                Main_State_Module.Self_Test_AutoReset_Counter--;

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Self_Test_AutoReset_Counter %u\n", CpuTimer2.InterruptCount, Main_State_Module.Self_Test_AutoReset_Counter ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
            }

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset == 1 ) //Reset
            {
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 0;
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW = 0;
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 0;

                Main_State_Module.Self_test_state = sd_ram_check_state; //轉態

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Self_test_state -> sd_ram_check_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }
    }
}




/////////////////////////////////////////////////////////////////////
//
// 硬體開機流程, fan, pfc, dcac, dcac
//
static inline Power_Up_Process(void)
{
    switch(Main_State_Module.Power_up_state)
    {
        /*-----------------------------------------------------------------------------------------*/
        case start_state:
        {
            if ( Calc_Elapsed_Time( Main_State_Module.Power_Up_start_time, CpuTimer2.InterruptCount ) > POWER_UP_START_DELAY_TIME )
            {
                //
                // 顯示DEBUG訊息
                //
                #ifdef DEBUG_ON
                    //
                    sprintf(SCI_Module_Variables.Debug_Msg,
                            "CPU_Timer2: %lu ms > DSP Firmware version: %x \n"
                            , CpuTimer2.InterruptCount, DSP_FW_Version_Code ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles


                    //
                    #ifdef DEFAULT_ASR_5075
                    sprintf(SCI_Module_Variables.Debug_Msg,
                            "CPU_Timer2: %lu ms > DEFAULT_ASR_5075\n"
                            , CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    //
                    #ifdef DEFAULT_ASR_5150
                    sprintf(SCI_Module_Variables.Debug_Msg,
                            "CPU_Timer2: %lu ms > DEFAULT_ASR_5150\n"
                            , CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                #endif

                Main_State_Module.fan_Check_start_time = CpuTimer2.InterruptCount;
                Fan_Speed_Control_Variables.Speed_command_goal = FAN_STANDBY_DUTY;
                Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.fan_error_scan = 1;
                FANFAIL1_start_time_reset();
                FANFAIL2_start_time_reset();
                ACFAIL_start_time_reset();

                FANFAIL_start_time_reset(); //for 全寬機種

                Global_Variables.StartUp_State_Flag.bit.AC_Input_Check = 0;
                Global_Variables.StartUp_State_Flag.bit.DCAC_Check = 0;
                Global_Variables.StartUp_State_Flag.bit.DCDC_Check = 0;
                Global_Variables.StartUp_State_Flag.bit.Fan_Check = 0;
                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 0;

                Main_State_Module.Power_up_state = fan_check_state; //轉態

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> fan_check_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            break;
        }
            /*-----------------------------------------------------------------------------------------*/
        case fan_check_state:
        {
            if ( Calc_Elapsed_Time( Main_State_Module.fan_Check_start_time, CpuTimer2.InterruptCount ) > POWER_UP_FAN_CHECK_TIME )
            {
                //Check ok
                Global_Variables.StartUp_State_Flag.bit.Fan_Check = 1;


                Main_State_Module.ac_on_Check_start_time = CpuTimer2.InterruptCount;

                Global_Variables.OUTPUT_FLAGS.bit.AC_ON = 1; //for全寬機種

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > AC_ON = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                //轉態
                Main_State_Module.Power_up_state = ac_on_check_state;

                //
                // 開ac_on for 全寬機種
                // 對半寬機種不影響
                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> ac_on_check_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ac_on_check_state:
        {
            if ( Calc_Elapsed_Time( Main_State_Module.ac_on_Check_start_time, CpuTimer2.InterruptCount ) > POWER_UP_AC_ON_CHECK_TIME )
            {

                Main_State_Module.ac_on_Check2_start_time = CpuTimer2.InterruptCount;

                PFC_UVP_start_time_reset();
                PFC_OVP_start_time_reset();
                PFC_OTP_start_time_reset();
                PFC_UVP1_start_time_reset(); //for 全寬機種
                PFC_OVP1_start_time_reset(); //for 全寬機種

                Global_Variables.OUTPUT_FLAGS.bit.PFC_ON = 1;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > PFC_ON = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.pfc_error_scan = 1;

                //轉態
                Main_State_Module.Power_up_state = ac_on_check2_state;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> ac_on_check2_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case ac_on_check2_state:
        {
            if ( Calc_Elapsed_Time( Main_State_Module.ac_on_Check2_start_time, CpuTimer2.InterruptCount ) > POWER_UP_AC_ON_CHECK2_TIME )
            {

                Main_State_Module.pfc_Check_start_time = CpuTimer2.InterruptCount;

                Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.pfc_error2_scan = 1;

                PFC_UVP2_start_time_reset(); //for 全寬機種
                PFC_OVP2_start_time_reset(); //for 全寬機種

                Global_Variables.OUTPUT_FLAGS.bit.PFC_ON2 = 1; //for全寬機種

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > PFC_ON2 = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //轉態
                Main_State_Module.Power_up_state = pfc_check_state;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> pfc_check_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case pfc_check_state:
        {
            if ( Calc_Elapsed_Time( Main_State_Module.pfc_Check_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_CHECK_TIME )
            {

                Main_State_Module.pfc_Check2_start_time = CpuTimer2.InterruptCount;

                Global_Variables.OUTPUT_FLAGS.bit.DCDC_ON = 1;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCDC_ON = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //轉態
                Main_State_Module.Power_up_state = pfc_check2_state;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> pfc_check2_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }
            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case pfc_check2_state:
        {
            if ( Calc_Elapsed_Time( Main_State_Module.pfc_Check2_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_CHECK2_TIME )
            {

                Main_State_Module.dcdc_Check_start_time = CpuTimer2.InterruptCount;

                Global_Variables.OUTPUT_FLAGS.bit.DCDC_ON2 = 1; //for全寬機種

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCDC_ON2 = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //Check ok
                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 1;
                Global_Variables.StartUp_State_Flag.bit.AC_Input_Check = 1;

                //
                // 設定EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.POWER_ON = 1;

                DC_OVP_start_time_reset();

                Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.dcdc_error_scan = 1;

                //轉態
                Main_State_Module.Power_up_state = dcdc_check_state;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> dcdc_check_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }


        /*-----------------------------------------------------------------------------------------*/
        case dcdc_check_state:
        {
            if ( Calc_Elapsed_Time( Main_State_Module.dcdc_Check_start_time, CpuTimer2.InterruptCount ) > POWER_UP_DCDC_CHECK_TIME )
            {

                Main_State_Module.dcac_Check_start_time = CpuTimer2.InterruptCount;
                Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.dcac_error_scan = 1;

                DCAC_OTP_start_time_reset();

                Global_Variables.OUTPUT_FLAGS.bit.DCAC_ENA = 1;

                Global_Variables.OUTPUT_FLAGS.bit.AC_OCPA = 1;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCAC_ENA = 1, AC_OCPA = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                //Check ok
                Global_Variables.StartUp_State_Flag.bit.DCDC_Check = 1;

                //轉態
                Main_State_Module.Power_up_state = dcac_check_state;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> dcac_check_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case dcac_check_state:
        {
            if ( Calc_Elapsed_Time( Main_State_Module.dcac_Check_start_time, CpuTimer2.InterruptCount ) > POWER_UP_DCAC_CHECK_TIME )
            {

                Global_Variables.StartUp_State_Flag.bit.DCAC_Check = 1;

                //轉態
                Main_State_Module.Power_up_state = power_up_ready_state;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_ready_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case power_up_ready_state:
        {

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case power_up_error_handle_state:
        {
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 0;


            Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.fan_error_scan = 0;
            Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.pfc_error_scan = 0;
            Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.pfc_error2_scan = 0;
            Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.dcac_error_scan = 0;
            Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.dcdc_error_scan = 0;

            //停止輸出，關閉DCAC PWM輸出；
            //計時100ms，關閉DCDC、PFC
            if ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL1_HW
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL2_HW
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP1_HW //for 全寬機種
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP1_HW //for 全寬機種
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP2_HW //for 全寬機種
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP2_HW //for 全寬機種
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.PFC_OTP_HW
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.DC_OVP_HW
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_OFF_ACFAIL_HW
                 || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_ON_ACFAIL_HW )
            {
                //
                // 輸出Relay OFF
                //
                Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 0;

                // 設定輸出狀態
                //
                Global_Variables.POWER_STATUS.bit.Output_ON = 0;

                //
                // 設定EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 0;

                Global_Variables.OUTPUT_FLAGS.bit.DCAC_ENA = 0;
                Global_Variables.OUTPUT_FLAGS.bit.AC_OCPA = 0;


                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCAC_ENA = 0, AC_OCPA = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif



                if ( Calc_Elapsed_Time( Main_State_Module.error_start_time, CpuTimer2.InterruptCount ) > PFC_DCDC_OFF_DELAY_TIME )
                {
                    Global_Variables.OUTPUT_FLAGS.bit.PFC_ON = 0;
                    Global_Variables.OUTPUT_FLAGS.bit.PFC_ON2 = 0; //for全寬機種

                    Global_Variables.OUTPUT_FLAGS.bit.DCDC_ON = 0;
                    Global_Variables.OUTPUT_FLAGS.bit.DCDC_ON2 = 0; //for全寬機種

                    Global_Variables.OUTPUT_FLAGS.bit.AC_ON = 0; //for全寬機種

                    //
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > PFC_ON = 0, PFC_ON2 = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    //
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCDC_ON = 0, DCDC_ON2 = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif


                    //
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > AC_ON = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    //轉態
                    Main_State_Module.Power_up_state = power_up_error_state;

                    //
                    #ifdef DEBUG_ON
                    sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                    #endif

                    break;
                }
            }

            //停止輸出，關閉DCAC PWM輸出；
            if ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.DCAC_OTP_SW )
            {
                //
                // 輸出Relay OFF
                //
                Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 0;

                // 設定輸出狀態
                //
                Global_Variables.POWER_STATUS.bit.Output_ON = 0;

                //
                // 設定EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 0;

                Global_Variables.OUTPUT_FLAGS.bit.DCAC_ENA = 0;
                Global_Variables.OUTPUT_FLAGS.bit.AC_OCPA = 0;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCAC_ENA = 0, AC_OCPA = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif


                //轉態
                Main_State_Module.Power_up_state = power_up_error_state;

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }

        /*-----------------------------------------------------------------------------------------*/
        case power_up_error_state:
        {
            FANFAIL1_start_time_reset();
            FANFAIL2_start_time_reset();
            PFC_UVP_start_time_reset();
            PFC_OVP_start_time_reset();
            PFC_OTP_start_time_reset();
            ACFAIL_start_time_reset();
            DCAC_OTP_start_time_reset();

            FANFAIL_start_time_reset(); //for 全寬機種
            PFC_UVP1_start_time_reset(); //for 全寬機種
            PFC_OVP1_start_time_reset(); //for 全寬機種
            PFC_UVP2_start_time_reset(); //for 全寬機種
            PFC_OVP2_start_time_reset(); //for 全寬機種

            DC_OVP_start_time_reset();


            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset == 1 )
            {
                Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 0;
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all = 0;
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all = 0;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = NO_ERROR_CODE;


                //轉態
                Main_State_Module.Power_up_state = start_state;

                Main_State_Module.Power_Up_start_time = CpuTimer2.InterruptCount;

                //debug msg
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> start_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                break;
            }

            break;
        }
    }
}




/////////////////////////////////////////////////////////////////////
//
//
//
static inline Ready0_Process(void)
{
    //
    #ifdef DEVELOP_SV01
    cpu_timer_module.cpu_timer0_isr_Cycle_Meas_Max = 0;
    #endif


    //
    // Clear 保護Reset
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 0;

    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Output_Relay_Enable == 1 )
    {
        //
        // 輸出Relay OFF
        //
        Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 0;
    }
    else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Output_Relay_Enable == 0 )
    {
        //
        // 輸出Relay 保持ON
        //
        Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 1;
    }


    //
    // 設定EXTIO
    //
    Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 0;

    // 設定輸出狀態
    //
    Global_Variables.POWER_STATUS.bit.Output_ON = 0;


    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.bit.SYNC != 1 )
    {
        //
        // Reset Freq Step
        //
        Global_Variables.FreqStep = DC_FREQ_STEP;
    }

    //
    // Reset 控制旗標
    //
    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;
    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;

    //
    // 清掉輸出中才會有效的事件旗標
    //
    Event_Module.Event2_Flag.bit.Trig_Value_Set_User_Change = 0;
    cpu_timer_module.Value_Set_User_Change = 0;

    //
    // 沒有輸出時, 預設CV
    //
    Output_Control_Module.Power_Control_Mode = CV_CONTROL_MODE;

    //
    // CV補償關閉
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow = 0;

    //
    // Reset補償狀態機
    //
    Output_Control_Module.DCV_Compensate_State = DCV_NO_COMPENSATE;
    Output_Control_Module.ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;

    //
    // Reset 補償量
    //
    Output_Control_Module.WAVE_Compensate_Offset = 0;
    Output_Control_Module.WAVE_Compensate_Offset2 = 0;
    CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
    CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;
    CPU_to_CLA_Variables.VREF_Compensate_Offset_2 = 0;
    CPU_to_CLA_Variables.VREF_Compensate_Offset_3 = 0;

    //
    // Reset 頻率補償
    //
    Output_Control_Module.Freq_Compensation_gain = 1.0;

    //
    // Reset DAC
    //
    Init_VREF_WAVE_Process();

    //
    // Reset VI擷取
    //
    Global_Variables.VI_RAW_OP_CODE.all = 0;
    Global_Variables.POWER_DISPLAY_VARIABLES.VI_Raw_Data_Counter = 0xA5A5;

    //
    // SEQ 狀態更新
    //
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_out_off;


    //
    // MS連線下的Ready0 Reset
    //
    if ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
    {
        Output_Control_Module.MS_ACDCV_Compensate_State = ACDCV_NO_COMPENSATE;
        Output_Control_Module.MS_DCV_Compensate_State = DCV_NO_COMPENSATE;
        Output_Control_Module.MS_Balance_Compensate_State = MS_BALANCE_NO_COMPENSATE;

        //
        // Slave清掉同步狀態旗標
        //
        Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[ Global_Variables.MS_Function_Variables.MS_Address ].all = 0;

        if ( Global_Variables.POWER_STATUS.bit.MS_Master == 0 )
        {
            //
            // Slave端 , 直接清掉CMD, 避免Busy卡住
            //
            MS_Control_Module_Variables.Master_SCI_Read_CMD.all = 0;
            MS_Control_Module_Variables.Master_SCI_Write_CMD.all = 0;
        }
    }
}




/////////////////////////////////////////////////////////////////////
//
//
//
static inline Ready_Process(void)
{
    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Output_Range_Option == AUTO_RANGE )
    {
        //
        // Auto Range 判斷
        //

        if ( ( Global_Variables.OUTPUT_MODE_FLAG.all != Global_Variables.OUTPUT_MODE_FLAG_Shadow.all )
             || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow )
             || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow )
             || ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow ) )
        {
            Global_Variables.OUTPUT_MODE_FLAG_Shadow.all = Global_Variables.OUTPUT_MODE_FLAG.all;

            //
            // Auto Range, 設定值改變
            //
            Adj_Output_Range_Event();
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

        // 設定Busy狀態
        //
        Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

    }

    #ifdef DEVELOP_SV75
    //
    // 為方便測試, 偵測ACV設定值觸發事件
    //
    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow != Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all )
    {
        if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all == 770000)
        {
            //
            // SDRAM同步
            //
            Event_Module.Event_Flag.bit.MS_Sync_SDRAM_Process = 1;

            // 設定Busy狀態
            //
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
        }
    }
    #endif

    //
    // 保持Shadow更新
    //
    Global_Variables.OUTPUT_MODE_FLAG_Shadow.all = Global_Variables.OUTPUT_MODE_FLAG.all;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;

    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_shadow = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user_shadow = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.all;

    //
    // 設定Remote ON/OFF
    //
    Global_Variables.OUTPUT_FLAGS.bit.REMOTE = Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Remote_Sense_Enable;


    //
    // 設定SYNC Code 0/1 => EXTIO
    //
    Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC0 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ 0 ].bit.Sync_Code_0;
    Global_Variables.EXT_OUTPUT_FLAG.bit.SEQ_SYNC1 = SEQ_Step_Section1.SEQ_Config_State1_set_user[ 0 ].bit.Sync_Code_1;


    //
    // EXTIO設定
    //
    //Global_Variables.EXT_OUTPUT_FLAG.bit.POWER_ON = 1;
    //Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 0;
    Global_Variables.EXT_OUTPUT_FLAG.bit.I_LIMITER = 0;
    //Global_Variables.EXT_OUTPUT_FLAG.bit.ZERO_CROSS_TRIG_OUT = 0;
    //Global_Variables.EXT_OUTPUT_FLAG.bit.SOFTWARE_BUSY = 0;

    #ifdef DEVELOP_SV62
    #else
    Global_Variables.EXT_OUTPUT_FLAG.bit.UNDEFINED_OUT1 = 0;
    #endif

    //
    // Zero_Cross_Width_Set x 倍率k
    // k = 100 / ( 2.775 x 排程數 )
    //
    cpu_timer_module.Zero_Cross_Width_Count = (Uint32)( (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Zero_Cross_Width_Set * 18.018 );

    //
    // I_peak_hold_start_time x 倍率k
    // k = 1000 / ( 2.775 x CLA排程數 )
    //
    CPU_to_CLA_Variables.IPK_Hold_Delay_Count = (Uint32)( (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.I_peak_hold_start_time * 180.18018 );
    if ( CPU_to_CLA_Variables.IPK_Hold_Delay_Count == 0 )
    {
        CPU_to_CLA_Variables.IPK_Hold_Delay_Count = 1;
    }


    CPU_to_CLA_Variables.VPK_Hold_Delay_Count = (Uint32)( (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_peak_hold_start_time * 180.18018 );
    if ( CPU_to_CLA_Variables.VPK_Hold_Delay_Count == 0 )
    {
        CPU_to_CLA_Variables.VPK_Hold_Delay_Count = 1;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    //
    // CLA內部短路保護有清除, 表示OCP Clear ok
    // 所以清掉OCP Clear
    //
    if ( CLA_to_CPU_Variables.Short_OCP == 0 )
    {
        CPU_to_CLA_Variables.Short_OCP_Clear = 0;
    }

    //
    // 清掉CLA _4CF_OCP Clear
    //
    if ( CLA_to_CPU_Variables._4CF_OCP == 0 )
    {
        CPU_to_CLA_Variables._4CF_OCP_Clear = 0;
    }


}


/////////////////////////////////////////////////////////////////////
//
//
//
static inline Output0_Process(void)
{
    //
    // Reset移動平均累加
    //
    Display_Move_Avg_Reset();

    //
    // 指定OUTPUT_MODE輸出流程進入初始化狀態
    //
    switch(Global_Variables.OUTPUT_MODE_FLAG_Shadow.all)
    {
        case DC_INT_MODE: //DC-INT
        case AC_INT_MODE: //AC-INT
        case ACDC_INT_MODE: //ACDC-INT
        {
            Output_ACDC_INT_Module.Output_ACDC_INT_State = acdc_int_output_init; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_INT_State -> acdc_int_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
        case AC_EXT_MODE: //AC-EXT
        case ACDC_EXT_MODE: //ACDC-EXT
        {
            Output_ACDC_EXT_Module.Output_ACDC_EXT_State = acdc_ext_output_init; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_EXT_State -> acdc_ext_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
        case AC_ADD_MODE: //AC-ADD
        case ACDC_ADD_MODE: //ACDC-ADD
        {
            Output_ACDC_ADD_Module.Output_ACDC_ADD_State = acdc_add_output_init; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_ADD_State -> acdc_add_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
        case AC_SYNC_MODE: //AC-SYNC
        case ACDC_SYNC_MODE: //ACDC-SYNC
        {
            Output_ACDC_SYNC_Module.Output_ACDC_SYNC_State = acdc_sync_output_init; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_ACDC_SYNC_State -> acdc_sync_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
        case SEQ_MODE: //SEQ
        {
            Output_SEQ_Module.Output_SEQ_State = seq_output_init; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_SEQ_State -> seq_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
        case AC_VCA_MODE: //AC-VCA
        {
            Output_AC_VCA_Module.Output_AC_VCA_State = ac_vca_output_init; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Output_AC_VCA_State -> ac_vca_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
        case MASTER_DC_INT_MODE:
        case SLAVE_DC_INT_MODE:
        case MASTER_AC_INT_MODE:
        case MASTER_ACDC_INT_MODE:
        case SLAVE_AC_INT_MODE:
        case SLAVE_ACDC_INT_MODE:
        {

            MS_Output_ACDC_INT_Module.MS_Output_ACDC_INT_State = MS_acdc_int_output_init; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Output_ACDC_INT_State -> MS_acdc_int_output_init\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
        default:
        {
            Main_State_Module.Machine_state = READY_STATE0; //轉態

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Output Mode illegal !***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> READY_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            break;
        }
    }
}


/////////////////////////////////////////////////////////////////////
//
//
//
static inline Output_Process(void)
{
    //
    // OUTPUT_MODE改變, 停止輸出
    //
    if ( Global_Variables.OUTPUT_MODE_FLAG_Shadow.all != Global_Variables.OUTPUT_MODE_FLAG.all )
    {
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;
        Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;

        Global_Variables.OUTPUT_MODE_FLAG_Shadow.all = Global_Variables.OUTPUT_MODE_FLAG.all;
    }

    //
    // 執行OUTPUT_MODE輸出流程
    //
    switch(Global_Variables.OUTPUT_MODE_FLAG_Shadow.all)
    {
        case DC_INT_MODE: //DC-INT
        case AC_INT_MODE: //AC-INT
        case ACDC_INT_MODE: //ACDC-INT
        {
            ACDC_INT_Output_Process();
            break;
        }
        case AC_EXT_MODE: //AC-EXT
        case ACDC_EXT_MODE: //ACDC-EXT
        {
            ACDC_EXT_Output_Process();
            break;
        }
        case AC_ADD_MODE: //AC-ADD
        case ACDC_ADD_MODE: //ACDC-ADD
        {
            ACDC_ADD_Output_Process();
            break;
        }
        case AC_SYNC_MODE: //AC-SYNC
        case ACDC_SYNC_MODE: //ACDC-SYNC
        {
            ACDC_SYNC_Output_Process();
            break;
        }
        case SEQ_MODE: //SEQ
        {
            SEQ_Output_Process();
            break;
        }
        case AC_VCA_MODE: //AC-VCA
        {
            AC_VCA_Output_Process();
            break;
        }
        case MASTER_DC_INT_MODE:
        case SLAVE_DC_INT_MODE:
        case MASTER_AC_INT_MODE:
        case MASTER_ACDC_INT_MODE:
        case SLAVE_AC_INT_MODE:
        case SLAVE_ACDC_INT_MODE:
        {
            MS_ACDC_INT_Output_Process();
            break;
        }
        default:
        {
            break;
        }
    }

}


/////////////////////////////////////////////////////////////////////
//
//
//
static inline Error_Handle0_Process(void)
{
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 0;

    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;
    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;

    //
    // Reset DAC
    //
    Init_VREF_WAVE_Process();

    Global_Variables.OUTPUT_FLAGS.bit.DCAC_ENA = 0;
    Global_Variables.OUTPUT_FLAGS.bit.AC_OCPA = 0;

    //
    // 更新校正狀態 => NG
    //
    Global_Variables.Calibration_State_Flag.all = 2;
}



/////////////////////////////////////////////////////////////////////
//
//
//
static inline Error_Handle1_Process(void)
{
    if ( Calc_Elapsed_Time( Main_State_Module.error_start_time, CpuTimer2.InterruptCount ) > ERROR_RELAY_OFF_DELAY_TIME )
    {
        //
        // Delay設定時間, 再去斷開輸出Relay
        //

        if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Output_Relay_Enable == 1 )
        {
            //
            // 輸出Relay OFF
            //
            Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 0;
        }
        else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Output_Relay_Enable == 0 )
        {
            //
            // 輸出Relay 保持ON
            //
            Global_Variables.OUTPUT_FLAGS.bit.OUT_ON = 1;
        }

        //
        // 設定EXTIO
        //
        Global_Variables.EXT_OUTPUT_FLAG.bit.OUTPUT_ON = 0;

        // 設定輸出狀態
        //
        Global_Variables.POWER_STATUS.bit.Output_ON = 0;


        if ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP1_HW //for 全寬機種
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP1_HW //for 全寬機種
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP2_HW //for 全寬機種
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP2_HW //for 全寬機種
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.PFC_OTP_HW
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.DC_OVP_HW
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_OFF_ACFAIL_HW
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_ON_ACFAIL_HW
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL1_HW
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL2_HW )
        {
            //
            // HW異常
            //

            //
            // 轉態
            //
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE2;

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE2\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
        else if ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.DCAC_OTP_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.p_I_peak_OCP_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.n_I_peak_OCP_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.I_rms_OCP_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Short_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.OVP_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.OPP_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.SDRAM_Error_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_SDRAM_SYNC_STATE_Error
                  || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.remote_OVP_SW )
        {
            //
            // SW異常
            //

            if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
            {
                //
                // 全寬機種, 進ERROR_STATE會繼續判斷ACFAIL
                //
                ACFAIL_start_time_reset();
            }

            //
            // 轉態
            //
            Main_State_Module.Machine_state = ERROR_STATE;


            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
    }

}



/////////////////////////////////////////////////////////////////////
//
//
//
static inline Error_Handle2_Process(void)
{
    if ( Calc_Elapsed_Time( Main_State_Module.error_start_time, CpuTimer2.InterruptCount ) > PFC_DCDC_OFF_DELAY_TIME )
    {
        //
        // Delay設定時間, 再去關PFC以及DCDC
        //

        Global_Variables.OUTPUT_FLAGS.bit.PFC_ON = 0;
        Global_Variables.OUTPUT_FLAGS.bit.PFC_ON2 = 0; //for全寬機種

        Global_Variables.OUTPUT_FLAGS.bit.DCDC_ON = 0;
        Global_Variables.OUTPUT_FLAGS.bit.DCDC_ON2 = 0; //for全寬機種

        Global_Variables.OUTPUT_FLAGS.bit.AC_ON = 0; //for全寬機種

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > PFC_ON = 0, PFC_ON2 = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCDC_ON = 0, DCDC_ON2 = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > AC_ON = 0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        if ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 )
        {
            //
            // 全寬機種, 進ERROR_STATE會繼續判斷ACFAIL
            //
            ACFAIL_start_time_reset();
        }

        //
        // 轉態
        //
        Main_State_Module.Machine_state = ERROR_STATE;

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif
    }

}


/////////////////////////////////////////////////////////////////////
//
//
//
static inline Error_Process(void)
{
    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = 0;
    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = 0;

    FANFAIL1_start_time_reset();
    FANFAIL2_start_time_reset();
    PFC_UVP_start_time_reset();
    PFC_OVP_start_time_reset();
    PFC_OTP_start_time_reset();
    //ACFAIL_start_time_reset();
    DCAC_OTP_start_time_reset();
    p_I_peak_ocp_start_time_reset();
    n_I_peak_ocp_start_time_reset();
    I_rms_ocp_start_time_reset();
    V_rms_ovp_start_time_reset();
    opp_start_time_reset();
    remote_OVP_start_time_reset();
    FANFAIL_start_time_reset(); //for 全寬機種
    PFC_UVP1_start_time_reset(); //for 全寬機種
    PFC_OVP1_start_time_reset(); //for 全寬機種
    PFC_UVP2_start_time_reset(); //for 全寬機種
    PFC_OVP2_start_time_reset(); //for 全寬機種

    DC_OVP_start_time_reset();

    MS_Output_Status_Check_Start_Time_reset();

    //
    // SEQ 狀態更新
    //
    Global_Variables.SEQ_FUNCTION_VARIABLES.SEQ_Current_State = _Sequence_out_off;


    //
    // 5150或5151 全寬機種
    // 異常時 也要判斷ACFAIL, 否則此時無法關機
    //
    if ( ( Global_Variables.INPUT_FLAGS.bit.ACFAIL == 0 )
         && ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series == ASR_5150 ) )
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.ACFAIL_start_time, CpuTimer2.InterruptCount ) > POWER_READY_ACFAIL_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_OFF_ACFAIL_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Output_OFF_ACFAIL_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif
        }
    }
    else
    {
        ACFAIL_start_time_reset();
    }


    //
    //按下Reset
    //
    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = 0;

        if ( ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error )
             || ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error )
             || ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_SDRAM_SYNC_STATE_Error ) )
        {
            //
            // MS連線異常重置
            //

            //
            // 觸發轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Connect_State -> MS_CONNECT_STATE_INIT0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }




        if ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP1_HW //for 全寬機種
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP1_HW //for 全寬機種
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP2_HW //for 全寬機種
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP2_HW //for 全寬機種
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.PFC_OTP_HW
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.DC_OVP_HW
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_OFF_ACFAIL_HW
             || Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_ON_ACFAIL_HW )
        {
            //
            //有關掉PFC及DCDC的保護重置處理
            //

            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all = 0;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all = 0;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = NO_ERROR_CODE;

            Main_State_Module.Machine_state = POWER_UP_STATE;
            Main_State_Module.Power_up_state = start_state;

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> POWER_UP_STATE\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> start_state\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.all = 0;

            Main_State_Module.Power_Up_start_time = CpuTimer2.InterruptCount;

            Main_State_Module.fan_Check_start_time = 0;
            Main_State_Module.ac_on_Check_start_time = 0;
            Main_State_Module.ac_on_Check2_start_time = 0;
            Main_State_Module.pfc_Check_start_time = 0;
            Main_State_Module.pfc_Check2_start_time = 0;
            Main_State_Module.dcac_Check_start_time = 0;
            Main_State_Module.dcdc_Check_start_time = 0;
            Main_State_Module.error_start_time = 0;
            Main_State_Module.output_start_time = 0;
            Main_State_Module.output_stop_time = 0;

            Main_State_Module.Self_Test_AutoReset_Counter = SELF_TEST_AUTO_RESET_COUNT;

            //
            // 觸發轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > MS_Connect_State -> MS_CONNECT_STATE_INIT0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
        else
        {
            //
            // 一般的保護重置處理
            //

            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all = 0;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all = 0;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = NO_ERROR_CODE;

            //CLA內部短路保護Clear
            CPU_to_CLA_Variables.Short_OCP_Clear = 1;

            CPU_to_CLA_Variables._4CF_OCP_Clear = 1;

            Global_Variables.OUTPUT_FLAGS.bit.DCAC_ENA = 1;

            Global_Variables.OUTPUT_FLAGS.bit.AC_OCPA = 1;

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > DCAC_ENA = 1, AC_OCPA = 1\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            if ( ( MS_Control_Module_Variables.MS_Connect_State == MS_CONNECT_OK_STATE )
                 && ( Global_Variables.POWER_STATUS.bit.MS_Master == 1 ) )
            {
                //
                // MS連線下, Master
                //

                //
                // 清掉CMD
                //
                MS_Control_Module_Variables.Master_SCI_Read_CMD.all = 0;
                MS_Control_Module_Variables.Master_SCI_Write_CMD.all = 0;

                //
                // 進行同步
                //
                MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;
                MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_81_Tx_Trig = 1;
                MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_82_Tx_Trig = 1;
                MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_83_Tx_Trig = 1;
                MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_84_Tx_Trig = 1;

                // 設定Busy狀態
                //
                Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            }

            //
            // 更新校正狀態 => 完成 (預設)
            //
            Global_Variables.Calibration_State_Flag.all = 1;

            //轉態
            Main_State_Module.Machine_state = READY_STATE0;

            //debug msg
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> READY_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
    }

}







//
// End of file
//
