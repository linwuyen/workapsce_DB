//###########################################################################
//
// FILE:    Main_State_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211014
//###########################################################################



#define SELF_TEST_AUTO_RESET_COUNT 3



//----------------------------------------------------
//
// 主迴圈排程01狀態機
// 排程調整則相對應的程式counter要確認
//
typedef enum
{
    MAIN_LOOP_SCHEDULE01_01_STATE,
    MAIN_LOOP_SCHEDULE01_02_STATE,
    MAIN_LOOP_SCHEDULE01_03_STATE

}MAIN_LOOP_SCHEDULE01_STATE_REG;


//----------------------------------------------------
//
// 主迴圈排程02狀態機
// 排程調整則相對應的程式counter要確認
//
typedef enum
{
    MAIN_LOOP_SCHEDULE02_01_STATE,
    MAIN_LOOP_SCHEDULE02_02_STATE,
    MAIN_LOOP_SCHEDULE02_03_STATE

}MAIN_LOOP_SCHEDULE02_STATE_REG;


//----------------------------------------------------
//
// 處理Busy狀態
//
typedef enum
{
    MACHINE_BUSY_STATE_0,
    MACHINE_BUSY_STATE_1

}MACHINE_BUSY_STATE_REG;


//----------------------------------------------------
//
// 處理換檔
//
typedef enum
{
    CHANGE_OUTPUT_RANGE_STATE_0,
    CHANGE_OUTPUT_RANGE_STATE_1,
    CHANGE_OUTPUT_RANGE_STATE_2,
    CHANGE_OUTPUT_RANGE_STATE_3

}CHANGE_OUTPUT_RANGE_STATE_REG;


//----------------------------------------------------
//
//主要狀態機
//0x41A Machine Process State
//
typedef enum
{
    SELF_TEST_STATE,
    POWER_UP_STATE,

    READY_STATE0,
    READY_STATE,

    OUTPUT_STATE0,
    OUTPUT_STATE,

    ERROR_HANDLE_STATE0,
    ERROR_HANDLE_STATE1,
    ERROR_HANDLE_STATE2,

    ERROR_STATE
}MACHINE_STATE_REG;


//----------------------------------------------------
//
// SELF_TEST_STATE
//
typedef enum
{
    sd_ram_check_state,
    eeprom_load_state,
    self_test_ready,
    self_test_error_state
}SELF_TEST_STATE_REG;




//----------------------------------------------------
//
// POWER_UP_STATE
//
typedef enum
{
    start_state,
    fan_check_state,
    ac_on_check_state, //for 全寬機種
    ac_on_check2_state, //for 全寬機種
    pfc_check_state,
    pfc_check2_state,
    dcac_check_state,
    dcdc_check_state,
    power_up_error_handle_state,
    power_up_error_state,
    power_up_ready_state
}POWER_UP_STATE_REG;




//----------------------------------------------------
struct  POWER_UP_STATE_CONTROL_FLAG_BITS {   // bit
    Uint16 fan_error_scan:1;     // 0
    Uint16 pfc_error_scan:1;     // 1
    Uint16 pfc_error2_scan:1;     // 2
    Uint16 dcac_error_scan:1;     // 3
    Uint16 dcdc_error_scan:1;     // 4


};
union POWER_UP_STATE_CONTROL_FLAG_REG {
   Uint16                                   all;
   struct POWER_UP_STATE_CONTROL_FLAG_BITS   bit;
};



//----------------------------------------------------
typedef struct MAIN_STATE_MODULE_VARIABLES_REG{

    Uint32 Power_Up_start_time;
    Uint32 fan_Check_start_time;
    Uint32 pfc_Check_start_time;
    Uint32 pfc_Check2_start_time;
    Uint32 ac_on_Check_start_time;
    Uint32 ac_on_Check2_start_time;
    Uint32 dcac_Check_start_time;
    Uint32 dcdc_Check_start_time;
    Uint32 error_start_time;
    Uint32 output_start_time;
    Uint32 output_stop_time;
    MACHINE_STATE_REG Machine_state;
    POWER_UP_STATE_REG Power_up_state;
    SELF_TEST_STATE_REG Self_test_state;

    Uint16 Self_Test_AutoReset_Counter;

    union POWER_UP_STATE_CONTROL_FLAG_REG POWER_UP_STATE_CONTROL_FLAG;

    MAIN_LOOP_SCHEDULE01_STATE_REG main_loop_schedule01_state;
    MAIN_LOOP_SCHEDULE02_STATE_REG main_loop_schedule02_state;
    MACHINE_BUSY_STATE_REG Machine_Busy_State;
    CHANGE_OUTPUT_RANGE_STATE_REG Change_Output_Range_State;


}MAIN_STATE_MODULE_VARIABLES_REG;




extern volatile MAIN_STATE_MODULE_VARIABLES_REG Main_State_Module;


void Main_Process(void);
void Init_Main_State_Module_Variables(void);
void Init_VREF_WAVE_Process(void);



//
// End of file
//
