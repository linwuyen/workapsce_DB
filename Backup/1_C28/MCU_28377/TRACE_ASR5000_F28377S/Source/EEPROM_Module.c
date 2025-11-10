//###########################################################################
//
// FILE:    EEPROM_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210916
//###########################################################################

#include "Global_VariableDefs.h"


volatile EEPROM_MODULE_VARIABLES_REG EEPROM_Module_Variables;



/////////////////////////////////////////////////////////////////////
//
// 變數初始化
//
void Init_EEROM_Module_Variables(void)
{
    EEPROM_Module_Variables.Read_buffer.all = 0;
    EEPROM_Module_Variables.Write_buffer.all = 0;
}





////////////////////////////////////////////////////////////////////
//
// CRC16計算
// length是以8bit為單位的長度計算
//
Uint16 CRC16_Calc(Uint16 *pointer, Uint16 length)
{
    Uint16 i, j;
    Uint16 reg_crc = 0xFFFF;

    for ( i = 0; i < length; i++ )
    {
        reg_crc ^= ( *pointer & 0xFF );

        for ( j = 0; j < 8; j ++ )
        {
            if ( reg_crc & 0x01 )
            {
                reg_crc = ( reg_crc >> 1 ) ^ 0xA001;
            }
            else
            {
                reg_crc = reg_crc >> 1;
            }
        }

        pointer ++;
    }

    return reg_crc;
}





////////////////////////////////////////////////////////////////////
//
//
//
void Read_EEPROM_addr_to_Buffer( Uint16 addr )
{
    Uint16 checksum1, checksum2;
    Uint16 msb, lsb;

    /*---------------------------------------------------------------*/
    msb = EEPROM_READ_Sequence( addr );
    lsb = EEPROM_READ_Sequence( addr + 1 );
    checksum1 = msb + lsb;
    checksum2 = EEPROM_READ_Sequence( addr + 2 );

    if ( checksum1 != checksum2 )
    {
        Global_Variables.POWER_FUNCTIONS_ERROR_FLAGS.bit.Calibration_Error_SW = 1;
        Global_Variables.POWER_FUNCTIONS_ERROR_CODE = EEPROM_READ_CHECKSUM_NG;

        //
        // 轉態
        //
        Main_State_Module.Machine_state = ERROR_HANDLE_STATE0;


        error_start_time_reset();

        //debug msg
        #ifdef DEBUG_ON
        sprintf(SCI_Module_Variables.Debug_Msg, "CPU_Timer2: %lu ms > ***Calibration_Error_SW***\n", CpuTimer2.InterruptCount ); //9530 cycles
        scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
        #endif

        EEPROM_Module_Variables.Read_buffer.MSB = 0;
        EEPROM_Module_Variables.Read_buffer.LSB = 0;

        ACFAIL_start_time_reset();

        return;
    }
    else
    {
        EEPROM_Module_Variables.Read_buffer.MSB = msb;
        EEPROM_Module_Variables.Read_buffer.LSB = lsb;

        ACFAIL_start_time_reset();
    }

}


////////////////////////////////////////////////////////////////////
//
//
//
Uint16 Write_Buffer_to_EEPROM_addr( Uint16 addr )
{
    Uint16 checksum;
    Uint16 msb, lsb;
    Uint16 err_count;

    err_count = 0;

    /*----------------------------------------------------------------------*/
    msb = EEPROM_Module_Variables.Write_buffer.MSB;
    lsb = EEPROM_Module_Variables.Write_buffer.LSB;
    checksum = msb + lsb;

    EEPROM_WRITE_Sequence( addr, msb );
    err_count = err_count + EEPROM_BUSY_Check_Sequence(EEPROM_CHECK_PERIOD, EEPROM_OVER_TIME_VALUE);

    EEPROM_WRITE_Sequence( addr + 1, lsb );
    err_count = err_count + EEPROM_BUSY_Check_Sequence(EEPROM_CHECK_PERIOD, EEPROM_OVER_TIME_VALUE);

    EEPROM_WRITE_Sequence( addr + 2, checksum );
    err_count = err_count + EEPROM_BUSY_Check_Sequence(EEPROM_CHECK_PERIOD, EEPROM_OVER_TIME_VALUE);

    ACFAIL_start_time_reset();

    return err_count;
}




//
// End of file
//
