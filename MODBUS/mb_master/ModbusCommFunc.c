/*
 * ModbusCommFunc.c
 *
 *  Created on: 2018¦~3¤ë28¤é
 *      Author: user
 */

#include "ModbusMaster.h"
#include "ModbusSci.h"



int16 ReservedFunction(SCI_MODBUS *pMbus)
{
	return MBUS_FAIL;
}

//#pragma CODE_SECTION(readHoldingRegister, "ramfuncs");
int16 readHoldingRegister(SCI_MODBUS *pMbus)
{
	HEADER_READ_HOLDINGREG *pack = 0;
	U16REG temp = {0, 0};
	U16REG *combine = 0;
	Uint16 m_words = 0;
	Uint16 m_addr = 0;

	while(2 > pMbus->sci->SCIFFTX.bit.TXFFST) {
		switch(pMbus->evHoldReg) {
		case _SEND_SLAVEID_AND_FUNCID:
			pMbus->tick1msec = 0;
			pMbus->stHoldInfo.tbcrc = DEFAULT_CRC;
			pMbus->stHoldInfo.crc = 0x0000;
			pMbus->sci->SCITXBUF = pMbus->pData->slave;
			ucMBCRC16(pMbus->pData->slave, &pMbus->stHoldInfo.tbcrc);
			pMbus->sci->SCITXBUF = pMbus->pData->function;
			ucMBCRC16(pMbus->pData->function, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_READING_FROM_ADDRESS;
			break;
		case _SEND_READING_FROM_ADDRESS:
			pMbus->tick1msec = 0;
			m_addr = pMbus->pData->address + 40000;
			temp.HiByte = (m_addr>>8)&0x00FF;
			pMbus->sci->SCITXBUF = temp.HiByte;
			ucMBCRC16(temp.HiByte, &pMbus->stHoldInfo.tbcrc);
			temp.LoByte = m_addr&0x00FF;
			pMbus->sci->SCITXBUF = temp.LoByte;
			ucMBCRC16(temp.LoByte, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_HOW_MANY_READING_POINTS;
			break;

		case _SEND_HOW_MANY_READING_POINTS:
			pMbus->tick1msec = 0;
			temp.HiByte = (pMbus->pData->points>>8)&0x00FF;
			pMbus->sci->SCITXBUF = temp.HiByte;
			ucMBCRC16(temp.HiByte, &pMbus->stHoldInfo.tbcrc);
			temp.LoByte = pMbus->pData->points&0x00FF;
			pMbus->sci->SCITXBUF = temp.LoByte;
			pMbus->stHoldInfo.crc = ucMBCRC16(temp.LoByte, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_CRC_OUT;
			break;

		case _SEND_CRC_OUT:
			pMbus->tick1msec = 0;
			pMbus->sci->SCITXBUF = (pMbus->stHoldInfo.crc>>0)&0x00FF;
			pMbus->sci->SCITXBUF = (pMbus->stHoldInfo.crc>>8)&0x00FF;
			pMbus->tick1msec = 0;
			pMbus->stHoldInfo.tbcrc = DEFAULT_CRC;
			pMbus->stHoldInfo.crc = 0x0000;
			pMbus->stHoldInfo.pRX = &pMbus->RxBuffer[0];
			pMbus->evHoldReg = _RECEIVE_ALL_OF_DATA;
			break;

		case _RECEIVE_ALL_OF_DATA:
			if(0 < pMbus->sci->SCIFFRX.bit.RXFFST){
				pMbus->tick1msec = 0;
				*pMbus->stHoldInfo.pRX = pMbus->sci->SCIRXBUF.all & 0x00FF;
				pMbus->stHoldInfo.crc = ucMBCRC16(*pMbus->stHoldInfo.pRX, &pMbus->stHoldInfo.tbcrc);
				pMbus->stHoldInfo.pRX++;
				if(0 == pMbus->stHoldInfo.crc) 	pMbus->evHoldReg = _PROCESS_ALL_OF_DATA;
			}
			return MBUS_WAIT;

		case _PROCESS_ALL_OF_DATA:
			pMbus->tick1msec = 0;
			pack = (HEADER_READ_HOLDINGREG *)&pMbus->RxBuffer[0];
			pMbus->evHoldReg = _SEND_SLAVEID_AND_FUNCID;
			if(0x0080&pack->Function) return MBUS_FAIL;
			else {
				combine = &pack->Data;
				m_words = pack->ByteCount/2;
				for(pMbus->stHoldInfo.m_index = 0; pMbus->stHoldInfo.m_index < m_words; pMbus->stHoldInfo.m_index++, combine++) {
					pMbus->pData->reg[pMbus->stHoldInfo.m_index] = ((combine->HiByte<<8)&0xFF00) | (combine->LoByte&0x00FF);
				}
				return MBUS_SUCCESS;
			}

		default:
			break;
		}
	}

	return MBUS_WAIT;
}

//#pragma CODE_SECTION(writeHoldingRegister, "ramfuncs");
int16 writeHoldingRegister(SCI_MODBUS *pMbus)
{
	HEADER_READ_HOLDINGREG *pack = 0;
	U16REG temp = {0, 0};
	Uint16 m_addr = 0;

	while(2 > pMbus->sci->SCIFFTX.bit.TXFFST) {
		switch(pMbus->evHoldReg) {
		case _SEND_SLAVEID_AND_FUNCID:
			pMbus->tick1msec = 0;
			pMbus->stHoldInfo.tbcrc = DEFAULT_CRC;
			pMbus->stHoldInfo.crc = 0x0000;
			pMbus->sci->SCITXBUF = pMbus->pData->slave;
			ucMBCRC16(pMbus->pData->slave, &pMbus->stHoldInfo.tbcrc);
			pMbus->sci->SCITXBUF = pMbus->pData->function;
			ucMBCRC16(pMbus->pData->function, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_READING_FROM_ADDRESS;
			break;
		case _SEND_READING_FROM_ADDRESS:
			pMbus->tick1msec = 0;
			m_addr = pMbus->pData->address + 40000;
			temp.HiByte = (m_addr>>8)&0x00FF;
			pMbus->sci->SCITXBUF = temp.HiByte;
			ucMBCRC16(temp.HiByte, &pMbus->stHoldInfo.tbcrc);
			temp.LoByte = m_addr&0x00FF;
			pMbus->sci->SCITXBUF = temp.LoByte;
			ucMBCRC16(temp.LoByte, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_A_DATA_OUT;
			break;

		case _SEND_A_DATA_OUT:
			pMbus->tick1msec = 0;
			temp.HiByte = (pMbus->pData->reg[0]>>8)&0x00FF;
			pMbus->sci->SCITXBUF = temp.HiByte;
			ucMBCRC16(temp.HiByte, &pMbus->stHoldInfo.tbcrc);
			temp.LoByte = pMbus->pData->reg[0]&0x00FF;
			pMbus->sci->SCITXBUF = temp.LoByte;
			pMbus->stHoldInfo.crc = ucMBCRC16(temp.LoByte, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_CRC_OUT;
			break;

		case _SEND_CRC_OUT:
			pMbus->tick1msec = 0;
			pMbus->sci->SCITXBUF = (pMbus->stHoldInfo.crc>>0)&0x00FF;
			pMbus->sci->SCITXBUF = (pMbus->stHoldInfo.crc>>8)&0x00FF;
			pMbus->stHoldInfo.tbcrc = DEFAULT_CRC;
			pMbus->stHoldInfo.crc = 0x0000;
			pMbus->stHoldInfo.pRX = &pMbus->RxBuffer[0];
			pMbus->evHoldReg = _RECEIVE_ALL_OF_DATA;
			break;

		case _RECEIVE_ALL_OF_DATA:
			if(0 < pMbus->sci->SCIFFRX.bit.RXFFST){
				pMbus->tick1msec = 0;
				*pMbus->stHoldInfo.pRX = pMbus->sci->SCIRXBUF.all & 0x00FF;
				pMbus->stHoldInfo.crc = ucMBCRC16(*pMbus->stHoldInfo.pRX, &pMbus->stHoldInfo.tbcrc);
				pMbus->stHoldInfo.pRX++;
				if(0 == pMbus->stHoldInfo.crc) 	pMbus->evHoldReg = _PROCESS_ALL_OF_DATA;
			}
			return MBUS_WAIT;

		case _PROCESS_ALL_OF_DATA:
			pMbus->tick1msec = 0;
			pack = (HEADER_READ_HOLDINGREG *)&pMbus->RxBuffer[0];
			pMbus->evHoldReg = _SEND_SLAVEID_AND_FUNCID;
			if(0x0080&pack->Function) return MBUS_FAIL;
			else return MBUS_SUCCESS;

		default:
			break;
		}
	}

	return MBUS_WAIT;
}

//#pragma CODE_SECTION(writeHoldingNRegister, "ramfuncs");
int16 writeHoldingNRegister(SCI_MODBUS *pMbus)
{
	HEADER_READ_HOLDINGREG *pack = 0;
	U16REG temp = {0, 0};
	Uint16 m_addr = 0;

	while(2 > pMbus->sci->SCIFFTX.bit.TXFFST) {
		switch(pMbus->evHoldReg) {
		case _SEND_SLAVEID_AND_FUNCID:
			pMbus->tick1msec = 0;
			pMbus->stHoldInfo.tbcrc = DEFAULT_CRC;
			pMbus->stHoldInfo.crc = 0x0000;
			pMbus->stHoldInfo.m_index = 0;
			pMbus->sci->SCITXBUF = pMbus->pData->slave;
			ucMBCRC16(pMbus->pData->slave, &pMbus->stHoldInfo.tbcrc);
			pMbus->sci->SCITXBUF = pMbus->pData->function;
			ucMBCRC16(pMbus->pData->function, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_READING_FROM_ADDRESS;
			break;
		case _SEND_READING_FROM_ADDRESS:
			pMbus->tick1msec = 0;
			m_addr = pMbus->pData->address + 40000;
			temp.HiByte = (m_addr>>8)&0x00FF;
			pMbus->sci->SCITXBUF = temp.HiByte;
			ucMBCRC16(temp.HiByte, &pMbus->stHoldInfo.tbcrc);
			temp.LoByte = m_addr&0x00FF;
			pMbus->sci->SCITXBUF = temp.LoByte;
			ucMBCRC16(temp.LoByte, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_HOW_MANY_READING_POINTS;
			break;

		case _SEND_HOW_MANY_READING_POINTS:
			pMbus->tick1msec = 0;
			temp.HiByte = (pMbus->pData->points>>8)&0x00FF;
			pMbus->sci->SCITXBUF = temp.HiByte;
			ucMBCRC16(temp.HiByte, &pMbus->stHoldInfo.tbcrc);
			temp.LoByte = pMbus->pData->points&0x00FF;
			pMbus->sci->SCITXBUF = temp.LoByte;
			ucMBCRC16(temp.LoByte, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_BYTE_COUNTS;
			break;

		case _SEND_BYTE_COUNTS:
			pMbus->tick1msec = 0;
			pMbus->sci->SCITXBUF = pMbus->pData->bytes;
			ucMBCRC16(pMbus->pData->bytes, &pMbus->stHoldInfo.tbcrc);
			pMbus->evHoldReg = _SEND_ALL_OF_DATA;
			break;

		case _SEND_ALL_OF_DATA:
			if(pMbus->stHoldInfo.m_index < pMbus->pData->points) {
				pMbus->tick1msec = 0;
				temp.HiByte = (pMbus->pData->reg[pMbus->stHoldInfo.m_index]>>8)&0x00FF;
				pMbus->sci->SCITXBUF = temp.HiByte;
				ucMBCRC16(temp.HiByte, &pMbus->stHoldInfo.tbcrc);
				temp.LoByte = pMbus->pData->reg[pMbus->stHoldInfo.m_index]&0x00FF;
				pMbus->sci->SCITXBUF = temp.LoByte;
				pMbus->stHoldInfo.crc = ucMBCRC16(temp.LoByte, &pMbus->stHoldInfo.tbcrc);
				pMbus->stHoldInfo.m_index++;
			}
			else pMbus->evHoldReg = _SEND_CRC_OUT;
			break;

		case _SEND_CRC_OUT:
			pMbus->tick1msec = 0;
			pMbus->sci->SCITXBUF = (pMbus->stHoldInfo.crc>>0)&0x00FF;
			pMbus->sci->SCITXBUF = (pMbus->stHoldInfo.crc>>8)&0x00FF;
			pMbus->stHoldInfo.tbcrc = DEFAULT_CRC;
			pMbus->stHoldInfo.crc = 0x0000;
			pMbus->stHoldInfo.pRX = &pMbus->RxBuffer[0];
			pMbus->evHoldReg = _RECEIVE_ALL_OF_DATA;
			break;

		case _RECEIVE_ALL_OF_DATA:
			if(0 < pMbus->sci->SCIFFRX.bit.RXFFST){
				pMbus->tick1msec = 0;
				*pMbus->stHoldInfo.pRX = pMbus->sci->SCIRXBUF.all & 0x00FF;
				pMbus->stHoldInfo.crc = ucMBCRC16(*pMbus->stHoldInfo.pRX, &pMbus->stHoldInfo.tbcrc);
				pMbus->stHoldInfo.pRX++;
				if(0 == pMbus->stHoldInfo.crc) pMbus->evHoldReg = _PROCESS_ALL_OF_DATA;
			}
			return MBUS_WAIT;

		case _PROCESS_ALL_OF_DATA:
			pMbus->tick1msec = 0;
			pack = (HEADER_READ_HOLDINGREG *)&pMbus->RxBuffer[0];
			pMbus->evHoldReg = _SEND_SLAVEID_AND_FUNCID;
			if(0x0080&pack->Function) return MBUS_FAIL;
			else return MBUS_SUCCESS;

		default:
			break;
		}
	}
	return MBUS_WAIT;
}


int16 (*ModbusFunc[25])(SCI_MODBUS *pMbus) = {
		ReservedFunction,		//0x00
		ReservedFunction,		//0x01
		ReservedFunction,		//0x02
		readHoldingRegister,		//0x03		Read Holding Registers
		ReservedFunction,		//0x04
		ReservedFunction,		//0x05
		writeHoldingRegister,		//0x06		Preset Single Register
		ReservedFunction,		//0x07
		ReservedFunction,		//0x08
		ReservedFunction,		//0x09
		ReservedFunction,		//0x0A
		ReservedFunction,		//0x0B
		ReservedFunction,		//0x0C
		ReservedFunction,		//0x0D
		ReservedFunction,		//0x0E
		ReservedFunction,		//0x0F
		writeHoldingNRegister,		//0x10		 Preset Multiple Regs
		ReservedFunction,		//0x11
		ReservedFunction,		//0x12
		ReservedFunction,		//0x13
		ReservedFunction,		//0x14
		ReservedFunction,		//0x15
		ReservedFunction,		//0x16
		ReservedFunction,		//0x17
		ReservedFunction		//0x18

};

Uint16 DisplayRegister[8] = {'8', '8','8','8','8'};
Uint16 KeyRegister[2] = {0, 0};

SCI_MODBUS mbcomm = DEFAULT_DISPMODBUS;


