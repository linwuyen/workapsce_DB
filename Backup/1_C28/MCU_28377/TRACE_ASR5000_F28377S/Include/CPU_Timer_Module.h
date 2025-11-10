//###########################################################################
//
// FILE:    CPU_Timer_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210901
//###########################################################################


/////////////////////////////////////////////////////////////////////

//
// F28377S CPU頻率, MHz
//
#define CPU_FREQ    200

//
// CPU timer0 主中斷週期, us
//
#define CPU0_PERIOD 2.775

//
// CPU timer1 主中斷週期, us
//
#define CPU1_PERIOD 4

//
// CPU timer2 主中斷週期, us
//
#define CPU2_PERIOD 1000

//
// CPU timer0 主中斷頻率, Hz
// = 1 / 2.775us
//
#define CPU0_FREQ 360360.36


//
// 100us / CPU_timer0週期 2.775us
//
//#define _100uSEC_DIV_CPU0_PERIOD 36.03603604


//
// 100us / ( CPU_timer0週期 2.775us x 排程數4 )
//
//#define _100uSEC_DIV_SEQ_RUN_PERIOD 9.009009
#define _100uSEC_DIV_SEQ_RUN_PERIOD 3.003003


//
// EXT AD取樣頻率, Hz
//
// EXT AD的程式執行頻率在排程裡, 採樣頻率 = 360kHz / 排程數
//
#define EXTAD_SAMPLE_FREQ     120120.12 //Hz
//#define EXTAD_SAMPLE_FREQ       90090.09 //Hz

//
// c1防止溢位的上限值
// 100 / 0.1Hz / 2.775us
//
//#define C1_FINAL_VALUE 360360360
// 100 / 10Hz / 2.775us
//
#define C1_FINAL_VALUE 3603604


//
// 1000us / 2.775us = 360.360
//
#define CPU_TIMER0_1ms 360


//
// 10000us / 2.775us = 3603.60
//
#define CPU_TIMER0_10ms 3604


//
// 1000000us / 2.775us = 360360.36
//
#define CPU_TIMER0_1s 360360




//----------------------------------------------------
//
// CPU_TIMER0排程01狀態機
// 排程調整則相對應的程式counter要確認
//
typedef enum
{
    CPU_TIMER0_SCHEDULE01_01_STATE,
    CPU_TIMER0_SCHEDULE01_02_STATE,
    CPU_TIMER0_SCHEDULE01_03_STATE,
    CPU_TIMER0_SCHEDULE01_04_STATE

}CPU_TIMER0_SCHEDULE01_STATE_REG;



//----------------------------------------------------
//
//
//
typedef enum
{
    MS_SDRAM_Sending_Ready,
    MS_SDRAM_Sending_Process0,
    MS_SDRAM_Sending_Process0a,
    MS_SDRAM_Sending_Process0b,
    MS_SDRAM_Sending_Process0c,
    MS_SDRAM_Sending_Process0d,
    MS_SDRAM_Sending_Process1,
    MS_SDRAM_Sending_Process1a,
    MS_SDRAM_Sending_Process1b,
    MS_SDRAM_Sending_Process2,
    MS_SDRAM_Sending_Process2a,
    MS_SDRAM_Sending_Process2b,
    MS_SDRAM_Sending_Process3,
    MS_SDRAM_Sending_Process3a,
    MS_SDRAM_Sending_Process3b,
    MS_SDRAM_Sending_Process4

}MS_SDRAM_SENDING_STATE_REG;


//----------------------------------------------------
//
//
//
typedef enum
{
    MS_SDRAM_Receving_Ready,
    MS_SDRAM_Receving_Process0,
    MS_SDRAM_Receving_Process1,
    MS_SDRAM_Receving_Process2,
    MS_SDRAM_Receving_Process3,
    MS_SDRAM_Receving_Process4

}MS_SDRAM_RECEVING_STATE_REG;




