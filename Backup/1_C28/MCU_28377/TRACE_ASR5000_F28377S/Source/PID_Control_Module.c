//###########################################################################
//
// FILE:    PID_Control_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220307
//###########################################################################

// 引用來源
/*=====================================================================================
 File name:        PID_REG3.C  (IQ version)

 Originator:    Digital Control Systems Group
            Texas Instruments

 Description:  The PID controller with anti-windup

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
-------------------------------------------------------------------------------------*/


#include "Global_VariableDefs.h"

volatile PID_CONTROL_MODULE_VARIABLES_REG PID_Control_Module;







/////////////////////////////////////////////////////////////////////
//
// 模組初始化
//
void Init_PID_Control_Module_Variables(void)
{

    //
    // ACV_Control_loop => 控制VREF offset
    //

    PID_Control_Module.ACV_Control_loop.Err = 0;
    PID_Control_Module.ACV_Control_loop.Fdb = 0;
    PID_Control_Module.ACV_Control_loop.Out = 0;
    PID_Control_Module.ACV_Control_loop.OutPreSat = 0;
    PID_Control_Module.ACV_Control_loop.Ref = 0;
    PID_Control_Module.ACV_Control_loop.SatErr = 0;
    PID_Control_Module.ACV_Control_loop.Ud = 0;
    PID_Control_Module.ACV_Control_loop.Ui = 0;
    PID_Control_Module.ACV_Control_loop.Up = 0;
    PID_Control_Module.ACV_Control_loop.Up1 = 0;

    PID_Control_Module.ACV_Control_loop.Kp = 10.0;
    PID_Control_Module.ACV_Control_loop.Ki = 1.0;
    PID_Control_Module.ACV_Control_loop.Kd = 0;
    PID_Control_Module.ACV_Control_loop.Kc = 0;
    PID_Control_Module.ACV_Control_loop.OutMax = 7000.0;
    PID_Control_Module.ACV_Control_loop.OutMin = -7000.0;



    //
    // DCV_Control_loop1 => 控制VREF offset
    //

    PID_Control_Module.DCV_Control_loop1.Err = 0;
    PID_Control_Module.DCV_Control_loop1.Fdb = 0;
    PID_Control_Module.DCV_Control_loop1.Out = 0;
    PID_Control_Module.DCV_Control_loop1.OutPreSat = 0;
    PID_Control_Module.DCV_Control_loop1.Ref = 0;
    PID_Control_Module.DCV_Control_loop1.SatErr = 0;
    PID_Control_Module.DCV_Control_loop1.Ud = 0;
    PID_Control_Module.DCV_Control_loop1.Ui = 0;
    PID_Control_Module.DCV_Control_loop1.Up = 0;
    PID_Control_Module.DCV_Control_loop1.Up1 = 0;

    PID_Control_Module.DCV_Control_loop1.Kp = 10.0;
    PID_Control_Module.DCV_Control_loop1.Ki = 1.0;
    PID_Control_Module.DCV_Control_loop1.Kd = 0;
    PID_Control_Module.DCV_Control_loop1.Kc = 0;
    PID_Control_Module.DCV_Control_loop1.OutMax = 6000.0;
    PID_Control_Module.DCV_Control_loop1.OutMin = -6000.0;


    //
    // DCV_Control_loop2 => 控制WAVE offset
    //

    PID_Control_Module.DCV_Control_loop2.Err = 0;
    PID_Control_Module.DCV_Control_loop2.Fdb = 0;
    PID_Control_Module.DCV_Control_loop2.Out = 0;
    PID_Control_Module.DCV_Control_loop2.OutPreSat = 0;
    PID_Control_Module.DCV_Control_loop2.Ref = 0;
    PID_Control_Module.DCV_Control_loop2.SatErr = 0;
    PID_Control_Module.DCV_Control_loop2.Ud = 0;
    PID_Control_Module.DCV_Control_loop2.Ui = 0;
    PID_Control_Module.DCV_Control_loop2.Up = 0;
    PID_Control_Module.DCV_Control_loop2.Up1 = 0;

    PID_Control_Module.DCV_Control_loop2.Kp = 10.0;
    PID_Control_Module.DCV_Control_loop2.Ki = 1.0;
    PID_Control_Module.DCV_Control_loop2.Kd = 0;
    PID_Control_Module.DCV_Control_loop2.Kc = 0;
    PID_Control_Module.DCV_Control_loop2.OutMax = 3276.0;
    PID_Control_Module.DCV_Control_loop2.OutMin = -3276.0;


    //
    // CC_Control_loop => 控制VREF offset
    //

    PID_Control_Module.CC_Control_loop.Err = 0;
    PID_Control_Module.CC_Control_loop.Fdb = 0;
    PID_Control_Module.CC_Control_loop.Out = 0;
    PID_Control_Module.CC_Control_loop.OutPreSat = 0;
    PID_Control_Module.CC_Control_loop.Ref = 0;
    PID_Control_Module.CC_Control_loop.SatErr = 0;
    PID_Control_Module.CC_Control_loop.Ud = 0;
    PID_Control_Module.CC_Control_loop.Ui = 0;
    PID_Control_Module.CC_Control_loop.Up = 0;
    PID_Control_Module.CC_Control_loop.Up1 = 0;

    PID_Control_Module.CC_Control_loop.Kp = 200.0;
    PID_Control_Module.CC_Control_loop.Ki = 1.0;
    PID_Control_Module.CC_Control_loop.Kd = 0;
    PID_Control_Module.CC_Control_loop.Kc = 0;
    PID_Control_Module.CC_Control_loop.OutMax = 100.0;
    PID_Control_Module.CC_Control_loop.OutMin = -65535.0;

    //
    // CP_Control_loop => 控制VREF offset
    //

    PID_Control_Module.CP_Control_loop.Err = 0;
    PID_Control_Module.CP_Control_loop.Fdb = 0;
    PID_Control_Module.CP_Control_loop.Out = 0;
    PID_Control_Module.CP_Control_loop.OutPreSat = 0;
    PID_Control_Module.CP_Control_loop.Ref = 0;
    PID_Control_Module.CP_Control_loop.SatErr = 0;
    PID_Control_Module.CP_Control_loop.Ud = 0;
    PID_Control_Module.CP_Control_loop.Ui = 0;
    PID_Control_Module.CP_Control_loop.Up = 0;
    PID_Control_Module.CP_Control_loop.Up1 = 0;

    PID_Control_Module.CP_Control_loop.Kp = 2.0;
    PID_Control_Module.CP_Control_loop.Ki = 0.1;
    PID_Control_Module.CP_Control_loop.Kd = 0;
    PID_Control_Module.CP_Control_loop.Kc = 0;
    PID_Control_Module.CP_Control_loop.OutMax = 100.0;
    PID_Control_Module.CP_Control_loop.OutMin = -65535.0;


    //
    // MS_Balance_Compensate_loop => 控制VREF offset
    //

    PID_Control_Module.MS_Balance_Compensate_loop.Err = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.Fdb = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.Out = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.OutPreSat = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.Ref = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.SatErr = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.Ud = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.Ui = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.Up = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.Up1 = 0;

    PID_Control_Module.MS_Balance_Compensate_loop.Kp = 10.0;
    PID_Control_Module.MS_Balance_Compensate_loop.Ki = 1.0;
    PID_Control_Module.MS_Balance_Compensate_loop.Kd = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.Kc = 0;
    PID_Control_Module.MS_Balance_Compensate_loop.OutMax = 6000.0;
    PID_Control_Module.MS_Balance_Compensate_loop.OutMin = -6000.0;


    //
    // MS_Balance_Fast_Compensate_loop => 控制VREF offset
    //

    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Err = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Fdb = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Out = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutPreSat = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ref = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.SatErr = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ud = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ui = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Up = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Up1 = 0;

    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Kp = 10.0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Ki = 1.0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Kd = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.Kc = 0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutMax = 6000.0;
    PID_Control_Module.MS_Balance_Fast_Compensate_loop.OutMin = -6000.0;

    //
    // Reverse_Current_Unload_Compensate_loop => 控制WAVE offset
    //

    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Err = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Fdb = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Out = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutPreSat = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ref = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.SatErr = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ud = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ui = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Up1 = 0;

    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kp = 10.0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Ki = 1.0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kd = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.Kc = 0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMax = 3276.0;
    PID_Control_Module.Reverse_Current_Unload_Compensate_loop.OutMin = -3276.0;


}


