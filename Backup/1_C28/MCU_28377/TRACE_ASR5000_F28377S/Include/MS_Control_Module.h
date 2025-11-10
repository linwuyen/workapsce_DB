//###########################################################################
//
// FILE:    MS_Control_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20221108
//###########################################################################


#define MS_SCI_COMM_OK  0
#define MS_SCI_COMM_NG  1


//
// MS連線階段
// Master傳送RS485訊息後, 判斷逾時的時間
//
#define MS_CONNECT_OVER_TIME 100 // (ms)

//
// MS連線OK階段
//
#define MASTER_WAIT_RX_NG_COUNT             3 //次

//#define MASTER_WAIT_RX_OVER_TIME            100 //(ms)
//#define SLAVE_WAIT_RX_OVER_TIME             100 //(ms)
#define RS485_RX_OVER_TIME                  200 // (ms)

//#define SLAVE_WAIT_SPI_SYNC_OVER_TIME       1 //(ms)
//#define WAIT_MS_SPI_CODE_6AB7_OVER_TIME     100 //(ms)
#define SPI_RX_OVER_TIME                    200 // (ms)

#define MS_OUTPUT_STATUS_NG_OVER_TIME       1000 //(ms)

#define WAIT_TIME0                          50 //(ms)
#define WAIT_MS_SPI_CODE_6AB7_TIME          2000 // (ms)


//
// Master SPIB 輸出同步訊號持續時間
//
#define MS_SYNC_TRIG_HOLD_TIME      360// (2.775us)


//
// Master傳到Slave cpu_timer_module.c1 的微調offset
// 跟多久同步一次有關係
// Example.
// 排程為5, => c1_offset = 5
//
#define MS_MASTER_C1_SYNC_OFFSET    5


//#define _360k_div_4096 0.011378
#define _4096_div_360k  0.011378
#define _4096_div_180k  0.022756
#define _4096_div_3600  1.137778


//
// MS RS485 Code List
//
#define MS_RS485_CODE_AB     0xAB
#define MS_RS485_CODE_BA     0xBA
#define MS_RS485_CODE_CD     0xCD
#define MS_RS485_CODE_77     0x77
#define MS_RS485_CODE_88     0x88
#define MS_RS485_CODE_00     0x00
#define MS_RS485_CODE_01     0x01
#define MS_RS485_CODE_02     0x02
#define MS_RS485_CODE_80     0x80
#define MS_RS485_CODE_81     0x81
#define MS_RS485_CODE_82     0x82
#define MS_RS485_CODE_83     0x83
#define MS_RS485_CODE_84     0x84




//----------------------------------------------------
//
// MS Connect 狀態機
//
typedef enum
{
    MS_CONNECT_STATE_INIT0,
    MS_CONNECT_STATE_INIT1,
    MS_CONNECT_STATE_MASTER0,
    MS_CONNECT_STATE_MASTER1,

    MS_CONNECT_STATE_SLAVE0,
    MS_CONNECT_STATE_SLAVE1,
    MS_CONNECT_STATE_SLAVE2,
    MS_CONNECT_STATE_SLAVE3,
    MS_CONNECT_STATE_SLAVE4,

    MS_CONNECT_OK_STATE,

    MS_SDRAM_SYNC_STATE_MASTER0,
    MS_SDRAM_SYNC_STATE_MASTER1,
    MS_SDRAM_SYNC_STATE_MASTER_Pending0,
    MS_SDRAM_SYNC_STATE_MASTER_Pending1,
    MS_SDRAM_SYNC_STATE_MASTER2,
    MS_SDRAM_SYNC_STATE_MASTER3,

    MS_SDRAM_SYNC_STATE_SLAVE_init,
    MS_SDRAM_SYNC_STATE_SLAVE0,
    MS_SDRAM_SYNC_STATE_SLAVE_Pending0,
    MS_SDRAM_SYNC_STATE_SLAVE1,
    MS_SDRAM_SYNC_STATE_SLAVE2,
    MS_SDRAM_SYNC_STATE_SLAVE3,
    MS_SDRAM_SYNC_STATE_SLAVE4,

    MS_CONNECT_ERROR_HANDLE_STATE,
    MS_CONNECT_ERROR_STATE



}MS_CONNECT_STATE_REG;



