/*===========================================================================================

    File Name       : tmMultiDrive.h

    Version         : V1_00_a

    Built Date      : 2018/03/19

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/03/19 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_MULTI_DRIVE_H
#define TM_MULTI_DRIVE_H
#include "tmModbusMaster.h"

#define MULTIDRIVE_ID				0

#define MULTIDRIVE_FC_65			65
#define MULTIDRIVE_FC_66			66
#define MULTIDRIVE_FC_67			67

#define MULTIDRIVE_FC_101			101
#define MULTIDRIVE_FC_102			102
#define MULTIDRIVE_FC_103			103

#define MULTIDRIVE_FEEDBACK_DATA_NUM		2
#define MULTIDRIVELITE_FEEDBACK_DATA_NUM	16
#define MULTIDRIVE_SUBID_NUM_MAX			8
#define MULTIDRIVE_ID_MAX					32

// Modbus Slave RTU
enum {

	// FC66 FC67
	MB_SLAVE_RTU_INDEX_FC66_FC67_ECHO_H = 2,
	MB_SLAVE_RTU_INDEX_FC66_FC67_ECHO_L = 3,

	MB_SLAVE_RTU_INDEX_FC66_FC67_DATA_1_H = 4,
	MB_SLAVE_RTU_INDEX_FC66_FC67_DATA_1_L = 5,

	// FC102 FC103
	MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_1_H = 2,
	MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_1_L = 3,

	MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_2_H = 4,
	MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_2_L = 5,
	MB_SLAVE_RTU_INDEX_FC102_FC103_CRC_1 = 6,
	MB_SLAVE_RTU_INDEX_FC102_FC103_CRC_2 = 7,
	MB_SLAVE_RTU_INDEX_FC102_FC103_NUM = 8
};

// Modbus Slave ASCII
enum {

	// FC66 FC67

	MB_SLAVE_ASCII_FC66_FC67_ECHO_H_0 = 5,
	MB_SLAVE_ASCII_FC66_FC67_ECHO_H_1 = 6,
	MB_SLAVE_ASCII_FC66_FC67_ECHO_L_0 = 7,
	MB_SLAVE_ASCII_FC66_FC67_ECHO_L_1 = 8,

	MB_SLAVE_ASCII_FC66_FC67_DATA_1_H_0 = 9,
	MB_SLAVE_ASCII_FC66_FC67_DATA_1_H_1 = 10,
	MB_SLAVE_ASCII_FC66_FC67_DATA_1_L_0 = 11,
	MB_SLAVE_ASCII_FC66_FC67_DATA_1_L_1 = 12,

	// FC102 FC103

	MB_SLAVE_ASCII_FC102_FC103_DATA_1_H_0 = 5,
	MB_SLAVE_ASCII_FC102_FC103_DATA_1_H_1 = 6,
	MB_SLAVE_ASCII_FC102_FC103_DATA_1_L_0 = 7,
	MB_SLAVE_ASCII_FC102_FC103_DATA_1_L_1 = 8,

	MB_SLAVE_ASCII_FC102_FC103_DATA_2_H_0 = 9,
	MB_SLAVE_ASCII_FC102_FC103_DATA_2_H_1 = 10,
	MB_SLAVE_ASCII_FC102_FC103_DATA_2_L_0 = 11,
	MB_SLAVE_ASCII_FC102_FC103_DATA_2_L_1 = 12

};

enum {

	MDL_CMD_ISTOP		= 0,			// Multi-Drive-Lite command ISTOP
	MDL_CMD_JG			= 1,			// Multi-Drive-Lite command JG

	MDL_CMD_FREE		= 5,			// Multi-Drive-Lite command FREE
	MDL_CMD_SVON		= 6,			// Multi-Drive-Lite command Servo ON
	MDL_CMD_SVOFF		= 7,			// Multi-Drive-Lite command Servo OFF
	MDL_CMD_RST			= 8,			// Multi-Drive-Lite command Alarm Reset
	MDL_CMD_BRAKE		= 9,			// Multi-Drive-Lite command Motor Short Brake

	MDL_CMD_NULL		= 99			// Multi-Drive-Lite command Do Nothing

};

enum{
	MULTI_CMD_STOP		= 0,			// Multi command STOP
	MULTI_CMD_JGF		= 1,			// Multi command JGF
	MULTI_CMD_JGR		= 2,            // Multi command JGR
	MULTI_CMD_JGS		= 3,			// Multi command JGS
	MULTI_CMD_JG0		= 4,            // Multi command JG0
	MULTI_CMD_FREE		= 5,			// Multi command FREE
	MULTI_CMD_SVON		= 6,			// Multi command Servo ON
	MULTI_CMD_SVOFF		= 7,			// Multi command Servo OFF

	MULTI_CMD_JG		= 10,			// Multi command JG
	MULTI_CMD_IMR		= 11,			// Multi command IMR
	MULTI_CMD_MR   		= 12,        	// Multi command MR
	MULTI_CMD_MA		= 13,         	// Multi command MA
	MULTI_CMD_CS		= 14,        	// Multi command CS
	MULTI_CMD_CMR   	= 15,        	// Multi command CMR
	MULTI_CMD_CMA		= 16,         	// Multi command CMA

	MULTI_CMD_NULL		= 99,			// Multi command Do Nothing

	MULTI_CMD_NOECHO	= 100,			// Multi command without response
};

enum {
	MDL_NO_ECHO = 0,

	MDL_ECHO_BITF_MOTOR_STATE = 0,
	MDL_ECHO_BITF_HALL_CNT,
	MDL_ECHO_BITF_SPD,
	MDL_ECHO_BITF_ERROR_INDEX,
	MDL_ECHO_BITF_IO_STATE,
	MDL_ECHO_BITF_BUS_V,
	MDL_ECHO_BITF_SHUNT_I,

	MDL_ECHO_BITF_NUM = 16
};


typedef struct{


	int		SubID;
	int		CMD;
	union {
		int		Data_1;
		int		Data;
	};
	union {
		int		Data_2;
		int		Echo;
	};

	int		FeedBack_FC;
	int		FeedBack_Num;
	int		FeedBack_Data[ MULTIDRIVELITE_FEEDBACK_DATA_NUM ];
	

}Struct_MultiDrive_SubDriver;



typedef struct{

	int		WriteFalg;
	int		IsMultiDrive;

	int		T_ID;
	int		T_FC;
	int		SubID_Num;
	int		Echo_Num;
	int		Echo_Cnt;

	Struct_MultiDrive_SubDriver			SubDriver[ MULTIDRIVE_SUBID_NUM_MAX ];

}Struct_MultiDrive;

/*===========================================================================================
    Function Name    : tmMultiDrive_Master_RTU
    Input            : modbus
                       multi_drive
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmMultiDrive_Master_RTU( Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive )
{
	int i = 0;
	int j = 0;

	modbus->T_Data[ i++ ] = multi_drive->T_ID;						// ID
	modbus->T_Data[ i++ ] = multi_drive->T_FC;						// Function Code

	modbus->T_Data[ i++ ] = multi_drive->SubID_Num;					// => SubId num

	for( j = 0; j < multi_drive->SubID_Num; j++ ){

		modbus->T_Data[ i++ ] = multi_drive->SubDriver[ j ].SubID;
		modbus->T_Data[ i++ ] = multi_drive->SubDriver[ j ].CMD;
		modbus->T_Data[ i++ ] = multi_drive->SubDriver[ j ].Data_1 / 256;
		modbus->T_Data[ i++ ] = multi_drive->SubDriver[ j ].Data_1 % 256;
		modbus->T_Data[ i++ ] = multi_drive->SubDriver[ j ].Data_2 / 256;
		modbus->T_Data[ i++ ] = multi_drive->SubDriver[ j ].Data_2 % 256;
	}

	unsigned char temp_data[2];
	tmModbus_CalculateCRC( modbus->T_Data, i, temp_data );

	modbus->T_Data[ i++ ] = temp_data[0];
	modbus->T_Data[ i++ ] = temp_data[1];

	modbus->T_ID = multi_drive->T_ID;
	modbus->T_FC = multi_drive->T_FC;
	modbus->T_DataBytes_Cnt = i;

}

/*===========================================================================================
    Function Name    : tmMultiDrive_Master_ASCII
    Input            : modbus
                       multi_drive
    Return           : Null
    Programmer       : Chaim.Chen@trumman.com.tw
    Description      : 
//==========================================================================================*/
static void tmMultiDrive_Master_ASCII( Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive )
{

	int i = 0;
	int j = 0;
	unsigned char LRC = 0;
	unsigned char temp_data[2];

	modbus->T_Data[ i++ ] = MB_ASCII_START_BYTE;

	tmModbus_UNCHAR2ASCII( multi_drive->T_ID,temp_data );				// Slave ID
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];			

	tmModbus_UNCHAR2ASCII( multi_drive->T_FC,temp_data );				// FC
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];

	tmModbus_UNCHAR2ASCII( multi_drive->SubID_Num,temp_data );		// SubID Num
	modbus->T_Data[i++] = temp_data[1];			 
	modbus->T_Data[i++] = temp_data[0];

	LRC =	multi_drive->T_ID +
			multi_drive->T_FC +
			multi_drive->SubID_Num;

	for( j = 0; j < multi_drive->SubID_Num; j++ ){

		tmModbus_UNCHAR2ASCII( multi_drive->SubDriver[ j ].SubID,temp_data );				// SubID
		modbus->T_Data[i++] = temp_data[1];			 
		modbus->T_Data[i++] = temp_data[0];

		tmModbus_UNCHAR2ASCII( multi_drive->SubDriver[ j ].CMD,temp_data );					// CMD
		modbus->T_Data[i++] = temp_data[1];			 
		modbus->T_Data[i++] = temp_data[0];

		tmModbus_UNCHAR2ASCII( multi_drive->SubDriver[ j ].Data_1 / 256,temp_data );		// Data 1 High
		modbus->T_Data[i++] = temp_data[1];			 
		modbus->T_Data[i++] = temp_data[0];

		tmModbus_UNCHAR2ASCII( multi_drive->SubDriver[ j ].Data_1 % 256,temp_data );		// Data 1 Low
		modbus->T_Data[i++] = temp_data[1];			  
		modbus->T_Data[i++] = temp_data[0];

		tmModbus_UNCHAR2ASCII( multi_drive->SubDriver[ j ].Data_2 / 256,temp_data );		// Data 2 High
		modbus->T_Data[i++] = temp_data[1];			 
		modbus->T_Data[i++] = temp_data[0];

		tmModbus_UNCHAR2ASCII( multi_drive->SubDriver[ j ].Data_2 % 256,temp_data );		// Data 2 Low
		modbus->T_Data[i++] = temp_data[1];			  
		modbus->T_Data[i++] = temp_data[0];


		LRC += (	multi_drive->SubDriver[ j ].SubID +
					multi_drive->SubDriver[ j ].CMD +
					multi_drive->SubDriver[ j ].Data_1 / 256 +
					multi_drive->SubDriver[ j ].Data_1 % 256 +
					multi_drive->SubDriver[ j ].Data_2 / 256 +
					multi_drive->SubDriver[ j ].Data_2 % 256 );

	}
	
	LRC = ( 256 - LRC ) & 0xff;

	tmModbus_UNCHAR2ASCII( LRC, temp_data);
	modbus->T_Data[i++] = temp_data[1];			
	modbus->T_Data[i++] = temp_data[0];
					
	modbus->T_Data[i++] = MB_ASCII_END_BYTE1;			
	modbus->T_Data[i++] = MB_ASCII_END_BYTE2;

	modbus->T_ID = multi_drive->T_ID;
	modbus->T_FC = multi_drive->T_FC;
	modbus->T_DataBytes_Cnt = i;

}

