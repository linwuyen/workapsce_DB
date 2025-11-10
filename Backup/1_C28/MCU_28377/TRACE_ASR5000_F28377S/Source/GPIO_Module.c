//###########################################################################
//
// FILE:    GPIO_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210903
//###########################################################################

#include "Global_VariableDefs.h"




/////////////////////////////////////////////////////////////////////
//
// 讀取MCP3301 AD資料
// 回傳 16bit => 無效3bit + Sign bit + 12 bit
//             x x x SB B11 B10 ... B0
//
// 注意：MCP3301每次間隔需>580ns
//
Uint16 Read_MCP3301_Sequence(void)
{
    Uint16 bit_count;
    Uint16 Read_buffer;

    bit_count = 16;     //接收16個bit

    /*----------------------------------------*/

    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | ENABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    //CS Fall To First Rising CLK Edge > 100ns
    DELAY_100ns;

    /*----------------------------------------*/
    //Read adc value

    Read_buffer = 0;

    while (bit_count)
    {
        CTRL_CLK_OUTPUT_L;

        DELAY_300ns;
        //Clock low time > 275ns
        //但是Clock frequency < 1.7MHz
        //所以取300ns

        Read_buffer = ( Read_buffer << 1 ) + CTRL_SDO_INPUT;

        CTRL_CLK_OUTPUT_H;

        DELAY_300ns;
        //Clock low time > 275ns
        //但是Clock frequency < 1.7MHz
        //=> 取300ns

        bit_count = bit_count - 1;
    }

    /*----------------------------------------*/

    CTRL_CLK_OUTPUT_L;

    /*----------------------------------------*/
    //CS = 0
    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    if (Read_buffer & 0x01000)
    {
        Read_buffer = 0;
    }
    else
    {
        Read_buffer = Read_buffer & 0x0FFF;
    }

    return Read_buffer;
}


/////////////////////////////////////////////////////////////////////
//
// 設定DAC7612輸出時序
// ch: DAC7612_CHA或DAC7612_CHB或DAC7612_CHAB
//     DAC7612_CHAB表示為DACA及DACB都同時載入
// da_value: 12bit DA值
//
// Example: Write_DAC7612_Sequence( DAC7612_CHAB, 0x078A )
//
void Write_DAC7612_Sequence(Uint16 ch, Uint16 da_value)
{
    Uint16 bit_scan;
    Uint16 data;

    //以免ch誤寫
    if (ch > 0x03)
    {
        ch = DAC7612_CHAB;
    }

    data = ( ch << 12 ) + ( da_value & 0x0FFF );

    data = data >> 1;
    //20210924 實際測試需要右移1bit,動作才會正常

    bit_scan = 0x2000;

    /*----------------------------------------*/

    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | ENABLE_DA1CS );

    DAC7612_CS_SETUP_TIME;
    //Select time > 30ns


    /*----------------------------------------*/
    //Scan and send data

    while (bit_scan)
    {
        if (data & bit_scan)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        DAC7612_CLK_L_TIME;
        //Clock low time > 30ns

        CTRL_CLK_OUTPUT_H;

        DAC7612_CLK_H_TIME;
        //Clock high time > 30ns

        bit_scan = bit_scan >> 1;
    }


    /*----------------------------------------*/
    CTRL_SDI_OUTPUT_L;
    CTRL_CLK_OUTPUT_L;

    /*----------------------------------------*/
    //Disable DAC7612

    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    /*----------------------------------------*/
    //Load Register

    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | ENABLE_DA1LOAD
                                     | DISABLE_DA1CS );
    DAC7612_LOAD_WIDTH_TIME;
    //Load pulse width > 20ns

    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

}



