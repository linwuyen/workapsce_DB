/*===========================================================================================

    File Name       : tmDrive.h

    Version         : V01_00

    Built Date      : 2018/06/14

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/06/14 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_DRIVE_H
#define TM_DRIVE_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include "tmModbusMaster.h"
#include "tmMultiDrive.h"

#define TM_DRIVE_PA_MAJOR_MAX	9

#define RECEIVED_STATE_READY		0
#define RECEIVED_STATE_BUSY			1
#define RECEIVED_STATE_DONE			2

#define TM_DEF_BAUD_RATE			115200
#define TM_DEF_DATA_BITS			8
#define TM_DEF_PARITY				0			// 0 = None, 1 = Odd, 2 = Even
#define TM_DEF_STOP_BITS			1

#define DRIVE_ADDRESS_CMD_ALM_RESET		0x0A00
#define DRIVE_ADDRESS_CMD_CONFIGURE		0x0A27

#define TM_T_DATA_SIZE				666
#define TM_R_DATA_SIZE				666

#define CMD_TYPE_NORMAL_MODBUS		0
#define CMD_TYPE_MULTI_DRIVE		1
#define CMD_TYPE_MULTI_DRIVE_LITE	2

#define MOTOR_STATE_STOP			0	            // Motor is stopped.
#define MOTOR_STATE_STARTING		1	            // Motor is in Starting procedure.
#define MOTOR_STATE_RUNNING			2	            // Motor is running.
#define MOTOR_STATE_EBRAKING		3	            // Motor is enmergency braking.
#define MOTOR_STATE_FREEING			4	            // Motor is freeing.
#define MOTOR_STATE_FAULT			5	            // 
#define MOTOR_STATE_WAIT			6	            // Motor is stopped and in parameter setting mode.
#define MOTOR_STATE_MOVE			7	            // Motor is moving
#define MOTOR_STATE_LOCK			8


using namespace boost::asio;
using namespace std;

namespace TM_Drive
{
	class SerialModbus
	{
		// Multi-Drive
	public:

		Struct_MultiDrive					MultiDrive;
		Struct_MultiDrive					MultiDriveLite;

	protected:

		// Serial settings
		string		Port_Name;								
		int			BaudRate;				// 115200
		int			DataBits;				// 8
		int			Parity;					// 0 = None, 1 = Odd, 2 = Even
		int			StopBits;				// 1

		// Main variables

		bool								Thread_Flag{ false };

		int									TimeOut_Flag{ false };
		int									FrameError_Flag{ false };
		int									CMD_Deny_Flag{ false };
		int									TimeOut_Period{ 100 };			// 100ms timeout	

		// Shared ptr

		//std::shared_ptr<int>				Received_State;


		std::shared_ptr<io_service>			IO_Service;
		std::shared_ptr<serial_port>		Serial_Port;
		std::shared_ptr<boost::mutex>		Mutex;
		//std::shared_ptr<boost::mutex>		Func_Mutex;
		std::shared_ptr<boost::thread>		Received_Thread_Pointer;
		std::shared_ptr<boost::thread>		DataProcess_Thread_Pointer;
		std::shared_ptr<boost::thread>		Main_Thread_Pointer;

		// Peripheral variables

		Struct_ModbusMaster					ModbusMaster;
		int									Received_State;

		unsigned char						T_Data[TM_T_DATA_SIZE];
		unsigned char						R_Data[TM_R_DATA_SIZE];

		int									R_Data_Cnt{ 0 };						// Received Data Cnt
		int									R_Data_Index{ 0 };

		int									Current_CmdType;
		unsigned int						RTU_GetData_Flag{ 0 };					// To indicate if receive data
		unsigned int						RTU_FrameLength{ 3 };					// unit = 1ms
		unsigned int						MultiDive_FrameLength{ 7 };				// unit = 1ms

		boost::posix_time::ptime			RTU_TP1;
		boost::posix_time::ptime			RTU_TP2;	
		boost::posix_time::ptime			TimeNow;
		boost::posix_time::ptime			Dummy_Time;
		boost::posix_time::ptime			TimeStamp;

		/*
		boost::posix_time::ptime			TP_FirstByte;
		boost::posix_time::ptime			TP_LastByte;

		boost::posix_time::ptime			TP_Done;
		boost::posix_time::ptime			TP_Ready;
		*/

	public:

		void Run();
		void Stop();

	private:
		void Received_Thread();
		void DataProcess_Thread();
		virtual void Main_Thread();

	public:
		SerialModbus( const std::string port_name );
		SerialModbus( const std::string port_name, int mode, int baud_rate = TM_DEF_BAUD_RATE, int data_bits = TM_DEF_DATA_BITS, int parity = TM_DEF_PARITY, int stop_bits = TM_DEF_STOP_BITS);

		virtual ~SerialModbus();

	public:
		void Set_Pa(const std::string port_name, int mode, int baud_rate, int data_bits, int parity, int stop_bits );
		void Set_TimeOutPerid( int period );
		int Open();
		int Close();

		void CheckRTUframe();
		void CheckTimeOut();

		void MultiDriveDataProcess();
		void MultiDriveLiteDataProcess();
		void DataProcess();

		int Write_Reg_Single( int id, int address, int vlaue, bool need_response = true );
		int Write_Reg_Multiple(int id, int address, int data_cnt, int* data, bool need_response = true);
		int Read_Reg( int id, int address, int data_cnt, int *r_data );
		int Check_Reg(int id, int address, int data);

		int SendCmd_AlarmRest( int id, bool need_response = true );
		int SendCmd_Configure( int id, bool need_response = true );

		// Multi-Drive
		int SetCmd_MultiDrive( int drive_index, int id, int cmd, int data_1, int data_2 );
		int SendCmd_MultiDrive( int drive_cnt );

		// Multi-Drive Lite
		int SetCmd_MultiDriveLite(int drive_index, int id, int cmd, int data, int echo);
		int SendCmd_MultiDriveLite(int drive_cnt);


	};
}



#endif

/************************** <END OF FILE> *****************************************/



