/*
 *  File Name: linkVariables.c
 *
 *  Created on: 8/27/2025
 *  Author: POWER-532A86
 */

#include "ModbusCommon.h"
#include "ModbusSlave.h"


void initRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	p->pReg->u16COMmonHEAder = 2423; 
	p->pReg->u16COMmonLENgth = 4; 
	p->pReg->u16MAChineINFOrmationOFFSet = 6; 
	p->pReg->u16USERPARameterOFFSet = 16; 
	p->pReg->u16ADVAncePARameterOFFSet = 36; 
	p->pReg->u16COMmonCHEcksum = 2485; 
	p->pReg->u16MAChineLENgth = 10; 
	p->pReg->u16COUntryCODe = 886; 
	p->pReg->u16VENderid = 2423; 
	p->pReg->u16PROductid = 1; 
	p->pReg->u16PARtid = 3; 
	p->pReg->u16MODuleid = 0; 
	sDrv.u32SN.all = 0; 
	p->pReg->u16VERsion = FW_VERSION; 
	p->pReg->u16BUIldDATe = FW_BUILDDATE; 
	p->pReg->u16USERPARameterLENgth = 20; 
	sDrv.f32RemoteVref = 0; 
	p->pReg->u16ADVAncePARameterLENgth = 48; 
	p->pReg->u16ADVAncePASsword = 0; 
	sDrv.u16RampUp100usec = T_SLEWRATE_UP; 
	sDrv.u16RampDown100usec = T_SLEWRATE_DOWN; 
}

void readRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	p->pReg->u32SERialNUMber = sDrv.u32SN.all; 
	p->pReg->u16MAInCONtrolSTAtus = sDrv.fgStatus; 
	p->pReg->u16MAInERROrSTAtus = sDrv.fgError; 
	p->pReg->u16MAInERROrMARk = sDrv.fgErrorMark.all; 
	p->pReg->u16MAInERROrRESult = sDrv.fgErrorResult.all; 
	p->pReg->u16m0gdERROrSTAtus = sIbSlave.sMirror.u16GDstat.all; 
	p->pReg->u16m0ADCERROrSTAtus = sIbSlave.sMirror.u16ADCstat.all; 
	p->pReg->u16ENABlePWM = sDrv.regPwm.u16Remote.all; 
	p->pReg->s16VOLtageREFerence = MB_SFtoQ15(sDrv.f32RemoteVref); 
	p->pReg->s16POSitiveCURrentLIMit = MB_SFtoQ15(sDrv.f32IrefLimitP); 
	p->pReg->s16NEGativeCURrentLIMit = MB_SFtoQ15(sDrv.f32IrefLimitN); 
	p->pReg->s16OVERSOUrceCURrentPROtection = MB_SFtoQ15(sDrv.f32RemoteHwSourceOcp); 
	p->pReg->s16OVERSINkCURrentPROtection = MB_SFtoQ15(sDrv.f32RemoteHwSinkOcp); 
	p->pReg->s16OVERVOLtageOUTPutPROtection = MB_SFtoQ15(sDrv.f32RemoteHwOvp); 
	p->pReg->s16OVERVOLtageINPUtPROtection = MB_SFtoQ15(sDrv.f32RemoteHwVinOvp); 
	p->pReg->s16MEAsureVOLtageOUTPut = MB_SFtoQ15(sDrv.sVO.sLPF.f32Out); 
	p->pReg->s16MEAsureVOLtageINPUt = MB_SFtoQ15(sDrv.sVIN.sLPF.f32Out); 
	p->pReg->s16MEAsureCURrentOUTPut = MB_SFtoQ15(sDrv.sIO.sLPF.f32Out); 
	p->pReg->s16MEAsureINDUctorCURrent = MB_SFtoQ15(sDrv.sIL.sLPF.f32Out); 
	p->pReg->f32VOLtageLOOpkp = sDrv.sLoopV.f32Kp; 
	p->pReg->f32VOLtageLOOpki = sDrv.sLoopV.f32Ki; 
	p->pReg->f32CURrentLOOpkp = sDrv.sLoopI.f32Kp; 
	p->pReg->f32CURrentLOOpki = sDrv.sLoopI.f32Ki; 
	p->pReg->f32OPENLOOpGAIn = sDrv.f32OpenGain; 
	p->pReg->u32HEArtbeatC28 = sDrv.u32HeartBeat; 
	p->pReg->u32HEArtbeatCLA = sCLA.u32HeartBeat; 
	p->pReg->u32rdMODe = sDrv.u32RdMode; 
	p->pReg->u16HARdwareSLEwrateup = sDrv.u16RampUp100usec; 
	p->pReg->u16HARdwareSLEwrateDOWn = sDrv.u16RampDown100usec; 
	p->pReg->u16CALibrationCOMmand = sRmtCal.u16Command.u16All; 
	p->pReg->u16CALibrationACK = sRmtCal.u16CmdAck.u16All; 
	p->pReg->f32CALibrationDATa = sRmtCal.f32Data; 
	p->pReg->f32CALibrationRETurn = sRmtCal.f32Return; 
	p->pReg->u16RECordWORkingSTAtus = sDrv.sRecord.fgStatus; 
	p->pReg->u16RECordERROrSTAtus = sDrv.sRecord.fgError; 
	p->pReg->s16BROwninVOLtage = MB_SFtoQ15(sDrv.f32BrownInPU); 
	p->pReg->s16BROwnOUTVOLtage = MB_SFtoQ15(sDrv.f32BrownOutPU); 
	p->pReg->f32GAInENABle = sDrv.f32GainEnable; 
	p->pReg->f32viGAIn = sDrv.f32VIgain; 
	p->pReg->u32ENABleCANbusMASter = sDrv.regMaster.all; 
	p->pReg->f32C28ISRCOStTIMe = sDrv.tpIsrCost.f32ElapsedUsec; 
	p->pReg->f32C28ISRMAXCOStTIMe = sDrv.tpIsrCost.f32MaxElapsedUsec; 
	p->pReg->s16BUCkVOUtMONitor1 = MB_SFtoQ15(sCbSlave.f32BuckVout[0]); 
	p->pReg->s16BUCkVOUtMONitor2 = MB_SFtoQ15(sCbSlave.f32BuckVout[1]); 
	p->pReg->s16BUCkVOUtMONitor3 = MB_SFtoQ15(sCbSlave.f32BuckVout[2]); 
	p->pReg->s16BUCkIOUTMONitor1 = MB_SFtoQ15(sCbSlave.f32BuckIout[0]); 
	p->pReg->s16BUCkIOUTMONitor2 = MB_SFtoQ15(sCbSlave.f32BuckIout[1]); 
	p->pReg->s16BUCkIOUTMONitor3 = MB_SFtoQ15(sCbSlave.f32BuckIout[2]); 
	p->pReg->u16onDELayMSEc = sDrv.sDelayCtrl.u16OnDelayMs; 
	p->pReg->u16OFFDELayMSEc = sDrv.sDelayCtrl.u16OffDelayMs; 
}

