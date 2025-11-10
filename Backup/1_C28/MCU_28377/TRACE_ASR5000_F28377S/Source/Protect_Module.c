//###########################################################################
//
// FILE:    Protect_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211015
//###########################################################################



#include "Global_VariableDefs.h"



/////////////////////////////////////////////////////////////////////
//
// PowerReady階段的異常掃描
//
void Power_Ready_Error_Scan(void)
{
#ifndef BYPASS_SCAN_ERROR_FOR_POWER_READY

    float float_buf;
    Uint32 uint32_buf2;
    int32 int32_buf, int32_buf2;

    /*------------------------------------------------*/
    //
    // FANFAIL1
    //
    if ( Global_Variables.INPUT_FLAGS.bit.FANFAIL1 == 1 )
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL1_start_time, CpuTimer2.InterruptCount ) > POWER_READY_FANFAIL1_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL1_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***FANFAIL1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

    }
    else
    {
        FANFAIL1_start_time_reset();
    }


    /*------------------------------------------------*/
    //
    // FANFAIL2
    //
    if ( Global_Variables.INPUT_FLAGS.bit.FANFAIL2 == 1 )
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL2_start_time, CpuTimer2.InterruptCount ) > POWER_READY_FANFAIL1_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL2_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***FANFAIL2_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
    }
    else
    {
        FANFAIL2_start_time_reset();
    }


    /*------------------------------------------------*/
    //
    // FANFAIL (for 全寬機種)
    //
    if ( Global_Variables.INPUT_FLAGS.bit.FANFAIL == 1 )
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL_start_time, CpuTimer2.InterruptCount ) > POWER_READY_FANFAIL_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL1_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***FANFAIL1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

    }
    else
    {
        FANFAIL_start_time_reset();
    }



    /*------------------------------------------------*/
    //
    // PFC UVP
    //
    if (Global_Variables.INPUT_FLAGS.bit.PFC_UVP == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP_start_time, CpuTimer2.InterruptCount ) > POWER_READY_PFC_UVP_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP1_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_UVP1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif
        }

    }
    else
    {
        PFC_UVP_start_time_reset();
    }

    /*------------------------------------------------*/
    //
    // PFC UVP1 (for 全寬機種)
    //
    if (Global_Variables.INPUT_FLAGS.bit.PFC_UVP1 == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP1_start_time, CpuTimer2.InterruptCount ) > POWER_READY_PFC_UVP1_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP1_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_UVP1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif
        }

    }
    else
    {
        PFC_UVP1_start_time_reset();
    }

    /*------------------------------------------------*/
    //
    // PFC UVP2 (for 全寬機種)
    //
    if (Global_Variables.INPUT_FLAGS.bit.PFC_UVP2 == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP2_start_time, CpuTimer2.InterruptCount ) > POWER_READY_PFC_UVP2_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP2_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_UVP2_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif
        }

    }
    else
    {
        PFC_UVP2_start_time_reset();
    }



    /*------------------------------------------------*/
    //
    // PFC OVP
    //
    if (Global_Variables.INPUT_FLAGS.bit.PFC_OVP == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP_start_time, CpuTimer2.InterruptCount ) > POWER_READY_PFC_OVP_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP1_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_OVP1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

    }
    else
    {
        PFC_OVP_start_time_reset();
    }

    /*------------------------------------------------*/
    //
    // PFC OVP1 (for 全寬機種)
    //
    if (Global_Variables.INPUT_FLAGS.bit.PFC_OVP1 == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP1_start_time, CpuTimer2.InterruptCount ) > POWER_READY_PFC_OVP1_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP1_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_OVP1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

    }
    else
    {
        PFC_OVP1_start_time_reset();
    }

    /*------------------------------------------------*/
    //
    // PFC OVP2 (for 全寬機種)
    //
    if (Global_Variables.INPUT_FLAGS.bit.PFC_OVP2 == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP2_start_time, CpuTimer2.InterruptCount ) > POWER_READY_PFC_OVP2_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP2_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_OVP2_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

    }
    else
    {
        PFC_OVP2_start_time_reset();
    }







    /*------------------------------------------------*/
    //
    // PFC OTP
    //
    if (Global_Variables.INPUT_FLAGS.bit.PFC_OTP == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OTP_start_time, CpuTimer2.InterruptCount ) > POWER_READY_PFC_OTP_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.PFC_OTP_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_OTP_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

    }
    else
    {
        PFC_OTP_start_time_reset();
    }

    /*------------------------------------------------*/
    //
    // AC FAIL
    //
