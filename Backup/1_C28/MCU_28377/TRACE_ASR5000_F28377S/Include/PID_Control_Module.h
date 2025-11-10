//###########################################################################
//
// FILE:    PID_Control_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20220307
//###########################################################################

// まノㄓ方
/* =================================================================================
File name:       PID_REG3.H  (IQ version)

Originator: Digital Control Systems Group
            Texas Instruments

Description:
Header file containing constants, data type definitions, and
function prototypes for the PIDREG3.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005 Version 3.20
------------------------------------------------------------------------------*/

#ifndef __PIDREG3_H__
#define __PIDREG3_H__



//----------------------------------------------------
//
//
//
typedef struct {  float  Ref;               // Input: Reference input
                  float  Fdb;               // Input: Feedback input
                  float  Err;               // Variable: Error
                  float  Kp;                // Parameter: Proportional gain
                  float  Up;                // Variable: Proportional output
                  float  Ui;                // Variable: Integral output
                  float  Ud;                // Variable: Derivative output
                  float  OutPreSat;         // Variable: Pre-saturated output
                  float  OutMax;            // Parameter: Maximum output
                  float  OutMin;            // Parameter: Minimum output
                  float  Out;               // Output: PID output
                  float  SatErr;            // Variable: Saturated difference
                  float  Ki;                // Parameter: Integral gain
                  float  Kc;                // Parameter: Integral correction gain
                  float  Kd;                // Parameter: Derivative gain
                  float  Up1;               // History: Previous proportional output
} PIDREG3;




//----------------------------------------------------
typedef struct PID_CONTROL_MODULE_VARIABLES_REG{

    PIDREG3 DCV_Control_loop1;
    PIDREG3 ACV_Control_loop;
    PIDREG3 DCV_Control_loop2;

    PIDREG3 CC_Control_loop;
    PIDREG3 CP_Control_loop;


    //
    // MSА瑈干纕北
    //
    PIDREG3 MS_Balance_Compensate_loop;
    PIDREG3 MS_Balance_Fast_Compensate_loop;

    //
    // 癴拈筿瑈更干纕北
    //
    PIDREG3 Reverse_Current_Unload_Compensate_loop;

}PID_CONTROL_MODULE_VARIABLES_REG;




extern volatile PID_CONTROL_MODULE_VARIABLES_REG PID_Control_Module;









/*------------------------------------------------------------------------------
Prototypes for the functions in PIDREG3.C
------------------------------------------------------------------------------*/

void Init_PID_Control_Module_Variables(void);

void ACV_Control_Loop_Calc(void);
void DCV_Control_Loop1_Calc(void);
void DCV_Control_Loop2_Calc(void);
void MS_Balance_Compensate_loop_Calc(void);

void CC_Control_Loop_Calc(void);
void CP_Control_Loop_Calc(void);


#endif // __PIDREG3_H__






//
// End of file
//
