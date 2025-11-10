/*
 * tstBlock.c
 *
 *  Created on: 2023¦~5¤ë24¤é
 *      Author: cody_chen
 */

#include <stdint.h>
#include "emu_eeprom.h"
#include "tst.h"


typedef enum
{
    _INIT_TEST_BLOCK_RAM  = 0,
    _RESET_BLOCK_RAM,
    _CALLBACK_From_Flash,
    _PROGRAM_BLOCK_TO_FLASH,
    _VARIFY_BLOCK_DATA,
    _RECORD_BLOCK_FLASH,
    _FREE_TEST_EEPROM
} TEST_EVENT;

typedef enum
{
    _EV_GET_ADDRESS = 0,
    _EV_UPLOAD_DATA,
    _EV_END_OF_UPLOAD,
    _EV_END_OF_INIT_TESTBLOCK
} REG_INIT_TSTBLOCK;

typedef REG_INIT_TSTBLOCK REG_RST_TSTBLOCK;

REG_RST_TSTBLOCK regRESTstBlock = _EV_GET_ADDRESS;


typedef struct
{
    REG_INIT_TSTBLOCK regInitTstBlock;
    TEST_EVENT tstfsm;

    uint16_t beginAddress;
    uint16_t ptrAddress;
    uint16_t endAddress;
    uint16_t procBytes;
    uint16_t testBytes;

    uint32_t Counter;

} ST_testblock;
ST_testblock Test_ST =
{
     .regInitTstBlock = _EV_GET_ADDRESS,
     .tstfsm = _FREE_TEST_EEPROM,
     .testBytes = EMU_KBYTES,
};

ST_testblock * p = &Test_ST;

#pragma SET_CODE_SECTION(".TI.ramfunc")

void initBlockRam(void)
{
    switch (p->regInitTstBlock)
    {
    case _EV_GET_ADDRESS:
        p->endAddress = p->beginAddress + p->testBytes;
        p->ptrAddress = p->beginAddress;
        p->procBytes = 0;
        p->regInitTstBlock = _EV_UPLOAD_DATA;
        break;

    case _EV_UPLOAD_DATA:
        if (p->procBytes < p->testBytes)
        {
            writeEmuEeprom(p->ptrAddress, p->ptrAddress % 0xFFFFFFFF);
            p->ptrAddress++;
            p->procBytes++;
            p->ptrAddress &= 0xFFFF;
            if (EMU_SIZE_OF_SECTOR <= p->ptrAddress)
                p->ptrAddress -= EMU_SIZE_OF_SECTOR;
        }
        else
        {
            p->ptrAddress = p->beginAddress;
            p->regInitTstBlock = _EV_END_OF_UPLOAD;
        }
        break;

    case _EV_END_OF_UPLOAD:
        p->tstfsm = _FREE_TEST_EEPROM;
        p->regInitTstBlock = _EV_GET_ADDRESS;
        break;

    default:
        break;
    }
}

void resetBlockRam(void)
{
    switch (regRESTstBlock)
    {
    case _EV_GET_ADDRESS:
        p->endAddress = p->beginAddress + p->testBytes;
        p->ptrAddress = p->beginAddress;
        p->procBytes = 0;
        regRESTstBlock = _EV_UPLOAD_DATA;
        break;

    case _EV_UPLOAD_DATA:
        if (p->procBytes < p->testBytes)
        {
            writeEmuEeprom(p->ptrAddress, 0xFFFF);
            p->ptrAddress++;
            p->procBytes++;
            if (EMU_SIZE_OF_SECTOR <= p->ptrAddress)
                p->ptrAddress -= EMU_SIZE_OF_SECTOR;
        }
        else
        {
            p->ptrAddress = p->beginAddress;
            regRESTstBlock = _EV_END_OF_UPLOAD;
        }
        break;

    case _EV_END_OF_UPLOAD:
        p->tstfsm = _FREE_TEST_EEPROM;
        regRESTstBlock = _EV_GET_ADDRESS;
        break;

    default:
        break;
    }
}
//uint32_t memaddr = 0x08C000;
//
//uint32_t *sourceAddress  = 0;
//uint16_t AddressData[4];
//uint32_t *p32GG = 0;
//uint16_t *p16GG = 0;
//
//void Testcallbackflash(void)
//{
//    int i = 0;
//    sourceAddress  = (uint32_t *)memaddr;
//    for (i = 0, p16GG = (uint16_t *)&sourceAddress[0]; i < 4; i++) {
////        p32GG = &sourceAddress[i];
////        p16GG = &AddressData[i];
//        AddressData[i] =  p16GG[i];
//    }
//}
void tstEmuEeprom(void)
{
    //   loopWriteBlock();
    switch (p->tstfsm)
{
    case _INIT_TEST_BLOCK_RAM:
        initBlockRam();
        p->Counter = (uint32_t)EMU_KBYTES * 100000;
        break;

    case _RESET_BLOCK_RAM:
        resetBlockRam();
        break;

    case _CALLBACK_From_Flash:
        callbackRAMFromFlash();
        p->tstfsm = _FREE_TEST_EEPROM;
        break;

    case _PROGRAM_BLOCK_TO_FLASH:
        setProgramEmuEeprom();
        p->tstfsm = _FREE_TEST_EEPROM;
        break;

    case _VARIFY_BLOCK_DATA:
        setVerifyEmuEeprom();
        p->tstfsm = _FREE_TEST_EEPROM;
        break;

    case _RECORD_BLOCK_FLASH:
        if (p->Counter > 0)
        {
            p->Counter--;
            if(p->Counter == 0 )
            {
                setProgramEmuEeprom();
                p->tstfsm = _FREE_TEST_EEPROM;
            }
        }
        break;

    case _FREE_TEST_EEPROM:

    default:
        break;
    }
}

#pragma SET_CODE_SECTION()

//==============================================================
