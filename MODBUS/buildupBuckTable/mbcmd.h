/*
 *  File Name: mbcmd.h
 *
 *  Created on: 8/27/2025
 *  Author: POWER-532A86
 */

#ifndef MBCMD_H_
#define MBCMD_H_



typedef enum {
	_g0_MODE = 0,
    _END_OF_MODE
} ID_MODE;


enum {
	_muCOMmonHEAder = 0,                    // #0  T_U16                
	_muCOMmonLENgth = 1,                    // #1  T_U16                
	_muMAChineINFOrmationOFFSet = 2,        // #2  T_U16                
	_muUSERPARameterOFFSet = 3,             // #3  T_U16                
	_muADVAncePARameterOFFSet = 4,          // #4  T_U16                
	_muCOMmonCHEcksum = 5,                  // #5  T_U16                
	_muMAChineLENgth = 6,                   // #6  T_U16                
	_muCOUntryCODe = 7,                     // #7  T_U16                
	_muVENderid = 8,                        // #8  T_U16                
	_muPROductid = 9,                       // #9  T_U16                // 1: PBL
	_muPARtid = 10,                         // #10  T_U16               // 1: PFC, 2: DAB, 3:BUCK
	_muMODuleid = 11,                       // #11  T_U16               
	_muSERialNUMber0 = 12,                  // #12  T_U32               
	_muSERialNUMber1 = 13,                  // #13  T_U32               
	_muVERsion = 14,                        // #14  T_U16               
	_muBUIldDATe = 15,                      // #15  T_U16               
	_muUSERPARameterLENgth = 16,            // #16  T_U16               
	_muMAInCONtrolSTAtus = 17,              // #17  T_U16               
	_muMAInERROrSTAtus = 18,                // #18  T_U16               
	_muMAInERROrMARk = 19,                  // #19  T_U16               
	_muMAInERROrRESult = 20,                // #20  T_U16               
	_mum0gdERROrSTAtus = 21,                // #21  T_U16               
	_mum0ADCERROrSTAtus = 22,               // #22  T_U16               
	_muENABlePWM = 23,                      // #23  T_U16               //Bit0: ON/OFF
	_muVOLtageREFerence = 24,               // #24  T_Q15               
	_muPOSitiveCURrentLIMit = 25,           // #25  T_Q15               
	_muNEGativeCURrentLIMit = 26,           // #26  T_Q15               
	_muOVERSOUrceCURrentPROtection = 27,    // #27  T_Q15               
	_muOVERSINkCURrentPROtection = 28,      // #28  T_Q15               
	_muOVERVOLtageOUTPutPROtection = 29,    // #29  T_Q15               
	_muOVERVOLtageINPUtPROtection = 30,     // #30  T_Q15               
	_muMEAsureTEMperature = 31,             // #31  T_Q15               
	_muMEAsureVOLtageOUTPut = 32,           // #32  T_Q15               
	_muMEAsureVOLtageINPUt = 33,            // #33  T_Q15               
	_muMEAsureCURrentOUTPut = 34,           // #34  T_Q15               
	_muMEAsureINDUctorCURrent = 35,         // #35  T_Q15               
	_muADVAncePARameterLENgth = 36,         // #36  T_U16               
	_muADVAncePASsword = 37,                // #37  T_U16               
	_muVOLtageLOOpkp0 = 38,                 // #38  T_F32               
	_muVOLtageLOOpkp1 = 39,                 // #39  T_F32               
	_muVOLtageLOOpki0 = 40,                 // #40  T_F32               
	_muVOLtageLOOpki1 = 41,                 // #41  T_F32               
	_muCURrentLOOpkp0 = 42,                 // #42  T_F32               
	_muCURrentLOOpkp1 = 43,                 // #43  T_F32               
	_muCURrentLOOpki0 = 44,                 // #44  T_F32               
	_muCURrentLOOpki1 = 45,                 // #45  T_F32               
	_muOPENLOOpGAIn0 = 46,                  // #46  T_F32               
	_muOPENLOOpGAIn1 = 47,                  // #47  T_F32               
	_muHEArtbeatC280 = 48,                  // #48  T_U32               
	_muHEArtbeatC281 = 49,                  // #49  T_U32               
	_muHEArtbeatCLA0 = 50,                  // #50  T_U32               
	_muHEArtbeatCLA1 = 51,                  // #51  T_U32               
	_murdMODe0 = 52,                        // #52  T_U32               // _FORCE_OUTPUT=1,   _OPEN_LOOP=2,  _CLOSE_LOOP=4
	_murdMODe1 = 53,                        // #53  T_U32               // _FORCE_OUTPUT=1,   _OPEN_LOOP=2,  _CLOSE_LOOP=4
	_muHARdwareSLEwrateup = 54,             // #54  T_U16               // x100usec
	_muHARdwareSLEwrateDOWn = 55,           // #55  T_U16               // x100usec
	_muCALibrationCOMmand = 56,             // #56  T_U16               // SaveFlash=0x4000, Vin=0x100, Vo=0x200, Io=0x400, IL=0x800
	_muCALibrationACK = 57,                 // #57  T_U16               
	_muCALibrationDATa0 = 58,               // #58  T_F32               
	_muCALibrationDATa1 = 59,               // #59  T_F32               
	_muCALibrationRETurn0 = 60,             // #60  T_F32               
	_muCALibrationRETurn1 = 61,             // #61  T_F32               
	_muRECordWORkingSTAtus = 62,            // #62  T_U16               
	_muRECordERROrSTAtus = 63,              // #63  T_U16               
	_muBROwninVOLtage = 64,                 // #64  T_Q15               
	_muBROwnOUTVOLtage = 65,                // #65  T_Q15               
	_muGAInENABle0 = 66,                    // #66  T_F32               
	_muGAInENABle1 = 67,                    // #67  T_F32               
	_muviGAIn0 = 68,                        // #68  T_F32               
	_muviGAIn1 = 69,                        // #69  T_F32               
	_muENABleCANbusMASter0 = 70,            // #70  T_U32               
	_muENABleCANbusMASter1 = 71,            // #71  T_U32               
	_muC28ISRCOStTIMe0 = 72,                // #72  T_F32               
	_muC28ISRCOStTIMe1 = 73,                // #73  T_F32               
	_muC28ISRMAXCOStTIMe0 = 74,             // #74  T_F32               
	_muC28ISRMAXCOStTIMe1 = 75,             // #75  T_F32               
	_muBUCkVOUtMONitor1 = 76,               // #76  T_Q15               
	_muBUCkVOUtMONitor2 = 77,               // #77  T_Q15               
	_muBUCkVOUtMONitor3 = 78,               // #78  T_Q15               
	_muBUCkIOUTMONitor1 = 79,               // #79  T_Q15               
	_muBUCkIOUTMONitor2 = 80,               // #80  T_Q15               
	_muBUCkIOUTMONitor3 = 81,               // #81  T_Q15               
	_muonDELayMSEc = 82,                    // #82  T_U16               
	_muOFFDELayMSEc = 83,                   // #83  T_U16               
    _end_of_0_id = 68
};