//----------------------------------------------------
//
// SEQ ON排程狀態機
// 排程調整則相對應的程式counter要確認
//
typedef enum
{
    SEQ_OUTPUT_ON_STATE0,
    SEQ_OUTPUT_ON_STATE1,
    SEQ_OUTPUT_ON_STATE2,
    SEQ_OUTPUT_ON_STATE3,


    SEQ_ACDC_OUTPUT_ON_STATE0,
    SEQ_ACDC_OUTPUT_ON_STATE1,
    SEQ_ACDC_OUTPUT_ON_STATE2,
    SEQ_ACDC_OUTPUT_ON_STATE3,

    SEQ_AC_OUTPUT_ON_STATE0,
    SEQ_AC_OUTPUT_ON_STATE1,
    SEQ_AC_OUTPUT_ON_STATE2,
    SEQ_AC_OUTPUT_ON_STATE3,

    SEQ_DC_OUTPUT_ON_STATE0,
    SEQ_DC_OUTPUT_ON_STATE1,
    SEQ_DC_OUTPUT_ON_STATE2,
    SEQ_DC_OUTPUT_ON_STATE3



}SEQ_OUTPUT_ON_STATE_REG;



//----------------------------------------------------
//
// Slew Rate狀態機
//
typedef enum
{
    WAVE_SLEWRATE_STATE,
    NO_SLEWRATE_STATE

}SLEW_RATE_STATE_REG;


//----------------------------------------------------
//
// Zero Cross Trig IO Out 狀態機
//
typedef enum
{
    ZERO_CROSS_INIT_STATE,
    ZERO_CROSS_READY_STATE,
    ZERO_CROSS_TRIG_HOLD_STATE

}ZERO_CROSS_STATE_REG;




