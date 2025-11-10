//###########################################################################
//
// FILE:    SPI_Parser_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211026
//###########################################################################


//
// SPI Address Table
//


//
// General Section (Read Only)
//
#define C2000_Version_spi_addr                      0x400
#define Machine_Status_spi_addr                     0x0401
//#define Null_spi_addr                               0x0402
#define Vrms_MSB_spi_addr                           0x0403
#define Vrms_LSB_spi_addr                           0x0404
#define Irms_MSB_spi_addr                           0x0405
#define Irms_LSB_spi_addr                           0x0406
#define Ipeak_max_MSB_spi_addr                      0x0407
#define Ipeak_max_LSB_spi_addr                      0x0408
#define W_MSB_spi_addr                              0x0409
#define W_LSB_spi_addr                              0x040A
#define Ipeak_min_MSB_spi_addr                      0x040B
#define Ipeak_min_LSB_spi_addr                      0x040C
#define Vpeak_max_MSB_spi_addr                      0x040D
#define Vpeak_max_LSB_spi_addr                      0x040E
#define Vpeak_min_MSB_spi_addr                      0x040F
#define Vpeak_min_LSB_spi_addr                      0x0410
#define Vdc_avg_MSB_spi_addr                        0x0411
#define Vdc_avg_LSB_spi_addr                        0x0412
#define Idc_avg_MSB_spi_addr                        0x0413
#define Idc_avg_LSB_spi_addr                        0x0414
#define VA_MSB_spi_addr                             0x0415
#define VA_LSB_spi_addr                             0x0416
#define VAR_MSB_spi_addr                            0x0417
#define VAR_LSB_spi_addr                            0x0418
#define PF_spi_addr                                 0x0419
//#define Null_spi_addr                               0x041A
#define CF_spi_addr                                 0x041B
//#define Frequency_spi_addr                          0x041C
#define Ipeak_hold_MSB_spi_addr                     0x041D
#define Ipeak_hold_LSB_spi_addr                     0x041E
#define Calibration_State_spi_addr                  0x041F
//#define Machine_Series_spi_addr                     0x0420
#define Startup_State_spi_addr                      0x0421
//#define C2000_CMD_DATA_CheckSum_spi_addr            0x0422
#define Frequency_MSB_spi_addr                      0x0423
#define Frequency_LSB_spi_addr                      0x0424
#define Sequence_Step_CheckSum_spi_addr             0x0425
#define Sequence_Run_Step_spi_addr                  0x0426
#define Sequence_State_spi_addr                     0x0427
#define Sequence_Set_Step_spi_addr                  0x0428
#define C2000_Alarm_Status_spi_addr                 0x0429
#define C2000_Alarm_Status_2_spi_addr               0x042A
#define Temperature_MSB_spi_addr                    0x042B
#define Temperature_LSB_spi_addr                    0x042C
#define Vpeak_hold_MSB_spi_addr                     0x042D
#define Vpeak_hold_LSB_spi_addr                     0x042E
//#define Null_spi_addr                               0x042F

//
// Calibration Section (Read Only) (整區一起讀取)
// 需要以32bit為單位分配位址
//
#define Calibration_CheckSum_spi_addr                  0x0430
#define Calibration_Data_Version_spi_addr              0x0431

#define ADC_to_V_FB_Scale_P_LSB_spi_addr_lr            0x0432
#define ADC_to_V_FB_Scale_P_MSB_spi_addr_lr            0x0433
#define ADC_to_V_FB_Scale_P_LSB_spi_addr_hr            0x0434
#define ADC_to_V_FB_Scale_P_MSB_spi_addr_hr            0x0435

#define ADC_to_V_FB_Scale_N_LSB_spi_addr_lr            0x0436
#define ADC_to_V_FB_Scale_N_MSB_spi_addr_lr            0x0437
#define ADC_to_V_FB_Scale_N_LSB_spi_addr_hr            0x0438
#define ADC_to_V_FB_Scale_N_MSB_spi_addr_hr            0x0439

#define ADC_to_V_FB_Offset_LSB_spi_addr_lr             0x043A
#define ADC_to_V_FB_Offset_MSB_spi_addr_lr             0x043B
#define ADC_to_V_FB_Offset_LSB_spi_addr_hr             0x043C
#define ADC_to_V_FB_Offset_MSB_spi_addr_hr             0x043D