////////////////////////////////////////////////////////////////////
//
//
//
void ACV_Control_Loop_Calc(void)
{

    //PID parameter
    //Kp
    //Ki = T/Ti
    //Kd = Td/T
    //T = sampling period (sec)

    // Compute the error
    //計算Err
    PID_Control_Module.ACV_Control_loop.Err = PID_Control_Module.ACV_Control_loop.Ref - PID_Control_Module.ACV_Control_loop.Fdb;

    // Compute the proportional output
    //計算比例Up
    PID_Control_Module.ACV_Control_loop.Up = PID_Control_Module.ACV_Control_loop.Kp * PID_Control_Module.ACV_Control_loop.Err;

    // Compute the integral output
    //計算積分Ui
    PID_Control_Module.ACV_Control_loop.Ui = PID_Control_Module.ACV_Control_loop.Ui + ( PID_Control_Module.ACV_Control_loop.Ki * PID_Control_Module.ACV_Control_loop.Up ) + ( PID_Control_Module.ACV_Control_loop.Kc * PID_Control_Module.ACV_Control_loop.SatErr );

    // Compute the derivative output
    //計算微分Ud
    PID_Control_Module.ACV_Control_loop.Ud = PID_Control_Module.ACV_Control_loop.Kd * ( PID_Control_Module.ACV_Control_loop.Up - PID_Control_Module.ACV_Control_loop.Up1 );

    // Compute the pre-saturated output
    //計算Up+Ui+Ud
    PID_Control_Module.ACV_Control_loop.OutPreSat = PID_Control_Module.ACV_Control_loop.Up + PID_Control_Module.ACV_Control_loop.Ui + PID_Control_Module.ACV_Control_loop.Ud;

    // Saturate the output
    if (PID_Control_Module.ACV_Control_loop.OutPreSat > PID_Control_Module.ACV_Control_loop.OutMax)
    {
        PID_Control_Module.ACV_Control_loop.Out = PID_Control_Module.ACV_Control_loop.OutMax;
    }
    else if (PID_Control_Module.ACV_Control_loop.OutPreSat < PID_Control_Module.ACV_Control_loop.OutMin)
    {
        PID_Control_Module.ACV_Control_loop.Out = PID_Control_Module.ACV_Control_loop.OutMin;
    }
    else
    {
        PID_Control_Module.ACV_Control_loop.Out = PID_Control_Module.ACV_Control_loop.OutPreSat;
    }

    // Compute the saturate difference
    PID_Control_Module.ACV_Control_loop.SatErr = PID_Control_Module.ACV_Control_loop.Out - PID_Control_Module.ACV_Control_loop.OutPreSat;

    // Update the previous proportional output
    //儲存Up(t-1)=Up(t)
    PID_Control_Module.ACV_Control_loop.Up1 = PID_Control_Module.ACV_Control_loop.Up;

}




