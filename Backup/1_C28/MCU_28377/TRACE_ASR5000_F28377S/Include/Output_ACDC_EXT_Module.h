//###########################################################################
//
// FILE:    Output_ACDC_EXT_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220314
//###########################################################################


//----------------------------------------------------
//
// OUTPUT_ACDC_EXT_STATE
//
typedef enum
{
    acdc_ext_output_init,
    acdc_ext_output_on_delay,
    acdc_ext_output_on,
    //acdc_ext_output_off_delay,
    //acdc_ext_output_off
    acdc_ext_output_off0,
    acdc_ext_output_off1,
    acdc_ext_output_off2
}OUTPUT_ACDC_EXT_STATE_REG;



//----------------------------------------------------
typedef struct OUTPUT_ACDC_EXT_MODULE_VARIABLES_REG{
    Uint32 Output_on_start_time;
    Uint32 Output_off_start_time;

    OUTPUT_ACDC_EXT_STATE_REG Output_ACDC_EXT_State;

}OUTPUT_ACDC_EXT_MODULE_VARIABLES_REG;


extern volatile OUTPUT_ACDC_EXT_MODULE_VARIABLES_REG Output_ACDC_EXT_Module;


void ACDC_EXT_Output_Process(void);
void Init_Output_ACDC_EXT_Module_Variables(void);



//
// End of file
//
