//###########################################################################
//
// FILE:    MS_Control_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20221108
//###########################################################################

#include "Global_VariableDefs.h"


MS_CONTROL_MODULE_VARIABLES_REG MS_Control_Module_Variables;






static inline MS_CONNECT_STATE_INIT0_Process(void);
static inline MS_CONNECT_STATE_INIT1_Process(void);

static inline MS_CONNECT_STATE_MASTER0_Process(void);
static inline MS_CONNECT_STATE_MASTER1_Process(void);

static inline MS_CONNECT_STATE_SLAVE0_Process(void);
static inline MS_CONNECT_STATE_SLAVE1_Process(void);
static inline MS_CONNECT_STATE_SLAVE2_Process(void);
static inline MS_CONNECT_STATE_SLAVE3_Process(void);
static inline MS_CONNECT_STATE_SLAVE4_Process(void);

static inline MS_SDRAM_SYNC_STATE_MASTER0_Process(void);
static inline MS_SDRAM_SYNC_STATE_MASTER1_Process(void);
static inline MS_SDRAM_SYNC_STATE_MASTER_Pending0_Process(void);
static inline MS_SDRAM_SYNC_STATE_MASTER_Pending1_Process(void);
static inline MS_SDRAM_SYNC_STATE_MASTER2_Process(void);
static inline MS_SDRAM_SYNC_STATE_MASTER3_Process(void);

static inline MS_SDRAM_SYNC_STATE_SLAVE_init_Process(void);
static inline MS_SDRAM_SYNC_STATE_SLAVE0_Process(void);
static inline MS_SDRAM_SYNC_STATE_SLAVE_Pending0_Process(void);
static inline MS_SDRAM_SYNC_STATE_SLAVE1_Process(void);
static inline MS_SDRAM_SYNC_STATE_SLAVE2_Process(void);
static inline MS_SDRAM_SYNC_STATE_SLAVE3_Process(void);
static inline MS_SDRAM_SYNC_STATE_SLAVE4_Process(void);

static inline MS_CONNECT_OK_STATE_Master_Process(void);
static inline MS_CONNECT_OK_STATE_Slave_Process(void);
static inline Slave_CPU0_TimeOut_Scan_Process(void);
static inline Master_CPU0_TimeOut_Scan_Process(void);

static inline MS_CONNECT_ERROR_HANDLE_STATE_Process(void);



static inline MASTER_CMD_SCHEDULE_00_TX_Process(void);
static inline MASTER_CMD_SCHEDULE_00_RX_Process(void);


static inline MASTER_CMD_SUB_SCHEDULE_01_TX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_01_RX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_02_TX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_02_RX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_03_TX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_03_RX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_04_TX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_04_RX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_05_TX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_05_RX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_06_TX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_06_RX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_07_TX_Process(void);
static inline MASTER_CMD_SUB_SCHEDULE_07_RX_Process(void);

static inline SLAVE_RS485_TX_Process(void);
static inline SLAVE_RS485_RX_Process(void);



/////////////////////////////////////////////////////////////////////
void Init_MS_Control_Module_Variables(void)
{
    Uint16 i;

    MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;


    MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

    MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_01_TX;

    MS_Control_Module_Variables.Slave_RS485_State = SLAVE_RS485_RX;


    MS_Control_Module_Variables.Master_reCheck_Counter = 0;

    MS_Control_Module_Variables.Master_SCI_Read_CMD.all = 0;
    MS_Control_Module_Variables.Master_SCI_Write_CMD.all = 0;

    for ( i = 0; i < 14; i ++ )
    {
        MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = 0;
        MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = 0;
        MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = 0;
        MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = 0;
        MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = 0;
    }

    MS_Control_Module_Variables.Sync_Wave_Addr_Counter = 0;
    MS_Control_Module_Variables.Sync_Wave_Page_Counter = 0;
    MS_Control_Module_Variables.Sync_Wave_data_CheckSum = 0;
    MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow = 0;

    MS_Control_Module_Variables.CpuTimer2_Counter = 0;
    MS_Control_Module_Variables.CpuTimer2_Count = 0;

}




