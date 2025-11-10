//###########################################################################
//
// FILE:    CLA_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210826
//###########################################################################



//#define DEVELOP_SV01
//for CCS 量測執行cycle

//
// Included Files
//
#include "F2837xS_Cla_defines.h"
#include <stdint.h>
#include "F28x_Project.h"

/*----------------------------------------------*/
//
// 定義 for 短路保護
//
#define SHORT_V_LEVEL       10.0
#define SHORT_I_LEVEL_H     62258 //65535 * 0.95
#define SHORT_I_LEVEL_L     3276  //65535 * 0.05

//
// 定義 for 4 CF OCP
//
//#define _4CF_OCP_DELAY_COUNT 1200 (無排程)
#define _4CF_OCP_DELAY_COUNT 600 // (= 無排程/排程總數)


//----------------------------------------------------
//
// CLA Control Flag
//
struct CLA_CONTROL_FLAG_BITS {          // bits
    Uint16 SPIB_Receive_Enable:1;       // 0
    Uint16 SPIB_Trasmit_Enable:1;       // 1
    Uint16 Cali_Process:1;              // 2

};

union CLA_CONTROL_FLAG_REG {
   Uint16                     all;
   struct CLA_CONTROL_FLAG_BITS bit;
};



//----------------------------------------------------
//
// DAC Clamp Flag
//
struct DAC_CLAMP_STATUS_BITS {            // bits
    Uint16 VREF_Limit_High_Clamp:1;     // 0
    Uint16 VREF_Limit_Low_Clamp:1;      // 1
    Uint16 WAVE_Limit_High_Clamp:1;     // 2
    Uint16 WAVE_Limit_Low_Clamp:1;      // 3

};

union DAC_CLAMP_STATUS_REG {
   Uint16                     all;
   struct DAC_CLAMP_STATUS_BITS bit;
};


//----------------------------------------------------
//
//
//
typedef union
{
    Uint32 all;

    struct
    {
        Uint32 dec2:12;
        Uint32 dec1:8;
        Uint32 addr:12;
    };

}DDSDATA2;






//----------------------------------------------------
//
// CPU -> CLA
// CLA Read
// CPU Read/Write
//
//
typedef struct CPU_TO_CLA_VARIABLES_REG{


    //Uint16 WAVE_set;
    int32  WAVE_set;
    //為了在CLA能對WAVE DAC箝制所以宣告成int32

    Uint16 VREF_set;

    float VREF_set_limit_High;
    float VREF_set_limit_Low;

    Uint16 WAVE_set_debug;
    Uint16 VREF_set_debug;

    Uint16 OUTPUT_FLAGS_all;

    float ADC_to_V_FB_Offset;
    //float ADC_to_V_FB_Scale;
    float ADC_to_I_FB_Offset;
    //float ADC_to_I_FB_Scale;

    float  ADC_to_V_FB_Scale_P;
    float  ADC_to_V_FB_Scale_N;
    float  ADC_to_I_FB_Scale_P;
    float  ADC_to_I_FB_Scale_N;

    float inv_Rs;
    float Cs_mpy_360000;

    Uint32  FreqStep;

    float VREF_Compensate_Offset_0;
    float VREF_Compensate_Offset_1;
    float VREF_Compensate_Offset_2;
    float VREF_Compensate_Offset_3;


    Uint16 TXBUFFER[2];

    Uint16 Short_OCP_Clear;

    Uint16 _4CF_OCP_Clear;
    float  _4CF_OCP_level;

    Uint16 Bypass_OCP_Flag;

    Uint32 IPK_Hold_Delay_Count;
    Uint16 IPK_Hold_Clear;

    Uint32 VPK_Hold_Delay_Count;
    Uint16 VPK_Hold_Clear;

    Uint16 SPIB_Tx_Data[5];

    union CLA_CONTROL_FLAG_REG Cla_Control_Flag;

    DDSDATA2 dds;

}CPU_TO_CLA_VARIABLES_REG;




//----------------------------------------------------
//
// CPU <- CLA
// CLA Read/Write
// CPU Read
//


