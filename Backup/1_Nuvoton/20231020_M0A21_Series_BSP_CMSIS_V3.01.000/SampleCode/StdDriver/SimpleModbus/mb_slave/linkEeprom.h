/*
 *  File Name: linkEeprom.h
 *
 *  Created on: 2023/10/13
 *  Author: POWER-532A86
 */

typedef struct {
    uint16_t size;
    void *ptr;
} EE_REG;
typedef EE_REG * HAL_EEREG;

#define _BLK16_USERREGS	5
uint16_t blkUserRegs[_BLK16_USERREGS];
const EE_REG regUserRegs[3] = {
              {2, (void*)&},
              {2, (void*)&},
              {1, (void*)&blkUserRegs[4]}};
#define _TABLE_USERREGS	3

