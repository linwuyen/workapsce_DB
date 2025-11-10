/*
 * tbmenu.c
 *
 *  Created on: 8/9/2023
 */
#include "ModbusSlave.h"
#include "mbcmd.h"


REG_MBUSDATA regMbusData;

int chkValidAddress(uint16_t addr) {
    switch(addr) {
	    case _muCOMmonHEADer:                                             
	    case _muCOMmonLENgth:                                              
	    case _muMAChineINFormationOFFset:                                   
	    case _muUSERPARameterOFFset:                                         
	    case _muADVancePARameterOFFset:                                       
	    case _muCOMmonCHECksum:                                                
	    case _muCLACONtrolSTATus:                                               
	    case _muCLAERRorSTATus:                                                  
	    case _muENABlePWM:                                                        
	    case _muCONtrolREGister:                                                   
	    case _muVOLtageREFerence:                                                   
	    case _muCONstantCURrentREFerence:                                            
	    case _muOVERCURrentPROTection:                                                
	    case _muOVERVOLtagePROTection:                                                 
	    case _muMEASureVOLtageOUTput:                                                   
	    case _muMEASureVOLtageINPut:                                                     
	    case _muMEASureCURrentOUTput:                                                     
	    case _muMEASureINDuctorCURrent:                                                    
	    case _muMEASureELECtronicLOADCURrent:                                                
	    case _muMEASureTEMperature:                                                          
	    case _muVOLtageLOOPKP0:                                                               
	    case _muVOLtageLOOPKP1:                                                                
	    case _muVOLtageLOOPKI0:                                                                 
	    case _muVOLtageLOOPKI1:                                                                  
	    case _muVOLtageLOOPMAXimum0:                                                              
	    case _muVOLtageLOOPMAXimum1:                                                               
	    case _muVOLtageLOOPMINimum0:                                                                
	    case _muVOLtageLOOPMINimum1:                                                                 
	    case _muCURrentLOOPKP0:                                                                       
	    case _muCURrentLOOPKP1:                                                                        
	    case _muCURrentLOOPKI0:                                                                         
	    case _muCURrentLOOPKI1:                                                                          
	    case _muCURrentLOOPMAXimum0:                                                                      
	    case _muCURrentLOOPMAXimum1:                                                                       
	    case _muCURrentLOOPMINimum0:                                                                        
	    case _muCURrentLOOPMINimum1:                                                                         
	    case _muCOUNtryCODe:                                                                                  
	    case _muVENderID:                                                                                      
	    case _muBUILdDATe0:                                                                                     
	    case _muBUILdDATe1:                                                                                      
        return 0;
    default:
        return MB_ERROR_ILLEGALADDR;
    }
}

