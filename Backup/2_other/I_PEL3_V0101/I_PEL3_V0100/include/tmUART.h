/*===========================================================================================

    File Name       : tmUART.h

    Version         : V1_00_a

    Built Date      : 2015/05/26

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2015/05/26 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_UART_H
#define TM_UART_H
#include "windows.h"
#include <process.h>
#include "String.h"
#include <sstream>
#include <vector>
#include "tmDriverCom.h"

using namespace System;
using namespace IO;
using namespace std;

#define R_BUFFER_SIZE  666
#define T_BUFFER_SIZE  666

#define MODBUS_RTU_CRC_CONST	0xA001
#define RTU_TIME_FRAME_LENGTH	4

#define TIME_OUT_CONST_MAX		65535
#define TIME_OUT_CONST			50
#define STABLE_TIME_CONST		500
#define PATH_STRING_LENGTH		200




#define MODBUS_FC_03			3
#define MODBUS_FC_06			6
#define MODBUS_FC_16			16

#define MODBUS_FC_03_BAD_FB		0x83
#define MODBUS_FC_06_BAD_FB		0x86

enum{
	STATE_CLOSED = 0,
	STATE_OPENED = 1,
	STATE_NUM = 2
};

enum{
    UART_READY       = 0,
    UART_BUSY        = 1,
    UART_DONE        = 2,
    UART_STATE_NUM   = 3
};


/*===========================================================================================
    ModBus Slave serial Transmission Modes select
//==========================================================================================*/
enum{
	MB_SLV_ASCII				= 0,
	MB_SLV_RTU					= 1,
	MB_SLV_TRANS_MODE_NUM       = 2
};


enum{
     MB_ASCII_START_BYTE				= 58,
     MB_ASCII_END_BYTE1					= 13,
     MB_ASCII_END_BYTE2					= 10
};


typedef struct{

	unsigned char R_Buffer[ R_BUFFER_SIZE ];
	unsigned char T_Buffer[ T_BUFFER_SIZE ];

	int R_Pointer;
	int T_Pointer;
	int State;
	int ReceiveFlag;
	int ReceiveID;
	int ReceiveFC;

	int ExceptionCode;

	bool Request_Flag;
	int Request_ID;
	int RequestAddress_H;
	int RequestAddress_L;
	int Request_Num;

	unsigned int current_driver;
	//
	unsigned int current_time;
	unsigned int Timeout_Flag;
	unsigned int R_data_number;
	unsigned int T_data_number;
	unsigned int get_data_flag;
	unsigned int rtu_time_point_1;
	unsigned int rtu_time_point_2;
	unsigned int rtu_time_frame_length;
//};
}Struct_TMUART;

/*===========================================================================================
    Function Name    : tmUART_VariableInitial
    Input            : uart
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Variable initial
//==========================================================================================*/
void tmUART_VariableInitial ( Struct_TMUART* uart );

/*===========================================================================================
    Function Name    : tmUART_CheckUARTframe
    Input            : uart
    Return           : Null
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : For RTU
//==========================================================================================*/
 void tmUART_CheckRTUframe( Struct_TMUART* uart );

 /*===========================================================================================
    Function Name    : tmUART_CheckRTU_CRC
    Input            : Null
    Return           : true or false
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : For RTU
//==========================================================================================*/
int tmUART_CheckRTU_CRC( unsigned char* src, int data_num );

/*===========================================================================================
    Function Name    : tmUART_CheckASCII_LRC
    Input            : data, data length
    Return           : true or false
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : For ASCII
//==========================================================================================*/
char tmUART_CheckASCII_LRC( unsigned char *data, int data_num );

/*===========================================================================================
    Function Name    : tmUART_CalculateCRC
    Input            : src : array data
                       data_lengh : data number
					   dec : CRC, dec[1] = High Byte, dec[0] = Low Byte
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_CalculateCRC( unsigned char * src, int data_lengh, unsigned char *dec );

/*===========================================================================================
    Function Name    : tmUART_ASCII2UNCHAR
    Input            : char_buffer_1 : ASCII char High Byte
                       char_buffer_0 : ASCII char Low Byte
    Return           : number calculated by char_buffer_1 & char_buffer_0
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : char_buffer_1 * 16 + char_buffer_0
//==========================================================================================*/
unsigned char tmUART_ASCII2UNCHAR(unsigned char char_buffer_1, unsigned char char_buffer_0);

