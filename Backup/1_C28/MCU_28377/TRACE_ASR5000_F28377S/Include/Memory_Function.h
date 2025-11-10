//###########################################################################
//
// FILE:    Memory_Function.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210819
//###########################################################################


#define SDRAM_BUFFER_SIZE       0x1000000   // 16M
#define WAVE_DATA_SIZE          0x1000      // 4k
#define LOCAL_RAM_BUFFER_SIZE   0x1000      // 4k

#define SEQ_STEP_MAX            0x400

#define WAVE_ARB1_ADDRESS       0x00000
#define WAVE_ARB2_ADDRESS       0x01000
#define WAVE_ARB3_ADDRESS       0x02000
#define WAVE_ARB4_ADDRESS       0x03000
#define WAVE_ARB5_ADDRESS       0x04000
#define WAVE_ARB6_ADDRESS       0x05000
#define WAVE_ARB7_ADDRESS       0x06000
#define WAVE_ARB8_ADDRESS       0x07000
#define WAVE_ARB9_ADDRESS       0x08000
#define WAVE_ARB10_ADDRESS      0x09000
#define WAVE_ARB11_ADDRESS      0x0A000
#define WAVE_ARB12_ADDRESS      0x0B000
#define WAVE_ARB13_ADDRESS      0x0C000
#define WAVE_ARB14_ADDRESS      0x0D000
#define WAVE_ARB15_ADDRESS      0x0E000
#define WAVE_ARB16_ADDRESS      0x0F000
#define WAVE_SIN_ADDRESS        0x10000
#define WAVE_SQU_ADDRESS        0x11000
#define WAVE_TRI_ADDRESS        0x12000




//----------------------------------------------------
typedef union
{
    Uint32 all;
    struct
    {
        Uint16 LSB:16;
        Uint16 MSB:16;
    };
}__Uint32_MSB_LSB;


//----------------------------------------------------
typedef union
{
    int32 all;
    struct
    {
        Uint16 LSB:16;
        Uint16 MSB:16;
    };
}_mint32_MSB_LSB;



//----------------------------------------------------
typedef struct
{

   Uint16 LocalRAMBuf[LOCAL_RAM_BUFFER_SIZE];

}LOCALRAM_VARS;


extern volatile LOCALRAM_VARS LocalRam_variables;




//----------------------------------------------------
typedef struct
{

   int16 SDRAMBuf[SDRAM_BUFFER_SIZE];

}SDRAM_VARS;

extern __attribute__((far)) volatile SDRAM_VARS SDRAM_variables;




//----------------------------------------------------
//
// V Raw data memory
//
typedef struct
{

   Uint16 local_ram[LOCAL_RAM_BUFFER_SIZE];

}V_RAW_SECTION;


extern volatile V_RAW_SECTION V_RAW;

//----------------------------------------------------
//
// I Raw data memory
//
typedef struct
{

   Uint16 local_ram[LOCAL_RAM_BUFFER_SIZE];

}I_RAW_SECTION;


extern volatile I_RAW_SECTION I_RAW;

//----------------------------------------------------
//
// Wave data memory
//
typedef struct
{

   int16 local_ram[6][LOCAL_RAM_BUFFER_SIZE];

   //
   // 規劃
   // 0: WAVE_SIN
   // 1: WAVE_SQU
   // 2: WAVE_TRI
   // 3: WAVE_ARBx
   // 4: WAVE_ARBx
   // 5: SDRAM同步使用

}WAVE_DATA_SECTION;


extern volatile WAVE_DATA_SECTION WAVE_DATA;













//----------------------------------------------------
//
// SEQ功能
//

//
// 0x961 SEQ Config1
//
struct  SEQ_FUNCTION_CONFIG1_FLAG_BITS {   // bit
    Uint16 DCV_Const:1;             // 0
    Uint16 DCV_Keep:1;              // 1
    Uint16 DCV_Sweep:1;             // 2
    Uint16 ACV_Const:1;             // 3
    Uint16 ACV_Keep:1;              // 4
    Uint16 ACV_Sweep:1;             // 5
    Uint16 FREQ_Const:1;            // 6
    Uint16 FREQ_Keep:1;             // 7
    Uint16 FREQ_Sweep:1;            // 8
    Uint16 Start_Phase_Enable:1;    // 9
    Uint16 Stop_Phase_Enable:1;     // 10
    Uint16 Sync_Code_0:1;           // 11
    Uint16 Sync_Code_1:1;           // 12
    Uint16 Sync_Code_2:1;           // 13
    Uint16 Step_Continuous:1;       // 14
    Uint16 Step_Hold:1;             // 15

};
union SEQ_FUNCTION_CONFIG1_FLAG_REG {
   Uint16                                   all;
   struct SEQ_FUNCTION_CONFIG1_FLAG_BITS   bit;
};


