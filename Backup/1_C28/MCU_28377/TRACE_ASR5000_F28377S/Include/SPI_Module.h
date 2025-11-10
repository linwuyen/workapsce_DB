//###########################################################################
//
// FILE:    SPI_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210825
//###########################################################################


//
// Calculate BRR: 7-bit baud rate register value
// SPI CLK freq = 500 kHz
// LSPCLK freq  = CPU freq / 1  (LSPCLKDIV = 0)
// BRR          = (LSPCLK freq / SPI CLK freq) - 1
//
#if CPU_FRQ_200MHZ
#define SPI_BRR        ((200E6 / 1) / 500E3) - 1
#endif

#if CPU_FRQ_150MHZ
#define SPI_BRR        ((150E6 / 4) / 500E3) - 1
#endif

#if CPU_FRQ_120MHZ
#define SPI_BRR        ((120E6 / 4) / 500E3) - 1
#endif


//----------------------------------------------------

struct  SPI_MODULE_CONTROL_BITS {  // bit
    Uint16 SPIB_RX_Trig:1;                 //00
    Uint16 SPIB_TX_Trig:1;                 //01

};
union SPI_MODULE_CONTROL_REG {
   Uint16                                    all;
   struct SPI_MODULE_CONTROL_BITS   bit;
};



//----------------------------------------------------
typedef struct SPI_MODULE_VARIABLE_REG{

    Uint16 SPIB_Rx_Data[3];
    Uint16 SPIB_Rx_Data_Shadow[3];

    Uint16 SPIB_Tx_Data[3];
    Uint16 SPIB_Tx_Data_Shadow[3];

    union  SPI_MODULE_CONTROL_REG SPI_Module_Control_flag;

}SPI_MODULE_VARIABLE_REG;


extern SPI_MODULE_VARIABLE_REG SPI_Module_Variables;




void Init_SPI_A_Reg(void);
void Init_SPI_A_GPIO(void);
void Init_ADS8353(void);

//void Init_SPI_B_Reg(void);
//void Init_SPI_B_GPIO(void);

void Init_SPI_B_GPIO_for_Master(void);
void Init_SPI_B_GPIO_for_Slave(void);
void Init_SPI_B_Reg_for_Master(void);
void Init_SPI_B_Reg_for_Master_SDRAM_SYNC(void);
void Init_SPI_B_Reg_for_Slave(void);
void Init_SPI_B_Reg_for_Slave_SDRAM_SYNC(void);

void Init_SPI_Module_Variables(void);

__interrupt void spib_rx_isr(void);

//
// End of file
//
