/*
 *  File Name: linkEeprom.h
 *
 *  Created on: 2023/5/8
 *  Author: POWER2-54FD92
 */

typedef struct {
    uint16_t size;
    void *ptr;
} EE_REG;
typedef EE_REG * HAL_EEREG;

#define _BLK16_USERREGS	7
uint16_t blkUserRegs[_BLK16_USERREGS];
const EE_REG regUserRegs[4] = {
              {2, (void*)&u16C1},
              {2, (void*)&u16C2},
              {2, (void*)&u16C3},
              {1, (void*)&blkUserRegs[6]}};
#define _TABLE_USERREGS	4

#define _BLK16_CTRLREGS	7
uint16_t blkCtrlRegs[_BLK16_CTRLREGS];
const EE_REG regCtrlRegs[4] = {
              {2, (void*)&u16B1},
              {2, (void*)&u16B2},
              {2, (void*)&u16B3},
              {1, (void*)&blkCtrlRegs[6]}};
#define _TABLE_CTRLREGS	4

#define _BLK16_BOARDREGS	13
uint16_t blkBoardRegs[_BLK16_BOARDREGS];
const EE_REG regBoardRegs[4] = {
              {4, (void*)&f32A1},
              {4, (void*)&f32A1},
              {4, (void*)&f32A3},
              {1, (void*)&blkBoardRegs[12]}};
#define _TABLE_BOARDREGS	4