////////////////////////////////////////////////////////////////////
//
//
//
void DCV_Control_Loop1_Calc(void)
{

    //PID parameter
    //Kp
    //Ki = T/Ti
    //Kd = Td/T
    //T = sampling period (sec)

    // Compute the error
    //計算Err
    PID_Control_Module.DCV_Control_loop1.Err = PID_Control_Module.DCV_Control_loop1.Ref - PID_Control_Module.DCV_Control_loop1.Fdb;

    // Compute the proportional output
    //計算比例Up
    PID_Control_Module.DCV_Control_loop1.Up = PID_Control_Module.DCV_Control_loop1.Kp * PID_Control_Module.DCV_Control_loop1.Err;

    // Compute the integral output
    //計算積分Ui
    PID_Control_Module.DCV_Control_loop1.Ui = PID_Control_Module.DCV_Control_loop1.Ui + ( PID_Control_Module.DCV_Control_loop1.Ki * PID_Control_Module.DCV_Control_loop1.Up ) + ( PID_Control_Module.DCV_Control_loop1.Kc * PID_Control_Module.DCV_Control_loop1.SatErr );

    // Compute the derivative output
    //計算微分Ud
    PID_Control_Module.DCV_Control_loop1.Ud = PID_Control_Module.DCV_Control_loop1.Kd * ( PID_Control_Module.DCV_Control_loop1.Up - PID_Control_Module.DCV_Control_loop1.Up1 );

    // Compute the pre-saturated output
    //計算Up+Ui+Ud
    PID_Control_Module.DCV_Control_loop1.OutPreSat = PID_Control_Module.DCV_Control_loop1.Up + PID_Control_Module.DCV_Control_loop1.Ui + PID_Control_Module.DCV_Control_loop1.Ud;

    // Saturate the output
    if (PID_Control_Module.DCV_Control_loop1.OutPreSat > PID_Control_Module.DCV_Control_loop1.OutMax)
    {
        PID_Control_Module.DCV_Control_loop1.Out = PID_Control_Module.DCV_Control_loop1.OutMax;
    }
    else if (PID_Control_Module.DCV_Control_loop1.OutPreSat < PID_Control_Module.DCV_Control_loop1.OutMin)
    {
        PID_Control_Module.DCV_Control_loop1.Out = PID_Control_Module.DCV_Control_loop1.OutMin;
    }
    else
    {
        PID_Control_Module.DCV_Control_loop1.Out = PID_Control_Module.DCV_Control_loop1.OutPreSat;
    }

    // Compute the saturate difference
    PID_Control_Module.DCV_Control_loop1.SatErr = PID_Control_Module.DCV_Control_loop1.Out - PID_Control_Module.DCV_Control_loop1.OutPreSat;

    // Update the previous proportional output
    //儲存Up(t-1)=Up(t)
    PID_Control_Module.DCV_Control_loop1.Up1 = PID_Control_Module.DCV_Control_loop1.Up;

}


