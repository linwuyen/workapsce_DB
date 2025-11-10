//###########################################################################
//
// FILE:    Meas_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211116
//###########################################################################




#define DISPLAY_SAMPLING_RATE 72000.0 //(2.775us)
// 例如
// 設計更新率 0.1sec
// =>   0.1s / 2.775us
//    = 0.1s x 36000Hz
//    = 36000 count



//----------------------------------------------------
//
//
//
typedef enum
{
    meas_sampling_ready_state,
    wait_period_start_state,
    sampling_state_4k,
    sampling_state_1k
}MEAS_SAMPLING_STATE_REG;




//----------------------------------------------------
//
// V I raw op code
//
struct  MEAS_MODULE_FLAG_BITS {  // bit

    Uint16 VI_RAW_OP_CODE_Trig:1;       //0

};
union MEAS_MODULE_FLAG_REG {
   Uint16                         all;
   struct MEAS_MODULE_FLAG_BITS   bit;
};



//----------------------------------------------------
typedef struct MEAS_MODULE_VARIABLES_REG{

    union MEAS_MODULE_FLAG_REG Flags;
    MEAS_SAMPLING_STATE_REG Meas_sampling_state;
    Uint16 Sampling_index;
    Uint16 Total_Sample_Points;
    double V_square_sum;
    double I_square_sum;
    double V_sum;
    double I_sum;

    Uint32 MoveAvg_Count;
    Uint32 MoveAvg_Total_Count;

    float  MoveAvg_V_rms_sum;
    float  MoveAvg_V_rms;
    float  MoveAvg_V_avg_sum;
    float  MoveAvg_V_avg;
    float  MoveAvg_p_V_peak_sum;
    float  MoveAvg_p_V_peak;
    float  MoveAvg_n_V_peak_sum;
    float  MoveAvg_n_V_peak;
    float  MoveAvg_AC_V_rms_sum;
    float  MoveAvg_AC_V_rms;


    float  MoveAvg_I_rms_sum;
    float  MoveAvg_I_rms;
    float  MoveAvg_I_avg_sum;
    float  MoveAvg_I_avg;
    float  MoveAvg_p_I_peak_sum;
    float  MoveAvg_p_I_peak;
    float  MoveAvg_n_I_peak_sum;
    float  MoveAvg_n_I_peak;

    float  MoveAvg_P_sum;
    float  MoveAvg_P;

    float  Meas_Period;

}MEAS_MODULE_VARIABLES_REG;


extern  volatile MEAS_MODULE_VARIABLES_REG Meas_Module;



void Init_Meas_Module_Variables(void);
void Measurement_Calc_Process(void);

void Display_Move_Avg_Reset(void);

//
// End of file
//
