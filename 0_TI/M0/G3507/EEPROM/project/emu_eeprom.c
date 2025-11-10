/*
 * emu_eeprom.c
 *
 *  Created on: 2023¦~5¤ë15¤é
 *      Author: cody_chen
 */


#include "emu_eeprom.h"



typedef enum {
    _INIT_EMU_EEPROM        = (0x0000),
    _CALL_BACK_EMU_EEPROM   = (0x0001<<0),
    _FREE_EMU_EEPROM        = (0x0001<<1),
    _BUSY_EMU_EEPROM        = (0x0001<<2),
    _PROGRAM_EMU_EEPROM     = (0x0001<<3),
    _VERIFY_EMU_EEPORM      = (0x0001<<4),
    _MASK_EMU_EEPROM_ERROR  = (0x8000)
} FS_EMUEE;
typedef uint16_t REG_EMUEE;  // Refer to FS_EMUEE

typedef union {
    uint8_t all[BYTES_OF_FLASH_WORD];
    uint64_t u64;
    struct {
        uint32_t Lo;
        uint32_t Hi;
    } u32;
} FLASH_WORD;

const FLASH_WORD DEFAULT_FLASH_WORD = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

typedef union {
    FLASH_WORD block;
    struct {
        uint16_t start;       // the data of start address
        uint16_t next;        // the data of next address
        uint16_t reserved;    // <the reserved field>
        uint16_t chksum;    // the checksum of header
    } info;
} REG_HEADER_CONTENT; // Data Range
typedef REG_HEADER_CONTENT * HAL_HEADER_CONTENT;

typedef union {
    FLASH_WORD block;
    struct {
        uint32_t start;       // the data of start address
        uint32_t next;        // the data of next address
    } info;
} REG_HEADER_CONTENT_CB; // Data Range
typedef REG_HEADER_CONTENT_CB * HAL_HEADER_CONTENT_CB;

typedef union {
    uint32_t all;
    struct {
        uint16_t idno;
        uint8_t  databyte;
        uint8_t  chksum;
    } info;
} REG_DATA32;
typedef REG_DATA32 * HAL_DATA32;

typedef union {
    FLASH_WORD block;
    struct {
        REG_DATA32 lo;
        REG_DATA32 hi;
    } info;
} REG_DATA64_CONTENT;
typedef REG_DATA64_CONTENT * HAL_DATA64_CONTENT;


typedef enum {
    _READ_HEADER_INDEX           = 0x0000,
    _GET_HEADER_INDEX            = (0x0001<<0),
    _PARSE_HEADER_CONTENT        = (0x0001<<1),
    _READ_FROM_FLASH_TO_RAM      = (0x0001<<2),
    _VERIFY_FLASH_BY_RAM         = (0x0001<<3),
    _MARK_ERROR_FOR_CALLBACK_FSM = 0x8000
} FSM_CALBACK;

typedef struct {
    FSM_CALBACK fsm;
    uint16_t    i;
    uint8_t     *ptr;
    uint32_t    u32Debug;
} ST_CALBACK;



typedef enum {
    _PREPARE_HEADER_INFORMATION     = 0x0000,
    _SCAN_VALID_DATA_IN_RAM         = (0x0001<<0),
    _PROGRAM_VALID_DATA64_TO_FLASH  = (0x0001<<1),
    _ERASE_DATA_SECTION             = (0x0001<<2),
    _RECORD_VALID_DATA_TO_HEADER    = (0x0001<<3),
    _REMARK_HEADER_INDEX            = (0x0001<<4),
    _ERASE_HEADER_SECTION           = (0x0001<<5),
    _END_OF_PROGRAM_EMU_EEPROM      = (0x0001<<6),
    _MASK_ERROR_FOR_PROG_EEPROM     = 0x8000
} FSM_PROGRAM;

typedef struct {
    FSM_PROGRAM fsm;
    uint16_t    i;
    uint8_t     *ptr;
    uint32_t    u32Debug;
} ST_PROGRAM;

