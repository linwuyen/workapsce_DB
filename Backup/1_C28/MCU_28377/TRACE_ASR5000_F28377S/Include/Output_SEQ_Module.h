//###########################################################################
//
// FILE:    Output_SEQ_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220824
//###########################################################################


#define SEQ_END         0
#define SEQ_NEXT        1
#define SEQ_JUMP        2
#define SEQ_BRANCH_1    3
#define SEQ_BRANCH_2    4



//----------------------------------------------------
//
// OUTPUT_SEQ_STATE
//
typedef enum
{
    seq_output_init,
    seq_output_on_delay,
    seq_output_on,
    //seq_output_off_delay,
    //seq_output_off
    seq_output_off0,
    seq_output_off1,
    seq_output_off2
}OUTPUT_SEQ_STATE_REG;



//----------------------------------------------------
typedef struct OUTPUT_SEQ_MODULE_VARIABLES_REG{
    Uint32 Output_on_start_time;
    Uint32 Output_off_start_time;

    OUTPUT_SEQ_STATE_REG Output_SEQ_State;

}OUTPUT_SEQ_MODULE_VARIABLES_REG;


extern volatile OUTPUT_SEQ_MODULE_VARIABLES_REG Output_SEQ_Module;


void SEQ_Output_Process(void);
void Init_Output_SEQ_Module_Variables(void);

//
// End of file
//
