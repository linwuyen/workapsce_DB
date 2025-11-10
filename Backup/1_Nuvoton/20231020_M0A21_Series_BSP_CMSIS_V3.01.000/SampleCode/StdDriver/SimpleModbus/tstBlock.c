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
int testBytes = EMU_KBYTES;

typedef enum {
	_EV_GET_ADDRESS = 0,
	_EV_UPLOAD_DATA,
	_EV_END_OF_UPLOAD,
	_EV_END_OF_INIT_TESTBLOCK
}REG_INIT_TSTBLOCK;

REG_INIT_TSTBLOCK evTstBlock = _EV_GET_ADDRESS;
void initBlockRam(void)
{	
		switch(evTstBlock) {
			case _EV_GET_ADDRESS:
				endAddress = beginAddress + testBytes - 1;	
				ptrAddress = beginAddress;
				evTstBlock = _EV_UPLOAD_DATA;
			break;
			
		  case _EV_UPLOAD_DATA:
				if(ptrAddress != endAddress) {
					 writeEmuEeprom(ptrAddress, ptrAddress%0xFF);
					 ptrAddress++;
					 if(EMU_SIZE_OF_SECTOR <= ptrAddress) ptrAddress -= EMU_SIZE_OF_SECTOR;
				}
				else {
					ptrAddress = beginAddress;
					evTstBlock = _EV_END_OF_UPLOAD;
				}
				
			break;
				
		  case _EV_END_OF_UPLOAD:
				evTstBlock = _EV_GET_ADDRESS;
				tstfsm = _FREE_TEST_EEPROM;
				
			break;
			
			default:
				
			break;
			
		}
}

void resetBlockRam(void)
{
		switch(evTstBlock) {
			case _EV_GET_ADDRESS:
				endAddress = beginAddress + testBytes - 1;	
				ptrAddress = beginAddress;
				evTstBlock = _EV_UPLOAD_DATA;
			break;
			
		  case _EV_UPLOAD_DATA:
				if(ptrAddress != endAddress) {
					 writeEmuEeprom(ptrAddress, 0xFF);
					 ptrAddress++;
					 if(EMU_SIZE_OF_SECTOR <= ptrAddress) ptrAddress -= EMU_SIZE_OF_SECTOR;
				}
				else {
					ptrAddress = beginAddress;
					evTstBlock = _EV_END_OF_UPLOAD;
				}
				
			break;
				
		  case _EV_END_OF_UPLOAD:
				evTstBlock = _EV_GET_ADDRESS;
				tstfsm = _FREE_TEST_EEPROM;
				
			break;
			
			default:
				
			break;
			
		}
}

void varifyBlockFlash(void)
{

}

uint32_t u32TestTimes = 0;
uint32_t u32TestTimeout = T_500MS;
uint32_t u32TestStep = 0;

void loopWriteBlock(void)
{
		static uint32_t u32Tick = 0;
		static uint32_t u32TimeStamp = 0;
		uint32_t u32TimeOut = 0;
		u32Tick = GET_TIMERCNT;

		if(u32TimeStamp > u32Tick) {
				u32TimeOut = u32Tick + SW_TIMER - u32TimeStamp;
		}
		else {
				u32TimeOut = u32Tick - u32TimeStamp;
		}

		if(u32TimeOut >= u32TestTimeout) {
			
				if(_FREE_TEST_EEPROM == tstfsm) {
						if(0 < u32TestTimes) {
								switch(u32TestStep) {
									case 0:
										tstfsm = _INIT_TEST_BLOCK_RAM;
										initBlockRam();
										break;
								  case 1:
										tstfsm = _PROGRAM_BLOCK_TO_FLASH;
										setProgramEmuEeprom();
										break;
									default:
										u32TestTimes--;
									  u32TestStep = 0;
									  break;
								}
								u32TestStep++;																
						}		
				}
				u32TimeStamp = u32Tick;
		}
}

void tstEmuEeprom(void)
{
	
	  loopWriteBlock();

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

