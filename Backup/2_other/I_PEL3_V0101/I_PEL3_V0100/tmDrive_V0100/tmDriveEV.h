/*===========================================================================================

    File Name       : tmDriveEV.h

    Version         : V01_00

    Built Date      : 2018/06/14

    Release Date    : Not Yet

    Programmer      : Chaim.Chen@trumman.com.tw

    Description     : 
				
    =========================================================================================

    History         : 2018/06/14 Perlimary version.  ( by Chaim )
===========================================================================================*/
/*  Trunmman Technology Corporation. All rights reserved. */

#ifndef TM_DRIVE_EV_H
#define TM_DRIVE_EV_H

#include "tmDrive_V0100\tmDrive.h"

#define MAX_SUPPORT_DRIVER_CNT		4

#define TM_ENCODER_RESOLUTION		10000


namespace TM_Drive
{
	class DriveEV : public SerialModbus {
	
	public:

		int									Driver_Cnt{ 1 };

		int									Driver_ID[MAX_SUPPORT_DRIVER_CNT];
		int									Driver_Cmd[MAX_SUPPORT_DRIVER_CNT];
		int									Driver_TargetSpd[MAX_SUPPORT_DRIVER_CNT];
		int									Driver_TargetSpd_Mask[MAX_SUPPORT_DRIVER_CNT];

		int									CurrentSpd[MULTIDRIVE_ID_MAX];
		int									ErrorIndex[MULTIDRIVE_ID_MAX];
		int									CurrentPos[MULTIDRIVE_ID_MAX];

		bool								Update_Flag{ false };


	public:
		DriveEV( const std::string port_name, int mode, int baud_rate = TM_DEF_BAUD_RATE, int data_bits = TM_DEF_DATA_BITS, int parity = TM_DEF_PARITY, int stop_bits = TM_DEF_STOP_BITS);
		~DriveEV();

	public:
		int SetDriverCnt(int cnt);

		int SetDriverID(int driver_index, int id);

		int Update_DriverSpeed( int driver_0_rpm, int driver_1_rpm = 0, int driver_2_rpm = 0, int driver_3_rpm = 0 );

		int Get_CurrentSpeed(int driver_index);
		int Get_ErrorIndex(int driver_index);
		int Get_CurrentPos(int driver_index);

		


	private:
		virtual void Main_Thread();


	};

}

#endif

/************************** <END OF FILE> *****************************************/