#define ADC_to_I_FB_Scale_P_LSB_spi_addr_lr            0x043E
#define ADC_to_I_FB_Scale_P_MSB_spi_addr_lr            0x043F
#define ADC_to_I_FB_Scale_P_LSB_spi_addr_hr            0x0440
#define ADC_to_I_FB_Scale_P_MSB_spi_addr_hr            0x0441

#define ADC_to_I_FB_Scale_N_LSB_spi_addr_lr            0x0442
#define ADC_to_I_FB_Scale_N_MSB_spi_addr_lr            0x0443
#define ADC_to_I_FB_Scale_N_LSB_spi_addr_hr            0x0444
#define ADC_to_I_FB_Scale_N_MSB_spi_addr_hr            0x0445

#define ADC_to_I_FB_Offset_LSB_spi_addr_lr             0x0446
#define ADC_to_I_FB_Offset_MSB_spi_addr_lr             0x0447
#define ADC_to_I_FB_Offset_LSB_spi_addr_hr             0x0448
#define ADC_to_I_FB_Offset_MSB_spi_addr_hr             0x0449

#define Cs_mpy_360000_LSB_spi_addr_lr                  0x044A
#define Cs_mpy_360000_MSB_spi_addr_lr                  0x044B
#define Cs_mpy_360000_LSB_spi_addr_hr                  0x044C
#define Cs_mpy_360000_MSB_spi_addr_hr                  0x044D

#define inv_Rs_LSB_spi_addr_lr                         0x044E
#define inv_Rs_MSB_spi_addr_lr                         0x044F
#define inv_Rs_LSB_spi_addr_hr                         0x0450
#define inv_Rs_MSB_spi_addr_hr                         0x0451

#define VSET_to_DAC_Scale_LSB_spi_addr_lr              0x0452
#define VSET_to_DAC_Scale_MSB_spi_addr_lr              0x0453
#define VSET_to_DAC_Scale_LSB_spi_addr_hr              0x0454
#define VSET_to_DAC_Scale_MSB_spi_addr_hr              0x0455

#define VSET_to_DAC_Offset_LSB_spi_addr_lr             0x0456
#define VSET_to_DAC_Offset_MSB_spi_addr_lr             0x0457
#define VSET_to_DAC_Offset_LSB_spi_addr_hr             0x0458
#define VSET_to_DAC_Offset_MSB_spi_addr_hr             0x0459

#define Freq_Compensation_coeff_a_LSB_spi_addr_lr      0x045A
#define Freq_Compensation_coeff_a_MSB_spi_addr_lr      0x045B
#define Freq_Compensation_coeff_a_LSB_spi_addr_hr      0x045C
#define Freq_Compensation_coeff_a_MSB_spi_addr_hr      0x045D

#define Freq_Compensation_coeff_b_LSB_spi_addr_lr      0x045E
#define Freq_Compensation_coeff_b_MSB_spi_addr_lr      0x045F
#define Freq_Compensation_coeff_b_LSB_spi_addr_hr      0x0460
#define Freq_Compensation_coeff_b_MSB_spi_addr_hr      0x0461

#define Freq_Compensation_coeff_c_LSB_spi_addr_lr      0x0462
#define Freq_Compensation_coeff_c_MSB_spi_addr_lr      0x0463
#define Freq_Compensation_coeff_c_LSB_spi_addr_hr      0x0464
#define Freq_Compensation_coeff_c_MSB_spi_addr_hr      0x0465

#define V_rms_Offset_LSB_spi_addr_lr                   0x0466
#define V_rms_Offset_MSB_spi_addr_lr                   0x0467
#define V_rms_Offset_LSB_spi_addr_hr                   0x0468
#define V_rms_Offset_MSB_spi_addr_hr                   0x0469

#define I_rms_Offset_LSB_spi_addr_lr                   0x046A
#define I_rms_Offset_MSB_spi_addr_lr                   0x046B
#define I_rms_Offset_LSB_spi_addr_hr                   0x046C
#define I_rms_Offset_MSB_spi_addr_hr                   0x046D

#define ADC_to_EXTIN_L1_Scale_LSB_spi_addr_lr          0x046E
#define ADC_to_EXTIN_L1_Scale_MSB_spi_addr_lr          0x046F
#define ADC_to_EXTIN_L1_Scale_LSB_spi_addr_hr          0x0470
#define ADC_to_EXTIN_L1_Scale_MSB_spi_addr_hr          0x0471