uint16_t getModbusData(uint16_t addr) {
    switch(addr) {
	    case _muCOMmonHEADer: return regMbusData.u16MbusData[0];                                                                      
	    case _muCOMmonLENgth: return regMbusData.u16MbusData[1];                                                                       
	    case _muMAChineINFormationOFFset: return regMbusData.u16MbusData[2];                                                                        
	    case _muUSERPARameterOFFset: return regMbusData.u16MbusData[3];                                                                         
	    case _muADVancePARameterOFFset: return regMbusData.u16MbusData[4];                                                                          
	    case _muCOMmonCHECksum: return regMbusData.u16MbusData[5];                                                                           
	    case _muCLACONtrolSTATus: return regMbusData.u16MbusData[6];                                                                            
	    case _muCLAERRorSTATus: return regMbusData.u16MbusData[7];                                                                             
	    case _muENABlePWM: return regMbusData.u16MbusData[8];                                                                              
	    case _muCONtrolREGister: return regMbusData.u16MbusData[9];                                                                               
	    case _muVOLtageREFerence: return regMbusData.u16MbusData[10];                                                                                
	    case _muCONstantCURrentREFerence: return regMbusData.u16MbusData[11];                                                                                 
	    case _muOVERCURrentPROTection: return regMbusData.u16MbusData[12];                                                                                  
	    case _muOVERVOLtagePROTection: return regMbusData.u16MbusData[13];                                                                                   
	    case _muMEASureVOLtageOUTput: return regMbusData.u16MbusData[14];                                                                                    
	    case _muMEASureVOLtageINPut: return regMbusData.u16MbusData[15];                                                                                     
	    case _muMEASureCURrentOUTput: return regMbusData.u16MbusData[16];                                                                                      
	    case _muMEASureINDuctorCURrent: return regMbusData.u16MbusData[17];                                                                                       
	    case _muMEASureELECtronicLOADCURrent: return regMbusData.u16MbusData[18];                                                                                        
	    case _muMEASureTEMperature: return regMbusData.u16MbusData[19];                                                                                         
	    case _muVOLtageLOOPKP0: return regMbusData.u16MbusData[20];                                                                                          
	    case _muVOLtageLOOPKP1: return regMbusData.u16MbusData[21];                                                                                           
	    case _muVOLtageLOOPKI0: return regMbusData.u16MbusData[22];                                                                                            
	    case _muVOLtageLOOPKI1: return regMbusData.u16MbusData[23];                                                                                             
	    case _muVOLtageLOOPMAXimum0: return regMbusData.u16MbusData[24];                                                                                              
	    case _muVOLtageLOOPMAXimum1: return regMbusData.u16MbusData[25];                                                                                               
	    case _muVOLtageLOOPMINimum0: return regMbusData.u16MbusData[26];                                                                                                
	    case _muVOLtageLOOPMINimum1: return regMbusData.u16MbusData[27];                                                                                                 
	    case _muCURrentLOOPKP0: return regMbusData.u16MbusData[28];                                                                                                  
	    case _muCURrentLOOPKP1: return regMbusData.u16MbusData[29];                                                                                                   
	    case _muCURrentLOOPKI0: return regMbusData.u16MbusData[30];                                                                                                    
	    case _muCURrentLOOPKI1: return regMbusData.u16MbusData[31];                                                                                                     
	    case _muCURrentLOOPMAXimum0: return regMbusData.u16MbusData[32];                                                                                                      
	    case _muCURrentLOOPMAXimum1: return regMbusData.u16MbusData[33];                                                                                                       
	    case _muCURrentLOOPMINimum0: return regMbusData.u16MbusData[34];                                                                                                        
	    case _muCURrentLOOPMINimum1: return regMbusData.u16MbusData[35];                                                                                                         
	    case _muCOUNtryCODe: return regMbusData.u16MbusData[36];                                                                                                          
	    case _muVENderID: return regMbusData.u16MbusData[37];                                                                                                           
	    case _muBUILdDATe0: return regMbusData.u16MbusData[38];                                                                                                            
	    case _muBUILdDATe1: return regMbusData.u16MbusData[39];                                                                                                             
    default:
        return 0xFFFF;
    }
}

