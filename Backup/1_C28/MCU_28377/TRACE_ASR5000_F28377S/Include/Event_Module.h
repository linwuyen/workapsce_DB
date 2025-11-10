//###########################################################################
//
// FILE:    Event_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211105
//###########################################################################




//----------------------------------------------------
//
// Event 1 事件旗標
//
struct EVENT1_FLAG_BITS {    // bits
    Uint16 Load_Cali_Variable_from_EEPROM:1;    //0
    Uint16 Save_Cali_Variable_to_EEPROM:1;      //1
    Uint16 Set_DAC7612_CHA:1;                   //2
    Uint16 Set_DAC7612_CHB:1;                   //3
    Uint16 Model_Type_Change:1;                 //4
    Uint16 Calc_VREFDAC_Limit:1;                //5
    Uint16 Calc_V_SlewRate_set:1;               //6
    Uint16 Set_Start_phase:1;                   //7
    Uint16 Set_Stop_phase:1;                    //8
    Uint16 Set_Cali_Variable_default:1;         //9
    Uint16 Set_Cali_Step:1;                     //10
    Uint16 Set_Cali_Meter_Value:1;              //11
    Uint16 Cancel_Cali_Process:1;               //12
    Uint16 Reload_Wave_CF_set:1;                //13
    Uint16 SCIA_echo:1;                         //14
    Uint16 MS_Sync_SDRAM_Process:1;             //15
};

union EVENT1_FLAG_REG {
   Uint16                 all;
   struct EVENT1_FLAG_BITS bit;
};

//----------------------------------------------------
//
// Event 2 事件旗標
//
struct EVENT2_FLAG_BITS {    // bits

    //Uint16 SYNC_Mode_Change_FREQ:1;             //0
    Uint16 SYNC_Mode_Change_Phase_Delay:1;      //1
    Uint16 Preload_SQU_WAVE_DATA:1;             //2
    Uint16 Preload_WAVE_DATA:1;                 //3
    Uint16 SCIA_ask_service:1;                  //4
    Uint16 VI_raw_data_trig:1;                  //5
    Uint16 Update_Cali_CheckSum:1;              //6
    Uint16 Trig_Value_Set_User_Change:1;        //7
    //Uint16 SYNC_Mode_Calc_Zero_Phase_Count:1;   //8
    Uint16 SEQ_Mode_Clear_Jump_Count:1;         //9
    Uint16 SEQ_Mode_Clear_All_Step:1;           //10
    Uint16 SEQ_Mode_Clear_CheckSum:1;           //11
    Uint16 Cali_Set_Max_DCV:1;                  //12
    Uint16 Cali_Set_Min_DCV:1;                  //13
    Uint16 Cali_Set_0_DCV:1;                    //14
    Uint16 Check_Cali_CheckSum:1;               //15
};

union EVENT2_FLAG_REG {
   Uint16                 all;
   struct EVENT2_FLAG_BITS bit;
};


//----------------------------------------------------
//
// Event 4 事件旗標
//
struct EVENT4_FLAG_BITS {    // bits

    Uint16 Input_Flags_Update:1;                    //0
    Uint16 Load_HL_Cali_Variable_from_EEPROM:1;     //1
    Uint16 Update_HL_Cali_CheckSum:1;               //2
    Uint16 Load_HL_Cali_to_Cali:1;                  //3
    Uint16 Set_Output_Range_Option:1;               //4
    Uint16 Change_Output_Range:1;                   //5
    Uint16 Adj_Output_Range:1;                      //6
    Uint16 Save_HL_Cali_Variable_to_EEPROM:1;       //7
    Uint16 SEQ_Mode_Refresh_CheckSum:1;             //8

};

union EVENT4_FLAG_REG {
   Uint16                 all;
   struct EVENT4_FLAG_BITS bit;
};


//----------------------------------------------------
//
// Event 3 事件旗標
//
struct EVENT3_FLAG_BITS {    // bits

    //Uint16 Change_ACV_DCV:1;                    //0
    //Uint16 Change_DCV:1;                        //1
    //Uint16 VCA_Mode_Change_ACV:1;               //2
    //Uint16 EXT_Mode_Change_GAIN:1;              //3
    Uint16 ADD_Mode_Change_ACV_DCV_GAIN:1;      //4
    //Uint16 Change_ACDC_to_DC_Calc_VREF:1;       //5
    Uint16 Change_FREQ:1;                       //6
    Uint16 Change_Waveform:1;                   //7
    Uint16 MS_Mode_Change_ACV_DCV:1;            //8
    Uint16 MS_Mode_Change_Phase_Set:1;          //9

