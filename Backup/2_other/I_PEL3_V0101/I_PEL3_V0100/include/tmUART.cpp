/*===========================================================================================

    File Name       : tmUART.c

    Version         : V1_00_a

    Built Date      : 2015/05/26

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2015/05/26 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */


#include "stdafx.h"
#include "windows.h"
#include <process.h>
#include "Type.h"
#include "tmUART.h"
#include "tmString.h"
#include <sstream>

using namespace System;
using namespace IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//Struct_TMUART					CG_RS232;
//Struct_TMUART					CG_RS485;
Struct_TMUART					CG_UART;

/*===========================================================================================
    Function Name    : tmUART_VariableInitial
    Input            : uart
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Variable initial
//==========================================================================================*/
void tmUART_VariableInitial ( Struct_TMUART* uart )
{
	uart->R_Pointer = 0;
	uart->T_Pointer = 0;
	uart->current_time = 0;
	uart->Request_Flag = NO;
	uart->ReceiveFlag = UART_READY;
	uart->Timeout_Flag = NO;
	uart->State = STATE_CLOSED;
	uart->rtu_time_frame_length = RTU_TIME_FRAME_LENGTH;
}


/*===========================================================================================
    Function Name    : tmUART_CheckUARTframe
    Input            : uart
    Return           : Null
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : For RTU
//==========================================================================================*/
 void tmUART_CheckRTUframe( Struct_TMUART* uart )
{
	unsigned int dummy_rtu_time_point_1;

	uart->current_time++;

	if( uart->get_data_flag != NO ){

		dummy_rtu_time_point_1 = uart->rtu_time_point_1;
		uart->rtu_time_point_2 = uart->current_time;

		if( (int)( uart->rtu_time_point_2 - dummy_rtu_time_point_1 ) > uart->rtu_time_frame_length ){

			uart->get_data_flag			= NO;
			uart->R_data_number			= uart->R_Pointer;
			uart->R_Pointer				= 0;
			uart->ReceiveFlag 			= UART_DONE;

		}
	}
}

/*===========================================================================================
    Function Name    : tmUART_CheckRTU_CRC
    Input            : Null
    Return           : true or false
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : For RTU
//==========================================================================================*/
int tmUART_CheckRTU_CRC( unsigned char* src, int data_num )
{
	unsigned char CRC_Calculation[2];
	unsigned char CRC_Received[2];

	tmUART_CalculateCRC( src, data_num - 2, CRC_Calculation );

	CRC_Received[0] = src[ data_num - 2 ];
	CRC_Received[1] = src[ data_num - 1 ];

	if( CRC_Received[0] == CRC_Calculation[0] && CRC_Received[1] == CRC_Calculation[1] ){
		return 1;
	}else{
		return 0;
	}	
}


/*===========================================================================================
    Function Name    : tmUART_CheckASCII_LRC
    Input            : data, data length
    Return           : true or false
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : For ASCII
//==========================================================================================*/
char tmUART_CheckASCII_LRC( unsigned char *data, int data_num )
{
	int i;
	unsigned char LRC = 0;		// LRC from calculated data
    unsigned char D_LRC = 0;	// LRC from the buffer directly
		
	for ( i=0; i < ((data_num-5)/2); i++ ) {		
		LRC += tmUART_ASCII2UNCHAR( data[1+i*2], data[2+i*2] );
	}

	D_LRC = tmUART_ASCII2UNCHAR(data[data_num-4], data[data_num-3] );
	D_LRC = ( D_LRC + LRC ) & 0xFF;

	if( D_LRC != 0 ) // Check LRC
        return 0;
    else
        return 1;
}