////////////////////////////////////////////////////////////////////
//
//
//
void DCV_Control_Loop2_Calc(void)
{

    //PID parameter
    //Kp
    //Ki = T/Ti
    //Kd = Td/T
    //T = sampling period (sec)

    // Compute the error
    //計算Err
    PID_Control_Module.DCV_Control_loop2.Err = PID_Control_Module.DCV_Control_loop2.Ref - PID_Control_Module.DCV_Control_loop2.Fdb;

    // Compute the proportional output
    //計算比例Up
    PID_Control_Module.DCV_Control_loop2.Up = PID_Control_Module.DCV_Control_loop2.Kp * PID_Control_Module.DCV_Control_loop2.Err;

    // Compute the integral output
    //計算積分Ui
    PID_Control_Module.DCV_Control_loop2.Ui = PID_Control_Module.DCV_Control_loop2.Ui + ( PID_Control_Module.DCV_Control_loop2.Ki * PID_Control_Module.DCV_Control_loop2.Up ) + ( PID_Control_Module.DCV_Control_loop2.Kc * PID_Control_Module.DCV_Control_loop2.SatErr );

    // Compute the derivative output
    //計算微分Ud
    PID_Control_Module.DCV_Control_loop2.Ud = PID_Control_Module.DCV_Control_loop2.Kd * ( PID_Control_Module.DCV_Control_loop2.Up - PID_Control_Module.DCV_Control_loop2.Up1 );

    // Compute the pre-saturated output
    //計算Up+Ui+Ud
    PID_Control_Module.DCV_Control_loop2.OutPreSat = PID_Control_Module.DCV_Control_loop2.Up + PID_Control_Module.DCV_Control_loop2.Ui + PID_Control_Module.DCV_Control_loop2.Ud;

    // Saturate the output
    if (PID_Control_Module.DCV_Control_loop2.OutPreSat > PID_Control_Module.DCV_Control_loop2.OutMax)
    {
        PID_Control_Module.DCV_Control_loop2.Out = PID_Control_Module.DCV_Control_loop2.OutMax;
    }
    else if (PID_Control_Module.DCV_Control_loop2.OutPreSat < PID_Control_Module.DCV_Control_loop2.OutMin)
    {
        PID_Control_Module.DCV_Control_loop2.Out = PID_Control_Module.DCV_Control_loop2.OutMin;
    }
    else
    {
        PID_Control_Module.DCV_Control_loop2.Out = PID_Control_Module.DCV_Control_loop2.OutPreSat;
    }

    // Compute the saturate difference
    PID_Control_Module.DCV_Control_loop2.SatErr = PID_Control_Module.DCV_Control_loop2.Out - PID_Control_Module.DCV_Control_loop2.OutPreSat;

    // Update the previous proportional output
    //儲存Up(t-1)=Up(t)
    PID_Control_Module.DCV_Control_loop2.Up1 = PID_Control_Module.DCV_Control_loop2.Up;

}