#define ADC_to_EXTIN_L1_offset_LSB_spi_addr_lr         0x0472
#define ADC_to_EXTIN_L1_offset_MSB_spi_addr_lr         0x0473
#define ADC_to_EXTIN_L1_offset_LSB_spi_addr_hr         0x0474
#define ADC_to_EXTIN_L1_offset_MSB_spi_addr_hr         0x0475

#define p_V_peak_Offset_LSB_spi_addr_lr                0x0476
#define p_V_peak_Offset_MSB_spi_addr_lr                0x0477
#define p_V_peak_Offset_LSB_spi_addr_hr                0x0478
#define p_V_peak_Offset_MSB_spi_addr_hr                0x0479

#define n_V_peak_Offset_LSB_spi_addr_lr                0x047A
#define n_V_peak_Offset_MSB_spi_addr_lr                0x047B
#define n_V_peak_Offset_LSB_spi_addr_hr                0x047C
#define n_V_peak_Offset_MSB_spi_addr_hr                0x047D

#define p_I_peak_Offset_LSB_spi_addr_lr                0x047E
#define p_I_peak_Offset_MSB_spi_addr_lr                0x047F
#define p_I_peak_Offset_LSB_spi_addr_hr                0x0480
#define p_I_peak_Offset_MSB_spi_addr_hr                0x0481

#define n_I_peak_Offset_LSB_spi_addr_lr                0x0482
#define n_I_peak_Offset_MSB_spi_addr_lr                0x0483
#define n_I_peak_Offset_LSB_spi_addr_hr                0x0484
#define n_I_peak_Offset_MSB_spi_addr_hr                0x0485
//#define Null_spi_addr                               0x04FF


#define V_and_I_Raw_Data_Counter_spi_addr           0x0701
#define External_Input_spi_addr                     0x0702
#define External_Output_spi_addr                    0x0703
#define External_AD_spi_addr                        0x0704
//#define V_Raw_Data_Address_Page_spi_addr            0x0705
//#define I_Raw_Data_Address_Page_spi_addr            0x0706
#define Wave_Data_Address_Page_spi_addr             0x0707
#define External_Frequency_H_spi_addr               0x0708
#define External_Frequency_L_spi_addr               0x0709
//#define Null_spi_addr                               0x070A

#define Update_C2000_Setting_spi_addr               0x0800
#define V_and_I_Raw_Data_Operate_spi_addr           0x0801
//#define Null_spi_addr                               0x0802

#define Print_All_Cali_Variable_spi_addr            0x0888



//
// Write Section
//
#define Output_ON_OFF_spi_addr                      0x0900

#define Fan_Speed_spi_addr                          0x0901

#define Output_Waveform_Set_spi_addr                0x0902
#define Output_Mode_Set_spi_addr                    0x0903
#define Output_Range_Set_spi_addr                   0x0904
//#define I_RMS_Limiter_Set_spi_addr                  0x0905
//#define p_I_Peak_Limiter_Set_spi_addr                0x0906
//#define n_I_Peak_Limiter_Set_spi_addr                0x0907
//#define EXTIO_Extend_Mode_Enable_spi_addr           0x0908
#define Ipeak_hold_Clear_spi_addr                   0x0909
#define T_Ipeak_hold_spi_addr                       0x090A
//#define Null_spi_addr                               0x090B
#define I_RMS_Limiter_ON_OFF_spi_addr               0x090E
#define I_Peak_Limiter_ON_OFF_spi_addr              0x090F
#define Frequency_Set_MSB_spi_addr                  0x0910
#define Frequency_Set_LSB_spi_addr                  0x0911
#define Vpeak_hold_Clear_spi_addr                   0x0912
#define T_Vpeak_hold_spi_addr                       0x0913
//#define Null_spi_addr                               0x0914
//#define Measure_Average_Mode_spi_addr               0x0915
//#define Null_spi_addr                               0x0916
//#define I_rms_limit_Kp_spi_addr                     0x0917
#define External_IO_Control_ON_OFF_spi_addr         0x0918
#define Zero_Change_Phase_ON_OFF_spi_addr           0x0919
#define Start_Phase_Set_spi_addr                    0x0920
#define Start_Phase_ON_OFF_spi_addr                 0x0921
#define Stop_Phase_Set_spi_addr                     0x0922
#define Stop_Phase_ON_OFF_spi_addr                  0x0923
#define Output_relay_Enable_Disable_spi_addr        0x0924
//#define OCP_ON_OFF_spi_addr                         0x0925
#define V_Measure_Compensation_ON_OFF_spi_addr      0x0926
#define V_Slew_Rate_spi_addr                        0x0927
//#define Fan_Slew_Rate_spi_addr                      0x0928


