/*
 * eeprom.c
 *
 *  Created on: 2023¦~4¤ë18¤é
 *      Author: cody_chen
 */

#include "common.h"
#include "eeprom.h"

#include "linkEeprom.h"

#define COMMON_HEADER_FORMAT_VERSION     0x01
#define INTERNAL_USER_AREE_OFFSET        0x00
#define CHASSIS_INFO_AREA_OFFSET         0x00
#define BOARD_INFO_AREA_OFFSET           0x01
#define PRODUCT_INFO_AREA_OFFSET         0x00
#define MULTIRECORD_AREA_OFFSET          0x80
#define PAD_OFFSET                       0x00

#define MEMADDR_COMMON_HEADER            0x00
#define MEMSIZE_COMMON_HEADER            0x08

#define MEMADDR_BOARD_OFFSET            (BOARD_INFO_AREA_OFFSET * 8)

#define EEPROM_PAGE_TIMEOUT              T_2MS      //EEPROM_PAGE_TIMEOUT > 1.5msec for 24C01
#define EEPROM_PAGE_SIZE                 16         //16Bytes for 24C01
#define SIZE_OF_MEMERY_ADDRESS           1          //1Byte for 24C01



typedef enum {
    _NO_ACTION_EEPROM = (0x0000),
    _INIT_EEPROM      = (0x0001<<0),
    _POPCMD_EEPROM    = (0x0001<<1),
    _CALLBACK_EEPROM  = (0x0001<<2),
    _FORMAT_EEPROM    = (0x0001<<3),
    _PROGRAM_EEPROM   = (0x0001<<4),
    _RESET_EEPROM     = (0x0001<<14),
    _ERRMASK_EEPROM   = (0x8000)
} EV_EEPROM;
typedef uint16_t FG_EEPROM;

#define SET_EE(bit)  FG_SET(bit, p->fgStat);
#define RST_EE(bit)  FG_RST(bit, p->fgStat);
#define SWTO_EE(bit) FG_SWTO(bit, p->fgStat);

typedef union {
    uint16_t u16Block[8];
    struct {
        uint16_t format_version;
        uint16_t internal_user_area;
        uint16_t chassis_info_area;
        uint16_t board_info_area;
        uint16_t product_info_area;
        uint16_t multi_record_area;
        uint16_t pad_reserved_area;
        uint16_t checksum;
    };
} BLK_COMMON_HEADER;

typedef enum {
    _I2C_TIMER_NO_ACTION = 0,
    _RESET_I2C_TIMER     = (0x0001<<0),
    _GET_AN_I2C_TIMEOUT  = (0x0001<<1)
} EV_I2C_TIMER;


typedef enum {
    _BLKTYP_CONTINUE_VAR_BLOCK = 0,
    _BLKTYP_SPECADDR_VAR_BLOCK = 1
} EE_BTP;
typedef uint16_t EE_BLKTYP;

typedef struct{
    uint16_t *pu16Block;
    uint16_t u16StartOfBlock;
    uint16_t u16SizeOfBlock;
    void     *pregTable;
    uint16_t u16SizeOfTable;
    uint16_t u16BlockType;
} EE_PACK;
typedef EE_PACK * HAL_PACK;

typedef struct {
    uint32_t  u32Base;
    uint16_t  u16SlaveID;
    FG_EEPROM fgStat;

    uint16_t  u16GetHeader;
    uint16_t  u16SetHeader;

    BLK_COMMON_HEADER blkCommonHeader;

    uint16_t  u16GetBoardInfo;
    uint16_t  u16SetBoardInfo;

    uint16_t  u16timeact;
    uint32_t  u32timetick;
    uint32_t  u32timestamp;
    uint32_t  u32timecnt;
    uint32_t  u32timeout;

    uint16_t  u16MemAddr;
    uint16_t  u16PageAddr;
    uint16_t  u16RemainSize;
    uint16_t  u16WriteSize;
    uint16_t  u16ReadSize;
    uint16_t  u16Index;
    uint16_t  u16Offset;
    uint16_t  u16Rsize;
    uint16_t  u16Checksum;

    uint16_t  *pu16BlockStep;
    HAL_PACK  pPack;


} ST_EEPROM;
typedef ST_EEPROM* HAL_EEPROM;


