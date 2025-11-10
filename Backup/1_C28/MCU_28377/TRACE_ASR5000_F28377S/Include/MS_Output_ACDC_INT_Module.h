//###########################################################################
//
// FILE:    MS_Output_ACDC_INT_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20221214
//###########################################################################


#define MASTER_WAIT_SLAVE_ON_OVER_TIME  500 //(ms)


//----------------------------------------------------
//
// MS_OUTPUT_ACDC_INT_STATE
//
typedef enum
{
    MS_acdc_int_output_init,
    MS_acdc_int_output_on_delay_0,
    MS_acdc_int_output_on_delay_Master0,
    //MS_acdc_int_output_on_delay_Master1,
    MS_acdc_int_output_on_delay_Slave0,
    MS_acdc_int_output_on_delay_Slave1,

    //MS_acdc_int_output_on_delay_1,
    MS_acdc_int_output_on,

    //MS_acdc_int_output_off_delay,
    //MS_acdc_int_output_off
    MS_acdc_int_output_off0,
    MS_acdc_int_output_off1,
    MS_acdc_int_output_off2

}MS_OUTPUT_ACDC_INT_STATE_REG;



//----------------------------------------------------
typedef struct MS_OUTPUT_ACDC_INT_MODULE_VARIABLES_REG{
    Uint32 Output_on_start_time;
    Uint32 Output_off_start_time;

    Uint32 Master_Wait_Slave_ON_Start_Time;

    MS_OUTPUT_ACDC_INT_STATE_REG MS_Output_ACDC_INT_State;

}MS_OUTPUT_ACDC_INT_MODULE_VARIABLES_REG;


extern volatile MS_OUTPUT_ACDC_INT_MODULE_VARIABLES_REG MS_Output_ACDC_INT_Module;


void MS_ACDC_INT_Output_Process(void);
void Init_MS_Output_ACDC_INT_Module_Variables(void);

//
// End of file
//