//----------------------------------------------------
//
// Master RS485 指令排程狀態機
//
typedef enum
{
    MASTER_CMD_SCHEDULE_00_TX,
    MASTER_CMD_SCHEDULE_00_RX,
    MASTER_CMD_SUB_SCHEDULE

}MASTER_CMD_SCHEDULE_REG;


//----------------------------------------------------
//
// Master RS485 指令排程狀態機-Sub
//
typedef enum
{
    MASTER_CMD_SUB_SCHEDULE_01_TX,
    MASTER_CMD_SUB_SCHEDULE_01_RX,
    MASTER_CMD_SUB_SCHEDULE_02_TX,
    MASTER_CMD_SUB_SCHEDULE_02_RX,
    MASTER_CMD_SUB_SCHEDULE_03_TX,
    MASTER_CMD_SUB_SCHEDULE_03_RX,
    MASTER_CMD_SUB_SCHEDULE_04_TX,
    MASTER_CMD_SUB_SCHEDULE_04_RX,
    MASTER_CMD_SUB_SCHEDULE_05_TX,
    MASTER_CMD_SUB_SCHEDULE_05_RX,
    MASTER_CMD_SUB_SCHEDULE_06_TX,
    MASTER_CMD_SUB_SCHEDULE_06_RX,
    MASTER_CMD_SUB_SCHEDULE_07_TX,
    MASTER_CMD_SUB_SCHEDULE_07_RX

}MASTER_CMD_SUB_SCHEDULE_REG;



//----------------------------------------------------
//
// SLAVE RS485 TXRX狀態機
//
typedef enum
{
    SLAVE_RS485_TX,
    SLAVE_RS485_RX

}SLAVE_RS485_TXRX_STATE_REG;





//----------------------------------------------------
//
// Master SCI寫入指令區
//
struct MASTER_SCI_WRITE_CMD_BITS {    // bits
    Uint16 Master_SCI_CMD_80_Tx_Trig:1;        //0
    //Uint16 Master_SCI_CMD_80_Rx_Trig:1;        //1
    Uint16 Master_SCI_CMD_81_Tx_Trig:1;        //2
    //Uint16 Master_SCI_CMD_81_Rx_Trig:1;        //3
    Uint16 Master_SCI_CMD_82_Tx_Trig:1;        //4
    //Uint16 Master_SCI_CMD_82_Rx_Trig:1;        //5
    Uint16 Master_SCI_CMD_83_Tx_Trig:1;        //6
    //Uint16 Master_SCI_CMD_83_Rx_Trig:1;        //7
    Uint16 Master_SCI_CMD_84_Tx_Trig:1;        //8
    //Uint16 Master_SCI_CMD_84_Rx_Trig:1;        //9
    Uint16 Master_SCI_CMD_80_Tx_Reset_Trig:1;  //10
};

union MASTER_SCI_WRITE_CMD_REG {
   Uint16                 all;
   struct MASTER_SCI_WRITE_CMD_BITS bit;
};


//----------------------------------------------------
//
// Master SCI讀取指令區
//
struct MASTER_SCI_READ_CMD_BITS {    // bits
    Uint16 Master_SCI_CMD_00_Tx_Trig:1;        //0
    //Uint16 Master_SCI_CMD_00_Rx_Trig:1;        //1
    Uint16 Master_SCI_CMD_01_Tx_Trig:1;        //2
    //Uint16 Master_SCI_CMD_01_Rx_Trig:1;        //3
    Uint16 Master_SCI_CMD_02_Tx_Trig:1;        //4
    //Uint16 Master_SCI_CMD_02_Rx_Trig:1;        //5
    Uint16 Master_SCI_CMD_03_Tx_Trig:1;        //6
    //Uint16 Master_SCI_CMD_03_Rx_Trig:1;        //7
};

