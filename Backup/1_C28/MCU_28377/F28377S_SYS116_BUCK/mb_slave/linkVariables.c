/*
 *  File Name: linkVariables.c
 *
 *  Created on: 2023/8/31
 *  Author: MASON
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
	sDrv.f32RemoteVref = 0; 
	sCLA.f32IrefLimitP = 0.28; 
	sCLA.f32IrefLimitN = -0.28; 
	sCLA.f32Ocp = 0.4; 
	sCLA.f32ILOcp = 0.4; 
	sCLA.f32ELOcp = 0.05; 
	sCLA.f32Ovp = 0.5; 
	sDrv.u16DacCh = 1; 
	sDrv.f32RemoteIELref = 0; 
	sCLA.f32ELVon = 8.33333333333333E-02; 
	sDrv.u16EnableEL = 0; 
	sDrv.u16ELVonEnable = 1; 
	p->pReg->u32FIRmwareBUIldDATe = 2023083110; 
	sCLA.sLoopV.f32Kp = 2; 
	sCLA.sLoopV.f32Ki = 0.02; 
	sCLA.sLoopV.f32Max = 1; 
	sCLA.sLoopV.f32Min = -1; 
	sCLA.sLoopI.f32Kp = 1; 
	sCLA.sLoopI.f32Ki = 0.1; 
	sCLA.sLoopI.f32Max = 1; 
	sCLA.sLoopI.f32Min = -1; 
	p->pReg->u16COUntryCODe = 886; 
	p->pReg->u16VENderid = 2423; 
	p->pReg->u32BUIldDATe = 2023083110; 
}

void readRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	p->pReg->u16CLACONtrolSTAtus = sCLA.fgStatus; 
	p->pReg->u16CLAERROrSTAtus = sCLA.fgError; 
	p->pReg->u16ENABlePWM = sDrv.u16EnablePwm; 
	p->pReg->s16VOLtageREFerence = MB_SFtoQ15(sDrv.f32RemoteVref); 
	p->pReg->s16POSitiveCURrentLIMit = MB_SFtoQ15(sCLA.f32IrefLimitP); 
	p->pReg->s16NEGativeCURrentLIMit = MB_SFtoQ15(sCLA.f32IrefLimitN); 
	p->pReg->s16OVERCURrentPROtection = MB_SFtoQ15(sCLA.f32Ocp); 
	p->pReg->s16OVERilCURrentPROtection = MB_SFtoQ15(sCLA.f32ILOcp); 
	p->pReg->s16OVERelCURrentPROtection = MB_SFtoQ15(sCLA.f32ELOcp); 
	p->pReg->s16OVERVOLtagePROtection = MB_SFtoQ15(sCLA.f32Ovp); 
	p->pReg->s16MEAsureVOLtageOUTPut = MB_SFtoQ15(sDrv.f32VO); 
	p->pReg->s16MEAsureVOLtageINPUt = MB_SFtoQ15(sDrv.f32VIN); 
	p->pReg->s16MEAsureCURrentOUTPut = MB_SFtoQ15(sDrv.f32IO); 
	p->pReg->s16MEAsureINDUctorCURrent = MB_SFtoQ15(sDrv.f32IL); 
	p->pReg->s16MEAsureELECtronicLOAdCURrent = MB_SFtoQ15(sDrv.f32IEL); 
	p->pReg->u16DACCHAnnel = sDrv.u16DacCh; 
	p->pReg->s16DACchDATaREFerence = MB_SFtoQ15(sDrv.f32RemoteIELref); 
	p->pReg->s16ELOAdVONREFerence = MB_SFtoQ15(sCLA.f32ELVon); 
	p->pReg->u32DACSPIFORmat = sDrv.sDAC.SPI_FORMAT.SpiData; 
	p->pReg->u16ENABleel = sDrv.u16EnableEL; 
	p->pReg->u16ELOAdERROrSTAtus = sCLA.ELfgError; 
	p->pReg->u16ELOAdLDNoSTAtus = sCLA.ELfgStatus; 
	p->pReg->u16ELOAdVONENABle = sDrv.u16ELVonEnable; 
	p->pReg->f32VOLtageLOOpkp = sCLA.sLoopV.f32Kp; 
	p->pReg->f32VOLtageLOOpki = sCLA.sLoopV.f32Ki; 
	p->pReg->f32VOLtageLOOpMAXimum = sCLA.sLoopV.f32Max; 
	p->pReg->f32VOLtageLOOpMINimum = sCLA.sLoopV.f32Min; 
	p->pReg->f32CURrentLOOpkp = sCLA.sLoopI.f32Kp; 
	p->pReg->f32CURrentLOOpki = sCLA.sLoopI.f32Ki; 
	p->pReg->f32CURrentLOOpMAXimum = sCLA.sLoopI.f32Max; 
	p->pReg->f32CURrentLOOpMINimum = sCLA.sLoopI.f32Min; 
	p->pReg->u32HEArtbeatC28 = sDrv.u32HeartBeat; 
	p->pReg->u32HEArtbeatCLA = sCLA.u32HeartBeat; 
}

void writeReg(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	switch(p->info.rwfrom) {
	case _muCLACONtrolSTAtus :   sCLA.fgStatus = p->pReg->u16CLACONtrolSTAtus; break;
	case _muCLAERROrSTAtus :   sCLA.fgError = p->pReg->u16CLAERROrSTAtus; break;
	case _muENABlePWM :   sDrv.u16EnablePwm = p->pReg->u16ENABlePWM; break;
	case _muVOLtageREFerence :   sDrv.f32RemoteVref = MB_Q15toSF(p->pReg->s16VOLtageREFerence); break;
	case _muPOSitiveCURrentLIMit :   sCLA.f32IrefLimitP = MB_Q15toSF(p->pReg->s16POSitiveCURrentLIMit); break;
	case _muNEGativeCURrentLIMit :   sCLA.f32IrefLimitN = MB_Q15toSF(p->pReg->s16NEGativeCURrentLIMit); break;
	case _muOVERCURrentPROtection :   sCLA.f32Ocp = MB_Q15toSF(p->pReg->s16OVERCURrentPROtection); break;
	case _muOVERilCURrentPROtection :   sCLA.f32ILOcp = MB_Q15toSF(p->pReg->s16OVERilCURrentPROtection); break;
	case _muOVERelCURrentPROtection :   sCLA.f32ELOcp = MB_Q15toSF(p->pReg->s16OVERelCURrentPROtection); break;
	case _muOVERVOLtagePROtection :   sCLA.f32Ovp = MB_Q15toSF(p->pReg->s16OVERVOLtagePROtection); break;
	case _muMEAsureVOLtageOUTPut :   sDrv.f32VO = MB_Q15toSF(p->pReg->s16MEAsureVOLtageOUTPut); break;
	case _muMEAsureVOLtageINPUt :   sDrv.f32VIN = MB_Q15toSF(p->pReg->s16MEAsureVOLtageINPUt); break;
	case _muMEAsureCURrentOUTPut :   sDrv.f32IO = MB_Q15toSF(p->pReg->s16MEAsureCURrentOUTPut); break;
	case _muMEAsureINDUctorCURrent :   sDrv.f32IL = MB_Q15toSF(p->pReg->s16MEAsureINDUctorCURrent); break;
	case _muMEAsureELECtronicLOAdCURrent :   sDrv.f32IEL = MB_Q15toSF(p->pReg->s16MEAsureELECtronicLOAdCURrent); break;
	case _muDACCHAnnel :   sDrv.u16DacCh = p->pReg->u16DACCHAnnel; break;
	case _muDACchDATaREFerence :   sDrv.f32RemoteIELref = MB_Q15toSF(p->pReg->s16DACchDATaREFerence); break;
	case _muELOAdVONREFerence :   sCLA.f32ELVon = MB_Q15toSF(p->pReg->s16ELOAdVONREFerence); break;
	case _muDACSPIFORmat0 :   sDrv.sDAC.SPI_FORMAT.SpiData = p->pReg->u32DACSPIFORmat; break;
	case _muENABleel :   sDrv.u16EnableEL = p->pReg->u16ENABleel; break;
	case _muELOAdERROrSTAtus :   sCLA.ELfgError = p->pReg->u16ELOAdERROrSTAtus; break;
	case _muELOAdLDNoSTAtus :   sCLA.ELfgStatus = p->pReg->u16ELOAdLDNoSTAtus; break;
	case _muELOAdVONENABle :   sDrv.u16ELVonEnable = p->pReg->u16ELOAdVONENABle; break;
	case _muVOLtageLOOpkp0 :   sCLA.sLoopV.f32Kp = p->pReg->f32VOLtageLOOpkp; break;
	case _muVOLtageLOOpki0 :   sCLA.sLoopV.f32Ki = p->pReg->f32VOLtageLOOpki; break;
	case _muVOLtageLOOpMAXimum0 :   sCLA.sLoopV.f32Max = p->pReg->f32VOLtageLOOpMAXimum; break;
	case _muVOLtageLOOpMINimum0 :   sCLA.sLoopV.f32Min = p->pReg->f32VOLtageLOOpMINimum; break;
	case _muCURrentLOOpkp0 :   sCLA.sLoopI.f32Kp = p->pReg->f32CURrentLOOpkp; break;
	case _muCURrentLOOpki0 :   sCLA.sLoopI.f32Ki = p->pReg->f32CURrentLOOpki; break;
	case _muCURrentLOOpMAXimum0 :   sCLA.sLoopI.f32Max = p->pReg->f32CURrentLOOpMAXimum; break;
	case _muCURrentLOOpMINimum0 :   sCLA.sLoopI.f32Min = p->pReg->f32CURrentLOOpMINimum; break;
	case _muHEArtbeatC280 :   sDrv.u32HeartBeat = p->pReg->u32HEArtbeatC28; break;
	case _muHEArtbeatCLA0 :   sCLA.u32HeartBeat = p->pReg->u32HEArtbeatCLA; break;
	default:
	    break;
	}
}

void writeRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	sCLA.fgStatus = p->pReg->u16CLACONtrolSTAtus; 
	sCLA.fgError = p->pReg->u16CLAERROrSTAtus; 
	sDrv.u16EnablePwm = p->pReg->u16ENABlePWM; 
	sDrv.f32RemoteVref = MB_Q15toSF(p->pReg->s16VOLtageREFerence); 
	sCLA.f32IrefLimitP = MB_Q15toSF(p->pReg->s16POSitiveCURrentLIMit); 
	sCLA.f32IrefLimitN = MB_Q15toSF(p->pReg->s16NEGativeCURrentLIMit); 
	sCLA.f32Ocp = MB_Q15toSF(p->pReg->s16OVERCURrentPROtection); 
	sCLA.f32ILOcp = MB_Q15toSF(p->pReg->s16OVERilCURrentPROtection); 
	sCLA.f32ELOcp = MB_Q15toSF(p->pReg->s16OVERelCURrentPROtection); 
	sCLA.f32Ovp = MB_Q15toSF(p->pReg->s16OVERVOLtagePROtection); 
	sDrv.f32VO = MB_Q15toSF(p->pReg->s16MEAsureVOLtageOUTPut); 
	sDrv.f32VIN = MB_Q15toSF(p->pReg->s16MEAsureVOLtageINPUt); 
	sDrv.f32IO = MB_Q15toSF(p->pReg->s16MEAsureCURrentOUTPut); 
	sDrv.f32IL = MB_Q15toSF(p->pReg->s16MEAsureINDUctorCURrent); 
	sDrv.f32IEL = MB_Q15toSF(p->pReg->s16MEAsureELECtronicLOAdCURrent); 
	sDrv.u16DacCh = p->pReg->u16DACCHAnnel; 
	sDrv.f32RemoteIELref = MB_Q15toSF(p->pReg->s16DACchDATaREFerence); 
	sCLA.f32ELVon = MB_Q15toSF(p->pReg->s16ELOAdVONREFerence); 
	sDrv.sDAC.SPI_FORMAT.SpiData = p->pReg->u32DACSPIFORmat; 
	sDrv.u16EnableEL = p->pReg->u16ENABleel; 
	sCLA.ELfgError = p->pReg->u16ELOAdERROrSTAtus; 
	sCLA.ELfgStatus = p->pReg->u16ELOAdLDNoSTAtus; 
	sDrv.u16ELVonEnable = p->pReg->u16ELOAdVONENABle; 
	sCLA.sLoopV.f32Kp = p->pReg->f32VOLtageLOOpkp; 
	sCLA.sLoopV.f32Ki = p->pReg->f32VOLtageLOOpki; 
	sCLA.sLoopV.f32Max = p->pReg->f32VOLtageLOOpMAXimum; 
	sCLA.sLoopV.f32Min = p->pReg->f32VOLtageLOOpMINimum; 
	sCLA.sLoopI.f32Kp = p->pReg->f32CURrentLOOpkp; 
	sCLA.sLoopI.f32Ki = p->pReg->f32CURrentLOOpki; 
	sCLA.sLoopI.f32Max = p->pReg->f32CURrentLOOpMAXimum; 
	sCLA.sLoopI.f32Min = p->pReg->f32CURrentLOOpMINimum; 
	sDrv.u32HeartBeat = p->pReg->u32HEArtbeatC28; 
	sCLA.u32HeartBeat = p->pReg->u32HEArtbeatCLA; 
}

