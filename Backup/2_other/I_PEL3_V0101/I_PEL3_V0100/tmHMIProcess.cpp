/*===========================================================================================

    File Name       : tmHMIProcess.cpp

    Version         : V1_00_a

    Built Date      : 2018/01/08

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/01/08 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */
#include "stdafx.h"
#include "tmHMIProcess.h"
#include "MD.h"
#include "tmUART.h"


Struct_HMIProcess				CG_HMIProcess;
extern Struct_MD				CG_MD;

/*===========================================================================================
    Function Name    : tmVariableInitial_HMIProcess
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmVariableInitial_HMIProcess (void )
{
	CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;
	CG_HMIProcess.SubState = 0;
}


/*===========================================================================================
    Function Name    : tmHMIProcess_State_Idle
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_Idle ( void )
{
	if( CG_MD.ComState == STATE_OPENED ){
		CG_HMIProcess.State = HMIPROCESS_STATE_PREPARE;
	}
}

/*===========================================================================================
    Function Name    : tmHMIProcess_State_Prepare
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_Prepare ( void )
{
	if( CG_MD.ComState == STATE_OPENED ){
		CG_HMIProcess.SubState = 0;
		CG_HMIProcess.State = HMIPROCESS_STATE_LOADHWEEP;
		//CG_HMIProcess.State = HMIPROCESS_STATE_GET_DRIVER_INFOR;
		CG_MD.Uart->ReceiveFlag = UART_READY;
		CG_MD.Uart->get_data_flag = 0;

		CG_MD.TimeOut_Flag = 0;
		CG_MD.Error_Flag = 0;
		CG_MD.ProgressBarValue = 0;

		//
		CG_MD.To_Export_Parameter = 0;
		CG_MD.ToResetDriver = 0;
		CG_MD.Export_State = EXPORT_STATE_IDLE;
		CG_MD.DriverCom.WriteFalg = 0;
		//

		for( int i = 0; i < MODBUS_MAX_REG; i++  ){
			for( int j = 0; j < MODBUS_MAX_REG; j++  ){
				CG_MD.CurrentReg[i][j] = 0;
				CG_MD.TargetReg[i][j] = 0;
			}
		}

		CG_MD.Message.clear();
		CG_MD.Message.push_back( "開始" );
		CG_MD.Message.push_back( "參數初始化" );
		CG_MD.Message.push_back( "-----------------------------------------------" );

		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Start_Time );

	}else{

		CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;
	}
}

/*===========================================================================================
    Function Name    : tmHMIProcess_State_GetDriverInfor
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_GetDriverInfor ( void )
{
	int address = 0;
	int num = 0;
	int recieved_num = 0;
	int i;

	char dummy_data[ 64 ];
	char dummy_data_HW[ 32 ];
	char dummy_data_FW[ 32 ];

	switch( CG_HMIProcess.SubState ){
	case DRIVER_INFOR_SUBSTATE_IDLE:
		CG_HMIProcess.SubState = DRIVER_INFOR_SUBSTATE_TOGET_COMCODE;
		CG_MD.Message.push_back( "讀取驅動器資訊" );
		break;
	case DRIVER_INFOR_SUBSTATE_TOGET_COMCODE:
		if( CG_MD.Uart->ReceiveFlag == UART_READY ){
			tmMD_UARTRead( COM_CODE_VER_INDEX, 0, CV_NUM );
			CG_MD.Message.push_back( "讀取Com Code" );
		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;

			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
				tmMD_UART_ReceivedData_Process ( CG_MD.DriverCom.Code, CV_NUM );
				CG_HMIProcess.SubState = DRIVER_INFOR_SUBSTATE_TOGET_HWVERSION;
				CG_MD.Message.push_back( "Com Code = " + to_string( (long double)CG_MD.DriverCom.Code[ CV_COM ] ) );
			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}
		break;
	case DRIVER_INFOR_SUBSTATE_TOGET_HWVERSION:

		if( CG_MD.DriverCom.Code[ CV_COM ] == COM_CODE_83 ){
			CG_MD.DriverCom.ParameterType = TM_PARAMETER_TYPE_6X14;
			num = DRIVER_VER_EQ_DATA_NUM;
			recieved_num = DRIVER_VER_EQ_DATA_NUM * 4;
		}else if( CG_MD.DriverCom.Code[ CV_COM ] == COM_CODE_84 ){
			CG_MD.DriverCom.ParameterType = TM_PARAMETER_TYPE_9X16;
			num = DRIVER_VER_EQ_DATA_NUM;
			recieved_num = DRIVER_VER_EQ_DATA_NUM * 4;
		}else if( CG_MD.DriverCom.Code[ CV_COM ] == COM_CODE_85 ){
			CG_MD.DriverCom.ParameterType = TM_PARAMETER_TYPE_9X16;
			num = PART_NUM_CHARS / 4;
			recieved_num = PART_NUM_CHARS;
		}else{
			CG_MD.DriverCom.ParameterType = TM_PARAMETER_TYPE_UNKNOWN;
			num = PART_NUM_CHARS / 4;
			recieved_num = PART_NUM_CHARS;
		}

		address = COM_DRIVER_VER_INDEX;
		if( CG_MD.Protocol == MB_SLV_RTU ){
			num *= 2;
		}

		if( CG_MD.Uart->ReceiveFlag == UART_READY ){
			tmMD_UARTRead( address, 0, num );
			CG_MD.Message.push_back( "讀取HW Version" );
		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;

			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
				tmMD_UART_ReceivedChar_Process ( dummy_data, recieved_num );

				if( CG_MD.DriverCom.Code[ CV_COM ] == COM_CODE_83 ||
					CG_MD.DriverCom.Code[ CV_COM ] == COM_CODE_84 ){

					for( i = 0; i < DRIVER_VER_HW_LENGTH; i++ ){
						dummy_data_HW[i] = dummy_data[i];
					}
					dummy_data_HW[i] = '\0';
					CG_MD.DriverCom.HW_Version = dummy_data_HW;

					for( i = 0; i < recieved_num - DRIVER_VER_HW_LENGTH; i++ ){
						dummy_data_FW[i] = dummy_data[ DRIVER_VER_HW_LENGTH + i ];
					}
					dummy_data_FW[i] = '\0';
					CG_MD.DriverCom.FW_Version = dummy_data_FW;
					

					CG_HMIProcess.SubState = DRIVER_INFOR_SUBSTATE_TOGET_SN;
					CG_MD.Message.push_back( "HW Version = " + CG_MD.DriverCom.HW_Version );
					CG_MD.Message.push_back( "FW Version = " + CG_MD.DriverCom.FW_Version );

				}else{
					dummy_data[ recieved_num ] = '\0';
					CG_MD.DriverCom.HW_Version = dummy_data;
					CG_HMIProcess.SubState = DRIVER_INFOR_SUBSTATE_TOGET_FWVERSION;
					CG_MD.Message.push_back( "HW Version = " + CG_MD.DriverCom.HW_Version );
				}	
			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}

		break;

	case DRIVER_INFOR_SUBSTATE_TOGET_FWVERSION:

		num = FW_VER_CHARS / 4;
		recieved_num = FW_VER_CHARS;
		if( CG_MD.Protocol == MB_SLV_RTU ){
			num *= 2;
		}

		if( CG_MD.Uart->ReceiveFlag == UART_READY ){	
			tmMD_UARTRead( COM_FW_VER_INDEX, 0, num );
			CG_MD.Message.push_back( "讀取FW Version" );
		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;
			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
				tmMD_UART_ReceivedChar_Process ( dummy_data_FW, recieved_num );

				dummy_data_FW[ recieved_num ] = '\0';
				CG_MD.DriverCom.FW_Version = dummy_data_FW;
				CG_HMIProcess.SubState = DRIVER_INFOR_SUBSTATE_TOGET_SN;
				CG_MD.Message.push_back( "FW Version = " + CG_MD.DriverCom.FW_Version );
				//String^ text = gcnew String( CG_MD.DriverCom.FW_Version.c_str() );
				//MessageBox::Show( text );
			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}	
		break;
	
	case DRIVER_INFOR_SUBSTATE_TOGET_SN:
		num = SN_CHARS / 4;
		recieved_num = SN_CHARS;
		if( CG_MD.Protocol == MB_SLV_RTU ){
			num *= 2;
		}

		if( CG_MD.Uart->ReceiveFlag == UART_READY ){	
			tmMD_UARTRead( COM_HW_SN_0_INDEX, 0, num );
			CG_MD.Message.push_back( "讀取SN" );
		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;
			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
				tmMD_UART_ReceivedChar_Process ( dummy_data, recieved_num );

				dummy_data[ recieved_num ] = '\0';
				CG_MD.DriverCom.SN = dummy_data;
				CG_HMIProcess.SubState = DRIVER_INFOR_SUBSTATE_DONE;
				CG_MD.Message.push_back( "SN = " + CG_MD.DriverCom.SN );
				//String^ text = gcnew String( CG_MD.DriverCom.SN.c_str() );
				//MessageBox::Show( text );
			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}	

		break;
	case DRIVER_INFOR_SUBSTATE_DONE:
	default:
		CG_MD.ProgressBarValue = 100;
		//CG_HMIProcess.State++;
		CG_HMIProcess.State = HMIPROCESS_STATE_DONE;
		CG_HMIProcess.SubState = 0;
		CG_MD.Message.push_back( "驅動器資訊讀取完畢" );
		CG_MD.Message.push_back( "-----------------------------------------------" );

		//
		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Current_Time );

		CG_MD.Message.push_back( "時間花費 = " +
			to_string( (long double)tmMD_CalculateTimeCost ( CG_MD.Start_Time, CG_MD.Current_Time ) ) +
			"ms" );
		//


		break;
	
	}


	if( CG_MD.ComState == STATE_CLOSED ){
		CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;
	}

}

/*===========================================================================================
    Function Name    : tmHMIProcess_State_LoadHWEEP
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_LoadHWEEP( void )
{

	static string msg = "";
	string dummy_str;
	char *dummy;

	CG_MD.ProgressBarValue = 50 * ( CG_HMIProcess.SubState + 1 ) / HWEEP_SUBSTATE_NUM;

	switch( CG_HMIProcess.SubState ){

	case HWEEP_SUBSTATE_LOAD_FILE:

		CG_MD.PHEL_Result = 1;

		if( CG_MD.HWEEP_Path != L"" ){

			String^ path = gcnew String( CG_MD.HWEEP_Path.c_str() );
			tm_OpenPHEL( ( Struct_PHEL *)&CG_MD.TargetPHEL, path );
			
			CG_MD.Message.push_back( "讀取HWEEP檔案" );

			if( CG_MD.TargetPHEL.Items.size() != 0 ){
				/*
				for( int i = 0; i < (int)CG_MD.TargetPHEL.Items.size(); i++ ){
					CG_MD.Message.push_back( tm_DisplayPHELItem( (Struct_PHEL*)&CG_MD.TargetPHEL, (Struct_PHEL_Item *)&CG_MD.TargetPHEL.Items[i] ) );
				}*/
				CG_MD.PHEL_Item_Index = 0;

				CG_HMIProcess.SubState++;

			}else{
				CG_HMIProcess.State++;
				CG_HMIProcess.SubState = 0;
				CG_MD.Message.push_back( "PHEL檔案內無資料,跳過HWEEP燒錄" );
				CG_MD.Message.push_back( "-----------------------------------------------" );
			}
		}else{
			CG_HMIProcess.State++;
			CG_HMIProcess.SubState = 0;
			CG_MD.Message.push_back( "HWEEP無檔案,跳過HWEEP燒錄" );
			CG_MD.Message.push_back( "-----------------------------------------------" );
		}
		break;

	case HWEEP_SUBSTATE_KEYIN_PW:

		if( CG_MD.Uart->ReceiveFlag == UART_READY ){
			tmMD_UARTWrite( COM_MASTER_LV_INDEX, MASTER_LEVEL_PASSWROD, MASTER_LEVEL_3 );
			CG_MD.Message.push_back( "AL3 CMD" );
		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;

			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
				CG_HMIProcess.SubState++;
				CG_MD.Message.push_back( "進入AL3" );
			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}

		break;
	case HWEEP_SUBSTATE_WRITE_DATA:
				
		if( CG_MD.Uart->ReceiveFlag == UART_READY ){

			switch( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Type ){
			case PHEL_DATA_TYPE_1BYTE_NUMBER:
				tmMD_UARTWriteMultiple ( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_H,
										CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_L,
										CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num,
										&CG_MD.TargetPHEL.Data[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Index ] );
				break;
			case PHEL_DATA_TYPE_1BYTE_CHAR:
				tmMD_UARTWriteMultiple_Char ( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_H,
											CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_L,
											CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num,
											&CG_MD.TargetPHEL.Data[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Index ] );
				break;
			case PHEL_DATA_TYPE_2BYTE_NUMBER:
				for( int i = 0; i < CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num; i++ ){
					CG_MD.TargetPHEL.DummyData[i] = CG_MD.TargetPHEL.Data[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Index + i * 2 + 0 ] * 256 +
											CG_MD.TargetPHEL.Data[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Index + i * 2 + 1 ];
				}
				tmMD_UARTWriteMultiple ( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_H,
										CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_L,
										CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num,
										CG_MD.TargetPHEL.DummyData );
				break;
			default:
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_OTHERS;
				CG_MD.Message.push_back( "PHEL資料錯誤(不支援的Type)" );
				break;
			}

			CG_MD.Message.push_back( "寫入: " + tm_DisplayPHELItem( (Struct_PHEL*)&CG_MD.TargetPHEL, (Struct_PHEL_Item *)&CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ] ) );

		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;

			if( tmMD_UARTCheckFrame() ){

				tmMD_UART_GetReceiveFC();
				if( CG_MD.Uart->ReceiveFC != 0x10 ){
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_CMD_DENY;
					CG_MD.Message.push_back( WRITE_DENY_DISPLAY_INFOR );
				}else{
					CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
					CG_MD.PHEL_Item_Index++;
					if( CG_MD.PHEL_Item_Index >= (int)CG_MD.TargetPHEL.Items.size() ){
						CG_HMIProcess.SubState++;
						CG_MD.Message.push_back( "寫入HWEEP完成" );
						CG_MD.Message.push_back( "----------" );
					}	
				}
			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}

		break;
	case HWEEP_SUBSTATE_RESET_DRIVER:

		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Time_Stamp );

		CG_MD.ToResetDriver = 1;

		CG_HMIProcess.SubState++;
		CG_MD.Message.push_back( "驅動器Reset" );

		break;
	case HWEEP_SUBSTATE_RESET_DRIVER_WAIT:
		
		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Current_Time );
		if( tmMD_CalculateTimeCost ( CG_MD.Time_Stamp, CG_MD.Current_Time ) > TIME_CONST_WAIT_RESET_STABLE ){
			CG_HMIProcess.SubState++;
		}
		
		break;
		
	case HWEEP_SUBSTATE_RELOAD_CMD:

		if( CG_MD.Uart->ReceiveFlag == UART_READY ){
			tmMD_UARTWrite( COM_COMMAND_INDEX, COM_CMD_RELOAD_HWEEP, 1 );
			CG_MD.Message.push_back( "重新讀取HWEEP CMD" );
		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;
			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
				CG_HMIProcess.SubState++;
				GetLocalTime( ( SYSTEMTIME* )&CG_MD.Time_Stamp );

			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}

		break;
	case HWEEP_SUBSTATE_RELOAD_CMD_WAIT:

		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Current_Time );
		if( tmMD_CalculateTimeCost ( CG_MD.Time_Stamp, CG_MD.Current_Time ) > TIME_CONST_WAIT_RELOAD_HWEEP ){
			CG_MD.PHEL_Item_Index = 0;
			CG_HMIProcess.SubState++;
		}

		break;
	case HWEEP_SUBSTATE_READ_DATA:

		if( CG_MD.Uart->ReceiveFlag == UART_READY ){

			if( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Type == PHEL_DATA_TYPE_1BYTE_CHAR ){
				if( CG_MD.Protocol == MB_SLV_ASCII ){
					tmMD_UARTRead ( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_H,
								CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_L,
								CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num / 4 );
				}else{
					tmMD_UARTRead ( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_H,
								CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_L,
								CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num / 2 );
				}

			}else{

				tmMD_UARTRead ( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_H,
								CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_L,
								CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num );
			}

			msg = "讀取Address( " +
				to_string( (long double)CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_H ) +
				", " +
				to_string( (long double)CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Address_L ) +
				" ) = "; 

		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;

			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;

				if( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Type == PHEL_DATA_TYPE_1BYTE_CHAR ){
					tmMD_UART_ReceivedChar_Process ( CG_MD.TargetPHEL.ReadBack_Char,
													CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num );

					dummy = new char[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num + 1 ];

					int result_good = 1;
					for( int i = 0; i < CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num; i++ ){
						dummy[i] = CG_MD.TargetPHEL.ReadBack_Char[ i ];

						if( dummy[i] != CG_MD.TargetPHEL.Data[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Index + i ] ){
							CG_MD.PHEL_Result = 0;
							result_good = 0;
						}
					}
					dummy[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num ] = '\0';
					dummy_str = dummy;
						
					msg += dummy_str;

					if( result_good ){
						msg += " ( Ok )";
					}else{
						msg += " ( Fail... )";
					}

					delete []dummy;

					CG_MD.Message.push_back( msg );

				}else{
					tmMD_UART_ReceivedData_Process ( CG_MD.TargetPHEL.ReadBack_Value,
													CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num );

					int result_good = 1;

					for( int i = 0; i < CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num; i++ ){

						msg += to_string( (long double)CG_MD.TargetPHEL.ReadBack_Value[ i ] );

						if( i < CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Num - 1 ){
							msg += " ";
						}

						if( CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Type == PHEL_DATA_TYPE_1BYTE_NUMBER ){
							if( CG_MD.TargetPHEL.ReadBack_Value[ i ] != CG_MD.TargetPHEL.Data[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Index + i ] ){
								CG_MD.PHEL_Result = 0;
								result_good = 0;
							}
						}else{
							int value = CG_MD.TargetPHEL.Data[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Index + i * 2 + 0 ] * 256 +
								CG_MD.TargetPHEL.Data[ CG_MD.TargetPHEL.Items[ CG_MD.PHEL_Item_Index ].Index + i * 2 + 1 ];

							if( CG_MD.TargetPHEL.ReadBack_Value[ i ] != value ){
								CG_MD.PHEL_Result = 0;
								result_good = 0;
							}
						}
			
					}

					if( result_good ){
						msg += " ( Ok )";
					}else{
						msg += " ( Fail... )";
					}

					CG_MD.Message.push_back( msg );

				}

				CG_MD.PHEL_Item_Index++;
				if( CG_MD.PHEL_Item_Index >= (int)CG_MD.TargetPHEL.Items.size() ){
					CG_HMIProcess.SubState++;
					CG_MD.Message.push_back( "讀取HWEEP完成" );
					CG_MD.Message.push_back( "----------" );
				}	
			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}

		break;
	case HWEEP_SUBSTATE_VERIFY:

		if( CG_MD.PHEL_Result ){
			CG_MD.Message.push_back( "資料驗證完成" );
			CG_MD.Message.push_back( "----------" );
			CG_HMIProcess.SubState++;
		}else{
			CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
			CG_HMIProcess.Error_Index = ERROR_INDEX_OTHERS;
			CG_MD.Message.push_back( "資料驗證失敗, 寫入與讀取資料不一致...:(" );

		}

		break;

	case HWEEP_SUBSTATE_DONE:

		CG_HMIProcess.State++;
		//CG_HMIProcess.State = HMIPROCESS_STATE_DONE;
		CG_HMIProcess.SubState = 0;
		CG_MD.Message.push_back( "HWEEP完畢" );
		CG_MD.Message.push_back( "-----------------------------------------------" );

		/*//
		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Current_Time );

		CG_MD.Message.push_back( "時間花費 = " +
			to_string( (long double)tmMD_CalculateTimeCost ( CG_MD.Start_Time, CG_MD.Current_Time ) ) +
			"ms" );
		//*/

		break;

	default:
		CG_MD.Message.push_back( "HWEEP Trip..." );

		break;
	
	}

	if( CG_MD.ComState == STATE_CLOSED ){
		CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;
	}
}


