//###########################################################################
//
// FILE:    Fan_Control_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211014
//###########################################################################



#include "Global_VariableDefs.h"

volatile FAN_SPEED_CONTROL_MODULE_VARIABLES_REG Fan_Speed_Control_Variables;


/////////////////////////////////////////////////////////////////////
void Init_Fan_Speed_Control_Variables(void)
{
    Fan_Speed_Control_Variables.Flags.all = 0;
    Fan_Speed_Control_Variables.Speed_command_goal = FAN_STANDBY_DUTY;

    Fan_Speed_Control_Variables.Speed_command = FAN_STANDBY_DUTY;
    Fan_Speed_Control_Variables.Speed_command_step = FAN_SPEED_STEP;

}








/////////////////////////////////////////////////////////////////////
void Fan_Speed_Command_Update(void)
{
    //
    // 暫時用"I peak hold delay"的位址
    // 所以最小值設定為1
    //
    if ( Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FanSpeed_set_user <= 1 )
    {
        if ( Global_Variables.POWER_STATUS.bit.Output_ON == 1 )
        {
            //
            // 輸出ON
            //

            //
            // 根據溫度計算風扇轉速
            //
            if ( Global_Variables.INPUT_VARIABLES.Temp_Sensor <= 30.0 )
            {
                Fan_Speed_Control_Variables.Speed_command_goal = 0.3;
            }
            else if ( ( Global_Variables.INPUT_VARIABLES.Temp_Sensor > 30.0 ) && ( Global_Variables.INPUT_VARIABLES.Temp_Sensor < 50.0 ) )
            {
                Fan_Speed_Control_Variables.Speed_command_goal = Global_Variables.INPUT_VARIABLES.Temp_Sensor * 0.025 - 0.45;
            }
            if ( Global_Variables.INPUT_VARIABLES.Temp_Sensor >= 50.0 )
            {
                Fan_Speed_Control_Variables.Speed_command_goal = 0.8;
            }

        }
        else if ( Global_Variables.POWER_STATUS.bit.Output_ON == 0 )
        {
            //
            // 輸出OFF
            //

            Fan_Speed_Control_Variables.Speed_command_goal = FAN_STANDBY_DUTY;
        }
    }
    else
    {
        Fan_Speed_Control_Variables.Speed_command_goal = ( (float)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FanSpeed_set_user - 1.0 ) * 0.01;
    }



    //
    // ACFAIL強制風扇全轉
    //
    if ( Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Output_ON_ACFAIL_HW == 1 )
    {
        Fan_Speed_Control_Variables.Speed_command_goal = FAN_STANDBY_DUTY;
    }

    //
    // 處理Fan Slew Rate
    //
    if ( Fan_Speed_Control_Variables.Speed_command_goal > Fan_Speed_Control_Variables.Speed_command )
    {
        Fan_Speed_Control_Variables.Speed_command = Fan_Speed_Control_Variables.Speed_command + Fan_Speed_Control_Variables.Speed_command_step;

        if ( Fan_Speed_Control_Variables.Speed_command >= Fan_Speed_Control_Variables.Speed_command_goal )
        {
            Fan_Speed_Control_Variables.Speed_command = Fan_Speed_Control_Variables.Speed_command_goal;
        }
    }
    else if ( Fan_Speed_Control_Variables.Speed_command_goal < Fan_Speed_Control_Variables.Speed_command )
    {
        Fan_Speed_Control_Variables.Speed_command = Fan_Speed_Control_Variables.Speed_command - Fan_Speed_Control_Variables.Speed_command_step;

        if ( Fan_Speed_Control_Variables.Speed_command <= Fan_Speed_Control_Variables.Speed_command_goal )
        {
            Fan_Speed_Control_Variables.Speed_command = Fan_Speed_Control_Variables.Speed_command_goal;
        }
    }
    else
    {
        Fan_Speed_Control_Variables.Speed_command = Fan_Speed_Control_Variables.Speed_command_goal;
    }

    EPwm5Regs.CMPB.bit.CMPB = EPWM5_TIMER_TBPRD * Fan_Speed_Control_Variables.Speed_command;  // Set Compare B value

}






//
// End of file
//