union MASTER_SCI_READ_CMD_REG {
   Uint16                 all;
   struct MASTER_SCI_READ_CMD_BITS bit;
};








//----------------------------------------------------
//
//
//
typedef struct MS_CONTROL_MODULE_VARIABLES_REG{

    MS_CONNECT_STATE_REG MS_Connect_State;

    MASTER_CMD_SCHEDULE_REG Master_CMD_Schedule_State;
    MASTER_CMD_SUB_SCHEDULE_REG Master_CMD_Sub_Schedule_State;

    SLAVE_RS485_TXRX_STATE_REG Slave_RS485_State;


    Uint16 Master_reCheck_Counter;

    //Uint32 Master_Wait_Rx_Start_Time;

    //Uint32 Slave_Wait_Rx_Start_Time;

    Uint32 MS_Output_Status_Check_Start_Time;

    Uint32 Slave_Wait_SPI_Sync_Code_Start_Time;

    Uint32 CpuTimer2_Counter;
    Uint32 CpuTimer2_Count;

    Uint32 CPU0_TimeOut_Check_Counter;

    union MASTER_SCI_WRITE_CMD_REG Master_SCI_Write_CMD;
    union MASTER_SCI_READ_CMD_REG Master_SCI_Read_CMD;

    Uint16 Master_SCI_CMD_80_Table[14];
    Uint16 Master_SCI_CMD_81_Table[14];
    Uint16 Master_SCI_CMD_82_Table[14];
    Uint16 Master_SCI_CMD_83_Table[14];
    Uint16 Master_SCI_CMD_84_Table[14];

    Uint16 Sync_Wave_Page_Counter;
    Uint16 Sync_Wave_Addr_Counter;
    Uint16 Sync_Wave_data_CheckSum;
    Uint16 Sync_Wave_data_CheckSum_shadow;

}MS_CONTROL_MODULE_VARIABLES_REG;


extern MS_CONTROL_MODULE_VARIABLES_REG MS_Control_Module_Variables;







//----------------------------------------------------


void Init_MS_Control_Module_Variables(void);

void MS_Connect_Process(void);

void MS_Output_Status_Check_Start_Time_reset(void);

//
// CMD Read
//

void SCI_CMD_00_Send(void);
void SCI_CMD_00_Receive(void);
void SCI_CMD_01_Send(void);
void SCI_CMD_01_Receive(void);
void SCI_CMD_02_Send(void);
void SCI_CMD_02_Receive(void);



//
// CMD Write
//

void Master_SCI_CMD_80_Table_Update(void);
void Master_SCI_CMD_80_Change_Check(void);
void Master_SCI_CMD_80_Send(void);
void Slave_SCI_CMD_80_Receive(void);

void Master_SCI_CMD_81_Table_Update(void);
void Master_SCI_CMD_81_Change_Check(void);
void Master_SCI_CMD_81_Send(void);
void Slave_SCI_CMD_81_Receive(void);

void Master_SCI_CMD_82_Table_Update(void);
void Master_SCI_CMD_82_Change_Check(void);
void Master_SCI_CMD_82_Send(void);
void Slave_SCI_CMD_82_Receive(void);

void Master_SCI_CMD_83_Table_Update(void);
void Master_SCI_CMD_83_Change_Check(void);
void Master_SCI_CMD_83_Send(void);
void Slave_SCI_CMD_83_Receive(void);

void Master_SCI_CMD_84_Table_Update(void);
void Master_SCI_CMD_84_Change_Check(void);
void Master_SCI_CMD_84_Send(void);
void Slave_SCI_CMD_84_Receive(void);















//
// End of file
//
