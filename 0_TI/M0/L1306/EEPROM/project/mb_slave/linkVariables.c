/*
 *  File Name: linkVariables.c
 *
 *  Created on: 2023/10/18
 *  Author: POWER-532A86
 */

#include "ModbusCommon.h"
#include "ModbusSlave.h"


void initRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	p->pReg->u16COMmonHEAder = 2423; 
	p->pReg->u16COMmonLENgth = 4; 
	p->pReg->u16MAChineINFOrmationOFFSet = 9000; 
	p->pReg->u16USERPARameterOFFSet = 1000; 
	p->pReg->u16ADVAncePARameterOFFSet = 1020; 
	p->pReg->u16COMmonCHEcksum = 13447; 
	p->pReg->f32VOLtageLOOpkp = 0.9; 
	p->pReg->f32VOLtageLOOpki = 0.0001; 
	p->pReg->f32VOLtageLOOpMAXimum = 1; 
	p->pReg->f32VOLtageLOOpMINimum = -1; 
	p->pReg->f32CURrentLOOpkp = 0.9; 
	p->pReg->f32CURrentLOOpki = 0.001; 
	p->pReg->f32CURrentLOOpMAXimum = 1; 
	p->pReg->f32CURrentLOOpMINimum = 0; 
	p->pReg->u16COUntryCODe = 886; 
	p->pReg->u16VENderid = 2423; 
	p->pReg->u32BUIldDATe = 2023011216; 
}

void readRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;

//	p->pReg->u16UARTemueeFSMSTAtus = EEPROMWrite.fsm;
//	p->pReg->u16U32Writesuccess = EEPROMWrite.u32writesuccess;
//	p->pReg->u16U8WRiteaction = EEPROMWrite.WriteAction;
}

void writeReg(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
//	switch(p->info.rwfrom) {
//	case _muUARTemueeFSMSTAtus :   EEPROMWrite.fsm = p->pReg->u16UARTemueeFSMSTAtus; break;
//	case _muU32Writesuccess :   EEPROMWrite.u32writesuccess = p->pReg->u16U32Writesuccess; break;
//	case _muU8WRiteaction :   EEPROMWrite.WriteAction = p->pReg->u16U8WRiteaction; break;
//	default:
//	    break;
//	}
}

void writeRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
//	EEPROMWrite.fsm = p->pReg->u16UARTemueeFSMSTAtus;
//	EEPROMWrite.u32writesuccess = p->pReg->u16U32Writesuccess;
//	EEPROMWrite.WriteAction = p->pReg->u16U8WRiteaction;
}

