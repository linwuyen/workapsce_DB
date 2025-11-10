/*
 *  File Name: linkVariables.c
 *
 *  Created on: 2023/10/13
 *  Author: POWER-532A86
 */

#include "ModbusCommon.h"
#include "ModbusSlave.h"


void initRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	p->pReg->s16VOLtageREFerence = MB_SFtoQ15(0); 
	p->pReg->u16CONstantCURrentREFerence = 7; 
}

void readRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	p->pReg->u32FLAshCOUnter = writesuccess; 
}

void writeReg(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	switch(p->info.rwfrom) {
	case _muFLAshCOUnter0 :   writesuccess = p->pReg->u32FLAshCOUnter; break;
	default:
	    break;
	}
}

void writeRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	writesuccess = p->pReg->u32FLAshCOUnter; 
}

