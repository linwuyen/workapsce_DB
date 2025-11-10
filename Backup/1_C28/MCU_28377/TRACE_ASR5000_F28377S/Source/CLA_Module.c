//###########################################################################
//
// FILE:    CLA_Module.c
//
// TITLE:
//
//###########################################################################
// $Create Date: 20210826
//###########################################################################

#include "Global_VariableDefs.h"



#pragma DATA_SECTION(CPU_to_CLA_Variables,"CpuToCla1MsgRAM");
volatile CPU_TO_CLA_VARIABLES_REG CPU_to_CLA_Variables;

#pragma DATA_SECTION(CLA_to_CPU_Variables,"Cla1ToCpuMsgRAM");
volatile CLA_TO_CPU_VARIABLES_REG CLA_to_CPU_Variables;

#pragma DATA_SECTION(CLA_Module_Variables,"CLADataLS0");
volatile CLA_MODULE_VARIABLES_REG CLA_Module_Variables;


/////////////////////////////////////////////////////////////////////
void Init_CLA_Module_Variables(void)
{
    Uint16 i;

    /////////////////////////////////////////////////////////
    CPU_to_CLA_Variables.OUTPUT_FLAGS_all = 0;

    CPU_to_CLA_Variables.VREF_set = (Uint16)Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
    CPU_to_CLA_Variables.WAVE_set = WAVE_DAC_ZERO;



    CPU_to_CLA_Variables.VREF_set_debug = (Uint16)Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
    CPU_to_CLA_Variables.WAVE_set_debug = WAVE_DAC_ZERO;



    CPU_to_CLA_Variables.TXBUFFER[0] = 0;
    CPU_to_CLA_Variables.TXBUFFER[1] = 0;

    CPU_to_CLA_Variables.ADC_to_V_FB_Offset = 0;
    //CPU_to_CLA_Variables.ADC_to_V_FB_Scale = 0;


    CPU_to_CLA_Variables.ADC_to_I_FB_Offset = 0;
    //CPU_to_CLA_Variables.ADC_to_I_FB_Scale = 0;

    CPU_to_CLA_Variables.inv_Rs = 0;
    CPU_to_CLA_Variables.Cs_mpy_360000 = 0;



    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_P = 0;
    CPU_to_CLA_Variables.ADC_to_V_FB_Scale_N = 0;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_P = 0;
    CPU_to_CLA_Variables.ADC_to_I_FB_Scale_N = 0;

    CPU_to_CLA_Variables.Short_OCP_Clear = 0;

    CPU_to_CLA_Variables._4CF_OCP_Clear = 0;
    CPU_to_CLA_Variables._4CF_OCP_level = Global_Variables.POWER_PROTECT_SETTING_VARIABLES._4CF_OCP_level;

    CPU_to_CLA_Variables.Bypass_OCP_Flag = 0;

    CPU_to_CLA_Variables.IPK_Hold_Clear = 0;
    CPU_to_CLA_Variables.VPK_Hold_Clear = 0;

    CPU_to_CLA_Variables.Cla_Control_Flag.all = 0;

    CPU_to_CLA_Variables.VREF_Compensate_Offset_0 = 0;
    CPU_to_CLA_Variables.VREF_Compensate_Offset_1 = 0;
    CPU_to_CLA_Variables.VREF_Compensate_Offset_2 = 0;
    CPU_to_CLA_Variables.VREF_Compensate_Offset_3 = 0;


    CPU_to_CLA_Variables.VREF_set_limit_High = 0;
    CPU_to_CLA_Variables.VREF_set_limit_Low = 0;

    CPU_to_CLA_Variables.FreqStep = 0;

    /////////////////////////////////////////////////////////

    CLA_to_CPU_Variables.VREF_set_shadow = (Uint16)Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;
    CLA_to_CPU_Variables.WAVE_set_shadow = WAVE_DAC_ZERO;

    CLA_to_CPU_Variables.VREF_set_shadow2 = Global_Variables.Calibration_Variables.VSET_to_DAC_Offset.all;


    CLA_to_CPU_Variables.V_FB = 0;
    CLA_to_CPU_Variables.V_FB_shadow = 0;
    CLA_to_CPU_Variables.I_FB = 0;
    CLA_to_CPU_Variables.I_FB_shadow = 0;

    CLA_to_CPU_Variables.I_shunt = 0;
    CLA_to_CPU_Variables.I_Rs = 0;
    CLA_to_CPU_Variables.I_Cs = 0;

    CLA_to_CPU_Variables.ADS8353_CHA = 0;
    CLA_to_CPU_Variables.ADS8353_CHB = 0;

    CLA_to_CPU_Variables.ADS8353_CHA_sum = 0;
    CLA_to_CPU_Variables.ADS8353_CHA_sum_shadow = 0;
    CLA_to_CPU_Variables.ADS8353_CHB_sum = 0;
    CLA_to_CPU_Variables.ADS8353_CHB_sum_shadow = 0;

    CLA_to_CPU_Variables.ADS8353_CHB_minus_offset_squ_sum = 0;
    CLA_to_CPU_Variables.ADS8353_CHB_minus_offset_squ_sum_shadow = 0;

    CLA_to_CPU_Variables.Sampling_index = 0;
    CLA_to_CPU_Variables.Total_Sample_Points = 0;


    CLA_to_CPU_Variables.V_square_sum_shadow = 0;
    CLA_to_CPU_Variables.I_square_sum_shadow = 0;
    CLA_to_CPU_Variables.V_sum_shadow = 0;
    CLA_to_CPU_Variables.I_sum_shadow = 0;

    CLA_to_CPU_Variables.p_V_peak_shadow = 0;
    CLA_to_CPU_Variables.n_V_peak_shadow = 65535;


    CLA_to_CPU_Variables.p_I_peak_shadow = 0;
    CLA_to_CPU_Variables.n_I_peak_shadow = 65535;

    CLA_to_CPU_Variables.V_square_sum = 0;
    CLA_to_CPU_Variables.I_square_sum = 0;
    CLA_to_CPU_Variables.V_sum = 0;
    CLA_to_CPU_Variables.I_sum = 0;
    CLA_to_CPU_Variables.p_V_peak = 0;
    CLA_to_CPU_Variables.n_V_peak = 65535;
    CLA_to_CPU_Variables.p_I_peak = 0;
    CLA_to_CPU_Variables.n_I_peak = 65535;

    CLA_to_CPU_Variables.P_sum = 0;
    CLA_to_CPU_Variables.P_sum_shadow = 0;

    CLA_to_CPU_Variables.RXBUFFER[0] = 0;
    CLA_to_CPU_Variables.RXBUFFER[1] = 0;


    CLA_to_CPU_Variables.Short_OCP = 0;
    CLA_to_CPU_Variables.Short_OCP_IFB = 0;

    CLA_to_CPU_Variables.Short_OCP_V_FB = 0;
    CLA_to_CPU_Variables.Short_OCP_c2 = 0;
    CLA_to_CPU_Variables.Short_OCP_VREF_set = 0;
    CLA_to_CPU_Variables.Short_OCP_WAVE_set = 0;

    CLA_to_CPU_Variables._4CF_OCP = 0;
    CLA_to_CPU_Variables._4CF_OCP_Count = _4CF_OCP_DELAY_COUNT;
    CLA_to_CPU_Variables._4CF_OCP_IFB = 0;

    for ( i = 0; i < 5; i ++ )
    {
        CPU_to_CLA_Variables.SPIB_Tx_Data[i] = 0x50 + i;
    }

    CLA_to_CPU_Variables.I_FB_sum = 0;
    CLA_to_CPU_Variables.I_FB_shadow2 = 0;

    CLA_to_CPU_Variables.DAC_Clamp_Status.all = 0;


    CLA_to_CPU_Variables.dds.all = 0;
    CLA_to_CPU_Variables.dds_shadow.all = 0;


    //20230719
    CLA_to_CPU_Variables.V_FB_sum = 0;
    CLA_to_CPU_Variables.V_FB_shadow2 = 0;
    CLA_to_CPU_Variables.ADS8353_CHA_sum2 = 0;
    CLA_to_CPU_Variables.ADS8353_CHB_sum2 = 0;
    CLA_to_CPU_Variables.ADS8353_CHA_sum_shadow2 = 0;
    CLA_to_CPU_Variables.ADS8353_CHB_sum_shadow2 = 0;

    CLA_to_CPU_Variables.Exec_Switch = 0;
    CLA_to_CPU_Variables.Avg_Counter = 0;
    /////////////////////////////////////////////////////////
    CLA_Module_Variables.cla_spi_state = CLA_SPI_INIT_STATE;

    CLA_Module_Variables.spi_errcount = 0;

    CLA_Module_Variables.Counter = 0;

    CLA_Module_Variables.cla_ipk_hold_state = CLA_IPK_HOLD_READY_STATE;

    CLA_Module_Variables.cla_schedule01_state = CLA_SCHEDULE01_01_STATE;

}



