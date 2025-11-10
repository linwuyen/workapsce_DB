/*
 * emu_eeprom.h
 *
 *  Created on: 2024¦~1¤ë5¤é
 *      Author: roger_lin
 */

#ifndef EMU_EEPROM_H_
#define EMU_EEPROM_H_

#include <stdio.h>
#include <stdint.h>

#include "common.h"

#define EMU_KBYTES                (10)
#define EMU_SIZE_OF_EEPROM        (1)*EMU_KBYTES

#define EMU_FLASH_PAGES           (2)
#define EMU_SIZE_OF_SECTOR        (4096)

#define EMU_START_OF_FLASH        ((uint32_t) 0x08C000)

#define EMU_START_OF_HEADER       (EMU_START_OF_FLASH)
#define EMU_START_OF_EEPROM       (EMU_START_OF_FLASH + EMU_SIZE_OF_SECTOR)
#define EMU_SIZE_OF_DATA          ((EMU_FLASH_PAGES-1)*EMU_SIZE_OF_SECTOR)
#define EMU_END_OF_FLASH          (EMU_START_OF_FLASH + EMU_FLASH_PAGES*EMU_SIZE_OF_SECTOR)
#define BYTES_OF_FLASH_WORD       (4)

#define SIZE_OF_U32DATA   2  //Align 16, u32 = sizeof(uint32_t) = 2

extern void execEmuEeprom(void);

extern int16_t writeEmuEepromBlock(uint16_t u16addr, uint8_t *pSrcByte, uint8_t length);
extern uint16_t* readEmuEeprom(uint16_t u16addr);
extern int16_t writeEmuEeprom(uint16_t u16addr, uint8_t u8Data);
extern int16_t setProgramEmuEeprom(void);
extern int16_t isEmuEepromFree(void);
extern int16_t setVerifyEmuEeprom(void);

extern uint16_t getEepromErrorStatus(void);
extern int8_t waittingEmuEeprom(void);

extern int16_t callbackRAMFromFlash(void);
//-------------------------------------------------


#endif /* EMU_EEPROM_H_ */