#ifndef DEVELOP_SV60
    if (Global_Variables.INPUT_FLAGS.bit.ACFAIL == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.ACFAIL_start_time, CpuTimer2.InterruptCount ) > POWER_READY_ACFAIL_ERROR_TIME )
        {
            //
            // 輸出中ACFAIL判斷為Power Input Anomaly
            // 沒有輸出時ACFAIL判斷為System Shutdown
            //
            if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 1 )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_ON_ACFAIL_HW = 1;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Output_ON_ACFAIL_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
            else if ( Global_Variables.POWER_CONTROL.bit.OUTPUT_ON == 0 )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_OFF_ACFAIL_HW = 1;

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Output_OFF_ACFAIL_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

            //
            // 設定EXTIO
            //
            Global_Variables.EXT_OUTPUT_FLAG.bit.POWER_ON = 0;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();


            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

    }
    else
    {
        ACFAIL_start_time_reset();
    }
#endif
    /*------------------------------------------------*/
    //
    // DCAC OTP
    //
    if ( Global_Variables.INPUT_VARIABLES.Temp_Sensor > DCAC_OTP_LEVEL )
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DCAC_OTP_start_time, CpuTimer2.InterruptCount ) > POWER_READY_DCAC_OTP_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.DCAC_OTP_SW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***DCAC_OTP_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
    }
    else
    {
        DCAC_OTP_start_time_reset();
    }



    /*------------------------------------------------*/
    //
    // DC OVP
    //
    if (Global_Variables.INPUT_FLAGS.bit.DC_OVP == 0)
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DC_OVP_start_time, CpuTimer2.InterruptCount ) > POWER_READY_DC_OVP_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.DC_OVP_HW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***DC_OVP_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }

    }
    else
    {
        DC_OVP_start_time_reset();
    }


    /*------------------------------------------------*/
    //
    // Ipeak+ OCP
    //
    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable == 0 ) //關閉Limiter時才檢查
    {
        float_buf = Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display_shadow;
        float_buf = float_buf * 10000;
        int32_buf = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.all;

        if ( ( (int32)float_buf > int32_buf ) && ( Output_Control_Module.Flag.bit.Bypass_OCP_Flag == 0 ) )
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_ocp_start_time, CpuTimer0.InterruptCount )
                    > POWER_READY_IPEAK_OCP_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.p_I_peak_OCP_SW = 1;

                //儲存異常發生的I_peak值
                //Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.p_I_peak_display_shadow;

                //轉態
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***p_I_peak_OCP_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
        }
        else
        {
            p_I_peak_ocp_start_time_reset();
        }
    }

    /*------------------------------------------------*/
    //
    // Ipeak- OCP
    //
    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable == 0 ) //關閉Limiter時才檢查
    {
        float_buf = Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display_shadow;
        float_buf = float_buf * 10000;
        int32_buf = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.all;

        if ( ( (int32)float_buf < int32_buf ) && ( Output_Control_Module.Flag.bit.Bypass_OCP_Flag == 0 ) )
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_ocp_start_time, CpuTimer0.InterruptCount )
                    > POWER_READY_IPEAK_OCP_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.n_I_peak_OCP_SW = 1;

                //儲存異常發生的I_peak值
                //Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.n_I_peak_display_shadow;

                //轉態
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***n_I_peak_OCP_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
        }
        else
        {
            n_I_peak_ocp_start_time_reset();
        }
    }

    /*------------------------------------------------*/
    //
    // Irms OCP
    //
    if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable == 0 )
    {
        //
        // I_RMS_Limit OFF
        // 判斷準位 = I RMS Limit Level
        // 延遲時間 = 0
        //

        uint32_buf2 = 0;

        float_buf = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
        float_buf = float_buf * 10000;
        int32_buf = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.all;
    }
    else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable == 1 )
    {
        //
        // I_RMS_Limit ON
        // 判斷準位 = 額定RMS電流
        // 延遲時間 = 250ms
        //

        uint32_buf2 = POWER_READY_IRMS_OCP_ERROR_TIME;

        float_buf = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display_shadow;
        float_buf = float_buf * 10000;
        int32_buf = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level_max;
    }


    if ( ( (int32)float_buf > int32_buf ) && ( Output_Control_Module.Flag.bit.Bypass_OCP_Flag == 0 ) )
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_ocp_start_time, CpuTimer2.InterruptCount )
                > uint32_buf2 )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.I_rms_OCP_SW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***I_rms_OCP_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
    }
    else
    {
        I_rms_ocp_start_time_reset();
    }


    /*------------------------------------------------*/
    //
    // 4CF OCP
    //

    if ( CLA_to_CPU_Variables._4CF_OCP == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.I_rms_OCP_SW = 1;

        //儲存異常發生的I_peak值
        //Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.all = CLA_to_CPU_Variables._4CF_OCP_IFB;

        //轉態
        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***_4CF_OCP***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }

    /*------------------------------------------------*/
    //
    // Short OCP
    //

    if ( CLA_to_CPU_Variables.Short_OCP == 1 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Short_SW = 1;

        //
        // 更新Short發生當下的數據
        //
        //Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.all = CLA_to_CPU_Variables.Short_OCP_IFB;
        Global_Variables.POWER_DISPLAY_VARIABLES.Short_OCP_V_FB.all = CLA_to_CPU_Variables.Short_OCP_V_FB;
        Global_Variables.POWER_DISPLAY_VARIABLES.Short_OCP_c2 = CLA_to_CPU_Variables.Short_OCP_c2;
        Global_Variables.POWER_DISPLAY_VARIABLES.Short_OCP_VREF_set = CLA_to_CPU_Variables.Short_OCP_VREF_set;
        Global_Variables.POWER_DISPLAY_VARIABLES.Short_OCP_WAVE_set = CLA_to_CPU_Variables.Short_OCP_WAVE_set;

        //轉態
        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Short_OCP***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        //
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

    }


    /*------------------------------------------------*/
    //
    // OVP
    //

    if ( Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display_shadow > Global_Variables.POWER_PROTECT_SETTING_VARIABLES.V_rms_limit_level )
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.V_rms_ovp_start_time, CpuTimer2.InterruptCount )
                > POWER_READY_OVP_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.OVP_SW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***OVP_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
    }
    else
    {
        V_rms_ovp_start_time_reset();
    }

    /*------------------------------------------------*/
    //
    // OPP
    // ( 視在功率(VA) > OPP_level ) 或 ( 平均功率 < -100W )
    //
    if ( ( Global_Variables.POWER_DISPLAY_VARIABLES.S_display_shadow > Global_Variables.POWER_PROTECT_SETTING_VARIABLES.OPP_level )
         || ( Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow.all < Global_Variables.POWER_PROTECT_SETTING_VARIABLES.Negative_OPP_Level ) )
    {
        if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.opp_start_time, CpuTimer2.InterruptCount )
                > POWER_READY_OPP_ERROR_TIME )
        {
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.OPP_SW = 1;

            //轉態
            Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


            error_start_time_reset();

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***OPP_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            //
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

        }
    }
    else
    {
        opp_start_time_reset();
    }


    /*------------------------------------------------*/
    //
    // Remote OVP
    //
    if ( ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Remote_Sense_Enable == 1 )
         &&( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable == 1 )
         && ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable_shadow == 1 )
         && ( ( Output_Control_Module.ACDCV_Compensate_State != ACDCV_NO_COMPENSATE )
              || ( Output_Control_Module.DCV_Compensate_State != DCV_NO_COMPENSATE ) )
         && ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
         && ( Output_Control_Module.Power_Control_Mode == CV_CONTROL_MODE ) )
    {
        switch(Global_Variables.OUTPUT_MODE_FLAG_Shadow.all)
        {
            case AC_VCA_MODE: //AC-VCA
            {
                float_buf = Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display_shadow;
                int32_buf2 = (int32)Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set - (int32)float_buf;

                int32_buf = (int32)( Global_Variables.EXT_INPUT_VARIABLES.EXT_ACV_Set * 0.3 ) + 2;
                //（電壓設定值x 0.3 + 2V）

                break;
            }
            case AC_INT_MODE: //AC-INT
            case AC_SYNC_MODE: //AC-SYNC
            {
                float_buf = Global_Variables.POWER_DISPLAY_VARIABLES.AC_V_rms_display_shadow;
                float_buf = float_buf * 10000;
                int32_buf2 = (int32)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all - (int32)float_buf;

                int32_buf = (int32)( (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.all * 0.3 ) + 20000;
                //（電壓設定值x 0.3 + 2V）

                break;
            }
            case DC_INT_MODE: //DC-INT
            {
                float_buf = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display_shadow;
                float_buf = float_buf * 10000;
                int32_buf2 = (int32)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all - (int32)float_buf;

                int32_buf = (int32)( (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.all * 0.3 ) + 20000;
                //（電壓設定值x 0.3 + 2V）

                break;
            }
            case ACDC_INT_MODE: //ACDC-INT
            case MASTER_ACDC_INT_MODE:
            case SLAVE_ACDC_INT_MODE:
            case ACDC_SYNC_MODE: //ACDC-SYNC
            case AC_EXT_MODE: //AC-EXT
            case ACDC_EXT_MODE: //ACDC-EXT
            case SLAVE_DC_INT_MODE:
            case MASTER_AC_INT_MODE:
            case SLAVE_AC_INT_MODE:
            case AC_ADD_MODE: //AC-ADD
            case ACDC_ADD_MODE: //ACDC-ADD
            case SEQ_MODE: //SEQ
            default:
            {
                int32_buf2 = 0;
                int32_buf = 0;

                break;
            }
        }


        if ( int32_buf2 > int32_buf )
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.remote_OVP_start_time, CpuTimer2.InterruptCount )
                    > POWER_READY_REMOTE_OVP_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.remote_OVP_SW = 1;

                //轉態
                Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


                error_start_time_reset();

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***remote_OVP_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Machine_state -> ERROR_HANDLE_STATE0\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
        }
        else
        {
            remote_OVP_start_time_reset();
        }
    }

    /*------------------------------------------------*/
    //
    // Calibration error
    //

