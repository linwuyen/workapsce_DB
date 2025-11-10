//###########################################################################
//
// FILE:    Output_Control_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211014
//###########################################################################

#define OCP_BYPASS_DELAY_TIME     1000 //(ms)

//
// 秖代干纕Delay
//
#define COMPENSATE_DELAY_TIME     18018 //(2.775us)


//
// DCV干纕ち传耞非
// DCV set, 虫 0.0001V
//
#define DCV_COMPENSATE_LOOP_CHANGE_LEVEL        1000000
#define DCV_COMPENSATE_LOOP_CHANGE_LEVEL_HR     2000000
//
// LEVEL > DCV set > -LEVEL
// DCV_Compensate_PID_Loop2(WAVE干纕)
//
// DCV set >= LEVEL ┪ DCV set <= -LEVEL
// DCV_Compensate_PID_Loop1(VREF干纕)

#define CHECK_CC_CP_DELAY_COUNT  200


//----------------------------------------------------
//
//
//
struct OUTPUT_CONTROL_FLAG_BITS {    // bits

    Uint16 Remote_Sense_Compensate_Calc:1;  //
    Uint16 Change_DCV:1;                    //
    Uint16 Change_ACV:1;                    //
    Uint16 Change_I_rms_limit:1;            //
    Uint16 Bypass_OCP_Flag:1;            //
};

union OUTPUT_CONTROL_FLAG_REG {
   Uint16                 all;
   struct OUTPUT_CONTROL_FLAG_BITS bit;
};




//----------------------------------------------------
//
// 北家Α篈诀
//
typedef enum
{
    CV_CONTROL_MODE,
    CC_CONTROL_MODE,
    CP_CONTROL_MODE

}POWER_CONTROL_MODE_STATE_REG;




//----------------------------------------------------
//
// DCV干纕北篈诀
//
typedef enum
{
    DCV_NO_COMPENSATE,
    DCV_DELAY_COMPENSATE,
    DCV_PID_COMPENSATE,
    DCV_STABLE_COMPENSATE

}DCV_COMPENSATE_STATE_REG;



//----------------------------------------------------
//
// AC+DC干纕北篈诀
//
typedef enum
{
    ACDCV_NO_COMPENSATE,
    ACDCV_DELAY_COMPENSATE,
    ACDCV_DCV_PID_COMPENSATE,
    ACDCV_DCV_STABLE_COMPENSATE,
    ACDCV_ACV_PID_COMPENSATE,
    ACDCV_ACV_STABLE_COMPENSATE,
    ACDCV_ACDCV_STABLE_NO_COMPENSATE
}ACDCV_COMPENSATE_STATE_REG;





//----------------------------------------------------
//
// MSキ颗干纕北篈诀
//
typedef enum
{
    MS_BALANCE_NO_COMPENSATE,
    MS_BALANCE_PID_COMPENSATE,
    MS_BALANCE_STABLE_COMPENSATE

}MS_BALANCE_COMPENSATE_STATE_REG;








//----------------------------------------------------
//
// CC干纕北篈诀
//
typedef enum
{
    CC_NO_COMPENSATE,
    CC_PID_COMPENSATE
    //CC_STABLE_COMPENSATE

}CC_COMPENSATE_STATE_REG;


//----------------------------------------------------
//
// CP干纕北篈诀
//
typedef enum
{
    CP_NO_COMPENSATE,
    CP_PID_COMPENSATE

}CP_COMPENSATE_STATE_REG;



//----------------------------------------------------
//
// OCP_Bypass_Control
//
typedef enum
{
    OCP_CHECK_NORMAL,
    OCP_CHECK_DELAY

}OCP_BYPASS_CONTROL_STATE_REG;



//----------------------------------------------------
typedef struct OUTPUT_CONTROL_MODULE_VARIABLES_REG{

    float Freq_Compensation_gain;

    float WAVE_Compensate_Offset;
    float WAVE_Compensate_Offset2;

    //
    // 干纕北柑耞Stable锣篈Counter
    //
    Uint16 Compensate_Control_Stable_Count;

    //
    // 耞锣篈CV, CC, CPCounter
    //
    Uint16 Control_Mode_Stable_Count;

    Uint32 OCP_Delay_start_time;

    Uint32 Compensate_Delay_Start_Time;

    Uint16 DCAC_ENA_shadow;

    Uint16 Check_CC_CP_Delay_Counter;

    union OUTPUT_CONTROL_FLAG_REG Flag;

    POWER_CONTROL_MODE_STATE_REG Power_Control_Mode;

    CC_COMPENSATE_STATE_REG CC_Compensate_State;
    CP_COMPENSATE_STATE_REG CP_Compensate_State;

    DCV_COMPENSATE_STATE_REG DCV_Compensate_State;
    ACDCV_COMPENSATE_STATE_REG ACDCV_Compensate_State;

    DCV_COMPENSATE_STATE_REG MS_DCV_Compensate_State;
    ACDCV_COMPENSATE_STATE_REG MS_ACDCV_Compensate_State;

    MS_BALANCE_COMPENSATE_STATE_REG MS_Balance_Compensate_State;


    OCP_BYPASS_CONTROL_STATE_REG OCP_Bypass_Control_State;

}OUTPUT_CONTROL_MODULE_VARIABLES_REG;




extern volatile OUTPUT_CONTROL_MODULE_VARIABLES_REG Output_Control_Module;



//void Input_Flags_Update(void);
void Output_Flags_Update(void);
void Input_Variables_Update(void);



//void EXT_VCA_Calc_Process(void);
void EXT_Freq_Calc_Process(void);

void Compensate_Calc_Process(void);


void DCV_Compensate_PID_Loop1(void);
void DCV_Compensate_PID_Loop2(void);
void ACDCV_Compensate_PID_Loop(void);

void MS_DCV_Compensate_PID_Loop1(void);
void MS_DCV_Compensate_PID_Loop2(void);
void MS_ACDCV_Compensate_PID_Loop(void);
void MS_Balance_Compensate_PID_Loop(void);



void CV_Compensate_Loop(void);

void CC_Compensate_PID_Loop(void);
void CP_Compensate_PID_Loop(void);


void Init_Output_Control_Module_Variables(void);

void Handle_DCAC_ON_OCP_Delay_Process(void);

//
// End of file
//
