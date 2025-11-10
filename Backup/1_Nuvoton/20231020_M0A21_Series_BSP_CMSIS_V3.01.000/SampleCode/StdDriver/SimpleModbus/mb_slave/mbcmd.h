/*
 *  File Name: mbcmd.h
 *
 *  Created on: 2023/10/13
 *  Author: POWER-532A86
 */

#ifndef MBCMD_H_
#define MBCMD_H_



typedef enum {
	_g1000_MODE = 0,
    _END_OF_MODE
} ID_MODE;


enum {
	_muCLACONtrolSTAtus = 1000,             // #1000  T_U16             
	_muCLAERROrSTAtus = 1001,               // #1001  T_U16             
	_muENABlePWM = 1002,                    // #1002  T_U16             
	_muCONtrolMODe = 1003,                  // #1003  T_U16             
	_muVOLtageREFerence = 1004,             // #1004  T_Q15             
	_muCONstantCURrentREFerence = 1005,     // #1005  T_U16             
	_muOVERCURrentPROtection = 1006,        // #1006  T_Q15             
	_muOVERVOLtagePROtection = 1007,        // #1007  T_Q15             
	_muFLAshCOUnter0 = 1008,                // #1008  T_U32             
	_muFLAshCOUnter1 = 1009,                // #1009  T_U32             
    _end_of_1000_id = 9
};

typedef union { 
    uint16_t u16MbusData[10];
    struct { 
		uint16_t u16CLACONtrolSTAtus;
		uint16_t u16CLAERROrSTAtus;
		uint16_t u16ENABlePWM;
		uint16_t u16CONtrolMODe;
		int16_t s16VOLtageREFerence;
		uint16_t u16CONstantCURrentREFerence;
		int16_t s16OVERCURrentPROtection;
		int16_t s16OVERVOLtagePROtection;
		uint32_t u32FLAshCOUnter;
    }; 
} REG_MBUSDATA;
extern REG_MBUSDATA regMbusData;
extern int chkValidAddress(uint16_t addr);
extern uint16_t getModbusData(uint16_t addr);
extern uint16_t setModbusData(uint16_t addr, uint16_t data);




#endif /* MBCMD_H_ */