ST_EEPROM sEE = {
        .u32Base = EEPROM_SRC,
        .u16SlaveID = EEPROM_ADDR,
        .fgStat = _INIT_EEPROM
};

uint16_t getChecksum(uint16_t * pByte, uint16_t length)
{
    uint16_t i;
    uint16_t chksum;
    for(i=0, chksum=0; i<length; i++) {
        chksum += (pByte[i]&0x00FF);
    }

    chksum = 0x0100 - chksum;

    return chksum;
}

void cntEepormTimeout(HAL_EEPROM v)
{
    if(_RESET_I2C_TIMER == v->u16timeact) {
        v->u32timetick = SW_TIMER - (uint32_t)CPUTimer_getTimerCount(SWTIRMER_BASE);

        if(v->u32timestamp > v->u32timetick) {
            v->u32timeout = v->u32timetick + SW_TIMER - v->u32timestamp;
        }
        else {
            v->u32timeout = v->u32timetick - v->u32timestamp;
        }

        if(v->u32timeout >= EEPROM_PAGE_TIMEOUT) {
            v->u16timeact = _GET_AN_I2C_TIMEOUT;
        }
    }
}

uint16_t getEepromTimeout(HAL_EEPROM v)
{
    return (uint16_t)(_GET_AN_I2C_TIMEOUT == v->u16timeact);
}

void rstEepormTimeout(HAL_EEPROM v)
{
    v->u32timeout = 0;
    v->u32timestamp = v->u32timetick = SW_TIMER - (uint32_t)CPUTimer_getTimerCount(SWTIRMER_BASE);
    v->u16timeact = _RESET_I2C_TIMER;
}

void stopEepormTimeout(HAL_EEPROM v)
{
    v->u16timeact = _I2C_TIMER_NO_ACTION;
}

typedef enum {
    _I2C_NO_ACTION = 0,
    _I2C_INITIAL_CTRL_REG          = (0x0001<<0),
    _I2C_SEND_SLAVE_ADDRESS        = (0x0001<<1),
    _I2C_SEND_MEM_ADDRESS          = (0x0001<<2),
    _I2C_WAIT_FOR_MADDR_READY      = (0x0001<<3),
    _I2C_RECEIVE_DATA_FROM_EEPROM  = (0x0001<<4),
    _I2C_TRANSFER_DATA_TO_EEPROM   = (0x0001<<5),
    _I2C_EEPROM_FREE               = (0x0001<<6),
    _I2C_PACK_BLCOK_DATA           = (0x0001<<7),
    _I2C_UNPACK_BLCOK_DATA         = (0x0001<<8),
    _I2C_EXEC_PAGE_WRITE           = (0x0001<<9),
    _I2C_WAIT_FOR_TIMEOUT          = (0x0001<<10),
    _I2C_ERROR_MASK                = (0x8000)
} EV_I2C_ACTION;

uint16_t u16EEHeader[MEMSIZE_COMMON_HEADER];


const EE_PACK packCommon = {
      .pu16Block = &u16EEHeader[0],
      .u16StartOfBlock = MEMADDR_COMMON_HEADER,
      .u16SizeOfBlock = MEMSIZE_COMMON_HEADER,
      .pregTable = (void*)&sEE.blkCommonHeader.u16Block[0],
      .u16SizeOfTable = MEMSIZE_COMMON_HEADER,
      .u16BlockType = _BLKTYP_CONTINUE_VAR_BLOCK
};

const EE_PACK packBoardInfo = {
      .pu16Block = &blkUserRegs[0],
      .u16StartOfBlock = MEMADDR_BOARD_OFFSET,
      .u16SizeOfBlock = _BLK16_USERREGS,
      .pregTable = (void*)&regUserRegs[0],
      .u16SizeOfTable = _TABLE_USERREGS,
      .u16BlockType = _BLKTYP_SPECADDR_VAR_BLOCK
};