uint16_t setModbusData(uint16_t addr, uint16_t data) {
    switch(addr) {
	    case _muCOMmonHEADer: regMbusData.u16MbusData[0] = data; break;                                                                                                              
	    case _muCOMmonLENgth: regMbusData.u16MbusData[1] = data; break;                                                                                                               
	    case _muMAChineINFormationOFFset: regMbusData.u16MbusData[2] = data; break;                                                                                                                
	    case _muUSERPARameterOFFset: regMbusData.u16MbusData[3] = data; break;                                                                                                                 
	    case _muADVancePARameterOFFset: regMbusData.u16MbusData[4] = data; break;                                                                                                                  
	    case _muCOMmonCHECksum: regMbusData.u16MbusData[5] = data; break;                                                                                                                   
	    case _muCLACONtrolSTATus: regMbusData.u16MbusData[6] = data; break;                                                                                                                    
	    case _muCLAERRorSTATus: regMbusData.u16MbusData[7] = data; break;                                                                                                                     
	    case _muENABlePWM: regMbusData.u16MbusData[8] = data; break;                                                                                                                      
	    case _muCONtrolREGister: regMbusData.u16MbusData[9] = data; break;                                                                                                                       
	    case _muVOLtageREFerence: regMbusData.u16MbusData[10] = data; break;                                                                                                                        
	    case _muCONstantCURrentREFerence: regMbusData.u16MbusData[11] = data; break;                                                                                                                         
	    case _muOVERCURrentPROTection: regMbusData.u16MbusData[12] = data; break;                                                                                                                          
	    case _muOVERVOLtagePROTection: regMbusData.u16MbusData[13] = data; break;                                                                                                                           
	    case _muMEASureVOLtageOUTput: regMbusData.u16MbusData[14] = data; break;                                                                                                                            
	    case _muMEASureVOLtageINPut: regMbusData.u16MbusData[15] = data; break;                                                                                                                             
	    case _muMEASureCURrentOUTput: regMbusData.u16MbusData[16] = data; break;                                                                                                                              
	    case _muMEASureINDuctorCURrent: regMbusData.u16MbusData[17] = data; break;                                                                                                                               
	    case _muMEASureELECtronicLOADCURrent: regMbusData.u16MbusData[18] = data; break;                                                                                                                                
	    case _muMEASureTEMperature: regMbusData.u16MbusData[19] = data; break;                                                                                                                                 
	    case _muVOLtageLOOPKP0: regMbusData.u16MbusData[20] = data; break;                                                                                                                                  
	    case _muVOLtageLOOPKP1: regMbusData.u16MbusData[21] = data; break;                                                                                                                                   
	    case _muVOLtageLOOPKI0: regMbusData.u16MbusData[22] = data; break;                                                                                                                                    
	    case _muVOLtageLOOPKI1: regMbusData.u16MbusData[23] = data; break;                                                                                                                                     
	    case _muVOLtageLOOPMAXimum0: regMbusData.u16MbusData[24] = data; break;                                                                                                                                      
	    case _muVOLtageLOOPMAXimum1: regMbusData.u16MbusData[25] = data; break;                                                                                                                                       
	    case _muVOLtageLOOPMINimum0: regMbusData.u16MbusData[26] = data; break;                                                                                                                                        
	    case _muVOLtageLOOPMINimum1: regMbusData.u16MbusData[27] = data; break;                                                                                                                                         
	    case _muCURrentLOOPKP0: regMbusData.u16MbusData[28] = data; break;                                                                                                                                          
	    case _muCURrentLOOPKP1: regMbusData.u16MbusData[29] = data; break;                                                                                                                                           
	    case _muCURrentLOOPKI0: regMbusData.u16MbusData[30] = data; break;                                                                                                                                            
	    case _muCURrentLOOPKI1: regMbusData.u16MbusData[31] = data; break;                                                                                                                                             
	    case _muCURrentLOOPMAXimum0: regMbusData.u16MbusData[32] = data; break;                                                                                                                                              
	    case _muCURrentLOOPMAXimum1: regMbusData.u16MbusData[33] = data; break;                                                                                                                                               
	    case _muCURrentLOOPMINimum0: regMbusData.u16MbusData[34] = data; break;                                                                                                                                                
	    case _muCURrentLOOPMINimum1: regMbusData.u16MbusData[35] = data; break;                                                                                                                                                 
	    case _muCOUNtryCODe: regMbusData.u16MbusData[36] = data; break;                                                                                                                                                  
	    case _muVENderID: regMbusData.u16MbusData[37] = data; break;                                                                                                                                                   
	    case _muBUILdDATe0: regMbusData.u16MbusData[38] = data; break;                                                                                                                                                    
	    case _muBUILdDATe1: regMbusData.u16MbusData[39] = data; break;                                                                                                                                                     
    default:
        return 0xFFFF;
    }
    return data;
}



