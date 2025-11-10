//###########################################################################
//
// FILE:    Output_ACDC_SYNC_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220309
//###########################################################################


//----------------------------------------------------
//
// OUTPUT_ACDC_SYNC_STATE
//
typedef enum
{
    acdc_sync_output_init,
    acdc_sync_output_on_delay,
    acdc_sync_output_on,
    //acdc_sync_output_off_delay,
    //acdc_sync_output_off
    acdc_sync_output_off0,
    acdc_sync_output_off1,
    acdc_sync_output_off2
}OUTPUT_ACDC_SYNC_STATE_REG;




//----------------------------------------------------
typedef struct OUTPUT_ACDC_SYNC_MODULE_VARIABLES_REG{
    Uint32 Output_on_start_time;
    Uint32 Output_off_start_time;

    OUTPUT_ACDC_SYNC_STATE_REG Output_ACDC_SYNC_State;

}OUTPUT_ACDC_SYNC_MODULE_VARIABLES_REG;


extern volatile OUTPUT_ACDC_SYNC_MODULE_VARIABLES_REG Output_ACDC_SYNC_Module;


void ACDC_SYNC_Output_Process(void);
void Init_Output_ACDC_SYNC_Module_Variables(void);

//
// End of file
//
