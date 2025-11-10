/*
 *  File Name: mbcmd.h
 *
 *  Created on: 2023/10/18
 *  Author: POWER-532A86
 */

#ifndef MBCMD_H_
#define MBCMD_H_



typedef enum {
	_g0_MODE = 0,
	_g1000_MODE,
	_g1020_MODE,
	_g9000_MODE,
    _END_OF_MODE
} ID_MODE;


enum {
	_muCOMmonHEAder = 0,                    // #0  T_U16                
	_muCOMmonLENgth = 1,                    // #1  T_U16                
	_muMAChineINFOrmationOFFSet = 2,        // #2  T_U16                
	_muUSERPARameterOFFSet = 3,             // #3  T_U16                
	_muADVAncePARameterOFFSet = 4,          // #4  T_U16                
	_muCOMmonCHEcksum = 5,                  // #5  T_U16                
    _end_of_0_id = 6
};

enum {
	_muUARTemueeFSMSTAtus = 1000,           // #1000  T_U16             
	_muU32Writesuccess = 1001,              // #1001  T_U16             
	_muU8WRiteaction = 1002,                // #1002  T_U16             
	_muFGEmueeCONtrol = 1003,               // #1003  T_U16             
	_muSTEmueescb = 1004,                   // #1004  T_U16             
	_muSTEmueespg = 1005,                   // #1005  T_U16             
	_muOVERCURrentPROtection = 1006,        // #1006  T_Q15             
	_muOVERVOLtagePROtection = 1007,        // #1007  T_Q15             
	_muMEAsureVOLtageOUTPut = 1008,         // #1008  T_Q15             
	_muMEAsureVOLtageINPUt = 1009,          // #1009  T_Q15             
	_muMEAsureCURrentOUTPut = 1010,         // #1010  T_Q15             
	_muMEAsureINDUctorCURrent = 1011,       // #1011  T_Q15             
	_muMEAsureELECtronicLOAdCURrent = 1012, // #1012  T_Q15             
	_muMEAsureTEMperature = 1013,           // #1013  T_Q15             
    _end_of_1000_id = 14
};

enum {
	_muVOLtageLOOpkp0 = 1020,               // #1020  T_F32             
	_muVOLtageLOOpkp1 = 1021,               // #1021  T_F32             
	_muVOLtageLOOpki0 = 1022,               // #1022  T_F32             
	_muVOLtageLOOpki1 = 1023,               // #1023  T_F32             
	_muVOLtageLOOpMAXimum0 = 1024,          // #1024  T_F32             
	_muVOLtageLOOpMAXimum1 = 1025,          // #1025  T_F32             
	_muVOLtageLOOpMINimum0 = 1026,          // #1026  T_F32             
	_muVOLtageLOOpMINimum1 = 1027,          // #1027  T_F32             
	_muCURrentLOOpkp0 = 1028,               // #1028  T_F32             
	_muCURrentLOOpkp1 = 1029,               // #1029  T_F32             
	_muCURrentLOOpki0 = 1030,               // #1030  T_F32             
	_muCURrentLOOpki1 = 1031,               // #1031  T_F32             
	_muCURrentLOOpMAXimum0 = 1032,          // #1032  T_F32             
	_muCURrentLOOpMAXimum1 = 1033,          // #1033  T_F32             
	_muCURrentLOOpMINimum0 = 1034,          // #1034  T_F32             
	_muCURrentLOOpMINimum1 = 1035,          // #1035  T_F32             
    _end_of_1020_id = 8
};

enum {
	_muCOUntryCODe = 9000,                  // #9000  T_U16             
	_muVENderid = 9001,                     // #9001  T_U16             
	_muBUIldDATe0 = 9002,                   // #9002  T_U32             
	_muBUIldDATe1 = 9003,                   // #9003  T_U32             
    _end_of_9000_id = 3
};

typedef union { 
    uint16_t u16MbusData[40];
    struct { 
		uint16_t u16COMmonHEAder;
		uint16_t u16COMmonLENgth;
		uint16_t u16MAChineINFOrmationOFFSet;
		uint16_t u16USERPARameterOFFSet;
		uint16_t u16ADVAncePARameterOFFSet;
		uint16_t u16COMmonCHEcksum;
		uint16_t u16UARTemueeFSMSTAtus;
		uint16_t u16U32Writesuccess;
		uint16_t u16U8WRiteaction;
		uint16_t u16FGEmueeCONtrol;
		uint16_t u16STEmueescb;
		uint16_t u16STEmueespg;
		int16_t s16OVERCURrentPROtection;
		int16_t s16OVERVOLtagePROtection;
		int16_t s16MEAsureVOLtageOUTPut;
		int16_t s16MEAsureVOLtageINPUt;
		int16_t s16MEAsureCURrentOUTPut;
		int16_t s16MEAsureINDUctorCURrent;
		int16_t s16MEAsureELECtronicLOAdCURrent;
		int16_t s16MEAsureTEMperature;
		float32_t f32VOLtageLOOpkp;
		float32_t f32VOLtageLOOpki;
		float32_t f32VOLtageLOOpMAXimum;
		float32_t f32VOLtageLOOpMINimum;
		float32_t f32CURrentLOOpkp;
		float32_t f32CURrentLOOpki;
		float32_t f32CURrentLOOpMAXimum;
		float32_t f32CURrentLOOpMINimum;
		uint16_t u16COUntryCODe;
		uint16_t u16VENderid;
		uint32_t u32BUIldDATe;
    }; 
} REG_MBUSDATA;
extern REG_MBUSDATA regMbusData;
extern int chkValidAddress(uint16_t addr);
extern uint16_t getModbusData(uint16_t addr);
extern uint16_t setModbusData(uint16_t addr, uint16_t data);




#endif /* MBCMD_H_ */