/////////////////////////////////////////////////////////////////////
//
// 讀取74HCT165輸出時序
//
Uint16 Read_74HCT165_Sequence(void)
{
    Uint16 bit_count;
    Uint16 Read_buffer;


    /*----------------------------------------*/
    //Load to Register

    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | ENABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    DELAY_30ns;
    // PL pulse width > 30ns

    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    DELAY_30ns;
    // recovery come > 30ns

    /*----------------------------------------*/
    //Enable ERR CS
    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | ENABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );


    /*----------------------------------------*/
    //data

    Read_buffer = 0;
    bit_count = 8;

    while (bit_count)
    {
        CTRL_CLK_OUTPUT_L;

        Read_buffer = ( Read_buffer << 1 ) + ERR_INPUT;

        DELAY_25ns;
        //pulse width > 24ns

        CTRL_CLK_OUTPUT_H;

        DELAY_50ns;
        //progation delay time < 50ns


        bit_count = bit_count - 1;
    }

    CTRL_CLK_OUTPUT_L;


    /*----------------------------------------*/
    //Disable ERR CS
    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );



    return Read_buffer;
}



/////////////////////////////////////////////////////////////////////
//
// 執行EEPROM Read時序
// addr: 10bit有效
//
Uint16 EEPROM_READ_Sequence(Uint16 addr)
{
    Uint16 bit_scan1;
    Uint16 bit_scan2;
    Uint16 bit_count;
    Uint16 data1;
    Uint16 data2;
    Uint16 Read_buffer;

    bit_scan1 = 0x04;   //Scan 3bit, Left to right
    data1 = 0x06;       //1(SB) + 10(OP)
    bit_scan2 = 0x0200; //Scan 10bit, Left to right
    data2 = addr;       //address 10bits
    bit_count = 16;     //接收16個bit


    /*----------------------------------------*/

    //93C86 CS = 1
    CTRL_Setup_4094_Output_Sequence(   ENABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    //CS setup time > 50ns
    EEPROM_CS_SETUP_TIME;

    /*----------------------------------------*/
    //SB + Opcode

    while (bit_scan1)
    {
        if (data1 & bit_scan1)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        EEPROM_CLK_L_TIME;
        //Clock low time > 200ns

        CTRL_CLK_OUTPUT_H;

        EEPROM_CLK_H_TIME;
        //Clock high time > 300ns
        //包含while執行cycle,所以先設定200ns

        bit_scan1 = bit_scan1 >> 1;
    }

    /*----------------------------------------*/
    //Address

    while (bit_scan2)
    {
        if (data2 & bit_scan2)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        EEPROM_CLK_L_TIME;
        //Clock low time > 200ns

        CTRL_CLK_OUTPUT_H;

        EEPROM_CLK_H_TIME;
        //Clock high time > 300ns
        //包含while執行cycle,所以先設定200ns

        bit_scan2 = bit_scan2 >> 1;
    }

    CTRL_SDI_OUTPUT_L;

    /*----------------------------------------*/
    //data

    Read_buffer = 0;

    while (bit_count)
    {
        CTRL_CLK_OUTPUT_L;

        EEPROM_CLK_L_TIME;
        //Clock low time > 200ns

        CTRL_CLK_OUTPUT_H;

        EEPROM_CLK_H_TIME;
        //Data output delay time < 400ns

        Read_buffer = ( Read_buffer << 1 ) + CTRL_SDO_INPUT;

        bit_count = bit_count - 1;
    }

    /*----------------------------------------*/

    CTRL_CLK_OUTPUT_L;

    /*----------------------------------------*/
    //Set 93C86 CS = 0
    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    return Read_buffer;
}


/////////////////////////////////////////////////////////////////////
//
// EEPROM Busy檢查 only for WRITE, ERASE, ERAL
// 回傳: Over time (1) / PASS (0)
// 參數: check_period(us)檢查週期, over_time_value over time設定值
//
// example: err = EEPROM_BUSY_Check_Sequence(100, 10000)
//          每100us檢查一次, 檢查次數>10000 (1sec), 則回傳over time, err=1
//
Uint16 EEPROM_BUSY_Check_Sequence(Uint16 check_period, Uint16 over_time_value)
{
    Uint16 tmp;
    Uint16 over_time_counter;

    tmp = 0x01;
    over_time_counter = over_time_value;

    /*----------------------------------------*/
    CTRL_Setup_4094_Output_Sequence(   ENABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    while (tmp)
    {
        tmp = !( 0x01 & CTRL_SDO_INPUT ); //get SDO

        DELAY_US( check_period );

        over_time_counter = over_time_counter - 1;

        if ( over_time_counter == 0 )
        {
            //Set 93C86 CS = 0
            CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                             | DISABLE_ERRLOAD
                                             | DISABLE_ERRCS
                                             | DISABLE_AD1CS
                                             | DISABLE_DA1LOAD
                                             | DISABLE_DA1CS );

            return 1; //Busy over time
        }
    }

    /*----------------------------------------*/
    //Set 93C86 CS = 0
    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    return 0; //No Error

}



/////////////////////////////////////////////////////////////////////
//
// 執行EEPROM Write時序
// addr: 10bit有效
// data: 16bit
//
// example: EEPROM_Write_Sequence(0x0198, 0x5AA5)
//
void EEPROM_WRITE_Sequence(Uint16 addr, Uint16 data)
{
    Uint16 bit_scan1;
    Uint16 bit_scan2;
    Uint16 bit_scan3;
    Uint16 data1;
    Uint16 data2;
    Uint16 data3;

    bit_scan1 = 0x04;   //Scan 3bit, Left to right
    data1 = 0x05;       //1(SB) + 01(OP)
    bit_scan2 = 0x0200; //Scan 10bit, Left to right
    data2 = addr;       //address 10bits
    bit_scan3 = 0x8000; //Scan 16bit, Left to right
    data3 = data;       //address 10bits


    /*----------------------------------------*/

    //93C86 CS = 1
    CTRL_Setup_4094_Output_Sequence(   ENABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    //CS setup time > 50ns
    EEPROM_CS_SETUP_TIME;

    /*----------------------------------------*/
    //SB + Opcode

    while (bit_scan1)
    {
        if (data1 & bit_scan1)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        EEPROM_CLK_L_TIME;
        //Clock low time > 200ns

        CTRL_CLK_OUTPUT_H;

        EEPROM_CLK_H_TIME;
        //Clock high time > 300ns
        //包含while執行cycle,所以先設定200ns

        bit_scan1 = bit_scan1 >> 1;
    }

    /*----------------------------------------*/
    //Address

    while (bit_scan2)
    {
        if (data2 & bit_scan2)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        EEPROM_CLK_L_TIME;
        //Clock low time > 200ns

        CTRL_CLK_OUTPUT_H;

        EEPROM_CLK_H_TIME;
        //Clock high time > 300ns
        //包含while執行cycle,所以先設定200ns

        bit_scan2 = bit_scan2 >> 1;
    }

    /*----------------------------------------*/
    //data

    while (bit_scan3)
    {
        if (data3 & bit_scan3)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        EEPROM_CLK_L_TIME;
        //Clock low time > 200ns

        CTRL_CLK_OUTPUT_H;

        EEPROM_CLK_H_TIME;
        //Clock high time > 300ns
        //包含while執行cycle,所以先設定200ns

        bit_scan3 = bit_scan3 >> 1;
    }

    /*----------------------------------------*/
    CTRL_SDI_OUTPUT_L;

    EEPROM_CLK_H_TIME;
    //Clock high time > 300ns

    CTRL_CLK_OUTPUT_L;

    /*----------------------------------------*/
    //Set 93C86 CS = 0
    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

}


/////////////////////////////////////////////////////////////////////
//
// 執行EEPROM命令時序
// cmd: EEPROM_EWEN, EEPROM_EWDS, EEPROM_ERASE, EEPROM_ERAL
// addr: 10bit有效
//       只有EEPROM_ERASE需要設定addr, 其餘cmd addr = 0
//
// example: tmp = EEPROM_Control_Sequence(EEPROM_EWEN, 0)
//          tmp = EEPROM_Control_Sequence(EEPROM_ERASE, 0x8F)
//
void EEPROM_Control_Sequence(Uint16 cmd, Uint16 addr)
{
    Uint16 bit_scan1;
    Uint16 bit_scan2;
    Uint16 data1;
    Uint16 data2;

    switch(cmd)
    {
        case EEPROM_EWEN:
        {
            bit_scan1 = 0x04;   //Scan 3bit, Left to right
            data1 = 0x04;       //1(SB) + 00(OP)
            bit_scan2 = 0x0200; //Scan 10bit, Left to right
            data2 = 0x0300;     //11 + XXXX XXXX
            break;
        }
        case EEPROM_EWDS:
        {
            bit_scan1 = 0x04;   //Scan 3bit, Left to right
            data1 = 0x04;       //1(SB) + 00(OP)
            bit_scan2 = 0x0200; //Scan 10bit, Left to right
            data2 = 0x0000;     //00 + XXXX XXXX
            break;
        }
        case EEPROM_ERASE:
        {
            bit_scan1 = 0x04;   //Scan 3bit, Left to right
            data1 = 0x07;       //1(SB) + 11(OP)
            bit_scan2 = 0x0200; //Scan 10bit, Left to right
            data2 = addr;       //address 10bit
            break;
        }
        case EEPROM_ERAL:
        {
            bit_scan1 = 0x04;   //Scan 3bit, Left to right
            data1 = 0x04;       //1(SB) + 00(OP)
            bit_scan2 = 0x0200; //Scan 10bit, Left to right
            data2 = 0x0200;     //10 + XXXX XXXX
            break;
        }
    }

    /*----------------------------------------*/

    //93C86 CS = 1
    CTRL_Setup_4094_Output_Sequence(   ENABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

    //CS setup time > 50ns
    EEPROM_CS_SETUP_TIME;

    /*----------------------------------------*/
    //SB + Opcode

    while (bit_scan1)
    {
        if (data1 & bit_scan1)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        EEPROM_CLK_L_TIME;
        //Clock low time > 200ns

        CTRL_CLK_OUTPUT_H;

        EEPROM_CLK_H_TIME;
        //Clock high time > 300ns
        //包含while執行cycle,所以先設定200ns

        bit_scan1 = bit_scan1 >> 1;
    }

    /*----------------------------------------*/
    //Address

    while (bit_scan2)
    {
        if (data2 & bit_scan2)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        EEPROM_CLK_L_TIME;
        //Clock low time > 200ns

        CTRL_CLK_OUTPUT_H;

        EEPROM_CLK_H_TIME;
        //Clock high time > 300ns
        //包含while執行cycle,所以先設定200ns

        bit_scan2 = bit_scan2 >> 1;
    }

    /*----------------------------------------*/
    CTRL_SDI_OUTPUT_L;

    EEPROM_CLK_H_TIME;
    //Clock high time > 300ns

    CTRL_CLK_OUTPUT_L;


    /*----------------------------------------*/
    //Set 93C86 CS = 0
    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

}





/////////////////////////////////////////////////////////////////////
//
// data指定4094輸出pin H/L
// data: ROM1CS, ERRLOAD, ERRCS, AD1CS, DA1LOAD, DA1CS
// Example: CTRL_Setup_4094_Output_Sequence( ROM1CS | DA1CS );
//
void CTRL_Setup_4094_Output_Sequence(Uint16 data)
{
    Uint16 bit_scan;


    /*----------------------------------------*/
    CTRL_CS_OUTPUT_H;

    DELAY_50ns;

    /*----------------------------------------*/
    CTRL_CS_OUTPUT_L;

    DELAY_50ns;
    //STR Setup time > 30nd

    /*----------------------------------------*/
    bit_scan = 0x80; //Scan 8bit, Left to right

    while (bit_scan)
    {
        if (data & bit_scan)
        {
            CTRL_SDI_OUTPUT_H;
        }
        else
        {
            CTRL_SDI_OUTPUT_L;
        }

        CTRL_CLK_OUTPUT_L;

        DELAY_30ns;
        //CP Pulse Width > 24ns

        CTRL_CLK_OUTPUT_H;

        DELAY_5ns;
        //CP Pulse Width > 24ns
        //實測while程式實際執行cycle已經大於24ns

        bit_scan = bit_scan >> 1;
    }

    /*----------------------------------------*/
    CTRL_SDI_OUTPUT_L;

    DELAY_30ns;
    //CP Pulse Width > 24ns

    CTRL_CLK_OUTPUT_L;

    DELAY_50ns;

    CTRL_CS_OUTPUT_H;
}




/////////////////////////////////////////////////////////////////////
//
//
void Init_VREF_4094_Sequence(void)
{
    Uint16 bit_scan;
    Uint16 data = 0;

    /*----------------------------------------*/
    //VREF_CS_OUTPUT_H;
    //DELAY_50ns;

    /*----------------------------------------*/
    VREF_CS_OUTPUT_L;

    DELAY_50ns;
    //STR Setup time > 30nd

    /*----------------------------------------*/
    bit_scan = 0x80; //Scan 8bit, Left to right

    while (bit_scan)
    {
        if (data & bit_scan)
        {
            VREF_SDI_OUTPUT_H;
        }
        else
        {
            VREF_SDI_OUTPUT_L;
        }

        VREF_CLK_OUTPUT_L;

        DELAY_30ns;
        //CP Pulse Width > 24ns

        VREF_CLK_OUTPUT_H;

        DELAY_5ns;
        //CP Pulse Width > 24ns
        //實測while程式實際執行cycle已經大於24ns

        bit_scan = bit_scan >> 1;
    }

    /*----------------------------------------*/
    VREF_SDI_OUTPUT_L;

    DELAY_30ns;
    //CP Pulse Width > 24ns

    VREF_CLK_OUTPUT_L;

    DELAY_50ns;

    VREF_CS_OUTPUT_H;
}












/////////////////////////////////////////////////////////////////////
void Init_GPIO_Module(void)
{
    GPIO_SetupPinOptions(94, GPIO_OUTPUT, GPIO_PUSHPULL);   //LED

    //
    // 外部並聯
    //
    GPIO_SetupPinOptions(53, GPIO_OUTPUT, GPIO_PUSHPULL);   //PAR_ENA
    GPIO_SetupPinOptions(54, GPIO_OUTPUT, GPIO_PUSHPULL);   //~SLAVE_ENA
    GPIO_SetupPinOptions(61, GPIO_INPUT, GPIO_ASYNC);       //SLAVE_ENA


    /*--------------------------------------------*/
    // Assign to CLA
    EALLOW;
    //GpioCtrlRegs.GPCCSEL4.bit.GPIO94=1;
    EDIS;

    /*--------------------------------------------*/
    GPIO_SetupPinOptions(4, GPIO_OUTPUT, GPIO_PUSHPULL);    //VMONI_LO
    GPIO_SetupPinOptions(6, GPIO_OUTPUT, GPIO_PUSHPULL);    //IMONI_LO

    GPIO_SetupPinOptions(28, GPIO_OUTPUT, GPIO_PUSHPULL);    //IOUPG
                                                             //Self Reset!!

    GPIO_SetupPinOptions(1, GPIO_OUTPUT, GPIO_PUSHPULL);    //CTRLCS
    GPIO_SetupPinOptions(3, GPIO_OUTPUT, GPIO_PUSHPULL);    //CTRLCLK
    GPIO_SetupPinOptions(5, GPIO_OUTPUT, GPIO_PUSHPULL);    //CTRLSDI
    GPIO_SetupPinOptions(7, GPIO_INPUT, GPIO_ASYNC);       //CTRLSDO
    //GPIO_SetupPinOptions(99, GPIO_INPUT, GPIO_ASYNC);      //ERR

    //
    // 與SPISTEB短接, 沒使用下設定為Input
    //
    GPIO_SetupPinOptions(57, GPIO_INPUT, GPIO_ASYNC);

    //
    // 與VREFCS短接, 沒使用下設定為Input
    //
    GPIO_SetupPinOptions(20, GPIO_INPUT, GPIO_ASYNC);


    /*--------------------------------------------*/
    //Initial CTRL

    CTRL_CLK_OUTPUT_L;
    CTRL_SDI_OUTPUT_L;

    CTRL_Setup_4094_Output_Sequence(   DISABLE_ROM1CS
                                     | DISABLE_ERRLOAD
                                     | DISABLE_ERRCS
                                     | DISABLE_AD1CS
                                     | DISABLE_DA1LOAD
                                     | DISABLE_DA1CS );

}



//
// End of file
//
