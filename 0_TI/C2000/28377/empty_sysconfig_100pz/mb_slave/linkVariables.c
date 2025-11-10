/*
 * linkVariables.c
 *
 *  Created on: 2023/8/10
 */
#include "ModbusCommon.h"
#include "ModbusSlave.h"


void initRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	p->pReg->u16COMmonHEADer = 2423; 
	p->pReg->u16COMmonLENgth = 4; 
	p->pReg->u16MAChineINFormationOFFset = 9000; 
	p->pReg->u16USERPARameterOFFset = 1000; 
	p->pReg->u16ADVancePARameterOFFset = 1020; 
	p->pReg->u16COMmonCHECksum = 13447; 
	p->pReg->s16VOLtageREFerence = MB_SFtoQ15(0); 
//	p->pReg->s16CONstantCURrentREFerence = MB_SFtoQ15(7);
	p->pReg->f32VOLtageLOOPKP = 1; 
	p->pReg->f32VOLtageLOOPKI = 0.04; 
	p->pReg->f32VOLtageLOOPMAXimum = 1; 
	p->pReg->f32VOLtageLOOPMINimum = -1; 
	p->pReg->f32CURrentLOOPKP = 1; 
	p->pReg->f32CURrentLOOPKI = 0.6; 
	p->pReg->f32CURrentLOOPMAXimum = 1; 
	p->pReg->f32CURrentLOOPMINimum = -1; 
	p->pReg->u16COUNtryCODe = 886; 
	p->pReg->u16VENderID = 2423; 
	p->pReg->u32BUILdDATe = 2023011216; 
}

void readRegN(void *v){ 

//	SCI_MODBUS *p = (SCI_MODBUS *) v;
}

void writeReg(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	switch(p->info.rwfrom) {
	default:
	    break;
	}
}

void writeRegN(void *v){ 

//	SCI_MODBUS *p = (SCI_MODBUS *) v;
}

