//###########################################################################
//
// FILE:    Output_AC_VCA_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220308
//###########################################################################


//----------------------------------------------------
//
// OUTPUT_AC_VCA_STATE
//
typedef enum
{
    ac_vca_output_init,
    ac_vca_output_on_delay,
    ac_vca_output_on,
    //ac_vca_output_off_delay,
    //ac_vca_output_off
    ac_vca_output_off0,
    ac_vca_output_off1,
    ac_vca_output_off2



}OUTPUT_AC_VCA_STATE_REG;



//----------------------------------------------------
typedef struct OUTPUT_AC_VCA_MODULE_VARIABLES_REG{
    Uint32 Output_on_start_time;
    Uint32 Output_off_start_time;

    OUTPUT_AC_VCA_STATE_REG Output_AC_VCA_State;

}OUTPUT_AC_VCA_MODULE_VARIABLES_REG;


extern volatile OUTPUT_AC_VCA_MODULE_VARIABLES_REG Output_AC_VCA_Module;


void AC_VCA_Output_Process(void);
void Init_Output_AC_VCA_Module_Variables(void);

//
// End of file
//
