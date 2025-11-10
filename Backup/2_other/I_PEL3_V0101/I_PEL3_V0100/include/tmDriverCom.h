/*===========================================================================================

    File Name       : tmDriverCom.h

    Version         : V1_00_a

    Built Date      : 2018/01/05

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/01/05 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_DRIVER_COM_H
#define TM_DRIVER_COM_H
#include "windows.h"
#include "tmString.h"

#define DRIVER_ID_RS232			17

#ifndef PARAMETER_MAJOR_NUM_CODE_83
#define PARAMETER_MAJOR_NUM_CODE_83  					6
#endif

#ifndef PARAMETER_MAJOR_NUM_CODE_84
#define PARAMETER_MAJOR_NUM_CODE_84  					9
#endif

#ifndef PARAMETER_MAJOR_NUM_CODE_85
#define PARAMETER_MAJOR_NUM_CODE_85  					9
#endif

#ifndef PARAMETER_MAJOR_NUM_CODE_86
#define PARAMETER_MAJOR_NUM_CODE_86 					9
#endif

#ifndef PARAMETER_MINOR_NUM_CODE_83
#define PARAMETER_MINOR_NUM_CODE_83    					14
#endif

#ifndef PARAMETER_MINOR_NUM_CODE_84
#define PARAMETER_MINOR_NUM_CODE_84    					16
#endif

#ifndef PARAMETER_MINOR_NUM_CODE_85
#define PARAMETER_MINOR_NUM_CODE_85    					16
#endif

#ifndef PARAMETER_MINOR_NUM_CODE_86
#define PARAMETER_MINOR_NUM_CODE_86    					32
#endif

#define DRIVER_VER_EQ_DATA_NUM  11
#define FW_VER_CHARS			28			// This value must be times of 4
#define PART_NUM_CHARS			28			// This value must be times of 4
#define SN_CHARS				16			// This value must be times of 4

#define DRIVER_VER_HW_LENGTH	16


#define  EXCEPTION_CODE_MEANING_TEXT		L"1 = Illegal Function, 2 = Illegal Address ( or Illegal AGV Format ), 3 = Illegal Value ( or Illegal AGV SubID Num ), 4 = Device Failure, 6 = Device Busy, 9 = Format Error"  


//! Code Ver defines
enum {
	CV_COM = 0,
	CV_HWE = 1,
	CV_FWE = 2,
	CV_HWV = 3,
	CV_FWV = 4,
	CV_NUM = 5
};

/*
COM Code
83	Parameter = 6 * 14¡AHW and FW 2 in 1
84	Parameter = 9 * 16¡AHW and FW 2 in 1
85	Parameter = 9 * 16¡AHW and FW are different address
*/
enum{
	COM_CODE_83 = 83,
	COM_CODE_84 = 84,
	COM_CODE_85 = 85
};

enum{
	TM_PARAMETER_TYPE_UNKNOWN = 0,
	TM_PARAMETER_TYPE_6X14,
	TM_PARAMETER_TYPE_9X16,
	
	TM_PARAMETER_TYPE_NUM
};

#define FWMODEL_LENGTH		7
#define VERSION_LENGTH		4

/*===========================================================================================
   Parameter Major macros
//==========================================================================================*/
enum{

	PARAMETER_MOTOR			    = 0,    // motor and Engineer parameter
	PARAMETER_GENERAL	 		= 1,	// general
	PARAMETER_SPEED		 	    = 2,	// speed and VR output range
	PARAMETER_ACC_DEC	        = 3,	// acc/dec
	PARAMETER_PROTECT			= 4,	// protect
    PARAMETER_IO                = 5,	// IO
	PARAMETER_DIGITAL			= 6,    // Digital Torque and duty
	PARAMETER_ADVANCE_FUNC		= 7,    // Adcanve function
	PARAMETER_COM				= 8,    // Communication
	PARAMETER_TOTALNUM	        = 9

};

/*===========================================================================================
	Protection ( major index = 8 )
	parameter ID
	Communication Region Parameters
//==========================================================================================*/
enum{
	COM_PA_X1_FUNC							= 0,
	COM_PA_X2_FUNC							= 1,
	COM_PA_X3_FUNC							= 2,
	COM_PA_X4_FUNC							= 3,
	COM_PA_X5_FUNC							= 4,
	COM_PA_X6_FUNC							= 5,
	COM_PA_X7_FUNC							= 6,
	COM_PA_X8_FUNC							= 7
	
};

// Communication Index Constant
enum{
	COM_HW_INFO_INDEX2				= 131,	// HW Info region.
	COM_HW_SN_0_INDEX				= 130,
	COM_HW_INFO_INDEX				= 129,
	COM_CODE_VER_INDEX				= 128,
	COM_MASTER_LV_INDEX				= 100,
	COM_DRIVER_VER_INDEX			= 99,
	COM_FW_VER_INDEX				= 98,
	//COM_MAXMIN_REQ_INDEX 			= 40,

