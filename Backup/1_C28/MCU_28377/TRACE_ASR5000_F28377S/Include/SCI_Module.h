//###########################################################################
//
// FILE:    SCI_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210915
//###########################################################################


//
// RS485傳送資料時, 控制TXC_RXC Hold為1的時間
// = 傳送資料呃時間 / CPU主中斷時間
//
// Example.
// BaudRate = 1000000 bps
// 傳送 16 byte資料 => 考慮Start及Stop bit => 160bit
// => 傳送160bit時間 = 160 / 1000000 = 160usec
// => 160us / 2.775us = 57.66 count
//
#define SCIC_TX_HOLD_COUNT 60







//----------------------------------------------------

struct  SCI_MODULE_STATUS_BITS {  // bit

    Uint16 SCIC_TX_Busy:1;                 //00
    Uint16 SCIC_RX_Data_Ready:1;           //01

};
union SCI_MODULE_STATUS_REG {
   Uint16                                    all;
   struct SCI_MODULE_STATUS_BITS   bit;
};

//----------------------------------------------------

struct  SCI_MODULE_CONTROL_BITS {  // bit
    Uint16 SCIC_TX_Trig:1;                 //00
    Uint16 SCIC_RX_Trig:1;                 //01

};
union SCI_MODULE_CONTROL_REG {
   Uint16                                    all;
   struct SCI_MODULE_CONTROL_BITS   bit;
};


//----------------------------------------------------
//
// SCIC(RS485) TXRX 狀態機
//
typedef enum
{
    SCIC_READY,
    SCIC_TX_LOAD_BUFFER,
    SCIC_TX_BUSY

}SCIC_TXRX_STATE_REG;


//----------------------------------------------------

typedef struct SCI_MODULE_VARIABLES_REG{

   Uint16  Receive_Char;
   char    *msg;
   char    Debug_Msg[100];

   char    SCIA_Rx_Data[100];
   char    SCIA_Rx_Data_shadow[100];
   Uint16  SCIA_Rx_Data_Count;
   Uint16  SCIA_Rx_Data2[2];
   Uint16  SCIA_Tx_Data2[2];
   char    SCIA_Tx_Data[100];
   char    SCIA_echo_Data[1];

   Uint16  SCIC_Tx_Data[16];
   Uint16  SCIC_Rx_Data[16];
   Uint16  SCIC_Rx_Data_Shadow[16];
   Uint16  SCIC_Rx_Data_0_Shadow;


   Uint32 MS_SCI_RX_NG_Count;
   Uint32 MS_SCI_RX_OK_Count;
   Uint32 MS_SCI_RX_TOTAL_Count;

   union  SCI_MODULE_STATUS_REG SCI_Module_Status_flag;
   union  SCI_MODULE_CONTROL_REG SCI_Module_Control_flag;

   SCIC_TXRX_STATE_REG SCIC_TxRx_State;

   Uint32 SCIC_TX_Start_Time;


}SCI_MODULE_VARIABLES_REG;


extern SCI_MODULE_VARIABLES_REG SCI_Module_Variables;




//----------------------------------------------------

void Init_SCI_Module_Variables(void);
void Init_SCI_A_GPIO(void);
void Init_SCI_A_Reg(void);

void Init_SCI_C_GPIO(void);
void Init_SCI_C_Reg(void);

void SCI_A_Send_Message(char * msg);
void SCI_A_Transmit(int data);

void scia_TXwrite_char(char *pointer, Uint16 length);

void SCIC_TXRX_Process(void);


//__interrupt void scic_tx_isr(void);
__interrupt void scic_rx_isr(void);

__interrupt void scia_rx_isr(void);


//
// End of file
//