////////////////////////////////////////////////////////////////////
//
// CLA_configClaMemory - Configure the CLA memory
//
void CLA_configClaMemory(void)
{
    extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;
    EALLOW;

#ifdef _FLASH
    //
    // Copy over code from FLASH to RAM
    //
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
           (uint32_t)&Cla1funcsLoadSize);
#endif //_FLASH

    //
    // Initialize and wait for CLA1ToCPUMsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    //
    // Initialize and wait for CPUToCLA1MsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    //
    // Select LS5RAM to be the programming space for the CLA
    // First configure the CLA to be the master for LS5 and then
    // set the space to be a program block
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;
    MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 1;

    //
    //Next configure LS0RAM and LS1RAM as data spaces for the CLA
    // First configure the CLA to be the master for LS0(1) and then
    // set the spaces to be code blocks
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;
    MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 0;

    EDIS;
}


////////////////////////////////////////////////////////////////////
//
// CLA_initCpu1Cla1 - Initialize CLA1 task vectors and end of task interrupts
//
void CLA_initCpu1Cla1(void)
{
    //
    // Compute all CLA task vectors
    // On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
    //
    EALLOW;
    Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
    //Cla1Regs.MVECT2 = (uint16_t)(&Cla1Task2);
    //Cla1Regs.MVECT3 = (uint16_t)(&Cla1Task3);
    //Cla1Regs.MVECT4 = (uint16_t)(&Cla1Task4);
    //Cla1Regs.MVECT5 = (uint16_t)(&Cla1Task5);
    //Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
    //Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
    //Cla1Regs.MVECT8 = (uint16_t)(&Cla1Task8);

    //
    // Enable the IACK instruction to start a task on CLA in software
    // for all  8 CLA tasks. Also, globally enable all 8 tasks (or a
    // subset of tasks) by writing to their respective bits in the
    // MIER register
    //
    Cla1Regs.MCTL.bit.IACKE = 1;
    //Cla1Regs.MIER.all = 0x00FF;
    Cla1Regs.MIER.all = 0x0001;

    //
    // Configure the vectors for the end-of-task interrupt for all
    // 8 tasks
    //
    PieVectTable.CLA1_1_INT  = &cla1Isr1;
    //PieVectTable.CLA1_2_INT  = &cla1Isr2;
    //PieVectTable.CLA1_3_INT  = &cla1Isr3;
    //PieVectTable.CLA1_4_INT  = &cla1Isr4;
    //PieVectTable.CLA1_5_INT  = &cla1Isr5;
    //PieVectTable.CLA1_6_INT  = &cla1Isr6;
    //PieVectTable.CLA1_7_INT  = &cla1Isr7;
    //PieVectTable.CLA1_8_INT  = &cla1Isr8;

    // Set the TINT0 as the trigger for task 1
    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1 = 68;

    //
    // Enable CLA interrupts at the group and subgroup levels
    //
    //PieCtrlRegs.PIEIER11.all = 0xFFFF;
    PieCtrlRegs.PIEIER11.all = 0x0001;
    //IER |= (M_INT11 );
}