typedef struct{
    REG_EMUEE           fgEmuEE;
    FLASH_WORD          HeaderSectionMark;
    uint16_t            u16HeaderIndex;
    uint32_t            u32HeaderAddress;
    uint8_t             u8HeaderChecksum;
    uint32_t            u32DataAddress;
    uint8_t             u8DataChecksum;
    HAL_DATA32          pDataContent;
    REG_HEADER_CONTENT        regHeader;
    REG_HEADER_CONTENT_CB  regHeadercb;

    REG_DATA64_CONTENT  regData64;
    uint16_t            u16RamCount;
    uint8_t             u8RAM[EMU_SIZE_OF_EEPROM];
    uint32_t            u32BoundaryAddress;
    uint32_t            u32BoundaryAddresscb;
    uint32_t            u32EraseAddress;
    uint32_t            u32OddEven;
    uint32_t            u32OddEvencb;
    uint32_t         u32DataAddresscb;

    ST_CALBACK          sCB;
    ST_PROGRAM          sPG;

} ST_EMUEE;

typedef ST_EMUEE * HAL_EMUEE;


// 1. After power on, need to initial all of RAM and set by 0xFF for formating this area.
// 2. Call back the first 64bits of first section of flash, to parsing where is the entry pointer.
// 3. Jump to the entry pointer to get the package of header, before parsing the package need to checksum correctly first.
// 4. After get the correct package of header, then jump to the start address of flash which is mention in the header.
// 5. Start to read from flash to ram, during reading you need to check each the package of data whether is corrected.
// If the content of Ram is not equal to 0xFF, then need to save the index, data and checksum to the flash.

#define SET_EE(bit)  FG_SET(bit, stEmuEE.fgEmuEE);
#define RST_EE(bit)  FG_RST(bit, stEmuEE.fgEmuEE);
#define SWTO_EE(bit) FG_SWTO(bit, stEmuEE.fgEmuEE);
#define GET_EE(bit)  FG_GET(bit, stEmuEE.fgEmuEE)
#define AND_EE(bit)  FG_AND(bit, stEmuEE.fgEmuEE)



ST_EMUEE stEmuEE;

uint16_t getEepromErrorStatus(void)
{
    return (stEmuEE.sPG.fsm & _MASK_ERROR_FOR_PROG_EEPROM);
}

uint8_t getChecksum(uint8_t * pByte, uint8_t length)
{
    uint8_t i;
    uint8_t chksum;
    for(i=0, chksum=0; i<length; i++) {
        chksum += (pByte[i]);
    }

    chksum = 0xFF- chksum;
    chksum = chksum + 0x01;

    return chksum;
}

void clearBit(uint8_t array[], int bitIndex) {
    int byteIndex = bitIndex / 8;         // Find the index of the byte containing the bit
    int bitOffset = bitIndex % 8;         // Find the offset of the bit within the byte

    uint8_t mask = ~(1u << bitOffset);    // Create a mask with the bit at bitOffset set to 0
    array[byteIndex] &= mask;             // Perform bitwise AND with the mask to clear the bit
}

void markBit(uint8_t array[], int bitIndex) {
    int byteIndex = bitIndex / 8;         // Find the index of the byte containing the bit
    int bitOffset = bitIndex % 8;         // Find the offset of the bit within the byte

    uint8_t mask = 1u << bitOffset;       // Create a mask with the bit at bitOffset set to 1
    array[byteIndex] |= mask;             // Perform bitwise OR with the mask to mark the bit
}

void getMask(uint8_t * pMask, int bitIndex) {
    for(int i = 0; i < bitIndex; i++) {
        pMask[0] |= (0x01 <<i);
    }
}

uint16_t getAddressSection(uint16_t address) {
    uint16_t section = address / EMU_SIZE_OF_SECTOR;  // Calculate the section index
    return section;
}