#define Frequency_Compensation_ON_OFF_spi_addr      0x0929
#define V_Slew_Rate_mode_spi_addr                   0x092A
#define SYNC_Frequency_Source_spi_addr              0x092B
#define Remote_Sense_ON_OFF_spi_addr                0x092C
#define CPU_State_spi_addr                          0x092D
//#define Null_spi_addr                               0x092E
#define I_RMS_Limiter_Set_MSB_spi_addr              0x0931
#define I_RMS_Limiter_Set_LSB_spi_addr              0x0932
#define p_I_Peak_Limiter_Set_MSB_spi_addr           0x0933
#define p_I_Peak_Limiter_Set_LSB_spi_addr           0x0934
#define n_I_Peak_Limiter_Set_MSB_spi_addr           0x0935
#define n_I_Peak_Limiter_Set_LSB_spi_addr           0x0936
#define Ext_Gain_Set_MSB_spi_addr                   0x0937
#define Ext_Gain_Set_LSB_spi_addr                   0x0938
#define DC_Voltage_Set_MSB_spi_addr                 0x0939
#define DC_Voltage_Set_LSB_spi_addr                 0x093A
#define AC_Voltage_Set_MSB_spi_addr                 0x093B
#define AC_Voltage_Set_LSB_spi_addr                 0x093C
#define VCA_Gain_Set_MSB_spi_addr                   0x093D
#define VCA_Gain_Set_LSB_spi_addr                   0x093E
//#define Null_spi_addr                               0x093F
//#define Ext_Gain_Set_spi_addr                       0x0940
//#define DC_Voltage_Set_spi_addr                     0x0941
#define SYNC_Mode_Phase_Delay_spi_addr              0x0942
//#define Null_spi_addr                               0x0943
#define Zero_Cross_Width_Set_spi_addr               0x0944
//#define Null_spi_addr                               0x0945
#define Zero_Cross_Option_spi_addr                  0x0946

#ifdef DEVELOP_SV76
#define SQU_Duty_Set_spi_addr                       0x0947
#define V_Rise_Time_Set_MSB_spi_addr                0x0948
#define V_Rise_Time_Set_LSB_spi_addr                0x0949
#else
//#define Null_spi_addr                             0x0947
//#define Null_spi_addr                             0x0948
//#define Null_spi_addr                             0x0949
#endif

#define ACV_Unit_spi_addr                           0x094A
//#define Null_spi_addr                               0x094B
#define Calibration_Step_spi_addr                   0x094D
#define Calibration_Value_to_C2000_MSB_spi_addr     0x094E
#define Calibration_Value_to_C2000_LSB_spi_addr     0x094F
#define Calibration_Recall_default_spi_addr         0x0950
//#define Null_spi_addr                               0x0951
#define Alarm_Clear_spi_addr                        0x0952
#define Model_Type_spi_addr                         0x0953
#define Calibration_Cancel_spi_addr                 0x0954
#define Calibration_Save_spi_addr                   0x0955
//#define V_Raw_Data_Address_Page_spi_addr_w          0x0956
//#define I_Raw_Data_Address_Page_spi_addr_w          0x0957
#define Wave_Data_Address_Page_spi_addr_w           0x0958
//#define Null_spi_addr                               0x0959
#define Sequence_Step_spi_addr                      0x0960
#define Sequence_Config_State_1_spi_addr            0x0961
#define Sequence_Config_State_2_spi_addr            0x0962
//#define Sequence_dc_value_spi_addr                  0x0963
//#define Sequence_ac_value_spi_addr                  0x0964
//#define Sequence_freq_value_spi_addr                0x0965
#define Sequence_star_phase_spi_addr                0x0966
#define Sequence_stop_phase_spi_addr                0x0967
#define Sequence_jump_spi_addr                      0x0968
#define Sequence_jump_count_spi_addr                0x0969
#define Sequence_branch_step_1_spi_addr             0x096A
#define Sequence_branch_step_2_spi_addr             0x096B
#define Sequence_step_time_MSB_spi_addr             0x096C
#define Sequence_step_time_LSB_spi_addr             0x096D
#define Sequence_start_spi_addr                     0x096E
#define Sequence_stop_spi_addr                      0x096F
#define Sequence_hold_spi_addr                      0x0970
#define Sequence_branch_1_spi_addr                  0x0971
#define Sequence_branch_2_spi_addr                  0x0972
#define Sequence_Mode_spi_addr                      0x0973
#define Clear_Sequence_CheckSum_spi_addr            0x0974
#define SEQ_CheckSum_Refresh_spi_addr               0x0975
//#define Null_spi_addr                               0x0976
#define Sequence_Step_Clear_spi_addr                0x0977
#define Sequence_Freq_Set_MSB_spi_addr              0x0978
#define Sequence_Freq_Set_LSB_spi_addr              0x0979
#define Sequence_dc_value_MSB_spi_addr              0x097A
#define Sequence_dc_value_LSB_spi_addr              0x097B
#define Sequence_ac_value_MSB_spi_addr              0x097C
#define Sequence_ac_value_LSB_spi_addr              0x097D
#define Sequence_SQU_Duty_spi_addr                  0x097E
//#define Null_spi_addr                               0x097F

