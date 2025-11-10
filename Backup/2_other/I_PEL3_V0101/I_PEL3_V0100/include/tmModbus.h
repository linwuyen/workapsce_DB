/*===========================================================================================

    File Name       : tmModbus.h

    Version         : V01_00

    Built Date      : 2018/06/12

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/06/12 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_MODBUS_H
#define TM_MODBUS_H

#define MODBUS_FC_00			0

#define MODBUS_FC_03			3
#define MODBUS_FC_06			6
#define MODBUS_FC_16			16

#define MODBUS_FC_03_BAD_FB		0x83
#define MODBUS_FC_06_BAD_FB		0x86
#define MODBUS_FC_16_BAD_FB		0x90

#define MODBUS_RTU_CRC_CONST	0xA001

/*===========================================================================================
    ModBus Slave serial Transmission Modes select
//==========================================================================================*/
enum{
	MB_MODE_ASCII						= 0,
	MB_MODE_RTU							= 1,
	MB_MODE_NUM							= 2
};

enum{
     MB_ASCII_START_BYTE				= 58,
     MB_ASCII_END_BYTE1					= 13,
     MB_ASCII_END_BYTE2					= 10
};

// Modbus Master RTU
enum{

	MB_MASTER_RTU_INDEX_ID = 0,
	MB_MASTER_RTU_INDEX_FC = 1,
	
	// FC3
	MB_MASTER_RTU_INDEX_FC3_ADDRESS_H = 2,
	MB_MASTER_RTU_INDEX_FC3_ADDRESS_L = 3,

	MB_MASTER_RTU_INDEX_FC3_DATANUM_H = 4,
	MB_MASTER_RTU_INDEX_FC3_DATANUM_L = 5,

	// FC6

	MB_MASTER_RTU_INDEX_FC6_ADDRESS_H = 2,
	MB_MASTER_RTU_INDEX_FC6_ADDRESS_L = 3,

	MB_MASTER_RTU_INDEX_FC6_VALUE_H = 4,
	MB_MASTER_RTU_INDEX_FC6_VALUE_L = 5,

	// FC10

	MB_MASTER_RTU_INDEX_FC10_ADDRESS_H = 2,
	MB_MASTER_RTU_INDEX_FC10_ADDRESS_L = 3,

	MB_MASTER_RTU_INDEX_FC10_DATANUM_H = 4,
	MB_MASTER_RTU_INDEX_FC10_DATANUM_L = 5,

	MB_MASTER_RTU_INDEX_FC10_BYTECNT = 6,

	MB_MASTER_RTU_INDEX_FC10_DATA0_H = 7,
	MB_MASTER_RTU_INDEX_FC10_DATA0_L = 8

};

// Modbus Master ASCII
enum{

	MB_MASTER_ASCII_STARTBYTE = 0,

	MB_MASTER_ASCII_INDEX_ID_0 = 1,
	MB_MASTER_ASCII_INDEX_ID_1 = 2,

	MB_MASTER_ASCII_INDEX_FC_0 = 3,
	MB_MASTER_ASCII_INDEX_FC_1 = 4,
	
	// FC3
	MB_MASTER_ASCII_INDEX_FC3_ADDRESS_H_0 = 5,
	MB_MASTER_ASCII_INDEX_FC3_ADDRESS_H_1 = 6,

	MB_MASTER_ASCII_INDEX_FC3_ADDRESS_L_0 = 7,
	MB_MASTER_ASCII_INDEX_FC3_ADDRESS_L_1 = 8,

	MB_MASTER_ASCII_INDEX_FC3_DATANUM_H_0 = 9,
	MB_MASTER_ASCII_INDEX_FC3_DATANUM_H_1 = 10,

	MB_MASTER_ASCII_INDEX_FC3_DATANUM_L_0 = 11,
	MB_MASTER_ASCII_INDEX_FC3_DATANUM_L_1 = 12,

	// FC6
	MB_MASTER_ASCII_INDEX_FC6_ADDRESS_H_0 = 5,
	MB_MASTER_ASCII_INDEX_FC6_ADDRESS_H_1 = 6,

	MB_MASTER_ASCII_INDEX_FC6_ADDRESS_L_0 = 7,
	MB_MASTER_ASCII_INDEX_FC6_ADDRESS_L_1 = 8,

	MB_MASTER_ASCII_INDEX_FC6_VALUE_H_0 = 9,
	MB_MASTER_ASCII_INDEX_FC6_VALUE_H_1 = 10,

	MB_MASTER_ASCII_INDEX_FC6_VALUE_L_0 = 11,
	MB_MASTER_ASCII_INDEX_FC6_VALUE_L_1 = 12,

	// FC10

	MB_MASTER_ASCII_INDEX_FC10_ADDRESS_H_0 = 5,
	MB_MASTER_ASCII_INDEX_FC10_ADDRESS_H_1 = 6,