int16_t writeEmuEepromBlock(uint16_t u16addr, uint8_t *pSrcByte, uint8_t length)
{
    uint16_t u16TempAddr = 0;
    uint16_t i = 0;
    if(_FREE_EMU_EEPROM == stEmuEE.fgEmuEE) {
        for(i=0; i<length; i++) {
            u16TempAddr = u16addr+i;
            if(EMU_KBYTES <= u16TempAddr) u16TempAddr -= EMU_KBYTES;
            stEmuEE.u8RAM[u16TempAddr] = pSrcByte[i];
        }

        SWTO_EE(_PROGRAM_EMU_EEPROM);
        return i;

    }
    else {
        return -1;
    }
}

int16_t writeEmuEeprom(uint16_t u16addr, uint8_t u8Data)
{
    uint16_t u16TempAddr = 0;
    if(EMU_KBYTES <= u16addr) u16TempAddr = u16addr - EMU_KBYTES;
    else                      u16TempAddr = u16addr;
    stEmuEE.u8RAM[u16TempAddr] = u8Data;

    return 1;
}

int16_t setProgramEmuEeprom(void)
{
    if(_FREE_EMU_EEPROM == stEmuEE.fgEmuEE) {
        SWTO_EE(_PROGRAM_EMU_EEPROM);
        return 1;
    }
    else {
        return 0;
    }
}

int16_t setVerifyEmuEeprom(void)
{
//    if(_FREE_EMU_EEPROM == stEmuEE.fgEmuEE) {
//        SWTO_EE(_VERIFY_EMU_EEPORM);
//        return 1;
//    }
//    else {
//        return 0;
//    }
}

int8_t waittingEmuEeprom(void)
{

        return (_FREE_EMU_EEPROM == stEmuEE.fgEmuEE);
}

uint16_t u16TempAddr = 0;
uint8_t * readEmuEeprom(uint16_t u16addr)
{

    if(_FREE_EMU_EEPROM == stEmuEE.fgEmuEE) {
        u16TempAddr = u16addr;
        if(EMU_KBYTES <= u16TempAddr) u16TempAddr -= EMU_KBYTES;
        return &stEmuEE.u8RAM[u16TempAddr];
    }
    else {
        return 0;
    }
}

void initEmuEeprom(HAL_EMUEE p)
{
    if(EMU_SIZE_OF_EEPROM > p->u16RamCount) {
        p->u8RAM[p->u16RamCount++] = 0xFF;
    }
    else {
        p->HeaderSectionMark = DEFAULT_FLASH_WORD;
        p->pDataContent = 0;
        p->sCB.fsm = _READ_HEADER_INDEX;
     //   p->sCB.ptr = (uint8_t *)EMU_START_OF_HEADER;
        p->sCB.ptr = (uint8_t *)0x00405000;

        p->sPG.fsm = _PREPARE_HEADER_INFORMATION;
        p->sPG.ptr = (uint8_t *)EMU_START_OF_EEPROM;
        SWTO_EE(_CALL_BACK_EMU_EEPROM);
    }
}

int16_t isEmuEepromFree(void)
{
    return GET_EE(_FREE_EMU_EEPROM);
}

int builtin_ctz64(uint64_t x) {
    if (x == 0) {
        // Special case: x is zero, which has no trailing zeros.
        return 64;
    }

    int count = 0;
    while ((x & 1) == 0) {
        // Keep shifting x to the right until the least significant bit becomes 1.
        x >>= 1;
        count++;
    }

    return count;
}

