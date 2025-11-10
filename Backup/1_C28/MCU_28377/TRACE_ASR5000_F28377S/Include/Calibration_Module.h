//###########################################################################
//
// FILE:    Calibration_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211117
//###########################################################################

//500V
#define CALIBRATION_HI_LIMIT_RATIO 0.909091
#define CALIBRATION_LO_LIMIT_RATIO 0.909091
//250V
//#define CALIBRATION_HI_LIMIT_RATIO 0.454545
//#define CALIBRATION_LO_LIMIT_RATIO 0.454545
//100V
//#define CALIBRATION_HI_LIMIT_RATIO 0.181818
//#define CALIBRATION_LO_LIMIT_RATIO 0.181818


#define CALI_VOLTGE  0
#define CALI_CURRENT 1
#define CALI_EXT     2

//(0.01Hz)
#define CALIBRATION_FREQ_FSET_LOW       10000UL
#define CALIBRATION_FREQ_FSET_MID       105000UL
#define CALIBRATION_FREQ_FSET_HIGH      199999UL
#define CALIBRATION_FREQ_VSET           3500000

#ifdef DEVELOP_SV73
#define CALIBRATION_FREQ_VSET_HR        6360000
#else
#define CALIBRATION_FREQ_VSET_HR        7000000
#endif

#define CALIBRATION__IRMS_OFFSET_FSET   5000UL


//(ms)
#define CALI_DELAY_TIME  1000UL

//for 璸衡VREF DACタ把计
#define REFDAC_GAIN     100.0
// Design ratio for REF DAC to Output Voltage

#define REFDAC_m        10.0
// ( R_FB2 / R_G1 ) x V_REF

#define REFDAC_n        0.106383
// ( R_FB2 / R_G2 ) x V_REF



//----------------------------------------------------
typedef union
{
    float32 all;
    struct
    {
        Uint16 LSB:16;
        Uint16 MSB:16;
    };
}_cfloat32_MSB_LSB;

//----------------------------------------------------
typedef union
{
    int32 all;
    struct
    {
        Uint16 LSB:16;
        Uint16 MSB:16;
    };
}_cint32_MSB_LSB;


//----------------------------------------------------
//
// Calibration state
//
typedef enum
{
    Calibration_Ready_state,

    //
    // 筿溃タ Low Range
    //
    Calibration_Vdc_state0,
    Calibration_Vdc_state1,
    Calibration_Vdc_state,
    Calibration_DAC_state,
    Calibration_I_offset_state,
    Calibration_Freq_step0_state,
    Calibration_Freq_step1_state,
    Calibration_Freq_step2_state,
    Calibration_Freq_step3_state,
    Calibration_Freq_step4_state,
    Calibration_Freq_step5_state,
    Calibration_Freq_step6_state,
    Calibration_Freq_step7_state,
    Calibration_Freq_step8_state,

    //
    // 筿溃タ High Range
    //
    Calibration_Vdc_state0_hr,
    Calibration_Vdc_state1_hr,
    Calibration_Vdc_state_hr,
    Calibration_DAC_state_hr,
    Calibration_I_offset_state_hr,
    Calibration_Freq_step0_state_hr,
    Calibration_Freq_step1_state_hr,
    Calibration_Freq_step2_state_hr,
    Calibration_Freq_step3_state_hr,
    Calibration_Freq_step4_state_hr,
    Calibration_Freq_step5_state_hr,
    Calibration_Freq_step6_state_hr,
    Calibration_Freq_step7_state_hr,
    Calibration_Freq_step8_state_hr,


    //
    // 筿瑈タ Low Range
    //
    Calibration_Idc_state0,
    Calibration_Idc_state1,
    Calibration_Idc_state,
    Calibration_Rs_Cs_state,
    Calibration_Irms_offset_step0_state,
    Calibration_Irms_offset_step1_state,

    //
    // 筿瑈タ High Range
    //
    Calibration_Idc_state0_hr,
    Calibration_Idc_state1_hr,
    Calibration_Idc_state_hr,
    Calibration_Rs_Cs_state_hr,
    Calibration_Irms_offset_step0_state_hr,
    Calibration_Irms_offset_step1_state_hr,



    //
    // EXTタ
    //
    Calibration_ExtAD_offset_step0_state,
    Calibration_ExtAD_offset_step1_state,
    Calibration_ExtAD_offset_step2_state



}CALIBRATION_PROCESS_STATE_REG;




//----------------------------------------------------
//
//
struct CALIBRATION_CONTROL_FLAG_BITS {    // bits
    Uint16 rsv:1;              // 0

};

union CALIBRATION_CONTROL_FLAG_REG {
   Uint16                 all;
   struct CALIBRATION_CONTROL_FLAG_BITS bit;
};





//----------------------------------------------------
//
// タ瑈祘把计
//
typedef struct  CALIBRATION_PROCESS_VARIABLES_REG {

    _cint32_MSB_LSB    Meter_Value[3][6]; //0x94E
    Uint16            Meter_Value_LSB_buf[3][6]; //for cmd

    Uint16 ADS8353_CHA_Value[3][6];
    Uint16 ADS8353_CHB_Value[3][6];

    Uint16 REF_DAC_Value[3][6];
    float  DCV_set[3][6];

    //float  Vrms_Value[3];
    //float  Irms_Value[3];
    //float  Irms_no_Scale_Value[3];

    float  Vrms_Value[2][3];
    float  Irms_no_Scale_Value[2][3];


    float  p_V_peak_Value[3][6];
    float  n_V_peak_Value[3][6];
    float  p_I_peak_Value[3][6];
    float  n_I_peak_Value[3][6];

    _cfloat32_MSB_LSB ADS8353_CHA_avg_display;
    float            ADS8353_CHA_avg_display_shadow;

    _cfloat32_MSB_LSB ADS8353_CHB_avg_display;
    float            ADS8353_CHB_avg_display_shadow;

    _cfloat32_MSB_LSB ADS8353_CHB_minus_offset_rms_display;
    float            ADS8353_CHB_minus_offset_rms_display_shadow;


    double ExtAD_sum;
    double ExtAD_Total_Sample_Points;

    int32  Step_DCV_Set[2][6];

}CALIBRATION_PROCESS_VARIABLES_REG;



//----------------------------------------------------
//
//
//
typedef struct CALIBRATION_MODULE_VARIABLES_REG{

    CALIBRATION_PROCESS_STATE_REG Calibration_process_state;
    Uint32 Calibration_AutoStart_delay_time;
    union CALIBRATION_CONTROL_FLAG_REG Control_Flags;

    CALIBRATION_PROCESS_VARIABLES_REG Calibration_process_Variables;

}CALIBRATION_MODULE_VARIABLES_REG;



extern volatile CALIBRATION_MODULE_VARIABLES_REG Calibration_Module_Variables;



void Init_Calibration_Variables(void);

void Calibration_Process(void);







//
// End of file
//
