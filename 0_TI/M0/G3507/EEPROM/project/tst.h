/*
 * tst.h
 *
 *  Created on: 2023¦~5¤ë24¤é
 *      Author: cody_chen
 */

#ifndef TST_H_
#define TST_H_

typedef enum {
    _INIT_TEST_BLOCK_RAM = 0,
    _RESET_BLOCK_RAM,
    _PROGRAM_BLOCK_TO_FLASH,
    _VARIFY_BLOCK_DATA,
    _Load_emu_DATA,
    _LOOP_PROGRAMEG,
    _FAE_CODE,
    _Earse,
    _FREE_TEST_EEPROM
} TEST_EVENT;
extern TEST_EVENT tstfsm;

typedef struct{
    uint32_t u32Addr;
    uint32_t u32Data;
    uint32_t u32AddrData_0;
    uint32_t u32AddrData_1;
    uint32_t u32AddrData_2;
    uint32_t u32AddrData_3;
    uint32_t u32AddrData_4;
    uint32_t u32AddrData_5;
    uint16_t u16Status;
    uint16_t u16Action;
    uint16_t COUNT;
    uint32_t fifostatus;
    uint32_t UARTBUSY;
}Modbus_tstpackage;
extern Modbus_tstpackage pkg;




extern void tstEmuEeprom(void);

#endif /* TST_H_ */
