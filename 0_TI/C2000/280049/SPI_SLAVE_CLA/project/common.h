/*
 * common.h
 *
 *  Created on: Apr 3, 2024
 *      Author: roger_lin
 */

#ifndef COMMON_H_
#define COMMON_H_
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"
#include "timetask.h"

#include "ctypedef.h"
#include "HwConfig.h"

typedef struct {

    ST_RAMPCTRL sVref;

    float32_t   f32VO;
    float32_t   f32VIN;
    float32_t   f32IL;
    float32_t   f32IO;
    float32_t   f32IEL;

    uint32_t    u32IsrCnt;

    float32_t   f32RemoteVref;


    uint16_t    fgDrv;
    uint16_t    fgFault;

    uint16_t    u16EnablePwm;

}  ST_DRV;

typedef ST_DRV* HAL_DRV;

extern volatile ST_DRV sDrv;

#include "linkPeripheral.h"
#include "PowerSequence.h"

#endif /* COMMON_H_ */
