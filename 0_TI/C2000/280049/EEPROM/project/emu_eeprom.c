/*
 * emu_eeprom.c
 *
 *  Created on: 2023¦~5¤ë15¤é
 *      Author: cody_chen
 */

#include "emu_eeprom.h"

typedef enum
{
    _INIT_EMU_EEPROM = (0x0000),
    _CALL_BACK_EMU_EEPROM = (0x0001 << 0),
    _FREE_EMU_EEPROM = (0x0001 << 1),
    _BUSY_EMU_EEPROM = (0x0001 << 2),
    _PROGRAM_EMU_EEPROM = (0x0001 << 3),
    _VERIFY_EMU_EEPORM = (0x0001 << 4),
    _MASK_EMU_EEPROM_ERROR = (0x8000)
} FS_EMUEE;
typedef uint16_t REG_EMUEE;  // Refer to FS_EMUEE

typedef union
{
    uint16_t all[BYTES_OF_FLASH_WORD];
    uint64_t u64;
    struct
    {
        uint32_t Lo;
        uint32_t Hi;
    } u32;
} FLASH_WORD;
const FLASH_WORD DEFAULT_FLASH_WORD = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };

typedef union
{
    FLASH_WORD block;
    struct
    {
        uint32_t start;       // the data of start address
        uint32_t next;        // the data of next address
    } info;
} REG_HEADER_CONTENT; // Data Range
typedef REG_HEADER_CONTENT *HAL_HEADER_CONTENT;

typedef union
{
    uint32_t all;
    struct
    {
        uint32_t idno;
        uint32_t databyte;
    } info;
} REG_DATA32;
typedef REG_DATA32 *HAL_DATA32;

typedef union
{
    FLASH_WORD block;
    struct
    {
        REG_DATA32 lo;
        REG_DATA32 hi;
    } info;
} REG_DATA64_CONTENT;
typedef REG_DATA64_CONTENT *HAL_DATA64_CONTENT;

typedef enum
{
    _READ_HEADER_INDEX = 0x0000,
    _GET_HEADER_INDEX = (0x0001 << 0),
    _PARSE_HEADER_CONTENT = (0x0001 << 1),
    _READ_FROM_FLASH_TO_RAM = (0x0001 << 2),
    _VERIFY_FLASH_BY_RAM = (0x0001 << 3),
    _MARK_ERROR_FOR_CALLBACK_FSM = 0x8000
} FSM_CALBACK;
typedef struct
{
    FSM_CALBACK fsm;
    uint16_t i;
    uint16_t *ptr;
    uint32_t u32Debug;
} ST_CALBACK;

typedef enum
{
    _PREPARE_HEADER_INFORMATION = 0x0000,
    _SCAN_VALID_DATA_IN_RAM = (0x0001 << 0),
    _PROGRAM_VALID_DATA64_TO_FLASH = (0x0001 << 1),
    _ERASE_DATA_SECTION = (0x0001 << 2),
    _RECORD_VALID_DATA_TO_HEADER = (0x0001 << 3),
    _REMARK_HEADER_INDEX = (0x0001 << 4),
    _ERASE_HEADER_SECTION = (0x0001 << 5),
    _END_OF_PROGRAM_EMU_EEPROM = (0x0001 << 6),
    _MASK_ERROR_FOR_PROG_EEPROM = 0x8000
} FSM_PROGRAM;
typedef struct
{
    FSM_PROGRAM fsm;
    uint16_t i;
    uint16_t *ptr;
    uint32_t u32Debug;
} ST_PROGRAM;

typedef struct
{
    REG_EMUEE fgEmuEE;
    FLASH_WORD HeaderSectionMark;
    uint32_t u16HeaderIndex;
    uint32_t u32HeaderAddress;
    uint16_t u8HeaderChecksum;
    uint32_t u32DataAddress;
    uint16_t u8DataChecksum;
    HAL_DATA32 pDataContent;
    REG_HEADER_CONTENT regHeader;
    REG_DATA64_CONTENT regData64;
    REG_DATA32 regData32;
    uint16_t u16RamCount;
    uint16_t u16RAM[EMU_SIZE_OF_EEPROM];
    uint32_t u32BoundaryAddress;
    uint32_t u32EraseAddress;
    uint32_t u32OddEven;

    uint16_t u16Dword;
    Fapi_FlashProgrammingCommandsType evType;

    ST_CALBACK sCB;
    ST_PROGRAM sPG;
} ST_EMUEE;

