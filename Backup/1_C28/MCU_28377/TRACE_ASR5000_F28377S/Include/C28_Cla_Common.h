/*
 * C28_Cla_Common.h
 *
 *  Created on: Nov 13, 2023
 *      Author: cody_chen
 */

#ifndef INCLUDE_C28_CLA_COMMON_H_
#define INCLUDE_C28_CLA_COMMON_H_



//
// 預設機種 (以下擇一)
//
#define DEFAULT_ASR_5075
//#define DEFAULT_ASR_5150


//#define DEBUG_ON
//PC RS232顯示Debug資料


//
// 測試用的搭配版本
// !!!要注意也會影響執行資源!!!
//

//#define DEVELOP_SV01
//#define DEVELOP_SV01 (CLA_Module.h)
//#define DEVELOP_SV01 (CLA_Task.cla)
//for CCS 量測執行cycle


//#define BYPASS_SCAN_ERRROR_WHEN_POWER_UP
//#define BYPASS_GEN_WAVE_TO_SDRAM
//#define BYPASS_SCAN_ERROR_FOR_POWER_READY
//#define BYPASS_CAL_CV_CC_CP_COMPENSATE
//#define BYPASS_LOAD_AND_CHECK_EEPROM
#define CHECK_THE_CHANGE_SET_USER()  //GpioDataRegs.GPBDAT.bit.GPIO53 = (cpu_timer_module.Value_Set_User_Change == 1);

//#define DEVELOP_SV39
// Temp拿來顯示Debug


#define DEVELOP_SV41
// 關閉MS補償
// MS_Balance_Compensate_Enable = 0
// VREF_Compensate_Offset_2 = 0


//#define DEVELOP_SV42
// 本機為Master

//#define DEVELOP_SV43
// 預設MS_Enable = 1, 1P2W(2 Unit)


#define DEVELOP_SV48
// MS連線不檢查FW版本



#define DEVELOP_SV51
// Bypass SDRAM同步


#define DEVELOP_SV52
// Bypass DCOVP



#define DEVELOP_SV54
// MS模式下, 不執行逆電流卸載補償
// 考慮到多機連線下, 會有電流互灌的風險, 所以Bypass



//#define DEVELOP_SV59
// Bypass ACFAIL的數位濾波處理

//#define DEVELOP_SV60
// Bypass ACFAIL相關NG檢查
// PS. 5150,5151機種會無法關機

//#define DEVELOP_SV61
// SYNC LINE模式下 Bypass硬體NG檢查



//#define DEVELOP_SV69
// 使用sampling_state來擷取debug資料


//#define BYPASS_ACFAIL

//#define DEVELOP_SV72
// Bypass校正檢查Cs, Rs及XT offset
// 單板測試校正使用


//#define DEVELOP_SV73
// 測試自動校正高檔, 把電壓降低


//#define DEVELOP_SV74
// 把CF, Q display拿來顯示Debug


//#define DEVELOP_SV75
// Master ACV=77V 觸發SDRAM同步測試


#define DEVELOP_SV76
// AM3352還沒更新前, 先使用此修改
// #define SQU_Duty_Set_spi_addr                       0x0947
// #define V_Rise_Time_Set_MSB_spi_addr                0x0948
// #define V_Rise_Time_Set_LSB_spi_addr                0x0949


//#define DEVELOP_SV77
// (.cla)
// CLA的DDS獨立計算
// 目前實測需要同步, 不然ATE測試高頻RMS會NG
// 這裡是因為MS Slave端有問題還要解決所以先切換過去


//#define DEVELOP_SV78
// AM3352還沒更新前, 先使用此修改
// Bypass SEQ_CheckSum_Refresh_spi_addr
// SEQ CheckSum計算方式維持不變


//
// EXTIO pin-8用來Debug使用
//
// Step 1.
//#define DEVELOP_SV62
// Bypass bit.UNDEFINED_OUT1 = 0

// Step 2. ( 以下擇一 )
//#define DEVELOP_SV58
// for SYNC LINE / SYNC EXT / EXT
//#define DEVELOP_SV63
// for SEQ



//
// 單板測試使用
//
//#define BYPASS_SCAN_ERRROR_WHEN_POWER_UP
//#define BYPASS_GEN_WAVE_TO_SDRAM
//#define BYPASS_SCAN_ERROR_FOR_POWER_READY
//#define BYPASS_CAL_CV_CC_CP_COMPENSATE
//#define BYPASS_LOAD_AND_CHECK_EEPROM //( 如果只有5V供電 )
//#define BYPASS_ACFAIL
//#define DEVELOP_SV72

#endif /* INCLUDE_C28_CLA_COMMON_H_ */