    Uint16 Set_Output_ON_OFF:1;                 //10

};

union EVENT3_FLAG_REG {
   Uint16                 all;
   struct EVENT3_FLAG_BITS bit;
};








//----------------------------------------------------
//
// Debug Event 事件旗標
//
struct DEBUG_EVENT_FLAG_BITS {    // bits

    Uint16 Print_All_Cali_Variable:1;    //0


};

union DEBUG_EVENT_FLAG_REG {
   Uint16                 all;
   struct DEBUG_EVENT_FLAG_BITS bit;
};


//----------------------------------------------------
typedef struct EVENT_MODULE_VARIABLES_REG{

    union EVENT1_FLAG_REG Event_Flag;
    union EVENT2_FLAG_REG Event2_Flag;
    union EVENT3_FLAG_REG Event3_Flag;
    union EVENT4_FLAG_REG Event4_Flag;


    union DEBUG_EVENT_FLAG_REG Debug_Event_Flag;


}EVENT_MODULE_VARIABLES_REG;




extern volatile EVENT_MODULE_VARIABLES_REG Event_Module;




void Event_Scan(void);
void Init_Event_Module_Variables(void);



void Set_V_Cali_Variable_default_Event(void);
void Set_I_Cali_Variable_default_Event(void);
void Set_EXT_Cali_Variable_default_Event(void);

void Load_Cali_Variable_from_EEPROM_Event(void);
void Save_Cali_Variable_to_EEPROM_Event(void);


void Set_Cali_Step_Event(void);
void Set_Cali_Step_Event_6step(void);

void Set_Cali_Meter_Value_Event(void);
void Cancel_Cali_Process_Event(void);
void Print_All_Cali_Variable_Event(void);
void Update_Cali_CheckSum_Event(void);

void Model_Type_Change_Event(void);
void Set_DAC7612_CHA_Event(void);
void Set_DAC7612_CHB_Event(void);

void VI_raw_data_trig_Event(void);


void Set_Start_phase_Event(void);
void Set_Stop_phase_Event(void);
void Calc_V_SlewRate_set_Event(void);
//void SYNC_Mode_Calc_Zero_Phase_Count_Event(void);


void Change_FREQ_Event(void);
//void SYNC_Mode_Change_FREQ_Event(void);

void SYNC_Mode_Change_Phase_Delay_Event(void);

void Preload_SQU_WAVE_DATA_Event(void);

void Preload_WAVE_DATA_Event(void);
void Change_Waveform_Event(void);



//void Change_ACV_DCV_Event(void);
void Change_DCV_Event(void);
//void EXT_Mode_Change_GAIN_Event(void);
void VCA_Mode_Change_ACV_Event(void);
void ADD_Mode_Change_ACV_DCV_GAIN_Event(void);
//void Change_ACDC_to_DC_Calc_VREF_Event(void);
void Trig_Value_Set_User_Change_Event(void);

void SEQ_Mode_Clear_Jump_Count_Event(void);

void SEQ_Mode_Clear_All_Step_Event(void);
void SEQ_Mode_Clear_CheckSum_Event(void);

void Reload_Wave_CF_set_Event(void);

//void Reload_EXTIO_Output_Status_Event(void);


void MS_Mode_Change_ACV_DCV_Event(void);

void Set_Output_ON_OFF_Event(void);

void MS_Mode_Change_Phase_Set_Event(void);

void Check_Cali_CheckSum_Event(void);


void Calc_VREFDAC_Limit_Event(void);

void SCIA_ask_service_Event(void);

void SCIA_echo_Event(void);

Uint16 hex_ascii_to_uint16( char char_value );
char hex_uint16_to_ascii( Uint16 uint16_value );


void Input_Flags_Update_Event(void);



void Load_HL_Cali_Variable_from_EEPROM_Event(void);
void Update_HL_Cali_CheckSum_Event(void);
void Load_HL_Cali_to_Cali_Event(void);
void Set_Output_Range_Option_Event(void);
void Change_Output_Range_Event(void);
void Adj_Output_Range_Event(void);
void Save_HL_Cali_Variable_to_EEPROM_Event(void);
void Set_V_HL_Cali_Variable_default_Event(void);
void Set_I_HL_Cali_Variable_default_Event(void);
void Set_EXT_HL_Cali_Variable_default_Event(void);

void SEQ_Mode_Refresh_CheckSum_Event(void);
//
// End of file
//
