/*
 *  File Name: linkEeprom.h
 *
 *  Created on: 2023/8/31
 *  Author: MASON
 */

typedef struct {
    uint16_t size;
    void *ptr;
} EE_REG;
typedef EE_REG * HAL_EEREG;

#define _BLK16_USERREGS	7
uint16_t blkUserRegs[_BLK16_USERREGS];
const EE_REG regUserRegs[4] = {
              {2, (void*)&sDrv.f32RemoteVref},
              {2, (void*)&sCLA.f32IrefLimitP},
              {2, (void*)&sCLA.f32IrefLimitN},
              {1, (void*)&blkUserRegs[6]}};
#define _TABLE_USERREGS	4

#define _BLK16_BOARDREGS	33
uint16_t blkBoardRegs[_BLK16_BOARDREGS];
const EE_REG regBoardRegs[9] = {
              {4, (void*)&sCLA.sLoopV.f32Kp},
              {4, (void*)&sCLA.sLoopV.f32Ki},
              {4, (void*)&sCLA.sLoopV.f32Max},
              {4, (void*)&sCLA.sLoopV.f32Min},
              {4, (void*)&sCLA.sLoopI.f32Kp},
              {4, (void*)&sCLA.sLoopI.f32Ki},
              {4, (void*)&sCLA.sLoopI.f32Max},
              {4, (void*)&sCLA.sLoopI.f32Min},
              {1, (void*)&blkBoardRegs[32]}};
#define _TABLE_BOARDREGS	9