/////////////////////////////////////////////////////////////////////
void MS_Connect_Process(void)
{
    switch( MS_Control_Module_Variables.MS_Connect_State )
    {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_INIT0:
        {
            MS_CONNECT_STATE_INIT0_Process();

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT1;

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_INIT1:
        {
            MS_CONNECT_STATE_INIT1_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_MASTER0:
        {
            MS_CONNECT_STATE_MASTER0_Process();

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 0 )
            {
                //
                // 關閉MS功能
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_MASTER1:
        {
            MS_CONNECT_STATE_MASTER1_Process();

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 0 )
            {
                //
                // 關閉MS功能
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_SLAVE0:
        {
            MS_CONNECT_STATE_SLAVE0_Process();

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 0 )
            {
                //
                // 關閉MS功能
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_SLAVE1:
        {
            MS_CONNECT_STATE_SLAVE1_Process();

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 0 )
            {
                //
                // 關閉MS功能
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_SLAVE2:
        {
            MS_CONNECT_STATE_SLAVE2_Process();


            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_OK_STATE;

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 0 )
            {
                //
                // 關閉MS功能
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_SLAVE3:
        {
            MS_CONNECT_STATE_SLAVE3_Process();

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 0 )
            {
                //
                // 關閉MS功能
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_STATE_SLAVE4:
        {
            MS_CONNECT_STATE_SLAVE4_Process();

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 0 )
            {
                //
                // 關閉MS功能
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_MASTER0:
        {
            MS_SDRAM_SYNC_STATE_MASTER0_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_MASTER1:
        {
            MS_SDRAM_SYNC_STATE_MASTER1_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_MASTER_Pending0:
        {
            MS_SDRAM_SYNC_STATE_MASTER_Pending0_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_MASTER_Pending1:
        {
            MS_SDRAM_SYNC_STATE_MASTER_Pending1_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_MASTER2:
        {
            MS_SDRAM_SYNC_STATE_MASTER2_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_MASTER3:
        {
            MS_SDRAM_SYNC_STATE_MASTER3_Process();

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_OK_STATE;

            break;
        }


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_SLAVE_init:
        {
            MS_SDRAM_SYNC_STATE_SLAVE_init_Process();

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE0;

            break;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_SLAVE0:
        {
            MS_SDRAM_SYNC_STATE_SLAVE0_Process();

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE_Pending0;

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_SLAVE_Pending0:
        {
            MS_SDRAM_SYNC_STATE_SLAVE_Pending0_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_SLAVE1:
        {
            MS_SDRAM_SYNC_STATE_SLAVE1_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_SLAVE2:
        {
            MS_SDRAM_SYNC_STATE_SLAVE2_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_SLAVE3:
        {
            MS_SDRAM_SYNC_STATE_SLAVE3_Process();

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_SDRAM_SYNC_STATE_SLAVE4:
        {
            MS_SDRAM_SYNC_STATE_SLAVE4_Process();

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_OK_STATE;

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_OK_STATE:
        {
            //
            // MS_Ready 階段
            //
            if ( Global_Variables.POWER_STATUS.bit.MS_Master == 1 )
            {
                Master_CPU0_TimeOut_Scan_Process();
                MS_CONNECT_OK_STATE_Master_Process();
            }
            else if ( Global_Variables.POWER_STATUS.bit.MS_Master == 0 )
            {
                Slave_CPU0_TimeOut_Scan_Process();
                MS_CONNECT_OK_STATE_Slave_Process();
            }

            if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 0 )
            {
                //
                // 關閉MS功能
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_INIT0;
            }

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_ERROR_HANDLE_STATE:
        {
            MS_CONNECT_ERROR_HANDLE_STATE_Process();

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_STATE;

            break;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MS_CONNECT_ERROR_STATE:
        {
            //
            // Reset overtime
            //

            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            //
            // 等待重置
            //

            break;
        }
    }
}


/*--------------------------------------------------------------------------------------------------------------------------*/
//
// Master/Slave SCI CMD Read
//


////////////////////////////////////////////////////////////////////
//
//
//
void SCI_CMD_00_Send(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;
    Uint16 addr;

    addr = Global_Variables.MS_Function_Variables.MS_Address;

    // Code
    i = 0;
    SCI_Module_Variables.SCIC_Tx_Data[i] = MS_RS485_CODE_00;

    // Address
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;

    // Machine Status
    Uint16_tmp = Global_Variables.POWER_STATUS.all;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_Power_Status[addr].all = Global_Variables.POWER_STATUS.all;

    // C2000 Alarm Status
    Uint16_tmp = Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_POWER_FUNCTIONS_ERROR_FLAGS[addr].all = Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.all;


    // C2000 Alarm Status 2
    Uint16_tmp = Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_POWER_FUNCTIONS_ERROR_FLAGS2[addr].all = Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.all;


    // P_display.MSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // P_display.LSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].P_display_shadow.all = Global_Variables.POWER_DISPLAY_VARIABLES.P_display_shadow.all;



    // MS_Sync_Status_Flags
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[ Global_Variables.MS_Function_Variables.MS_Address ].all;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

}

////////////////////////////////////////////////////////////////////
//
//
//
void SCI_CMD_00_Receive(void)
{
    Uint16 Uint16_tmp;
    Uint16 i, addr;


    // Address
    i = 1;
    addr = SCI_Module_Variables.SCIC_Rx_Data[i];

    // Machine Status
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_Power_Status[addr].all = Uint16_tmp;

    // C2000 Alarm Status
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_FUNCTIONS_ERROR_FLAGS[addr].all = Uint16_tmp;

    // C2000 Alarm Status 2
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_FUNCTIONS_ERROR_FLAGS2[addr].all = Uint16_tmp;


    // P_display.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].P_display_shadow.MSB = Uint16_tmp;

    // P_display.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].P_display_shadow.LSB = Uint16_tmp;

    // MS_Sync_Status_Flags
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_Sync_Status_Flags[addr].all = Uint16_tmp;



}





////////////////////////////////////////////////////////////////////
//
//
//
void SCI_CMD_01_Send(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;
    Uint16 addr;

    addr = Global_Variables.MS_Function_Variables.MS_Address;

    // Code
    i = 0;
    SCI_Module_Variables.SCIC_Tx_Data[i] = MS_RS485_CODE_01;

    // Address
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;

    // V_avg_display.MSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // V_avg_display.LSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_avg_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_avg_display.all;


    // V_rms_display.MSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // V_rms_display.LSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_rms_display.all;


    // V_peak_hold_display.MSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.V_peak_hold_display.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // V_peak_hold_display.LSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.V_peak_hold_display.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_peak_hold_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.V_peak_hold_display.all;

}

////////////////////////////////////////////////////////////////////
//
//
//
void SCI_CMD_01_Receive(void)
{
    Uint16 Uint16_tmp;
    Uint16 i, addr;


    // Address
    i = 1;
    addr = SCI_Module_Variables.SCIC_Rx_Data[i];



    // V_avg_display.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_avg_display.MSB = Uint16_tmp;

    // V_avg_display.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_avg_display.LSB = Uint16_tmp;


    // V_rms_display.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_rms_display.MSB = Uint16_tmp;

    // V_rms_display.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_rms_display.LSB = Uint16_tmp;


    // V_peak_hold_display.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_peak_hold_display.MSB = Uint16_tmp;

    // V_peak_hold_display.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].V_peak_hold_display.LSB = Uint16_tmp;



}





////////////////////////////////////////////////////////////////////
//
//
//
void SCI_CMD_02_Send(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;
    Uint16 addr;

    addr = Global_Variables.MS_Function_Variables.MS_Address;

    // Code
    i = 0;
    SCI_Module_Variables.SCIC_Tx_Data[i] = MS_RS485_CODE_02;

    // Address
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;

    // I_avg_display.MSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // I_avg_display.LSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_avg_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_avg_display.all;


    // I_rms_display.MSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // I_rms_display.LSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_rms_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_rms_display.all;


    // I_peak_hold_display.MSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // I_peak_hold_display.LSB
    Uint16_tmp = Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_peak_hold_display.all = Global_Variables.POWER_DISPLAY_VARIABLES.I_peak_hold_display.all;

}

////////////////////////////////////////////////////////////////////
//
//
//
void SCI_CMD_02_Receive(void)
{
    Uint16 Uint16_tmp;
    Uint16 i, addr;


    // Address
    i = 1;
    addr = SCI_Module_Variables.SCIC_Rx_Data[i];



    // I_avg_display.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_avg_display.MSB = Uint16_tmp;

    // I_avg_display.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_avg_display.LSB = Uint16_tmp;


    // I_rms_display.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_rms_display.MSB = Uint16_tmp;

    // I_rms_display.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_rms_display.LSB = Uint16_tmp;


    // I_peak_hold_display.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_peak_hold_display.MSB = Uint16_tmp;

    // I_peak_hold_display.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_POWER_DISPLAY_VARIABLES[addr].I_peak_hold_display.LSB = Uint16_tmp;


}






/*--------------------------------------------------------------------------------------------------------------------------*/
//
// Master/Slave SCI CMD Write
//



////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_80_Send(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    // Code
    i = 0;
    SCI_Module_Variables.SCIC_Tx_Data[i] = MS_RS485_CODE_80;

    // Address
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;

    //
    Uint16_tmp = 0;
    Uint16_tmp = (Uint16)Global_Variables.POWER_CONTROL.bit.OUTPUT_ON;
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable << 1 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable << 2 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable << 3 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable << 4 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit << 5 );
    if ( MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Reset_Trig == 1 )
    {
        //
        // Master觸發Reset
        //
        MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Reset_Trig = 0;
        Uint16_tmp = Uint16_tmp + ( 0x01 << 6 );
    }
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;




    // Start_Phase_Enable, Stop_Phase_Enable
    Uint16_tmp = 0;
    Uint16_tmp = (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable;
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable << 1 );
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // Waveform_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // OUTPUT_MODE_set_user
    Uint16_tmp = Global_Variables.OUTPUT_MODE_set_user;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // V_SlewRate_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // Start_Phase_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // Stop_Phase_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

}



////////////////////////////////////////////////////////////////////
//
//
//
void Slave_SCI_CMD_80_Receive(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;


    //
    i = 2;
    //Global_Variables.POWER_CONTROL.bit.OUTPUT_ON = SCI_Module_Variables.SCIC_Rx_Data[i] & 0x01;
    Global_Variables.POWER_CONTROL.bit.OUTPUT_ON_user = SCI_Module_Variables.SCIC_Rx_Data[i] & 0x01;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable = ( SCI_Module_Variables.SCIC_Rx_Data[i] >> 1 ) & 0x01;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable = ( SCI_Module_Variables.SCIC_Rx_Data[i] >> 2 ) & 0x01;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable = ( SCI_Module_Variables.SCIC_Rx_Data[i] >> 3 ) & 0x01;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable = ( SCI_Module_Variables.SCIC_Rx_Data[i] >> 4 ) & 0x01;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit = ( SCI_Module_Variables.SCIC_Rx_Data[i] >> 5 ) & 0x01;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset = ( SCI_Module_Variables.SCIC_Rx_Data[i] >> 6 ) & 0x01;


    //
    // 觸發事件
    //
    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;
    Event_Module.Event3_Flag.bit.Set_Output_ON_OFF = 1;

    // Start_Phase_Enable, Stop_Phase_Enable
    i ++;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable = SCI_Module_Variables.SCIC_Rx_Data[i] & 0x01;
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable = ( SCI_Module_Variables.SCIC_Rx_Data[i] >> 1 ) & 0x01;


    // Waveform_set_user
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user = Uint16_tmp;


    // OUTPUT_MODE_set_user
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.OUTPUT_MODE_set_user = Uint16_tmp;

    //
    // 使用者設定的輸出模式更新進來
    //
    Global_Variables.OUTPUT_MODE_FLAG.all = ( Global_Variables.OUTPUT_MODE_FLAG.all & 0x0180 ) + ( Global_Variables.OUTPUT_MODE_set_user & 0x007F );


    // V_SlewRate_set_user
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user = Uint16_tmp;

    // Start_Phase_set_user
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user = Uint16_tmp;



    // Stop_Phase_set_user
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user = Uint16_tmp;

}





////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_81_Send(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    // Code
    i = 0;
    SCI_Module_Variables.SCIC_Tx_Data[i] = MS_RS485_CODE_81;

    // Address
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;


    // FREQ_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // FREQ_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // DCV_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // DCV_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // ACV_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // ACV_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

}



////////////////////////////////////////////////////////////////////
//
//
//
void Slave_SCI_CMD_81_Receive(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;




    // FREQ_set_user.MSB
    i  = 2;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.MSB = Uint16_tmp;


    // FREQ_set_user.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.LSB = Uint16_tmp;


    // DCV_set_user.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.MSB = Uint16_tmp;


    // DCV_set_user.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.LSB = Uint16_tmp;

    // ACV_set_user.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.MSB = Uint16_tmp;


    // ACV_set_user.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.LSB = Uint16_tmp;

}






////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_82_Send(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    // Code
    i = 0;
    SCI_Module_Variables.SCIC_Tx_Data[i] = MS_RS485_CODE_82;

    // Address
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;


    // I_rms_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // I_rms_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // p_I_peak_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // p_I_peak_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // n_I_peak_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // n_I_peak_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


}



////////////////////////////////////////////////////////////////////
//
//
//
void Slave_SCI_CMD_82_Receive(void)
{

    Uint16 Uint16_tmp;
    Uint16 i;



    // I_rms_limit_level.MSB
    i = 2;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.MSB = Uint16_tmp;


    // I_rms_limit_level.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.LSB = Uint16_tmp;



    // p_I_peak_limit_level.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.MSB = Uint16_tmp;


    // p_I_peak_limit_level.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.LSB = Uint16_tmp;

    // n_I_peak_limit_level.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.MSB = Uint16_tmp;


    // n_I_peak_limit_level.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.LSB = Uint16_tmp;

    //
    // 觸發事件
    //
    Event_Module.Event_Flag.bit.Set_DAC7612_CHA = 1; //Trig event
    Event_Module.Event_Flag.bit.Set_DAC7612_CHB = 1; //Trig event

}








////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_83_Send(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    // Code
    i = 0;
    SCI_Module_Variables.SCIC_Tx_Data[i] = MS_RS485_CODE_83;

    // Address
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;


    // Ext_GAIN_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // Ext_GAIN_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // VCA_GAIN_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.MSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    // VCA_GAIN_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.LSB;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;


    // SYNC_Phase_Delay_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    //
    // TBD
    //
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = 0;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = 0;


}



////////////////////////////////////////////////////////////////////
//
//
//
void Slave_SCI_CMD_83_Receive(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;



    // Ext_GAIN_set_user.MSB
    i = 2;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.MSB = Uint16_tmp;


    // Ext_GAIN_set_user.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.LSB = Uint16_tmp;



    // VCA_GAIN_set_user.MSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.MSB = Uint16_tmp;


    // VCA_GAIN_set_user.LSB
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.LSB = Uint16_tmp;


    // SYNC_Phase_Delay_set_user
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user = Uint16_tmp;

}




////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_84_Send(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    // Code
    i = 0;
    SCI_Module_Variables.SCIC_Tx_Data[i] = MS_RS485_CODE_84;

    // Address
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;


    //
    // 3P4W, L2 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[1];
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    //
    // 3P4W, L3 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[2];
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    //
    // 1P3W, L2 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_1P3W_Phase_set_user[1];
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = Uint16_tmp & 0xFF;

    //
    // TBD
    //
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = 0;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = 0;

    //
    // TBD
    //
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = 0;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = 0;

    //
    // TBD
    //
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = 0;
    i ++;
    SCI_Module_Variables.SCIC_Tx_Data[i] = 0;


}





////////////////////////////////////////////////////////////////////
//
//
//
void Slave_SCI_CMD_84_Receive(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;


    //
    // 3P4W, L2 Phase set
    //
    i = 2;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[1] = Uint16_tmp;


    //
    // 3P4W, L3 Phase set
    //
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[2] = Uint16_tmp;

    //
    // 1P3W, L2 Phase set
    //
    i ++;
    Uint16_tmp = SCI_Module_Variables.SCIC_Rx_Data[i] << 8;
    i ++;
    Uint16_tmp = Uint16_tmp + SCI_Module_Variables.SCIC_Rx_Data[i];
    Global_Variables.MS_Function_Variables.MS_1P3W_Phase_set_user[1] = Uint16_tmp;

}





/////////////////////////////////////////////////////////////////////
void Master_SCI_CMD_80_Table_Update(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;



    //
    Uint16_tmp = 0;
    Uint16_tmp = (Uint16)Global_Variables.POWER_CONTROL.bit.OUTPUT_ON;
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable << 1 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable << 2 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable << 3 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable << 4 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit << 5 );
    //Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset << 6 );
    i = 2;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = Uint16_tmp & 0xFF;

    // Start_Phase_Enable, Stop_Phase_Enable
    Uint16_tmp = 0;
    Uint16_tmp = (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable;
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable << 1 );
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = Uint16_tmp & 0xFF;


    // Waveform_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = Uint16_tmp & 0xFF;


    // OUTPUT_MODE_set_user
    Uint16_tmp = Global_Variables.OUTPUT_MODE_set_user;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = Uint16_tmp & 0xFF;


    // V_SlewRate_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = Uint16_tmp & 0xFF;


    // Start_Phase_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = Uint16_tmp & 0xFF;

    // Stop_Phase_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] = Uint16_tmp & 0xFF;

}



////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_81_Table_Update(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;



    // FREQ_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.MSB;
    i = 2;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = Uint16_tmp & 0xFF;

    // FREQ_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.LSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = Uint16_tmp & 0xFF;


    // DCV_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.MSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = Uint16_tmp & 0xFF;


    // DCV_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.LSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = Uint16_tmp & 0xFF;


    // ACV_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.MSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = Uint16_tmp & 0xFF;


    // ACV_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.LSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] = Uint16_tmp & 0xFF;

}





////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_82_Table_Update(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;



    // I_rms_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.MSB;
    i = 2;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = Uint16_tmp & 0xFF;

    // I_rms_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.LSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = Uint16_tmp & 0xFF;


    // p_I_peak_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.MSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = Uint16_tmp & 0xFF;

    // p_I_peak_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.LSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = Uint16_tmp & 0xFF;


    // n_I_peak_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.MSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = Uint16_tmp & 0xFF;

    // n_I_peak_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.LSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] = Uint16_tmp & 0xFF;


}





////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_83_Table_Update(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;


    // Ext_GAIN_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.MSB;
    i = 2;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = Uint16_tmp & 0xFF;

    // Ext_GAIN_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.LSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = Uint16_tmp & 0xFF;


    // VCA_GAIN_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.MSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = Uint16_tmp & 0xFF;

    // VCA_GAIN_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.LSB;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = Uint16_tmp & 0xFF;


    // SYNC_Phase_Delay_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = Uint16_tmp & 0xFF;

    //
    // TBD
    //
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = 0;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] = 0;


}




////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_84_Table_Update(void)
{

    Uint16 Uint16_tmp;
    Uint16 i;

    //
    // 3P4W, L2 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[1];
    i = 2;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = Uint16_tmp & 0xFF;

    //
    // 3P4W, L3 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[2];
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = Uint16_tmp & 0xFF;

    //
    // 1P3W, L2 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_1P3W_Phase_set_user[1];
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = Uint16_tmp & 0xFF;

    //
    // TBD
    //
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = 0;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = 0;

    //
    // TBD
    //
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = 0;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = 0;

    //
    // TBD
    //
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = 0;
    i ++;
    MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] = 0;
}



////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_80_Change_Check(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    Uint16 table[14];

    //
    Uint16_tmp = 0;
    Uint16_tmp = (Uint16)Global_Variables.POWER_CONTROL.bit.OUTPUT_ON;
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_rms_limit_Enable << 1 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.I_peak_limit_Enable << 2 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.CV_Compensation_Enable << 3 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Freq_Compensation_Enable << 4 );
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.ACV_Unit << 5 );
    //Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Protection_Reset << 6 );
    i = 2;
    table[i] = Uint16_tmp & 0xFF;

    // Start_Phase_Enable, Stop_Phase_Enable
    Uint16_tmp = 0;
    Uint16_tmp = (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Start_Phase_Enable;
    Uint16_tmp = Uint16_tmp + ( (Uint16)Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS.bit.Stop_Phase_Enable << 1 );
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // Waveform_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // OUTPUT_MODE_set_user
    Uint16_tmp = Global_Variables.OUTPUT_MODE_set_user;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // V_SlewRate_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.V_SlewRate_set_user;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // Start_Phase_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Start_Phase_set_user;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    // Stop_Phase_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Stop_Phase_set_user;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    for ( i = 2; i < 14; i ++ )
    {
        if ( MS_Control_Module_Variables.Master_SCI_CMD_80_Table[i] != table[i] )
        {
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;

            // 設定Busy狀態
            //
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

        }
    }
}





////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_81_Change_Check(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    Uint16 table[14];


    // FREQ_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.MSB;
    i = 2;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    // FREQ_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.FREQ_set_user.LSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // DCV_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.MSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // DCV_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.DCV_set_user.LSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // ACV_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.MSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // ACV_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.ACV_set_user.LSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    for ( i = 2; i < 14; i ++ )
    {
        if ( MS_Control_Module_Variables.Master_SCI_CMD_81_Table[i] != table[i] )
        {
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_81_Tx_Trig = 1;

            // 設定Busy狀態
            //
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

        }
    }

}





////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_82_Change_Check(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    Uint16 table[14];


    // I_rms_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.MSB;
    i = 2;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    // I_rms_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.I_rms_limit_level.LSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // p_I_peak_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.MSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    // p_I_peak_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.p_I_peak_limit_level.LSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // n_I_peak_limit_level.MSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.MSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    // n_I_peak_limit_level.LSB
    Uint16_tmp = Global_Variables.POWER_PROTECT_SETTING_VARIABLES.n_I_peak_limit_level.LSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    for ( i = 2; i < 14; i ++ )
    {
        if ( MS_Control_Module_Variables.Master_SCI_CMD_82_Table[i] != table[i] )
        {
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_82_Tx_Trig = 1;

            // 設定Busy狀態
            //
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

        }
    }

}





////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_83_Change_Check(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    Uint16 table[14];

    // Ext_GAIN_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.MSB;
    i = 2;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    // Ext_GAIN_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Ext_GAIN_set_user.LSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // VCA_GAIN_set_user.MSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.MSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    // VCA_GAIN_set_user.LSB
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.VCA_GAIN_set_user.LSB;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    // SYNC_Phase_Delay_set_user
    Uint16_tmp = Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SYNC_Phase_Delay_set_user;
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    //
    // TBD
    //
    i ++;
    table[i] = 0;
    i ++;
    table[i] = 0;

    for ( i = 2; i < 14; i ++ )
    {
        if ( MS_Control_Module_Variables.Master_SCI_CMD_83_Table[i] != table[i] )
        {
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_83_Tx_Trig = 1;

            // 設定Busy狀態
            //
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

        }
    }


}





////////////////////////////////////////////////////////////////////
//
//
//
void Master_SCI_CMD_84_Change_Check(void)
{
    Uint16 Uint16_tmp;
    Uint16 i;

    Uint16 table[14];

    //
    // 3P4W, L2 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[1];
    i = 2;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    //
    // 3P4W, L3 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_3P4W_Phase_set_user[2];
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;

    //
    // 1P3W, L2 Phase set
    //
    Uint16_tmp = Global_Variables.MS_Function_Variables.MS_1P3W_Phase_set_user[1];
    i ++;
    table[i] = ( Uint16_tmp >> 8 ) & 0xFF;
    i ++;
    table[i] = Uint16_tmp & 0xFF;


    //
    // TBD
    //
    i ++;
    table[i] = 0;
    i ++;
    table[i] = 0;

    //
    // TBD
    //
    i ++;
    table[i] = 0;
    i ++;
    table[i] = 0;

    //
    // TBD
    //
    i ++;
    table[i] = 0;
    i ++;
    table[i] = 0;

    for ( i = 2; i < 14; i ++ )
    {
        if ( MS_Control_Module_Variables.Master_SCI_CMD_84_Table[i] != table[i] )
        {
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_84_Tx_Trig = 1;

            // 設定Busy狀態
            //
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

        }
    }
}





/////////////////////////////////////////////////////////////////////
void MS_Output_Status_Check_Start_Time_reset(void)
{
    MS_Control_Module_Variables.MS_Output_Status_Check_Start_Time = CpuTimer2.InterruptCount;
}




/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_INIT0_Process(void)
{
    //
    // INIT-0
    //

    Global_Variables.MS_Function_Variables.MS_Address = 0xFF;
    Global_Variables.MS_Function_Variables.MS_Total_Units = 0;
    Global_Variables.POWER_STATUS.bit.MS_Master = 0;

    Init_MS_Control_Module_Variables();

    //
    // 設定 PAR_ENA 輸出Low
    //
    PAR_ENA_0;

    //
    // 預設RS485為接收
    //
    TXC_RXC_0;

    //
    // 初始化SCI
    //
    Init_SCI_C_GPIO();
    Init_SCI_C_Reg();

    //
    // 預設Disable SPI接收
    //
    n_SLAVE_ENA_1;


    //
    // 預設SPI Slave
    //
    Init_SPI_B_GPIO_for_Slave();
    Init_SPI_B_Reg_for_Slave();

    //
    // Reset工作模式 OUTPUT_MODE
    //
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_Master = 0;
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_Slave = 0;

    //
    // 啟用優化
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 1;

}


/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_INIT1_Process(void)
{
    //
    // INIT-1
    //

    if ( ( Main_State_Module.Machine_state == READY_STATE )
         && ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Enable == 1 ) )
    {
        //
        // 機器開機READY且MS_Enable才進行連線處理
        //

        if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Master == 1 )
        {
            //
            // 設定本機為Master
            //

            Global_Variables.POWER_STATUS.bit.MS_Master = 1;

            Global_Variables.MS_Function_Variables.MS_Address = 0;

            //
            // Enable下一台
            //
            PAR_ENA_1;

            //
            // Reset接收旗標, 準備接收資料
            //
            //SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;
            // 20230310 有可能PAR_ENA_1設定後, Slave馬上傳RS485資料, 如果這裡會導致沒收到

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_MASTER0;
        }
        else if ( Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.MS_Master == 0 )
        {
            //
            // 本機設定為Slave
            //

            Global_Variables.POWER_STATUS.bit.MS_Master = 0;


            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_SLAVE0;
        }
    }

}


/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_MASTER0_Process(void)
{
    //
    // Master-0
    //

    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // RS485收到資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;

        if ( SCI_Module_Variables.SCIC_Rx_Data[ 0 ] != MS_RS485_CODE_AB )
        {
            //
            // Code 不對, NG
            //
            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_AB_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;


        }
        else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series & 0xFF ) != SCI_Module_Variables.SCIC_Rx_Data[2] )
        {
            //
            // 機種不一樣, NG
            //
            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Machine_Type_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MACHINE_TYPE_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

        }
        #ifndef DEVELOP_SV48
        else if ( ( ( ( DSP_FW_Version_Code >> 8 ) & 0xFF ) != SCI_Module_Variables.SCIC_Rx_Data[3] )
                  || ( ( ( DSP_FW_Version_Code ) & 0xFF ) != SCI_Module_Variables.SCIC_Rx_Data[4] ) )
        {
            //
            // 版本不一樣, NG
            //
            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_FW_Version_Code_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_FW_VER_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

        }
        #endif
        else
        {
            //
            // 資料OK, 準備回傳
            //

            Uint16 i;

            //
            // 初始化 Tx Data
            //
            for ( i = 0; i < 14; i ++ )
            {
                SCI_Module_Variables.SCIC_Tx_Data[i] = 0xFF;
            }

            //
            // Code
            //
            SCI_Module_Variables.SCIC_Tx_Data[0] = MS_RS485_CODE_AB;

            //
            // Address ( Master下一台機器的address固定為0x01 )
            //
            SCI_Module_Variables.SCIC_Tx_Data[1] = 0x01;

            //
            // MS_Output_Phase
            //
            SCI_Module_Variables.SCIC_Tx_Data[2] = Global_Variables.MS_Function_Variables.MS_Output_Phase & 0xFF;

            //
            // 觸發傳送
            //
            SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

            //
            // Reset接收旗標, 準備接收資料
            //
            //SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;
            // 20230310 避免沒收到資料以及統一寫法, 這裡不執行

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_MASTER1;

        }

    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_MASTER1_Process(void)
{
    //
    // Master-1
    //

    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // 收到資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;

        if ( SCI_Module_Variables.SCIC_Rx_Data[ 0 ] == MS_RS485_CODE_BA )
        {
            //
            // RS485的資料有收到MS_RS485_CODE_BA, 表示Slave已經完成交握
            // 則進入連線OK
            //

            //
            // 清掉CMD
            //
            MS_Control_Module_Variables.Master_SCI_Read_CMD.all = 0;
            MS_Control_Module_Variables.Master_SCI_Write_CMD.all = 0;

            //
            // 進行同步
            //
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 1;
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_81_Tx_Trig = 1;
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_82_Tx_Trig = 1;
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_83_Tx_Trig = 1;
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_84_Tx_Trig = 1;


            #ifndef DEVELOP_SV51
            //
            // SDRAM同步
            //
            Event_Module.Event_Flag.bit.MS_Sync_SDRAM_Process = 1;
            #endif

            // 設定Busy狀態
            //
            Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;

            //
            // Reset NG timer
            //
            MS_Output_Status_Check_Start_Time_reset();

            //
            // 設定工作模式旗標
            //
            Global_Variables.OUTPUT_MODE_FLAG.bit.MS_Master = 1;

            //
            // 更新單機數
            //
            if  ( ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _3P4W_3UNITS )
                  || ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _1P2W_3UNITS ) )
            {
                Global_Variables.MS_Function_Variables.MS_Total_Units = 3;
            }
            else
            {
                Global_Variables.MS_Function_Variables.MS_Total_Units = 2;

            }

            //
            // 關閉優化
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 0;

            Init_SPI_B_GPIO_for_Master();
            Init_SPI_B_Reg_for_Master();

            //
            // Disable SPI接收
            //
            n_SLAVE_ENA_1;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_OK_STATE;

        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_SLAVE0_Process(void)
{
    //
    // Slave-0
    //


    if ( GpioDataRegs.GPBDAT.bit.GPIO61 == 1 )
    {
        //
        // 上一台單機有設定PAR_ENA = High
        //

        Uint16 i;

        //
        // 初始化 Tx Data
        //
        for ( i = 0; i < 14; i ++ )
        {
            SCI_Module_Variables.SCIC_Tx_Data[i] = 0xFF;
        }

        //
        // Code
        //
        SCI_Module_Variables.SCIC_Tx_Data[0] = MS_RS485_CODE_AB;

        //
        // Address
        //
        SCI_Module_Variables.SCIC_Tx_Data[1] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;

        //
        // Machine Type
        //
        SCI_Module_Variables.SCIC_Tx_Data[2] = Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series & 0xFF;

        //
        // DSP Version-H
        //
        SCI_Module_Variables.SCIC_Tx_Data[3] = ( DSP_FW_Version_Code >> 8 ) & 0xFF;

        //
        // DSP Version-L
        //
        SCI_Module_Variables.SCIC_Tx_Data[4] = DSP_FW_Version_Code & 0xFF;


        //
        // 觸發傳送
        //
        SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

        //
        // Reset overtime
        //
        MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
        MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

        //
        // Reset接收旗標, 準備接收資料
        //
        //SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;
        // 20230310 避免沒收到資料以及統一寫法, 這裡不執行

        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_SLAVE1;
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_SLAVE1_Process(void)
{
    //
    // Slave-1
    //

    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // 收到資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;

        if ( SCI_Module_Variables.SCIC_Rx_Data[ 0 ] != MS_RS485_CODE_AB )
        {
            //
            // Code 不對, NG
            //
            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Slave_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_AB_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

        }
        else
        {
            //
            // 接收資料ok
            //

            //
            // 來自上一台指定的Address
            //
            Global_Variables.MS_Function_Variables.MS_Address = SCI_Module_Variables.SCIC_Rx_Data[ 1 ];

            Global_Variables.MS_Function_Variables.MS_Output_Phase = SCI_Module_Variables.SCIC_Rx_Data[ 2 ];

            //
            // 判斷是否還有下一台
            //
            if  ( ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _3P4W_3UNITS )
                  || ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _1P2W_3UNITS ) )
            {
                //
                // 3P4W or 1P2W(3 Units)
                //

                if ( Global_Variables.MS_Function_Variables.MS_Address == 1 )
                {
                    //
                    // 本機Address #1, 還有下一台
                    //

                    //
                    // Enable下一台
                    //
                    PAR_ENA_1;

                    //
                    // Reset接收旗標, 準備接收資料
                    //
                    //SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;
                    // 20230310 避免沒收到資料以及統一寫法, 這裡不執行

                    //
                    // 轉態
                    //
                    MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_SLAVE3;
                }
                else if ( Global_Variables.MS_Function_Variables.MS_Address == 2 )
                {
                    //
                    // 本機Address #2, 最後一台
                    //

                    //
                    // 轉態
                    //
                    MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_SLAVE2;
                }
                else
                {
                    //
                    // 以上都不是, 則Address NG
                    //
                    Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
                    Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_ADDRESS_NG;

                    //
                    // 轉態
                    //
                    MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
                }
            }
            else if  ( ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _1P2W_2UNITS )
                       || ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _1P3W_2UNITS ) )
            {
                //
                // 1P2W(2 Units) or 1P3W
                //

                if ( Global_Variables.MS_Function_Variables.MS_Address == 1 )
                {
                    //
                    // 本機Address #1, 最後一台
                    //

                    //
                    // 轉態
                    //
                    MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_SLAVE2;
                }
                else
                {
                    //
                    // 以上都不是, 則Address NG
                    //
                    Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
                    Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_ADDRESS_NG;

                    //
                    // 轉態
                    //
                    MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
                }
            }
        }
    }
    else
    {
        //
        // 沒收到資料, 檢查是否逾時
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // 逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Slave_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_AB_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }

    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_SLAVE2_Process(void)
{
    //
    // Slave-2
    //

    //
    // 最後一台Slave傳送MS_RS485_CODE_BA並進入連線ok
    //

    Uint16 i;

    //
    // 初始化 Tx Data
    //
    for ( i = 0; i < 14; i ++ )
    {
        SCI_Module_Variables.SCIC_Tx_Data[i] = 0xFF;
    }

    //
    // Code
    //
    SCI_Module_Variables.SCIC_Tx_Data[0] = MS_RS485_CODE_BA;

    //
    // Address
    //
    SCI_Module_Variables.SCIC_Tx_Data[1] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;

    //
    // Machine Type
    //
    SCI_Module_Variables.SCIC_Tx_Data[2] = Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series & 0xFF;

    //
    // DSP Version-H
    //
    SCI_Module_Variables.SCIC_Tx_Data[3] = ( DSP_FW_Version_Code >> 8 ) & 0xFF;

    //
    // DSP Version-L
    //
    SCI_Module_Variables.SCIC_Tx_Data[4] = DSP_FW_Version_Code & 0xFF;

    //
    // 觸發傳送
    //
    SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;


    //
    // 設定工作模式旗標
    //
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_Slave = 1;

    //
    // 更新單機數
    //
    if  ( ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _3P4W_3UNITS )
          || ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _1P2W_3UNITS ) )
    {
        Global_Variables.MS_Function_Variables.MS_Total_Units = 3;
    }
    else
    {
        Global_Variables.MS_Function_Variables.MS_Total_Units = 2;

    }

    //
    // 關閉優化
    //
    Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 0;

    //
    // Reset overtime
    //
    //MS_Control_Module_Variables.Slave_Wait_Rx_Start_Time = CpuTimer2.InterruptCount;
    MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
    MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;


    Init_SPI_B_GPIO_for_Slave();
    Init_SPI_B_Reg_for_Slave();

    //
    // Enable SPI接收
    //
    n_SLAVE_ENA_0;


}

