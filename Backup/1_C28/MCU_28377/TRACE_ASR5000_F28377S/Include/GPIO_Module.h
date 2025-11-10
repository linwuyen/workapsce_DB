//###########################################################################
//
// FILE:    GPIO_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210903
//###########################################################################


#define CTRL_CS_OUTPUT_H   {GpioDataRegs.GPASET.bit.GPIO1 = 1;}
#define CTRL_CS_OUTPUT_L   {GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;}
#define CTRL_CLK_OUTPUT_H  {GpioDataRegs.GPASET.bit.GPIO3 = 1;}
#define CTRL_CLK_OUTPUT_L  {GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;}
#define CTRL_SDI_OUTPUT_H  {GpioDataRegs.GPASET.bit.GPIO5 = 1;}
#define CTRL_SDI_OUTPUT_L  {GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;}
#define CTRL_SDO_INPUT     (GpioDataRegs.GPADAT.bit.GPIO7)
#define ERR_INPUT          (GpioDataRegs.GPDDAT.bit.GPIO99)


#define VREF_CS_OUTPUT_H   {GpioDataRegs.GPASET.bit.GPIO19 = 1;}
#define VREF_CS_OUTPUT_L   {GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;}
#define VREF_CLK_OUTPUT_H  {GpioDataRegs.GPASET.bit.GPIO18 = 1;}
#define VREF_CLK_OUTPUT_L  {GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;}
#define VREF_SDI_OUTPUT_H  {GpioDataRegs.GPASET.bit.GPIO16 = 1;}
#define VREF_SDI_OUTPUT_L  {GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;}

//
// RS485 Direction Control
//
#define TXC_RXC_0          {GpioDataRegs.GPCCLEAR.bit.GPIO66 = 1;}  // Receive
#define TXC_RXC_1          {GpioDataRegs.GPCSET.bit.GPIO66 = 1;}     // Transmit
#define PAR_ENA_0          {GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;}
#define PAR_ENA_1          {GpioDataRegs.GPBSET.bit.GPIO53 = 1;}

#define n_SLAVE_ENA_0      {GpioDataRegs.GPBCLEAR.bit.GPIO54 = 1;}
#define n_SLAVE_ENA_1      {GpioDataRegs.GPBSET.bit.GPIO54 = 1;}


//200MHz => 5ns/step
#define DELAY_5ns       asm(" RPT #1  || NOP")
#define DELAY_20ns      asm(" RPT #4  || NOP")
#define DELAY_25ns      asm(" RPT #5  || NOP")
#define DELAY_30ns      asm(" RPT #6  || NOP")
#define DELAY_50ns      asm(" RPT #10 || NOP")
#define DELAY_100ns     asm(" RPT #20 || NOP")
#define DELAY_200ns     asm(" RPT #40 || NOP")
#define DELAY_250ns     asm(" RPT #50 || NOP")
#define DELAY_300ns     asm(" RPT #60 || NOP")
#define DELAY_400ns     asm(" RPT #80 || NOP")


//CTRL_Setup_4094_Output_Sequence
#define ENABLE_ROM1CS  0x01
#define ENABLE_ERRLOAD 0x00
#define ENABLE_ERRCS   0x00
#define ENABLE_AD1CS   0x00
#define ENABLE_DA1LOAD 0x00
#define ENABLE_DA1CS   0x00
#define DISABLE_ROM1CS  0x00
#define DISABLE_ERRLOAD 0x02
#define DISABLE_ERRCS   0x04
#define DISABLE_AD1CS   0x08
#define DISABLE_DA1LOAD 0x10
#define DISABLE_DA1CS   0x20


//EEPROM_Control_Sequence
#define EEPROM_EWEN     0x01
#define EEPROM_EWDS     0x02
#define EEPROM_ERASE    0x03
#define EEPROM_ERAL     0x04

#define EEPROM_CS_SETUP_TIME asm(" RPT #10 || NOP")
#define EEPROM_CLK_H_TIME asm(" RPT #60 || NOP")
#define EEPROM_CLK_L_TIME asm(" RPT #60 || NOP")



//Write_DAC7612_Sequence
#define DAC7612_CHA  0x02
#define DAC7612_CHB  0x03
#define DAC7612_CHAB 0x00
#define DAC7612_CHX  0x01


#define DAC7612_CS_SETUP_TIME asm(" RPT #7 || NOP")
#define DAC7612_CLK_H_TIME asm(" RPT #7 || NOP")
#define DAC7612_CLK_L_TIME asm(" RPT #7 || NOP")
#define DAC7612_LOAD_WIDTH_TIME asm(" RPT #5 || NOP")








void Init_GPIO_Module(void);

void CTRL_Setup_4094_Output_Sequence(Uint16 data);
void Init_VREF_4094_Sequence(void);

void EEPROM_Control_Sequence(Uint16 cmd, Uint16 addr);
void EEPROM_WRITE_Sequence(Uint16 addr, Uint16 data);
Uint16 EEPROM_BUSY_Check_Sequence(Uint16 check_period, Uint16 over_time_value);
Uint16 EEPROM_READ_Sequence(Uint16 addr);

Uint16 Read_74HCT165_Sequence(void);

void Write_DAC7612_Sequence(Uint16 ch, Uint16 da_value);

Uint16 Read_MCP3301_Sequence(void);


//
// End of file
//