//
// 0x962 SEQ Config2
//
struct  SEQ_FUNCTION_CONFIG2_FLAG_BITS {   // bit
    Uint16 Step_Stop:1;             // 0
    Uint16 Jump_Enable:1;           // 1
    Uint16 WAVE_Form:5;             // 2-6
    Uint16 Branch1_Enable:1;        // 7
    Uint16 Branch2_Enable:1;        // 8
    Uint16 Sync_Code_3:1;           // 9
    Uint16 Sync_Code_4:1;           // 10
    Uint16 Wave_Const:1;            // 11
    Uint16 Wave_Keep:1;             // 12
    Uint16 Sync_Code_5:1;           // 13
    Uint16 Sync_Code_6:1;           // 14
    Uint16 Sync_Code_7:1;           // 15

};
union SEQ_FUNCTION_CONFIG2_FLAG_REG {
   Uint16                                   all;
   struct SEQ_FUNCTION_CONFIG2_FLAG_BITS   bit;
};





//----------------------------------------------------
//
// SEQ Step Section 1
//
typedef struct
{
    union SEQ_FUNCTION_CONFIG1_FLAG_REG SEQ_Config_State1_set_user[SEQ_STEP_MAX];   //0x961
    union SEQ_FUNCTION_CONFIG2_FLAG_REG SEQ_Config_State2_set_user[SEQ_STEP_MAX];   //0x962
    Uint16 SEQ_Start_Phase_set_user[SEQ_STEP_MAX];                                  //0x966
    Uint16 SEQ_Stop_Phase_set_user[SEQ_STEP_MAX];                                   //0x967

}SEQ_STEP_SECTION1;


extern volatile SEQ_STEP_SECTION1 SEQ_Step_Section1;


//----------------------------------------------------
//
// SEQ Step Section 2
//
typedef struct
{
    Uint16 SEQ_Jump_set_user[SEQ_STEP_MAX];         // 0x968
    Uint16 SEQ_Jump_Count_set_user[SEQ_STEP_MAX];   // 0x969
    Uint16 SEQ_Branch1_set_user[SEQ_STEP_MAX];      // 0x96A
    Uint16 SEQ_Branch2_set_user[SEQ_STEP_MAX];      // 0x96B

}SEQ_STEP_SECTION2;


extern volatile SEQ_STEP_SECTION2 SEQ_Step_Section2;


//----------------------------------------------------
//
// SEQ Step Section 3
//
typedef struct
{
    __Uint32_MSB_LSB SEQ_Step_Time_set_user[SEQ_STEP_MAX];  // 0x96C
    __Uint32_MSB_LSB SEQ_Freq_set_user[SEQ_STEP_MAX];       // 0x978

}SEQ_STEP_SECTION3;


extern volatile SEQ_STEP_SECTION3 SEQ_Step_Section3;


//----------------------------------------------------
//
// SEQ Step Section 4
//
typedef struct
{
    _mint32_MSB_LSB SEQ_DCV_set_user[SEQ_STEP_MAX]; // 0x97A
    _mint32_MSB_LSB SEQ_ACV_set_user[SEQ_STEP_MAX]; // 0x97C

}SEQ_STEP_SECTION4;


extern volatile SEQ_STEP_SECTION4 SEQ_Step_Section4;



//----------------------------------------------------
//
// SEQ Step Section 5
//
typedef struct
{
    Uint16 SEQ_SQU_Duty_set_user[SEQ_STEP_MAX];

}SEQ_STEP_SECTION5;


extern volatile SEQ_STEP_SECTION5 SEQ_Step_Section5;



//----------------------------------------------------
typedef struct MEMORY_MODULE_VARIABLES_REG{

    Uint32 SDRAM_Error_Count;

    Uint16  wave_buffer_section;
    Uint16  wave_buffer_section_3;
    Uint16  wave_buffer_section_4;

    Uint16  wave_buffer_section_shadow;

}MEMORY_MODULE_VARIABLES_REG;




extern MEMORY_MODULE_VARIABLES_REG Memory_Module;









/////////////////////////////////////////////////////////////////////

void Local_RAM_and_SDRAM_Clear(void);
Uint16 SDRAM_Self_Test(void);
Uint16 SDRAM_Self_Test_V2(void);

//Uint16 Load_WAVE_SDRAM_to_LOCAL_RAM_Process(void);
Uint16 Load_WAVE_SDRAM_to_LOCAL_RAM_Process( Uint16 waveform_set, Uint16 wave_buffer );

void local_memory_reset( volatile Uint16 *pointer, Uint16 length );
void local_memory_copy( Uint16 volatile *direction,
                        Uint16 volatile *source ,
                        Uint16 direction_address,
                        Uint16 source_address,
                        Uint16 length );


void Init_Memory_Module_Variables(void);


//
// End of file
//