//----------------------------------------------------
//
// DAC輸出控制子狀態
//
typedef enum
{
    //
    // READY
    //
    CPU_TIMER0_OUTPUT_READY_STATE,


    //
    // AC-INT / DC-INT / AC+DC-INT
    //
    ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0,
    ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1,
    ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE,
    ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0,
    //ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1,
    ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2,
    ACDC_INT_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE,

    ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE0,
    ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE1,
    ACDC_INT_CPU_TIMER0_OUTPUT_V_RISE_STATE2,

    //DC_INT_CPU_TIMER0_OUTPUT_ON_STATE,
    //DC_INT_CPU_TIMER0_OUTPUT_CHANGE_STATE,

    //
    // MS Master
    // AC-INT / DC-INT / AC+DC-INT
    //
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0_Shadow,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0_Shadow,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE1,
    MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE,

#ifdef BYPASS
    MS_Master_DC_INT_CPU_TIMER0_OUTPUT_ON_STATE,
    MS_Master_DC_INT_CPU_TIMER0_OUTPUT_CHANGE_STATE0,
    MS_Master_DC_INT_CPU_TIMER0_OUTPUT_CHANGE_STATE1,
    MS_Master_DC_INT_CPU_TIMER0_OUTPUT_CHANGE_STATE2,
    MS_Master_DC_INT_CPU_TIMER0_OUTPUT_OFF_STATE,
#endif


    //
    // MS Slave
    // AC-INT / DC-INT / AC+DC-INT
    //
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0,
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE1,
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE2,
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_ON_STATE,
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0,
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1,
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2,
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0,
    MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE1,

#ifdef BYPASS
    MS_Slave_DC_INT_CPU_TIMER0_OUTPUT_ON_STATE0,
    MS_Slave_DC_INT_CPU_TIMER0_OUTPUT_ON_STATE1,
    MS_Slave_DC_INT_CPU_TIMER0_OUTPUT_CHANGE_STATE0,
    MS_Slave_DC_INT_CPU_TIMER0_OUTPUT_CHANGE_STATE1,
    MS_Slave_DC_INT_CPU_TIMER0_OUTPUT_CHANGE_STATE2,
    MS_Slave_DC_INT_CPU_TIMER0_OUTPUT_OFF_STATE,
#endif




    //
    // AC-SYNC / AC+DC-SYNC
    //
    ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE0,
    ACDC_SYNC_CPU_TIMER0_OUTPUT_START_PHASE_STATE1,
    ACDC_SYNC_CPU_TIMER0_OUTPUT_ON_STATE,
    ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0,
    ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1,
    ACDC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2,
    ACDC_SYNC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0,
    ACDC_SYNC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1,

    //DC_SYNC_CPU_TIMER0_OUTPUT_ON_STATE,
    //DC_SYNC_CPU_TIMER0_OUTPUT_CHANGE_STATE,

    //
    // AC-EXT / AC+DC-EXT
    //
    ACDC_EXT_CPU_TIMER0_OUTPUT_ON_STATE,

    //
    // AC-ADD / AC+DC-ADD
    //
    ACDC_ADD_CPU_TIMER0_OUTPUT_START_PHASE_STATE0,
    ACDC_ADD_CPU_TIMER0_OUTPUT_START_PHASE_STATE1,
    ACDC_ADD_CPU_TIMER0_OUTPUT_ON_STATE,
    ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0,
    //ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1,
    ACDC_ADD_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE2,
    ACDC_ADD_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE,


    //
    // AC-VCA
    //
    AC_VCA_CPU_TIMER0_OUTPUT_START_PHASE_STATE0,
    AC_VCA_CPU_TIMER0_OUTPUT_START_PHASE_STATE1,
    AC_VCA_CPU_TIMER0_OUTPUT_ON_STATE,
    AC_VCA_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0,
    AC_VCA_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1,
    AC_VCA_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE,


    //
    // SEQ
    //
    SEQ_CPU_TIMER0_OUTPUT_READY_STATE,
    SEQ_CPU_TIMER0_OUTPUT_STEP_INIT_STATE,
    SEQ_CPU_TIMER0_OUTPUT_ON_STATE,
    SEQ_CPU_TIMER0_OUTPUT_HOLD_STATE,
    SEQ_CPU_TIMER0_OUTPUT_END_HOLD_STATE,
    SEQ_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0,
    SEQ_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1,
    SEQ_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2,


    //
    // ACDC-SEQ
    //
    SEQ_ACDC_CPU_TIMER0_OUTPUT_READY_STATE,
    SEQ_ACDC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE,
    SEQ_ACDC_CPU_TIMER0_OUTPUT_ON_STATE,
    SEQ_ACDC_CPU_TIMER0_OUTPUT_HOLD_STATE,
    SEQ_ACDC_CPU_TIMER0_OUTPUT_END_HOLD_STATE,
    SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE,
    SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0,
    SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1,
    SEQ_ACDC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2,


    //
    // AC-SEQ
    //
    SEQ_AC_CPU_TIMER0_OUTPUT_READY_STATE,
    SEQ_AC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE,
    SEQ_AC_CPU_TIMER0_OUTPUT_ON_STATE,
    SEQ_AC_CPU_TIMER0_OUTPUT_HOLD_STATE,
    SEQ_AC_CPU_TIMER0_OUTPUT_END_HOLD_STATE,
    SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE,
    SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0,
    SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1,
    SEQ_AC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2,


    //
    // DC-SEQ
    //
    SEQ_DC_CPU_TIMER0_OUTPUT_READY_STATE,
    SEQ_DC_CPU_TIMER0_OUTPUT_STEP_INIT_STATE,
    SEQ_DC_CPU_TIMER0_OUTPUT_ON_STATE,
    SEQ_DC_CPU_TIMER0_OUTPUT_HOLD_STATE,
    SEQ_DC_CPU_TIMER0_OUTPUT_END_HOLD_STATE,
    SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE,
    SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE0,
    SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE1,
    SEQ_DC_CPU_TIMER0_OUTPUT_STOP_PHASE_STATE2,


    //
    // OFF
    //
    CPU_TIMER0_OUTPUT_OFF_STATE0,
    CPU_TIMER0_OUTPUT_OFF_STATE1,
    CPU_TIMER0_OUTPUT_OFF_STATE2,
    CPU_TIMER0_OUTPUT_OFF_STATE3,
    CPU_TIMER0_OUTPUT_OFF_STATE4

}CPU_TIMER0_STATE_REG;



//----------------------------------------------------
//
// EXTAD計算週期子狀態
//
typedef enum
{
    EXTAD_INIT_STATE,
    EXTAD_L_STATE,
    EXTAD_H_STATE

}EXTAD_STATE_REG;