/*===========================================================================================
    Function Name    : tmUART_CalculateCRC
    Input            : src : array data
                       data_lengh : data number
					   dec : CRC, dec[1] = High Byte, dec[0] = Low Byte
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_CalculateCRC( unsigned char * src, int data_lengh, unsigned char *dec )
{
	unsigned int CRC = 0xffff;
	int j = 0;
	int k = 0;
	int last_bit;

	for( j = 0; j < data_lengh; j++){	
		CRC ^= src[ j ];
		for( k = 0; k < 8; k++ ){
			last_bit = CRC & 0x01;
			CRC = CRC >> 1; 
			if( last_bit == 1 ){      
				CRC ^= MODBUS_RTU_CRC_CONST;
			}
		}
	}

	dec[1] = CRC / 256;
	dec[0] = CRC % 256;
   
}

/*===========================================================================================
    Function Name    : tmUART_ASCII2UNCHAR
    Input            : char_buffer_1 : ASCII char High Byte
                       char_buffer_0 : ASCII char Low Byte
    Return           : number calculated by char_buffer_1 & char_buffer_0
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : char_buffer_1 * 16 + char_buffer_0
//==========================================================================================*/
unsigned char tmUART_ASCII2UNCHAR(unsigned char char_buffer_1, unsigned char char_buffer_0)
{	
	unsigned char temp=0;
	unsigned char temp_0=0;
	unsigned char temp_1=0;
	if(char_buffer_1>64){
		temp_1 = ((char_buffer_1-0x41+10)*16);	//65 =>A
	}
	if(char_buffer_1<58){
		temp_1 = ((char_buffer_1-0x30)*16); //48 =>0
	}
	if(char_buffer_0>64){
		temp_0 = (char_buffer_0-0x41+10);	//65 =>A
	}
	if(char_buffer_0<58){
		temp_0 = (char_buffer_0-0x30); //48 =>0
	}
	temp = (temp_1 + temp_0);
	return (temp);
}
/*===========================================================================================
    Function Name    : tmUART_UNCHAR2ASCII
    Input            : char_buffer : input value
                       final_char : 2 ASCII chars
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : final_char[0] = Low Byte, final_char[1] = High Byte
//==========================================================================================*/
void tmUART_UNCHAR2ASCII(unsigned char char_buffer,unsigned char *final_char)
{
    //char final_char[2];	
	char trans_buffer_1 = (char_buffer/16);
	char trans_buffer_0 = (char_buffer - trans_buffer_1*16);
	if(trans_buffer_1>9){
		trans_buffer_1 = ((trans_buffer_1-10) + 65);	//65 =>A
	}else{
		trans_buffer_1 = (trans_buffer_1 + 48);		//48 =>0
	}
	if(trans_buffer_0>9){
		trans_buffer_0 = ((trans_buffer_0-10) + 65);	//65 =>A
	}else{
		trans_buffer_0 = (trans_buffer_0 + 48);		//48 =>0
	}
	final_char[0]=trans_buffer_0;
	final_char[1]=trans_buffer_1;
}

