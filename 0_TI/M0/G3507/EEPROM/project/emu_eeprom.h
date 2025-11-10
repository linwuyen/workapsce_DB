/*
 * emu_eeprom.h
 *
 *  Created on: 2023¦~5¤ë15¤é
 *      Author: cody_chen
 */

#ifndef EMU_EEPROM_H_
#define EMU_EEPROM_H_

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include "common.h"

#define EMU_KBYTES                (256)
#define EMU_SIZE_OF_EEPROM        (1)*EMU_KBYTES

#define EMU_FLASH_PAGES           (6)
#define EMU_SIZE_OF_SECTOR        (1024)
#define EMU_START_OF_FLASH        ((uint32_t) 0x00005000)

#define EMU_START_OF_HEADER       (EMU_START_OF_FLASH)
#define EMU_START_OF_HEADER_CB    ((uint32_t) 0x00405400)

#define EMU_START_OF_EEPROM       (EMU_START_OF_FLASH + EMU_SIZE_OF_SECTOR)
#define EMU_SIZE_OF_DATA          ((EMU_FLASH_PAGES-1)*EMU_SIZE_OF_SECTOR)
#define EMU_END_OF_FLASH          (EMU_START_OF_FLASH + EMU_FLASH_PAGES*EMU_SIZE_OF_SECTOR)
#define BYTES_OF_FLASH_WORD       (8)


extern void execEmuEeprom(void);
extern int16_t writeEmuEepromBlock(uint16_t u16addr, uint8_t *pSrcByte, uint8_t length);
extern uint8_t* readEmuEeprom(uint16_t u16addr);
extern int16_t writeEmuEeprom(uint16_t u16addr, uint8_t u8Data);
extern int16_t setProgramEmuEeprom(void);
extern int16_t isEmuEepromFree(void);
extern int16_t setVerifyEmuEeprom(void);

extern uint16_t getEepromErrorStatus(void);
extern int8_t waittingEmuEeprom(void);

extern void  Load_EEPROM_Data_G (void);
#endif /* EMU_EEPROM_H_ */