typedef ST_EMUEE *HAL_EMUEE;

ST_EMUEE stEmuEE = {
                     .u16Dword = 4,
                     .evType = Fapi_DataOnly };

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

#pragma SET_CODE_SECTION(".TI.ramfunc")

Fapi_StatusType FlashAPIState;

Fapi_StatusType DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
        uint32_t address, uint16_t *data)
{
    ST_EMUEE *p = &stEmuEE;
    FlashAPIState = Fapi_issueProgrammingCommand((uint32*) address,
                                                 (uint16*) data, p->u16Dword, 0,
                                                 0, p->evType);

    while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy)
        ;

    if (FlashAPIState != Fapi_Status_Success)
    {
        // Check Flash API documentation for possible errors
        asm (" ESTOP0");
    }
    return FlashAPIState;
}

uint16_t getEepromErrorStatus(void)
{
    return (stEmuEE.sPG.fsm & _MASK_ERROR_FOR_PROG_EEPROM);
}

uint8_t getChecksum(uint8_t *pByte, uint8_t length)
{
    uint8_t i;
    uint8_t chksum;
    for (i = 0, chksum = 0; i < length; i++)
    {
        chksum += (pByte[i]);
    }

    chksum = 0xFF - chksum;
    chksum = chksum + 0x01;

    return chksum;
}

void clearBit(uint16_t array[], int bitIndex)
{
    int byteIndex = bitIndex / 16; // Find the index of the byte containing the bit
    int bitOffset = bitIndex % 16; // Find the offset of the bit within the byte

    uint16_t mask = ~(1u << bitOffset); // Create a mask with the bit at bitOffset set to 0
    array[byteIndex] &= mask; // Perform bitwise AND with the mask to clear the bit
}

void markBit(uint8_t array[], int bitIndex)
{
    int byteIndex = bitIndex / 8; // Find the index of the byte containing the bit
    int bitOffset = bitIndex % 8;  // Find the offset of the bit within the byte

    uint8_t mask = 1u << bitOffset; // Create a mask with the bit at bitOffset set to 1
    array[byteIndex] |= mask; // Perform bitwise OR with the mask to mark the bit
}

void getMask(uint8_t *pMask, int bitIndex)
{
    int i;
    for (i = 0; i < bitIndex; i++)
    {
        pMask[0] |= (0x01 << i);
    }
}

uint32_t getAddressSection(uint32_t address)
{
    uint32_t section = address / EMU_SIZE_OF_SECTOR; // Calculate the section index
    return section;
}

int16_t writeEmuEepromBlock(uint16_t u16addr, uint8_t *pSrcByte,
                            uint16_t length)
{
    uint16_t u16TempAddr = 0;
    uint16_t i = 0;
    if (_FREE_EMU_EEPROM == stEmuEE.fgEmuEE)
    {
        for (i = 0; i < length; i++)
        {
            u16TempAddr = u16addr + i;
            if (EMU_KBYTES <= u16TempAddr)
                u16TempAddr -= EMU_KBYTES;
            stEmuEE.u16RAM[u16TempAddr] = pSrcByte[i];
        }

        SWTO_EE(_PROGRAM_EMU_EEPROM);
        return i;

    }
    else
    {
        return -1;
    }
}

int16_t writeEmuEeprom(uint16_t u16addr, uint16_t u16Data)
{
    uint32_t u16TempAddr = 0;
    if (EMU_KBYTES <= u16addr)
        u16TempAddr = u16addr - EMU_KBYTES;
    else
        u16TempAddr = u16addr;
    stEmuEE.u16RAM[u16TempAddr] = u16Data;

    return 1;
}

int16_t callbackRAMFromFlash(void)
{
    if (_FREE_EMU_EEPROM == stEmuEE.fgEmuEE)
    {
        SWTO_EE(_CALL_BACK_EMU_EEPROM);
        return 1;
    }
    else
    {
        return 0;
    }
}

int16_t setProgramEmuEeprom(void)
{
    if (_FREE_EMU_EEPROM == stEmuEE.fgEmuEE)
    {
        SWTO_EE(_PROGRAM_EMU_EEPROM);
        return 1;
    }
    else
    {
        return 0;
    }
}

