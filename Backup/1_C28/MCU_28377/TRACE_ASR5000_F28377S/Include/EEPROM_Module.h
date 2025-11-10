//###########################################################################
//
// FILE:    EEPROM_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210916
//###########################################################################


#define EEPROM_SIZE   0x0400      // 1024 x 16bit

#define EEPROM_CHECK_PERIOD     100  //us
#define EEPROM_OVER_TIME_VALUE  1000
// example:
// 設定1000,每 100us(0.1ms)檢查一次,也就是>100ms則over time


#define WRITE_OVER_TIME     0x01
#define ERASE_OVER_TIME     0x02
#define ERAL_OVER_TIME      0x03
#define DATA_VERIFY_ERROR   0x04



/////////////////////////////////////////////////////////////////////
//
// EEPROM位址分配
//

//
// 校正值版本
//
#define CALI_DATA_VER_EEPROM_ADDR                  0x0F0


//
// 存放位址 0x100-0x1FF
//
#define LR_ADC_TO_V_FB_OFFSET_EEPROM_ADDR     0x100
//#define LR_ADC_TO_V_FB_SCALE_EEPROM_ADDR      0x103
#define LR_ADC_TO_I_FB_OFFSET_EEPROM_ADDR     0x106
//#define LR_ADC_TO_I_FB_SCALE_EEPROM_ADDR      0x109
#define LR_FREQ_TO_VRMS_A_EEPROM_ADDR         0x10C
#define LR_FREQ_TO_VRMS_B_EEPROM_ADDR         0x10F
#define LR_FREQ_TO_VRMS_C_EEPROM_ADDR         0x112
#define LR_CS_MPY_360000_EEPROM_ADDR          0x115
#define LR_INV_RS_EEPROM_ADDR                 0x118
#define LR_V_RMS_OFFSET_EEPROM_ADDR           0x11B
#define LR_I_RMS_OFFSET_EEPROM_ADDR           0x11E
#define LR_P_V_PEAK_OFFSET_EEPROM_ADDR        0x121
#define LR_N_V_PEAK_OFFSET_EEPROM_ADDR        0x124
#define LR_P_I_PEAK_OFFSET_EEPROM_ADDR        0x127
#define LR_N_I_PEAK_OFFSET_EEPROM_ADDR        0x12A
#define LR_ADC_TO_V_FB_SCALE_P_EEPROM_ADDR    0x12D
#define LR_ADC_TO_V_FB_SCALE_N_EEPROM_ADDR    0x130
#define LR_ADC_TO_I_FB_SCALE_P_EEPROM_ADDR    0x133
#define LR_ADC_TO_I_FB_SCALE_N_EEPROM_ADDR    0x136
#define LR_VSET_TO_DAC_SCALE_EEPROM_ADDR      0x139
#define LR_VSET_TO_DAC_OFFSET_EEPROM_ADDR     0x13C
//#define LR_IREF_TO_DAC_SCALE_EEPROM_ADDR      0x13F
//#define LR_IREF_TO_DAC_OFFSET_EEPROM_ADDR     0x142
#define LR_ADC_to_EXTIN_L1_SCALE_EEPROM_ADDR      0x145
#define LR_ADC_to_EXTIN_L1_OFFSET_EEPROM_ADDR      0x148



