//###########################################################################
//
// FILE:    Wave_Generate_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211108
//###########################################################################



#include "Global_VariableDefs.h"





////////////////////////////////////////////////////////////////////
//
// 自行產生squ wave, Local
//
void Generate_Square_wave_data_process_local(void)
{

    Uint32 i;
    int16 int16_buf;

    for ( i = 0; i < 2048; i++ )
    {
        int16_buf = (int16)32767;

        //WAVE_DATA.sdram[i] = int16_buf; //7 cycles
        WAVE_DATA.local_ram[1][i] = int16_buf;
    }

    for ( i = 2048; i < 4096; i++ )
    {
        int16_buf = (int16)(-32767);

        //WAVE_DATA.sdram[i] = int16_buf; //7 cycles
        WAVE_DATA.local_ram[1][i] = int16_buf;
    }

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SQU].all = 1;

    //
    // 改變CF值, 觸發事件
    //
    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

}



////////////////////////////////////////////////////////////////////
//
// 自行產生Tri wave, Local
//
void Generate_Tri_wave_data_process_local(void)
{
    int32 i;
    int16 int16_buf;

    for ( i = 0; i < 1024; i++ )
    {
        int16_buf = (int16)( i * 32 );

        //WAVE_DATA.sdram[i] = int16_buf; //7 cycles
        WAVE_DATA.local_ram[2][i] = int16_buf;
    }

    for ( i = 1024; i < 3072; i++ )
    {
        int16_buf = (int16)( 65535 - i * 32 );

        //WAVE_DATA.sdram[i] = int16_buf; //7 cycles
        WAVE_DATA.local_ram[2][i] = int16_buf;
    }

    for ( i = 3072; i < 4096; i++ )
    {
        int16_buf = (int16)( i * 32 - 131071 );

        //WAVE_DATA.sdram[i] = int16_buf; //7 cycles
        WAVE_DATA.local_ram[2][i] = int16_buf;
    }

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_TRI].all = 1.7320508;

    //
    // 改變CF值, 觸發事件
    //
    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

}



////////////////////////////////////////////////////////////////////
//
// 自行產生sin wave, Local
//
void Generate_Sine_wave_data_process_local(void)
{
    float rad;
    float sin_out;
    Uint32 i;
    int16 int16_buf;

    for ( i = 0; i < 4096; i++ )
    {
        rad = ( (float)i * 2.0 * M_PI ) / 4096.0;
        //sin_out = 0.5 + ( sinf(rad) / 2.0 );
        sin_out = sinf(rad);
        int16_buf = ( int16 )( 32767.0 * sin_out );

        //WAVE_DATA.sdram[i] = int16_buf; //7 cycles
        WAVE_DATA.local_ram[0][i] = int16_buf;

        asm("       NOP");                    // Wait one cycle

    }

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SIN].all = 1.4142136;

    //
    // 改變CF值, 觸發事件
    //
    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

}






















////////////////////////////////////////////////////////////////////
//
// 自行產生Tri wave
//
void Generate_Tri_wave_data_process(void)
{

    int32 i;
    Uint32 page, CheckSum_page;
    int16 int16_buf;

    //載入wave page
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page = WAVE_TRI;
    page = (Uint32)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page << 12;
    CheckSum_page = page + 0x14000;


    for ( i = 0; i < 1024; i++ )
    {
        int16_buf = (int16)( i * 32 );

        *( SDRAM_variables.SDRAMBuf + page + i ) = int16_buf; //7 cycles

        //Checksum
        *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ) = ( int16_buf >> 8 ) + int16_buf; //7 cycles
    }

    for ( i = 1024; i < 3072; i++ )
    {
        int16_buf = (int16)( 65535 - i * 32 );

        *( SDRAM_variables.SDRAMBuf + page + i ) = int16_buf; //7 cycles

        //Checksum
        *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ) = ( int16_buf >> 8 ) + int16_buf; //7 cycles
    }

    for ( i = 3072; i < 4096; i++ )
    {
        int16_buf = (int16)( i * 32 - 131071 );

        *( SDRAM_variables.SDRAMBuf + page + i ) = int16_buf; //7 cycles

        //Checksum
        *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ) = ( int16_buf >> 8 ) + int16_buf; //7 cycles

    }

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_TRI].all = 1.7320508;

    //
    // 改變CF值, 觸發事件
    //
    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

}



////////////////////////////////////////////////////////////////////
//
// 自行產生squ wave
//
void Generate_Square_wave_data_process(void)
{

    Uint32 i;
    Uint32 page, CheckSum_page;
    int16 int16_buf;
    Uint32 duty_set;

    //載入wave page
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page = WAVE_SQU;
    page = (Uint32)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page << 12;
    CheckSum_page = page + 0x14000;

    //
    // 載入SQU Duty以及鉗制處理
    //
    duty_set = (Uint32)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.SQU_Duty_set_user;
    if ( duty_set > 4095 )
    {
        duty_set = 4095;
    }



    for ( i = 0; i < duty_set; i++ )
    {
        int16_buf = (int16)32767;

        *( SDRAM_variables.SDRAMBuf + page + i ) = int16_buf; //7 cycles

        //Checksum
        *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ) = ( int16_buf >> 8 ) + int16_buf; //7 cycles

    }

    for ( i = duty_set; i < 4096; i++ )
    {
        int16_buf = (int16)(-32767);

        *( SDRAM_variables.SDRAMBuf + page + i ) = (int16)(-32767); //7 cycles

        //Checksum
        *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ) = ( int16_buf >> 8 ) + int16_buf; //7 cycles

    }

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SQU].all = 1;

    //
    // 改變CF值, 觸發事件
    //
    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

}



////////////////////////////////////////////////////////////////////
//
// 自行產生sin wave
//
void Generate_Sine_wave_data_process(void)
{
    float rad;
    float sin_out;
    Uint32 i;
    Uint32 page, CheckSum_page;
    int16 int16_buf;

    //載入wave page
    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page = WAVE_SIN; //sin
    page = (Uint32)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_data_page << 12;
    CheckSum_page = page + 0x14000;


    for ( i = 0; i < 4096; i++ )
    {
        rad = ( (float)i * 2.0 * M_PI ) / 4096.0;
        //sin_out = 0.5 + ( sinf(rad) / 2.0 );
        sin_out = sinf(rad);
        int16_buf = ( int16 )( 32767.0 * sin_out );

        *( SDRAM_variables.SDRAMBuf + page + i ) = int16_buf; //7 cycles

        //Checksum
        *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ) = ( int16_buf >> 8 ) + int16_buf; //7 cycles


        asm("       NOP");                    // Wait one cycle

        //WAVE_DATA.sdram[tmp02] = rx_buffer[1];
        //定址超過所以採用以下寫法加快速度
    }

    Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Wave_CF_set_user[WAVE_SIN].all = 1.4142136;

    //
    // 改變CF值, 觸發事件
    //
    Event_Module.Event_Flag.bit.Reload_Wave_CF_set = 1;

}



//
// End of file
//