void writeReg(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	switch(p->info.rwfrom) {
	case _muSERialNUMber0 :   sDrv.u32SN.all = p->pReg->u32SERialNUMber; break;
	case _muMAInCONtrolSTAtus :   sDrv.fgStatus = p->pReg->u16MAInCONtrolSTAtus; break;
	case _muMAInERROrSTAtus :   sDrv.fgError = p->pReg->u16MAInERROrSTAtus; break;
	case _muMAInERROrMARk :   sDrv.fgErrorMark.all = p->pReg->u16MAInERROrMARk; break;
	case _muMAInERROrRESult :   sDrv.fgErrorResult.all = p->pReg->u16MAInERROrRESult; break;
	case _mum0gdERROrSTAtus :   sIbSlave.sMirror.u16GDstat.all = p->pReg->u16m0gdERROrSTAtus; break;
	case _mum0ADCERROrSTAtus :   sIbSlave.sMirror.u16ADCstat.all = p->pReg->u16m0ADCERROrSTAtus; break;
	case _muENABlePWM :   sDrv.regPwm.u16Remote.all = p->pReg->u16ENABlePWM; break;
	case _muVOLtageREFerence :   sDrv.f32RemoteVref = MB_Q15toSF(p->pReg->s16VOLtageREFerence); break;
	case _muPOSitiveCURrentLIMit :   sDrv.f32IrefLimitP = MB_Q15toSF(p->pReg->s16POSitiveCURrentLIMit); break;
	case _muNEGativeCURrentLIMit :   sDrv.f32IrefLimitN = MB_Q15toSF(p->pReg->s16NEGativeCURrentLIMit); break;
	case _muOVERSOUrceCURrentPROtection :   sDrv.f32RemoteHwSourceOcp = MB_Q15toSF(p->pReg->s16OVERSOUrceCURrentPROtection); break;
	case _muOVERSINkCURrentPROtection :   sDrv.f32RemoteHwSinkOcp = MB_Q15toSF(p->pReg->s16OVERSINkCURrentPROtection); break;
	case _muOVERVOLtageOUTPutPROtection :   sDrv.f32RemoteHwOvp = MB_Q15toSF(p->pReg->s16OVERVOLtageOUTPutPROtection); break;
	case _muOVERVOLtageINPUtPROtection :   sDrv.f32RemoteHwVinOvp = MB_Q15toSF(p->pReg->s16OVERVOLtageINPUtPROtection); break;
	case _muMEAsureVOLtageOUTPut :   sDrv.sVO.sLPF.f32Out = MB_Q15toSF(p->pReg->s16MEAsureVOLtageOUTPut); break;
	case _muMEAsureVOLtageINPUt :   sDrv.sVIN.sLPF.f32Out = MB_Q15toSF(p->pReg->s16MEAsureVOLtageINPUt); break;
	case _muMEAsureCURrentOUTPut :   sDrv.sIO.sLPF.f32Out = MB_Q15toSF(p->pReg->s16MEAsureCURrentOUTPut); break;
	case _muMEAsureINDUctorCURrent :   sDrv.sIL.sLPF.f32Out = MB_Q15toSF(p->pReg->s16MEAsureINDUctorCURrent); break;
	case _muVOLtageLOOpkp0 :   sDrv.sLoopV.f32Kp = p->pReg->f32VOLtageLOOpkp; break;
	case _muVOLtageLOOpki0 :   sDrv.sLoopV.f32Ki = p->pReg->f32VOLtageLOOpki; break;
	case _muCURrentLOOpkp0 :   sDrv.sLoopI.f32Kp = p->pReg->f32CURrentLOOpkp; break;
	case _muCURrentLOOpki0 :   sDrv.sLoopI.f32Ki = p->pReg->f32CURrentLOOpki; break;
	case _muOPENLOOpGAIn0 :   sDrv.f32OpenGain = p->pReg->f32OPENLOOpGAIn; break;
	case _muHEArtbeatC280 :   sDrv.u32HeartBeat = p->pReg->u32HEArtbeatC28; break;
	case _muHEArtbeatCLA0 :   sCLA.u32HeartBeat = p->pReg->u32HEArtbeatCLA; break;
	case _murdMODe0 :   sDrv.u32RdMode = p->pReg->u32rdMODe; break;
	case _muHARdwareSLEwrateup :   sDrv.u16RampUp100usec = p->pReg->u16HARdwareSLEwrateup; break;
	case _muHARdwareSLEwrateDOWn :   sDrv.u16RampDown100usec = p->pReg->u16HARdwareSLEwrateDOWn; break;
	case _muCALibrationCOMmand :   sRmtCal.u16Command.u16All = p->pReg->u16CALibrationCOMmand; break;
	case _muCALibrationACK :   sRmtCal.u16CmdAck.u16All = p->pReg->u16CALibrationACK; break;
	case _muCALibrationDATa0 :   sRmtCal.f32Data = p->pReg->f32CALibrationDATa; break;
	case _muCALibrationRETurn0 :   sRmtCal.f32Return = p->pReg->f32CALibrationRETurn; break;
	case _muRECordWORkingSTAtus :   sDrv.sRecord.fgStatus = p->pReg->u16RECordWORkingSTAtus; break;
	case _muRECordERROrSTAtus :   sDrv.sRecord.fgError = p->pReg->u16RECordERROrSTAtus; break;
	case _muBROwninVOLtage :   sDrv.f32BrownInPU = MB_Q15toSF(p->pReg->s16BROwninVOLtage); break;
	case _muBROwnOUTVOLtage :   sDrv.f32BrownOutPU = MB_Q15toSF(p->pReg->s16BROwnOUTVOLtage); break;
	case _muGAInENABle0 :   sDrv.f32GainEnable = p->pReg->f32GAInENABle; break;
	case _muviGAIn0 :   sDrv.f32VIgain = p->pReg->f32viGAIn; break;
	case _muENABleCANbusMASter0 :   sDrv.regMaster.all = p->pReg->u32ENABleCANbusMASter; break;
	case _muC28ISRCOStTIMe0 :   sDrv.tpIsrCost.f32ElapsedUsec = p->pReg->f32C28ISRCOStTIMe; break;
	case _muC28ISRMAXCOStTIMe0 :   sDrv.tpIsrCost.f32MaxElapsedUsec = p->pReg->f32C28ISRMAXCOStTIMe; break;
	case _muBUCkVOUtMONitor1 :   sCbSlave.f32BuckVout[0] = MB_Q15toSF(p->pReg->s16BUCkVOUtMONitor1); break;
	case _muBUCkVOUtMONitor2 :   sCbSlave.f32BuckVout[1] = MB_Q15toSF(p->pReg->s16BUCkVOUtMONitor2); break;
	case _muBUCkVOUtMONitor3 :   sCbSlave.f32BuckVout[2] = MB_Q15toSF(p->pReg->s16BUCkVOUtMONitor3); break;
	case _muBUCkIOUTMONitor1 :   sCbSlave.f32BuckIout[0] = MB_Q15toSF(p->pReg->s16BUCkIOUTMONitor1); break;
	case _muBUCkIOUTMONitor2 :   sCbSlave.f32BuckIout[1] = MB_Q15toSF(p->pReg->s16BUCkIOUTMONitor2); break;
	case _muBUCkIOUTMONitor3 :   sCbSlave.f32BuckIout[2] = MB_Q15toSF(p->pReg->s16BUCkIOUTMONitor3); break;
	case _muonDELayMSEc :   sDrv.sDelayCtrl.u16OnDelayMs = p->pReg->u16onDELayMSEc; break;
	case _muOFFDELayMSEc :   sDrv.sDelayCtrl.u16OffDelayMs = p->pReg->u16OFFDELayMSEc; break;
	default:
	    break;
	}
}