	MB_MASTER_ASCII_INDEX_FC10_ADDRESS_L_0 = 7,
	MB_MASTER_ASCII_INDEX_FC10_ADDRESS_L_1 = 8,

	MB_MASTER_ASCII_INDEX_FC10_DATANUM_H_0 = 9,
	MB_MASTER_ASCII_INDEX_FC10_DATANUM_H_1 = 10,

	MB_MASTER_ASCII_INDEX_FC10_DATANUM_L_0 = 11,
	MB_MASTER_ASCII_INDEX_FC10_DATANUM_L_1 = 12,

	MB_MASTER_ASCII_INDEX_FC10_BYTECNT_0 = 13,
	MB_MASTER_ASCII_INDEX_FC10_BYTECNT_1 = 14,

	MB_MASTER_ASCII_INDEX_FC10_DATA0_H_0 = 15,
	MB_MASTER_ASCII_INDEX_FC10_DATA0_H_1 = 16,

	MB_MASTER_ASCII_INDEX_FC10_DATA0_L_0 = 17,
	MB_MASTER_ASCII_INDEX_FC10_DATA0_L_1 = 18

};

// Modbus Slave RTU
enum{

	MB_SLAVE_RTU_INDEX_ID = 0,
	MB_SLAVE_RTU_INDEX_FC = 1,
	
	// FC3

	MB_SLAVE_RTU_INDEX_FC3_BYTECNT = 2,
	MB_SLAVE_RTU_INDEX_FC3_DATA0_H = 3,
	MB_SLAVE_RTU_INDEX_FC3_DATA0_L = 4,

	// FC6

	MB_SLAVE_RTU_INDEX_FC6_ADDRESS_H = 2,
	MB_SLAVE_RTU_INDEX_FC6_ADDRESS_L = 3,

	MB_SLAVE_RTU_INDEX_FC6_VALUE_H = 4,
	MB_SLAVE_RTU_INDEX_FC6_VALUE_L = 5,

	// FC10

	MB_SLAVE_RTU_INDEX_FC10_ADDRESS_H = 2,
	MB_SLAVE_RTU_INDEX_FC10_ADDRESS_L = 3,

	MB_SLAVE_RTU_INDEX_FC10_DATANUM_H = 4,
	MB_SLAVE_RTU_INDEX_FC10_DATANUM_L = 5

};

// Modbus Slave ASCII
enum{

	MB_SLAVE_ASCII_STARTBYTE = 0,

	MB_SLAVE_ASCII_INDEX_ID_0 = 1,
	MB_SLAVE_ASCII_INDEX_ID_1 = 2,

	MB_SLAVE_ASCII_INDEX_FC_0 = 3,
	MB_SLAVE_ASCII_INDEX_FC_1 = 4,
	
	// FC3
	MB_SLAVE_ASCII_INDEX_FC3_BYTECNT_0 = 5,
	MB_SLAVE_ASCII_INDEX_FC3_BYTECNT_1 = 6,

	MB_SLAVE_ASCII_INDEX_FC3_DATA0_H_0 = 7,
	MB_SLAVE_ASCII_INDEX_FC3_DATA0_H_1 = 8,

	MB_SLAVE_ASCII_INDEX_FC3_DATA0_L_0 = 9,
	MB_SLAVE_ASCII_INDEX_FC3_DATA0_L_1 = 10,

	// FC6
	MB_SLAVE_ASCII_INDEX_FC6_ADDRESS_H_0 = 5,
	MB_SLAVE_ASCII_INDEX_FC6_ADDRESS_H_1 = 6,

	MB_SLAVE_ASCII_INDEX_FC6_ADDRESS_L_0 = 7,
	MB_SLAVE_ASCII_INDEX_FC6_ADDRESS_L_1 = 8,

	MB_SLAVE_ASCII_INDEX_FC6_VALUE_H_0 = 9,
	MB_SLAVE_ASCII_INDEX_FC6_VALUE_H_1 = 10,

	MB_SLAVE_ASCII_INDEX_FC6_VALUE_L_0 = 11,
	MB_SLAVE_ASCII_INDEX_FC6_VALUE_L_1 = 12,

	// FC10

	MB_SLAVE_ASCII_INDEX_FC10_ADDRESS_H_0 = 5,
	MB_SLAVE_ASCII_INDEX_FC10_ADDRESS_H_1 = 6,

	MB_SLAVE_ASCII_INDEX_FC10_ADDRESS_L_0 = 7,
	MB_SLAVE_ASCII_INDEX_FC10_ADDRESS_L_1 = 8,

	MB_SLAVE_ASCII_INDEX_FC10_DATANUM_H_0 = 9,
	MB_SLAVE_ASCII_INDEX_FC10_DATANUM_H_1 = 10,

	MB_SLAVE_ASCII_INDEX_FC10_DATANUM_L_0 = 11,
	MB_SLAVE_ASCII_INDEX_FC10_DATANUM_L_1 = 12

};