//
// 存放位址 0x200-0x2FF
//
#define HR_ADC_TO_V_FB_OFFSET_EEPROM_ADDR     0x200
//#define HR_ADC_TO_V_FB_SCALE_EEPROM_ADDR      0x203
#define HR_ADC_TO_I_FB_OFFSET_EEPROM_ADDR     0x206
//#define HR_ADC_TO_I_FB_SCALE_EEPROM_ADDR      0x209
#define HR_FREQ_TO_VRMS_A_EEPROM_ADDR         0x20C
#define HR_FREQ_TO_VRMS_B_EEPROM_ADDR         0x20F
#define HR_FREQ_TO_VRMS_C_EEPROM_ADDR         0x212
#define HR_CS_MPY_360000_EEPROM_ADDR          0x215
#define HR_INV_RS_EEPROM_ADDR                 0x218
#define HR_V_RMS_OFFSET_EEPROM_ADDR           0x21B
#define HR_I_RMS_OFFSET_EEPROM_ADDR           0x21E
#define HR_P_V_PEAK_OFFSET_EEPROM_ADDR        0x221
#define HR_N_V_PEAK_OFFSET_EEPROM_ADDR        0x224
#define HR_P_I_PEAK_OFFSET_EEPROM_ADDR        0x227
#define HR_N_I_PEAK_OFFSET_EEPROM_ADDR        0x22A
#define HR_ADC_TO_V_FB_SCALE_P_EEPROM_ADDR    0x22D
#define HR_ADC_TO_V_FB_SCALE_N_EEPROM_ADDR    0x230
#define HR_ADC_TO_I_FB_SCALE_P_EEPROM_ADDR    0x233
#define HR_ADC_TO_I_FB_SCALE_N_EEPROM_ADDR    0x236
#define HR_VSET_TO_DAC_SCALE_EEPROM_ADDR      0x239
#define HR_VSET_TO_DAC_OFFSET_EEPROM_ADDR     0x23C
//#define HR_IREF_TO_DAC_SCALE_EEPROM_ADDR      0x23F
//#define HR_IREF_TO_DAC_OFFSET_EEPROM_ADDR     0x242
#define HR_ADC_to_EXTIN_L1_SCALE_EEPROM_ADDR      0x245
#define HR_ADC_to_EXTIN_L1_OFFSET_EEPROM_ADDR      0x248






/////////////////////////////////////////////////////////////////////
//
// 校正初始值
//


/*---------------------------------------------------------------*/
//
// 5075機種
//
//20230218
#define ADC_TO_V_FB_OFFSET_5075     -32747.0762
#define ADC_TO_V_FB_SCALE_P_5075    0.01714
#define ADC_TO_V_FB_SCALE_N_5075    0.01714
//20230218
#define ADC_TO_I_FB_OFFSET_5075     -32777
#define ADC_TO_I_FB_SCALE_P_5075    0.00111
#define ADC_TO_I_FB_SCALE_N_5075    0.00111
//20230218
#define VSET_to_DAC_SCALE_5075      115.2157
#define VSET_to_DAC_OFFSET_5075     734.5603
//#define IREF_TO_DAC_SCALE_5075      111.6818
#define IREF_TO_DAC_SCALE_5075      100.0
#define IREF_TO_DAC_OFFSET_5075     0
//20230218
#define FREQ_TO_VRMS_A_5075         -1.2612e-08
#define FREQ_TO_VRMS_B_5075         -2.8991e-05
#define FREQ_TO_VRMS_C_5075         1.0028


/*---------------------------------------------------------------*/
//
// 5150機種
// => 5150 Low Range
//

/*
#define ADC_TO_V_FB_OFFSET_5150     -32604.6445
#define ADC_TO_V_FB_SCALE_P_5150    0.016925482
#define ADC_TO_V_FB_SCALE_N_5150    0.016929435
#define ADC_TO_I_FB_OFFSET_5150     -32738
#define ADC_TO_I_FB_SCALE_P_5150    0.002142773
#define ADC_TO_I_FB_SCALE_N_5150    0.002146873
#define VSET_to_DAC_SCALE_5150      113.257309
#define VSET_to_DAC_OFFSET_5150     745.677917
#define IREF_TO_DAC_SCALE_5150      50.0
#define IREF_TO_DAC_OFFSET_5150     0
#define FREQ_TO_VRMS_A_5150         2.34E-08
#define FREQ_TO_VRMS_B_5150         -2.77E-06
#define FREQ_TO_VRMS_C_5150         1.00004292
*/
//20230911
#define ADC_TO_V_FB_OFFSET_5150     -32858.2578
#define ADC_TO_V_FB_SCALE_P_5150    0.0169414859
#define ADC_TO_V_FB_SCALE_N_5150    0.0169422589
#define ADC_TO_I_FB_OFFSET_5150     -32738
#define ADC_TO_I_FB_SCALE_P_5150    0.002142773
#define ADC_TO_I_FB_SCALE_N_5150    0.002146873
#define VSET_to_DAC_SCALE_5150      113.357513
#define VSET_to_DAC_OFFSET_5150     692.403625
#define IREF_TO_DAC_SCALE_5150      50.0
#define IREF_TO_DAC_OFFSET_5150     0
#define FREQ_TO_VRMS_A_5150         -8.39731396e-09
#define FREQ_TO_VRMS_B_5150         -1.32136411e-05
#define FREQ_TO_VRMS_C_5150         1.00140524




