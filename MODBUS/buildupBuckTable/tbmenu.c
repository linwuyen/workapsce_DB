/*
 *  File Name: tbmenu.c
 *
 *  Created on: 8/27/2025
 *  Author: POWER-532A86
 */

#include "ModbusSlave.h"
#include "mbcmd.h"


REG_MBUSDATA regMbusData;

int chkValidAddress(uint16_t addr) {
    switch(addr) {
	    case _muCOMmonHEAder:                                           
	    case _muCOMmonLENgth:                                           
	    case _muMAChineINFOrmationOFFSet:                               
	    case _muUSERPARameterOFFSet:                                    
	    case _muADVAncePARameterOFFSet:                                 
	    case _muCOMmonCHEcksum:                                         
	    case _muMAChineLENgth:                                          
	    case _muCOUntryCODe:                                            
	    case _muVENderid:                                               
	    case _muPROductid:                                              // 1: PBL
	    case _muPARtid:                                                 // 1: PFC, 2: DAB, 3:BUCK
	    case _muMODuleid:                                               
	    case _muSERialNUMber0:                                          
	    case _muSERialNUMber1:                                          
	    case _muVERsion:                                                
	    case _muBUIldDATe:                                              
	    case _muUSERPARameterLENgth:                                    
	    case _muMAInCONtrolSTAtus:                                      
	    case _muMAInERROrSTAtus:                                        
	    case _muMAInERROrMARk:                                          
	    case _muMAInERROrRESult:                                        
	    case _mum0gdERROrSTAtus:                                        
	    case _mum0ADCERROrSTAtus:                                       
	    case _muENABlePWM:                                              //Bit0: ON/OFF
	    case _muVOLtageREFerence:                                       
	    case _muPOSitiveCURrentLIMit:                                   
	    case _muNEGativeCURrentLIMit:                                   
	    case _muOVERSOUrceCURrentPROtection:                            
	    case _muOVERSINkCURrentPROtection:                              
	    case _muOVERVOLtageOUTPutPROtection:                            
	    case _muOVERVOLtageINPUtPROtection:                             
	    case _muMEAsureTEMperature:                                     
	    case _muMEAsureVOLtageOUTPut:                                   
	    case _muMEAsureVOLtageINPUt:                                    
	    case _muMEAsureCURrentOUTPut:                                   
	    case _muMEAsureINDUctorCURrent:                                 
	    case _muADVAncePARameterLENgth:                                 
	    case _muADVAncePASsword:                                        
	    case _muVOLtageLOOpkp0:                                         
	    case _muVOLtageLOOpkp1:                                         
	    case _muVOLtageLOOpki0:                                         
	    case _muVOLtageLOOpki1:                                         
	    case _muCURrentLOOpkp0:                                         
	    case _muCURrentLOOpkp1:                                         
	    case _muCURrentLOOpki0:                                         
	    case _muCURrentLOOpki1:                                         
	    case _muOPENLOOpGAIn0:                                          
	    case _muOPENLOOpGAIn1:                                          
	    case _muHEArtbeatC280:                                          
	    case _muHEArtbeatC281:                                          
	    case _muHEArtbeatCLA0:                                          
	    case _muHEArtbeatCLA1:                                          
	    case _murdMODe0:                                                // _FORCE_OUTPUT=1,   _OPEN_LOOP=2,  _CLOSE_LOOP=4
	    case _murdMODe1:                                                // _FORCE_OUTPUT=1,   _OPEN_LOOP=2,  _CLOSE_LOOP=4
	    case _muHARdwareSLEwrateup:                                     // x100usec
	    case _muHARdwareSLEwrateDOWn:                                   // x100usec
	    case _muCALibrationCOMmand:                                     // SaveFlash=0x4000, Vin=0x100, Vo=0x200, Io=0x400, IL=0x800
	    case _muCALibrationACK:                                         
	    case _muCALibrationDATa0:                                       
	    case _muCALibrationDATa1:                                       
	    case _muCALibrationRETurn0:                                     
	    case _muCALibrationRETurn1:                                     
	    case _muRECordWORkingSTAtus:                                    
	    case _muRECordERROrSTAtus:                                      
	    case _muBROwninVOLtage:                                         
	    case _muBROwnOUTVOLtage:                                        
	    case _muGAInENABle0:                                            
	    case _muGAInENABle1:                                            
	    case _muviGAIn0:                                                
	    case _muviGAIn1:                                                
	    case _muENABleCANbusMASter0:                                    
	    case _muENABleCANbusMASter1:                                    
	    case _muC28ISRCOStTIMe0:                                        
	    case _muC28ISRCOStTIMe1:                                        
	    case _muC28ISRMAXCOStTIMe0:                                     
	    case _muC28ISRMAXCOStTIMe1:                                     
	    case _muBUCkVOUtMONitor1:                                       
	    case _muBUCkVOUtMONitor2:                                       
	    case _muBUCkVOUtMONitor3:                                       
	    case _muBUCkIOUTMONitor1:                                       
	    case _muBUCkIOUTMONitor2:                                       
	    case _muBUCkIOUTMONitor3:                                       
	    case _muonDELayMSEc:                                            
	    case _muOFFDELayMSEc:                                           
        return 0;
    default:
        return MB_ERROR_ILLEGALADDR;
    }
}