////////////////////////////////////////////////////////////////////
//
// cla1Isr1 - CLA1 ISR 1
//
__interrupt void cla1Isr1 ()
{
    //
    // Acknowledge the end-of-task interrupt for task 1
    //
    PieCtrlRegs.PIEACK.all = M_INT11;

    //
    // Uncomment to halt debugger and stop here
    //
    //    asm(" ESTOP0");
}

////////////////////////////////////////////////////////////////////
//
// cla1Isr1 - CLA1 ISR 2
//
__interrupt void cla1Isr2 ()
{
    asm(" ESTOP0");
}

////////////////////////////////////////////////////////////////////
//
// cla1Isr1 - CLA1 ISR 3
//
__interrupt void cla1Isr3 ()
{
    asm(" ESTOP0");
}

////////////////////////////////////////////////////////////////////
//
// cla1Isr1 - CLA1 ISR 4
//
__interrupt void cla1Isr4 ()
{
    asm(" ESTOP0");
}

////////////////////////////////////////////////////////////////////
//
// cla1Isr1 - CLA1 ISR 5
//
__interrupt void cla1Isr5 ()
{
    asm(" ESTOP0");
}

////////////////////////////////////////////////////////////////////
//
// cla1Isr1 - CLA1 ISR 6
//
__interrupt void cla1Isr6 ()
{
    asm(" ESTOP0");
}

////////////////////////////////////////////////////////////////////
//
// cla1Isr1 - CLA1 ISR 7
//
__interrupt void cla1Isr7 ()
{
    asm(" ESTOP0");
}

////////////////////////////////////////////////////////////////////
//
// cla1Isr1 - CLA1 ISR 8
//
__interrupt void cla1Isr8 ()
{
    //
    // Acknowledge the end-of-task interrupt for task 8
    //
    PieCtrlRegs.PIEACK.all = M_INT11;

    //
    // Uncomment to halt debugger and stop here
    //
//    asm(" ESTOP0");
}











//
// End of file
//

