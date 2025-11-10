/*
 * Check_EEPROM.h
 *
 *  Created on: 2023¦~12¤ë7¤é
 *      Author: roger_lin
 */

#ifndef I2C_EEPROM_CHECKEEPROM_H_
#define I2C_EEPROM_CHECKEEPROM_H_

//===================EEPROM=========================

#define CONTROLLER_SCL_CNT_TIMEOUT  500000
#define EEPROM_BASE   I2C_EEPROM_INST

typedef enum {
    _EV_IDIE_RAM           =      0x0000,
    _EV_PROGRAM_RAM        = (0x0001<<0),
    _EV_WAIT_FREE_EEPROM   = (0x0001<<1),
    _EV_EEPROM_STATUS      = (0x0001<<2),
    _EV_VERIFY_RAM         = (0x0001<<3),
    _EV_TIMEOUT            = (0x0001<<4),
    _EV_WRITEACTION        = (0x0001<<5),
    _MASK_EERPROM_ERROR    =      0x8000
} FSM_EEPROM;

typedef enum {
    _EV_IDIE_I2C            =        0x0000,
    _EV_RESET_I2C           =   (0x0001<<0),
    _EV_DISABLE_STRETCHING  =   (0x0001<<1),
    _MASK_I2C_BUS_ERROR     =        0x8000
} FSM_I2C_BUS;

typedef struct {
    DL_I2C_CONTROLLER_SCL   REG_SCL;
    FSM_EEPROM                fsmEE;
    FSM_I2C_BUS              fsmI2C;

    uint32_t             u32SCL_Cnt;
    uint32_t          u32SuccessCnt;
    uint32_t          u32TimeOutCnt;
} ST_I2C_EEPROM;


typedef ST_I2C_EEPROM * Test_CALBACK;
extern  ST_I2C_EEPROM     TestEEPROM;

//main.c
extern void tstWrite();
extern void checkI2CBus();
//i2C_eeprom.c
extern void writeEEPROM();
//===================I2C========================\\

#endif /* I2C_EEPROM_CHECKEEPROM_H_ */
