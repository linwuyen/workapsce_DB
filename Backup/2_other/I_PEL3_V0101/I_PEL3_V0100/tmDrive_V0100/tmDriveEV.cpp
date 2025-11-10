/*===========================================================================================

File Name       : tmDriveEV.cpp

Version         : V01_00

Built Date      : 2018/10/02

Release Date    : Not Yet

Programmer      : Chaim.Chen@trumman.com.tw

Description     :

=========================================================================================

History         : 2018/10/02 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#include "stdafx.h"
#include "tmDrive_V0100\tmDriveEV.h"

using namespace std;
using namespace boost::asio;


namespace TM_Drive {


	DriveEV::DriveEV(const std::string port_name, int mode, int baud_rate, int data_bits, int parity, int stop_bits ) : SerialModbus( port_name, mode, baud_rate, data_bits, parity, stop_bits )
	{


	}

	DriveEV::~DriveEV()
	{

	}


	void DriveEV::Main_Thread()
	{
		for (int i = 0; i < Driver_Cnt; i++) {
#if(1)
			Check_Reg(Driver_ID[i], 0x0308, 3000);
			Check_Reg(Driver_ID[i], 0x0400, 10);
			Check_Reg(Driver_ID[i], 0x0408, 10);
			Check_Reg(Driver_ID[i], 0x0700, 2000);

			Check_Reg(Driver_ID[i], 0x0103, 0);

			Check_Reg(Driver_ID[i], 0x0200, 0);

			Check_Reg(Driver_ID[i], 0x0800, 0);
			Check_Reg(Driver_ID[i], 0x0209, 3);

			Check_Reg(Driver_ID[i], 0x020D, 1);		

			Check_Reg(Driver_ID[i], 0x090B, 500);
#else
			Check_Reg(Driver_ID[i], 0x0308, 3000);
			Check_Reg(Driver_ID[i], 0x0400, 1000);
			Check_Reg(Driver_ID[i], 0x0408, 1000);

			Check_Reg(Driver_ID[i], 0x0404, 1000);
			Check_Reg(Driver_ID[i], 0x040C, 1000);

			Check_Reg(Driver_ID[i], 0x0700, 2000);

			Check_Reg(Driver_ID[i], 0x0103, 0);
			Check_Reg(Driver_ID[i], 0x0800, 2);
			Check_Reg(Driver_ID[i], 0x020D, 1);

			Check_Reg(Driver_ID[i], 0x0503, 0);

			Check_Reg(Driver_ID[i], 0x090B, 500);

#endif

			SendCmd_Configure(Driver_ID[i]);
			//SetCmd_MultiDrive(i, Driver_ID[i], MULTI_CMD_SVON, 0, 0);
			SetCmd_MultiDriveLite(i, Driver_ID[i], MDL_CMD_SVON, 0, 0 );
		}

		//SendCmd_MultiDrive(Driver_Cnt);
		SendCmd_MultiDriveLite(Driver_Cnt);
		Sleep(10);

		for (int i = 0; i < Driver_Cnt; i++) {
			int state;
			do {
				Read_Reg(Driver_ID[i], 0x0000, 1, &state);
				if (state == MOTOR_STATE_FAULT) {
					SendCmd_AlarmRest(Driver_ID[i]);
				}

			} while (state != MOTOR_STATE_MOVE);

			//SetCmd_MultiDrive(i, Driver_ID[i], MULTI_CMD_CS, 0, 0);
			//SendCmd_MultiDrive(Driver_ID[i]);

		}

		//

		while (Thread_Flag) {
#if(1)
			if (Update_Flag) {

				for (int i = 0; i < Driver_Cnt; i++) {
					Driver_TargetSpd[i] = Driver_TargetSpd_Mask[i];
				}

				Update_Flag = false;
			}

			for (int i = 0; i < Driver_Cnt; i++) {
				//SetCmd_MultiDrive(i, Driver_ID[i], MULTI_CMD_JG, 0, Driver_TargetSpd[i]);
				//SetCmd_MultiDriveLite(i, Driver_ID[i], MDL_CMD_JG, Driver_TargetSpd[i],( 1 << MDL_ECHO_BITF_SPD ) );

				SetCmd_MultiDriveLite(i, Driver_ID[i], MDL_CMD_JG, Driver_TargetSpd[i], (1 << MDL_ECHO_BITF_SPD ) | ( 1 << MDL_ECHO_BITF_HALL_CNT ) | ( 1 << MDL_ECHO_BITF_ERROR_INDEX)  );
			}

			//SendCmd_MultiDrive(Driver_Cnt);
			SendCmd_MultiDriveLite(Driver_Cnt);

			//int pos_h;
			//int pos_l;

			for (int i = 0; i < Driver_Cnt; i++) {
				/*
				pos_h = (short)MultiDrive.SubDriver[i].FeedBack_Data[0];
				pos_l = (short)MultiDrive.SubDriver[i].FeedBack_Data[1];

				CurrentPos[Driver_ID[i]] = pos_h * TM_ENCODER_RESOLUTION + pos_l;
				*/

				CurrentSpd[Driver_ID[i]] = (short)MultiDriveLite.SubDriver[i].FeedBack_Data[ MDL_ECHO_BITF_SPD ];

				CurrentHallCnt[Driver_ID[i]] = (short)MultiDriveLite.SubDriver[i].FeedBack_Data[MDL_ECHO_BITF_HALL_CNT];

				ErrorIndex[Driver_ID[i]] = (short)MultiDriveLite.SubDriver[i].FeedBack_Data[MDL_ECHO_BITF_ERROR_INDEX];

				if (ErrorIndex[Driver_ID[i]] == 21) { // 21 = Command timeout
					SendCmd_AlarmRest(Driver_ID[i]);
				}

			}


