/*
 *  File Name: linkEeprom.h
 *
 *  Created on: 8/27/2025
 *  Author: POWER-532A86
 */

typedef struct {
    uint16_t size;
    void *ptr;
} EE_REG;
typedef EE_REG * HAL_EEREG;

#define _BLK16_USERPARAMS	13
uint16_t blkUserParams[_BLK16_USERPARAMS];
const EE_REG regUserParams[7] = {
              {2, (void*)&sDrv.f32IrefLimitP},
              {2, (void*)&sDrv.f32IrefLimitN},
              {2, (void*)&sDrv.f32RemoteHwSourceOcp},
              {2, (void*)&sDrv.f32RemoteHwSinkOcp},
              {2, (void*)&sDrv.f32RemoteHwOvp},
              {2, (void*)&sDrv.f32RemoteHwVinOvp},
              {1, (void*)&blkUserParams[12]}};
#define _TABLE_USERPARAMS	7

#define _BLK16_ADVPARAMS	21
uint16_t blkAdvParams[_BLK16_ADVPARAMS];
const EE_REG regAdvParams[6] = {
              {4, (void*)&sDrv.sLoopV.f32Kp},
              {4, (void*)&sDrv.sLoopV.f32Ki},
              {4, (void*)&sDrv.sLoopI.f32Kp},
              {4, (void*)&sDrv.sLoopI.f32Ki},
              {4, (void*)&sDrv.f32OpenGain},
              {1, (void*)&blkAdvParams[20]}};
#define _TABLE_ADVPARAMS	6