#ifdef DEVELOP_SV76
#else
#define SQU_Duty_Set_spi_addr                       0x09B0
#define V_Rise_Time_Set_MSB_spi_addr                0x09B1
#define V_Rise_Time_Set_LSB_spi_addr                0x09B2
#endif

#define Waveform_CF_Set_LSB_ARB1_spi_addr           0x09E0
#define Waveform_CF_Set_MSB_ARB1_spi_addr           0x09E1
#define Waveform_CF_Set_LSB_ARB2_spi_addr           0x09E2
#define Waveform_CF_Set_MSB_ARB2_spi_addr           0x09E3
#define Waveform_CF_Set_LSB_ARB3_spi_addr           0x09E4
#define Waveform_CF_Set_MSB_ARB3_spi_addr           0x09E5
#define Waveform_CF_Set_LSB_ARB4_spi_addr           0x09E6
#define Waveform_CF_Set_MSB_ARB4_spi_addr           0x09E7
#define Waveform_CF_Set_LSB_ARB5_spi_addr           0x09E8
#define Waveform_CF_Set_MSB_ARB5_spi_addr           0x09E9
#define Waveform_CF_Set_LSB_ARB6_spi_addr           0x09EA
#define Waveform_CF_Set_MSB_ARB6_spi_addr           0x09EB
#define Waveform_CF_Set_LSB_ARB7_spi_addr           0x09EC
#define Waveform_CF_Set_MSB_ARB7_spi_addr           0x09ED
#define Waveform_CF_Set_LSB_ARB8_spi_addr           0x09EE
#define Waveform_CF_Set_MSB_ARB8_spi_addr           0x09EF
#define Waveform_CF_Set_LSB_ARB9_spi_addr           0x09F0
#define Waveform_CF_Set_MSB_ARB9_spi_addr           0x09F1
#define Waveform_CF_Set_LSB_ARB10_spi_addr          0x09F2
#define Waveform_CF_Set_MSB_ARB10_spi_addr          0x09F3
#define Waveform_CF_Set_LSB_ARB11_spi_addr          0x09F4
#define Waveform_CF_Set_MSB_ARB11_spi_addr          0x09F5
#define Waveform_CF_Set_LSB_ARB12_spi_addr          0x09F6
#define Waveform_CF_Set_MSB_ARB12_spi_addr          0x09F7
#define Waveform_CF_Set_LSB_ARB13_spi_addr          0x09F8
#define Waveform_CF_Set_MSB_ARB13_spi_addr          0x09F9
#define Waveform_CF_Set_LSB_ARB14_spi_addr          0x09FA
#define Waveform_CF_Set_MSB_ARB14_spi_addr          0x09FB
#define Waveform_CF_Set_LSB_ARB15_spi_addr          0x09FC
#define Waveform_CF_Set_MSB_ARB15_spi_addr          0x09FD
#define Waveform_CF_Set_LSB_ARB16_spi_addr          0x09FE
#define Waveform_CF_Set_MSB_ARB16_spi_addr          0x09FF
#define Waveform_CF_Set_LSB_SIN_spi_addr            0x0A00
#define Waveform_CF_Set_MSB_SIN_spi_addr            0x0A01
#define Waveform_CF_Set_LSB_SQU_spi_addr            0x0A02
#define Waveform_CF_Set_MSB_SQU_spi_addr            0x0A03
#define Waveform_CF_Set_LSB_TRI_spi_addr            0x0A04
#define Waveform_CF_Set_MSB_TRI_spi_addr            0x0A05