////////////////////////////////////////////////////////////////////
//
//
//
void CC_Control_Loop_Calc(void)
{

    //PID parameter
    //Kp
    //Ki = T/Ti
    //Kd = Td/T
    //T = sampling period (sec)

    // Compute the error
    //計算Err
    PID_Control_Module.CC_Control_loop.Err = PID_Control_Module.CC_Control_loop.Ref - PID_Control_Module.CC_Control_loop.Fdb;

    // Compute the proportional output
    //計算比例Up
    PID_Control_Module.CC_Control_loop.Up = PID_Control_Module.CC_Control_loop.Kp * PID_Control_Module.CC_Control_loop.Err;

    // Compute the integral output
    //計算積分Ui
    PID_Control_Module.CC_Control_loop.Ui = PID_Control_Module.CC_Control_loop.Ui + ( PID_Control_Module.CC_Control_loop.Ki * PID_Control_Module.CC_Control_loop.Up ) + ( PID_Control_Module.CC_Control_loop.Kc * PID_Control_Module.CC_Control_loop.SatErr );

    // Compute the derivative output
    //計算微分Ud
    PID_Control_Module.CC_Control_loop.Ud = PID_Control_Module.CC_Control_loop.Kd * ( PID_Control_Module.CC_Control_loop.Up - PID_Control_Module.CC_Control_loop.Up1 );

    // Compute the pre-saturated output
    //計算Up+Ui+Ud
    PID_Control_Module.CC_Control_loop.OutPreSat = PID_Control_Module.CC_Control_loop.Up + PID_Control_Module.CC_Control_loop.Ui + PID_Control_Module.CC_Control_loop.Ud;

    // Saturate the output
    if (PID_Control_Module.CC_Control_loop.OutPreSat > PID_Control_Module.CC_Control_loop.OutMax)
    {
        PID_Control_Module.CC_Control_loop.Out = PID_Control_Module.CC_Control_loop.OutMax;
    }
    else if (PID_Control_Module.CC_Control_loop.OutPreSat < PID_Control_Module.CC_Control_loop.OutMin)
    {
        PID_Control_Module.CC_Control_loop.Out = PID_Control_Module.CC_Control_loop.OutMin;
    }
    else
    {
        PID_Control_Module.CC_Control_loop.Out = PID_Control_Module.CC_Control_loop.OutPreSat;
    }

    // Compute the saturate difference
    PID_Control_Module.CC_Control_loop.SatErr = PID_Control_Module.CC_Control_loop.Out - PID_Control_Module.CC_Control_loop.OutPreSat;

    // Update the previous proportional output
    //儲存Up(t-1)=Up(t)
    PID_Control_Module.CC_Control_loop.Up1 = PID_Control_Module.CC_Control_loop.Up;

}



////////////////////////////////////////////////////////////////////
//
//
//
void CP_Control_Loop_Calc(void)
{

    //PID parameter
    //Kp
    //Ki = T/Ti
    //Kd = Td/T
    //T = sampling period (sec)

    // Compute the error
    //計算Err
    PID_Control_Module.CP_Control_loop.Err = PID_Control_Module.CP_Control_loop.Ref - PID_Control_Module.CP_Control_loop.Fdb;

    // Compute the proportional output
    //計算比例Up
    PID_Control_Module.CP_Control_loop.Up = PID_Control_Module.CP_Control_loop.Kp * PID_Control_Module.CP_Control_loop.Err;

    // Compute the integral output
    //計算積分Ui
    PID_Control_Module.CP_Control_loop.Ui = PID_Control_Module.CP_Control_loop.Ui + ( PID_Control_Module.CP_Control_loop.Ki * PID_Control_Module.CP_Control_loop.Up ) + ( PID_Control_Module.CP_Control_loop.Kc * PID_Control_Module.CP_Control_loop.SatErr );

    // Compute the derivative output
    //計算微分Ud
    PID_Control_Module.CP_Control_loop.Ud = PID_Control_Module.CP_Control_loop.Kd * ( PID_Control_Module.CP_Control_loop.Up - PID_Control_Module.CP_Control_loop.Up1 );

    // Compute the pre-saturated output
    //計算Up+Ui+Ud
    PID_Control_Module.CP_Control_loop.OutPreSat = PID_Control_Module.CP_Control_loop.Up + PID_Control_Module.CP_Control_loop.Ui + PID_Control_Module.CP_Control_loop.Ud;

    // Saturate the output
    if (PID_Control_Module.CP_Control_loop.OutPreSat > PID_Control_Module.CP_Control_loop.OutMax)
    {
        PID_Control_Module.CP_Control_loop.Out = PID_Control_Module.CP_Control_loop.OutMax;
    }
    else if (PID_Control_Module.CP_Control_loop.OutPreSat < PID_Control_Module.CP_Control_loop.OutMin)
    {
        PID_Control_Module.CP_Control_loop.Out = PID_Control_Module.CP_Control_loop.OutMin;
    }
    else
    {
        PID_Control_Module.CP_Control_loop.Out = PID_Control_Module.CP_Control_loop.OutPreSat;
    }

    // Compute the saturate difference
    PID_Control_Module.CP_Control_loop.SatErr = PID_Control_Module.CP_Control_loop.Out - PID_Control_Module.CP_Control_loop.OutPreSat;

    // Update the previous proportional output
    //儲存Up(t-1)=Up(t)
    PID_Control_Module.CP_Control_loop.Up1 = PID_Control_Module.CP_Control_loop.Up;

}




















