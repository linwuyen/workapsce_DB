/*
 *  File Name: linkVariables.c
 *
 *  Created on: 2023/5/8
 *  Author: POWER2-54FD92
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
	u16C2 = 0; 
	u16C3 = 0.532127178395637; 
	f32A1 = 0.9; 
	f32A1 = 0.0001; 
	f32A3 = 1; 
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
	p->pReg->u16CONtrolMODe = u16C1; 
	p->pReg->s16VOLtageREFerence = MB_SFtoQ15(u16C2); 
	p->pReg->s16CONstantCURrentREFerence = MB_SFtoQ15(u16C3); 
	p->pReg->s16MEAsureVOLtageOUTPut = MB_SFtoQ15(u16B1); 
	p->pReg->s16MEAsureINDUctorCURrent = MB_SFtoQ15(u16B2); 
	p->pReg->s16MEAsureELECtronicLOAdCURrent = MB_SFtoQ15(u16B3); 
	p->pReg->f32VOLtageLOOpkp = f32A1; 
	p->pReg->f32VOLtageLOOpki = f32A1; 
	p->pReg->f32VOLtageLOOpMAXimum = f32A3; 
}

void writeReg(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	switch(p->info.rwfrom) {
	case _muCONtrolMODe :   u16C1 = p->pReg->u16CONtrolMODe; break;
	case _muVOLtageREFerence :   u16C2 = MB_Q15toSF(p->pReg->s16VOLtageREFerence); break;
	case _muCONstantCURrentREFerence :   u16C3 = MB_Q15toSF(p->pReg->s16CONstantCURrentREFerence); break;
	case _muMEAsureVOLtageOUTPut :   u16B1 = MB_Q15toSF(p->pReg->s16MEAsureVOLtageOUTPut); break;
	case _muMEAsureINDUctorCURrent :   u16B2 = MB_Q15toSF(p->pReg->s16MEAsureINDUctorCURrent); break;
	case _muMEAsureELECtronicLOAdCURrent :   u16B3 = MB_Q15toSF(p->pReg->s16MEAsureELECtronicLOAdCURrent); break;
	case _muVOLtageLOOpkp0 :   f32A1 = p->pReg->f32VOLtageLOOpkp; break;
	case _muVOLtageLOOpki0 :   f32A1 = p->pReg->f32VOLtageLOOpki; break;
	case _muVOLtageLOOpMAXimum0 :   f32A3 = p->pReg->f32VOLtageLOOpMAXimum; break;
	default:
	    break;
	}
}

void writeRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	u16C1 = p->pReg->u16CONtrolMODe; 
	u16C2 = MB_Q15toSF(p->pReg->s16VOLtageREFerence); 
	u16C3 = MB_Q15toSF(p->pReg->s16CONstantCURrentREFerence); 
	u16B1 = MB_Q15toSF(p->pReg->s16MEAsureVOLtageOUTPut); 
	u16B2 = MB_Q15toSF(p->pReg->s16MEAsureINDUctorCURrent); 
	u16B3 = MB_Q15toSF(p->pReg->s16MEAsureELECtronicLOAdCURrent); 
	f32A1 = p->pReg->f32VOLtageLOOpkp; 
	f32A1 = p->pReg->f32VOLtageLOOpki; 
	f32A3 = p->pReg->f32VOLtageLOOpMAXimum; 
}

