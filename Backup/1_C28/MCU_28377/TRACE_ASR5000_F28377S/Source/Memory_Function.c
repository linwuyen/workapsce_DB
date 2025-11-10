//###########################################################################
//
// FILE:    Memory_Function.c
//
// TITLE:   Memory read, write, clear... support function.
//
//###########################################################################
// $Create Date: 20210819
//###########################################################################

#include "Global_VariableDefs.h"

#pragma DATA_SECTION(LocalRam_variables, "ramgs0");
volatile LOCALRAM_VARS LocalRam_variables;

#pragma DATA_SECTION(V_RAW, "ramgs1");
volatile V_RAW_SECTION V_RAW;

#pragma DATA_SECTION(I_RAW, "ramgs2");
volatile I_RAW_SECTION I_RAW;

//#pragma DATA_SECTION(V_RAW_Shadow, "ramgs3");
//volatile V_RAW_SECTION V_RAW_Shadow;

//#pragma DATA_SECTION(I_RAW_Shadow, "ramgs4");
//volatile I_RAW_SECTION I_RAW_Shadow;


#pragma DATA_SECTION(WAVE_DATA, "ramgs5_to_10");
volatile WAVE_DATA_SECTION WAVE_DATA;


#pragma DATA_SECTION(SEQ_Step_Section1, "ramgs3");
volatile SEQ_STEP_SECTION1 SEQ_Step_Section1;

#pragma DATA_SECTION(SEQ_Step_Section2, "ramgs4");
volatile SEQ_STEP_SECTION2 SEQ_Step_Section2;


#pragma DATA_SECTION(SEQ_Step_Section3, "ramgs11");
volatile SEQ_STEP_SECTION3 SEQ_Step_Section3;

#pragma DATA_SECTION(SEQ_Step_Section4, "ramgs12");
volatile SEQ_STEP_SECTION4 SEQ_Step_Section4;

#pragma DATA_SECTION(SEQ_Step_Section5, "ramgs13");
volatile SEQ_STEP_SECTION5 SEQ_Step_Section5;


__attribute__((far)) volatile SDRAM_VARS SDRAM_variables;


MEMORY_MODULE_VARIABLES_REG Memory_Module;




/////////////////////////////////////////////////////////////////////
//
// 模組初始化
//
void Init_Memory_Module_Variables(void)
{

    Memory_Module.SDRAM_Error_Count = 0;

    Memory_Module.wave_buffer_section = 0;

    Memory_Module.wave_buffer_section_3 = 0;
    Memory_Module.wave_buffer_section_4 = 0;
}








/////////////////////////////////////////////////////////////////////
void Local_RAM_and_SDRAM_Clear(void)
{
    Uint32 i, mem_size;
    Uint32 sdram_size_div_mem_size;

    i = 0;
    mem_size = 0x1000; //4k
    sdram_size_div_mem_size = SDRAM_BUFFER_SIZE / mem_size;

    //
    // Clear Local ram memory
    //
    local_memory_reset( LocalRam_variables.LocalRAMBuf, LOCAL_RAM_BUFFER_SIZE);

    //
    // Clear V Raw, I Raw memory
    //
    local_memory_reset( V_RAW.local_ram, LOCAL_RAM_BUFFER_SIZE);
    local_memory_reset( I_RAW.local_ram, LOCAL_RAM_BUFFER_SIZE);

    //
    // Clear SDRAM memory
    //
    for ( i = 0; i < sdram_size_div_mem_size; i++ )
    {
        memcpy_fast_far( SDRAM_variables.SDRAMBuf + ( i * mem_size ),
                         LocalRam_variables.LocalRAMBuf, mem_size );
    }

    //
    // Clear SEQ Step Memory
    //

    for ( i = 0; i < SEQ_STEP_MAX; i ++ )
    {
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].all = 0;
        SEQ_Step_Section1.SEQ_Config_State2_set_user[i].all = 0;

        SEQ_Step_Section1.SEQ_Config_State2_set_user[i].bit.WAVE_Form = WAVE_SIN;
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].bit.ACV_Const = 1;
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].bit.DCV_Const = 1;
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].bit.FREQ_Const = 1;
        SEQ_Step_Section1.SEQ_Config_State1_set_user[i].bit.Step_Continuous = 1;

        SEQ_Step_Section2.SEQ_Branch1_set_user[i] = 0;
        SEQ_Step_Section2.SEQ_Branch2_set_user[i] = 0;

        SEQ_Step_Section2.SEQ_Jump_set_user[i] = 0;
        SEQ_Step_Section2.SEQ_Jump_Count_set_user[i] = 1;

        SEQ_Step_Section1.SEQ_Start_Phase_set_user[i] = 0;
        SEQ_Step_Section1.SEQ_Stop_Phase_set_user[i] = 0;

        SEQ_Step_Section3.SEQ_Freq_set_user[i].all = 5000;
        SEQ_Step_Section4.SEQ_DCV_set_user[i].all = 0;
        SEQ_Step_Section4.SEQ_ACV_set_user[i].all = 0;

        SEQ_Step_Section5.SEQ_SQU_Duty_set_user[i] = 2048;

        SEQ_Step_Section3.SEQ_Step_Time_set_user[i].all = 1000;
    }

}



