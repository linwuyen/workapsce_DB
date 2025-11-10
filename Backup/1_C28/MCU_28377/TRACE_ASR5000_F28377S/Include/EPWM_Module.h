//###########################################################################
//
// FILE:    EPWM_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210902
//###########################################################################



/*************Define*************/

//Fan control, GPIO9, EPWM5B
#define EPWM5_TIMER_TBPRD  4000
#define EPWM5_TIMER_TBPHS     0
#define EPWM5_DUTY_OFF      0.5
#define FAN_STANDBY_DUTY    0.2
//#define FAN_SPEED_STEP      0.02
#define FAN_SPEED_STEP      0.005 //20230712 °Q½×«á­×§ï


//CLK0, GPIO13, EPWM7B
#define EPWM7_TIMER_TBPRD  1000
#define EPWM7_TIMER_TBPHS   500
#define EPWM7_DUTY_OFF      0.5

//CLK180, GPIO15, EPWM8B
#define EPWM8_TIMER_TBPRD  1000
#define EPWM8_TIMER_TBPHS     0
#define EPWM8_DUTY_OFF      0.5




/*************Variables*************/



/*************Function*************/

void Init_EPWM_Module(void);

void Init_EPWM5_Reg(void);
void Init_EPWM7_Reg(void);
void Init_EPWM8_Reg(void);

void Init_EPWM4_Reg(void);





//
// End of file
//