//----------------------------------------------------
//
//
//
typedef struct CPU_TIMER_MODULE_REG{

    // *** Debug *** //
    Uint32 Debug01;
    Uint32 Debug02;
    Uint16 Debug03;
    Uint16 Debug04;


    CPU_TIMER0_SCHEDULE01_STATE_REG cpu_timer0_schedule01_state;
    SLEW_RATE_STATE_REG slew_rate_state;
    CPU_TIMER0_STATE_REG cpu_timer0_state;
    MS_SDRAM_SENDING_STATE_REG MS_SDRAM_Sending_State;
    MS_SDRAM_RECEVING_STATE_REG MS_SDRAM_Receving_State;


    //Uint32 counter1;

    SEQ_OUTPUT_ON_STATE_REG SEQ_Output_ON_state;

    ZERO_CROSS_STATE_REG Zero_Cross_State;


    float  wave_c2_decimal;
    float  wave_value;

    Uint16 Value_Set_User_Change;


    Uint32 EXTAD_TTL_time_stamp;

    float EXTAD_TTL_Period;
    float EXTAD_TTL_Period_shadow;

    EXTAD_STATE_REG EXTAD_state;
    Uint16 EXTAD_Change_Counter;

    Uint32 PLC_Trig_time_stamp;
    //觸發Phase lock control process

    //2.775us
    Uint32 Cpu_Timer0_Time_Stamp_01;
    Uint32 Cpu_Timer0_Time_Stamp_02;
    Uint32 Cpu_Timer0_Time_Stamp_03;
    Uint32 Cpu_Timer0_Time_Stamp_04;
    Uint32 MS_Sync_0_Singal_Start_Time;

    //1ms
    Uint32 Cpu_Timer2_Time_Stamp_01;
    Uint32 Cpu_Timer2_Time_Stamp_02;
    Uint32 Cpu_Timer2_Time_Stamp_03;
    Uint32 LED_Flick_Interval;

    Uint16 I_peak_hold_clear_counter;
    Uint16 V_peak_hold_clear_counter;

    Uint32 Zero_Cross_Width_Counter;
    Uint32 Zero_Cross_Width_Count;

    #ifdef DEVELOP_SV01
    //
    // 量測執行時間 (CPU timer)
    //
    Uint32 Compensate_Calc_Process_Cycle_Meas_Start;
    Uint32 Compensate_Calc_Process_Cycle_Meas_Result;
    Uint32 Cycle_Meas_Start_03;
    Uint32 Cycle_Meas_Result_03;
    Uint32 MS_SCI_RX_Cycle_Meas_Start;
    Uint32 MS_SCI_RX_Cycle_Meas_Result;
    Uint32 Power_Ready_Error_Scan_Cycle_Meas_Start;
    Uint32 Power_Ready_Error_Scan_Cycle_Meas_Result;
    Uint32 ATE_Debug_Meas_Start;
    Uint32 ATE_Debug_Meas_Result;
    Uint16 ATE_Debug_Trig;

    //
    // 量測執行時間 (PWM Counter)
    //
    Uint16 cpu_timer0_isr_Cycle_Meas_Start;
    Uint16 cpu_timer0_isr_Cycle_Meas_Result;
    Uint16 cpu_timer0_isr_Cycle_Meas_Max;
    Uint16 SPIB_Cycle_Meas_Start;
    Uint16 SPIB_Cycle_Meas_Result;

    float  Ratio_Check_CPU0_div_CPU2;
    float  Ratio_Check_CLA_div_CPU2;
    float  Ratio_Check_CPU0_div_CLA;
    #endif


    Uint16 Exec_Switch;
}CPU_TIMER_MODULE_REG;


extern volatile CPU_TIMER_MODULE_REG cpu_timer_module;





//----------------------------------------------------

void Init_CPU_Timer_Reg(void);
void Init_CPU_Timer_Module_Variables(void);

__interrupt void cpu_timer0_isr(void);
//__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);


Uint32 Calc_Elapsed_Time(Uint32 start_time, Uint32 current_time);



//
// End of file
//
