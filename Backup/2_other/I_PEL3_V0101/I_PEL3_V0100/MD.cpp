/*===========================================================================================

    File Name       : MD.cpp

    Version         : V1_00_a

    Built Date      : 2017/07/31

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2017/07/31 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */
#include "stdafx.h"
#include "MD.h"

Struct_MD							CG_MD;
extern Struct_TMUART				CG_UART;
extern Struct_HMIProcess			CG_HMIProcess;

/*===========================================================================================
    Function Name    : tmVariableInitial_MD
    Input            : Null 
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmVariableInitial_MD ( void )
{
	CG_MD.ComState = STATE_CLOSED;
	

	CG_MD.To_Export_Parameter = 0;
	CG_MD.ToResetDriver = 0;
	CG_MD.Export_State = EXPORT_STATE_IDLE;
	CG_MD.ProgressBarValue = 0;
	CG_MD.Finished_Cnt = 0;

	for( int i = 0; i < MODBUS_MAX_REG; i++  ){
		for( int j = 0; j < MODBUS_MAX_REG; j++  ){
			CG_MD.CurrentReg[i][j] = 0;
			CG_MD.TargetReg[i][j] = 0;
		}
	}

	//==
	CG_MD.Uart = &CG_UART;

	tmUART_VariableInitial ( CG_MD.Uart );
	tmVariableInitial_HMIProcess();

}

/*===========================================================================================
    Function Name    : tmMD_ClearCurrentReg
    Input            : Null 
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_ClearCurrentReg ( int index )
{
	for( int i = 0; i < MODBUS_MAX_REG; i++  ){
		CG_MD.CurrentReg[index][i] = 0;
	}
}

/*===========================================================================================
    Function Name    : tmMD_TimerProcess
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_TimerProcess ( void )
{
	if( CG_MD.ComState == STATE_OPENED && CG_MD.Uart->ReceiveFlag == UART_BUSY ){

		tmUART_CheckRTUframe( CG_MD.Uart );

		if( ++CG_MD.Uart->current_time > TIME_OUT_CONST_MAX ){
			CG_MD.Uart->current_time = TIME_OUT_CONST_MAX;
		}
		if( CG_MD.Uart->current_time > TIME_OUT_CONST ){

			CG_MD.Uart->Timeout_Flag = 1;
			CG_HMIProcess.State = HMIPROCESS_STATE_TIMEOUT;
			CG_HMIProcess.Error_Index = ERROR_INDEX_TIMEOUT;

		}
	}else{
		CG_MD.Uart->current_time = 0;
		CG_MD.Uart->Timeout_Flag = 0;
	}
}

/*===========================================================================================
    Function Name    : tmMD_UARTRead
    Input            : address_h
					   address_l
					   num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UARTRead ( int address_h, int address_l, int num )
{
	if( CG_MD.IsRS485 ){
		CG_MD.Uart->Request_ID = CG_MD.SlaveID;
	}else{
		CG_MD.Uart->Request_ID = DRIVER_ID_RS232;
	}

	if( CG_MD.Protocol == MB_SLV_ASCII ){
		tmUART_Modbus_FC3_Master_ASCII( CG_MD.Uart, CG_MD.Uart->Request_ID, address_h, address_l, num );
	}else{
		tmUART_Modbus_FC3_Master_RTU( CG_MD.Uart, CG_MD.Uart->Request_ID, address_h, address_l, num );
	}
}

/*===========================================================================================
    Function Name    : tmMD_UARTWrite
    Input            : address_h
					   address_l
					   value
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UARTWrite ( int address_h, int address_l, int value )
{
	if( CG_MD.IsRS485 ){
		CG_MD.Uart->Request_ID = CG_MD.SlaveID;
	}else{
		CG_MD.Uart->Request_ID = DRIVER_ID_RS232;
	}

	if( CG_MD.Protocol == MB_SLV_ASCII ){
		tmUART_Modbus_FC6_Master_ASCII( CG_MD.Uart, CG_MD.Uart->Request_ID, address_h, address_l, value );
	}else{
		tmUART_Modbus_FC6_Master_RTU( CG_MD.Uart, CG_MD.Uart->Request_ID, address_h, address_l, value );
	}
}

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
void tmMD_UARTWriteMultiple ( int address_h, int address_l, int num, int* data )
{
	if( CG_MD.IsRS485 ){
		CG_MD.Uart->Request_ID = CG_MD.SlaveID;
	}else{
		CG_MD.Uart->Request_ID = DRIVER_ID_RS232;
	}
	if( CG_MD.Protocol == MB_SLV_ASCII ){
		tmUART_Modbus_FC16_Master_ASCII( CG_MD.Uart, CG_MD.Uart->Request_ID, address_h, address_l, num, data );
	}else{
		tmUART_Modbus_FC16_Master_RTU( CG_MD.Uart, CG_MD.Uart->Request_ID, address_h, address_l, num, data );
	}
}

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
void tmMD_UARTWriteMultiple_Char ( int address_h, int address_l, int num, int* data )
{
	if( CG_MD.IsRS485 ){
		CG_MD.Uart->Request_ID = CG_MD.SlaveID;
	}else{
		CG_MD.Uart->Request_ID = DRIVER_ID_RS232;
	}
	if( CG_MD.Protocol == MB_SLV_ASCII ){
		tmUART_Modbus_FC16_Master_ASCII_CHAR( CG_MD.Uart, CG_MD.Uart->Request_ID, address_h, address_l, num / 4, data );
	}else{
		tmUART_Modbus_FC16_Master_RTU_CHAR( CG_MD.Uart, CG_MD.Uart->Request_ID, address_h, address_l, num / 2, data );
	}
}

/*===========================================================================================
    Function Name    : tmMD_UARTCheckFrame
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
int tmMD_UARTCheckFrame ( void )
{
	int frame_is_good;
	if( CG_MD.Protocol == MB_SLV_ASCII ){
		frame_is_good = tmUART_CheckASCII_LRC( CG_MD.Uart->R_Buffer, CG_MD.Uart->R_data_number );
	}else{
		frame_is_good = tmUART_CheckRTU_CRC( CG_MD.Uart->R_Buffer, CG_MD.Uart->R_data_number );
	}
	return frame_is_good;
}

/*===========================================================================================
    Function Name    : tmMD_UART_GetReceiveFC
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UART_GetReceiveFC ( void )
{
	if( CG_MD.Protocol == MB_SLV_ASCII ){	
		CG_MD.Uart->ReceiveFC = tmUART_ASCII2UNCHAR( CG_MD.Uart->R_Buffer[ 3 ], CG_MD.Uart->R_Buffer[ 4 ] );
	}else{
		CG_MD.Uart->ReceiveFC = CG_MD.Uart->R_Buffer[ 1 ];
	}
}

/*===========================================================================================
    Function Name    : tmMD_UART_ReceivedData_Process
    Input            : dst
					   number
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UART_ReceivedData_Process ( int *dst, int number )
{
	if( CG_MD.Protocol == MB_SLV_ASCII ){	
		tmUART_ReceivedData_Process_ASCII( CG_MD.Uart, dst, number );
	}else{
		tmUART_ReceivedData_Process_RTU( CG_MD.Uart, dst, number );
	}
}

/*===========================================================================================
    Function Name    : tmMD_UART_ReceivedChar_Process
    Input            : dst
					   number
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UART_ReceivedChar_Process ( char *dst, int number )
{
	if( CG_MD.Protocol == MB_SLV_ASCII ){	
		tmUART_ReceivedChar_Process_ASCII( CG_MD.Uart, dst, number );
	}else{
		tmUART_ReceivedChar_Process_RTU( CG_MD.Uart, dst, number );
	}
}

/*===========================================================================================
    Function Name    : tmMD_UART_Exception_Process
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmMD_UART_Exception_Process ( void )
{
	if( CG_MD.Protocol == MB_SLV_ASCII ){	
		CG_MD.Uart->ExceptionCode = tmUART_ASCII2UNCHAR( CG_MD.Uart->R_Buffer[ 5 ], CG_MD.Uart->R_Buffer[ 6 ] );
	}else{
		CG_MD.Uart->ExceptionCode = CG_MD.Uart->R_Buffer[ 2 ];
	}
}

/*===========================================================================================
    Function Name    : tmMD_CalculateTimeCost
    Input            :	Start_Time
						End_Time
    Return           : Cost time ( ms )
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
unsigned __int64 tmMD_CalculateTimeCost ( SYSTEMTIME	Start_Time, SYSTEMTIME	End_Time )
{
	unsigned __int64 Cost_Time;
	ULARGE_INTEGER FileTime_Start;
	ULARGE_INTEGER FileTime_End;

	SystemTimeToFileTime( &Start_Time, (_FILETIME*)&FileTime_Start ); 
	SystemTimeToFileTime( &End_Time, (_FILETIME*)&FileTime_End ); 

	Cost_Time = ( FileTime_End.QuadPart - FileTime_Start.QuadPart ) / 10000;
			
	return Cost_Time;
}

/*===========================================================================================
    Function Name    : tmSaveResult
    Input            : test_state ( 1 = good, 0 = bad )
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmSaveResult( int test_state )
{
	
	StreamWriter^ writefile = gcnew StreamWriter( "Results.txt", 1 );
	String^ sn = gcnew String( CG_MD.DriverCom.SN.c_str() );
	String^ hw = gcnew String( CG_MD.DriverCom.HW_Version.c_str() );
	String^ fw = gcnew String( CG_MD.DriverCom.FW_Version.c_str() );
	String^ result;
	String^ display;
	String^ hw_path;
	String^ fw_path = "";

	if( test_state ){
		result = "Ok";
	}else{
		result = "Fail";
	}

	if( CG_MD.HWEEP_Path != L"" ){
		hw_path = gcnew String( CG_MD.HWEEP_Path.c_str() );
	}else{
		hw_path = "µLHWEEP";
	}

	if( CG_MD.FWEEP_Path != L"" ){

		if( CG_MD.ToLoadValue ){
			fw_path += "Value, ";
		}
		if( CG_MD.ToLoadDef ){
			fw_path += "Def, ";
		}
		if( CG_MD.ToLoadMax ){
			fw_path += "Max, ";
		}
		if( CG_MD.ToLoadMin ){
			fw_path += "Min, ";
		}

		fw_path += gcnew String( CG_MD.FWEEP_Path.c_str() );

	}else{
		fw_path = "µLFWEEP";
	}

	SYSTEMTIME time;
	GetLocalTime( &time );
				
	String^ datetime =	Convert::ToString( time.wYear ) + "." +
						Convert::ToString( time.wMonth ) + "." +
						Convert::ToString( time.wDay ) + "." +
						Convert::ToString( time.wHour ) + "." +
						Convert::ToString( time.wMinute );

	display = datetime + "	" +
		sn + "	" +
		hw + "	" +
		fw + "	" +
		result + "	" +
		hw_path + "	" +
		fw_path;

	writefile->WriteLine( display );

	writefile->Close();
}

/************************** <END OF FILE> *****************************************/