////////////////////////////////////////////////////////////////////
//
// SDRAM ARB1~TRI Section test
//
Uint16 SDRAM_Self_Test(void)
{
    Uint16 data;
    Uint32 i, j;
    Uint32 mem_size;
    Uint32 wave_total_page;
    Uint16 Test_Result_NG = 0;


    Memory_Module.SDRAM_Error_Count = 0;

    i = 0;
    j = 0;
    mem_size = LOCAL_RAM_BUFFER_SIZE; //4k
    wave_total_page = 19;




    /*-------------------------------------------*/
    //
    // For 重複開機SDRAM Check NG的問題而加入
    // 實測先針對section 1 檢查一遍, 之後就OK了
    //
    i = 0;

    /*--- Local RAM 先塞滿測試的資料 ---*/
    data = 0xA5A5;

    for ( j = 0; j < mem_size; j++) //114687 cycles
    {
        LocalRam_variables.LocalRAMBuf[j] = data;
        data = data + 1;
    }

    /*--- 利用RAM一次寫入4k速度比較快也節省時間 ---*/
    memcpy_fast_far( SDRAM_variables.SDRAMBuf + ( i * mem_size ),
                     LocalRam_variables.LocalRAMBuf, mem_size ); //16551 cycles

    /*--- 清掉Local RAM ---*/
    local_memory_reset( LocalRam_variables.LocalRAMBuf, mem_size); //94232 cycles


    /*--- 把之前寫入SDRAM的資料讀出來 ---*/
    memcpy_fast_far( LocalRam_variables.LocalRAMBuf,
                     SDRAM_variables.SDRAMBuf + ( i * mem_size ), mem_size ); //37212 cycles

    /*--- 檢查讀出來的資料是否正確 ---*/
    data = 0xA5A5;

    for ( j = 0; j < mem_size; j++ )
    {
        if( LocalRam_variables.LocalRAMBuf[j] != data )
        {
            //NG
            //Memory_Module.SDRAM_Error_Count++;
        }
        data = data + 1;
    }






    /*-------------------------------------------*/


    for ( i = 0; i < wave_total_page; i++ ) //wave總共19頁4k
    {
        /*--- Local RAM 先塞滿測試的資料 ---*/
        data = 0xA5A5;

        for ( j = 0; j < mem_size; j++) //114687 cycles
        {
            LocalRam_variables.LocalRAMBuf[j] = data;
            data = data + 1;
        }

        /*--- 利用RAM一次寫入4k速度比較快也節省時間 ---*/
        memcpy_fast_far( SDRAM_variables.SDRAMBuf + ( i * mem_size ),
                         LocalRam_variables.LocalRAMBuf, mem_size ); //16551 cycles

        /*--- 清掉Local RAM ---*/
        local_memory_reset( LocalRam_variables.LocalRAMBuf, mem_size); //94232 cycles


        /*--- 把之前寫入SDRAM的資料讀出來 ---*/
        memcpy_fast_far( LocalRam_variables.LocalRAMBuf,
                         SDRAM_variables.SDRAMBuf + ( i * mem_size ), mem_size ); //37212 cycles

        /*--- 檢查讀出來的資料是否正確 ---*/
        data = 0xA5A5;

        for ( j = 0; j < mem_size; j++ )
        {
            if( LocalRam_variables.LocalRAMBuf[j] != data )
            {
                //NG
                Memory_Module.SDRAM_Error_Count++;
            }
            data = data + 1;

        }


        if ( Memory_Module.SDRAM_Error_Count > 0 ) //NG
        {
            ///debug msg///
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg,
                    "CPU_Timer2: %lu ms > SDRAM WAVE section %lu, total error: %lu ....NG, \n",
                    CpuTimer2.InterruptCount, i, Memory_Module.SDRAM_Error_Count ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            Test_Result_NG = 1;
            //判定NG
        }

        Memory_Module.SDRAM_Error_Count = 0;
    }


    if ( Test_Result_NG == 1 )
    {
        return(1); //NG
    }
    else
    {
        return(0); //PASS
    }


}