typedef union { 
    uint16_t u16MbusData[84];
    struct { 
		uint16_t u16COMmonHEAder;
		uint16_t u16COMmonLENgth;
		uint16_t u16MAChineINFOrmationOFFSet;
		uint16_t u16USERPARameterOFFSet;
		uint16_t u16ADVAncePARameterOFFSet;
		uint16_t u16COMmonCHEcksum;
		uint16_t u16MAChineLENgth;
		uint16_t u16COUntryCODe;
		uint16_t u16VENderid;
		uint16_t u16PROductid;
		uint16_t u16PARtid;
		uint16_t u16MODuleid;
		uint32_t u32SERialNUMber;
		uint16_t u16VERsion;
		uint16_t u16BUIldDATe;
		uint16_t u16USERPARameterLENgth;
		uint16_t u16MAInCONtrolSTAtus;
		uint16_t u16MAInERROrSTAtus;
		uint16_t u16MAInERROrMARk;
		uint16_t u16MAInERROrRESult;
		uint16_t u16m0gdERROrSTAtus;
		uint16_t u16m0ADCERROrSTAtus;
		uint16_t u16ENABlePWM;
		int16_t s16VOLtageREFerence;
		int16_t s16POSitiveCURrentLIMit;
		int16_t s16NEGativeCURrentLIMit;
		int16_t s16OVERSOUrceCURrentPROtection;
		int16_t s16OVERSINkCURrentPROtection;
		int16_t s16OVERVOLtageOUTPutPROtection;
		int16_t s16OVERVOLtageINPUtPROtection;
		int16_t s16MEAsureTEMperature;
		int16_t s16MEAsureVOLtageOUTPut;
		int16_t s16MEAsureVOLtageINPUt;
		int16_t s16MEAsureCURrentOUTPut;
		int16_t s16MEAsureINDUctorCURrent;
		uint16_t u16ADVAncePARameterLENgth;
		uint16_t u16ADVAncePASsword;
		float32_t f32VOLtageLOOpkp;
		float32_t f32VOLtageLOOpki;
		float32_t f32CURrentLOOpkp;
		float32_t f32CURrentLOOpki;
		float32_t f32OPENLOOpGAIn;
		uint32_t u32HEArtbeatC28;
		uint32_t u32HEArtbeatCLA;
		uint32_t u32rdMODe;
		uint16_t u16HARdwareSLEwrateup;
		uint16_t u16HARdwareSLEwrateDOWn;
		uint16_t u16CALibrationCOMmand;
		uint16_t u16CALibrationACK;
		float32_t f32CALibrationDATa;
		float32_t f32CALibrationRETurn;
		uint16_t u16RECordWORkingSTAtus;
		uint16_t u16RECordERROrSTAtus;
		int16_t s16BROwninVOLtage;
		int16_t s16BROwnOUTVOLtage;
		float32_t f32GAInENABle;
		float32_t f32viGAIn;
		uint32_t u32ENABleCANbusMASter;
		float32_t f32C28ISRCOStTIMe;
		float32_t f32C28ISRMAXCOStTIMe;
		int16_t s16BUCkVOUtMONitor1;
		int16_t s16BUCkVOUtMONitor2;
		int16_t s16BUCkVOUtMONitor3;
		int16_t s16BUCkIOUTMONitor1;
		int16_t s16BUCkIOUTMONitor2;
		int16_t s16BUCkIOUTMONitor3;
		uint16_t u16onDELayMSEc;
		uint16_t u16OFFDELayMSEc;
    }; 
} REG_MBUSDATA;
extern REG_MBUSDATA regMbusData;
extern int chkValidAddress(uint16_t addr);
extern uint16_t getModbusData(uint16_t addr);
extern uint16_t setModbusData(uint16_t addr, uint16_t data);




#endif /* MBCMD_H_ */

