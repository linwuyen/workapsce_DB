/*
 *  File Name: tbmenu.c
 *
 *  Created on: 2023/10/13
 *  Author: POWER-532A86
 */

#include "ModbusSlave.h"
#include "mbcmd.h"


REG_MBUSDATA regMbusData;

int chkValidAddress(uint16_t addr) {
    switch(addr) {
	    case _muCLACONtrolSTAtus:                                       
	    case _muCLAERROrSTAtus:                                         
	    case _muENABlePWM:                                              
	    case _muCONtrolMODe:                                            
	    case _muVOLtageREFerence:                                       
	    case _muCONstantCURrentREFerence:                               
	    case _muOVERCURrentPROtection:                                  
	    case _muOVERVOLtagePROtection:                                  
	    case _muFLAshCOUnter0:                                          
	    case _muFLAshCOUnter1:                                          
        return 0;
    default:
        return MB_ERROR_ILLEGALADDR;
    }
}

uint16_t getModbusData(uint16_t addr) {
    switch(addr) {
	    case _muCLACONtrolSTAtus: return regMbusData.u16MbusData[0];                            
	    case _muCLAERROrSTAtus: return regMbusData.u16MbusData[1];                            
	    case _muENABlePWM: return regMbusData.u16MbusData[2];                            
	    case _muCONtrolMODe: return regMbusData.u16MbusData[3];                            
	    case _muVOLtageREFerence: return regMbusData.u16MbusData[4];                            
	    case _muCONstantCURrentREFerence: return regMbusData.u16MbusData[5];                            
	    case _muOVERCURrentPROtection: return regMbusData.u16MbusData[6];                            
	    case _muOVERVOLtagePROtection: return regMbusData.u16MbusData[7];                            
	    case _muFLAshCOUnter0: return regMbusData.u16MbusData[8];                            
	    case _muFLAshCOUnter1: return regMbusData.u16MbusData[9];                            
    default:
        return 0xFFFF;
    }
}

uint16_t setModbusData(uint16_t addr, uint16_t data) {
    switch(addr) {
	    case _muCLACONtrolSTAtus: regMbusData.u16MbusData[0] = data; break;                            
	    case _muCLAERROrSTAtus: regMbusData.u16MbusData[1] = data; break;                            
	    case _muENABlePWM: regMbusData.u16MbusData[2] = data; break;                            
	    case _muCONtrolMODe: regMbusData.u16MbusData[3] = data; break;                            
	    case _muVOLtageREFerence: regMbusData.u16MbusData[4] = data; break;                            
	    case _muCONstantCURrentREFerence: regMbusData.u16MbusData[5] = data; break;                            
	    case _muOVERCURrentPROtection: regMbusData.u16MbusData[6] = data; break;                            
	    case _muOVERVOLtagePROtection: regMbusData.u16MbusData[7] = data; break;                            
	    case _muFLAshCOUnter0: regMbusData.u16MbusData[8] = data; break;                            
	    case _muFLAshCOUnter1: regMbusData.u16MbusData[9] = data; break;                            
    default:
        return 0xFFFF;
    }
    return data;
}



