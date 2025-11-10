//###########################################################################
//
// FILE:    SPI_Parser_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211026
//###########################################################################




#include "Global_VariableDefs.h"


////////////////////////////////////////////////////////////////////
//
//
//
void CMD_Parser_Process( volatile Uint16 *rx_buffer, volatile Uint16 *tx_buffer )
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
