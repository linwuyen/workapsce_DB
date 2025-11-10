/*
 * tstBlock.c
 *
 *  Created on: 2023¦~5¤ë24¤é
 *      Author: cody_chen
 */


#include <stdint.h>
#include "emu_eeprom.h"
#include "tst.h"

typedef enum {
    _INIT_TEST_BLOCK_RAM = 0,
    _RESET_BLOCK_RAM,
    _PROGRAM_BLOCK_TO_FLASH,
    _VARIFY_BLOCK_DATA,
    _FREE_TEST_EEPROM
} TEST_EVENT;

TEST_EVENT tstfsm = _FREE_TEST_EEPROM;
int beginAddress = 0;
int ptrAddress = 0;
int endAddress = 0;
int procBytes = 0;
int testBytes = EMU_KBYTES;

typedef enum {
    _EV_GET_ADDRESS = 0,
    _EV_UPLOAD_DATA,
    _EV_END_OF_UPLOAD,
    _EV_END_OF_INIT_TESTBLOCK
}REG_INIT_TSTBLOCK;

typedef REG_INIT_TSTBLOCK REG_RST_TSTBLOCK;

REG_INIT_TSTBLOCK regInitTstBlock = _EV_GET_ADDRESS;

void initBlockRam(void)
{
        switch(regInitTstBlock) {
            case _EV_GET_ADDRESS:
                endAddress = beginAddress + testBytes;
                ptrAddress = beginAddress;
                procBytes = 0;
                regInitTstBlock = _EV_UPLOAD_DATA;
            break;

          case _EV_UPLOAD_DATA:
                if(procBytes < testBytes) {
                     writeEmuEeprom(ptrAddress, ptrAddress%0xFF);
                     ptrAddress++;
                     procBytes++;
                     ptrAddress&=0x3FF;
                     if(EMU_SIZE_OF_SECTOR <= ptrAddress) ptrAddress -= EMU_SIZE_OF_SECTOR;
                }
                else {
                    ptrAddress = beginAddress;
                    regInitTstBlock = _EV_END_OF_UPLOAD;
                }

            break;

          case _EV_END_OF_UPLOAD:
                tstfsm = _FREE_TEST_EEPROM;
                regInitTstBlock = _EV_GET_ADDRESS;
            break;

            default:
            break;
        }
}




REG_RST_TSTBLOCK regRESTstBlock = _EV_GET_ADDRESS;

void resetBlockRam(void)
{
        switch(regRESTstBlock) {
            case _EV_GET_ADDRESS:
                endAddress = beginAddress + testBytes;
                ptrAddress = beginAddress;
                procBytes = 0;
                regRESTstBlock = _EV_UPLOAD_DATA;
            break;

          case _EV_UPLOAD_DATA:
              if(procBytes < testBytes) {
                     writeEmuEeprom(ptrAddress, 0xFF);
                     ptrAddress++;
                     procBytes++;
                     if(EMU_SIZE_OF_SECTOR <= ptrAddress) ptrAddress -= EMU_SIZE_OF_SECTOR;
                }
                else {
                    ptrAddress = beginAddress;
                    regRESTstBlock = _EV_END_OF_UPLOAD;
                }

            break;

          case _EV_END_OF_UPLOAD:
                tstfsm = _FREE_TEST_EEPROM;
                regRESTstBlock = _EV_GET_ADDRESS;
            break;

            default:

            break;

        }
}

void varifyBlockFlash(void)
{

}

uint32_t u32TestTimes = 100;
uint32_t u32TestTimeout = T_100MS;
uint32_t u32TestStep = 0;


void loopWriteBlock(void)
{
        static uint32_t u32Tick = 0;
        static uint32_t u32TimeStamp = 0;
        uint32_t u32TimeOut = 0;
        u32Tick = SWTIMER_CNT;

        if(u32TimeStamp > u32Tick) {
                u32TimeOut = u32Tick + SW_TIMER - u32TimeStamp;
        }
        else {
                u32TimeOut = u32Tick - u32TimeStamp;
        }

        if(u32TimeOut >= u32TestTimeout) {

                if(_FREE_TEST_EEPROM == tstfsm) {
                        //if(0 < u32TestTimes) {
                                switch(u32TestStep) {
                                  case 0:
                                         tstfsm = _INIT_TEST_BLOCK_RAM;
                                         initBlockRam();
                                      break;
                                  case 1:
                                        tstfsm = _PROGRAM_BLOCK_TO_FLASH;
                                        setProgramEmuEeprom();
                               //        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN );
                                        break;
                                  case 2:
                                       tstfsm = _VARIFY_BLOCK_DATA;
                                       setVerifyEmuEeprom();
                                       u32TestStep++;
                                      break;
                                  default:
                                      //u32TestTimes--;
                                      u32TestStep = 0;
                                      break;
                                }
                                u32TestStep++;
                        //}
                }
                u32TimeStamp = u32Tick;
        }
}

void tstEmuEeprom(void)
{

   //   loopWriteBlock();

    switch(tstfsm) {
    case _INIT_TEST_BLOCK_RAM:
        initBlockRam();
        break;

    case _RESET_BLOCK_RAM:
        resetBlockRam();
        break;

    case _PROGRAM_BLOCK_TO_FLASH:
        setProgramEmuEeprom();
                tstfsm = _FREE_TEST_EEPROM;
        break;

    case _VARIFY_BLOCK_DATA:
        setVerifyEmuEeprom();
            tstfsm = _FREE_TEST_EEPROM;
        break;

    case _FREE_TEST_EEPROM:
    default:

        break;
    }


}