void writeRegN(void *v){ 

	SCI_MODBUS *p = (SCI_MODBUS *) v;
	sDrv.u32SN.all = p->pReg->u32SERialNUMber; 
	sDrv.fgStatus = p->pReg->u16MAInCONtrolSTAtus; 
	sDrv.fgError = p->pReg->u16MAInERROrSTAtus; 
	sDrv.fgErrorMark.all = p->pReg->u16MAInERROrMARk; 
	sDrv.fgErrorResult.all = p->pReg->u16MAInERROrRESult; 
	sIbSlave.sMirror.u16GDstat.all = p->pReg->u16m0gdERROrSTAtus; 
	sIbSlave.sMirror.u16ADCstat.all = p->pReg->u16m0ADCERROrSTAtus; 
	sDrv.regPwm.u16Remote.all = p->pReg->u16ENABlePWM; 
	sDrv.f32RemoteVref = MB_Q15toSF(p->pReg->s16VOLtageREFerence); 
	sDrv.f32IrefLimitP = MB_Q15toSF(p->pReg->s16POSitiveCURrentLIMit); 
	sDrv.f32IrefLimitN = MB_Q15toSF(p->pReg->s16NEGativeCURrentLIMit); 
	sDrv.f32RemoteHwSourceOcp = MB_Q15toSF(p->pReg->s16OVERSOUrceCURrentPROtection); 
	sDrv.f32RemoteHwSinkOcp = MB_Q15toSF(p->pReg->s16OVERSINkCURrentPROtection); 
	sDrv.f32RemoteHwOvp = MB_Q15toSF(p->pReg->s16OVERVOLtageOUTPutPROtection); 
	sDrv.f32RemoteHwVinOvp = MB_Q15toSF(p->pReg->s16OVERVOLtageINPUtPROtection); 
	sDrv.sVO.sLPF.f32Out = MB_Q15toSF(p->pReg->s16MEAsureVOLtageOUTPut); 
	sDrv.sVIN.sLPF.f32Out = MB_Q15toSF(p->pReg->s16MEAsureVOLtageINPUt); 
	sDrv.sIO.sLPF.f32Out = MB_Q15toSF(p->pReg->s16MEAsureCURrentOUTPut); 
	sDrv.sIL.sLPF.f32Out = MB_Q15toSF(p->pReg->s16MEAsureINDUctorCURrent); 
	sDrv.sLoopV.f32Kp = p->pReg->f32VOLtageLOOpkp; 
	sDrv.sLoopV.f32Ki = p->pReg->f32VOLtageLOOpki; 
	sDrv.sLoopI.f32Kp = p->pReg->f32CURrentLOOpkp; 
	sDrv.sLoopI.f32Ki = p->pReg->f32CURrentLOOpki; 
	sDrv.f32OpenGain = p->pReg->f32OPENLOOpGAIn; 
	sDrv.u32HeartBeat = p->pReg->u32HEArtbeatC28; 
	sCLA.u32HeartBeat = p->pReg->u32HEArtbeatCLA; 
	sDrv.u32RdMode = p->pReg->u32rdMODe; 
	sDrv.u16RampUp100usec = p->pReg->u16HARdwareSLEwrateup; 
	sDrv.u16RampDown100usec = p->pReg->u16HARdwareSLEwrateDOWn; 
	sRmtCal.u16Command.u16All = p->pReg->u16CALibrationCOMmand; 
	sRmtCal.u16CmdAck.u16All = p->pReg->u16CALibrationACK; 
	sRmtCal.f32Data = p->pReg->f32CALibrationDATa; 
	sRmtCal.f32Return = p->pReg->f32CALibrationRETurn; 
	sDrv.sRecord.fgStatus = p->pReg->u16RECordWORkingSTAtus; 
	sDrv.sRecord.fgError = p->pReg->u16RECordERROrSTAtus; 
	sDrv.f32BrownInPU = MB_Q15toSF(p->pReg->s16BROwninVOLtage); 
	sDrv.f32BrownOutPU = MB_Q15toSF(p->pReg->s16BROwnOUTVOLtage); 
	sDrv.f32GainEnable = p->pReg->f32GAInENABle; 
	sDrv.f32VIgain = p->pReg->f32viGAIn; 
	sDrv.regMaster.all = p->pReg->u32ENABleCANbusMASter; 
	sDrv.tpIsrCost.f32ElapsedUsec = p->pReg->f32C28ISRCOStTIMe; 
	sDrv.tpIsrCost.f32MaxElapsedUsec = p->pReg->f32C28ISRMAXCOStTIMe; 
	sCbSlave.f32BuckVout[0] = MB_Q15toSF(p->pReg->s16BUCkVOUtMONitor1); 
	sCbSlave.f32BuckVout[1] = MB_Q15toSF(p->pReg->s16BUCkVOUtMONitor2); 
	sCbSlave.f32BuckVout[2] = MB_Q15toSF(p->pReg->s16BUCkVOUtMONitor3); 
	sCbSlave.f32BuckIout[0] = MB_Q15toSF(p->pReg->s16BUCkIOUTMONitor1); 
	sCbSlave.f32BuckIout[1] = MB_Q15toSF(p->pReg->s16BUCkIOUTMONitor2); 
	sCbSlave.f32BuckIout[2] = MB_Q15toSF(p->pReg->s16BUCkIOUTMONitor3); 
	sDrv.sDelayCtrl.u16OnDelayMs = p->pReg->u16onDELayMSEc; 
	sDrv.sDelayCtrl.u16OffDelayMs = p->pReg->u16OFFDELayMSEc; 
}