/*===========================================================================================
    Function Name    : tmUART_DataReceived
    Input            : serialPort
                       uart
					   mode: ASCII or RTU
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_DataReceived( System::IO::Ports::SerialPort^  serialPort, Struct_TMUART* uart, int	mode ) {

	int numbytes = serialPort->BytesToRead;
	array<unsigned char>^ ReadByte = gcnew array<unsigned char>( numbytes );
					
	if( numbytes < R_BUFFER_SIZE ){
		serialPort->Read( ReadByte, 0, numbytes );

		for( int i = 0; i < numbytes; i++ ){
			uart->R_Buffer[ uart->R_Pointer++ ] = ReadByte[i];

			switch( mode ){
			case MB_SLV_RTU:
				uart->get_data_flag = YES;
				uart->rtu_time_point_1 = uart->current_time;
				break;
			case MB_SLV_ASCII:
			default:
				if( ReadByte[i] == MB_ASCII_START_BYTE ){
					uart->R_Buffer[ 0 ] = MB_ASCII_START_BYTE;
					uart->R_Pointer = 1;    

				}else if( ReadByte[i] == MB_ASCII_END_BYTE2 ){
					uart->R_data_number = uart->R_Pointer;
					uart->R_Pointer = 0;
					uart->ReceiveFlag = UART_DONE;       	 
				}
				break;
			}
					
			if ( uart->R_Pointer >= R_BUFFER_SIZE ){
				uart->R_Pointer = 0;	
			}
		}
	}

}

/*===========================================================================================
    Function Name    : tmUART_Transmit_ResetSignal
    Input            : serialPort
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_Transmit_ResetSignal( System::IO::Ports::SerialPort^  serialPort )
{
	serialPort->DtrEnable = true;
	serialPort->DtrEnable = false;
}

/*===========================================================================================
    Function Name    : tmUART_ReceivedData_Process_ASCII
    Input            : uart
					   dst
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
void tmUART_ReceivedData_Process_ASCII( Struct_TMUART* uart, int *dst, int number )
{
	for( int i = 0; i < number; i++ ){
		dst[i] = tmUART_ASCII2UNCHAR( uart->R_Buffer[ i * 4 + 7 ], uart->R_Buffer[ i * 4 + 8 ] ) * 256 +
				 tmUART_ASCII2UNCHAR( uart->R_Buffer[ i * 4 + 9 ], uart->R_Buffer[ i * 4 + 10 ] );
	}
}

/*===========================================================================================
    Function Name    : tmUART_ReceivedData_Process_RTU
    Input            : uart
					   dst
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
void tmUART_ReceivedData_Process_RTU( Struct_TMUART* uart, int *dst, int number )
{
	for( int i = 0; i < number; i++ ){
		dst[i] = uart->R_Buffer[ i * 2 + 3 ] * 256 + uart->R_Buffer[ i * 2 + 4 ];
	}
}

/*===========================================================================================
    Function Name    : tmUART_ReceivedChar_Process_ASCII
    Input            : uart
					   dst
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
void tmUART_ReceivedChar_Process_ASCII( Struct_TMUART* uart, char *dst, int number )
{
	for( int i = 0; i < number; i++ ){
		dst[i] = uart->R_Buffer[ 7 + i ];
	}
}

/*===========================================================================================
    Function Name    : tmUART_ReceivedChar_Process_RTU
    Input            : uart
					   dst
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
void tmUART_ReceivedChar_Process_RTU( Struct_TMUART* uart, char *dst, int number )
{
	for( int i = 0; i < number; i++ ){
		dst[i] = uart->R_Buffer[ 3 + i ];
	}
}

/*===========================================================================================
    Function Name    : tmUART_CheckRequest
    Input            : serialPort
                       uart
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_CheckRequest( System::IO::Ports::SerialPort^  serialPort, Struct_TMUART* uart )
{
	if( uart->Request_Flag == YES ){
		uart->Request_Flag = NO;
		uart->current_time = 0;
		tmUART_DataTransmit( serialPort, uart, uart->T_data_number );
	}
}


/*===========================================================================================
    Function Name    : tmUART_DataTransmit
    Input            : serialPort
                       uart
					   num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
void tmUART_DataTransmit( System::IO::Ports::SerialPort^  serialPort, Struct_TMUART* uart, int	num )
{
	array<unsigned char>^ send_data = gcnew array<unsigned char>( num );

	for( int i = 0; i < num; i++ ){
		send_data[i] = uart->T_Buffer[i];
	}
	uart->ReceiveFlag = UART_BUSY;
	if( serialPort->IsOpen ){
		serialPort->Write( send_data, 0, num );
	}
}

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
void tmUART_Modbus_FC3_Master_RTU( Struct_TMUART* uart, int ID, int address_h, int address_l, int num )
{
	int i = 0;
	int j = 0;

	uart->T_Buffer[ i++ ] = ID;					//SlaveID
	uart->T_Buffer[ i++ ] = 3;					//Function Code

	uart->T_Buffer[ i++ ] = address_h;			// =>Address
	uart->T_Buffer[ i++ ] = address_l;			// =>Address

	uart->T_Buffer[ i++ ] = num / 256;			// => data num
	uart->T_Buffer[ i++ ] = num % 256;			// => data num

	unsigned char temp_data[2];
	tmUART_CalculateCRC( uart->T_Buffer, i, temp_data );

	uart->T_Buffer[ i++ ] = temp_data[0];
	uart->T_Buffer[ i++ ] = temp_data[1];

	uart->T_data_number = i;

	uart->RequestAddress_H = address_h;
	uart->RequestAddress_L = address_l;
	uart->Request_Num = num;
	uart->Request_ID = ID;
	uart->Request_Flag = YES;
}

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
void tmUART_Modbus_FC3_Master_ASCII( Struct_TMUART* uart, int ID, int address_h, int address_l, int num )
{
	int i = 0;
	int j = 0;
	unsigned char LRC = 0;
	unsigned char temp_data[2];

	uart->T_Buffer[ i++ ] = MB_ASCII_START_BYTE;

	tmUART_UNCHAR2ASCII( ID,temp_data );		// Slave ID
	uart->T_Buffer[i++] = temp_data[1];			 
	uart->T_Buffer[i++] = temp_data[0];			

	uart->T_Buffer[ i++ ] = '0';				// Function Code
	uart->T_Buffer[ i++ ] = '3';

	tmUART_UNCHAR2ASCII( address_h,temp_data );	// Address h
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];			

	tmUART_UNCHAR2ASCII( address_l,temp_data );	// Address l
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];			

	tmUART_UNCHAR2ASCII( num / 256,temp_data );	// data num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];			

	tmUART_UNCHAR2ASCII( num % 256,temp_data );	// data num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];

	LRC =	ID +
			3 +
			address_h +
			address_l +
			num / 256 +
			num % 256;
	
	LRC = ( 256 - LRC ) & 0xff;

	tmUART_UNCHAR2ASCII( LRC, temp_data);
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];
					
	uart->T_Buffer[i++] = MB_ASCII_END_BYTE1;			
	uart->T_Buffer[i++] = MB_ASCII_END_BYTE2;

	uart->T_data_number = i;

	uart->RequestAddress_H = address_h;
	uart->RequestAddress_L = address_l;
	uart->Request_Num = num;
	uart->Request_ID = ID;
	uart->Request_Flag = YES;

}

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
void tmUART_Modbus_FC6_Master_RTU( Struct_TMUART* uart, int ID, int address_h, int address_l, int num )
{
	int i = 0;
	int j = 0;

	uart->T_Buffer[ i++ ] = ID;					//SlaveID
	uart->T_Buffer[ i++ ] = 6;					//Function Code

	uart->T_Buffer[ i++ ] = address_h;			// =>Address
	uart->T_Buffer[ i++ ] = address_l;			// =>Address

	uart->T_Buffer[ i++ ] = num / 256;			// => data num
	uart->T_Buffer[ i++ ] = num % 256;			// => data num

	unsigned char temp_data[2];
	tmUART_CalculateCRC( uart->T_Buffer, i, temp_data );

	uart->T_Buffer[ i++ ] = temp_data[0];
	uart->T_Buffer[ i++ ] = temp_data[1];

	uart->T_data_number = i;

	uart->RequestAddress_H = address_h;
	uart->RequestAddress_L = address_l;
	uart->Request_ID = ID;
	uart->Request_Flag = YES;
}

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
void tmUART_Modbus_FC6_Master_ASCII( Struct_TMUART* uart, int ID, int address_h, int address_l, int num )
{
	int i = 0;
	int j = 0;
	unsigned char LRC = 0;
	unsigned char temp_data[2];

	uart->T_Buffer[ i++ ] = MB_ASCII_START_BYTE;

	tmUART_UNCHAR2ASCII( ID,temp_data );		// Slave ID
	uart->T_Buffer[i++] = temp_data[1];			 
	uart->T_Buffer[i++] = temp_data[0];			

	uart->T_Buffer[ i++ ] = '0';				// Function Code
	uart->T_Buffer[ i++ ] = '6';

	tmUART_UNCHAR2ASCII( address_h,temp_data );	// Address h
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];			

	tmUART_UNCHAR2ASCII( address_l,temp_data );	// Address l
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];			

	tmUART_UNCHAR2ASCII( num / 256,temp_data );	// data num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];			

	tmUART_UNCHAR2ASCII( num % 256,temp_data );	// data num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];

	LRC =	ID +
			6 +
			address_h +
			address_l +
			num / 256 +
			num % 256;
	
	LRC = ( 256 - LRC ) & 0xff;

	tmUART_UNCHAR2ASCII( LRC, temp_data);
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];
					
	uart->T_Buffer[i++] = MB_ASCII_END_BYTE1;			
	uart->T_Buffer[i++] = MB_ASCII_END_BYTE2;

	uart->T_data_number = i;

	uart->RequestAddress_H = address_h;
	uart->RequestAddress_L = address_l;
	uart->Request_ID = ID;
	uart->Request_Flag = YES;
}

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
void tmUART_Modbus_FC16_Master_RTU( Struct_TMUART* uart, int ID, int address_h, int address_l, int num, int* data )
{
	
	int i = 0;
	int j = 0;

	uart->T_Buffer[ i++ ] = ID;					//SlaveID
	uart->T_Buffer[ i++ ] = 16;					//Function Code

	uart->T_Buffer[ i++ ] = address_h;			// =>Address
	uart->T_Buffer[ i++ ] = address_l;			// =>Address

	uart->T_Buffer[ i++ ] = num / 256;			// => data num
	uart->T_Buffer[ i++ ] = num % 256;			// => data num

	uart->T_Buffer[ i++ ] = num * 2;			// => byte num

	for( j = 0; j < num; j++ ){
		uart->T_Buffer[ i++ ] = data[j] / 256;	// => data
		uart->T_Buffer[ i++ ] = data[j] % 256;	// => data
	}

	unsigned char temp_data[2];
	tmUART_CalculateCRC( uart->T_Buffer, i, temp_data );

	uart->T_Buffer[ i++ ] = temp_data[0];
	uart->T_Buffer[ i++ ] = temp_data[1];

	uart->T_data_number = i;

	uart->RequestAddress_H = address_h;
	uart->RequestAddress_L = address_l;
	uart->Request_ID = ID;
	uart->Request_Flag = YES;
	
}

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
void tmUART_Modbus_FC16_Master_ASCII( Struct_TMUART* uart, int ID, int address_h, int address_l, int num, int* data )
{
	
	int i = 0;
	int j = 0;
	unsigned char LRC = 0;
	unsigned char temp_data[2];

	uart->T_Buffer[ i++ ] = MB_ASCII_START_BYTE;

	tmUART_UNCHAR2ASCII( ID,temp_data );		// Slave ID
	uart->T_Buffer[i++] = temp_data[1];			 
	uart->T_Buffer[i++] = temp_data[0];

	uart->T_Buffer[ i++ ] = '1';				// Function Code
	uart->T_Buffer[ i++ ] = '0';

	tmUART_UNCHAR2ASCII( address_h,temp_data );	// Address h
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];		

	tmUART_UNCHAR2ASCII( address_l,temp_data );	// Address l
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];		

	tmUART_UNCHAR2ASCII( num / 256,temp_data );	// data num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];

	tmUART_UNCHAR2ASCII( num % 256,temp_data );	// data num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];

	tmUART_UNCHAR2ASCII( num * 2,temp_data );	// byte num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];

	LRC =	ID +
			16 +
			address_h +
			address_l +
			num / 256 +
			num % 256 +
			num * 2;

	for( j = 0; j < num; j++ ){

		tmUART_UNCHAR2ASCII( data[j] / 256,temp_data );	// data h
		uart->T_Buffer[i++] = temp_data[1];			
		uart->T_Buffer[i++] = temp_data[0];

		tmUART_UNCHAR2ASCII( data[j] % 256,temp_data );	// data l
		uart->T_Buffer[i++] = temp_data[1];			
		uart->T_Buffer[i++] = temp_data[0];

		LRC += ( data[j] / 256 + data[j] % 256 );
	}

	LRC = ( 256 - LRC ) & 0xff;

	tmUART_UNCHAR2ASCII( LRC, temp_data);
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];
					
	uart->T_Buffer[i++] = MB_ASCII_END_BYTE1;			
	uart->T_Buffer[i++] = MB_ASCII_END_BYTE2;

	uart->T_data_number = i;

	uart->RequestAddress_H = address_h;
	uart->RequestAddress_L = address_l;
	uart->Request_ID = ID;
	uart->Request_Flag = YES;
	
}

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
void tmUART_Modbus_FC16_Master_ASCII_CHAR( Struct_TMUART* uart, int ID, int address_h, int address_l, int num, int* data )
{
	int i = 0;
	int j = 0;
	unsigned char LRC = 0;
	unsigned char temp_data[2];

	uart->T_Buffer[ i++ ] = MB_ASCII_START_BYTE;

	tmUART_UNCHAR2ASCII( ID,temp_data );		// Slave ID
	uart->T_Buffer[i++] = temp_data[1];			 
	uart->T_Buffer[i++] = temp_data[0];

	uart->T_Buffer[ i++ ] = '1';				// Function Code
	uart->T_Buffer[ i++ ] = '0';

	tmUART_UNCHAR2ASCII( address_h,temp_data );	// Address h
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];		

	tmUART_UNCHAR2ASCII( address_l,temp_data );	// Address l
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];		

	tmUART_UNCHAR2ASCII( num / 256,temp_data );	// data num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];

	tmUART_UNCHAR2ASCII( num % 256,temp_data );	// data num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];

	tmUART_UNCHAR2ASCII( num * 2,temp_data );	// byte num
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];

	LRC =	ID +
			16 +
			address_h +
			address_l +
			num / 256 +
			num % 256 +
			num * 2;

	for( j = 0; j < num; j++ ){
	
		uart->T_Buffer[i++] = data[ j * 4 + 0 ];			
		uart->T_Buffer[i++] = data[ j * 4 + 1 ];	
		uart->T_Buffer[i++] = data[ j * 4 + 2 ];			
		uart->T_Buffer[i++] = data[ j * 4 + 3 ];

		LRC += tmUART_ASCII2UNCHAR( data[ j * 4 + 0 ], data[ j * 4 + 1 ] ) ;
		LRC += tmUART_ASCII2UNCHAR( data[ j * 4 + 2 ], data[ j * 4 + 3 ] ) ;
	}

	LRC = ( 256 - LRC ) & 0xff;

	tmUART_UNCHAR2ASCII( LRC, temp_data);
	uart->T_Buffer[i++] = temp_data[1];			
	uart->T_Buffer[i++] = temp_data[0];
					
	uart->T_Buffer[i++] = MB_ASCII_END_BYTE1;			
	uart->T_Buffer[i++] = MB_ASCII_END_BYTE2;

	uart->T_data_number = i;

	uart->RequestAddress_H = address_h;
	uart->RequestAddress_L = address_l;
	uart->Request_ID = ID;
	uart->Request_Flag = YES;
	
}

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
void tmUART_Modbus_FC16_Master_RTU_CHAR( Struct_TMUART* uart, int ID, int address_h, int address_l, int num, int* data )
{
	
	int i = 0;
	int j = 0;

	uart->T_Buffer[ i++ ] = ID;					//SlaveID
	uart->T_Buffer[ i++ ] = 16;					//Function Code

	uart->T_Buffer[ i++ ] = address_h;			// =>Address
	uart->T_Buffer[ i++ ] = address_l;			// =>Address

	uart->T_Buffer[ i++ ] = num / 256;			// => data num
	uart->T_Buffer[ i++ ] = num % 256;			// => data num

	uart->T_Buffer[ i++ ] = num * 2;			// => byte num

	for( j = 0; j < num / 2; j++ ){

		uart->T_Buffer[i++] = data[ j * 4 + 0 ];			
		uart->T_Buffer[i++] = data[ j * 4 + 1 ];	
		uart->T_Buffer[i++] = data[ j * 4 + 2 ];			
		uart->T_Buffer[i++] = data[ j * 4 + 3 ];

	}

	unsigned char temp_data[2];
	tmUART_CalculateCRC( uart->T_Buffer, i, temp_data );

	uart->T_Buffer[ i++ ] = temp_data[0];
	uart->T_Buffer[ i++ ] = temp_data[1];

	uart->T_data_number = i;

	uart->RequestAddress_H = address_h;
	uart->RequestAddress_L = address_l;
	uart->Request_ID = ID;
	uart->Request_Flag = YES;
	
}

/************************** <END OF FILE> *****************************************/