uint16_t getModbusData(uint16_t addr) {
    switch(addr) {
	    case _muCOMmonHEAder: return regMbusData.u16MbusData[0];                            
	    case _muCOMmonLENgth: return regMbusData.u16MbusData[1];                            
	    case _muMAChineINFOrmationOFFSet: return regMbusData.u16MbusData[2];                            
	    case _muUSERPARameterOFFSet: return regMbusData.u16MbusData[3];                            
	    case _muADVAncePARameterOFFSet: return regMbusData.u16MbusData[4];                            
	    case _muCOMmonCHEcksum: return regMbusData.u16MbusData[5];                            
	    case _muMAChineLENgth: return regMbusData.u16MbusData[6];                            
	    case _muCOUntryCODe: return regMbusData.u16MbusData[7];                            
	    case _muVENderid: return regMbusData.u16MbusData[8];                            
	    case _muPROductid: return regMbusData.u16MbusData[9];                            // 1: PBL
	    case _muPARtid: return regMbusData.u16MbusData[10];                            // 1: PFC, 2: DAB, 3:BUCK
	    case _muMODuleid: return regMbusData.u16MbusData[11];                            
	    case _muSERialNUMber0: return regMbusData.u16MbusData[12];                            
	    case _muSERialNUMber1: return regMbusData.u16MbusData[13];                            
	    case _muVERsion: return regMbusData.u16MbusData[14];                            
	    case _muBUIldDATe: return regMbusData.u16MbusData[15];                            
	    case _muUSERPARameterLENgth: return regMbusData.u16MbusData[16];                            
	    case _muMAInCONtrolSTAtus: return regMbusData.u16MbusData[17];                            
	    case _muMAInERROrSTAtus: return regMbusData.u16MbusData[18];                            
	    case _muMAInERROrMARk: return regMbusData.u16MbusData[19];                            
	    case _muMAInERROrRESult: return regMbusData.u16MbusData[20];                            
	    case _mum0gdERROrSTAtus: return regMbusData.u16MbusData[21];                            
	    case _mum0ADCERROrSTAtus: return regMbusData.u16MbusData[22];                            
	    case _muENABlePWM: return regMbusData.u16MbusData[23];                            //Bit0: ON/OFF
	    case _muVOLtageREFerence: return regMbusData.u16MbusData[24];                            
	    case _muPOSitiveCURrentLIMit: return regMbusData.u16MbusData[25];                            
	    case _muNEGativeCURrentLIMit: return regMbusData.u16MbusData[26];                            
	    case _muOVERSOUrceCURrentPROtection: return regMbusData.u16MbusData[27];                            
	    case _muOVERSINkCURrentPROtection: return regMbusData.u16MbusData[28];                            
	    case _muOVERVOLtageOUTPutPROtection: return regMbusData.u16MbusData[29];                            
	    case _muOVERVOLtageINPUtPROtection: return regMbusData.u16MbusData[30];                            
	    case _muMEAsureTEMperature: return regMbusData.u16MbusData[31];                            
	    case _muMEAsureVOLtageOUTPut: return regMbusData.u16MbusData[32];                            
	    case _muMEAsureVOLtageINPUt: return regMbusData.u16MbusData[33];                            
	    case _muMEAsureCURrentOUTPut: return regMbusData.u16MbusData[34];                            
	    case _muMEAsureINDUctorCURrent: return regMbusData.u16MbusData[35];                            
	    case _muADVAncePARameterLENgth: return regMbusData.u16MbusData[36];                            
	    case _muADVAncePASsword: return regMbusData.u16MbusData[37];                            
	    case _muVOLtageLOOpkp0: return regMbusData.u16MbusData[38];                            
	    case _muVOLtageLOOpkp1: return regMbusData.u16MbusData[39];                            
	    case _muVOLtageLOOpki0: return regMbusData.u16MbusData[40];                            
	    case _muVOLtageLOOpki1: return regMbusData.u16MbusData[41];                            
	    case _muCURrentLOOpkp0: return regMbusData.u16MbusData[42];                            
	    case _muCURrentLOOpkp1: return regMbusData.u16MbusData[43];                            
	    case _muCURrentLOOpki0: return regMbusData.u16MbusData[44];                            
	    case _muCURrentLOOpki1: return regMbusData.u16MbusData[45];                            
	    case _muOPENLOOpGAIn0: return regMbusData.u16MbusData[46];                            
	    case _muOPENLOOpGAIn1: return regMbusData.u16MbusData[47];                            
	    case _muHEArtbeatC280: return regMbusData.u16MbusData[48];                            
	    case _muHEArtbeatC281: return regMbusData.u16MbusData[49];                            
	    case _muHEArtbeatCLA0: return regMbusData.u16MbusData[50];                            
	    case _muHEArtbeatCLA1: return regMbusData.u16MbusData[51];                            
	    case _murdMODe0: return regMbusData.u16MbusData[52];                            // _FORCE_OUTPUT=1,   _OPEN_LOOP=2,  _CLOSE_LOOP=4
	    case _murdMODe1: return regMbusData.u16MbusData[53];                            // _FORCE_OUTPUT=1,   _OPEN_LOOP=2,  _CLOSE_LOOP=4
	    case _muHARdwareSLEwrateup: return regMbusData.u16MbusData[54];                            // x100usec
	    case _muHARdwareSLEwrateDOWn: return regMbusData.u16MbusData[55];                            // x100usec
	    case _muCALibrationCOMmand: return regMbusData.u16MbusData[56];                            // SaveFlash=0x4000, Vin=0x100, Vo=0x200, Io=0x400, IL=0x800
	    case _muCALibrationACK: return regMbusData.u16MbusData[57];                            
	    case _muCALibrationDATa0: return regMbusData.u16MbusData[58];                            
	    case _muCALibrationDATa1: return regMbusData.u16MbusData[59];                            
	    case _muCALibrationRETurn0: return regMbusData.u16MbusData[60];                            
	    case _muCALibrationRETurn1: return regMbusData.u16MbusData[61];                            
	    case _muRECordWORkingSTAtus: return regMbusData.u16MbusData[62];                            
	    case _muRECordERROrSTAtus: return regMbusData.u16MbusData[63];                            
	    case _muBROwninVOLtage: return regMbusData.u16MbusData[64];                            
	    case _muBROwnOUTVOLtage: return regMbusData.u16MbusData[65];                            
	    case _muGAInENABle0: return regMbusData.u16MbusData[66];                            
	    case _muGAInENABle1: return regMbusData.u16MbusData[67];                            
	    case _muviGAIn0: return regMbusData.u16MbusData[68];                            
	    case _muviGAIn1: return regMbusData.u16MbusData[69];                            
	    case _muENABleCANbusMASter0: return regMbusData.u16MbusData[70];                            
	    case _muENABleCANbusMASter1: return regMbusData.u16MbusData[71];                            
	    case _muC28ISRCOStTIMe0: return regMbusData.u16MbusData[72];                            
	    case _muC28ISRCOStTIMe1: return regMbusData.u16MbusData[73];                            
	    case _muC28ISRMAXCOStTIMe0: return regMbusData.u16MbusData[74];                            
	    case _muC28ISRMAXCOStTIMe1: return regMbusData.u16MbusData[75];                            
	    case _muBUCkVOUtMONitor1: return regMbusData.u16MbusData[76];                            
	    case _muBUCkVOUtMONitor2: return regMbusData.u16MbusData[77];                            
	    case _muBUCkVOUtMONitor3: return regMbusData.u16MbusData[78];                            
	    case _muBUCkIOUTMONitor1: return regMbusData.u16MbusData[79];                            
	    case _muBUCkIOUTMONitor2: return regMbusData.u16MbusData[80];                            
	    case _muBUCkIOUTMONitor3: return regMbusData.u16MbusData[81];                            
	    case _muonDELayMSEc: return regMbusData.u16MbusData[82];                            
	    case _muOFFDELayMSEc: return regMbusData.u16MbusData[83];                            
    default:
        return 0xFFFF;
    }
}