/*===========================================================================================
    Function Name    : tmModbus_CalculateCRC
    Input            : src : array data
                       data_lengh : data number
					   dst : CRC, dst[1] = High Byte, dst[0] = Low Byte
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmModbus_CalculateCRC( unsigned char * src, int data_lengh, unsigned char *dst )
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

	dst[1] = CRC / 256;
	dst[0] = CRC % 256;
   
}

/*===========================================================================================
    Function Name    : tmModbus_ASCII2UNCHAR
    Input            : char_buffer_1 : ASCII char High Byte
                       char_buffer_0 : ASCII char Low Byte
    Return           : number calculated by char_buffer_1 & char_buffer_0
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : char_buffer_1 * 16 + char_buffer_0
//==========================================================================================*/
static unsigned char tmModbus_ASCII2UNCHAR( unsigned char char_buffer_1, unsigned char char_buffer_0 )
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
    Function Name    : tmModbus_UNCHAR2ASCII
    Input            : char_buffer : input value
                       final_char : 2 ASCII chars
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : final_char[0] = Low Byte, final_char[1] = High Byte
//==========================================================================================*/
static void tmModbus_UNCHAR2ASCII( unsigned char char_buffer,unsigned char *final_char )
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
    Function Name    : tmModbus_CheckRTU_CRC
    Input            :	1. src
						2. data_num
    Return           : 1 = good, 0 = bad
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : For RTU
//==========================================================================================*/
static int tmModbus_CheckRTU_CRC( unsigned char* src, int data_num )
{
	unsigned char crc_calculation[2];
	unsigned char crc_received[2];

	tmModbus_CalculateCRC( src, data_num - 2, crc_calculation );

	crc_received[0] = src[ data_num - 2 ];
	crc_received[1] = src[ data_num - 1 ];

	if( crc_received[0] == crc_calculation[0] && crc_received[1] == crc_calculation[1] ){
		return 1;
	}else{
		return 0;
	}	
}


/*===========================================================================================
    Function Name    : tmModbus_CheckASCII_LRC
    Input            :	1. src
						2. data_num
    Return           : 1 = good, 0 = bad
    Programmer       : Chaum.Chen@trumman.com.tw
    Description      : For ASCII
//==========================================================================================*/
static char tmModbus_CheckASCII_LRC( unsigned char *src, int data_num )
{
	int i;
	unsigned char lrc_calculation = 0;		// LRC from calculated data
    unsigned char lrc_rceived = 0;			// LRC from the buffer directly
	unsigned char result;
		
	for ( i = 0; i < ( data_num - 5 ) / 2; i++ ) {		
		lrc_calculation += tmModbus_ASCII2UNCHAR( src[ 1  +i * 2 ], src[ 2 + i * 2 ] );
	}

	lrc_rceived = tmModbus_ASCII2UNCHAR( src[ data_num - 4 ], src[ data_num - 3 ] );
	result = ( lrc_calculation + lrc_rceived ) & 0xFF;

	if( result == 0 ){
        return 1;
	}else{
        return 0;
	}
}

/*===========================================================================================
	Function Name    : tmModbus_CheckFrame
	Input            :	1.mode
						2.src
						3.data_num
	Return           : 1 = good, 0 = bad
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static int tmModbus_CheckFrame(int mode, unsigned char* src, int data_num)
{
	int frame_is_good;
	if (mode == MB_MODE_ASCII) {
		frame_is_good = tmModbus_CheckASCII_LRC(src, data_num);
	}else {
		frame_is_good = tmModbus_CheckRTU_CRC(src, data_num);
	}
	return frame_is_good;
}

/*===========================================================================================
Function Name    : tmModbus_GetEchoID
Input            :	1.mode
					2.src
Return           : echo id
Programmer       : Chaim.Chen@trumman.com.tw
Description      :
//==========================================================================================*/
static int tmModbus_GetEchoID(int mode, unsigned char* src)
{
	int id;
	if (mode == MB_MODE_ASCII) {
		id = tmModbus_ASCII2UNCHAR( src[MB_SLAVE_ASCII_INDEX_ID_0], src[MB_SLAVE_ASCII_INDEX_ID_1]);
	}else {
		id = src[MB_SLAVE_RTU_INDEX_ID];
	}
	return id;
}

/*===========================================================================================
Function Name    : tmModbus_GetEchoFC
Input            :	1.mode
					2.src
Return           : echo function code
Programmer       : Chaim.Chen@trumman.com.tw
Description      :
//==========================================================================================*/
static int tmModbus_GetEchoFC(int mode, unsigned char* src)
{
	int fc;
	if (mode == MB_MODE_ASCII) {
		fc = tmModbus_ASCII2UNCHAR(src[MB_SLAVE_ASCII_INDEX_FC_0], src[MB_SLAVE_ASCII_INDEX_FC_1]);
	}else {
		fc = src[MB_SLAVE_RTU_INDEX_FC];
	}
	return fc;
}
#endif

/************************** <END OF FILE> *****************************************/