void callbackFromFlash(HAL_EMUEE p)
{
    switch (p->sCB.fsm) {
    case _READ_HEADER_INDEX:

        // Copy the header mark in flash to the header mark in ram.
        for(int i = 0; i<BYTES_OF_FLASH_WORD; i++) {
            p->HeaderSectionMark.all[i] = p->sCB.ptr[i];
        }

        // Reset Header Index
        p->u16HeaderIndex = 0;
        p->sCB.fsm =_GET_HEADER_INDEX;
        break;

    case _GET_HEADER_INDEX:
        // Get the position of the least significant set bit
        p->u16HeaderIndex = builtin_ctz64(p->HeaderSectionMark.u64);

        if(0 == p->u16HeaderIndex) {
            // emuEEPROM is empty.
            p->regHeader.info.start = EMU_START_OF_EEPROM;
            p->regHeader.info.next = EMU_START_OF_EEPROM;
            p->regHeader.info.reserved = 0xFFFF;
            p->regHeader.info.chksum = getChecksum((uint8_t *)&p->regHeader.block.all[0], 6);

            p->regHeadercb.info.start = EMU_START_OF_HEADER_CB;
            p->regHeadercb.info.next = EMU_START_OF_HEADER_CB;

            p->sCB.fsm =_READ_HEADER_INDEX;
            SWTO_EE(_FREE_EMU_EEPROM);
        }
        else {
            p->u32HeaderAddress = (uint32_t)(p->sCB.ptr + p->u16HeaderIndex * BYTES_OF_FLASH_WORD);
            p->sCB.fsm =_PARSE_HEADER_CONTENT;
        }
        break;

    case _PARSE_HEADER_CONTENT:

        p->u8HeaderChecksum = getChecksum((uint8_t *)p->u32HeaderAddress, BYTES_OF_FLASH_WORD);

        if(0 == p->u8HeaderChecksum) {
            p->regHeader = *((HAL_HEADER_CONTENT)p->u32HeaderAddress);
            p->u32DataAddress = (uint32_t)(p->regHeader.info.start);
            p->sCB.fsm =_READ_FROM_FLASH_TO_RAM;
        }
        else {
            p->sCB.fsm |=_MARK_ERROR_FOR_CALLBACK_FSM;
        }
        break;

    case _READ_FROM_FLASH_TO_RAM:
        if(p->u32DataAddress != p->regHeader.info.next) {
            // Get a 32bits from the data section.
            p->u8DataChecksum = getChecksum((uint8_t *)p->u32DataAddress, 4);
            if(0 == p->u8DataChecksum) {
                p->pDataContent = (HAL_DATA32)p->u32DataAddress;
                p->u8RAM[p->pDataContent->info.idno] = p->pDataContent->info.databyte;
                // Go to the next data
                p->u32DataAddress += 4;
                if(EMU_END_OF_FLASH <= p->u32DataAddress) {
                    // If over the range of emulation, then need to roll over the data address.
                    p->u32DataAddress -= EMU_SIZE_OF_DATA;
                }

            }
            else {
                p->sCB.fsm |=_MARK_ERROR_FOR_CALLBACK_FSM;
            }
        }
        else {
            // Reset fsm for entry next time.
            p->sCB.fsm =_READ_HEADER_INDEX;
            SWTO_EE(_FREE_EMU_EEPROM);
        }
        break;

    default:
        break;
    }
}