/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_SLAVE3_Process(void)
{
    //
    // Slave-3
    //

    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;

        if ( SCI_Module_Variables.SCIC_Rx_Data[ 0 ] != MS_RS485_CODE_AB )
        {
            //
            // Code 不對, NG
            //
            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_AB_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

        }
        else if ( ( Global_Variables.MACHINE_SERIES_IMFORMATION.Machine_series & 0xFF ) != SCI_Module_Variables.SCIC_Rx_Data[2] )
        {
            //
            // 機種不一樣, NG
            //
            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Machine_Type_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MACHINE_TYPE_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

        }
        #ifndef DEVELOP_SV48
        else if ( ( ( ( DSP_FW_Version_Code >> 8 ) & 0xFF ) != SCI_Module_Variables.SCIC_Rx_Data[3] )
                  || ( ( ( DSP_FW_Version_Code ) & 0xFF ) != SCI_Module_Variables.SCIC_Rx_Data[4] ) )
        {
            //
            // 版本不一樣, NG
            //
            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_FW_Version_Code_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_FW_VER_NG;


            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

        }
        #endif
        else
        {
            Uint16 i;

            //
            // 初始化 Tx Data
            //
            for ( i = 0; i < 14; i ++ )
            {
                SCI_Module_Variables.SCIC_Tx_Data[i] = 0xFF;
            }

            //
            // Code
            //
            SCI_Module_Variables.SCIC_Tx_Data[0] = MS_RS485_CODE_AB;

            //
            // Address ( 下一台Slave Address固定為2 )
            //
            SCI_Module_Variables.SCIC_Tx_Data[1] = 0x02;

            //
            // MS_Output_Phase
            //
            SCI_Module_Variables.SCIC_Tx_Data[2] = Global_Variables.MS_Function_Variables.MS_Output_Phase & 0xFF;

            //
            // 觸發傳送
            //
            SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

            //
            // Reset overtime
            //
            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            //
            // Reset接收旗標, 準備接收資料
            //
            //SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;
            // 20230310 避免沒收到資料以及統一寫法, 這裡不執行

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_STATE_SLAVE4;
        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_STATE_SLAVE4_Process(void)
{
    //
    // Slave-4
    //

    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;

        if ( SCI_Module_Variables.SCIC_Rx_Data[ 0 ] == MS_RS485_CODE_BA )
        {
            //
            // RS485的資料有傳送MS_RS485_CODE_BA, 則進入連線OK
            //

            //
            // 設定工作模式旗標
            //
            Global_Variables.OUTPUT_MODE_FLAG.bit.MS_Slave = 1;

            //
            // 更新單機數
            //
            if  ( ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _3P4W_3UNITS )
                  || ( Global_Variables.MS_Function_Variables.MS_Output_Phase == _1P2W_3UNITS ) )
            {
                Global_Variables.MS_Function_Variables.MS_Total_Units = 3;
            }
            else
            {
                Global_Variables.MS_Function_Variables.MS_Total_Units = 2;

            }

            //
            // 關閉優化
            //
            Global_Variables.POWER_FUNCTIONS_ENABLE_FLAGS2.bit.Display_Modify_Enable = 0;

            //
            // Reset overtime
            //
            //MS_Control_Module_Variables.Slave_Wait_Rx_Start_Time = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            Init_SPI_B_GPIO_for_Slave();
            Init_SPI_B_Reg_for_Slave();

            //
            // Enable SPI接收
            //
            n_SLAVE_ENA_0;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_OK_STATE;
        }
    }
    else
    {
        //
        // 沒收到資料, 檢查是否逾時
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // 逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Slave_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_BA_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }
    }
}


