//###########################################################################
//
// FILE:    Fan_Control_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211014
//###########################################################################


#define FAN_CHECK_ERROR_TIME 2000


struct  FAN_SPEED_CONTROL_MODULE_FLAG_BITS {        // bit    description
    Uint16 Fan_Error:1;        //0
    Uint16 Fan_Error_Reset:1;  //1
};
union FAN_SPEED_CONTROL_MODULE_FLAG_REG {
    Uint16                                      all;
    struct FAN_SPEED_CONTROL_MODULE_FLAG_BITS   bit;
};


typedef struct FAN_SPEED_CONTROL_MODULE_VARIABLES_REG{
    Uint32 Check_start_time;

    float  Speed_command;
    float  Speed_command_goal; //Speed command value //0x0901
    float  Speed_command_step;

    union FAN_SPEED_CONTROL_MODULE_FLAG_REG Flags;         //Flags
}FAN_SPEED_CONTROL_MODULE_VARIABLES_REG;





extern volatile FAN_SPEED_CONTROL_MODULE_VARIABLES_REG Fan_Speed_Control_Variables;


void Init_Fan_Speed_Control_Variables(void);
void Fan_Speed_Command_Update(void);

//
// End of file
//