#endif //BYPASS_SCAN_ERROR_FOR_POWER_READY
}



/////////////////////////////////////////////////////////////////////
//
// PowerUp階段的異常掃描
//
void Power_Up_Error_Scan(void)
{
#ifndef BYPASS_SCAN_ERRROR_WHEN_POWER_UP

    if ( Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.fan_error_scan == 1 )
    {
        /*------------------------------------------------*/
        //
        // FANFAIL1
        //
        if ( Global_Variables.INPUT_FLAGS.bit.FANFAIL1 == 1 )
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL1_start_time, CpuTimer2.InterruptCount ) > POWER_UP_FANFAIL1_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL1_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.Fan_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;

                error_start_time_reset();

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***FANFAIL1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            FANFAIL1_start_time_reset();
        }


        /*------------------------------------------------*/
        //
        // FANFAIL2
        //
        if ( Global_Variables.INPUT_FLAGS.bit.FANFAIL2 == 1 )
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL2_start_time, CpuTimer2.InterruptCount ) > POWER_UP_FANFAIL2_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL2_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.Fan_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;

                error_start_time_reset();

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***FANFAIL2_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            FANFAIL2_start_time_reset();
        }


        /*------------------------------------------------*/
        //
        // FANFAIL (for 全寬機種)
        //
        if ( Global_Variables.INPUT_FLAGS.bit.FANFAIL == 1 )
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL_start_time, CpuTimer2.InterruptCount ) > POWER_UP_FANFAIL_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.FANFAIL1_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.Fan_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;

                error_start_time_reset();

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***FANFAIL1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            FANFAIL_start_time_reset();
        }

        /*------------------------------------------------*/
        //
        // AC FAIL
        //