int16_t setVerifyEmuEeprom(void)
{
    if (_FREE_EMU_EEPROM == stEmuEE.fgEmuEE)
    {
        SWTO_EE(_VERIFY_EMU_EEPORM);
        return 1;
    }
    else
    {
        return 0;
    }
}

int8_t waittingEmuEeprom(void)
{

    return (_FREE_EMU_EEPROM == stEmuEE.fgEmuEE);
}

uint16_t* readEmuEeprom(uint16_t u16addr)
{
    uint16_t u16TempAddr = 0;
    if (_FREE_EMU_EEPROM == stEmuEE.fgEmuEE)
    {
        u16TempAddr = u16addr;
        if (EMU_KBYTES <= u16TempAddr)
            u16TempAddr -= EMU_KBYTES;
        return &stEmuEE.u16RAM[u16TempAddr];
    }
    else
    {
        return 0;
    }
}

int16_t isEmuEepromFree(void)
{
    return GET_EE(_FREE_EMU_EEPROM);
}

void initEmuEeprom(HAL_EMUEE p)
{
    if (EMU_SIZE_OF_EEPROM > p->u16RamCount)
    {
        p->u16RAM[p->u16RamCount++] = 0xFFFF;
    }
    else
    {
        p->HeaderSectionMark = DEFAULT_FLASH_WORD;

        p->pDataContent = 0;
        p->sCB.fsm = _READ_HEADER_INDEX;
        p->sCB.ptr = (uint16_t*) EMU_START_OF_HEADER;

        p->sPG.fsm = _PREPARE_HEADER_INFORMATION;
        p->sPG.ptr = (uint16_t*) EMU_START_OF_EEPROM;
        SWTO_EE(_CALL_BACK_EMU_EEPROM);
    }
}

int builtin_ctz64(uint64_t x)
{
    if (x == 0)
    {
        // Special case: x is zero, which has no trailing zeros.
        return 64;
    }

    int count = 0;
    while ((x & 1) == 0)
    {
        // Keep shifting x to the right until the least significant bit becomes 1.
        x >>= 1;
        count++;
    }

    return count;
}

void callbackFromFlash(HAL_EMUEE p)
{

    switch (p->sCB.fsm)
    {
    case _READ_HEADER_INDEX:
        // Copy the header mark in flash to the header mark in ram.
        for (int i = 0; i < BYTES_OF_FLASH_WORD; i++)
        {

            p->HeaderSectionMark.all[i] = p->sCB.ptr[i];
        }

        // Reset Header Index
        p->u16HeaderIndex = 0;
        p->sCB.fsm = _GET_HEADER_INDEX;
        break;

    case _GET_HEADER_INDEX:

        // Get the position of the least significant set bit
        p->u16HeaderIndex = builtin_ctz64(p->HeaderSectionMark.u64);

        if (0 == p->u16HeaderIndex)
        {
            // emuEEPROM is empty.
            p->regHeader.info.start = EMU_START_OF_EEPROM;
            p->regHeader.info.next = EMU_START_OF_EEPROM;
            p->sCB.fsm = _READ_HEADER_INDEX;
            SWTO_EE(_FREE_EMU_EEPROM);
        }
        else
        {
            p->u32HeaderAddress = (uint32_t) (p->sCB.ptr+ p->u16HeaderIndex * BYTES_OF_FLASH_WORD);
            p->sCB.fsm = _PARSE_HEADER_CONTENT;
        }
        break;

    case _PARSE_HEADER_CONTENT:

        p->regHeader = *((HAL_HEADER_CONTENT) p->u32HeaderAddress);
        p->u32DataAddress = (uint32_t) (p->regHeader.info.start);
        p->sCB.fsm = _READ_FROM_FLASH_TO_RAM;

        break;

    case _READ_FROM_FLASH_TO_RAM:

        if (p->u32DataAddress != p->regHeader.info.next)
        {
            // Get a 32bits from the data section.

            p->pDataContent = (HAL_DATA32) p->u32DataAddress;
            p->u16RAM[p->pDataContent->info.idno] =  p->pDataContent->info.databyte;
            // Go to the next data
            p->u32DataAddress += 4;
            if (EMU_END_OF_FLASH <= p->u32DataAddress)
            {
                // If over the range of emulation, then need to roll over the data address.
                p->u32DataAddress -= EMU_SIZE_OF_DATA;
            }

            }
        else
        {
            // Reset FSM for entry next time.
            p->sCB.fsm = _READ_HEADER_INDEX;
            SWTO_EE(_FREE_EMU_EEPROM);
        }
        break;

    default:
        break;
    }
}