#define PTR_TABLE  ((HAL_EEREG)v->pPack->pregTable)
#define PTR_VAR    ((uint16_t*)v->pPack->pregTable)
#define PTR_BLOCK  (v->pPack->pu16Block)

uint16_t cpyVar2Blk(void *blk, void *var, uint16_t bsize)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t chksum = 0;

    uint16_t *u16blk = (uint16_t *)blk;
    uint16_t *u16var = (uint16_t *)var;
    uint16_t u16mod = 0;

    for(i=0, j=0, chksum=0; i<bsize; i++) {
        u16mod = i & 0x0001;
        if(0 == u16mod) {
            u16blk[i] = (u16var[j]>>0) & 0x00FF;
        }
        else {
            u16blk[i] = (u16var[j]>>8) & 0x00FF;
            j++;
        }
        chksum += u16blk[i];
    }

    return chksum;
}

uint16_t cpyBlk2Var(void *blk, void *var, uint16_t bsize)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t chksum = 0;

    uint16_t *u16blk = (uint16_t *)blk;
    uint16_t *u16var = (uint16_t *)var;
    uint16_t u16mod = 0;

    for(i=0, j=0, chksum=0; i<bsize; i++) {
        u16mod = i & 0x0001;
        if(0 == u16mod) {
            //u16blk[i] = (u16var[j]>>0) & 0x00FF;
            u16var[j] = (u16blk[i]<<0) & 0x00FF;
        }
        else {
            //u16blk[i] = (u16var[j]>>8) & 0x00FF;
            u16var[j] |= (u16blk[i]<<8) & 0xFF00;
            j++;
        }
        chksum += u16blk[i];
    }

    return chksum;
}

#define SET_BLK(bit)  FG_SET(bit, *v->pu16BlockStep);
#define RST_BLK(bit)  FG_RST(bit, *v->pu16BlockStep);
#define SWTO_BLK(bit) FG_SWTO(bit, *v->pu16BlockStep);
#define GET_BLK(bit)  FG_GET(bit, *v->pu16BlockStep)
#define AND_BLK(bit)  FG_AND(bit, *v->pu16BlockStep)

