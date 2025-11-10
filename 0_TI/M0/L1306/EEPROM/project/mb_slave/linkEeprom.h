/*
 *  File Name: linkEeprom.h
 *
 *  Created on: 2023/10/18
 *  Author: POWER-532A86
 */

typedef struct {
    uint16_t size;
    void *ptr;
} EE_REG;
typedef EE_REG * HAL_EEREG;

#define _BLK16_CTRLREGS	5
uint16_t blkCtrlRegs[_BLK16_CTRLREGS];
const EE_REG regCtrlRegs[3] = {
              {2, (void*)&},
              {2, (void*)&},
              {1, (void*)&blkCtrlRegs[4]}};
#define _TABLE_CTRLREGS	3

#define _BLK16_BOARDREGS	13
uint16_t blkBoardRegs[_BLK16_BOARDREGS];
const EE_REG regBoardRegs[4] = {
              {4, (void*)&},
              {4, (void*)&},
              {4, (void*)&},
              {1, (void*)&blkBoardRegs[12]}};
#define _TABLE_BOARDREGS	4

