//###########################################################################
//
// FILE:    McBSP_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210913
//###########################################################################


#define SPI_BUFFER_SIZE 10


//----------------------------------------------------
//McBSP_SPI_STATE
typedef enum
{
    SPI_Ready_state,
    SPI_Parsing_state,
    SPI_Reply_state
}McBSP_SPI_STATE_REG;



//----------------------------------------------------
struct  McBSP_MODULE_FLAG_BITS {        // bit    description
   Uint16 Over_Flow:1;  //0
   Uint16 Under_Flow:1; //1
};

union McBSP_MODULE_FLAG_REG {
   Uint16                          all;
   struct McBSP_MODULE_FLAG_BITS   bit;
};




//----------------------------------------------------
typedef struct McBSP_MODULE_VARIABLES_REG{

   McBSP_SPI_STATE_REG McBSP_spi_state;
   Uint16  RXBUFFER[SPI_BUFFER_SIZE];
   Uint16  TXBUFFER[SPI_BUFFER_SIZE];
   union   McBSP_MODULE_FLAG_REG Flags;

}McBSP_MODULE_VARIABLES_REG;




extern volatile McBSP_MODULE_VARIABLES_REG McBSP_Module_Variables;


void Init_McBSP_Module_Variables(void);
void Init_McBSP_B_GPIO(void);
void Init_McBSP_B_Reg(void);

__interrupt void Mcbsp_RxINTB_ISR(void);
void McBSP_SPI_Timer_Process(void);


//
// End of file
//
