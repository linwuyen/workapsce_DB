/*
 * ControlPWM.h
 *
 *  Created on: 2024¦~2¤ë19¤é
 *      Author: roger_lin
 */

#ifndef CONTROLPWM_H_
#define CONTROLPWM_H_

#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"



typedef enum
{
    _NO_ACTION       = 0x0000,
    _Enable_PWM      = 0x0001,
    _Enable_ADC      = 0x0002,
    _Enable_DB       = 0x0003,
    _PWM_CONFIG      = 0x0004,
    _Enable_DAC      = 0x0005,
    _MARK_ERROR_FSM  = 0x8000
} PWM_EVENT;


typedef struct
{
   PWM_EVENT PWMfsm;

   uint16_t ADC0Results;
//TB
   uint16_t u16TBPeriodCounter;
   uint16_t u16TBCounter;
   EPWM_TimeBaseCountMode TBCountMode;
   uint16_t u16TBphaseCount;
   EPWM_SyncOutPulseMode TBPulseMode;

//CC
   uint16_t CC_AcompCount;
   uint16_t CC_BcompCount;
   uint16_t DB_REDCounter;

} ST_PWM;

typedef ST_PWM * HAL_DRV;

extern ST_PWM ST_PWM_Ctl;

extern void tstPWMFUN (void);
extern void init_Epwm(void);
extern void ADC_trigger (void);
#endif /* CONTROLPWM_H_ */
