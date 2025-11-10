/*===========================================================================================

File Name       : tmDrive.cpp

Version         : V01_00

Built Date      : 2018/08/13

Release Date    : Not Yet

Programmer      : Chaim.Chen@trumman.com.tw

Description     :

=========================================================================================

History         : 2018/08/13 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#include "stdafx.h"
#include <iostream>
#include <string>
#include "tmDrive_V0100\tmDrive.h"


using namespace std;
using namespace boost::asio;


namespace TM_Drive {

	SerialModbus::SerialModbus( const std::string port_name)
	{

		tmModbusMaster_VariableInitial( &ModbusMaster, MB_MODE_ASCII );
		tmModbusMaster_T_Data_Assign( &ModbusMaster, T_Data );
		tmModbusMaster_R_Data_Assign( &ModbusMaster, R_Data );

		Set_Pa( 
			port_name,
			MB_MODE_ASCII,
			TM_DEF_BAUD_RATE,
			TM_DEF_DATA_BITS,
			TM_DEF_PARITY,
			TM_DEF_STOP_BITS );
	}

	SerialModbus::SerialModbus( const std::string port_name, int mode, int baud_rate, int data_bits, int parity, int stop_bits )
	{

		tmModbusMaster_VariableInitial(&ModbusMaster, mode );
		tmModbusMaster_T_Data_Assign( &ModbusMaster, T_Data );
		tmModbusMaster_R_Data_Assign( &ModbusMaster, R_Data );

		Set_Pa(
			port_name,
			mode,
			baud_rate,
			data_bits,
			parity,
			stop_bits );
	}

	SerialModbus::~SerialModbus() {
		Stop();
		Close();
	}

	void SerialModbus::Set_Pa(const std::string port_name, int mode, int baud_rate, int data_bits, int parity, int stop_bits)
	{
		ModbusMaster.Mode = mode;
		Port_Name = port_name;
		BaudRate = baud_rate;
		DataBits = data_bits;
		Parity = parity;
		StopBits = stop_bits;
	}

	void SerialModbus::Set_TimeOutPerid(int period)
	{
		TimeOut_Period = period;
	}

	int SerialModbus::Open()
	{
		if ( Serial_Port != NULL) {
			return -1;
		}

		if ( IO_Service ) {
			Serial_Port.reset();
			IO_Service.reset();
		}

		Mutex = std::shared_ptr<boost::mutex>{ new boost::mutex };
		//Func_Mutex = std::shared_ptr<boost::mutex>{ new boost::mutex };
		IO_Service = std::shared_ptr<io_service>{ new io_service() };
		Serial_Port = std::shared_ptr<serial_port>{ new serial_port( *IO_Service ) };

		try{
			Serial_Port->open( Port_Name );
			Serial_Port->set_option( serial_port_base::baud_rate( BaudRate ) );
			Serial_Port->set_option( serial_port_base::character_size( DataBits ) );

			if ( Parity == 0 ) {
				Serial_Port->set_option( serial_port_base::parity( serial_port_base::parity::none ) );
			}else if ( Parity == 1 ) {
				Serial_Port->set_option( serial_port_base::parity( serial_port_base::parity::odd ) );
			}else {
				Serial_Port->set_option( serial_port_base::parity( serial_port_base::parity::even ) );
			}
			if ( StopBits == 1 ) {
				Serial_Port->set_option( serial_port_base::stop_bits( serial_port_base::stop_bits::one ) );
			}else {
				Serial_Port->set_option( serial_port_base::stop_bits( serial_port_base::stop_bits::two ) );
			}

			return 1;
		}catch (std::exception &ex) {
			std::cout << "open exception : " << ex.what() << std::endl;
			return 0;
		}

	}

	int SerialModbus::Close()
	{
		if ( Serial_Port && Serial_Port->is_open() ) {
			Serial_Port->close();
		}
		return 1;
	}
	void SerialModbus::Run()
	{
		if ( !Thread_Flag ) {
			//Open();
			if ( Open() ) {
				Thread_Flag = true;
				Received_Thread_Pointer = std::shared_ptr<boost::thread>(new boost::thread(boost::bind(&SerialModbus::Received_Thread, this)));
				DataProcess_Thread_Pointer = std::shared_ptr<boost::thread>(new boost::thread(boost::bind(&SerialModbus::DataProcess_Thread, this)));
				Main_Thread_Pointer = std::shared_ptr<boost::thread>(new boost::thread(boost::bind(&SerialModbus::Main_Thread, this)));
			}
		}
	}

	void SerialModbus::Received_Thread()
	{
		//if ( Received_Thread_Flag && Serial_Port && Serial_Port->is_open() ) {
		//while ( Thread_Flag ) {
		while (1) {

			if (Serial_Port && Serial_Port->is_open()) {

				Dummy_Time = boost::posix_time::microsec_clock::local_time();	// dummy read, without this action, the following time read will be a little longer, why?

				char data[1]{ 0 };
				Serial_Port->read_some(buffer(data, 1));

				{

					boost::mutex::scoped_lock lock(*Mutex);

					R_Data[R_Data_Index++] = data[0];

					switch (ModbusMaster.Mode) {
					case MB_MODE_RTU:
						RTU_TP1 = boost::posix_time::microsec_clock::local_time();	// this is "the following" as above mentioned
						RTU_GetData_Flag = 1;
						break;
					case MB_MODE_ASCII:
					default:

						if (data[0] == MB_ASCII_START_BYTE) {
							R_Data[0] = MB_ASCII_START_BYTE;
							R_Data_Index = 1;
						}
						else if (data[0] == MB_ASCII_END_BYTE2) {
							R_Data_Cnt = R_Data_Index;
							R_Data_Index = 0;
							Received_State = RECEIVED_STATE_DONE;
						}
						break;
					}

					if (R_Data_Index >= TM_R_DATA_SIZE) {
						R_Data_Index = 0;
					}
				}
			}

		}
	}

	void SerialModbus::DataProcess_Thread()
	{
		//while (Thread_Flag) {
		while (1) {

			CheckRTUframe();
			CheckTimeOut();

			DataProcess();
		}
	}

	void SerialModbus::Main_Thread()
	{
		
		
		

	}


	void SerialModbus::Stop()
	{
		if (Thread_Flag) {

			//Write_Reg_Single(2, 0x1400, 0);

			Thread_Flag = false;

			//Received_Thread_Pointer->join();
			//DataProcess_Thread_Pointer->join();
			Main_Thread_Pointer->join();
		}
	}

	void SerialModbus::CheckRTUframe()
	{
		if (RTU_GetData_Flag) {

			boost::posix_time::ptime			dummy_tp1;

			dummy_tp1 = RTU_TP1;
			RTU_TP2 = boost::posix_time::microsec_clock::local_time();

			boost::posix_time::time_duration gap = RTU_TP2 - dummy_tp1;

			//if ( gap.total_milliseconds() >= RTU_FrameLength ) {
			if( ( Current_CmdType == CMD_TYPE_NORMAL_MODBUS && gap.total_milliseconds() >= RTU_FrameLength ) ||
				( Current_CmdType != CMD_TYPE_NORMAL_MODBUS && gap.total_milliseconds() >= MultiDive_FrameLength ) ){
				
				boost::mutex::scoped_lock lock(*Mutex);

				RTU_GetData_Flag = 0;
				R_Data_Cnt = R_Data_Index;
				R_Data_Index = 0;

				Received_State = RECEIVED_STATE_DONE;
					
			}
		}
	}

	void SerialModbus::CheckTimeOut()
	{
		if (Received_State == RECEIVED_STATE_BUSY) {

			boost::posix_time::ptime			dummy_tp;
			dummy_tp = TimeStamp;

			TimeNow = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_duration gap = TimeNow - dummy_tp;

			if (gap.total_milliseconds() > TimeOut_Period &&
				!RTU_GetData_Flag ) {	// This condition is very critical!!!  Caused by CPU lag...

				boost::mutex::scoped_lock lock(*Mutex);

				RTU_GetData_Flag = 0;
				R_Data_Index = 0;
				cout << "No response..." << endl;

				TimeOut_Flag = true;

				Received_State = RECEIVED_STATE_READY;
				
			}

		}
	}

	void SerialModbus::MultiDriveDataProcess()
	{
		int data_index = 0;
		int data_cnt;
		unsigned char *data;

		for (int i = 0; i < MultiDrive.SubID_Num; i++) {

			if (MultiDrive.SubDriver[i].FeedBack_Num != 0) {

				if (data_index < R_Data_Cnt) {

					data = &R_Data[data_index];
					data_cnt = MB_SLAVE_RTU_INDEX_FC102_FC103_NUM;

					if (tmModbus_CheckFrame(ModbusMaster.Mode, data, data_cnt)) {

						ModbusMaster.R_ID = tmModbus_GetEchoID(ModbusMaster.Mode, data);
						ModbusMaster.R_FC = tmModbus_GetEchoFC(ModbusMaster.Mode, data);

						tmMultiDrive_ReceivedData_Process(&ModbusMaster, &MultiDrive, data);

					}
					else {
						FrameError_Flag = true;
						cout << "Frame Error..." << endl;
						for (int i = 0; i < data_cnt; i++) {
							cout << (int)data[i] << " ";
						}
						cout << endl;
					}

					data_index += data_cnt;

				}else {
					cout << "Id : " << (int)MultiDrive.SubDriver[i].SubID << " no response..." << endl;
					break;
				}

			}
		}
	}

	void SerialModbus::MultiDriveLiteDataProcess()
	{
		int data_index = 0;
		int data_cnt;
		unsigned char *data;

		for (int i = 0; i < MultiDriveLite.SubID_Num; i++ ) {

			if ( MultiDriveLite.SubDriver[i].FeedBack_Num != 0 ) {

				if (data_index < R_Data_Cnt) {

					data = &R_Data[data_index];
					data_cnt = 6 + 2 * MultiDriveLite.SubDriver[i].FeedBack_Num;

					if (tmModbus_CheckFrame(ModbusMaster.Mode, data, data_cnt)) {

						ModbusMaster.R_ID = tmModbus_GetEchoID(ModbusMaster.Mode, data);
						ModbusMaster.R_FC = tmModbus_GetEchoFC(ModbusMaster.Mode, data);

						tmMultiDriveLite_ReceivedData_Process(&ModbusMaster, &MultiDriveLite, data);

					}
					else {
						FrameError_Flag = true;
						cout << "Frame Error..." << endl;
						for (int i = 0; i < data_cnt; i++) {
							cout << (int)data[i] << " ";
						}
						cout << endl;
					}

					data_index += data_cnt;

				}else {
					cout << "Id : " << (int)MultiDriveLite.SubDriver[i].SubID << " no response..." << endl;
					break;
				}

			}
		}
	}

	void SerialModbus::DataProcess()
	{
		if (Received_State == RECEIVED_STATE_DONE) {
		
			boost::mutex::scoped_lock lock(*Mutex);

			if (Current_CmdType == CMD_TYPE_MULTI_DRIVE) {
				MultiDriveDataProcess();
			}
			else if (Current_CmdType == CMD_TYPE_MULTI_DRIVE_LITE) {
				MultiDriveLiteDataProcess();
			}
			else if (tmModbus_CheckFrame(ModbusMaster.Mode, R_Data, R_Data_Cnt)) {

				ModbusMaster.R_ID = tmModbus_GetEchoID(ModbusMaster.Mode, R_Data);
				ModbusMaster.R_FC = tmModbus_GetEchoFC(ModbusMaster.Mode, R_Data);

				switch (ModbusMaster.R_FC) {
				case MODBUS_FC_03:

					break;
				case MODBUS_FC_06:
				case MODBUS_FC_16:
					break;
				case MODBUS_FC_03_BAD_FB:
				case MODBUS_FC_06_BAD_FB:
				case MODBUS_FC_16_BAD_FB:
					CMD_Deny_Flag = true;
					cout << "CMD Deney..." << endl;
					break;
				case MULTIDRIVE_FC_66:
				case MULTIDRIVE_FC_67:
					tmMultiDriveLite_ReceivedData_Process(&ModbusMaster, &MultiDriveLite, R_Data);
					break;
				case MULTIDRIVE_FC_102:
				case MULTIDRIVE_FC_103:
					tmMultiDrive_ReceivedData_Process(&ModbusMaster, &MultiDrive);
					break;
				default:
					cout << "Unknow FC..." << endl;
					break;
				}

			}
			else {
				FrameError_Flag = true;
				cout << "Frame Error..." << endl;
				for (int i = 0; i < R_Data_Cnt; i++) {
					cout << (int)R_Data[i] << " ";
				}
				cout << endl;
			}
			Received_State = RECEIVED_STATE_READY;

		}

	}


	int SerialModbus::Write_Reg_Single(int id, int address, int vlaue, bool need_response )
	{

		if (Serial_Port->is_open()) {

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			{
				boost::mutex::scoped_lock lock(*Mutex);

				Current_CmdType = CMD_TYPE_NORMAL_MODBUS;

				if (ModbusMaster.Mode == MB_MODE_ASCII) {
					tmModbusMaster_FC6_ASCII(&ModbusMaster, id, address, vlaue);
				}
				else {
					tmModbusMaster_FC6_RTU(&ModbusMaster, id, address, vlaue);
				}

				CMD_Deny_Flag = false;
				TimeOut_Flag = false;
				FrameError_Flag = false;

				R_Data_Index = 0;
				TimeStamp = boost::posix_time::microsec_clock::local_time();

				if (need_response) {
					//boost::mutex::scoped_lock func_lock(*Func_Mutex);
					Received_State = RECEIVED_STATE_BUSY;			
				}

				Serial_Port->write_some(buffer(ModbusMaster.T_Data, ModbusMaster.T_DataBytes_Cnt));
			}

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			return 1;
		}else {
			return 0;
		}
	}

	int SerialModbus::Write_Reg_Multiple( int id, int address, int data_cnt, int* data, bool need_response)
	{

		if (Serial_Port->is_open()) {

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			{
				boost::mutex::scoped_lock lock(*Mutex);

				Current_CmdType = CMD_TYPE_NORMAL_MODBUS;

				if (ModbusMaster.Mode == MB_MODE_ASCII) {
					tmModbusMaster_FC16_ASCII(&ModbusMaster, id, address, data_cnt, data);
				}else {
					tmModbusMaster_FC16_RTU(&ModbusMaster, id, address, data_cnt, data);

				}

				CMD_Deny_Flag = false;
				TimeOut_Flag = false;
				FrameError_Flag = false;

				R_Data_Index = 0;
			
				TimeStamp = boost::posix_time::microsec_clock::local_time();

				if (need_response) {
					//boost::mutex::scoped_lock func_lock(*Func_Mutex);
					Received_State = RECEIVED_STATE_BUSY;
				}

				Serial_Port->write_some(buffer(ModbusMaster.T_Data, ModbusMaster.T_DataBytes_Cnt));
			}

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			return 1;
		}
		else {
			return 0;
		}
	}

	int SerialModbus::Read_Reg(int id, int address, int data_cnt, int *r_data)
	{
		if (Serial_Port->is_open()) {

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}
			{
				boost::mutex::scoped_lock lock(*Mutex);

				Current_CmdType = CMD_TYPE_NORMAL_MODBUS;

				if (ModbusMaster.Mode == MB_MODE_ASCII) {
					tmModbusMaster_FC3_ASCII(&ModbusMaster, id, address, data_cnt );
				}else {
					tmModbusMaster_FC3_RTU(&ModbusMaster, id, address, data_cnt );
				}

				CMD_Deny_Flag = false;
				TimeOut_Flag = false;
				FrameError_Flag = false;

				R_Data_Index = 0;

				TimeStamp = boost::posix_time::microsec_clock::local_time();

				//boost::mutex::scoped_lock func_lock(*Func_Mutex);
				Received_State = RECEIVED_STATE_BUSY;

				Serial_Port->write_some(buffer(ModbusMaster.T_Data, ModbusMaster.T_DataBytes_Cnt));

			}

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			if (TimeOut_Flag) {
				return 0;
			}else if( FrameError_Flag ) {
				return 0;
			}else if (!tmModbusMaster_FC03_ReceivedData_Process( &ModbusMaster, r_data, data_cnt )) {
				cout << "Read error..." << endl;
				return 0;
			}
			return 1;
		}else {
			return 0;
		}
	}

	int SerialModbus::Check_Reg( int id, int address, int data )
	{
		int r_data;
		if ( Read_Reg(id, address, 1, &r_data) ) {
			if ( r_data == data ) {
				return 1;
			}else {
				return Write_Reg_Single( id, address, data );
			}
		} else {
			return 0;
		}
	}

	int SerialModbus::SendCmd_AlarmRest( int id, bool need_response )
	{
		return Write_Reg_Single(id, DRIVE_ADDRESS_CMD_ALM_RESET, 1, need_response);
	}

	int SerialModbus::SendCmd_Configure(int id, bool need_response)
	{
		return Write_Reg_Single(id, DRIVE_ADDRESS_CMD_CONFIGURE, 1, need_response);
	}

	// Multi-Drive
	int SerialModbus::SetCmd_MultiDrive(int drive_index, int id, int cmd, int data_1, int data_2)
	{
		if (drive_index < MULTIDRIVE_SUBID_NUM_MAX) {
			MultiDrive.SubDriver[drive_index].SubID = id & 0xFFFF;
			MultiDrive.SubDriver[drive_index].CMD = cmd & 0xFFFF;
			MultiDrive.SubDriver[drive_index].Data_1 = data_1 & 0xFFFF;
			MultiDrive.SubDriver[drive_index].Data_2 = data_2 & 0xFFFF;
			return 1;
		}else {
			cout << "Out of range : max drive cnt..." << endl;
			return 0;
		}
	}

	int SerialModbus::SendCmd_MultiDrive( int drive_cnt )
	{

		if (Serial_Port->is_open()) {

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			{
				boost::mutex::scoped_lock lock(*Mutex);

				MultiDrive.T_ID = MULTIDRIVE_ID;
				MultiDrive.T_FC = MULTIDRIVE_FC_101;
				MultiDrive.SubID_Num = drive_cnt;
				MultiDrive.Echo_Cnt = 0;
				MultiDrive.Echo_Num = 0;

				if (ModbusMaster.Mode == MB_MODE_ASCII) {
					tmMultiDrive_Master_ASCII(&ModbusMaster, &MultiDrive);
				}
				else {
					tmMultiDrive_Master_RTU(&ModbusMaster, &MultiDrive);
				}

				CMD_Deny_Flag = false;
				TimeOut_Flag = false;
				FrameError_Flag = false;

				R_Data_Index = 0;

				TimeStamp = boost::posix_time::microsec_clock::local_time();

				for (int i = 0; i < MultiDrive.SubID_Num; i++) {
					if (MultiDrive.SubDriver[i].CMD < MULTI_CMD_NOECHO) {
						MultiDrive.Echo_Num++;

						MultiDrive.SubDriver[i].FeedBack_Num = 2;
					}
				}
				if (MultiDrive.Echo_Num != 0) {
					//boost::mutex::scoped_lock func_lock(*Func_Mutex);
					if (MultiDrive.Echo_Num == 1 || ModbusMaster.Mode == MB_MODE_ASCII) {
						Current_CmdType = CMD_TYPE_NORMAL_MODBUS;
					}
					else {
						Current_CmdType = CMD_TYPE_MULTI_DRIVE;
					}
					Received_State = RECEIVED_STATE_BUSY;
				}


				Serial_Port->write_some(buffer(ModbusMaster.T_Data, ModbusMaster.T_DataBytes_Cnt));

			}

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			if (TimeOut_Flag) {
				return 0;
			}else if (FrameError_Flag) {
				return 0;
			}	
			return 1;
		}else {
			return 0;
		}
	}

	// Multi-Drive Lite
	int SerialModbus::SetCmd_MultiDriveLite(int drive_index, int id, int cmd, int data, int echo )
	{
		if (drive_index < MULTIDRIVE_SUBID_NUM_MAX) {
			MultiDriveLite.SubDriver[drive_index].SubID = id & 0xFFFF;
			MultiDriveLite.SubDriver[drive_index].CMD = cmd & 0xFFFF;
			MultiDriveLite.SubDriver[drive_index].Data = data & 0xFFFF;
			MultiDriveLite.SubDriver[drive_index].Echo = echo & 0xFFFF;
			return 1;
		}
		else {
			cout << "Out of range : max drive cnt..." << endl;
			return 0;
		}
	}

	int SerialModbus::SendCmd_MultiDriveLite(int drive_cnt)
	{

		if (Serial_Port->is_open()) {	

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			{
				boost::mutex::scoped_lock lock(*Mutex);

				MultiDriveLite.T_ID = MULTIDRIVE_ID;
				MultiDriveLite.T_FC = MULTIDRIVE_FC_65;
				MultiDriveLite.SubID_Num = drive_cnt;
				MultiDriveLite.Echo_Cnt = 0;
				MultiDriveLite.Echo_Num = 0;

				if (ModbusMaster.Mode == MB_MODE_ASCII) {
					tmMultiDrive_Master_ASCII(&ModbusMaster, &MultiDriveLite);
				}
				else {
					tmMultiDrive_Master_RTU(&ModbusMaster, &MultiDriveLite);
				}

				CMD_Deny_Flag = false;
				TimeOut_Flag = false;
				FrameError_Flag = false;

				R_Data_Index = 0;

				TimeStamp = boost::posix_time::microsec_clock::local_time();

				for (int i = 0; i < MultiDriveLite.SubID_Num; i++) {
					if (MultiDriveLite.SubDriver[i].Echo != 0) {
						MultiDriveLite.Echo_Num++;

						int num = 0;
						for (int j = 0; j < MULTIDRIVELITE_FEEDBACK_DATA_NUM; j++) {
							if ((MultiDriveLite.SubDriver[i].Echo >> j) & 0x01) {
								num++;
							}
						}
						MultiDriveLite.SubDriver[i].FeedBack_Num = num;

					}
				}
				if (MultiDriveLite.Echo_Num != 0) {
					//boost::mutex::scoped_lock func_lock(*Func_Mutex);
					if (MultiDriveLite.Echo_Num == 1 || ModbusMaster.Mode == MB_MODE_ASCII) {
						Current_CmdType = CMD_TYPE_NORMAL_MODBUS;
					}
					else {
						Current_CmdType = CMD_TYPE_MULTI_DRIVE_LITE;
					}

					Received_State = RECEIVED_STATE_BUSY;

				}

				Serial_Port->write_some(buffer(ModbusMaster.T_Data, ModbusMaster.T_DataBytes_Cnt));
			}

			//while (Received_State != RECEIVED_STATE_READY);
			while (1) {
				Sleep(1);
				if (Received_State == RECEIVED_STATE_READY) {
					break;
				}
			}

			if (TimeOut_Flag) {
				return 0;
			}
			else if (FrameError_Flag) {
				return 0;
			}
			return 1;
		}
		else {
			return 0;
		}
	}

	
	
}






