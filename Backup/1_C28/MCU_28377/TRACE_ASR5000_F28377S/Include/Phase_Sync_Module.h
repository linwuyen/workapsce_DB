//###########################################################################
//
// FILE:    Phase_Sync_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20230615
//###########################################################################




#define SYNC_EXT_PHASE_OFFSET 0.344
// 84uSec*4096 (ASR-2000)



//----------------------------------------------------
//
// 變數
//
typedef struct CONTROL_VARIABLES_REG{
    float SYNC_T;
    float delta_T;
    float fs;
    float festimate;
    float ferr;
    float Angle_err;
    float K_f;
    float K_Angle;
    float Phase_shift;
    float dds_addr;

}CONTROL_VARIABLES_REG;



//----------------------------------------------------
//
// 旗標
//
struct  CONTROL_FLAG_BITS {   // bit
    Uint16 Phase_Sync_Process:1;    // 0

};
union CONTROL_FLAG_REG {
   Uint16                       all;
   struct CONTROL_FLAG_BITS   bit;
};




//----------------------------------------------------
//
// 模組宣告
//
typedef struct PHASE_SYNC_MODULE_REG{

    CONTROL_VARIABLES_REG LINE_Phase_Sync_Variables;
    union CONTROL_FLAG_REG LINE_Phase_Sync_Flags;

}PHASE_SYNC_MODULE_REG;


extern  volatile PHASE_SYNC_MODULE_REG Phase_Sync_Module;



void Init_Phase_Sync_Module_Variables(void);
void Phase_Sync_Process(void);



//
// End of file
//