void varifyFlashByRAM(HAL_EMUEE p)
{
    int i;

    switch (p->sCB.fsm)
    {

    case _READ_HEADER_INDEX:

        // Copy the header mark in flash to the header mark in ram.
        for (i = 0; i < BYTES_OF_FLASH_WORD; i++)
        {
            p->HeaderSectionMark.all[i] = p->sCB.ptr[i];
        }
        // Reset Header Index
        p->u16HeaderIndex = 0;
        p->sCB.fsm = _GET_HEADER_INDEX;
        break;

    case _GET_HEADER_INDEX:

        p->u16HeaderIndex = builtin_ctz64(p->HeaderSectionMark.u64);

        if (0 == p->u16HeaderIndex)
        {
            // emuEEPROM is empty.
            p->regHeader.info.start = EMU_START_OF_EEPROM;
            p->regHeader.info.next = EMU_START_OF_EEPROM;

            p->sCB.fsm = _READ_HEADER_INDEX;
            SWTO_EE(_FREE_EMU_EEPROM);
        }
        else
        {
            p->u32HeaderAddress = (uint32_t) (p->sCB.ptr
                    + p->u16HeaderIndex * BYTES_OF_FLASH_WORD);
            p->sCB.fsm = _PARSE_HEADER_CONTENT;
        }
        break;

    case _PARSE_HEADER_CONTENT:

        p->regHeader = *((HAL_HEADER_CONTENT) p->u32HeaderAddress);
        p->u32DataAddress = (uint32_t) (p->regHeader.info.start);
        p->sCB.fsm = _VERIFY_FLASH_BY_RAM;
        break;

    case _VERIFY_FLASH_BY_RAM:
        if (p->u32DataAddress != p->regHeader.info.next)
        {
            // Get a 32bits from the data section.
            p->pDataContent = (HAL_DATA32) p->u32DataAddress;

            if (p->u16RAM[p->pDataContent->info.idno]!= p->pDataContent->info.databyte)
            {
                p->sCB.fsm |= _MARK_ERROR_FOR_CALLBACK_FSM;
            }
            else
            {
                // Go to the next data
                p->u32DataAddress += 4;
                if (EMU_END_OF_FLASH <= p->u32DataAddress)
                {
                    // If over the range of emulation, then need to roll over the data address.
                    p->u32DataAddress -= EMU_SIZE_OF_DATA;
                }
            }
        }
        else
        {
            // Reset fsm for entry next time.
            p->sCB.fsm = _READ_HEADER_INDEX;
            SWTO_EE(_FREE_EMU_EEPROM);
        }
        break;

    default:
        break;
    }
}

