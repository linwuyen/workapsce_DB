/*
 * linkVariables.c
 *
 *  Created on: 8/9/2023
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
	sDrv.f32RemoteVref = 0; 
	sCLA.f32IrefLimit = 0.931222562192364; 
	sCLA.sLoopV.f32Kp = 1; 
	sCLA.sLoopV.f32Ki = 0.04; 
	sCLA.sLoopV.f32Max = 1; 
	sCLA.sLoopV.f32Min = -1; 
	sCLA.sLoopI.f32Kp = 1; 
	sCLA.sLoopI.f32Ki = 0.6; 
	sCLA.sLoopI.f32Max = 1; 
	sCLA.sLoopI.f32Min = -1; 
	p->pReg->u16COUNtryCODe = 886; 
	p->pReg->u16VENderID = 2423; 
	p->pReg->u32BUILdDATe = 2023011216; 
}

void readRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	p->pReg->u16CLACONtrolSTATus = sCLA.fgStatus; 
	p->pReg->u16CLAERRorSTATus = sCLA.fgError; 
	p->pReg->u16ENABlePWM = sDrv.u16EnablePwm; 
	p->pReg->s16VOLtageREFerence = MB_SFtoQ15(sDrv.f32RemoteVref); 
	p->pReg->s16CONstantCURrentREFerence = MB_SFtoQ15(sCLA.f32IrefLimit); 
	p->pReg->s16MEASureVOLtageOUTput = MB_SFtoQ15(sDrv.f32VO); 
	p->pReg->s16MEASureVOLtageINPut = MB_SFtoQ15(sDrv.f32VIN); 
	p->pReg->s16MEASureCURrentOUTput = MB_SFtoQ15(sDrv.f32IO); 
	p->pReg->s16MEASureINDuctorCURrent = MB_SFtoQ15(sDrv.f32IL); 
	p->pReg->s16MEASureELECtronicLOADCURrent = MB_SFtoQ15(sDrv.f32IEL); 
	p->pReg->f32VOLtageLOOPKP = sCLA.sLoopV.f32Kp; 
	p->pReg->f32VOLtageLOOPKI = sCLA.sLoopV.f32Ki; 
	p->pReg->f32VOLtageLOOPMAXimum = sCLA.sLoopV.f32Max; 
	p->pReg->f32VOLtageLOOPMINimum = sCLA.sLoopV.f32Min; 
	p->pReg->f32CURrentLOOPKP = sCLA.sLoopI.f32Kp; 
	p->pReg->f32CURrentLOOPKI = sCLA.sLoopI.f32Ki; 
	p->pReg->f32CURrentLOOPMAXimum = sCLA.sLoopI.f32Max; 
	p->pReg->f32CURrentLOOPMINimum = sCLA.sLoopI.f32Min; 
}

void writeReg(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	switch(p->info.rwfrom) {
	case _muCLACONtrolSTATus :   sCLA.fgStatus = p->pReg->u16CLACONtrolSTATus; break;
	case _muCLAERRorSTATus :   sCLA.fgError = p->pReg->u16CLAERRorSTATus; break;
	case _muENABlePWM :   sDrv.u16EnablePwm = p->pReg->u16ENABlePWM; break;
	case _muVOLtageREFerence :   sDrv.f32RemoteVref = MB_Q15toSF(p->pReg->s16VOLtageREFerence); break;
	case _muCONstantCURrentREFerence :   sCLA.f32IrefLimit = MB_Q15toSF(p->pReg->s16CONstantCURrentREFerence); break;
	case _muMEASureVOLtageOUTput :   sDrv.f32VO = MB_Q15toSF(p->pReg->s16MEASureVOLtageOUTput); break;
	case _muMEASureVOLtageINPut :   sDrv.f32VIN = MB_Q15toSF(p->pReg->s16MEASureVOLtageINPut); break;
	case _muMEASureCURrentOUTput :   sDrv.f32IO = MB_Q15toSF(p->pReg->s16MEASureCURrentOUTput); break;
	case _muMEASureINDuctorCURrent :   sDrv.f32IL = MB_Q15toSF(p->pReg->s16MEASureINDuctorCURrent); break;
	case _muMEASureELECtronicLOADCURrent :   sDrv.f32IEL = MB_Q15toSF(p->pReg->s16MEASureELECtronicLOADCURrent); break;
	case _muVOLtageLOOPKP0 :   sCLA.sLoopV.f32Kp = p->pReg->f32VOLtageLOOPKP; break;
	case _muVOLtageLOOPKI0 :   sCLA.sLoopV.f32Ki = p->pReg->f32VOLtageLOOPKI; break;
	case _muVOLtageLOOPMAXimum0 :   sCLA.sLoopV.f32Max = p->pReg->f32VOLtageLOOPMAXimum; break;
	case _muVOLtageLOOPMINimum0 :   sCLA.sLoopV.f32Min = p->pReg->f32VOLtageLOOPMINimum; break;
	case _muCURrentLOOPKP0 :   sCLA.sLoopI.f32Kp = p->pReg->f32CURrentLOOPKP; break;
	case _muCURrentLOOPKI0 :   sCLA.sLoopI.f32Ki = p->pReg->f32CURrentLOOPKI; break;
	case _muCURrentLOOPMAXimum0 :   sCLA.sLoopI.f32Max = p->pReg->f32CURrentLOOPMAXimum; break;
	case _muCURrentLOOPMINimum0 :   sCLA.sLoopI.f32Min = p->pReg->f32CURrentLOOPMINimum; break;
	default:
	    break;
	}
}

void writeRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	sCLA.fgStatus = p->pReg->u16CLACONtrolSTATus; 
	sCLA.fgError = p->pReg->u16CLAERRorSTATus; 
	sDrv.u16EnablePwm = p->pReg->u16ENABlePWM; 
	sDrv.f32RemoteVref = MB_Q15toSF(p->pReg->s16VOLtageREFerence); 
	sCLA.f32IrefLimit = MB_Q15toSF(p->pReg->s16CONstantCURrentREFerence); 
	sDrv.f32VO = MB_Q15toSF(p->pReg->s16MEASureVOLtageOUTput); 
	sDrv.f32VIN = MB_Q15toSF(p->pReg->s16MEASureVOLtageINPut); 
	sDrv.f32IO = MB_Q15toSF(p->pReg->s16MEASureCURrentOUTput); 
	sDrv.f32IL = MB_Q15toSF(p->pReg->s16MEASureINDuctorCURrent); 
	sDrv.f32IEL = MB_Q15toSF(p->pReg->s16MEASureELECtronicLOADCURrent); 
	sCLA.sLoopV.f32Kp = p->pReg->f32VOLtageLOOPKP; 
	sCLA.sLoopV.f32Ki = p->pReg->f32VOLtageLOOPKI; 
	sCLA.sLoopV.f32Max = p->pReg->f32VOLtageLOOPMAXimum; 
	sCLA.sLoopV.f32Min = p->pReg->f32VOLtageLOOPMINimum; 
	sCLA.sLoopI.f32Kp = p->pReg->f32CURrentLOOPKP; 
	sCLA.sLoopI.f32Ki = p->pReg->f32CURrentLOOPKI; 
	sCLA.sLoopI.f32Max = p->pReg->f32CURrentLOOPMAXimum; 
	sCLA.sLoopI.f32Min = p->pReg->f32CURrentLOOPMINimum; 
}