/*---------------------------------------------------------------*/
//
// 5151機種
// => 5150 High Range
//

/*
#define ADC_TO_V_FB_OFFSET_5151     -32663.8965
#define ADC_TO_V_FB_SCALE_P_5151    0.034220349
#define ADC_TO_V_FB_SCALE_N_5151    0.034248155
#define ADC_TO_I_FB_OFFSET_5151     -32513
#define ADC_TO_I_FB_SCALE_P_5151    0.001074567
#define ADC_TO_I_FB_SCALE_N_5151    0.00106636
#define VSET_to_DAC_SCALE_5151      57.93
#define VSET_to_DAC_OFFSET_5151     746.356873
#define IREF_TO_DAC_SCALE_5151      100.0
#define IREF_TO_DAC_OFFSET_5151     0
#define FREQ_TO_VRMS_A_5151         2.11E-09
#define FREQ_TO_VRMS_B_5151         -2.23E-05
#define FREQ_TO_VRMS_C_5151         1.00220859
*/
//20230911
#define ADC_TO_V_FB_OFFSET_5151     -32846.8984
#define ADC_TO_V_FB_SCALE_P_5151    0.0340369307
#define ADC_TO_V_FB_SCALE_N_5151    0.033897195
#define ADC_TO_I_FB_OFFSET_5151     -32513
#define ADC_TO_I_FB_SCALE_P_5151    0.001074567
#define ADC_TO_I_FB_SCALE_N_5151    0.00106636
#define VSET_to_DAC_SCALE_5151      56.6100464
#define VSET_to_DAC_OFFSET_5151     1029.40564
#define IREF_TO_DAC_SCALE_5151      100.0
#define IREF_TO_DAC_OFFSET_5151     0
#define FREQ_TO_VRMS_A_5151         -6.83740264e-09
#define FREQ_TO_VRMS_B_5151         -2.12583091e-05
#define FREQ_TO_VRMS_C_5151         1.00219429


/*---------------------------------------------------------------*/
//
// 不分機種
//
// 方便測試, 直接帶入手上機器校正後的值
#define INV_RS_REF                  1.05E-05
#define CS_MPY_360000_REF           0.013984131
#define V_RMS_OFFSET                -0.525713444
#define I_RMS_OFFSET                -0.07565587
#define P_V_PEAK_OFFSET             0
#define N_V_PEAK_OFFSET             0
#define P_I_PEAK_OFFSET             0
#define N_I_PEAK_OFFSET             0
#define ADC_to_EXTIN_L1_SCALE_REF   5698.0498
#define ADC_to_EXTIN_L1_OFFSET_REF  49139.6094
#define ADC_TO_TEMP_C               0.1221


//
// 校正流程判斷NG使用
//
//判斷NG
#define INV_RS_MAX                  0.0001
#define CS_MPY_360000_MAX           7.2e-2
#define ADC_to_EXTIN_L1_OFFSET_MAX  73765.6641 //1.5
#define ADC_to_EXTIN_L1_OFFSET_MIN  24588.5547 //0.5



//----------------------------------------------------
typedef union
{
    float32 all;
    struct
    {
        Uint16 LSB:16;
        Uint16 MSB:16;
    };
}_efloat32_MSB_LSB;



typedef struct EEPROM_MODULE_VARIABLES_REG
{

    Uint16 Self_test_result;

    _efloat32_MSB_LSB  Read_buffer;
    _efloat32_MSB_LSB  Write_buffer;

}EEPROM_MODULE_VARIABLES_REG;


extern volatile EEPROM_MODULE_VARIABLES_REG EEPROM_Module_Variables;



void Init_EEROM_Module_Variables(void);

Uint16 CRC16_Calc(Uint16 *pointer, Uint16 length);

void Read_EEPROM_addr_to_Buffer( Uint16 addr );
Uint16 Write_Buffer_to_EEPROM_addr( Uint16 addr );



//
// End of file
//