uint16_t setBlock(HAL_EEPROM v)
{

    switch(*v->pu16BlockStep) {
    case _I2C_INITIAL_CTRL_REG:
        v->u16MemAddr = v->pPack->u16StartOfBlock;
        v->u16PageAddr =  EEPROM_PAGE_SIZE - v->u16MemAddr % EEPROM_PAGE_SIZE;
        v->u16RemainSize = v->pPack->u16SizeOfBlock;
        v->u16WriteSize =  v->u16RemainSize < v->u16PageAddr ? v->u16RemainSize : v->u16PageAddr;
        v->u16Checksum = 0;
        v->u16Index = 0;
        v->u16Offset = 0;

        rstEepormTimeout(v);
        stopEepormTimeout(v);
        SWTO_BLK(_I2C_PACK_BLCOK_DATA);
        break;

    case _I2C_PACK_BLCOK_DATA:
        if((v->pPack->u16SizeOfTable-1) > v->u16Index) {
            switch(v->pPack->u16BlockType) {
            case _BLKTYP_CONTINUE_VAR_BLOCK:
                // Copy each variables to the block.
                v->u16Checksum += cpyVar2Blk((void*)&PTR_BLOCK[v->u16Offset], (void*)(&PTR_VAR[v->u16Index]), 1);
                v->u16Checksum &= 0x00FF;
                v->u16Offset+=1;
                v->u16Index++;
                break;
            case _BLKTYP_SPECADDR_VAR_BLOCK:
                // Copy each variables to the block.
                v->u16Checksum += cpyVar2Blk((void*)&PTR_BLOCK[v->u16Offset], (void*)(PTR_TABLE[v->u16Index].ptr), PTR_TABLE[v->u16Index].size);
                v->u16Checksum &= 0x00FF;
                v->u16Offset+=PTR_TABLE[v->u16Index].size;
                v->u16Index++;
                break;
            default:
                SET_BLK(_I2C_ERROR_MASK);
                break;
            }
        }
        else {
            // Calculate the checksum & Save the checksum into Block.
            PTR_BLOCK[v->pPack->u16SizeOfBlock-1] = (0x0100 - v->u16Checksum) & 0x00FF;
            // Reset the index of table.
            v->u16Index = 0;
            v->u16Offset = 0;
            SWTO_BLK(_I2C_SEND_SLAVE_ADDRESS);
        }
        break;

    case _I2C_SEND_SLAVE_ADDRESS:
       // Setup slave address
        I2C_setSlaveAddress(v->u32Base, v->u16SlaveID);
        I2C_setConfig(v->u32Base, I2C_MASTER_SEND_MODE);
        I2C_sendStartCondition(v->u32Base);
        SWTO_BLK(_I2C_SEND_MEM_ADDRESS);
        break;

    case _I2C_SEND_MEM_ADDRESS:
        I2C_setDataCount(v->u32Base, v->u16WriteSize + SIZE_OF_MEMERY_ADDRESS);
#if (1 == SIZE_OF_MEMERY_ADDRESS)
        I2C_putData(v->u32Base,  v->u16MemAddr);
        SWTO_BLK(_I2C_TRANSFER_DATA_TO_EEPROM);
#elif (2 == SIZE_OF_MEMERY_ADDRESS)
        I2C_putData(v->u32Base,  (v->u16MemAddr>>8)&0x00FF);  // High Byte
        I2C_putData(v->u32Base,  (v->u16MemAddr>>0)&0x00FF);  // Low Byte
        SWTO_BLK(_I2C_TRANSFER_DATA_TO_EEPROM);
#else
        SET_BLK(_I2C_ERROR_MASK);
#endif
        break;


    case _I2C_TRANSFER_DATA_TO_EEPROM:
        if((v->u16Index < v->u16WriteSize)&&(0xF > I2C_getTxFIFOStatus(v->u32Base))) {
            I2C_putData(v->u32Base, PTR_BLOCK[v->u16Index + v->u16Offset]);
            v->u16Index++;
        }
        else if(0x0 < I2C_getTxFIFOStatus(v->u32Base)) {
            // Wait for sending all out
        }
        else {
            rstEepormTimeout(v);
            v->u16RemainSize -= v->u16WriteSize;
            v->u16MemAddr += v->u16WriteSize;
            v->u16PageAddr =  EEPROM_PAGE_SIZE - v->u16MemAddr % EEPROM_PAGE_SIZE;
            I2C_sendStopCondition(v->u32Base);
            SWTO_BLK(_I2C_WAIT_FOR_TIMEOUT);
        }

        break;

    case _I2C_WAIT_FOR_TIMEOUT:
        if(getEepromTimeout(v)) {
            rstEepormTimeout(v);
            stopEepormTimeout(v);
            if(0 < v->u16RemainSize){
                // Save the current write size as the next starting address.
                v->u16Offset = v->u16WriteSize;
                // Reset the index of block.
                v->u16Index = 0;
                // Get the next write size.
                v->u16WriteSize = (v->u16RemainSize < v->u16PageAddr ? v->u16RemainSize : v->u16PageAddr);
                SWTO_BLK(_I2C_SEND_SLAVE_ADDRESS);
            }
            else {
                SWTO_BLK(_I2C_EEPROM_FREE);
            }
        }
        break;

    case _I2C_EEPROM_FREE:

        break;

    case _I2C_NO_ACTION:
    default:
        if(GET_BLK(_I2C_ERROR_MASK)) {
            // Wait for cleaning this error mask
        }
        else {
            SWTO_BLK(_I2C_INITIAL_CTRL_REG);
        }
        break;
    }

    return *v->pu16BlockStep;
}

