/*
 * mbcmd.h
 *
 *  Created on: 2023/8/10
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
	_muCOMmonHEADer = 0,                     // #0  T_U16                 
	_muCOMmonLENgth = 1,                     // #1  T_U16                 
	_muMAChineINFormationOFFset = 2,         // #2  T_U16                 
	_muUSERPARameterOFFset = 3,              // #3  T_U16                 
	_muADVancePARameterOFFset = 4,           // #4  T_U16                 
	_muCOMmonCHECksum = 5,                   // #5  T_U16                 
    _end_of_0_id = 6
};

enum {
	_muCLACONtrolSTATus = 1000,              // #1000  T_U16              
	_muCLAERRorSTATus = 1001,                // #1001  T_U16              
	_muENABlePWM = 1002,                     // #1002  T_U16              
	_muCONtrolREGister = 1003,               // #1003  T_U16              
	_muVOLtageREFerence = 1004,              // #1004  T_Q15              
	_muCONstantCURrentREFerence = 1005,      // #1005  T_Q15              
	_muOVERCURrentPROTection = 1006,         // #1006  T_Q15              
	_muOVERVOLtagePROTection = 1007,         // #1007  T_Q15              
	_muMEASureVOLtageOUTput = 1008,          // #1008  T_Q15              
	_muMEASureVOLtageINPut = 1009,           // #1009  T_Q15              
	_muMEASureCURrentOUTput = 1010,          // #1010  T_Q15              
	_muMEASureINDuctorCURrent = 1011,        // #1011  T_Q15              
	_muMEASureELECtronicLOADCURrent = 1012,  // #1012  T_Q15              
	_muMEASureTEMperature = 1013,            // #1013  T_Q15              
    _end_of_1000_id = 14
};

enum {
	_muVOLtageLOOPKP0 = 1020,                // #1020  T_F32              
	_muVOLtageLOOPKP1 = 1021,                // #1021  T_F32              
	_muVOLtageLOOPKI0 = 1022,                // #1022  T_F32              
	_muVOLtageLOOPKI1 = 1023,                // #1023  T_F32              
	_muVOLtageLOOPMAXimum0 = 1024,           // #1024  T_F32              
	_muVOLtageLOOPMAXimum1 = 1025,           // #1025  T_F32              
	_muVOLtageLOOPMINimum0 = 1026,           // #1026  T_F32              
	_muVOLtageLOOPMINimum1 = 1027,           // #1027  T_F32              
	_muCURrentLOOPKP0 = 1028,                // #1028  T_F32              
	_muCURrentLOOPKP1 = 1029,                // #1029  T_F32              
	_muCURrentLOOPKI0 = 1030,                // #1030  T_F32              
	_muCURrentLOOPKI1 = 1031,                // #1031  T_F32              
	_muCURrentLOOPMAXimum0 = 1032,           // #1032  T_F32              
	_muCURrentLOOPMAXimum1 = 1033,           // #1033  T_F32              
	_muCURrentLOOPMINimum0 = 1034,           // #1034  T_F32              
	_muCURrentLOOPMINimum1 = 1035,           // #1035  T_F32              
    _end_of_1020_id = 8
};

enum {
	_muCOUNtryCODe = 9000,                   // #9000  T_U16              
	_muVENderID = 9001,                      // #9001  T_U16              
	_muBUILdDATe0 = 9002,                    // #9002  T_U32              
	_muBUILdDATe1 = 9003,                    // #9003  T_U32              
    _end_of_9000_id = 3
};

typedef union { 
    uint16_t u16MbusData[40];
    struct { 
		uint16_t u16COMmonHEADer;
		uint16_t u16COMmonLENgth;
		uint16_t u16MAChineINFormationOFFset;
		uint16_t u16USERPARameterOFFset;
		uint16_t u16ADVancePARameterOFFset;
		uint16_t u16COMmonCHECksum;
		uint16_t u16CLACONtrolSTATus;
		uint16_t u16CLAERRorSTATus;
		uint16_t u16ENABlePWM;
		uint16_t u16CONtrolREGister;
		int16_t s16VOLtageREFerence;
		int16_t s16CONstantCURrentREFerence;
		int16_t s16OVERCURrentPROTection;
		int16_t s16OVERVOLtagePROTection;
		int16_t s16MEASureVOLtageOUTput;
		int16_t s16MEASureVOLtageINPut;
		int16_t s16MEASureCURrentOUTput;
		int16_t s16MEASureINDuctorCURrent;
		int16_t s16MEASureELECtronicLOADCURrent;
		int16_t s16MEASureTEMperature;
		float32_t f32VOLtageLOOPKP;
		float32_t f32VOLtageLOOPKI;
		float32_t f32VOLtageLOOPMAXimum;
		float32_t f32VOLtageLOOPMINimum;
		float32_t f32CURrentLOOPKP;
		float32_t f32CURrentLOOPKI;
		float32_t f32CURrentLOOPMAXimum;
		float32_t f32CURrentLOOPMINimum;
		uint16_t u16COUNtryCODe;
		uint16_t u16VENderID;
		uint32_t u32BUILdDATe;
    }; 
} REG_MBUSDATA;
extern REG_MBUSDATA regMbusData;
extern int chkValidAddress(uint16_t addr);
extern uint16_t getModbusData(uint16_t addr);
extern uint16_t setModbusData(uint16_t addr, uint16_t data);




#endif /* MBCMD_H_ */