void varifyFlashByRAM(HAL_EMUEE p)
{
    switch (p->sCB.fsm) {
    case _READ_HEADER_INDEX:

        // Copy the header mark in flash to the header mark in ram.
        for(int i = 0; i<BYTES_OF_FLASH_WORD; i++) {
            p->HeaderSectionMark.all[i] = p->sCB.ptr[i];
        }

        // Reset Header Index
        p->u16HeaderIndex = 0;
        p->sCB.fsm =_GET_HEADER_INDEX;
        break;

    case _GET_HEADER_INDEX:
//        for(int i = 0; i<BYTES_OF_FLASH_WORD; i++) {
//            // Get the position of the least significant set bit
//            p->u16HeaderIndex += __builtin_ctz(p->HeaderSectionMark.all[i]);
//        }
        p->u16HeaderIndex = builtin_ctz64(p->HeaderSectionMark.u64);

        if(0 == p->u16HeaderIndex) {
            // emuEEPROM is empty.
//            p->regHeader.info.start = EMU_START_OF_EEPROM;
//            p->regHeader.info.next = EMU_START_OF_EEPROM;
//            p->regHeader.info.reserved = 0xFFFF;
//            p->regHeader.info.chksum = getChecksum((uint8_t *)&p->regHeader.block.all[0], 6);

            p->regHeadercb.info.start = EMU_START_OF_HEADER_CB;
            p->regHeadercb.info.next = EMU_START_OF_HEADER_CB;

            p->sCB.fsm =_READ_HEADER_INDEX;
            SWTO_EE(_FREE_EMU_EEPROM);
        }
        else {
            p->u32HeaderAddress = (uint32_t)(p->sCB.ptr + p->u16HeaderIndex * BYTES_OF_FLASH_WORD);
           p->sCB.fsm =_PARSE_HEADER_CONTENT;

           p->sCB.fsm =_READ_HEADER_INDEX;
          // __BKPT();
           SWTO_EE(_FREE_EMU_EEPROM);
        }
        break;

    case _PARSE_HEADER_CONTENT:

        p->u8HeaderChecksum = getChecksum((uint8_t *)p->u32HeaderAddress, BYTES_OF_FLASH_WORD);

        if(0 == p->u8HeaderChecksum) {
            p->regHeader = *((HAL_HEADER_CONTENT)p->u32HeaderAddress);
         //   p->u32DataAddress = (uint32_t)(p->regHeader.info.start);
            p->u32DataAddresscb = (uint32_t)(p->regHeadercb.info.start);
            p->regHeadercb =*((HAL_HEADER_CONTENT_CB)p->u32HeaderAddress);

            p->sCB.fsm =_VERIFY_FLASH_BY_RAM;
        }
        else {
            p->sCB.fsm |=_MARK_ERROR_FOR_CALLBACK_FSM;
        }
        break;

    case _VERIFY_FLASH_BY_RAM:
        if(p->u32DataAddresscb != p->regHeadercb.info.next) {
            // Get a 32bits from the data section.
            p->u8DataChecksum = getChecksum((uint8_t *)p->u32DataAddress, 4);
            if(0 == p->u8DataChecksum) {
                p->pDataContent = (HAL_DATA32)p->u32DataAddress;

                if(p->u8RAM[p->pDataContent->info.idno] != p->pDataContent->info.databyte) {
                    p->sCB.fsm |=_MARK_ERROR_FOR_CALLBACK_FSM;
                }
                else {
                    // Go to the next data
                    p->u32DataAddress += 4;
                    if(EMU_END_OF_FLASH <= p->u32DataAddress) {
                        // If over the range of emulation, then need to roll over the data address.
                        p->u32DataAddress -= EMU_SIZE_OF_DATA;
                    }
                }
            }
            else {
                p->sCB.fsm |=_MARK_ERROR_FOR_CALLBACK_FSM;
            }
        }
        else {
            // Reset fsm for entry next time.
            p->sCB.fsm =_READ_HEADER_INDEX;
            SWTO_EE(_FREE_EMU_EEPROM);
        }
        break;

    default:
        break;
    }
}

FLASH_WORD * p64 = 0;
uint32_t *p32 = 0;

