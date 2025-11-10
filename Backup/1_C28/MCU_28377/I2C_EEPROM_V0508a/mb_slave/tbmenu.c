/*
 *  File Name: tbmenu.c
 *
 *  Created on: 2023/5/8
 *  Author: POWER2-54FD92
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
	    case _muCLACONtrolSTAtus:                                       
	    case _muCLAERROrSTAtus:                                         
	    case _muENABlePWM:                                              
	    case _muCONtrolMODe:                                            
	    case _muVOLtageREFerence:                                       
	    case _muCONstantCURrentREFerence:                               
	    case _muOVERCURrentPROtection:                                  
	    case _muOVERVOLtagePROtection:                                  
	    case _muMEAsureVOLtageOUTPut:                                   
	    case _muMEAsureVOLtageINPUt:                                    
	    case _muMEAsureCURrentOUTPut:                                   
	    case _muMEAsureINDUctorCURrent:                                 
	    case _muMEAsureELECtronicLOAdCURrent:                            
	    case _muMEAsureTEMperature:                                     
	    case _muVOLtageLOOpkp0:                                         
	    case _muVOLtageLOOpkp1:                                         
	    case _muVOLtageLOOpki0:                                         
	    case _muVOLtageLOOpki1:                                         
	    case _muVOLtageLOOpMAXimum0:                                    
	    case _muVOLtageLOOpMAXimum1:                                    
	    case _muVOLtageLOOpMINimum0:                                    
	    case _muVOLtageLOOpMINimum1:                                    
	    case _muCURrentLOOpkp0:                                         
	    case _muCURrentLOOpkp1:                                         
	    case _muCURrentLOOpki0:                                         
	    case _muCURrentLOOpki1:                                         
	    case _muCURrentLOOpMAXimum0:                                    
	    case _muCURrentLOOpMAXimum1:                                    
	    case _muCURrentLOOpMINimum0:                                    
	    case _muCURrentLOOpMINimum1:                                    
	    case _muCOUntryCODe:                                            
	    case _muVENderid:                                               
	    case _muBUIldDATe0:                                             
	    case _muBUIldDATe1:                                             
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
	    case _muCLACONtrolSTAtus: return regMbusData.u16MbusData[6];                            
	    case _muCLAERROrSTAtus: return regMbusData.u16MbusData[7];                            
	    case _muENABlePWM: return regMbusData.u16MbusData[8];                            
	    case _muCONtrolMODe: return regMbusData.u16MbusData[9];                            
	    case _muVOLtageREFerence: return regMbusData.u16MbusData[10];                            
	    case _muCONstantCURrentREFerence: return regMbusData.u16MbusData[11];                            
	    case _muOVERCURrentPROtection: return regMbusData.u16MbusData[12];                            
	    case _muOVERVOLtagePROtection: return regMbusData.u16MbusData[13];                            
	    case _muMEAsureVOLtageOUTPut: return regMbusData.u16MbusData[14];                            
	    case _muMEAsureVOLtageINPUt: return regMbusData.u16MbusData[15];                            
	    case _muMEAsureCURrentOUTPut: return regMbusData.u16MbusData[16];                            
	    case _muMEAsureINDUctorCURrent: return regMbusData.u16MbusData[17];                            
	    case _muMEAsureELECtronicLOAdCURrent: return regMbusData.u16MbusData[18];                            
	    case _muMEAsureTEMperature: return regMbusData.u16MbusData[19];                            
	    case _muVOLtageLOOpkp0: return regMbusData.u16MbusData[20];                            
	    case _muVOLtageLOOpkp1: return regMbusData.u16MbusData[21];                            
	    case _muVOLtageLOOpki0: return regMbusData.u16MbusData[22];                            
	    case _muVOLtageLOOpki1: return regMbusData.u16MbusData[23];                            
	    case _muVOLtageLOOpMAXimum0: return regMbusData.u16MbusData[24];                            
	    case _muVOLtageLOOpMAXimum1: return regMbusData.u16MbusData[25];                            
	    case _muVOLtageLOOpMINimum0: return regMbusData.u16MbusData[26];                            
	    case _muVOLtageLOOpMINimum1: return regMbusData.u16MbusData[27];                            
	    case _muCURrentLOOpkp0: return regMbusData.u16MbusData[28];                            
	    case _muCURrentLOOpkp1: return regMbusData.u16MbusData[29];                            
	    case _muCURrentLOOpki0: return regMbusData.u16MbusData[30];                            
	    case _muCURrentLOOpki1: return regMbusData.u16MbusData[31];                            
	    case _muCURrentLOOpMAXimum0: return regMbusData.u16MbusData[32];                            
	    case _muCURrentLOOpMAXimum1: return regMbusData.u16MbusData[33];                            
	    case _muCURrentLOOpMINimum0: return regMbusData.u16MbusData[34];                            
	    case _muCURrentLOOpMINimum1: return regMbusData.u16MbusData[35];                            
	    case _muCOUntryCODe: return regMbusData.u16MbusData[36];                            
	    case _muVENderid: return regMbusData.u16MbusData[37];                            
	    case _muBUIldDATe0: return regMbusData.u16MbusData[38];                            
	    case _muBUIldDATe1: return regMbusData.u16MbusData[39];                            
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
	    case _muCLACONtrolSTAtus: regMbusData.u16MbusData[6] = data; break;                            
	    case _muCLAERROrSTAtus: regMbusData.u16MbusData[7] = data; break;                            
	    case _muENABlePWM: regMbusData.u16MbusData[8] = data; break;                            
	    case _muCONtrolMODe: regMbusData.u16MbusData[9] = data; break;                            
	    case _muVOLtageREFerence: regMbusData.u16MbusData[10] = data; break;                            
	    case _muCONstantCURrentREFerence: regMbusData.u16MbusData[11] = data; break;                            
	    case _muOVERCURrentPROtection: regMbusData.u16MbusData[12] = data; break;                            
	    case _muOVERVOLtagePROtection: regMbusData.u16MbusData[13] = data; break;                            
	    case _muMEAsureVOLtageOUTPut: regMbusData.u16MbusData[14] = data; break;                            
	    case _muMEAsureVOLtageINPUt: regMbusData.u16MbusData[15] = data; break;                            
	    case _muMEAsureCURrentOUTPut: regMbusData.u16MbusData[16] = data; break;                            
	    case _muMEAsureINDUctorCURrent: regMbusData.u16MbusData[17] = data; break;                            
	    case _muMEAsureELECtronicLOAdCURrent: regMbusData.u16MbusData[18] = data; break;                            
	    case _muMEAsureTEMperature: regMbusData.u16MbusData[19] = data; break;                            
	    case _muVOLtageLOOpkp0: regMbusData.u16MbusData[20] = data; break;                            
	    case _muVOLtageLOOpkp1: regMbusData.u16MbusData[21] = data; break;                            
	    case _muVOLtageLOOpki0: regMbusData.u16MbusData[22] = data; break;                            
	    case _muVOLtageLOOpki1: regMbusData.u16MbusData[23] = data; break;                            
	    case _muVOLtageLOOpMAXimum0: regMbusData.u16MbusData[24] = data; break;                            
	    case _muVOLtageLOOpMAXimum1: regMbusData.u16MbusData[25] = data; break;                            
	    case _muVOLtageLOOpMINimum0: regMbusData.u16MbusData[26] = data; break;                            
	    case _muVOLtageLOOpMINimum1: regMbusData.u16MbusData[27] = data; break;                            
	    case _muCURrentLOOpkp0: regMbusData.u16MbusData[28] = data; break;                            
	    case _muCURrentLOOpkp1: regMbusData.u16MbusData[29] = data; break;                            
	    case _muCURrentLOOpki0: regMbusData.u16MbusData[30] = data; break;                            
	    case _muCURrentLOOpki1: regMbusData.u16MbusData[31] = data; break;                            
	    case _muCURrentLOOpMAXimum0: regMbusData.u16MbusData[32] = data; break;                            
	    case _muCURrentLOOpMAXimum1: regMbusData.u16MbusData[33] = data; break;                            
	    case _muCURrentLOOpMINimum0: regMbusData.u16MbusData[34] = data; break;                            
	    case _muCURrentLOOpMINimum1: regMbusData.u16MbusData[35] = data; break;                            
	    case _muCOUntryCODe: regMbusData.u16MbusData[36] = data; break;                            
	    case _muVENderid: regMbusData.u16MbusData[37] = data; break;                            
	    case _muBUIldDATe0: regMbusData.u16MbusData[38] = data; break;                            
	    case _muBUIldDATe1: regMbusData.u16MbusData[39] = data; break;                            
    default:
        return 0xFFFF;
    }
    return data;
}