////////////////////////////////////////////////////////////////////
//
//
//
void MS_Balance_Compensate_loop_Calc(void)
{

    //PID parameter
    //Kp
    //Ki = T/Ti
    //Kd = Td/T
    //T = sampling period (sec)

    // Compute the error
    //計算Err
    PID_Control_Module.MS_Balance_Compensate_loop.Err = PID_Control_Module.MS_Balance_Compensate_loop.Ref - PID_Control_Module.MS_Balance_Compensate_loop.Fdb;

    // Compute the proportional output
    //計算比例Up
    PID_Control_Module.MS_Balance_Compensate_loop.Up = PID_Control_Module.MS_Balance_Compensate_loop.Kp * PID_Control_Module.MS_Balance_Compensate_loop.Err;

    // Compute the integral output
    //計算積分Ui
    PID_Control_Module.MS_Balance_Compensate_loop.Ui = PID_Control_Module.MS_Balance_Compensate_loop.Ui + ( PID_Control_Module.MS_Balance_Compensate_loop.Ki * PID_Control_Module.MS_Balance_Compensate_loop.Up ) + ( PID_Control_Module.MS_Balance_Compensate_loop.Kc * PID_Control_Module.MS_Balance_Compensate_loop.SatErr );

    // Compute the derivative output
    //計算微分Ud
    PID_Control_Module.MS_Balance_Compensate_loop.Ud = PID_Control_Module.MS_Balance_Compensate_loop.Kd * ( PID_Control_Module.MS_Balance_Compensate_loop.Up - PID_Control_Module.MS_Balance_Compensate_loop.Up1 );

    // Compute the pre-saturated output
    //計算Up+Ui+Ud
    PID_Control_Module.MS_Balance_Compensate_loop.OutPreSat = PID_Control_Module.MS_Balance_Compensate_loop.Up + PID_Control_Module.MS_Balance_Compensate_loop.Ui + PID_Control_Module.MS_Balance_Compensate_loop.Ud;

    // Saturate the output
    if (PID_Control_Module.MS_Balance_Compensate_loop.OutPreSat > PID_Control_Module.MS_Balance_Compensate_loop.OutMax)
    {
        PID_Control_Module.MS_Balance_Compensate_loop.Out = PID_Control_Module.MS_Balance_Compensate_loop.OutMax;
    }
    else if (PID_Control_Module.MS_Balance_Compensate_loop.OutPreSat < PID_Control_Module.MS_Balance_Compensate_loop.OutMin)
    {
        PID_Control_Module.MS_Balance_Compensate_loop.Out = PID_Control_Module.MS_Balance_Compensate_loop.OutMin;
    }
    else
    {
        PID_Control_Module.MS_Balance_Compensate_loop.Out = PID_Control_Module.MS_Balance_Compensate_loop.OutPreSat;
    }

    // Compute the saturate difference
    PID_Control_Module.MS_Balance_Compensate_loop.SatErr = PID_Control_Module.MS_Balance_Compensate_loop.Out - PID_Control_Module.MS_Balance_Compensate_loop.OutPreSat;

    // Update the previous proportional output
    //儲存Up(t-1)=Up(t)
    PID_Control_Module.MS_Balance_Compensate_loop.Up1 = PID_Control_Module.MS_Balance_Compensate_loop.Up;

}










//
// End of file
//