	COM_DYNAMIC_DATA_INDEX			= 0,
	COM_PA_EEP_INDEX				= 1,	// from 1 to 9, 0 is reserved for dynamic data for old protocol.
	COM_DEFAULT_EEP_INDEX 			= 11,	// from 11 to 19, 10 is reserved for command type for old protocol.
    COM_MAX_EEP_INDEX 				= 21,	// from 21 to 29
    COM_MIN_EEP_INDEX 				= 31,	// from 31 to 39
	COM_MAXMIN_EEP_INDEX			= 41,	// from 41 to 49 special request reserved.
	COM_RO_EEP_INDEX				= 51,	// from 51 to 59 is Read only EEP region.

	// RAM only
	COM_PA_RAM_INDEX				= 61,	// from 61 to 69 parameter RAM_EEP region.
	COM_MONITOR_DATA_USER_INDEX		= 70,	// Monitor data for user
	COM_RO_RAM_INDEX				= 71,	// from 71 to 79 Read Only RAM region.
	COM_MONITOR_DATA_ENG_INDEX		= 73,	// Monitor data for engineer.

	COM_COMMAND_INDEX				= 10,	// Command type region.
	COM_UART_XIN_INDEX				= 20,   // Uart Input Xn region.
	COM_UART_YOUT_INDEX				= 30    // Uart Output Yx region.
};

enum{
    ILLEGAL_FUNCTION            = 0x01,
    ILLEGAL_DATA_ADDRESS        = 0x02,
    ILLEGAL_DATA_VALUE          = 0x03,

	ILLEGAL_AGV_SUBID_NUM       = 0x03,
	ILLEGAL_AGV_FORMAT      	= 0x02,

    SLAVE_DEVICE_FAILURE        = 0x04,

    SLAVE_DEVICE_BUSY           = 0x06,

    FORMAT_ERROR                = 0x09
};

// Net IO
enum{
	NET_IO_X0 = 0,
	NET_IO_X1,
	NET_IO_X2,
	NET_IO_X3,
	NET_IO_X4,
	NET_IO_X5,
	NET_IO_X6,
	NET_IO_X7,
	NET_IO_NUM
};

// Command defines.
enum {
	COM_CMD_ALARM_RESET				= 0,
	
	COM_CMD_PA_RESET				= 32,	// 32 +0
	COM_CMD_IO_RESET				= 33,   // 32 +1
	COM_CMD_ERROR_HISTORY_RESET		= 34,   // 32 +2
	
	COM_CMD_LOAD_ALL_PA_START       = 35,	// 32 +3	// Reserved.
	COM_CMD_LOAD_ALL_PA_DONE		= 36,	// 32 +4    // Reserved.
	
	COM_CMD_WARN_HISTORY_RESET		= 37,	// 32 +5
	COM_CMD_COM_HISTORY_RESET		= 38,	// 32 +6
	COM_CMD_CONFIG					= 39,   // 32 +7
	COM_CMD_ALL_EEP_TO_RAM			= 40,   // 32 +8
	COM_CMD_ALL_RAM_TO_EEP			= 41,   // 32 +9

	COM_CMD_RELOAD_FWEEP			= 42,   // 32 +10
	COM_CMD_RELOAD_HWEEP			= 43    // 32 +11
};

// MasterLevel defines
enum{
	MASTER_LEVEL_0			= 0,
	MASTER_LEVEL_1			= 1,
	MASTER_LEVEL_2		   	= 2,
	MASTER_LEVEL_3			= 3,
	MASTER_LEVEL_PASSWROD	= 123
};


enum{
	PA_SUBITEM_VALUE = 0,
	PA_SUBITEM_DEFAULT,
	PA_SUBITEM_MAX,
	PA_SUBITEM_MIN,
	PA_SUBITEM_RAM,
	PA_SUBITEM_NUM
};


static const int Const_Pa_SubItem_Table[ PA_SUBITEM_NUM ] = 
{ COM_PA_EEP_INDEX - 1, COM_DEFAULT_EEP_INDEX - 1, COM_MAX_EEP_INDEX - 1, COM_MIN_EEP_INDEX - 1, COM_PA_RAM_INDEX - 1 };



typedef struct{
	
	int			Code[ CV_NUM ];
	string		HW_Version;
	string		FW_Version;
	string		SN;

	int			ParameterType;	// 0 = 6 * 14, 1 = 9 * 16

	int			FirstItemIndex;		// First item showed in Parameter listview
	int			LastItemIndex;		// Last item showed in Parameter listview
	int			FirstItemAddress;
	int			LastItemAddress;

	int			WatchPage_Address_H;

	// Read process:
	// First:  Value => Def => Max => Min => Ram =>
	// Next:   Value => Def => Max => Min => Ram =>
	// ...
	// Last:   Value => Def => Max => Min => Ram =>
	// First:  Value => Def => Max => Min => Ram =>
	int			CurrentRead_Item;
	int			CurrentRead_SubItem_Index;
	int			CurrentRead_Address;

	int			WriteFalg;
	int			WriteAddress;
	int			WriteValue;

	int			CurrentWrite_Item;

}Struct_DriverCom;
#endif

/************************** <END OF FILE> *****************************************/

