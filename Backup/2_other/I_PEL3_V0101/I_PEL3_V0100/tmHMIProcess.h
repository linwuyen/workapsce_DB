/*===========================================================================================

    File Name       : tmHMIProcess.h

    Version         : V1_00_a

    Built Date      : 2018/01/08

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/01/08 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_HMIPROCESS_H
#define TM_HMIPROCESS_H
#include "windows.h"
#include "tmString.h"

enum {
	HMIPROCESS_STATE_IDLE				= 0,
	HMIPROCESS_STATE_PREPARE,

	HMIPROCESS_STATE_LOADHWEEP,
	HMIPROCESS_STATE_LOADFWEEP,

	HMIPROCESS_STATE_GET_DRIVER_INFOR,

	HMIPROCESS_STATE_DONE,
	
	HMIPROCESS_STATE_TIMEOUT,
	HMIPROCESS_STATE_ERROR,
	HMIPROCESS_STATE_NUM

};


enum{
	ERROR_INDEX_NO_ERROR = 0,
	ERROR_INDEX_TIMEOUT,
	ERROR_INDEX_FRAME,
	ERROR_INDEX_CMD_DENY,
	ERROR_INDEX_OTHERS,
	ERROR_INDEX_NUM
};

static const string Const_Comunication_State[ ERROR_INDEX_NUM ] = { "Fine",	"Timeout", "Frame error", "Command deny" };

#define WRITE_DENY_DISPLAY_INFOR	"資料拒絕寫入,可能原因如下\n1. AL可能被強制清除(232或485timeout功能被啟動,需將之關閉)\n2.其他原因(請RD協助)"

enum{
	DRIVER_INFOR_SUBSTATE_IDLE				= 0,
	DRIVER_INFOR_SUBSTATE_TOGET_COMCODE,
	DRIVER_INFOR_SUBSTATE_TOGET_HWVERSION,
	DRIVER_INFOR_SUBSTATE_TOGET_FWVERSION,
	DRIVER_INFOR_SUBSTATE_TOGET_SN,
	DRIVER_INFOR_SUBSTATE_DONE,
	DRIVER_INFOR_SUBSTATE_NUM
};

enum{

	HWEEP_SUBSTATE_LOAD_FILE = 0,

	HWEEP_SUBSTATE_KEYIN_PW,
	HWEEP_SUBSTATE_WRITE_DATA,
	HWEEP_SUBSTATE_RESET_DRIVER,
	HWEEP_SUBSTATE_RESET_DRIVER_WAIT,
	HWEEP_SUBSTATE_RELOAD_CMD,
	HWEEP_SUBSTATE_RELOAD_CMD_WAIT,
	HWEEP_SUBSTATE_READ_DATA,
	HWEEP_SUBSTATE_VERIFY,

	HWEEP_SUBSTATE_DONE,
	HWEEP_SUBSTATE_NUM
};

enum{

	FWEEP_SUBSTATE_LOAD_FILE = 0,

	FWEEP_SUBSTATE_KEYIN_PW,

	FWEEP_SUBSTATE_WRITE_VALUE,
	FWEEP_SUBSTATE_WRITE_DEF,
	FWEEP_SUBSTATE_WRITE_MAX,
	FWEEP_SUBSTATE_WRITE_MIN,

	FWEEP_SUBSTATE_RESET_DRIVER,
	FWEEP_SUBSTATE_RESET_DRIVER_WAIT,
	FWEEP_SUBSTATE_RELOAD_CMD,
	FWEEP_SUBSTATE_RELOAD_CMD_WAIT,

	FWEEP_SUBSTATE_READ_VALUE,
	FWEEP_SUBSTATE_READ_DEF,
	FWEEP_SUBSTATE_READ_MAX,
	FWEEP_SUBSTATE_READ_MIN,

	FWEEP_SUBSTATE_VERIFY,

	FWEEP_SUBSTATE_DONE,
	FWEEP_SUBSTATE_NUM
};

typedef struct{
	
	int		State;
	int		State_Old;
	int		SubState;

	int		Error_Index;

}Struct_HMIProcess;


/*===========================================================================================
    Function Name    : tmVariableInitial_HMIProcess
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmVariableInitial_HMIProcess ( void );

/*===========================================================================================
    Function Name    : tmHMIProcess_State
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_Idle ( void );
void tmHMIProcess_State_Prepare ( void );
void tmHMIProcess_State_GetDriverInfor ( void );

void tmHMIProcess_State_LoadHWEEP( void );
void tmHMIProcess_State_LoadFWEEP( void );

void tmHMIProcess_State_Done ( void );

void tmHMIProcess_State_TimeOut ( void );
void tmHMIProcess_State_Error ( void );

//! Function Call for HMI Process State Machine	
static void ( *const tmHMIProcess_Handler[ HMIPROCESS_STATE_NUM ] ) ( void ) = 
{
	tmHMIProcess_State_Idle,
	tmHMIProcess_State_Prepare,
	
	//
	tmHMIProcess_State_LoadHWEEP,
	tmHMIProcess_State_LoadFWEEP,

	tmHMIProcess_State_GetDriverInfor,

	tmHMIProcess_State_Done,

	tmHMIProcess_State_TimeOut,
	tmHMIProcess_State_Error
};

#endif

/************************** <END OF FILE> *****************************************/