#ifndef DEVELOP_SV60
        if (Global_Variables.INPUT_FLAGS.bit.ACFAIL == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.ACFAIL_start_time, CpuTimer2.InterruptCount ) > POWER_UP_ACFAIL_ERROR_TIME )
            {
                //
                // 開機中ACFAIL判斷為Power Input Abnomaly
                //
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_ON_ACFAIL_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.AC_Input_Check = 2;

                //
                // 設定EXTIO
                //
                Global_Variables.EXT_OUTPUT_FLAG.bit.POWER_ON = 0;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Output_ON_ACFAIL_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
            }

        }
        else
        {
            ACFAIL_start_time_reset();
        }
#endif
    }



    if ( Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.pfc_error_scan == 1 )
    {
        /*------------------------------------------------*/
        //
        // PFC UVP
        //
        if (Global_Variables.INPUT_FLAGS.bit.PFC_UVP == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_UVP_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP1_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_UVP1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            PFC_UVP_start_time_reset();
        }


        /*------------------------------------------------*/
        //
        // PFC UVP1 (for 全寬機種)
        //
        if (Global_Variables.INPUT_FLAGS.bit.PFC_UVP1 == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP1_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_UVP1_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP1_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_UVP1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            PFC_UVP1_start_time_reset();
        }




        /*------------------------------------------------*/
        //
        // PFC OVP
        //
        if (Global_Variables.INPUT_FLAGS.bit.PFC_OVP == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_OVP_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP1_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_OVP1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            PFC_OVP_start_time_reset();
        }

        /*------------------------------------------------*/
        //
        // PFC OVP1 (for 全寬機種)
        //
        if (Global_Variables.INPUT_FLAGS.bit.PFC_OVP1 == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP1_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_OVP1_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP1_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_OVP1_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            PFC_OVP1_start_time_reset();
        }


        /*------------------------------------------------*/
        //
        // PFC OTP
        //
        if (Global_Variables.INPUT_FLAGS.bit.PFC_OTP == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OTP_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_OTP_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.PFC_OTP_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_OTP_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            PFC_OTP_start_time_reset();
        }

    }


    if ( Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.pfc_error2_scan == 1 )
    {

        /*------------------------------------------------*/
        //
        // PFC UVP2 (for 全寬機種)
        //
        if (Global_Variables.INPUT_FLAGS.bit.PFC_UVP2 == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP2_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_UVP2_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_UVP2_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_UVP2_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            PFC_UVP2_start_time_reset();
        }


        /*------------------------------------------------*/
        //
        // PFC OVP2 (for 全寬機種)
        //
        if (Global_Variables.INPUT_FLAGS.bit.PFC_OVP2 == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP2_start_time, CpuTimer2.InterruptCount ) > POWER_UP_PFC_OVP2_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.PFC_OVP2_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.PFC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***PFC_OVP2_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            PFC_OVP2_start_time_reset();
        }


    }


    if ( Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.dcac_error_scan == 1 )
    {
        /*------------------------------------------------*/
        //
        // DCAC OTP
        //
        if ( Global_Variables.INPUT_VARIABLES.Temp_Sensor > DCAC_OTP_LEVEL )
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DCAC_OTP_start_time, CpuTimer2.InterruptCount ) > POWER_UP_DCAC_OTP_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.DCAC_OTP_SW = 1;

                Global_Variables.StartUp_State_Flag.bit.DCAC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***DCAC_OTP_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }
        }
        else
        {
            DCAC_OTP_start_time_reset();
        }

    }



    if ( Main_State_Module.POWER_UP_STATE_CONTROL_FLAG.bit.dcdc_error_scan == 1 )
    {
        /*------------------------------------------------*/
        //
        // DCDC OVP
        //
        if (Global_Variables.INPUT_FLAGS.bit.DC_OVP == 0)
        {
            if ( Calc_Elapsed_Time( Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DC_OVP_start_time, CpuTimer2.InterruptCount ) > POWER_UP_DC_OVP_ERROR_TIME )
            {
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.DC_OVP_HW = 1;

                Global_Variables.StartUp_State_Flag.bit.DCDC_Check = 2;

                //轉態
                Main_State_Module.Power_up_state = power_up_error_handle_state;


                error_start_time_reset();

                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***DC_OVP_HW***\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

                //
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > Power_up_state -> power_up_error_handle_state\n", CpuTimer2.InterruptCount ); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif

            }

        }
        else
        {
            DC_OVP_start_time_reset();
        }

    }