uint16_t setModbusData(uint16_t addr, uint16_t data) {
    switch(addr) {
	    case _muCOMmonHEAder: regMbusData.u16MbusData[0] = data; break;                            
	    case _muCOMmonLENgth: regMbusData.u16MbusData[1] = data; break;                            
	    case _muMAChineINFOrmationOFFSet: regMbusData.u16MbusData[2] = data; break;                            
	    case _muUSERPARameterOFFSet: regMbusData.u16MbusData[3] = data; break;                            
	    case _muADVAncePARameterOFFSet: regMbusData.u16MbusData[4] = data; break;                            
	    case _muCOMmonCHEcksum: regMbusData.u16MbusData[5] = data; break;                            
	    case _muMAChineLENgth: regMbusData.u16MbusData[6] = data; break;                            
	    case _muCOUntryCODe: regMbusData.u16MbusData[7] = data; break;                            
	    case _muVENderid: regMbusData.u16MbusData[8] = data; break;                            
	    case _muPROductid: regMbusData.u16MbusData[9] = data; break;                            // 1: PBL
	    case _muPARtid: regMbusData.u16MbusData[10] = data; break;                            // 1: PFC, 2: DAB, 3:BUCK
	    case _muMODuleid: regMbusData.u16MbusData[11] = data; break;                            
	    case _muSERialNUMber0: regMbusData.u16MbusData[12] = data; break;                            
	    case _muSERialNUMber1: regMbusData.u16MbusData[13] = data; break;                            
	    case _muVERsion: regMbusData.u16MbusData[14] = data; break;                            
	    case _muBUIldDATe: regMbusData.u16MbusData[15] = data; break;                            
	    case _muUSERPARameterLENgth: regMbusData.u16MbusData[16] = data; break;                            
	    case _muMAInCONtrolSTAtus: regMbusData.u16MbusData[17] = data; break;                            
	    case _muMAInERROrSTAtus: regMbusData.u16MbusData[18] = data; break;                            
	    case _muMAInERROrMARk: regMbusData.u16MbusData[19] = data; break;                            
	    case _muMAInERROrRESult: regMbusData.u16MbusData[20] = data; break;                            
	    case _mum0gdERROrSTAtus: regMbusData.u16MbusData[21] = data; break;                            
	    case _mum0ADCERROrSTAtus: regMbusData.u16MbusData[22] = data; break;                            
	    case _muENABlePWM: regMbusData.u16MbusData[23] = data; break;                            //Bit0: ON/OFF
	    case _muVOLtageREFerence: regMbusData.u16MbusData[24] = data; break;                            
	    case _muPOSitiveCURrentLIMit: regMbusData.u16MbusData[25] = data; break;                            
	    case _muNEGativeCURrentLIMit: regMbusData.u16MbusData[26] = data; break;                            
	    case _muOVERSOUrceCURrentPROtection: regMbusData.u16MbusData[27] = data; break;                            
	    case _muOVERSINkCURrentPROtection: regMbusData.u16MbusData[28] = data; break;                            
	    case _muOVERVOLtageOUTPutPROtection: regMbusData.u16MbusData[29] = data; break;                            
	    case _muOVERVOLtageINPUtPROtection: regMbusData.u16MbusData[30] = data; break;                            
	    case _muMEAsureTEMperature: regMbusData.u16MbusData[31] = data; break;                            
	    case _muMEAsureVOLtageOUTPut: regMbusData.u16MbusData[32] = data; break;                            
	    case _muMEAsureVOLtageINPUt: regMbusData.u16MbusData[33] = data; break;                            
	    case _muMEAsureCURrentOUTPut: regMbusData.u16MbusData[34] = data; break;                            
	    case _muMEAsureINDUctorCURrent: regMbusData.u16MbusData[35] = data; break;                            
	    case _muADVAncePARameterLENgth: regMbusData.u16MbusData[36] = data; break;                            
	    case _muADVAncePASsword: regMbusData.u16MbusData[37] = data; break;                            
	    case _muVOLtageLOOpkp0: regMbusData.u16MbusData[38] = data; break;                            
	    case _muVOLtageLOOpkp1: regMbusData.u16MbusData[39] = data; break;                            
	    case _muVOLtageLOOpki0: regMbusData.u16MbusData[40] = data; break;                            
	    case _muVOLtageLOOpki1: regMbusData.u16MbusData[41] = data; break;                            
	    case _muCURrentLOOpkp0: regMbusData.u16MbusData[42] = data; break;                            
	    case _muCURrentLOOpkp1: regMbusData.u16MbusData[43] = data; break;                            
	    case _muCURrentLOOpki0: regMbusData.u16MbusData[44] = data; break;                            
	    case _muCURrentLOOpki1: regMbusData.u16MbusData[45] = data; break;                            
	    case _muOPENLOOpGAIn0: regMbusData.u16MbusData[46] = data; break;                            
	    case _muOPENLOOpGAIn1: regMbusData.u16MbusData[47] = data; break;                            
	    case _muHEArtbeatC280: regMbusData.u16MbusData[48] = data; break;                            
	    case _muHEArtbeatC281: regMbusData.u16MbusData[49] = data; break;                            
	    case _muHEArtbeatCLA0: regMbusData.u16MbusData[50] = data; break;                            
	    case _muHEArtbeatCLA1: regMbusData.u16MbusData[51] = data; break;                            
	    case _murdMODe0: regMbusData.u16MbusData[52] = data; break;                            // _FORCE_OUTPUT=1,   _OPEN_LOOP=2,  _CLOSE_LOOP=4
	    case _murdMODe1: regMbusData.u16MbusData[53] = data; break;                            // _FORCE_OUTPUT=1,   _OPEN_LOOP=2,  _CLOSE_LOOP=4
	    case _muHARdwareSLEwrateup: regMbusData.u16MbusData[54] = data; break;                            // x100usec
	    case _muHARdwareSLEwrateDOWn: regMbusData.u16MbusData[55] = data; break;                            // x100usec
	    case _muCALibrationCOMmand: regMbusData.u16MbusData[56] = data; break;                            // SaveFlash=0x4000, Vin=0x100, Vo=0x200, Io=0x400, IL=0x800
	    case _muCALibrationACK: regMbusData.u16MbusData[57] = data; break;                            
	    case _muCALibrationDATa0: regMbusData.u16MbusData[58] = data; break;                            
	    case _muCALibrationDATa1: regMbusData.u16MbusData[59] = data; break;                            
	    case _muCALibrationRETurn0: regMbusData.u16MbusData[60] = data; break;                            
	    case _muCALibrationRETurn1: regMbusData.u16MbusData[61] = data; break;                            
	    case _muRECordWORkingSTAtus: regMbusData.u16MbusData[62] = data; break;                            
	    case _muRECordERROrSTAtus: regMbusData.u16MbusData[63] = data; break;                            
	    case _muBROwninVOLtage: regMbusData.u16MbusData[64] = data; break;                            
	    case _muBROwnOUTVOLtage: regMbusData.u16MbusData[65] = data; break;                            
	    case _muGAInENABle0: regMbusData.u16MbusData[66] = data; break;                            
	    case _muGAInENABle1: regMbusData.u16MbusData[67] = data; break;                            
	    case _muviGAIn0: regMbusData.u16MbusData[68] = data; break;                            
	    case _muviGAIn1: regMbusData.u16MbusData[69] = data; break;                            
	    case _muENABleCANbusMASter0: regMbusData.u16MbusData[70] = data; break;                            
	    case _muENABleCANbusMASter1: regMbusData.u16MbusData[71] = data; break;                            
	    case _muC28ISRCOStTIMe0: regMbusData.u16MbusData[72] = data; break;                            
	    case _muC28ISRCOStTIMe1: regMbusData.u16MbusData[73] = data; break;                            
	    case _muC28ISRMAXCOStTIMe0: regMbusData.u16MbusData[74] = data; break;                            
	    case _muC28ISRMAXCOStTIMe1: regMbusData.u16MbusData[75] = data; break;                            
	    case _muBUCkVOUtMONitor1: regMbusData.u16MbusData[76] = data; break;                            
	    case _muBUCkVOUtMONitor2: regMbusData.u16MbusData[77] = data; break;                            
	    case _muBUCkVOUtMONitor3: regMbusData.u16MbusData[78] = data; break;                            
	    case _muBUCkIOUTMONitor1: regMbusData.u16MbusData[79] = data; break;                            
	    case _muBUCkIOUTMONitor2: regMbusData.u16MbusData[80] = data; break;                            
	    case _muBUCkIOUTMONitor3: regMbusData.u16MbusData[81] = data; break;                            
	    case _muonDELayMSEc: regMbusData.u16MbusData[82] = data; break;                            
	    case _muOFFDELayMSEc: regMbusData.u16MbusData[83] = data; break;                            
    default:
        return 0xFFFF;
    }
    return data;
}



