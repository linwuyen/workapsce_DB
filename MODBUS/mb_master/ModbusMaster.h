/*
 * ModbusSlave.h
 *
 *  Created on: 2018¦~3¤ë26¤é
 *      Author: user
 */

#ifndef MODBUSSLAVE_H_
#define MODBUSSLAVE_H_

#include "ModbusCommon.h"
#include "ModbusSci.h"     // Device Headerfile and Examples Include File
#include "CRC16.h"


typedef struct {
	Uint16 HiByte;
	Uint16 LoByte;
} U16REG;

typedef struct {
	Uint16 Slave;
	Uint16 Function;
	U16REG Address;
} MODBUS_HEADER;

typedef struct {
	Uint16 Slave;
	Uint16 Function;
	Uint16 ByteCount;
	U16REG Data;
} HEADER_READ_HOLDINGREG;


typedef struct {
	Uint16 Slave;
	Uint16 Function;
	U16REG Address;
	U16REG Data;
} HEADER_WRITE_HOLDINGREG;

typedef struct {
	Uint16 Slave;
	Uint16 Function;
	U16REG Address;
	U16REG Points;
	Uint16 Bytes;
	U16REG Data;
} HEADER_WRITE_N_HOLDINGREG;

typedef struct {
	Uint16 Slave;
	Uint16 Function;
	Uint16 ErrorNo;
	U16REG CRC;
} HEADER_ERROR_REG;


typedef enum{
	_INIT_SCI_GPIO = 0,
	_INIT_SCI_CONFIG,
	_POP_COMMAND_OUT,
	_EXE_RW_COMMAND,
	_SAVE_ERROR_LOG,
	_PROCESS_TIMEOUT,
	_RESET_FIFO_DATA
} EV_MODBUS;

#define DEFAULT_SCI_MODBUS (SCI_MODBUS) { \
		.evstep = _INIT_SCI_GPIO, \
		.state = 0}

typedef struct {
	Uint16 slave;
	Uint16 function;
	Uint16 address;
	Uint16 points;
	Uint16 bytes;
	Uint16 *reg;
} CMDPACK;

#define DEFAULT_CMDPACK (CMDPACK) { \
			.slave = 0, \
			.function = 0, \
			.address = 0, \
			.points = 0, \
			.bytes = 0, \
			.reg = 0 }

typedef enum {
	_SEND_SLAVEID_AND_FUNCID = 0,
	_SEND_READING_FROM_ADDRESS,
	_SEND_HOW_MANY_READING_POINTS,
	_SEND_BYTE_COUNTS,
	_SEND_ALL_OF_DATA,
	_SEND_A_DATA_OUT,
	_SEND_CRC_OUT,
	_RECEIVE_ALL_OF_DATA,
	_PROCESS_ALL_OF_DATA
} EV_HOLDINGREG;

typedef struct {
	ST_CRC tbcrc;
	Uint16 crc;
	Uint16 *pRX;
	Uint16 m_index;
} ST_HOLDINGINFO;

#define DEFAULT_HOLDINGINFO (ST_HOLDINGINFO) { \
		.tbcrc = DEFAULT_CRC, \
		.crc = 0x0000, \
		.pRX = 0, \
		.m_index = 0 }

typedef struct {
	CMDPACK *pData;
	EV_MODBUS evstep;	//Event Step;
	MODBUS_STATUS state;
	EV_HOLDINGREG evHoldReg;
	ST_HOLDINGINFO stHoldInfo;
	void (*initgpio)(void);
	volatile struct SCI_REGS *sci;
	Uint16 RxBuffer[256];
	Uint32 tick1msec;
} SCI_MODBUS;

#define DEFAULT_DISPMODBUS (SCI_MODBUS){ \
	.initgpio = InitSciaGpio, \
	.sci = &SciaRegs, \
	.stHoldInfo = DEFAULT_HOLDINGINFO, \
	.evHoldReg = _SEND_SLAVEID_AND_FUNCID }

extern Uint16 DisplayRegister[8];
extern Uint16 KeyRegister[2];
extern SCI_MODBUS mbcomm;

extern int16 pushCmdPack(CMDPACK * src);
extern CMDPACK * popCmdPack(void);
extern int16 exeModbusMaster(SCI_MODBUS *mbus);

#endif /* MODBUSSLAVE_H_ */