////////////////////////////////////////////////////////////////////
//
// SDRAM ARB1~TRI Section test
//
Uint16 SDRAM_Self_Test_V2(void)
{

    Uint16 sdram_buf;
    Uint16 data;
    Uint32 i, j;
    Uint32 mem_size;
    Uint32 wave_total_page;
    Uint16 Test_Result_NG = 0;

    Memory_Module.SDRAM_Error_Count = 0;

    i = 0;
    j = 0;
    mem_size = LOCAL_RAM_BUFFER_SIZE; //4k
    wave_total_page = 19;

    //////////////////////////////////////////////////
    //
    // 檢查wave page
    //
    for ( i = 0; i < wave_total_page; i++ )
    {
        data = 0xA5A5;

        for ( j = 0; j < mem_size; j++ )
        {
            *( SDRAM_variables.SDRAMBuf + ( i << 12 ) + j ) = data; //7 cycles
            sdram_buf = *( SDRAM_variables.SDRAMBuf + ( i << 12 ) + j );

            if ( data != sdram_buf )
            {
                //NG
                Memory_Module.SDRAM_Error_Count++;

                ///debug msg///
                /*
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg,
                        "CPU_Timer2: %lu ms > Section %lu-%lu, data = %X, sdram_buf != %X ....NG\n",
                        CpuTimer2.InterruptCount, i, j, data, sdram_buf); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
                */
            }
            else
            {
                ///debug msg///
                /*
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg,
                        "CPU_Timer2: %lu ms > Section %lu-%lu, data = %X, sdram_buf == %X ....PASS\n",
                        CpuTimer2.InterruptCount, i, j, data, sdram_buf); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
                */

            }

            data = data + 1;
        }

        if ( Memory_Module.SDRAM_Error_Count > 0 ) //NG
        {
            ///debug msg///
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg,
                    "CPU_Timer2: %lu ms > SDRAM WAVE section %lu, total error: %lu ....NG, \n",
                    CpuTimer2.InterruptCount, i, Memory_Module.SDRAM_Error_Count ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            Test_Result_NG = 1;
            //判定NG
        }

        Memory_Module.SDRAM_Error_Count = 0;
    }


    //////////////////////////////////////////////////
    //
    // 檢查checksum page
    //
    for ( i = 0; i < wave_total_page; i++ )
    {
        data = 0xA5A5;

        for ( j = 0; j < mem_size; j++ )
        {
            *( SDRAM_variables.SDRAMBuf + ( ( i << 12 ) + 0x14000 ) + j ) = data; //7 cycles
            sdram_buf = *( SDRAM_variables.SDRAMBuf + ( ( i << 12 ) + 0x14000 ) + j );

            if ( data != sdram_buf )
            {
                //NG
                Memory_Module.SDRAM_Error_Count++;

                ///debug msg///
                /*
                #ifdef DEBUG_ON
                sprintf(SCI_Module_Variables.Debug_Msg,
                        "CPU_Timer2: %lu ms > Section %lu-%lu, data = %X, sdram_buf != %X ....NG\n",
                        CpuTimer2.InterruptCount, i, j, data, sdram_buf); //9530 cycles
                scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
                #endif
                */
            }

            data = data + 1;
        }

        if ( Memory_Module.SDRAM_Error_Count > 0 ) //NG
        {
            ///debug msg///
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg,
                    "CPU_Timer2: %lu ms > SDRAM CheckSum section %lu, total error: %lu ....NG, \n",
                    CpuTimer2.InterruptCount, i, Memory_Module.SDRAM_Error_Count ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif

            Test_Result_NG = 1;
            //判定NG
        }

        Memory_Module.SDRAM_Error_Count = 0;
    }




    if ( Test_Result_NG == 1 )
    {
        return(1); //NG
    }
    else
    {
        return(0); //PASS
    }

}