typedef struct CLA_TO_CPU_VARIABLES_REG{


    Uint16 WAVE_set_shadow;
    Uint16 VREF_set_shadow;
    float VREF_set_shadow2;

    float V_FB;
    float V_FB_shadow;

    //20230719
    float V_FB_sum;
    float V_FB_shadow2;

    float I_FB;
    float I_FB_shadow;
    float I_shunt;
    float I_Rs;
    float I_Cs;
    float I_FB_sum;
    float I_FB_shadow2;


    Uint16 ADS8353_DUMMY;
    Uint16 ADS8353_CHA;
    Uint16 ADS8353_CHB;

    double ADS8353_CHA_sum;
    double ADS8353_CHB_sum;
    double ADS8353_CHA_sum_shadow;
    double ADS8353_CHB_sum_shadow;

    //20230719
    float ADS8353_CHA_sum2;
    float ADS8353_CHB_sum2;
    float ADS8353_CHA_sum_shadow2;
    float ADS8353_CHB_sum_shadow2;

    double ADS8353_CHB_minus_offset_squ_sum;
    double ADS8353_CHB_minus_offset_squ_sum_shadow;

    Uint16 Sampling_index;
    Uint16 Total_Sample_Points;

    double V_square_sum_shadow;
    double I_square_sum_shadow;
    double V_sum_shadow;
    double I_sum_shadow;
    float p_V_peak_shadow;
    float n_V_peak_shadow;
    float p_I_peak_shadow;
    float n_I_peak_shadow;

    double V_square_sum;
    double I_square_sum;
    double V_sum;
    double I_sum;
    float p_V_peak;
    float n_V_peak;
    float p_I_peak;
    float n_I_peak;

    double P_sum_shadow;
    double P_sum;

    Uint16 RXBUFFER[2];


    #ifdef DEVELOP_SV01
    Uint16 Cla1Task1_Cycle_Meas_Start;
    Uint16 Cla1Task1_Cycle_Meas_Result;
    Uint16 Cla1Task1_Cycle_Meas_Max;
    #endif

    Uint16 Short_OCP;
    float  Short_OCP_IFB;

    float  Short_OCP_V_FB;
    Uint16 Short_OCP_c2;
    Uint16 Short_OCP_VREF_set;
    Uint16 Short_OCP_WAVE_set;

    Uint16 _4CF_OCP;
    float  _4CF_OCP_IFB;
    Uint16 _4CF_OCP_Count;

    Uint32 IPK_Hold_Delay_Counter;
    float  IPK_Hold;

    Uint32 VPK_Hold_Delay_Counter;
    float  VPK_Hold;

    DDSDATA2 dds;
    DDSDATA2 dds_shadow;

    union DAC_CLAMP_STATUS_REG DAC_Clamp_Status;

    Uint16 Exec_Switch;
    Uint16 Avg_Counter;

}CLA_TO_CPU_VARIABLES_REG;


//----------------------------------------------------
//
// CLA SPI狀態機
//
typedef enum
{
    CLA_SPI_INIT_STATE,
    CLA_SPI_READY_STATE,
    CLA_SPI_PARSING_STATE,
    CLA_SPI_REPLY_STATE
}CLA_SPI_STATE_REG;


//----------------------------------------------------
//
// CLA I peak hold狀態機
//
typedef enum
{
    CLA_IPK_HOLD_READY_STATE,
    CLA_IPK_HOLD_DELAY_STATE,
    CLA_IPK_HOLD_UPDATING_STATE
}CLA_IPK_HOLD_STATE_REG;

//----------------------------------------------------
//
// CLA V peak hold狀態機
//
typedef enum
{
    CLA_VPK_HOLD_READY_STATE,
    CLA_VPK_HOLD_DELAY_STATE,
    CLA_VPK_HOLD_UPDATING_STATE
}CLA_VPK_HOLD_STATE_REG;


//----------------------------------------------------
//
// CLA排程01狀態機
// 排程調整則相對應的程式counter要確認
//
typedef enum
{
    CLA_SCHEDULE01_01_STATE,
    CLA_SCHEDULE01_02_STATE,
    CLA_SCHEDULE01_03_STATE,
    CLA_SCHEDULE01_04_STATE,
    CLA_SCHEDULE01_05_STATE

}CLA_SCHEDULE01_STATE_REG;


//----------------------------------------------------
//
// CLA Meas 排程
//
typedef enum
{
    CLA_MEAS_01_STATE,
    CLA_MEAS_02_STATE,
    CLA_MEAS_03_STATE,
    CLA_MEAS_04_STATE,
    CLA_MEAS_05_STATE,
    CLA_MEAS_06_STATE,
    CLA_MEAS_07_STATE

}CLA_MEAS_STATE_REG;



typedef struct CLA_MODULE_VARIABLES_REG{

    Uint32 Counter;
    CLA_SPI_STATE_REG cla_spi_state;
    Uint32 spi_errcount;
    CLA_IPK_HOLD_STATE_REG cla_ipk_hold_state;
    CLA_VPK_HOLD_STATE_REG cla_vpk_hold_state;
    CLA_SCHEDULE01_STATE_REG cla_schedule01_state;
    CLA_MEAS_STATE_REG cla_meas_state;

}CLA_MODULE_VARIABLES_REG;


extern volatile CLA_TO_CPU_VARIABLES_REG CLA_to_CPU_Variables;
extern volatile CPU_TO_CLA_VARIABLES_REG CPU_to_CLA_Variables;
extern volatile CLA_MODULE_VARIABLES_REG CLA_Module_Variables;





//----------------------------------------------------
//
// Function Prototypes
//
// The following are symbols defined in the CLA assembly code
// Including them in the shared header file makes them
// .global and the main CPU can make use of them.
//
__interrupt void Cla1Task1();
__interrupt void Cla1Task2();
__interrupt void Cla1Task3();
__interrupt void Cla1Task4();
__interrupt void Cla1Task5();
__interrupt void Cla1Task6();
__interrupt void Cla1Task7();
__interrupt void Cla1Task8();

void CLA_configClaMemory(void);
void CLA_initCpu1Cla1(void);
void CLA_runTest(void);

__interrupt void cla1Isr1 ();
__interrupt void cla1Isr2 ();
__interrupt void cla1Isr3 ();
__interrupt void cla1Isr4 ();
__interrupt void cla1Isr5 ();
__interrupt void cla1Isr6 ();
__interrupt void cla1Isr7 ();
__interrupt void cla1Isr8 ();

void Init_CLA_Module_Variables(void);

//
// End of file
//