/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_MASTER0_Process(void)
{
    Uint16 i;

    //
    // 初始化 Tx Data
    //
    for ( i = 0; i < 14; i ++ )
    {
        SCI_Module_Variables.SCIC_Tx_Data[i] = 0xFF;
    }
    SCI_Module_Variables.SCIC_Tx_Data[0] = MS_RS485_CODE_CD; //Code
    SCI_Module_Variables.SCIC_Tx_Data[1] = Global_Variables.MS_Function_Variables.MS_Address; //MS Address

    //
    // 觸發傳送
    //
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
    {
        SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

        //
        // Reset overtime
        //
        MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
        MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_MASTER1;
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_MASTER1_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready == 1 )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            if ( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_CD )
            {
                //
                // Code OK
                //

                //
                // Reset Counter
                //
                MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
                MS_Control_Module_Variables.CpuTimer2_Count = WAIT_TIME0;

                //
                // 初始化Wave Page Counter
                //
                MS_Control_Module_Variables.Sync_Wave_Page_Counter = WAVE_ARB1;

                //
                // 切換CPU0工作模式
                //
                Global_Variables.OUTPUT_MODE_FLAG.bit.MS_SDRAM_SYNC = 1;
                Global_Variables.OUTPUT_MODE_FLAG_Shadow.all = Global_Variables.OUTPUT_MODE_FLAG.all;

                Init_SPI_B_Reg_for_Master_SDRAM_SYNC();

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_MASTER_Pending0;
            }
            else if ( SCI_Module_Variables.SCIC_Rx_Data[0] != 0xCD )
            {
                //
                // Code NG
                //

                //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_SDRAM_SYNC_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_CD_NG;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }
        }
    }
    else if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready == 0 )
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_SDRAM_SYNC_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_CD_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_MASTER_Pending0_Process(void)
{
    if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
    {

        //
        // 初始化Wave addr Counter以及CheckSum
        //
        MS_Control_Module_Variables.Sync_Wave_Addr_Counter = 0;
        MS_Control_Module_Variables.Sync_Wave_data_CheckSum = 0;

        //
        // 轉態
        //
        //cpu_timer_module.cpu_timer0_schedule01_state = MS_SDRAM_Sending_Process0;
        cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Process0;

        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_MASTER_Pending1;

    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_MASTER_Pending1_Process(void)
{
    //if ( cpu_timer_module.cpu_timer0_schedule01_state == MS_SDRAM_Sending_Process4 )
    if ( cpu_timer_module.MS_SDRAM_Sending_State == MS_SDRAM_Sending_Process4 )
    {
        //
        // 主中斷 SPI傳送完畢
        //

        //
        // Reset overtime
        //
        MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
        MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

        //
        // 轉態
        //
        //cpu_timer_module.cpu_timer0_schedule01_state = CPU_TIMER0_SCHEDULE01_01_STATE;
        cpu_timer_module.MS_SDRAM_Sending_State = MS_SDRAM_Sending_Ready;

        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_MASTER2;

    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_MASTER2_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready == 1 )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;



        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            if ( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_77 )
            {
                //
                // Code OK
                //

                //
                // Wave Page切到下一組
                //
                MS_Control_Module_Variables.Sync_Wave_Page_Counter ++;

                if ( MS_Control_Module_Variables.Sync_Wave_Page_Counter <= WAVE_TRI )
                {
                    //
                    // WAVE還沒傳完
                    //

                    //
                    // Reset Counter
                    //
                    MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
                    MS_Control_Module_Variables.CpuTimer2_Count = WAIT_TIME0;

                    //
                    // 轉態
                    //
                    MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_MASTER_Pending0;

                }
                else
                {
                    //
                    // 傳送完畢
                    //

                    //
                    // 轉態
                    //
                    MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_MASTER3;

                }
            }
            else
            {
                //
                // Code NG
                //

                //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_SDRAM_SYNC_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_77_NG;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }

        }
    }
    else if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready == 0 )
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_SDRAM_SYNC_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_77_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_MASTER3_Process(void)
{
    //
    // SDRAM同步結束
    //
    Event_Module.Event_Flag.bit.MS_Sync_SDRAM_Process = 0;

    //
    // 清掉CMD
    //
    MS_Control_Module_Variables.Master_SCI_Read_CMD.all = 0;
    MS_Control_Module_Variables.Master_SCI_Write_CMD.all = 0;

    //
    // Reset NG timer
    //
    MS_Output_Status_Check_Start_Time_reset();

    //
    // 切換CPU0工作模式
    //
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_SDRAM_SYNC = 0;
    Global_Variables.OUTPUT_MODE_FLAG_Shadow.all = Global_Variables.OUTPUT_MODE_FLAG.all;

    //
    // Reset SPI
    //
    Init_SPI_B_Reg_for_Master();

}


/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_SLAVE_init_Process(void)
{
    //
    // 設定SPI通訊
    //
    Init_SPI_B_Reg_for_Slave_SDRAM_SYNC();

    //
    // 切換CPU0工作模式
    //
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_SDRAM_SYNC = 1;
    Global_Variables.OUTPUT_MODE_FLAG_Shadow.all = Global_Variables.OUTPUT_MODE_FLAG.all;

}


/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_SLAVE0_Process(void)
{
    //
    // Reset Counter, 觸發主中斷進入"MS_SDRAM_SYNC_STATE"
    //

    //
    // 初始設定Wave addr Counter以及CheckSum
    //
    MS_Control_Module_Variables.Sync_Wave_Addr_Counter = 0;
    MS_Control_Module_Variables.Sync_Wave_data_CheckSum = 0;
    MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow = 0;

    //
    // Reset Timeout Counter
    //
    MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
    //MS_Control_Module_Variables.CpuTimer2_Count = SPI_RX_OVER_TIME;
    MS_Control_Module_Variables.CpuTimer2_Count = 3000;

    //
    // 轉態
    //
    //cpu_timer_module.cpu_timer0_schedule01_state = MS_SDRAM_Receving_Process0;
    cpu_timer_module.MS_SDRAM_Receving_State = MS_SDRAM_Receving_Process0;

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_SLAVE_Pending0_Process(void)
{
    //if ( cpu_timer_module.cpu_timer0_schedule01_state == MS_SDRAM_Receving_Process4 )
    if ( cpu_timer_module.MS_SDRAM_Receving_State == MS_SDRAM_Receving_Process4 )
    {
        //
        // 接收4096及CheckSum完畢
        //

        //
        // 轉態
        //
        //cpu_timer_module.cpu_timer0_schedule01_state = CPU_TIMER0_SCHEDULE01_01_STATE;
        cpu_timer_module.MS_SDRAM_Receving_State = MS_SDRAM_Receving_Ready;

        if ( MS_Control_Module_Variables.Sync_Wave_data_CheckSum_shadow == MS_Control_Module_Variables.Sync_Wave_data_CheckSum )
        {
            //
            // CheckSum OK
            //

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE1;
        }
        else
        {
            //
            // CheckSum NG
            //

            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_SDRAM_SYNC_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = MS_SYNC_SDRAM_CHECKSUM_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }
    }
    //else if ( cpu_timer_module.cpu_timer0_schedule01_state != MS_SDRAM_Receving_Process4 )
    else if ( cpu_timer_module.MS_SDRAM_Receving_State != MS_SDRAM_Receving_Process4 )
    {
        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // 主中斷一直沒有進入MS_SDRAM_Receving_Process4, 接受逾時NG
            //

            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_SDRAM_SYNC_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_SPI_CHECKSUM_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            //
            // 轉態
            //
            //cpu_timer_module.cpu_timer0_schedule01_state = CPU_TIMER0_SCHEDULE01_01_STATE;
            cpu_timer_module.MS_SDRAM_Receving_State = MS_SDRAM_Receving_Ready;

        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_SLAVE1_Process(void)
{
    //
    // 將RAM的WAVE資料轉存到SDRAM
    //

    Uint32 i;
    Uint32 wave_page, CheckSum_page;

    wave_page = (Uint32)MS_Control_Module_Variables.Sync_Wave_Page_Counter << 12;
    CheckSum_page = wave_page + 0x14000;

    for ( i = 0; i < 4096; i++ ) //48 x 4096 cycles
    {
        *( SDRAM_variables.SDRAMBuf + wave_page + i ) = WAVE_DATA.local_ram[5][i];

        //Checksum
        *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ) = ( (int16)WAVE_DATA.local_ram[5][i] >> 8 ) + (int16)WAVE_DATA.local_ram[5][i]; // 11cycles
    }

    if ( Global_Variables.MS_Function_Variables.MS_Total_Units == ( Global_Variables.MS_Function_Variables.MS_Address + 1 ) )
    {
        //
        // 本機為最後一台, 則回傳再轉態
        //

        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE2;
    }
    else
    {
        //
        // 本機不是最後一台, 則轉態
        //

        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE3;
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_SLAVE2_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
    {
        // Code
        SCI_Module_Variables.SCIC_Tx_Data[0] = MS_RS485_CODE_77;

        // Address
        SCI_Module_Variables.SCIC_Tx_Data[1] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;

        //
        // 觸發傳送
        //
        SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE3;
    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_SLAVE3_Process(void)
{
    //
    // 切換到下一組Wave Page
    //
    MS_Control_Module_Variables.Sync_Wave_Page_Counter ++;

    if ( MS_Control_Module_Variables.Sync_Wave_Page_Counter <= WAVE_TRI )
    {
        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE0;

    }
    else
    {
        //
        // 轉態
        //
        MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE4;

    }

}

/////////////////////////////////////////////////////////////////////
static inline MS_SDRAM_SYNC_STATE_SLAVE4_Process(void)
{
    //
    // SDRAM同步結束
    //
    Event_Module.Event_Flag.bit.MS_Sync_SDRAM_Process = 0;

    //
    // 觸發事件
    //
    Event_Module.Event2_Flag.bit.Preload_WAVE_DATA = 1;

    //
    // Reset overtime
    //
    MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
    MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

    //
    // 切換CPU0工作模式
    //
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_SDRAM_SYNC = 1;
    Global_Variables.OUTPUT_MODE_FLAG_Shadow.all = Global_Variables.OUTPUT_MODE_FLAG.all;

    //
    // Reset SPI
    //
    Init_SPI_B_Reg_for_Slave();
}

/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_OK_STATE_Master_Process(void)
{
    /*
    if ( Calc_Elapsed_Time( cpu_timer_module.MS_Sync_0_Singal_Start_Time, CpuTimer0.InterruptCount ) > MS_SYNC_TRIG_HOLD_TIME )
    {
        //
        // Sync Trig 0, 同步控制用的trig指令
        // 固定觸發持續約1ms就清除
        //
        Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = 0;
    }
    */


    if ( ( ( Global_Variables.MS_Function_Variables.MS_Total_Units == 2 )
           && ( Global_Variables.MS_Function_Variables.MS_Power_Status[0].bit.Output_ON != Global_Variables.MS_Function_Variables.MS_Power_Status[1].bit.Output_ON ) )
       || ( ( Global_Variables.MS_Function_Variables.MS_Total_Units == 3 )
            && ( ( Global_Variables.MS_Function_Variables.MS_Power_Status[0].bit.Output_ON != Global_Variables.MS_Function_Variables.MS_Power_Status[1].bit.Output_ON )
                 || ( Global_Variables.MS_Function_Variables.MS_Power_Status[0].bit.Output_ON != Global_Variables.MS_Function_Variables.MS_Power_Status[2].bit.Output_ON ) ) ) )
    {
        //
        // Master檢查Slave輸出狀態是否與本機不一致
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.MS_Output_Status_Check_Start_Time, CpuTimer2.InterruptCount ) > MS_OUTPUT_STATUS_NG_OVER_TIME )
        {
            //
            // 逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Output_Status_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_POWER_STATUS_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }
    }
    else
    {
        MS_Output_Status_Check_Start_Time_reset();
    }




    //
    // 循環傳送命令
    //
    // MASTER_CMD_SCHEDULE_00_TX -> MASTER_CMD_SCHEDULE_00_RX
    // -> MASTER_CMD_SUB_SCHEDULE_01_TX ->  MASTER_CMD_SUB_SCHEDULE_01_RX
    // -> MASTER_CMD_SCHEDULE_00_TX -> MASTER_CMD_SCHEDULE_00_RX
    // -> MASTER_CMD_SUB_SCHEDULE_02_TX ->  MASTER_CMD_SUB_SCHEDULE_02_RX
    // -> MASTER_CMD_SCHEDULE_00_TX -> MASTER_CMD_SCHEDULE_00_RX
    // -> MASTER_CMD_SUB_SCHEDULE_03_TX ->  MASTER_CMD_SUB_SCHEDULE_02_RX
    // ...
    //
    switch( MS_Control_Module_Variables.Master_CMD_Schedule_State )
    {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MASTER_CMD_SCHEDULE_00_TX:
        {
            //
            #ifdef DEVELOP_SV01
            /*--------------------Cycle量測 Step2--------------------*/
            cpu_timer_module.MS_SCI_RX_Cycle_Meas_Result = CpuTimer0.InterruptCount - cpu_timer_module.MS_SCI_RX_Cycle_Meas_Start; //8 cycles
            // 對應的執行時間 = ( Cycle_Meas_Result ) x 2.775us

            /*--------------------Cycle量測 Step1--------------------*/
            cpu_timer_module.MS_SCI_RX_Cycle_Meas_Start = CpuTimer0.InterruptCount;
            #endif


            MASTER_CMD_SCHEDULE_00_TX_Process();

            break;

        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MASTER_CMD_SCHEDULE_00_RX:
        {
            MASTER_CMD_SCHEDULE_00_RX_Process();
            break;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case MASTER_CMD_SUB_SCHEDULE:
        {

            switch( MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State )
            {
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_01_TX:
                {
                    MASTER_CMD_SUB_SCHEDULE_01_TX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_01_RX:
                {
                    MASTER_CMD_SUB_SCHEDULE_01_RX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_02_TX:
                {
                    MASTER_CMD_SUB_SCHEDULE_02_TX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_02_RX:
                {
                    MASTER_CMD_SUB_SCHEDULE_02_RX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_03_TX:
                {
                    MASTER_CMD_SUB_SCHEDULE_03_TX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_03_RX:
                {
                    MASTER_CMD_SUB_SCHEDULE_03_RX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_04_TX:
                {
                    MASTER_CMD_SUB_SCHEDULE_04_TX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_04_RX:
                {
                    MASTER_CMD_SUB_SCHEDULE_04_RX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_05_TX:
                {
                    MASTER_CMD_SUB_SCHEDULE_05_TX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_05_RX:
                {
                    MASTER_CMD_SUB_SCHEDULE_05_RX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_06_TX:
                {
                    MASTER_CMD_SUB_SCHEDULE_06_TX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_06_RX:
                {
                    MASTER_CMD_SUB_SCHEDULE_06_RX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_07_TX:
                {
                    MASTER_CMD_SUB_SCHEDULE_07_TX_Process();
                    break;
                }
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                case MASTER_CMD_SUB_SCHEDULE_07_RX:
                {
                    MASTER_CMD_SUB_SCHEDULE_07_RX_Process();
                    break;
                }
            }
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_OK_STATE_Slave_Process(void)
{
    //
    // Slave端 RS485 TXRX狀態切換
    //
    switch( MS_Control_Module_Variables.Slave_RS485_State )
    {
        case SLAVE_RS485_RX:
        {
            SLAVE_RS485_RX_Process();

            break;
        }

        case SLAVE_RS485_TX:
        {
            SLAVE_RS485_TX_Process();

            break;
        }
    }
}



/////////////////////////////////////////////////////////////////////
static inline Master_CPU0_TimeOut_Scan_Process(void)
{
    //
    // 檢查是否在這些狀態卡住
    //
    switch( cpu_timer_module.cpu_timer0_state )
    {
        /* --------------------------------------------------------------------------------- */
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter ++;

            if ( MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter > 1000 )
            {
                //
                // NG
                //

                MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_Slave_Wait_Change_Sync_OVERTIME;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }

            break;
        }
        /* --------------------------------------------------------------------------------- */
        case MS_Master_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter ++;

            if ( MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter > 1000 )
            {
                //
                // NG
                //

                MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_Slave_Wait_OFF_Sync_OVERTIME;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }

            break;
        }

        /* --------------------------------------------------------------------------------- */
        default:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

            break;
        }
    }

}



/////////////////////////////////////////////////////////////////////
static inline Slave_CPU0_TimeOut_Scan_Process(void)
{
    //
    // 檢查是否在這些狀態卡住
    //
    switch( cpu_timer_module.cpu_timer0_state )
    {
        /* --------------------------------------------------------------------------------- */
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_START_PHASE_STATE0:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter ++;

            if ( MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter > 100 )
            {
                //
                // NG
                //

                MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_SPI_ON_Sync_Trig_OVERTIME;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }

            break;
        }
        /* --------------------------------------------------------------------------------- */
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE0:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter ++;

            if ( MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter > 100 )
            {
                //
                // NG
                //

                MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_SPI_Change_Sync_Trig_0_OVERTIME;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }

            break;
        }
        /* --------------------------------------------------------------------------------- */
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_CHANGE_PHASE_STATE1:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter ++;

            if ( MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter > 100 )
            {
                //
                // NG
                //

                MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_SPI_Change_Sync_Trig_1_OVERTIME;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }

            break;
        }
        /* --------------------------------------------------------------------------------- */
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE0:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter ++;

            if ( MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter > 100 )
            {
                //
                // NG
                //

                MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_SPI_OFF_Sync_Trig_0_OVERTIME;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }

            break;
        }
        /* --------------------------------------------------------------------------------- */
        case MS_Slave_ACDC_INT_CPU_TIMER0_OUTPUT_OFF_STATE1:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter ++;

            if ( MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter > 100 )
            {
                //
                // NG
                //

                MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

                Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
                Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_SPI_OFF_Sync_Trig_1_OVERTIME;

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
            }

            break;
        }

        /* --------------------------------------------------------------------------------- */
        default:
        {
            MS_Control_Module_Variables.CPU0_TimeOut_Check_Counter = 0;

            break;
        }
    }

}











/////////////////////////////////////////////////////////////////////
static inline MS_CONNECT_ERROR_HANDLE_STATE_Process(void)
{
    //
    // 設定 PAR_ENA 輸出Low
    //
    PAR_ENA_0;

    //
    // Reset SCI
    //
    ScicRegs.SCICTL1.bit.SWRESET = 0;       // Reset SCI
    ScicRegs.SCICTL1.bit.SWRESET = 1;

    //
    // Reset工作模式旗標
    //
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_Master = 0;
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_Slave = 0;
    Global_Variables.OUTPUT_MODE_FLAG.bit.MS_SDRAM_SYNC = 0;

    //
    // 清掉事件
    //
    Event_Module.Event_Flag.bit.MS_Sync_SDRAM_Process = 0;

    //
    // 轉態
    //
    Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;

}






/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SCHEDULE_00_TX_Process(void)
{
    SCI_CMD_00_Send();

    //
    // 觸發傳送
    //
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
    {
        SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

        //
        // Reset overtime
        //
        MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
        MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_RX;

    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SCHEDULE_00_RX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;



        if ( SCI_Module_Variables.SCIC_Rx_Data[0] != MS_RS485_CODE_00 )
        {
            //
            // Code NG
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_00_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }

        //
        // 接收資料處理
        //
        SCI_CMD_00_Receive();


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //


            if ( Event_Module.Event_Flag.bit.MS_Sync_SDRAM_Process == 1 )
            {
                //
                // 觸發SDRAM同步流程
                //

                //
                // 轉態
                //
                MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_MASTER0;

            }

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SUB_SCHEDULE;
        }

    }
    else
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_00_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }
    }

}



/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_01_TX_Process(void)
{
    SCI_CMD_01_Send();

    //
    // 觸發傳送
    //
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
    {
        SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

        //
        // Reset overtime
        //
        MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
        MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;


        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_01_RX;

    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_01_RX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;


        if ( SCI_Module_Variables.SCIC_Rx_Data[0] != MS_RS485_CODE_01 )
        {
            //
            // Code NG
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_01_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }

        //
        // 接收資料處理
        //
        SCI_CMD_01_Receive();


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_02_TX;
        }

    }
    else
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_01_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_02_TX_Process(void)
{
    SCI_CMD_02_Send();

    //
    // 觸發傳送
    //
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
    {
        SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

        //
        // Reset overtime
        //
        MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
        MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;


        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_02_RX;
    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_02_RX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;


        if ( SCI_Module_Variables.SCIC_Rx_Data[0] != MS_RS485_CODE_02 )
        {
            //
            // Code NG
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_02_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }

        //
        // 接收資料處理
        //
        SCI_CMD_02_Receive();


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_03_TX;

        }

    }
    else
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_02_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_03_TX_Process(void)
{
    //
    // Master設定Slave指令處理
    //

    //
    // Master比對資料是否與Table不同, 觸發Tx_Trig
    //
    Master_SCI_CMD_80_Change_Check();

    //
    // 程式內部也會觸發 Tx_Trig, 強制執行同步
    //

    if ( MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig )
    {
        Master_SCI_CMD_80_Send();

        //
        // 觸發傳送
        //
        if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
        {
            SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

            //
            // 清掉SPI同步指令
            //
            Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = 0;

            //
            // SCI有傳送才清掉旗標
            //
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_80_Tx_Trig = 0;

            //
            // Master更新到內部的Table
            //
            Master_SCI_CMD_80_Table_Update();

            //
            // Reset overtime
            //
            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_03_RX;
        }

    }
    else
    {
        //
        // 比對資料與Table相同
        //

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_04_TX;

    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_03_RX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;



        if ( SCI_Module_Variables.SCIC_Rx_Data[0] != MS_RS485_CODE_80 )
        {
            //
            // Code NG
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_80_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_04_TX;


        }

    }
    else
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_80_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;


        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_04_TX_Process(void)
{
    //
    // Master設定Slave指令處理
    //

    //
    // Master比對資料是否與Table不同, 觸發Tx_Trig
    //
    Master_SCI_CMD_81_Change_Check();

    //
    // 程式內部也會觸發 Tx_Trig, 強制執行同步
    //

    if ( MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_81_Tx_Trig )
    {
        Master_SCI_CMD_81_Send();

        //
        // 觸發傳送
        //
        if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
        {
            SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

            //
            // 清掉SPI同步指令
            //
            Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = 0;

            //
            // SCI有傳送才清掉旗標
            //
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_81_Tx_Trig = 0;

            //
            // Master更新到內部的Table
            //
            Master_SCI_CMD_81_Table_Update();

            //
            // Reset overtime
            //
            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_04_RX;
        }

    }
    else
    {
        //
        // 比對資料與Table相同
        //

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_05_TX;
    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_04_RX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;



        if ( SCI_Module_Variables.SCIC_Rx_Data[0] != MS_RS485_CODE_81 )
        {
            //
            // Code NG
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_81_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_05_TX;


        }

    }
    else
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_81_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;


        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_05_TX_Process(void)
{
    //
    // Master設定Slave指令處理
    //

    //
    // Master比對資料是否與Table不同, 觸發Tx_Trig
    //
    Master_SCI_CMD_82_Change_Check();

    //
    // 程式內部也會觸發 Tx_Trig, 強制執行同步
    //

    if ( MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_82_Tx_Trig )
    {
        Master_SCI_CMD_82_Send();

        //
        // 觸發傳送
        //
        if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
        {
            SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

            //
            // 清掉SPI同步指令
            //
            Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = 0;

            //
            // SCI有傳送才清掉旗標
            //
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_82_Tx_Trig = 0;

            //
            // Master更新到內部的Table
            //
            Master_SCI_CMD_82_Table_Update();

            //
            // Reset overtime
            //
            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_05_RX;
        }

    }
    else
    {
        //
        // 比對資料與Table相同
        //

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_06_TX;

    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_05_RX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;



        if ( SCI_Module_Variables.SCIC_Rx_Data[0] != MS_RS485_CODE_82 )
        {
            //
            // Code NG
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_82_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_06_TX;


        }

    }
    else
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_82_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;


        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_06_TX_Process(void)
{
    //
    // Master設定Slave指令處理
    //

    //
    // Master比對資料是否與Table不同, 觸發Tx_Trig
    //
    Master_SCI_CMD_83_Change_Check();

    //
    // 程式內部也會觸發 Tx_Trig, 強制執行同步
    //

    if ( MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_83_Tx_Trig )
    {
        Master_SCI_CMD_83_Send();

        //
        // 觸發傳送
        //
        if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
        {
            SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

            //
            // 清掉SPI同步指令
            //
            Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = 0;

            //
            // SCI有傳送才清掉旗標
            //
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_83_Tx_Trig = 0;

            //
            // Master更新到內部的Table
            //
            Master_SCI_CMD_83_Table_Update();

            //
            // Reset overtime
            //
            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_06_RX;


        }

    }
    else
    {
        //
        // 比對資料與Table相同
        //

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_07_TX;


    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_06_RX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;



        if ( SCI_Module_Variables.SCIC_Rx_Data[0] != MS_RS485_CODE_83 )
        {
            //
            // Code NG
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_83_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_07_TX;


        }

    }
    else
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_83_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;


        }
    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_07_TX_Process(void)
{
    //
    // Master設定Slave指令處理
    //

    //
    // Master比對資料是否與Table不同, 觸發Tx_Trig
    //
    Master_SCI_CMD_84_Change_Check();

    //
    // 程式內部也會觸發 Tx_Trig, 強制執行同步
    //

    if ( MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_84_Tx_Trig )
    {
        Master_SCI_CMD_84_Send();

        //
        // 觸發傳送
        //
        if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
        {
            SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

            //
            // 清掉SPI同步指令
            //
            Global_Variables.MS_Function_Variables.Output_Sync_Trig.all = 0;

            //
            // SCI有傳送才清掉旗標
            //
            MS_Control_Module_Variables.Master_SCI_Write_CMD.bit.Master_SCI_CMD_84_Tx_Trig = 0;

            //
            // Master更新到內部的Table
            //
            Master_SCI_CMD_84_Table_Update();

            //
            // Reset overtime
            //
            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_07_RX;


        }

    }
    else
    {
        //
        // 比對資料與Table相同
        //

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

        //
        // 轉態
        //
        MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_01_TX;


    }

}

/////////////////////////////////////////////////////////////////////
static inline MASTER_CMD_SUB_SCHEDULE_07_RX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;



        if ( SCI_Module_Variables.SCIC_Rx_Data[0] != MS_RS485_CODE_84 )
        {
            //
            // Code NG
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_CMD_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = CHECK_MS_RS485_CODE_84_NG;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;

            return;
        }


        if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Total_Units - 1 ) )
        {
            //
            // 資料來自最後一台
            //

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Schedule_State = MASTER_CMD_SCHEDULE_00_TX;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Master_CMD_Sub_Schedule_State = MASTER_CMD_SUB_SCHEDULE_01_TX;


        }

    }
    else
    {
        //
        // 沒收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // Master等接收Slave Rx逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Master_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = WAIT_MS_RS485_CODE_84_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;


        }
    }

}





/////////////////////////////////////////////////////////////////////
static inline SLAVE_RS485_RX_Process(void)
{
    //
    // Slave 持續接收, 判斷Code然後回傳
    //

    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready == 1 )
    {
        //
        // Rx收到有效資料
        //

        SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready = 0;


        //
        // 解析CMD Code處理流程
        //
        switch( SCI_Module_Variables.SCIC_Rx_Data[0] )
        {
            case MS_RS485_CODE_00:
            {
                //
                #ifdef DEVELOP_SV01
                /*--------------------Cycle量測 Step2--------------------*/
                cpu_timer_module.MS_SCI_RX_Cycle_Meas_Result = CpuTimer0.InterruptCount - cpu_timer_module.MS_SCI_RX_Cycle_Meas_Start; //8 cycles
                // 對應的執行時間 = ( Cycle_Meas_Result ) x 2.775us

                /*--------------------Cycle量測 Step1--------------------*/
                cpu_timer_module.MS_SCI_RX_Cycle_Meas_Start = CpuTimer0.InterruptCount;
                #endif

                SCI_CMD_00_Receive();
                break;
            }
            case MS_RS485_CODE_01:
            {
                SCI_CMD_01_Receive();
                break;
            }
            case MS_RS485_CODE_02:
            {
                SCI_CMD_02_Receive();
                break;
            }
            case MS_RS485_CODE_80:
            {
                if ( SCI_Module_Variables.SCIC_Rx_Data[1] == 0x00 )
                {
                    //
                    // 來自Master的指令才更新
                    //
                    Slave_SCI_CMD_80_Receive();
                }
                break;
            }
            case MS_RS485_CODE_81:
            {
                if ( SCI_Module_Variables.SCIC_Rx_Data[1] == 0x00 )
                {
                    //
                    // 來自Master的指令才更新
                    //
                    Slave_SCI_CMD_81_Receive();
                }
                break;
            }
            case MS_RS485_CODE_82:
            {
                if ( SCI_Module_Variables.SCIC_Rx_Data[1] == 0x00 )
                {
                    //
                    // 來自Master的指令才更新
                    //
                    Slave_SCI_CMD_82_Receive();
                }
                break;
            }
            case MS_RS485_CODE_83:
            {
                if ( SCI_Module_Variables.SCIC_Rx_Data[1] == 0x00 )
                {
                    //
                    // 來自Master的指令才更新
                    //
                    Slave_SCI_CMD_83_Receive();
                }
                break;
            }
            case MS_RS485_CODE_84:
            {
                if ( SCI_Module_Variables.SCIC_Rx_Data[1] == 0x00 )
                {
                    //
                    // 來自Master的指令才更新
                    //
                    Slave_SCI_CMD_84_Receive();
                }
                break;
            }
            case MS_RS485_CODE_CD:
            {
                if ( SCI_Module_Variables.SCIC_Rx_Data[1] == 0x00 )
                {
                    //
                    // 來自Master的指令才更新
                    //

                    //
                    // 觸發SDRAM同步流程
                    //
                    Event_Module.Event_Flag.bit.MS_Sync_SDRAM_Process = 1;

                    // 設定Busy狀態
                    //
                    Global_Variables.POWER_STATUS.bit.Output_ONOFF_Busy = 1;
                }
                break;
            }
        }

        //
        // 判斷是否要回傳
        //
        if ( ( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_00 )
             ||( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_01 )
             ||( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_02 ) )
        {
            if ( SCI_Module_Variables.SCIC_Rx_Data[1] == ( Global_Variables.MS_Function_Variables.MS_Address - 1 ) )
            {
                //
                // 收到來自上一台資料, 則回傳
                //

                //
                // 儲存Code
                //
                SCI_Module_Variables.SCIC_Rx_Data_0_Shadow = SCI_Module_Variables.SCIC_Rx_Data[0];

                //
                // 轉態
                //
                MS_Control_Module_Variables.Slave_RS485_State = SLAVE_RS485_TX;
            }
        }
        else if ( ( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_80 )
                  ||( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_81 )
                  ||( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_82 )
                  ||( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_83 )
                  ||( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_84 ) )
        {
            if ( Global_Variables.MS_Function_Variables.MS_Total_Units == ( Global_Variables.MS_Function_Variables.MS_Address + 1 ) )
            {
                //
                // 本機是最後一台則回傳
                //

                //
                // 儲存Code
                //
                SCI_Module_Variables.SCIC_Rx_Data_0_Shadow = SCI_Module_Variables.SCIC_Rx_Data[0];

                //
                // 轉態
                //
                MS_Control_Module_Variables.Slave_RS485_State = SLAVE_RS485_TX;
            }
        }
        else if ( SCI_Module_Variables.SCIC_Rx_Data[0] == MS_RS485_CODE_CD )
        {
            if ( Global_Variables.MS_Function_Variables.MS_Total_Units == ( Global_Variables.MS_Function_Variables.MS_Address + 1 ) )
            {
                //
                // 本機是最後一台, 先轉態TX, 傳送資料後再轉態進入同步SDRAM流程
                //

                //
                // 儲存Code
                //
                SCI_Module_Variables.SCIC_Rx_Data_0_Shadow = SCI_Module_Variables.SCIC_Rx_Data[0];

                //
                // 轉態
                //
                MS_Control_Module_Variables.Slave_RS485_State = SLAVE_RS485_TX;
            }
            else
            {
                //
                // 本機不是最後一台, 直接轉態進入同步SDRAM流程
                //

                //
                // 初始設定Wave Page Counter
                //
                MS_Control_Module_Variables.Sync_Wave_Page_Counter = WAVE_ARB1;

                //
                // 轉態
                //
                //MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE0;
                MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE_init;
            }
        }
        else
        {
            //
            // 不需要回傳, 則回到SLAVE_RS485_RX, 繼續接收
            //

            //
            // Reset overtime
            //
            //MS_Control_Module_Variables.Slave_Wait_Rx_Start_Time = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
            MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.Slave_RS485_State = SLAVE_RS485_RX;
        }
    }
    else if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_RX_Data_Ready == 0 )
    {
        //
        // 沒有收到資料
        //

        if ( Calc_Elapsed_Time( MS_Control_Module_Variables.CpuTimer2_Counter, CpuTimer2.InterruptCount ) > MS_Control_Module_Variables.CpuTimer2_Count )
        {
            //
            // 逾時
            //

            //Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_Slave_SCI_Rx_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS2.bit.MS_CONNECT_OK_STATE_Error = 1;
            Global_Variables.POWER_FUNCTIONS_ERROR_CODE = SLAVE_RS485_RX_OVERTIME;

            //
            // 轉態
            //
            MS_Control_Module_Variables.MS_Connect_State = MS_CONNECT_ERROR_HANDLE_STATE;
        }
    }
}



/////////////////////////////////////////////////////////////////////
static inline SLAVE_RS485_TX_Process(void)
{
    if ( SCI_Module_Variables.SCI_Module_Status_flag.bit.SCIC_TX_Busy == 0 )
    {
        //
        // 解析CMD Code, 準備回傳資料
        //
        switch( SCI_Module_Variables.SCIC_Rx_Data_0_Shadow )
        {
            case MS_RS485_CODE_00:
            {
                SCI_CMD_00_Send();
                break;
            }
            case MS_RS485_CODE_01:
            {
                SCI_CMD_01_Send();
                break;
            }
            case MS_RS485_CODE_02:
            {
                SCI_CMD_02_Send();
                break;
            }
            case MS_RS485_CODE_80:
            case MS_RS485_CODE_81:
            case MS_RS485_CODE_82:
            case MS_RS485_CODE_83:
            case MS_RS485_CODE_84:
            {
                // Code
                SCI_Module_Variables.SCIC_Tx_Data[0] = SCI_Module_Variables.SCIC_Rx_Data_0_Shadow;

                // Address
                SCI_Module_Variables.SCIC_Tx_Data[1] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;
                break;
            }
            case MS_RS485_CODE_CD:
            {
                // Code
                SCI_Module_Variables.SCIC_Tx_Data[0] = SCI_Module_Variables.SCIC_Rx_Data_0_Shadow;

                // Address
                SCI_Module_Variables.SCIC_Tx_Data[1] = Global_Variables.MS_Function_Variables.MS_Address & 0xFF;

                //
                // 初始設定Wave Page Counter
                //
                MS_Control_Module_Variables.Sync_Wave_Page_Counter = WAVE_ARB1;

                //
                // 轉態
                //
                //MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE0;
                MS_Control_Module_Variables.MS_Connect_State = MS_SDRAM_SYNC_STATE_SLAVE_init;

                break;
            }
        }

        //
        // 觸發傳送
        //
        SCI_Module_Variables.SCI_Module_Control_flag.bit.SCIC_TX_Trig = 1;

        //
        // Reset overtime
        //
        MS_Control_Module_Variables.CpuTimer2_Counter = CpuTimer2.InterruptCount;
        MS_Control_Module_Variables.CpuTimer2_Count = RS485_RX_OVER_TIME;

        //
        // 轉態
        //
        MS_Control_Module_Variables.Slave_RS485_State = SLAVE_RS485_RX;
    }

}








//
// End of file
//