/*===========================================================================================
    Function Name    : tmHMIProcess_State_LoadFWEEP
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_LoadFWEEP ( void )
{
	static string msg = "";
	string dummy_str;
	//char *dummy;
	static int pa_major_max = 0;
	static int pa_minor_max = 0;
	int *data;

	CG_MD.ProgressBarValue = 50 + 50 * ( CG_HMIProcess.SubState + 1 ) / FWEEP_SUBSTATE_NUM;

	switch( CG_HMIProcess.SubState ){

	case FWEEP_SUBSTATE_LOAD_FILE:

		CG_MD.FWEEP_Result = 1;

		if( CG_MD.FWEEP_Path != L"" ){

			String^ path = gcnew String( CG_MD.FWEEP_Path.c_str() );

			CG_MD.Message.push_back( "讀取參數檔案" );

			CG_MD.FWEEP_Item_Index = 0;
			

			if( String::Compare( path, path->Length-5, ".PELB", 0, 5 ) == 0 ){
				tm_OpenPELB( ( Struct_PELB *)&CG_MD.TargetPELB, path );
				CG_MD.FWEEP_Type = FWEEP_TYPE_PELB;
				pa_major_max = PELB_MAJOR_MAX;
				pa_minor_max = PELB_MINOR_MAX;
				CG_HMIProcess.SubState++;
			}else if( String::Compare( path, path->Length-5, ".PEL2", 0, 5 ) == 0 ){
				tm_OpenPEL2( ( Struct_PEL2 *)&CG_MD.TargetPEL2, path );
				CG_MD.FWEEP_Type = FWEEP_TYPE_PEL2;
				pa_major_max = PEL2_MAJOR_MAX;
				pa_minor_max = PEL2_MINOR_MAX;
				CG_HMIProcess.SubState++;
			}else if( String::Compare( path, path->Length-4, ".PEL", 0, 4 ) == 0 ){
				tm_OpenPEL( ( Struct_PEL *)&CG_MD.TargetPEL, path );
				CG_MD.FWEEP_Type = FWEEP_TYPE_PEL;
				pa_major_max = PEL_MAJOR_MAX;
				pa_minor_max = PEL_MINOR_MAX;
				CG_HMIProcess.SubState++;
			}else{
				CG_MD.Message.push_back( "暫不支援此類型參數...:(" );
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
			}	
			
		}else{
			CG_HMIProcess.State++;
			CG_HMIProcess.SubState = 0;
			CG_MD.Message.push_back( "FWEEP無檔案,跳過FWEEP燒錄" );
			CG_MD.Message.push_back( "-----------------------------------------------" );
		}
		break;

	case FWEEP_SUBSTATE_KEYIN_PW:

		if( CG_MD.Uart->ReceiveFlag == UART_READY ){
			tmMD_UARTWrite( COM_MASTER_LV_INDEX, MASTER_LEVEL_PASSWROD, MASTER_LEVEL_3 );
			CG_MD.Message.push_back( "AL3 CMD" );
		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;

			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
				CG_HMIProcess.SubState++;
				CG_MD.Message.push_back( "進入AL3" );
			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}

		break;
		
	case FWEEP_SUBSTATE_WRITE_VALUE:

		if( CG_MD.ToLoadValue ){
			if( CG_MD.Uart->ReceiveFlag == UART_READY ){

				dummy_str = "寫入: " +
						Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-01 ~ " +
						Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-" +
						Number2string( pa_minor_max, 2 ) +
						" 's Value = ";

				if( CG_MD.FWEEP_Type == FWEEP_TYPE_PELB ){
					tmMD_UARTWriteMultiple ( COM_PA_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPELB.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPELB.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}
				}else if( CG_MD.FWEEP_Type == FWEEP_TYPE_PEL2 ){
					tmMD_UARTWriteMultiple ( COM_PA_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPEL2.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPEL2.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}else{
					tmMD_UARTWriteMultiple ( COM_PA_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPEL.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPEL.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}
				
				CG_MD.Message.push_back( dummy_str );

			}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
				CG_MD.Uart->ReceiveFlag = UART_READY;

				if( tmMD_UARTCheckFrame() ){

					tmMD_UART_GetReceiveFC();
					if( CG_MD.Uart->ReceiveFC != 0x10 ){
						CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
						CG_HMIProcess.Error_Index = ERROR_INDEX_CMD_DENY;
						CG_MD.Message.push_back( WRITE_DENY_DISPLAY_INFOR );
					}else{
						CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
						CG_MD.FWEEP_Item_Index++;
		
						if( CG_MD.FWEEP_Item_Index >= pa_major_max ){
							CG_MD.FWEEP_Item_Index = 0;
							CG_HMIProcess.SubState++;
							CG_MD.Message.push_back( "寫入Value完成" );
							CG_MD.Message.push_back( "----------" );
						}
					}

				}else{
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
					CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
				}
			}


		}else{
			CG_HMIProcess.SubState++;
			CG_MD.Message.push_back( "跳過Value" );
			CG_MD.Message.push_back( "----------" );
		}
		
		break;

	case FWEEP_SUBSTATE_WRITE_DEF:

		if( CG_MD.ToLoadDef ){
			if( CG_MD.Uart->ReceiveFlag == UART_READY ){

				dummy_str = "寫入: " +
						Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-01 ~ " +
						Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-" +
						Number2string( pa_minor_max, 2 ) +
						" 's Def = ";

				if( CG_MD.FWEEP_Type == FWEEP_TYPE_PELB ){
					tmMD_UARTWriteMultiple ( COM_DEFAULT_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPELB.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPELB.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}else if( CG_MD.FWEEP_Type == FWEEP_TYPE_PEL2 ){
					tmMD_UARTWriteMultiple ( COM_DEFAULT_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPEL2.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPEL2.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}else{
					tmMD_UARTWriteMultiple ( COM_DEFAULT_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPEL.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPEL.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}
				
				CG_MD.Message.push_back( dummy_str );

			}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
				CG_MD.Uart->ReceiveFlag = UART_READY;

				if( tmMD_UARTCheckFrame() ){

					tmMD_UART_GetReceiveFC();
					if( CG_MD.Uart->ReceiveFC != 0x10 ){
						CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
						CG_HMIProcess.Error_Index = ERROR_INDEX_CMD_DENY;
						CG_MD.Message.push_back( WRITE_DENY_DISPLAY_INFOR );
					}else{
						CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
						CG_MD.FWEEP_Item_Index++;
		
						if( CG_MD.FWEEP_Item_Index >= pa_major_max ){
							CG_MD.FWEEP_Item_Index = 0;
							CG_HMIProcess.SubState++;
							CG_MD.Message.push_back( "寫入Def完成" );
							CG_MD.Message.push_back( "----------" );
						}
					}

				}else{
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
					CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
				}
			}


		}else{
			CG_HMIProcess.SubState++;
			CG_MD.Message.push_back( "跳過Def" );
			CG_MD.Message.push_back( "----------" );
		}

		break;
	case FWEEP_SUBSTATE_WRITE_MAX:

		if( CG_MD.ToLoadMax ){
			if( CG_MD.Uart->ReceiveFlag == UART_READY ){

				dummy_str = "寫入: " +
						Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-01 ~ " +
						Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-" +
						Number2string( pa_minor_max, 2 ) +
						" 's Max = ";

				if( CG_MD.FWEEP_Type == FWEEP_TYPE_PELB ){

					tmMD_UARTWriteMultiple ( COM_MAX_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPELB.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPELB.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}else if( CG_MD.FWEEP_Type == FWEEP_TYPE_PEL2 ){
					tmMD_UARTWriteMultiple ( COM_MAX_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPEL2.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPEL2.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}else{
					tmMD_UARTWriteMultiple ( COM_MAX_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPEL.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPEL.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}
				
				CG_MD.Message.push_back( dummy_str );

			}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
				CG_MD.Uart->ReceiveFlag = UART_READY;

				if( tmMD_UARTCheckFrame() ){

					tmMD_UART_GetReceiveFC();
					if( CG_MD.Uart->ReceiveFC != 0x10 ){
						CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
						CG_HMIProcess.Error_Index = ERROR_INDEX_CMD_DENY;
						CG_MD.Message.push_back( WRITE_DENY_DISPLAY_INFOR );
					}else{
						CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
						CG_MD.FWEEP_Item_Index++;
		
						if( CG_MD.FWEEP_Item_Index >= pa_major_max ){
							CG_MD.FWEEP_Item_Index = 0;
							CG_HMIProcess.SubState++;
							CG_MD.Message.push_back( "寫入Max完成" );
							CG_MD.Message.push_back( "----------" );
						}
					}

				}else{
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
					CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
				}
			}


		}else{
			CG_HMIProcess.SubState++;
			CG_MD.Message.push_back( "跳過Max" );
			CG_MD.Message.push_back( "----------" );
		}

		break;

	case FWEEP_SUBSTATE_WRITE_MIN:

		if( CG_MD.ToLoadMin ){
			if( CG_MD.Uart->ReceiveFlag == UART_READY ){

				dummy_str = "寫入: " +
						Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-01 ~ " +
						Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-" +
						Number2string( pa_minor_max, 2 ) +
						" 's Min = ";
				if( CG_MD.FWEEP_Type == FWEEP_TYPE_PELB ){
					tmMD_UARTWriteMultiple ( COM_MIN_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPELB.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPELB.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}
				}else if( CG_MD.FWEEP_Type == FWEEP_TYPE_PEL2 ){
					tmMD_UARTWriteMultiple ( COM_MIN_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPEL2.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPEL2.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}else{
					tmMD_UARTWriteMultiple ( COM_MIN_EEP_INDEX + CG_MD.FWEEP_Item_Index,
											0,
											pa_minor_max,
											&CG_MD.TargetPEL.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ 0 ]  );

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)CG_MD.TargetPEL.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}
					}

				}
				
				CG_MD.Message.push_back( dummy_str );

			}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
				CG_MD.Uart->ReceiveFlag = UART_READY;

				if( tmMD_UARTCheckFrame() ){

					tmMD_UART_GetReceiveFC();
					if( CG_MD.Uart->ReceiveFC != 0x10 ){
						CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
						CG_HMIProcess.Error_Index = ERROR_INDEX_CMD_DENY;
						CG_MD.Message.push_back( WRITE_DENY_DISPLAY_INFOR );
					}else{

						CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
						CG_MD.FWEEP_Item_Index++;
		
						if( CG_MD.FWEEP_Item_Index >= pa_major_max ){
							CG_MD.FWEEP_Item_Index = 0;
							CG_HMIProcess.SubState++;
							CG_MD.Message.push_back( "寫入Min完成" );
							CG_MD.Message.push_back( "----------" );
						}
					}

				}else{
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
					CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
				}
			}


		}else{
			CG_HMIProcess.SubState++;
			CG_MD.Message.push_back( "跳過Min" );
			CG_MD.Message.push_back( "----------" );
		}

		break;
		
	case FWEEP_SUBSTATE_RESET_DRIVER:
		
		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Time_Stamp );

		CG_MD.ToResetDriver = 1;

		CG_HMIProcess.SubState++;
		CG_MD.Message.push_back( "驅動器Reset" );
		
		break;

	case FWEEP_SUBSTATE_RESET_DRIVER_WAIT:
		
		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Current_Time );
		if( tmMD_CalculateTimeCost ( CG_MD.Time_Stamp, CG_MD.Current_Time ) > TIME_CONST_WAIT_RESET_STABLE ){
			CG_HMIProcess.SubState++;
		}
		
		break;
		
	case FWEEP_SUBSTATE_RELOAD_CMD:

		if( CG_MD.Uart->ReceiveFlag == UART_READY ){
			tmMD_UARTWrite( COM_COMMAND_INDEX, COM_CMD_RELOAD_FWEEP, 1 );
			CG_MD.Message.push_back( "重新讀取FWEEP CMD" );
		}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
			CG_MD.Uart->ReceiveFlag = UART_READY;
			if( tmMD_UARTCheckFrame() ){
				CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;
				CG_HMIProcess.SubState++;
				GetLocalTime( ( SYSTEMTIME* )&CG_MD.Time_Stamp );

			}else{
				CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
				CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
				CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
			}
		}

		break;
	case FWEEP_SUBSTATE_RELOAD_CMD_WAIT:

		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Current_Time );
		if( tmMD_CalculateTimeCost ( CG_MD.Time_Stamp, CG_MD.Current_Time ) > TIME_CONST_WAIT_RELOAD_FWEEP ){
			CG_MD.PHEL_Item_Index = 0;
			CG_HMIProcess.SubState++;
		}

		break;
	case FWEEP_SUBSTATE_READ_VALUE:

		if( CG_MD.ToLoadValue ){

			if( CG_MD.Uart->ReceiveFlag == UART_READY ){

				tmMD_UARTRead ( COM_PA_EEP_INDEX + CG_MD.FWEEP_Item_Index,
								0,
								pa_minor_max );

				msg = "讀取: " +
					Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-01 ~ " +
					Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-" +
					Number2string( pa_minor_max, 2 ) +
					" 's Value ";

			}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
				CG_MD.Uart->ReceiveFlag = UART_READY;

				if( tmMD_UARTCheckFrame() ){
					CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;

					data = new int[ pa_minor_max ];	
					tmMD_UART_ReceivedData_Process ( data, pa_minor_max );

					int result_good = 1;

					dummy_str = "";

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)data[ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}

						int target_value;
						if( CG_MD.FWEEP_Type == FWEEP_TYPE_PELB ){
							target_value = CG_MD.TargetPELB.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ i ];
						}else if( CG_MD.FWEEP_Type == FWEEP_TYPE_PEL2 ){
							target_value = CG_MD.TargetPEL2.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ i ];
						}else{
							target_value = CG_MD.TargetPEL.Data[ PA_SUBITEM_VALUE ][ CG_MD.FWEEP_Item_Index ][ i ];
						}

						if( data[ i ] != target_value ){
							CG_MD.FWEEP_Result = 0;
							result_good = 0;
						}
					
					}

					delete []data;

					if( result_good ){
						msg += " ( Ok ) :	" + dummy_str;
					}else{
						msg += " ( Fail ) :	" + dummy_str;
					}

					CG_MD.Message.push_back( msg );

					CG_MD.FWEEP_Item_Index++;
					if( CG_MD.FWEEP_Item_Index >= pa_major_max ){
						CG_MD.FWEEP_Item_Index = 0;
						CG_HMIProcess.SubState++;
						CG_MD.Message.push_back( "讀取Value完成" );
						CG_MD.Message.push_back( "----------" );
					}	
				}else{
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
					CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
				}
			}

		}else{
			CG_HMIProcess.SubState++;
			CG_MD.Message.push_back( "跳過Value" );
			CG_MD.Message.push_back( "----------" );
		}
		
		break;
	case FWEEP_SUBSTATE_READ_DEF:

		if( CG_MD.ToLoadDef ){

			if( CG_MD.Uart->ReceiveFlag == UART_READY ){

				tmMD_UARTRead ( COM_DEFAULT_EEP_INDEX + CG_MD.FWEEP_Item_Index,
								0,
								pa_minor_max );

				msg = "讀取: " +
					Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-01 ~ " +
					Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-" +
					Number2string( pa_minor_max, 2 ) +
					" 's Def ";

			}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
				CG_MD.Uart->ReceiveFlag = UART_READY;

				if( tmMD_UARTCheckFrame() ){
					CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;

					data = new int[ pa_minor_max ];	
					tmMD_UART_ReceivedData_Process ( data, pa_minor_max );

					int result_good = 1;

					dummy_str = "";

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)data[ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}

						int target_value;
						if( CG_MD.FWEEP_Type == FWEEP_TYPE_PELB ){
							target_value = CG_MD.TargetPELB.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ i ];
						}else if( CG_MD.FWEEP_Type == FWEEP_TYPE_PEL2 ){
							target_value = CG_MD.TargetPEL2.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ i ];
						}else{
							target_value = CG_MD.TargetPEL.Data[ PA_SUBITEM_DEFAULT ][ CG_MD.FWEEP_Item_Index ][ i ];
						}

						if( data[ i ] != target_value ){
							CG_MD.FWEEP_Result = 0;
							result_good = 0;
						}
					
					}

					delete []data;

					if( result_good ){
						msg += " ( Ok ) :	" + dummy_str;
					}else{
						msg += " ( Fail ) :	" + dummy_str;
					}

					CG_MD.Message.push_back( msg );

					CG_MD.FWEEP_Item_Index++;
					if( CG_MD.FWEEP_Item_Index >= pa_major_max ){
						CG_HMIProcess.SubState++;
						CG_MD.FWEEP_Item_Index = 0;
						CG_MD.Message.push_back( "讀取Def完成" );
						CG_MD.Message.push_back( "----------" );
					}	
				}else{
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
					CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
				}
			}

		}else{
			CG_HMIProcess.SubState++;
			CG_MD.Message.push_back( "跳過Def" );
			CG_MD.Message.push_back( "----------" );
		}

		break;
	case FWEEP_SUBSTATE_READ_MAX:

		if( CG_MD.ToLoadMax ){

			if( CG_MD.Uart->ReceiveFlag == UART_READY ){

				tmMD_UARTRead ( COM_MAX_EEP_INDEX + CG_MD.FWEEP_Item_Index,
								0,
								pa_minor_max );

				msg = "讀取: " +
					Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-01 ~ " +
					Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-" +
					Number2string( pa_minor_max, 2 ) +
					" 's Max ";

			}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
				CG_MD.Uart->ReceiveFlag = UART_READY;

				if( tmMD_UARTCheckFrame() ){
					CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;

					data = new int[ pa_minor_max ];	
					tmMD_UART_ReceivedData_Process ( data, pa_minor_max );

					int result_good = 1;

					dummy_str = "";

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)data[ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}

						int target_value;
						if( CG_MD.FWEEP_Type == FWEEP_TYPE_PELB ){
							target_value = CG_MD.TargetPELB.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ i ];
						}else if( CG_MD.FWEEP_Type == FWEEP_TYPE_PEL2 ){
							target_value = CG_MD.TargetPEL2.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ i ];
						}else{
							target_value = CG_MD.TargetPEL.Data[ PA_SUBITEM_MAX ][ CG_MD.FWEEP_Item_Index ][ i ];
						}

						if( data[ i ] != target_value ){
							CG_MD.FWEEP_Result = 0;
							result_good = 0;
						}
					
					}

					delete []data;

					if( result_good ){
						msg += " ( Ok ) :	" + dummy_str;
					}else{
						msg += " ( Fail ) :	" + dummy_str;
					}

					CG_MD.Message.push_back( msg );

					CG_MD.FWEEP_Item_Index++;
					if( CG_MD.FWEEP_Item_Index >= pa_major_max ){
						CG_HMIProcess.SubState++;
						CG_MD.FWEEP_Item_Index = 0;
						CG_MD.Message.push_back( "讀取Max完成" );
						CG_MD.Message.push_back( "----------" );
					}	
				}else{
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
					CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
				}
			}

		}else{
			CG_HMIProcess.SubState++;
			CG_MD.Message.push_back( "跳過Max" );
			CG_MD.Message.push_back( "----------" );
		}

		break;
	case FWEEP_SUBSTATE_READ_MIN:

		if( CG_MD.ToLoadMin ){

			if( CG_MD.Uart->ReceiveFlag == UART_READY ){

				tmMD_UARTRead ( COM_MIN_EEP_INDEX + CG_MD.FWEEP_Item_Index,
								0,
								pa_minor_max );

				msg = "讀取: " +
					Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-01 ~ " +
					Number2string( CG_MD.FWEEP_Item_Index + 1, 2 ) + "-" +
					Number2string( pa_minor_max, 2 ) +
					" 's Min ";

			}else if( CG_MD.Uart->ReceiveFlag == UART_DONE ){	
				CG_MD.Uart->ReceiveFlag = UART_READY;

				if( tmMD_UARTCheckFrame() ){
					CG_HMIProcess.Error_Index = ERROR_INDEX_NO_ERROR;

					data = new int[ pa_minor_max ];	
					tmMD_UART_ReceivedData_Process ( data, pa_minor_max );

					int result_good = 1;

					dummy_str = "";

					for( int i = 0; i < pa_minor_max; i++ ){
						dummy_str += to_string( (long double)data[ i ] );
						if( i < pa_minor_max - 1 ){
							dummy_str += "	";
						}

						int target_value;


						if( CG_MD.FWEEP_Type == FWEEP_TYPE_PELB ){
							target_value = CG_MD.TargetPELB.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ i ];
						}else if( CG_MD.FWEEP_Type == FWEEP_TYPE_PEL2 ){
							target_value = CG_MD.TargetPEL2.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ i ];
						}else{
							target_value = CG_MD.TargetPEL.Data[ PA_SUBITEM_MIN ][ CG_MD.FWEEP_Item_Index ][ i ];
						}

						if( data[ i ] != target_value ){
							CG_MD.FWEEP_Result = 0;
							result_good = 0;
						}
					
					}

					delete []data;

					if( result_good ){
						msg += " ( Ok ) :	" + dummy_str;
					}else{
						msg += " ( Fail ) :	" + dummy_str;
					}

					CG_MD.Message.push_back( msg );

					CG_MD.FWEEP_Item_Index++;
					if( CG_MD.FWEEP_Item_Index >= pa_major_max ){
						CG_HMIProcess.SubState++;
						CG_MD.FWEEP_Item_Index = 0;
						CG_MD.Message.push_back( "讀取Min完成" );
						CG_MD.Message.push_back( "----------" );
					}	
				}else{
					CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
					CG_HMIProcess.Error_Index = ERROR_INDEX_FRAME;
					CG_MD.Message.push_back( "資料錯誤(Frame Error)" );
				}
			}

		}else{
			CG_HMIProcess.SubState++;
			CG_MD.Message.push_back( "跳過Min" );
			CG_MD.Message.push_back( "----------" );
		}

		break;
	case FWEEP_SUBSTATE_VERIFY:

		if( CG_MD.FWEEP_Result ){
			CG_MD.Message.push_back( "資料驗證完成" );
			CG_MD.Message.push_back( "----------" );
			CG_HMIProcess.SubState++;
		}else{
			CG_HMIProcess.State = HMIPROCESS_STATE_ERROR;
			CG_HMIProcess.Error_Index = ERROR_INDEX_OTHERS;
			CG_MD.Message.push_back( "資料驗證失敗, 寫入與讀取資料不一致...:(" );

		}

		break;

	case FWEEP_SUBSTATE_DONE:

		//CG_HMIProcess.State++;
		CG_HMIProcess.State = HMIPROCESS_STATE_GET_DRIVER_INFOR;
		CG_HMIProcess.SubState = 0;
		CG_MD.Message.push_back( "FWEEP完畢" );
		CG_MD.Message.push_back( "-----------------------------------------------" );

		//
		/*
		GetLocalTime( ( SYSTEMTIME* )&CG_MD.Current_Time );

		CG_MD.Message.push_back( "時間花費 = " +
			to_string( (long double)tmMD_CalculateTimeCost ( CG_MD.Start_Time, CG_MD.Current_Time ) ) +
			"ms" );
		*/
		//

		break;

	default:
		CG_MD.Message.push_back( "FWEEP Trip..." );

		break;
	
	}

	if( CG_MD.ComState == STATE_CLOSED ){
		CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;
	}

}