#endif //BYPASS_SCAN_ERRROR_WHEN_POWER_UP
}


/////////////////////////////////////////////////////////////////////
void FANFAIL1_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL1_start_time = CpuTimer2.InterruptCount;
}

/////////////////////////////////////////////////////////////////////
void FANFAIL2_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL2_start_time = CpuTimer2.InterruptCount;
}



/////////////////////////////////////////////////////////////////////
void PFC_UVP_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void PFC_OVP_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP_start_time = CpuTimer2.InterruptCount;
}

/////////////////////////////////////////////////////////////////////
void FANFAIL_start_time_reset(void) //for 全寬機種
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.FANFAIL_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void PFC_UVP1_start_time_reset(void) //for 全寬機種
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP1_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void PFC_OVP1_start_time_reset(void) //for 全寬機種
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP1_start_time = CpuTimer2.InterruptCount;
}

/////////////////////////////////////////////////////////////////////
void PFC_UVP2_start_time_reset(void) //for 全寬機種
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_UVP2_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void PFC_OVP2_start_time_reset(void) //for 全寬機種
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OVP2_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void PFC_OTP_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.PFC_OTP_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void ACFAIL_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.ACFAIL_start_time = CpuTimer2.InterruptCount;
}

/////////////////////////////////////////////////////////////////////
void DCAC_OTP_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DCAC_OTP_start_time = CpuTimer2.InterruptCount;
}