void progEmuEeprom(HAL_EMUEE p)
{
    DL_FLASHCTL_COMMAND_STATUS FlashAPIState;

    switch(p->sPG.fsm) {
    case _PREPARE_HEADER_INFORMATION:
        p->u16RamCount = 0;
        p->regHeader.info.start = p->regHeader.info.next;
        p->regHeadercb.info.start = p->regHeadercb.info.next;

        p->regHeader.info.reserved = 0xFFFF;
        p->regHeader.info.chksum = 0xFFFF;
        p->sPG.fsm = _SCAN_VALID_DATA_IN_RAM;

        break;

    case _SCAN_VALID_DATA_IN_RAM:
        if(EMU_SIZE_OF_EEPROM > p->u16RamCount) {
            // Scan from 0 to EMU_SIZE_OF_EEPROM
            if(0xFF != p->u8RAM[p->u16RamCount]) {

                p->u32OddEven = p->regHeader.info.next % BYTES_OF_FLASH_WORD;
                p->u32BoundaryAddress = p->regHeader.info.next - p->u32OddEven;

                p->u32OddEvencb = p->regHeadercb.info.next % BYTES_OF_FLASH_WORD;
                p->u32BoundaryAddresscb = p->regHeadercb.info.next -p->u32OddEvencb;

                p64 = (FLASH_WORD *)p->u32BoundaryAddresscb;
                if(p->u32OddEven) {
                    // Recording a byte of odd address.
                    p->regData64.info.hi.info.idno = p->u16RamCount;
                    p->regData64.info.hi.info.databyte = p->u8RAM[p->u16RamCount];
                    p->regData64.info.hi.info.chksum = getChecksum((uint8_t*)&p->regData64.info.hi.all, 3);
                    p->regData64.info.lo.all = 0xFFFFFFFF;
                    p32 = &p64->u32.Hi;
                    //                    __BKPT(0);
                }
                else {
                    // Recording a byte of even address.
                    p->regData64.info.lo.info.idno = p->u16RamCount;
                    p->regData64.info.lo.info.databyte = p->u8RAM[p->u16RamCount];
                    p->regData64.info.lo.info.chksum = getChecksum((uint8_t*)&p->regData64.info.lo.all, 3);
                    p->regData64.info.hi.all = 0xFFFFFFFF;
                    p32 = &p64->u32.Lo;
//                    __BKPT(0);

                }

                // Check the content in Flash whether is empty.
                if(0xFFFFFFFF != *p32) {
                    p->sPG.fsm = _ERASE_DATA_SECTION;
                }
                else {
                    p->sPG.fsm = _PROGRAM_VALID_DATA64_TO_FLASH;

                }
            }
            p->u16RamCount++;
        }
        else {
            p->sPG.fsm = _RECORD_VALID_DATA_TO_HEADER;
        }
        break;

    case _PROGRAM_VALID_DATA64_TO_FLASH:
        DL_FlashCTL_unprotectSector(FLASHCTL, p->u32BoundaryAddress, DL_FLASHCTL_REGION_SELECT_MAIN);
        DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(FLASHCTL, p->u32BoundaryAddress, (uint32_t *)&p->regData64.block.all[0]);

        p->regHeader.info.next += 4;
        p->regHeadercb.info.next +=4;
        if(EMU_END_OF_FLASH <= p->regHeader.info.next) {
            // If over the range of emulation, then need to roll over the data address.
            p->regHeader.info.next -= EMU_SIZE_OF_DATA;
            p->regHeadercb.info.next -= EMU_SIZE_OF_DATA;
        }
        p->sPG.fsm = _SCAN_VALID_DATA_IN_RAM;
        break;

    case _ERASE_DATA_SECTION:
        p->u32EraseAddress = (uint32_t)(getAddressSection(p->u32BoundaryAddress) * EMU_SIZE_OF_SECTOR);

        DL_FlashCTL_unprotectSector(FLASHCTL, p->u32EraseAddress, DL_FLASHCTL_REGION_SELECT_MAIN);
        DL_FlashCTL_eraseMemoryFromRAM(FLASHCTL, p->u32EraseAddress, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

        if(EMU_KBYTES == 512)
        {
            p->u32EraseAddress += 1024;
            DL_FlashCTL_unprotectSector(FLASHCTL, p->u32EraseAddress, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_eraseMemoryFromRAM(FLASHCTL, p->u32EraseAddress, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
        }

        if (DL_FLASHCTL_COMMAND_STATUS_FAILED == FlashAPIState) {
            p->sPG.fsm |= _MASK_ERROR_FOR_PROG_EEPROM;
        }
        else {
            p->sPG.fsm = _PROGRAM_VALID_DATA64_TO_FLASH;
          //  __BKPT();

        }
        break;

    case _RECORD_VALID_DATA_TO_HEADER:
        p->regHeader.info.chksum = getChecksum((uint8_t*)&p->regHeader.block.all[0], 6);

        if(63 < p->u16HeaderIndex) {
            // Over the section of header, need to format this section, restart from one, remark header index by 0xFFFF,FFFF,FFFF,FFFE.
            p->sPG.fsm = _ERASE_HEADER_SECTION;
        }
        else {
            // Get the address of next package of header.
            p->u32HeaderAddress = (uint32_t)(EMU_START_OF_HEADER + (p->u16HeaderIndex + 1) * BYTES_OF_FLASH_WORD);
            // Save the data information into the section of header about start, stop of data and this package of checksum.
            DL_FlashCTL_unprotectSector(FLASHCTL, p->u32HeaderAddress, DL_FLASHCTL_REGION_SELECT_MAIN);
            FlashAPIState = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(FLASHCTL, p->u32HeaderAddress, (uint32_t *)&p->regHeader.block.all[0]);
            if (DL_FLASHCTL_COMMAND_STATUS_FAILED == FlashAPIState) {
                p->sPG.fsm |= _MASK_ERROR_FOR_PROG_EEPROM;
            }
            else {
                p->sPG.fsm = _REMARK_HEADER_INDEX;
            }
        }
        break;

    case _REMARK_HEADER_INDEX:
        clearBit(&p->HeaderSectionMark.all[0] ,p->u16HeaderIndex++);
        DL_FlashCTL_unprotectSector(FLASHCTL, EMU_START_OF_HEADER, DL_FLASHCTL_REGION_SELECT_MAIN);
        FlashAPIState = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(FLASHCTL, EMU_START_OF_HEADER, (uint32_t *)&p->HeaderSectionMark.all[0]);
        if (DL_FLASHCTL_COMMAND_STATUS_FAILED == FlashAPIState) {
            p->sPG.fsm |= _MASK_ERROR_FOR_PROG_EEPROM;
        }
        else {
            p->sPG.fsm = _END_OF_PROGRAM_EMU_EEPROM;
        }
        break;

    case _ERASE_HEADER_SECTION:
        DL_FlashCTL_unprotectSector(FLASHCTL, EMU_START_OF_HEADER, DL_FLASHCTL_REGION_SELECT_MAIN);
        FlashAPIState = DL_FlashCTL_eraseMemoryFromRAM(FLASHCTL, EMU_START_OF_HEADER, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
        if (DL_FLASHCTL_COMMAND_STATUS_FAILED == FlashAPIState) {
            p->sPG.fsm |= _MASK_ERROR_FOR_PROG_EEPROM;
        }
        else {
            p->u16HeaderIndex = 0;
            p->HeaderSectionMark.u64 = 0xFFFFFFFFFFFFFFFF;
            p->sPG.fsm = _RECORD_VALID_DATA_TO_HEADER;
        }
        break;

    case _END_OF_PROGRAM_EMU_EEPROM:
        p->sPG.fsm = _PREPARE_HEADER_INFORMATION;
        SWTO_EE(_FREE_EMU_EEPROM);
        break;

    default:
        break;
    }
}


void execEmuEeprom(void)
{
    HAL_EMUEE p = &stEmuEE;

    switch(p->fgEmuEE) {
    case _INIT_EMU_EEPROM:
        initEmuEeprom(p);
        break;

    case _CALL_BACK_EMU_EEPROM:
        callbackFromFlash(p);
        break;

    case _FREE_EMU_EEPROM:

        break;


    case _PROGRAM_EMU_EEPROM:
        progEmuEeprom(p);
        break;

    case _VERIFY_EMU_EEPORM:
        varifyFlashByRAM(p);
        break;



    default:
        SET_EE(_MASK_EMU_EEPROM_ERROR);
        break;
    }
}