uint16_t getBlock(HAL_EEPROM v)
{

    switch(*v->pu16BlockStep) {
    case _I2C_INITIAL_CTRL_REG:
        v->u16MemAddr = v->pPack->u16StartOfBlock;
        v->u16ReadSize = v->pPack->u16SizeOfBlock;
        v->u16Checksum = 0;
        v->u16Index = 0;

        rstEepormTimeout(v);
        stopEepormTimeout(v);
        SWTO_BLK(_I2C_SEND_SLAVE_ADDRESS);
        break;

    case _I2C_SEND_SLAVE_ADDRESS:
       // Setup slave address
        I2C_setSlaveAddress(v->u32Base, v->u16SlaveID);
        I2C_setConfig(v->u32Base, I2C_MASTER_SEND_MODE);
        I2C_sendStartCondition(v->u32Base);
        SWTO_BLK(_I2C_SEND_MEM_ADDRESS);
        break;

    case _I2C_SEND_MEM_ADDRESS:
        I2C_setDataCount(v->u32Base, SIZE_OF_MEMERY_ADDRESS);
        rstEepormTimeout(v);
#if (1 == SIZE_OF_MEMERY_ADDRESS)
        I2C_putData(v->u32Base,  v->u16MemAddr);
        SWTO_BLK(_I2C_WAIT_FOR_MADDR_READY);
#elif (2 == SIZE_OF_MEMERY_ADDRESS)
        I2C_putData(v->u32Base,  (v->u16MemAddr>>8)&0x00FF);  // High Byte
        I2C_putData(v->u32Base,  (v->u16MemAddr>>0)&0x00FF);  // Low Byte
        SWTO_BLK(_I2C_WAIT_FOR_MADDR_READY);
#else
        SET_BLK(_I2C_ERROR_MASK);
#endif
        break;

    case _I2C_WAIT_FOR_MADDR_READY:
        if(0x0 == I2C_getTxFIFOStatus(v->u32Base)) {
            v->u16Index = 0;
            I2C_setSlaveAddress(v->u32Base, v->u16SlaveID);
            I2C_setConfig(v->u32Base, I2C_MASTER_RECEIVE_MODE);
            I2C_setDataCount(v->u32Base, v->u16ReadSize);
            I2C_sendStartCondition(v->u32Base);
            rstEepormTimeout(v);
            SWTO_BLK(_I2C_RECEIVE_DATA_FROM_EEPROM);
        }

        if(getEepromTimeout(v)) {
            SWTO_BLK(_I2C_ERROR_MASK);
        }
        break;

    case _I2C_RECEIVE_DATA_FROM_EEPROM:
        while(I2C_getRxFIFOStatus(v->u32Base)) {
            rstEepormTimeout(v);
            PTR_BLOCK[v->u16Index] = I2C_getData(v->u32Base);
            v->u16Checksum += PTR_BLOCK[v->u16Index];
            v->u16Checksum &= 0x00FF;
            v->u16Index++;
            if(v->u16ReadSize == v->u16Index) {
                v->u16Index = 0;
                v->u16Offset = 0;
                v->u16Checksum = (0x100 - v->u16Checksum) & 0x00FF;
                I2C_sendStopCondition(v->u32Base);
                stopEepormTimeout(v);

                if(0 == v->u16Checksum) {
                    SWTO_BLK(_I2C_UNPACK_BLCOK_DATA);
                }
                else {
                    SET_BLK(_I2C_ERROR_MASK);
                }
            }
        }

        if(getEepromTimeout(v)) {
            SWTO_BLK(_I2C_ERROR_MASK);
        }
        break;

    case _I2C_UNPACK_BLCOK_DATA:
        if(v->u16Index < v->pPack->u16SizeOfTable) {
            switch(v->pPack->u16BlockType) {
            case _BLKTYP_CONTINUE_VAR_BLOCK:
                cpyBlk2Var((void*)&PTR_BLOCK[v->u16Offset], (void*)(&PTR_VAR[v->u16Index]), 1);
                v->u16Offset+=1;
                v->u16Index++;
                break;
            case _BLKTYP_SPECADDR_VAR_BLOCK:
                cpyBlk2Var((void*)&PTR_BLOCK[v->u16Offset], (void*)(PTR_TABLE[v->u16Index].ptr), PTR_TABLE[v->u16Index].size);
                v->u16Offset+=PTR_TABLE[v->u16Index].size;
                v->u16Index++;
                break;
            default:
                SET_BLK(_I2C_ERROR_MASK);
                break;
            }
        }
        else {
            rstEepormTimeout(v);
            SWTO_BLK(_I2C_WAIT_FOR_TIMEOUT);
        }
        break;

    case _I2C_WAIT_FOR_TIMEOUT:
        if(getEepromTimeout(v)) {
            rstEepormTimeout(v);
            stopEepormTimeout(v);
            SWTO_BLK(_I2C_EEPROM_FREE);
        }
        break;

    case _I2C_EEPROM_FREE:

        break;

    case _I2C_NO_ACTION:
    default:
        if(GET_BLK(_I2C_ERROR_MASK)) {
            // Wait for cleaning this error mask
        }
        else {
            SWTO_BLK(_I2C_INITIAL_CTRL_REG);
        }
        break;
    }

    return *v->pu16BlockStep;
}

