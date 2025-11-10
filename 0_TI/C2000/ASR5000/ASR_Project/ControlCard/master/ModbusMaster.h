/*
 * ModbusMaster.h
 *
 *  Created on: Jul 10, 2025
 *      Author: roger_lin
 */

#ifndef MODBUSMASTER_H_
#define MODBUSMASTER_H_

#define DEFAULT_CMDFIFO (CMDFIFO){ \
            .pb = &CmdPack[0], \
            .pi = &CmdPack[0], \
            .po = &CmdPack[0], \
            .pe = &CmdPack[SIZE_OF_FIFO-1] }

#define DEFAULT_CMDPACK (CMDPACK) { \
            .slave = 0, \
            .function = 0, \
            .address = 0, \
            .points = 0, \
            .bytes = 0, \
            .reg = 0 }

#define  SIZE_OF_FIFO    16

typedef struct {
    uint16_t slave;
    uint16_t function;
    uint16_t address;
    uint16_t points;
    uint16_t bytes;
    uint16_t *reg;
} CMDPACK;

typedef struct {
    CMDPACK *pb;
    CMDPACK *pi;
    CMDPACK *po;
    CMDPACK *pe;
} CMDFIFO;



typedef enum {
    TRANSACTION_SUCCESS,
    TRANSACTION_IN_PROGRESS,
    TRANSACTION_FAILED_TIMEOUT,
    TRANSACTION_FAILED_BAD_CRC,
    TRANSACTION_FAILED_EXCEPTION,
    TRANSACTION_FAILED_INVALID_RESP
} TRANSACTION_STATUS;

typedef struct {
    uint16_t slaveID;
    uint16_t functionCode;
    uint16_t startAddr;
    uint16_t numRegs;
    uint16_t *pData;
    uint16_t exceptionCode;
    volatile TRANSACTION_STATUS status;
} MODBUS_TRANSACTION;

typedef enum {
    _MASTER_IDLE               = 0,
    _TRANSACTION_SPI_M,
    _MASTER_PREPARE_REQUEST,
    _MASTER_START_TRANSMISSION,
    _MASTER_WAIT_FOR_RESPONSE,
    _MASTER_PROCESS_RESPONSE,
    _MASTER_TRANSACTION_COMPLETE,
} EV_MASTER_STEP;


typedef struct
{
    uint16_t ID;
    uint16_t FUNC;
    uint16_t Address_hi;
    uint16_t Address_lo;
    uint16_t Quanity_hi;
    uint16_t Quanity_lo;
    uint16_t CRC_Lo;
    uint16_t CRC_Hi;
    uint16_t data_to_crc[8];
} PACKAGE;


typedef struct
{
    EV_MASTER_STEP   FSM;
    PACKAGE         pack;
    uint16_t      u16CNT;
    uint16_t    u16index;
} st_Control;

extern st_Control st_MB;
extern int16_t exeModbusMaster (SCI_MODBUS *mbus);


#endif /* MODBUSMASTER_H_ */