/*===========================================================================================
    Function Name    : tmHMIProcess_State_Done
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_Done ( void )
{
	if( CG_MD.ComState == STATE_CLOSED ){
		CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;
	}
}

/*===========================================================================================
    Function Name    : tmHMIProcess_State_TimeOut
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_TimeOut ( void )
{
	if( CG_MD.Uart->current_time > 2 * TIME_OUT_CONST ){
		CG_MD.Uart->current_time = 0;

		CG_MD.Uart->Timeout_Flag = 0;

		//CG_MD.Message.push_back( "沒有回應, 請確認接線與設定是否正確...:(" );
		CG_MD.TimeOut_Flag = 1;
		CG_HMIProcess.State = HMIPROCESS_STATE_DONE;
		CG_HMIProcess.SubState = 0;

	}

	if( CG_MD.ComState == STATE_CLOSED ){
		CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;
	}
}

/*===========================================================================================
    Function Name    : tmHMIProcess_State_Error
    Input            : Null
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      :
//==========================================================================================*/
void tmHMIProcess_State_Error ( void )
{

	//CG_MD.Message.push_back( "發生異常, 提前結束...:(" );

	CG_HMIProcess.State = HMIPROCESS_STATE_DONE;
	CG_HMIProcess.SubState = 0;
	CG_MD.Error_Flag = 1;

	if( CG_MD.ComState == STATE_CLOSED ){
		CG_HMIProcess.State = HMIPROCESS_STATE_IDLE;
	}

}



/************************** <END OF FILE> *****************************************/

