/*===========================================================================================

    File Name       : tmModbusMaster.h

    Version         : V01_00

    Built Date      : 2018/06/12

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/06/12 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_MODBUS_MASTER_H
#define TM_MODBUS_MASTER_H

#include "tmModbus.h"

#define MODBUS_R_DATA_SIZE_MAX	100



typedef struct{

	int Mode;
	
	unsigned char *T_Data;
	unsigned char *R_Data;

	int	R_Data_Processed[ MODBUS_R_DATA_SIZE_MAX ];

	long int	T_BITF;
	int			T_ID;
	int			T_FC;
	int			T_DataBytes_Cnt;

	long int	R_BITF;
	int			R_ID;
	int			R_FC;
	int			R_ExceptionCode;

}Struct_ModbusMaster;


/*===========================================================================================
    Function Name    :	tmModbusMaster_VariableInitial
    Input            :	1.modbus
						2.mode: 0 = ASCII, 1 = RTU
    Return           :	Null
    Programmer       :	Chaim.Chen@trumman.com.tw
    Description      :	Variable initial
						This function must be called at least once
//==========================================================================================*/
static void tmModbusMaster_VariableInitial ( Struct_ModbusMaster* modbus, int mode )
{
	modbus->Mode = mode;

	modbus->T_BITF = 0;
	modbus->T_ID = 0;
	modbus->T_FC = 0;
	modbus->T_DataBytes_Cnt = 0;

	modbus->R_BITF = 0;
	modbus->R_ID = 0;
	modbus->R_FC = 0;
	modbus->R_ExceptionCode = 0;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Set_Mode
    Input            :	1.modbus
						2.mode
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Set Mode: 0 = ASCII, 1 = RTU
//==========================================================================================*/
static void tmModbusMaster_Set_Mode ( Struct_ModbusMaster* modbus, int mode )
{
	modbus->Mode = mode;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_T_Data_Assign
    Input            :	1.modbus
						2.t_data: array for transmit data ( type = char )
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : This function must be called to assign T Data
//==========================================================================================*/
static void tmModbusMaster_T_Data_Assign ( Struct_ModbusMaster* modbus, unsigned char *t_data )
{
	modbus->T_Data= t_data;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_R_Data_Assign
    Input            :	1.modbus
						2.r_data: array for receieve data ( type = char )
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : This function must be called to assign R Data
//==========================================================================================*/
static void tmModbusMaster_R_Data_Assign ( Struct_ModbusMaster* modbus, unsigned char *r_data )
{
	modbus->R_Data= r_data;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Set_T_ID
    Input            :	1.modbus
						2.t_id
    Return           : T_ID
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Get transmit ID
//==========================================================================================*/
static void tmModbusMaster_Set_T_ID ( Struct_ModbusMaster* modbus, int t_id )
{
	modbus->T_ID = t_id;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Get_Mode
    Input            :	1.modbus
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Get Mode: 0 = ASCII, 1 = RTU
//==========================================================================================*/
static int tmModbusMaster_Get_Mode ( Struct_ModbusMaster* modbus )
{
	return modbus->Mode;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Get_T_ID
    Input            :	1.modbus
    Return           : T_ID
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Get transmit ID
//==========================================================================================*/
static int tmModbusMaster_Get_T_ID ( Struct_ModbusMaster* modbus )
{
	return modbus->T_ID;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Get_T_FC
    Input            :	1.modbus
    Return           : T_FC
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Get transmit FC
//==========================================================================================*/
static int tmModbusMaster_Get_T_FC ( Struct_ModbusMaster* modbus )
{
	return modbus->T_FC;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Get_T_DataBytes_Cnt
    Input            :	1.modbus
    Return           : T_DataBytes_Cnt
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Get transmit DataBytes Cnt
//==========================================================================================*/
static int tmModbusMaster_Get_T_DataBytes_Cnt ( Struct_ModbusMaster* modbus )
{
	return modbus->T_DataBytes_Cnt;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Get_R_ID
    Input            :	1.modbus
    Return           : R_ID
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Get recieved ID
//==========================================================================================*/
static int tmModbusMaster_Get_R_ID ( Struct_ModbusMaster* modbus )
{
	return modbus->R_ID;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Get_R_FC
    Input            :	1.modbus
    Return           : R_FC
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Get recieved FC
//==========================================================================================*/
static int tmModbusMaster_Get_R_FC ( Struct_ModbusMaster* modbus )
{
	return modbus->R_FC;
}

/*===========================================================================================
    Function Name    : tmModbusMaster_Get_R_ExceptionCode 
    Input            :	1.modbus
    Return           : R_ExceptionCode 
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : Get recieved ExceptionCode
//==========================================================================================*/
static int tmModbusMaster_Get_R_ExceptionCode ( Struct_ModbusMaster* modbus )
{
	return modbus->R_ExceptionCode;
}




/*===========================================================================================
    Function Name    : tmUART_Modbus_FC3_Master_RTU
    Input            : 1. modbus
                       2. id
					   3. address
					   4. num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmModbusMaster_FC3_RTU( Struct_ModbusMaster* modbus, int id, int address, int num )
{
	int i = 0;
	int address_h = address / 256;
	int address_l = address & 0xff;
	int num_h = num / 256;
	int num_l = num & 0xff;
	unsigned char temp_data[2];

	modbus->T_Data[ i++ ] = id;					//SlaveID
	modbus->T_Data[ i++ ] = MODBUS_FC_03;		//Function Code

	modbus->T_Data[ i++ ] = address_h;			// =>Address
	modbus->T_Data[ i++ ] = address_l;			// =>Address

	modbus->T_Data[ i++ ] = num_h;				// => data num
	modbus->T_Data[ i++ ] = num_l;				// => data num

	
	tmModbus_CalculateCRC( modbus->T_Data, i, temp_data );

	modbus->T_Data[ i++ ] = temp_data[0];
	modbus->T_Data[ i++ ] = temp_data[1];

	modbus->T_ID = id;
	modbus->T_FC = MODBUS_FC_03;
	modbus->T_DataBytes_Cnt = i;

}

/*===========================================================================================
    Function Name    : tmModbusMaster_FC3_ASCII
    Input            : 1. modbus
                       2. id
					   3. address
					   4. num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmModbusMaster_FC3_ASCII( Struct_ModbusMaster* modbus, int id, int address, int num )
{
	int i = 0;
	int address_h = address / 256;
	int address_l = address & 0xff;
	int num_h = num / 256;
	int num_l = num & 0xff;
	unsigned char LRC = 0;
	unsigned char temp_data[2];

	modbus->T_Data[ i++ ] = MB_ASCII_START_BYTE;

	tmModbus_UNCHAR2ASCII( id,temp_data );					// Slave ID
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];			

	tmModbus_UNCHAR2ASCII( MODBUS_FC_03,temp_data );		// Function Code
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];	

	tmModbus_UNCHAR2ASCII( address_h,temp_data );			// Address h
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];				

	tmModbus_UNCHAR2ASCII( address_l,temp_data );			// Address l
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];			

	tmModbus_UNCHAR2ASCII( num_h,temp_data );				// num h
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];				

	tmModbus_UNCHAR2ASCII( num_l,temp_data );				// num l
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];

	LRC =	id +
			MODBUS_FC_03 +
			address_h +
			address_l +
			num_h +
			num_l;
	
	LRC = ( 256 - LRC ) & 0xff;

	tmModbus_UNCHAR2ASCII( LRC, temp_data );
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];
					
	modbus->T_Data[i++] = MB_ASCII_END_BYTE1;			
	modbus->T_Data[i++] = MB_ASCII_END_BYTE2;

	modbus->T_ID = id;
	modbus->T_FC = MODBUS_FC_03;
	modbus->T_DataBytes_Cnt = i;

}

/*===========================================================================================
    Function Name    : tmUART_Modbus_FC6_Master_RTU
    Input            : 1. modbus
                       2. id
					   3. address
					   4. data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmModbusMaster_FC6_RTU( Struct_ModbusMaster* modbus, int id, int address, int data )
{
	int i = 0;
	int address_h = address / 256;
	int address_l = address & 0xff;
	int data_h = data / 256;
	int data_l = data & 0xff;
	unsigned char temp_data[2];

	modbus->T_Data[ i++ ] = id;					//SlaveID
	modbus->T_Data[ i++ ] = MODBUS_FC_06;		//Function Code

	modbus->T_Data[ i++ ] = address_h;			// =>Address
	modbus->T_Data[ i++ ] = address_l;			// =>Address

	modbus->T_Data[ i++ ] = data_h;				// => data num
	modbus->T_Data[ i++ ] = data_l;				// => data num

	
	tmModbus_CalculateCRC( modbus->T_Data, i, temp_data );

	modbus->T_Data[ i++ ] = temp_data[0];
	modbus->T_Data[ i++ ] = temp_data[1];

	modbus->T_ID = id;
	modbus->T_FC = MODBUS_FC_06;
	modbus->T_DataBytes_Cnt = i;

}

/*===========================================================================================
    Function Name    : tmModbusMaster_FC6_ASCII
    Input            : 1. modbus
                       2. id
					   3. address
					   4. data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmModbusMaster_FC6_ASCII( Struct_ModbusMaster* modbus, int id, int address, int data )
{
	int i = 0;
	int address_h = address / 256;
	int address_l = address & 0xff;
	int data_h = data / 256;
	int data_l = data & 0xff;
	unsigned char LRC = 0;
	unsigned char temp_data[2];

	modbus->T_Data[ i++ ] = MB_ASCII_START_BYTE;

	tmModbus_UNCHAR2ASCII( id,temp_data );					// Slave ID
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];			

	tmModbus_UNCHAR2ASCII( MODBUS_FC_06,temp_data );		// Function Code
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];	

	tmModbus_UNCHAR2ASCII( address_h,temp_data );			// Address h
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];				

	tmModbus_UNCHAR2ASCII( address_l,temp_data );			// Address l
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];			

	tmModbus_UNCHAR2ASCII( data_h,temp_data );				// data h
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];				

	tmModbus_UNCHAR2ASCII( data_l,temp_data );				// data l
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];

	LRC =	id +
			MODBUS_FC_06 +
			address_h +
			address_l +
			data_h +
			data_l;
	
	LRC = ( 256 - LRC ) & 0xff;

	tmModbus_UNCHAR2ASCII( LRC, temp_data );
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];
					
	modbus->T_Data[i++] = MB_ASCII_END_BYTE1;			
	modbus->T_Data[i++] = MB_ASCII_END_BYTE2;

	modbus->T_ID = id;
	modbus->T_FC = MODBUS_FC_06;
	modbus->T_DataBytes_Cnt = i;

}

/*===========================================================================================
    Function Name    : tmModbusMaster_FC16_RTU
    Input            :	1. modbus
						2. id
						3. address
						4. num
						5. data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmModbusMaster_FC16_RTU( Struct_ModbusMaster* modbus, int id, int address, int num, int* data )
{
	
	int i = 0;
	int j = 0;
	int address_h = address / 256;
	int address_l = address & 0xff;
	int num_h = num / 256;
	int num_l = num & 0xff;
	unsigned char temp_data[2];

	modbus->T_Data[ i++ ] = id;					//SlaveID
	modbus->T_Data[ i++ ] = MODBUS_FC_16;		//Function Code

	modbus->T_Data[ i++ ] = address_h;			// =>Address
	modbus->T_Data[ i++ ] = address_l;			// =>Address

	modbus->T_Data[ i++ ] = num_h;				// => data num
	modbus->T_Data[ i++ ] = num_l;				// => data num

	modbus->T_Data[ i++ ] = num * 2;			// => byte num

	for( j = 0; j < num; j++ ){
		modbus->T_Data[ i++ ] = data[j] / 256;	// => data h
		modbus->T_Data[ i++ ] = data[j] & 0xff;	// => data l
	}

	tmModbus_CalculateCRC( modbus->T_Data, i, temp_data );

	modbus->T_Data[ i++ ] = temp_data[0];
	modbus->T_Data[ i++ ] = temp_data[1];

	modbus->T_ID = id;
	modbus->T_FC = MODBUS_FC_16;
	modbus->T_DataBytes_Cnt = i;
	
}

/*===========================================================================================
    Function Name    : tmModbusMaster_FC16_ASCII
    Input            :	1. modbus
						2. id
						3. address
						4. num
						5. data
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmModbusMaster_FC16_ASCII( Struct_ModbusMaster* modbus, int id, int address, int num, int* data )
{
	
	int i = 0;
	int j = 0;
	int address_h = address / 256;
	int address_l = address & 0xff;
	int num_h = num / 256;
	int num_l = num & 0xff;
	unsigned char LRC = 0;
	unsigned char temp_data[2];

	modbus->T_Data[ i++ ] = MB_ASCII_START_BYTE;

	tmModbus_UNCHAR2ASCII( id,temp_data );					// Slave ID
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];

	tmModbus_UNCHAR2ASCII( MODBUS_FC_16,temp_data );		// Function Code
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];

	tmModbus_UNCHAR2ASCII( address_h,temp_data );			// Address h
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];

	tmModbus_UNCHAR2ASCII( address_l,temp_data );			// Address l
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];	

	tmModbus_UNCHAR2ASCII( num_h,temp_data );				// num h
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];

	tmModbus_UNCHAR2ASCII( num_l,temp_data );				// num l
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];	

	tmModbus_UNCHAR2ASCII( num * 2,temp_data );				// byte num
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];


	LRC =	id +
			MODBUS_FC_16 +
			address_h +
			address_l +
			num_h +
			num_l +
			num * 2;

	for( j = 0; j < num; j++ ){

		tmModbus_UNCHAR2ASCII( data[j] / 256,temp_data );	// data h
		modbus->T_Data[i++] = temp_data[1];			 
		modbus->T_Data[i++] = temp_data[0];

		tmModbus_UNCHAR2ASCII( data[j] & 0xff,temp_data );	// data l
		modbus->T_Data[i++] = temp_data[1];			 
		modbus->T_Data[i++] = temp_data[0];

		LRC += ( data[j] / 256 ) + ( data[j] & 0xff );
	}

	LRC = ( 256 - LRC ) & 0xff;

	tmModbus_UNCHAR2ASCII( LRC, temp_data);
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];
					
	modbus->T_Data[i++] = MB_ASCII_END_BYTE1;			
	modbus->T_Data[i++] = MB_ASCII_END_BYTE2;

	modbus->T_ID = id;
	modbus->T_FC = MODBUS_FC_16;
	modbus->T_DataBytes_Cnt = i;
	
}

/*===========================================================================================
    Function Name    : tmModbusMaster_FC03_ReceivedData_Process_RTU
    Input            :	1. modbus
						2. dst
						3. num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
static void tmModbusMaster_FC03_ReceivedData_Process_RTU( Struct_ModbusMaster* modbus, int *dst, int num )
{
	int i;
	for( i = 0; i < num; i++ ){
		dst[i] =	modbus->R_Data[ MB_SLAVE_RTU_INDEX_FC3_DATA0_H + i * 2 ] * 256 +
					modbus->R_Data[ MB_SLAVE_RTU_INDEX_FC3_DATA0_L + i * 2 ];
	}
}

/*===========================================================================================
    Function Name    : tmModbusMaster_FC03_ReceivedData_Process_ASCII
    Input            :	1. modbus
						2. dst
						3. num
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : for modbus FC3 return data
//==========================================================================================*/
static void tmModbusMaster_FC03_ReceivedData_Process_ASCII( Struct_ModbusMaster* modbus, int *dst, int num )
{
	int i;
	for( i = 0; i < num; i++ ){
		dst[i] = tmModbus_ASCII2UNCHAR( modbus->R_Data[ MB_SLAVE_ASCII_INDEX_FC3_DATA0_H_0 + i * 4 ],
										modbus->R_Data[ MB_SLAVE_ASCII_INDEX_FC3_DATA0_H_1 + i * 4 ] ) * 256 +
				 tmModbus_ASCII2UNCHAR( modbus->R_Data[ MB_SLAVE_ASCII_INDEX_FC3_DATA0_L_0 + i * 4 ],
										modbus->R_Data[ MB_SLAVE_ASCII_INDEX_FC3_DATA0_L_1 + i * 4 ] );
	}
}

/*===========================================================================================
Function Name    : tmModbusMaster_FC03_ReceivedData_Process
Input            :	1. modbus
					2. dst
					3. num
Return           : 1 = good, 0 = bad
Programmer       : Chaim.Chen@trumman.com.tw
Description      :
//==========================================================================================*/
static int tmModbusMaster_FC03_ReceivedData_Process(Struct_ModbusMaster* modbus, int *dst, int num)
{
	if (modbus->R_FC == MODBUS_FC_03) {
		if (modbus->Mode == MB_MODE_ASCII) {
			tmModbusMaster_FC03_ReceivedData_Process_ASCII(modbus, dst, num);
		}else {
			tmModbusMaster_FC03_ReceivedData_Process_RTU(modbus, dst, num);
		}
		return 1;
	}else {
		return 0;
	}
}

#endif

/************************** <END OF FILE> *****************************************/