#endif

			//Sleep(1);
		}

		Sleep(100);

		for (int i = 0; i < Driver_Cnt; i++) {
			//SetCmd_MultiDrive(i, Driver_ID[i], MULTI_CMD_JG, 0, 0);
			SetCmd_MultiDriveLite(i, Driver_ID[i], MDL_CMD_JG, 0, (1 << MDL_ECHO_BITF_SPD));
		}

		//if (SendCmd_MultiDrive(Driver_Cnt)) {
		if (SendCmd_MultiDriveLite(Driver_Cnt)) {
			cout << "Stop..." << endl;
		}
		else {
			//cout << "Multi-Drive error..." << endl;
			cout << "Multi-Drive Lite error..." << endl;
		}


	}

	int DriveEV::SetDriverCnt(int cnt)
	{
		if (cnt <= MAX_SUPPORT_DRIVER_CNT) {
			Driver_Cnt = cnt;
			return 1;
		}
		else {
			cout << "Driver cnt is not allowed...value must <= " << MAX_SUPPORT_DRIVER_CNT << endl;
			return 0;
		}
	}

	int DriveEV::SetDriverID(int driver_index, int id) {

		if (driver_index < MAX_SUPPORT_DRIVER_CNT) {
			Driver_ID[driver_index] = id;
			return 1;
		}
		else {
			cout << "Driver index is not allowed...value must < " << MAX_SUPPORT_DRIVER_CNT << endl;
			return 0;
		}
	}

	int DriveEV::Update_DriverSpeed( int driver_0_rpm, int driver_1_rpm, int driver_2_rpm, int driver_3_rpm)
	{
		boost::posix_time::ptime tp1 = boost::posix_time::microsec_clock::local_time();
		while (Update_Flag) {	
			boost::posix_time::ptime tp2 = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_duration timepass = tp2 - tp1;
			if (timepass.total_milliseconds() > TimeOut_Period) {
				break;
			}
		}

		Driver_TargetSpd_Mask[0] = driver_0_rpm;
		Driver_TargetSpd_Mask[1] = driver_1_rpm;
		Driver_TargetSpd_Mask[2] = driver_2_rpm;
		Driver_TargetSpd_Mask[3] = driver_3_rpm;

		Update_Flag = true;

		return 1;
	}

	int DriveEV::Get_CurrentSpeed(int driver_index)
	{
		if (driver_index < MAX_SUPPORT_DRIVER_CNT) {
			return CurrentSpd[ Driver_ID[driver_index] ];
		}
		else {
			return 0;
		}
	}

	int DriveEV::Get_CurrentHallCnt(int driver_index)
	{
		if (driver_index < MAX_SUPPORT_DRIVER_CNT) {
			return CurrentHallCnt[Driver_ID[driver_index]];
		}
		else {
			return 0;
		}
	}

	int DriveEV::Get_ErrorIndex(int driver_index)
	{
		if (driver_index < MAX_SUPPORT_DRIVER_CNT) {
			return ErrorIndex[ Driver_ID[driver_index] ];
		}
		else {
			return 0;
		}
	}

	int DriveEV::Get_CurrentPos(int driver_index)
	{
		if (driver_index < MAX_SUPPORT_DRIVER_CNT) {
			return CurrentPos[ Driver_ID[driver_index] ];
		}
		else {
			return 0;
		}
	}
	
}