/*===========================================================================================
    Function Name    : tmUART_UNCHAR2ASCII
    Input            : char_buffer : input value
                       final_char : 2 ASCII chars
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : final_char[0] = Low Byte, final_char[1] = High Byte
//==========================================================================================*/
void tmUART_UNCHAR2ASCII(unsigned char char_buffer,unsigned char *final_char);

/*===========================================================================================
    Function Name    : tmUART_DataReceived
    Input            : serialPort
                       uart
					   mode: ASCII or RTU
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_DataReceived( System::IO::Ports::SerialPort^  serialPort, Struct_TMUART* uart, int	mode );

/*===========================================================================================
    Function Name    : tmUART_Transmit_ResetSignal
    Input            : serialPort
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Transmit_ResetSignal( System::IO::Ports::SerialPort^  serialPort );

/*===========================================================================================
    Function Name    : tmUART_ReceivedData_Process_ASCII
    Input            : uart
					   dst
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
void tmUART_ReceivedData_Process_ASCII( Struct_TMUART* uart, int *dst, int number );

/*===========================================================================================
    Function Name    : tmUART_ReceivedData_Process_RTU
    Input            : uart
					   dst
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
void tmUART_ReceivedData_Process_RTU( Struct_TMUART* uart, int *dst, int number );

/*===========================================================================================
    Function Name    : tmUART_ReceivedChar_Process_ASCII
    Input            : uart
					   dst
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
void tmUART_ReceivedChar_Process_ASCII( Struct_TMUART* uart, char *dst, int number );

/*===========================================================================================
    Function Name    : tmUART_ReceivedChar_Process_RTU
    Input            : uart
					   dst
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
void tmUART_ReceivedChar_Process_RTU( Struct_TMUART* uart, char *dst, int number );

/*===========================================================================================
    Function Name    : tmUART_CheckRequest
    Input            : serialPort
                       uart
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_CheckRequest( System::IO::Ports::SerialPort^  serialPort, Struct_TMUART* uart );

/*===========================================================================================
    Function Name    : tmUART_DataTransmit
    Input            : serialPort
                       uart
					   num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_DataTransmit( System::IO::Ports::SerialPort^  serialPort, Struct_TMUART* uart, int	num );

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC3_Master_RTU
    Input            : uart
                       ID
					   address_h
					   address_l
					   num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Modbus_FC3_Master_RTU( Struct_TMUART* uart, int ID, int address_h, int address_l, int num );

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC3_Master_ASCII
    Input            : uart
                       ID
					   address_h
					   address_l
					   num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Modbus_FC3_Master_ASCII( Struct_TMUART* uart, int ID, int address_h, int address_l, int num );

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC6_Master_RTU
    Input            : uart
                       ID
					   address_h
					   address_l
					   num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Modbus_FC6_Master_RTU( Struct_TMUART* uart, int ID, int address_h, int address_l, int num );

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC6_Master_ASCII
    Input            : uart
                       ID
					   address_h
					   address_l
					   num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Modbus_FC6_Master_ASCII( Struct_TMUART* uart, int ID, int address_h, int address_l, int num );

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC16_Master_RTU
    Input            : uart
                       ID
					   address_h
					   address_l
					   num
					   data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Modbus_FC16_Master_RTU( Struct_TMUART* uart, int ID, int address_h, int address_l, int num, int* data );

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC16_Master_ASCII
    Input            : uart
                       ID
					   address_h
					   address_l
					   num
					   data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Modbus_FC16_Master_ASCII( Struct_TMUART* uart, int ID, int address_h, int address_l, int num, int* data );

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC16_Master_ASCII_CHAR
    Input            : uart
                       ID
					   address_h
					   address_l
					   num
					   data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : This function is for loading SN and PN
//==========================================================================================*/
void tmUART_Modbus_FC16_Master_ASCII_CHAR( Struct_TMUART* uart, int ID, int address_h, int address_l, int num, int* data );

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC16_Master_RTU_CHAR
    Input            : uart
                       ID
					   address_h
					   address_l
					   num
					   data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Modbus_FC16_Master_RTU_CHAR( Struct_TMUART* uart, int ID, int address_h, int address_l, int num, int* data );

#endif

/************************** <END OF FILE> *****************************************/