/////////////////////////////////////////////////////////////////////
Uint16 Load_WAVE_SDRAM_to_LOCAL_RAM_Process( Uint16 waveform_set, Uint16 wave_buffer )
{
    Uint32 i;
    Uint32 mem_size;
    Uint32 wave_page, CheckSum_page;
    int16 int16_buf, int16_buf2, int16_buf3;

    mem_size = LOCAL_RAM_BUFFER_SIZE; //4k

    wave_page = (Uint32)waveform_set << 12 ;
    CheckSum_page = wave_page + 0x14000;


    for ( i = 0; i < mem_size; i++ ) //48 x 4096 cycles
    {
        int16_buf = *( SDRAM_variables.SDRAMBuf + wave_page + i ); //10 cycles

        //計算checkSum
        int16_buf2 = ( int16_buf >> 8 ) + int16_buf; //6 cycles

        int16_buf3 = *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ); //10 cycles

        if ( int16_buf2 == int16_buf3 ) //22 cycles
        {
            WAVE_DATA.local_ram[wave_buffer][i] = int16_buf;
        }
        else
        {

            //NG
            return 1;
        }
    }

    //OK
    return 0;

}














#ifdef BYPASS
/////////////////////////////////////////////////////////////////////
Uint16 Load_WAVE_SDRAM_to_LOCAL_RAM_Process(void)
{
    Uint32 i;
    Uint32 mem_size;
    Uint32 wave_page, CheckSum_page;
    int16 int16_buf, int16_buf2, int16_buf3;

    mem_size = LOCAL_RAM_BUFFER_SIZE; //4k

    wave_page = (Uint32)Global_Variables.POWER_OUTPUT_SETTING_VARIABLES.Waveform_set_user << 12 ;
    CheckSum_page = wave_page + 0x14000;



    for ( i = 0; i < mem_size; i++ ) //48 x 4096 cycles
    {
        int16_buf = *( SDRAM_variables.SDRAMBuf + wave_page + i ); //10 cycles

        //計算checkSum
        int16_buf2 = ( int16_buf >> 8 ) + int16_buf; //6 cycles

        int16_buf3 = *( SDRAM_variables.SDRAMBuf + CheckSum_page + i ); //10 cycles

        if ( int16_buf2 == int16_buf3 ) //22 cycles
        {
            WAVE_DATA.sdram[i] = int16_buf;

        }
        else
        {
            //NG

            ///debug msg///
            #ifdef DEBUG_ON
            sprintf(SCI_Module_Variables.Debug_Msg,
                    "CPU_Timer2: %lu ms > Load SDRAM to Local RAM .... NG, \n",
                    CpuTimer2.InterruptCount ); //9530 cycles
            scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
            #endif


            return 1;
        }
    }

    ///debug msg///
    #ifdef DEBUG_ON
    sprintf(SCI_Module_Variables.Debug_Msg,
            "CPU_Timer2: %lu ms > Load SDRAM to Local RAM .... OK, \n",
            CpuTimer2.InterruptCount ); //9530 cycles
    scia_TXwrite_char(SCI_Module_Variables.Debug_Msg , strlen(SCI_Module_Variables.Debug_Msg)); //4,575,499 cycles
    #endif

    return 0;



}
#endif








/////////////////////////////////////////////////////////////////////
void local_memory_reset( volatile Uint16 *pointer, Uint16 length )
{
	Uint16 i;
    Uint16 data;

    data = 0x0;
	
	for( i = 0; i < length; i++ )
	{
		*pointer = data;
		pointer ++;
	}
}

/////////////////////////////////////////////////////////////////////
void local_memory_copy( Uint16 volatile *direction,
                        Uint16 volatile *source ,
                        Uint16 direction_address,
                        Uint16 source_address,
                        Uint16 length )
{
	Uint16 i;

    source = ( Uint16* )( source + source_address );
    direction = ( Uint16* )( direction + direction_address );

    for( i = 0; i < length; i++ )
	{
		*direction = *source;
		direction++;
		source++;
	}
}














//
// End of file
//
