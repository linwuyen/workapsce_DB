/*===========================================================================================

    File Name       : MD.h

    Version         : V1_00_a

    Built Date      : 2017/07/31

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2017/07/31 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_MD_H
#define TM_MD_H
#include "windows.h"
#include "tmString.h"
#include "tmDriverCom.h"
#include "tmHMIProcess.h"
#include "tmUART.h"
#include "tmPEL2.h"
#include "tmPELB.h"
#include "tmPEL.h"
#include "tmPHEL.h"
#include <vector>
#include <process.h>
#include <sstream>
#include <vcclr.h>

using namespace System;
using namespace IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace std;

#ifndef MODEL_INDEX_AUTO
#define MODEL_INDEX_AUTO		0
#endif

#ifndef NETIO_NUM
#define NETIO_NUM		8
#endif

#ifndef INIT_FILE_NAME
#define INIT_FILE_NAME		L"Init.txt"
#endif

#ifndef DRIVERLIST_FOLDER_NAME
#define DRIVERLIST_FOLDER_NAME		L"DRIVERLIST"
#endif

#define DRIVER_PROTOCOL_RS232		MB_SLV_ASCII

#define MODBUS_MAX_REG		256

#define FW_VERSION_CONST			100

#define VERSION_LENGTH_CONST		4

#define TSBT_CONFIG_SIZE_NORMAL		29
#define TSBT_CONFIG_SIZE_SMALL		25

#define LOCATION_OFFSET_X			100
#define LOCATION_OFFSET_Y			100

#define TIME_CONST_WAIT_RESET_STABLE	500
#define TIME_CONST_WAIT_RELOAD_HWEEP	500
#define TIME_CONST_WAIT_RELOAD_FWEEP	500

enum{
	TM_DATABITS_7BITS	= 0,
	TM_DATABITS_8BITS,
	TM_DATABITS_NUM
};
static int Const_Table_DataBits[ TM_DATABITS_NUM ] = { 7, 8 };

enum{
	TM_PARITY_NONE	= 0,
	TM_PARITY_EVEN,
	TM_PARITY_ODD
};

enum{
	TM_STOPBITS_1BITS	= 0,
	TM_STOPBITS_2BITS
};

enum{
	MAINPAGE_PARAMETER	= 0,
	MAINPAGE_MONITOR,
	MAINPAGE_DIAGNOSIS,
	MAINPAGE_MULTI_DRIVE,
	MAINPAGE_HISTORY,
	MAINPAGE_NUM
};

enum{
	ACCESS_LEVEL_0 = 0,
	ACCESS_LEVEL_1,
	ACCESS_LEVEL_2,
	ACCESS_LEVEL_3,
	ACCESS_LEVEL_NUM
};

enum{
	APPLY_STATE_IDLE = 0,
	APPLY_STATE_PREPARE_CONFIG,
	APPLY_STATE_PREPARE_REPOWER,
	APPLY_STATE_DONE_CONFIG,
	APPLY_STATE_DONE_REPOWER,
	APPLY_STATE_NUM
};

enum{
	EXPORT_STATE_IDLE = 0,
	EXPORT_STATE_LOAD_PA,
	EXPORT_STATE_CHECK_RANGE,
	EXPORT_STATE_RUN,
	EXPORT_STATE_DONE,
	EXPORT_STATE_CANCEL,
	EXPORT_STATE_NUM
};

#define PASSWORD		L"225"
#define AL0_TEXT		L"AL0"
#define AL1_TEXT		L"AL1"
#define AL2_TEXT		L"AL2"
#define AL3_TEXT		L"AL3"

enum{
	READ_DATA_TYPE_PARAMETER = 0,
	READ_DATA_TYPE_DYNAMIC,
	READ_DATA_TYPE_MONITOR,
	READ_DATA_TYPE_HISTORY,
	READ_DATA_TYPE_HWINFOR,
	READ_DATA_TYPE_NET_X,
	READ_DATA_TYPE_IMPORT_PA,
	READ_DATA_TYPE_DIAGNOSIS,
	READ_DATA_TYPE_WATCH_PAGE,
	READ_DATA_TYPE_FORFUN,
	READ_DATA_TYPE_NUM
};

enum{
	FWEEP_TYPE_PEL	= 0,
	FWEEP_TYPE_PEL2,
	FWEEP_TYPE_PELB,
	FWEEP_TYPE_NUM
};

typedef struct{


	//==
	int					ComState;
	Struct_DriverCom	DriverCom;

	Struct_TMUART*		Uart;

	//
	vector<string>		Message;

	Struct_PEL2			SavedPEL2;
	Struct_PEL			SavedPEL;

	Struct_PELB			TargetPELB;
	Struct_PEL2			TargetPEL2;
	Struct_PEL			TargetPEL;
	Struct_PHEL			TargetPHEL;

	//int					IsPEL2;
	int					FWEEP_Type;
	int					ToLoadValue;
	int					ToLoadDef;
	int					ToLoadMax;
	int					ToLoadMin;

	int					CurrentReg[ MODBUS_MAX_REG ][ MODBUS_MAX_REG ];	// For modbus, there are 256 * 256 registers
	int					TargetReg[ MODBUS_MAX_REG ][ MODBUS_MAX_REG ];
	int					SavedReg[ MODBUS_MAX_REG ][ MODBUS_MAX_REG ];

	//

	int		PHEL_Result;
	int		FWEEP_Result;

	int		TimeOut_Flag;
	int		Error_Flag;

	int		ProgressBarValue;
	int		Finished_Cnt;

	// The following variables are related to form item

	int		ParameterData_Item;		// Parameter Item
	int		ParameterData_SubItem;	// Parameter Subitem

	int		FWEEP_Item_Index;
	int		PHEL_Item_Index;

	int		ToConfigDriver;
	int		ToResetDriver;

	int		Read_Data_Type;
	int		Read_Data_Page;

	int		To_Export_Parameter;
	int		To_Read_All_Parameter;

	int		Export_State;
	int		Export_ErrorIndex_H;
	int		Export_ErrorIndex_L;

	wstring	ComName;
	int		ComIndex;
	int		BaudRate;

	int		Protocol;
	int		IsRS485;
	int		SlaveID;
	wstring	HWEEP_Path;
	wstring	FWEEP_Path;

	//
	SYSTEMTIME	Time_Stamp;
	SYSTEMTIME	Current_Time;
	SYSTEMTIME	Start_Time;

}Struct_MD;

/*===========================================================================================
    Function Name    : tmVariableInitial_MD
    Input            : Null 
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmVariableInitial_MD ( void );

/*===========================================================================================
    Function Name    : tmMD_ClearCurrentReg
    Input            : Null 
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_ClearCurrentReg ( int index );

/*===========================================================================================
    Function Name    : tmMD_TimerProcess
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_TimerProcess ( void );


/*===========================================================================================
    Function Name    : tmMD_UARTRead
    Input            : address_h
					   address_l
					   num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UARTRead ( int address_h, int address_l, int num );

/*===========================================================================================
    Function Name    : tmMD_UARTWrite
    Input            : address_h
					   address_l
					   value
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UARTWrite ( int address_h, int address_l, int value );

/*===========================================================================================
    Function Name    : tmMD_UARTWriteMultiple
    Input            : address_h
					   address_l
					   num
					   data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UARTWriteMultiple ( int address_h, int address_l, int num, int* data );

/*===========================================================================================
    Function Name    : tmMD_UARTWriteMultiple_Char
    Input            : address_h
					   address_l
					   num
					   data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UARTWriteMultiple_Char ( int address_h, int address_l, int num, int* data );

/*===========================================================================================
    Function Name    : tmMD_UARTCheckFrame
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
int tmMD_UARTCheckFrame ( void );

/*===========================================================================================
    Function Name    : tmMD_UART_GetReceiveFC
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UART_GetReceiveFC ( void );

/*===========================================================================================
    Function Name    : tmMD_UART_ReceivedData_Process
    Input            : dst
					   number
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UART_ReceivedData_Process ( int *dst, int number );

/*===========================================================================================
    Function Name    : tmMD_UART_ReceivedChar_Process
    Input            : dst
					   number
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UART_ReceivedChar_Process ( char *dst, int number );

/*===========================================================================================
    Function Name    : tmMD_UART_Exception_Process
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UART_Exception_Process ( void );

/*===========================================================================================
    Function Name    : tmMD_CalculateTimeCost
    Input            :	Start_Time
						End_Time
    Return           : Cost time ( ms )
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
unsigned __int64 tmMD_CalculateTimeCost ( SYSTEMTIME	Start_Time, SYSTEMTIME	End_Time );

/*===========================================================================================
    Function Name    : tmSaveResult
    Input            : test_state ( 1 = good, 0 = bad )
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmSaveResult( int test_state );

#endif

/************************** <END OF FILE> *****************************************/