//
// Calibration Section (Write) (整區一起寫入)
// 需要以32bit為單位分配位址
//
#define Calibration_CheckSum_spi_addr_w                  0x0A10
#define Calibration_Data_Version_spi_addr_w              0x0A11

#define ADC_to_V_FB_Scale_P_LSB_spi_addr_w_lr            0x0A12
#define ADC_to_V_FB_Scale_P_MSB_spi_addr_w_lr            0x0A13
#define ADC_to_V_FB_Scale_P_LSB_spi_addr_w_hr            0x0A14
#define ADC_to_V_FB_Scale_P_MSB_spi_addr_w_hr            0x0A15

#define ADC_to_V_FB_Scale_N_LSB_spi_addr_w_lr            0x0A16
#define ADC_to_V_FB_Scale_N_MSB_spi_addr_w_lr            0x0A17
#define ADC_to_V_FB_Scale_N_LSB_spi_addr_w_hr            0x0A18
#define ADC_to_V_FB_Scale_N_MSB_spi_addr_w_hr            0x0A19

#define ADC_to_V_FB_Offset_LSB_spi_addr_w_lr             0x0A1A
#define ADC_to_V_FB_Offset_MSB_spi_addr_w_lr             0x0A1B
#define ADC_to_V_FB_Offset_LSB_spi_addr_w_hr             0x0A1C
#define ADC_to_V_FB_Offset_MSB_spi_addr_w_hr             0x0A1D

#define ADC_to_I_FB_Scale_P_LSB_spi_addr_w_lr            0x0A1E
#define ADC_to_I_FB_Scale_P_MSB_spi_addr_w_lr            0x0A1F
#define ADC_to_I_FB_Scale_P_LSB_spi_addr_w_hr            0x0A20
#define ADC_to_I_FB_Scale_P_MSB_spi_addr_w_hr            0x0A21

#define ADC_to_I_FB_Scale_N_LSB_spi_addr_w_lr            0x0A22
#define ADC_to_I_FB_Scale_N_MSB_spi_addr_w_lr            0x0A23
#define ADC_to_I_FB_Scale_N_LSB_spi_addr_w_hr            0x0A24
#define ADC_to_I_FB_Scale_N_MSB_spi_addr_w_hr            0x0A25

#define ADC_to_I_FB_Offset_LSB_spi_addr_w_lr             0x0A26
#define ADC_to_I_FB_Offset_MSB_spi_addr_w_lr             0x0A27
#define ADC_to_I_FB_Offset_LSB_spi_addr_w_hr             0x0A28
#define ADC_to_I_FB_Offset_MSB_spi_addr_w_hr             0x0A29

#define Cs_mpy_360000_LSB_spi_addr_w_lr                  0x0A2A
#define Cs_mpy_360000_MSB_spi_addr_w_lr                  0x0A2B
#define Cs_mpy_360000_LSB_spi_addr_w_hr                  0x0A2C
#define Cs_mpy_360000_MSB_spi_addr_w_hr                  0x0A2D

#define inv_Rs_LSB_spi_addr_w_lr                         0x0A2E
#define inv_Rs_MSB_spi_addr_w_lr                         0x0A2F
#define inv_Rs_LSB_spi_addr_w_hr                         0x0A30
#define inv_Rs_MSB_spi_addr_w_hr                         0x0A31

#define VSET_to_DAC_Scale_LSB_spi_addr_w_lr              0x0A32
#define VSET_to_DAC_Scale_MSB_spi_addr_w_lr              0x0A33
#define VSET_to_DAC_Scale_LSB_spi_addr_w_hr              0x0A34
#define VSET_to_DAC_Scale_MSB_spi_addr_w_hr              0x0A35

#define VSET_to_DAC_Offset_LSB_spi_addr_w_lr             0x0A36
#define VSET_to_DAC_Offset_MSB_spi_addr_w_lr             0x0A37
#define VSET_to_DAC_Offset_LSB_spi_addr_w_hr             0x0A38
#define VSET_to_DAC_Offset_MSB_spi_addr_w_hr             0x0A39