/////////////////////////////////////////////////////////////////////
void DC_OVP_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.DC_OVP_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void p_I_peak_ocp_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_ocp_start_time = CpuTimer0.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void n_I_peak_ocp_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_ocp_start_time = CpuTimer0.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
//void p_I_short_ocp_start_time_reset(void)
//{
//    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_short_ocp_start_time = CpuTimer0.InterruptCount; //改用timer0
//}

/////////////////////////////////////////////////////////////////////
//void n_I_short_ocp_start_time_reset(void)
//{
//    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_short_ocp_start_time = CpuTimer0.InterruptCount; //改用timer0
//}


/////////////////////////////////////////////////////////////////////
void I_rms_ocp_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_ocp_start_time = CpuTimer2.InterruptCount;
}

/////////////////////////////////////////////////////////////////////
void V_rms_ovp_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.V_rms_ovp_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void opp_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.opp_start_time = CpuTimer2.InterruptCount;
}


/////////////////////////////////////////////////////////////////////
void remote_OVP_start_time_reset(void)
{
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.remote_OVP_start_time = CpuTimer2.InterruptCount;
}




/////////////////////////////////////////////////////////////////////
void error_start_time_reset(void)
{
    Main_State_Module.error_start_time = CpuTimer2.InterruptCount;
}











//
// End of file
//