void progEmuEeprom(HAL_EMUEE p)
{
//    DL_FLASHCTL_COMMAND_STATUS FlashAPIState;
//      Fapi_StatusType  FlashAPIState;

    switch (p->sPG.fsm)
    {
    case _PREPARE_HEADER_INFORMATION:
        p->u16RamCount = 0;
        p->regHeader.info.start = p->regHeader.info.next;
        p->sPG.fsm = _SCAN_VALID_DATA_IN_RAM;

        break;

    case _SCAN_VALID_DATA_IN_RAM:
        if (EMU_SIZE_OF_EEPROM > p->u16RamCount)
        {
            // Scan from 0 to EMU_SIZE_OF_EEPROM
            if (0xFF != p->u16RAM[p->u16RamCount])
            {
                p->regData32.info.idno = p->u16RamCount;
                p->regData32.info.databyte = p->u16RAM[p->u16RamCount];
                p->u32BoundaryAddress = p->regHeader.info.next;

                if (p->regData32.info.databyte == 65535)
                {
                    p->sPG.fsm = _PREPARE_HEADER_INFORMATION;
                }
                // Check the content in Flash whether is empty.
                if (0xFFFFFFFF != *((uint32_t*) p->regHeader.info.next))
                {
                    p->sPG.fsm = _ERASE_DATA_SECTION;
                }
                else
                {
                    p->sPG.fsm = _PROGRAM_VALID_DATA64_TO_FLASH;
                }
            }
            p->u16RamCount++;
        }
        else
        {
            p->sPG.fsm = _RECORD_VALID_DATA_TO_HEADER;
        }
        break;

    case _PROGRAM_VALID_DATA64_TO_FLASH:
        if(p->regData32.info.databyte != 0xFFFF)
        {
        FlashAPIState = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated( p->u32BoundaryAddress, (uint16_t*) &p->regData32.info);
        p->regHeader.info.next += p->u16Dword;
        }
//      FlashAPIState =  DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(p->u32BoundaryAddress, (uint32_t *)&p->u16TestRam);



        if (EMU_END_OF_FLASH <= p->regHeader.info.next)
        {
            // If over the range of emulation, then need to roll over the data address.
            p->regHeader.info.next -= EMU_SIZE_OF_DATA;
        }
        p->sPG.fsm = _SCAN_VALID_DATA_IN_RAM;
        break;

    case _ERASE_DATA_SECTION:
        p->u32EraseAddress = (uint32_t) (getAddressSection(
                p->u32BoundaryAddress) * EMU_SIZE_OF_SECTOR);
        FlashAPIState = Fapi_issueAsyncCommandWithAddress(
                Fapi_EraseSector, (uint32*) p->u32EraseAddress);

        if (Fapi_Error_Fail == FlashAPIState)
        {
            p->sPG.fsm |= _MASK_ERROR_FOR_PROG_EEPROM;
        }
        else
        {
            p->sPG.fsm = _PROGRAM_VALID_DATA64_TO_FLASH;

        }
        break;

    case _RECORD_VALID_DATA_TO_HEADER:
        if (63 < p->u16HeaderIndex)
        {
            // Over the section of header, need to format this section, restart from one, remark header index by 0xFFFF,FFFF,FFFF,FFFE.
            p->sPG.fsm = _ERASE_HEADER_SECTION;
        }
        else
        {
            // Get the address of next package of header.
            p->u32HeaderAddress = (uint32_t) (EMU_START_OF_HEADER
                    + (p->u16HeaderIndex + 1) * BYTES_OF_FLASH_WORD);
            // Save the data information into the section of header about start, stop of data and this package of checksum.
            FlashAPIState = Fapi_issueProgrammingCommand(
                    (uint32*) p->u32HeaderAddress,
                    (uint16*) &p->regHeader.block, p->u16Dword, 0, 0,
                    Fapi_DataOnly);

            if (Fapi_Error_Fail == FlashAPIState)
            {
                p->sPG.fsm |= _MASK_ERROR_FOR_PROG_EEPROM;
            }
            else
            {
                p->sPG.fsm = _REMARK_HEADER_INDEX;
            }
        }
        break;

    case _REMARK_HEADER_INDEX:
        clearBit(&p->HeaderSectionMark.all[0], p->u16HeaderIndex++);
        FlashAPIState = Fapi_issueProgrammingCommand(
                (uint32*) EMU_START_OF_HEADER,
                (uint16*) &p->HeaderSectionMark.all[0], p->u16Dword, 0, 0,
                Fapi_DataOnly);
        if (Fapi_Error_Fail == FlashAPIState)
        {
            p->sPG.fsm |= _MASK_ERROR_FOR_PROG_EEPROM;
        }
        else
        {
            p->sPG.fsm = _END_OF_PROGRAM_EMU_EEPROM;
        }
        break;

    case _ERASE_HEADER_SECTION:
        FlashAPIState = Fapi_issueAsyncCommandWithAddress(
                Fapi_EraseSector, (uint32*) EMU_START_OF_HEADER);
        if (Fapi_Error_Fail == FlashAPIState)
        {
            p->sPG.fsm |= _MASK_ERROR_FOR_PROG_EEPROM;
        }
        else
        {
            p->u16HeaderIndex = 0;
            p->HeaderSectionMark.u64 = 0xFFFFFFFFFFFFFFFF;
            p->sPG.fsm = _RECORD_VALID_DATA_TO_HEADER;
        }
        break;

    case _END_OF_PROGRAM_EMU_EEPROM:

        p->sPG.fsm = _PREPARE_HEADER_INFORMATION;
        SWTO_EE(_FREE_EMU_EEPROM)
        ;
        break;

    default:
        break;
    }
}

void execEmuEeprom(void)
{
    HAL_EMUEE p = &stEmuEE;

    switch (p->fgEmuEE)
    {
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
        SET_EE(_MASK_EMU_EEPROM_ERROR)
        ;
        break;
    }
}

#pragma SET_CODE_SECTION()