#define Freq_Compensation_coeff_a_LSB_spi_addr_w_lr      0x0A3A
#define Freq_Compensation_coeff_a_MSB_spi_addr_w_lr      0x0A3B
#define Freq_Compensation_coeff_a_LSB_spi_addr_w_hr      0x0A3C
#define Freq_Compensation_coeff_a_MSB_spi_addr_w_hr      0x0A3D

#define Freq_Compensation_coeff_b_LSB_spi_addr_w_lr      0x0A3E
#define Freq_Compensation_coeff_b_MSB_spi_addr_w_lr      0x0A3F
#define Freq_Compensation_coeff_b_LSB_spi_addr_w_hr      0x0A40
#define Freq_Compensation_coeff_b_MSB_spi_addr_w_hr      0x0A41

#define Freq_Compensation_coeff_c_LSB_spi_addr_w_lr      0x0A42
#define Freq_Compensation_coeff_c_MSB_spi_addr_w_lr      0x0A43
#define Freq_Compensation_coeff_c_LSB_spi_addr_w_hr      0x0A44
#define Freq_Compensation_coeff_c_MSB_spi_addr_w_hr      0x0A45

#define V_rms_Offset_LSB_spi_addr_w_lr                   0x0A46
#define V_rms_Offset_MSB_spi_addr_w_lr                   0x0A47
#define V_rms_Offset_LSB_spi_addr_w_hr                   0x0A48
#define V_rms_Offset_MSB_spi_addr_w_hr                   0x0A49

#define I_rms_Offset_LSB_spi_addr_w_lr                   0x0A4A
#define I_rms_Offset_MSB_spi_addr_w_lr                   0x0A4B
#define I_rms_Offset_LSB_spi_addr_w_hr                   0x0A4C
#define I_rms_Offset_MSB_spi_addr_w_hr                   0x0A4D

#define ADC_to_EXTIN_L1_Scale_LSB_spi_addr_w_lr          0x0A4E
#define ADC_to_EXTIN_L1_Scale_MSB_spi_addr_w_lr          0x0A4F
#define ADC_to_EXTIN_L1_Scale_LSB_spi_addr_w_hr          0x0A50
#define ADC_to_EXTIN_L1_Scale_MSB_spi_addr_w_hr          0x0A51

#define ADC_to_EXTIN_L1_offset_LSB_spi_addr_w_lr         0x0A52
#define ADC_to_EXTIN_L1_offset_MSB_spi_addr_w_lr         0x0A53
#define ADC_to_EXTIN_L1_offset_LSB_spi_addr_w_hr         0x0A54
#define ADC_to_EXTIN_L1_offset_MSB_spi_addr_w_hr         0x0A55

#define p_V_peak_Offset_LSB_spi_addr_w_lr                0x0A56
#define p_V_peak_Offset_MSB_spi_addr_w_lr                0x0A57
#define p_V_peak_Offset_LSB_spi_addr_w_hr                0x0A58
#define p_V_peak_Offset_MSB_spi_addr_w_hr                0x0A59

#define n_V_peak_Offset_LSB_spi_addr_w_lr                0x0A5A
#define n_V_peak_Offset_MSB_spi_addr_w_lr                0x0A5B
#define n_V_peak_Offset_LSB_spi_addr_w_hr                0x0A5C
#define n_V_peak_Offset_MSB_spi_addr_w_hr                0x0A5D

#define p_I_peak_Offset_LSB_spi_addr_w_lr                0x0A5E
#define p_I_peak_Offset_MSB_spi_addr_w_lr                0x0A5F
#define p_I_peak_Offset_LSB_spi_addr_w_hr                0x0A60
#define p_I_peak_Offset_MSB_spi_addr_w_hr                0x0A61

#define n_I_peak_Offset_LSB_spi_addr_w_lr                0x0A62
#define n_I_peak_Offset_MSB_spi_addr_w_lr                0x0A63
#define n_I_peak_Offset_LSB_spi_addr_w_hr                0x0A64
#define n_I_peak_Offset_MSB_spi_addr_w_hr                0x0A65

//#define Null_spi_addr                               0x0AFF
//
// !!注意!!
// 如果新增, 則需要調整"Check_Cali_CheckSum"事件的觸發點

#define V_Raw_Data_spi_addr                         0x1000
#define I_Raw_Data_spi_addr                         0x2000
#define Wave_Data_spi_addr                          0x3000


void CMD_Parser_Process( volatile Uint16 *rx_buffer, volatile Uint16 *tx_buffer);


//
// End of file
//
