//###########################################################################
//
// FILE:    Output_ACDC_INT_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211124
//###########################################################################


//----------------------------------------------------
//
// OUTPUT_ACDC_INT_STATE
//
typedef enum
{
    acdc_int_output_init,
    acdc_int_output_on_delay,
    acdc_int_output_on_v_rise,
    acdc_int_output_on,
    //acdc_int_output_off_delay,
    acdc_int_output_off0,
    acdc_int_output_off1,
    acdc_int_output_off2

}OUTPUT_ACDC_INT_STATE_REG;



//----------------------------------------------------
typedef struct OUTPUT_ACDC_INT_MODULE_VARIABLES_REG{
    Uint32 Output_on_start_time;
    Uint32 Output_off_start_time;

    OUTPUT_ACDC_INT_STATE_REG Output_ACDC_INT_State;

    float V_Rise_Count;
    float WAVE_per_Count;
    Uint32 V_Rise_Counter;
    Uint16 V_Rise_Bypass_Flag;

    Uint32 DCV_Zero_Stable_Counter;


}OUTPUT_ACDC_INT_MODULE_VARIABLES_REG;


extern volatile OUTPUT_ACDC_INT_MODULE_VARIABLES_REG Output_ACDC_INT_Module;


void ACDC_INT_Output_Process(void);
void Init_Output_ACDC_INT_Module_Variables(void);

//
// End of file
//