/*===========================================================================================
	Function Name    : tmMultiDrive_ReceivedData_Process_RTU
	Input            :	1. modbus
						2. multi_drive
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDrive_ReceivedData_Process_RTU(Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive)
{
	for (int i = 0; i < MULTIDRIVE_SUBID_NUM_MAX; i++) {
		if (modbus->R_ID == multi_drive->SubDriver[i].SubID) {
			multi_drive->SubDriver[i].FeedBack_FC = modbus->R_FC;
			multi_drive->SubDriver[i].FeedBack_Data[0] =
				modbus->R_Data[ MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_1_H ] * 256 +
				modbus->R_Data[ MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_1_L ];

			multi_drive->SubDriver[i].FeedBack_Data[1] =
				modbus->R_Data[ MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_2_H ] * 256 +
				modbus->R_Data[ MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_2_L ];

			break;
		}
	}
}

/*===========================================================================================
	Function Name    : tmMultiDrive_ReceivedData_Process_RTU
	Input            :	1. modbus
						2. multi_drive
						3. r_data
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDrive_ReceivedData_Process_RTU( Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive, unsigned char *r_data )
{
	for (int i = 0; i < MULTIDRIVE_SUBID_NUM_MAX; i++) {
		if (modbus->R_ID == multi_drive->SubDriver[i].SubID) {
			multi_drive->SubDriver[i].FeedBack_FC = modbus->R_FC;
			multi_drive->SubDriver[i].FeedBack_Data[0] =
				r_data[MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_1_H] * 256 +
				r_data[MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_1_L];

			multi_drive->SubDriver[i].FeedBack_Data[1] =
				r_data[MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_2_H] * 256 +
				r_data[MB_SLAVE_RTU_INDEX_FC102_FC103_DATA_2_L];

			break;
		}
	}
}

/*===========================================================================================
	Function Name    : tmMultiDrive_ReceivedData_Process_ASCII
	Input            :	1. modbus
						2. multi_drive
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDrive_ReceivedData_Process_ASCII(Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive)
{
	for (int i = 0; i < MULTIDRIVE_SUBID_NUM_MAX; i++) {
		if (modbus->R_ID == multi_drive->SubDriver[i].SubID) {
			multi_drive->SubDriver[i].FeedBack_FC = modbus->R_FC;
			multi_drive->SubDriver[i].FeedBack_Data[0] =
				tmModbus_ASCII2UNCHAR(	modbus->R_Data[MB_SLAVE_ASCII_FC102_FC103_DATA_1_H_0],
										modbus->R_Data[MB_SLAVE_ASCII_FC102_FC103_DATA_1_H_1]) * 256 +
				tmModbus_ASCII2UNCHAR(	modbus->R_Data[MB_SLAVE_ASCII_FC102_FC103_DATA_1_L_0],
										modbus->R_Data[MB_SLAVE_ASCII_FC102_FC103_DATA_1_L_1]);

			multi_drive->SubDriver[i].FeedBack_Data[1] =
				tmModbus_ASCII2UNCHAR(	modbus->R_Data[MB_SLAVE_ASCII_FC102_FC103_DATA_2_H_0],
										modbus->R_Data[MB_SLAVE_ASCII_FC102_FC103_DATA_2_H_1]) * 256 +
				tmModbus_ASCII2UNCHAR(	modbus->R_Data[MB_SLAVE_ASCII_FC102_FC103_DATA_2_L_0],
										modbus->R_Data[MB_SLAVE_ASCII_FC102_FC103_DATA_2_L_1]);

			break;
		}
	}
}

/*===========================================================================================
	Function Name    : tmMultiDrive_ReceivedData_Process_ASCII
	Input            :	1. modbus
						2. multi_drive
						3. r_data
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDrive_ReceivedData_Process_ASCII(Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive, unsigned char *r_data)
{
	for (int i = 0; i < MULTIDRIVE_SUBID_NUM_MAX; i++) {
		if (modbus->R_ID == multi_drive->SubDriver[i].SubID) {
			multi_drive->SubDriver[i].FeedBack_FC = modbus->R_FC;
			multi_drive->SubDriver[i].FeedBack_Data[0] =
				tmModbus_ASCII2UNCHAR(	r_data[MB_SLAVE_ASCII_FC102_FC103_DATA_1_H_0],
										r_data[MB_SLAVE_ASCII_FC102_FC103_DATA_1_H_1]) * 256 +
				tmModbus_ASCII2UNCHAR(	r_data[MB_SLAVE_ASCII_FC102_FC103_DATA_1_L_0],
										r_data[MB_SLAVE_ASCII_FC102_FC103_DATA_1_L_1]);

			multi_drive->SubDriver[i].FeedBack_Data[1] =
				tmModbus_ASCII2UNCHAR(	r_data[MB_SLAVE_ASCII_FC102_FC103_DATA_2_H_0],
										r_data[MB_SLAVE_ASCII_FC102_FC103_DATA_2_H_1]) * 256 +
				tmModbus_ASCII2UNCHAR(	r_data[MB_SLAVE_ASCII_FC102_FC103_DATA_2_L_0],
										r_data[MB_SLAVE_ASCII_FC102_FC103_DATA_2_L_1]);

			break;
		}
	}
}
/*===========================================================================================
	Function Name    : tmMultiDrive_ReceivedData_Process
	Input            :	1. modbus
						2. multi_drive
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDrive_ReceivedData_Process(Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive)
{
	if (modbus->Mode == MB_MODE_ASCII) {
		tmMultiDrive_ReceivedData_Process_ASCII(modbus, multi_drive);
	}else {
		tmMultiDrive_ReceivedData_Process_RTU(modbus, multi_drive);
	}
}

/*===========================================================================================
	Function Name    : tmMultiDrive_ReceivedData_Process
	Input            :	1. modbus
						2. multi_drive
						3. r_data
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDrive_ReceivedData_Process(Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive, unsigned char *r_data )
{
	if (modbus->Mode == MB_MODE_ASCII) {
		tmMultiDrive_ReceivedData_Process_ASCII(modbus, multi_drive, r_data );
	}
	else {
		tmMultiDrive_ReceivedData_Process_RTU(modbus, multi_drive, r_data );
	}
}

/*===========================================================================================
	Function Name    : tmMultiDriveLite_ReceivedData_Process_RTU
	Input            :	1. modbus
						2. multi_drive
						3. r_data
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDriveLite_ReceivedData_Process_RTU(Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive, unsigned char *r_data)
{
	int echo;
	int offset = 0;

	for (int i = 0; i < MULTIDRIVE_SUBID_NUM_MAX; i++) {
		if (modbus->R_ID == multi_drive->SubDriver[i].SubID) {
			multi_drive->SubDriver[i].FeedBack_FC = modbus->R_FC;
			echo =	r_data[MB_SLAVE_RTU_INDEX_FC66_FC67_ECHO_H] * 256 +
					r_data[MB_SLAVE_RTU_INDEX_FC66_FC67_ECHO_L];

			for (int j = 0; j < MULTIDRIVELITE_FEEDBACK_DATA_NUM; j++) {

				if ((echo >> j) & 0x01) {
					multi_drive->SubDriver[i].FeedBack_Data[j] =
						r_data[ offset + MB_SLAVE_RTU_INDEX_FC66_FC67_DATA_1_H] * 256 +
						r_data[ offset + MB_SLAVE_RTU_INDEX_FC66_FC67_DATA_1_L];


					offset += 2;
				}

			}

			break;
		}
	}
}

/*===========================================================================================
	Function Name    : tmMultiDriveLite_ReceivedData_Process_ASCII
	Input            :	1. modbus
						2. multi_drive
						3. r_data
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDriveLite_ReceivedData_Process_ASCII(Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive, unsigned char *r_data)
{
	int echo;
	int offset = 0;

	for (int i = 0; i < MULTIDRIVE_SUBID_NUM_MAX; i++) {
		if (modbus->R_ID == multi_drive->SubDriver[i].SubID) {
			multi_drive->SubDriver[i].FeedBack_FC = modbus->R_FC;
			echo =	tmModbus_ASCII2UNCHAR(	r_data[MB_SLAVE_ASCII_FC66_FC67_ECHO_H_0],
											r_data[MB_SLAVE_ASCII_FC66_FC67_ECHO_H_1]) * 256 +
					tmModbus_ASCII2UNCHAR(	r_data[MB_SLAVE_ASCII_FC66_FC67_ECHO_L_0],
											r_data[MB_SLAVE_ASCII_FC66_FC67_ECHO_L_1]);

			for (int j = 0; j < MULTIDRIVELITE_FEEDBACK_DATA_NUM; j++ ) {

				if ( ( echo >> j ) & 0x01 ) {
					multi_drive->SubDriver[i].FeedBack_Data[j] =
						tmModbus_ASCII2UNCHAR(	r_data[ offset + MB_SLAVE_ASCII_FC102_FC103_DATA_1_H_0 ],
												r_data[ offset + MB_SLAVE_ASCII_FC102_FC103_DATA_1_H_1 ]) * 256 +
						tmModbus_ASCII2UNCHAR(	r_data[ offset + MB_SLAVE_ASCII_FC102_FC103_DATA_1_L_0 ],
												r_data[ offset + MB_SLAVE_ASCII_FC102_FC103_DATA_1_L_1 ]);

					offset += 4;
				}

			}

			break;
		}
	}
}

/*===========================================================================================
	Function Name    : tmMultiDriveLite_ReceivedData_Process
	Input            :	1. modbus
						2. multi_drive
						3. r_data
	Return           : Null
	Programmer       : Chaim.Chen@trumman.com.tw
	Description      :
//==========================================================================================*/
static void tmMultiDriveLite_ReceivedData_Process(Struct_ModbusMaster* modbus, Struct_MultiDrive* multi_drive, unsigned char *r_data)
{
	if (modbus->Mode == MB_MODE_ASCII) {
		tmMultiDriveLite_ReceivedData_Process_ASCII(modbus, multi_drive, r_data);
	}
	else {
		tmMultiDriveLite_ReceivedData_Process_RTU(modbus, multi_drive, r_data);
	}
}

#endif

/************************** <END OF FILE> *****************************************/