uint16_t setBoardInfo(HAL_EEPROM v)
{
    if(&v->u16SetBoardInfo != v->pu16BlockStep) {
        v->pu16BlockStep = &v->u16SetBoardInfo;
        v->pPack = (HAL_PACK)&packBoardInfo;
    }

    return setBlock(v);
}


uint16_t getBoardInfo(HAL_EEPROM v)
{
    if(&v->u16GetBoardInfo != v->pu16BlockStep) {
        v->pu16BlockStep = &v->u16GetBoardInfo;
        v->pPack = (HAL_PACK)&packBoardInfo;
    }

    return getBlock(v);
}



uint16_t getCommonHeader(HAL_EEPROM v)
{

    if(&v->u16GetHeader != v->pu16BlockStep) {
        v->pu16BlockStep = &v->u16GetHeader;
        v->pPack = (HAL_PACK)&packCommon;
    }

    return getBlock(v);
}


uint16_t setCommonHeader(HAL_EEPROM v)
{

    if(&v->u16SetHeader != v->pu16BlockStep) {

        v->blkCommonHeader.format_version = COMMON_HEADER_FORMAT_VERSION;
        v->blkCommonHeader.internal_user_area = INTERNAL_USER_AREE_OFFSET;
        v->blkCommonHeader.chassis_info_area = CHASSIS_INFO_AREA_OFFSET;
        v->blkCommonHeader.board_info_area = BOARD_INFO_AREA_OFFSET;
        v->blkCommonHeader.product_info_area = PRODUCT_INFO_AREA_OFFSET;
        v->blkCommonHeader.multi_record_area = MULTIRECORD_AREA_OFFSET;
        v->blkCommonHeader.pad_reserved_area = PAD_OFFSET;
        v->blkCommonHeader.checksum = getChecksum(v->blkCommonHeader.u16Block, MEMSIZE_COMMON_HEADER-1);

        v->pu16BlockStep = &v->u16SetHeader;
        v->pPack = (HAL_PACK)&packCommon;
    }

    return setBlock(v);
}


void execEEPROM(void)
{
    HAL_EEPROM p = &sEE;

    switch(p->fgStat) {
    case _INIT_EEPROM:
        getCommonHeader(p);
        break;

    case _RESET_EEPROM:
        setCommonHeader(p);
        break;

    case _PROGRAM_EEPROM:
        setBoardInfo(p);
        break;

    case _CALLBACK_EEPROM:
        getBoardInfo(p);
        break;

    default:
        p->u32Base = EEPROM_SRC;
        p->u16SlaveID = EEPROM_ADDR;
        rstEepormTimeout(p);
        stopEepormTimeout(p);
        SET_EE(_INIT_EEPROM);
        break;
    }

    cntEepormTimeout(p);

}

