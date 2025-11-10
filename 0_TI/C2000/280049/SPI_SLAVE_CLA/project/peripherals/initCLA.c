/*
 * initCLA.c
 *
 *  Created on: 2022¦~3¤ë29¤é
 *      Author: cody_chen
 */
#include "initCLA.h"

#pragma SET_DATA_SECTION("cla_shared")

volatile ST_CLA sCLA;

#pragma SET_DATA_SECTION()   // Reset section to default

void initClaCtrlParam(void)
{
    sCLA.fgStatus = _CSTAT_NO_ACTION;

    sCLA.sVIN.sLPF.f32K1 = VIN_LPF_K1;
    sCLA.sVIN.sCali.f32Gain = 1.0f;
    sCLA.sVIN.sCali.f32Offset = 0.0f;

    sCLA.sVO.sLPF.f32K1 = VIN_LPF_K1;
    sCLA.sVO.sCali.f32Gain = 1.0f;
    sCLA.sVO.sCali.f32Offset = 0.0f;

    sCLA.sIO.sLPF.f32K1 = IO_LPF_K1;
    sCLA.sIO.sCali.f32Gain = 1.0f;
    sCLA.sIO.sCali.f32Offset = 0.0f;

    sCLA.sIL.sLPF.f32K1 = IL_LPF_K1;
    sCLA.sIL.sCali.f32Gain = 1.0f;
    sCLA.sIL.sCali.f32Offset = 0.0f;

    sCLA.sIEL.sLPF.f32K1 = IL_LPF_K1;
    sCLA.sIEL.sCali.f32Gain = 1.0f;
    sCLA.sIEL.sCali.f32Offset = 0.0f;

    sCLA.sVIN.sCali.f32Base = VIN_SCALE;
    sCLA.sVO.sCali.f32Base = VOUT_SCALE;
    sCLA.sIO.sCali.f32Base = IO_SCALE;
    sCLA.sIL.sCali.f32Base = IL_SCALE;
    sCLA.sIEL.sCali.f32Base = IEL_SCALE;


    sCLA.sTempC.sIIR.f32K1 = 0.00001f;

    sCLA.sLoopV.f32Ki = _V_LOOP_KI;
    sCLA.sLoopV.f32Kp = _V_LOOP_KP;
    sCLA.sLoopV.f32Min = -1.0f;
    sCLA.sLoopV.f32Max = 1.0f;
    sCLA.sLoopV.f32Itemp = 0.0f;
    sCLA.sLoopV.u16StopUi = false;

    sCLA.sLoopI.f32Ki = _I_LOOP_KI;
    sCLA.sLoopI.f32Kp = _I_LOOP_KP;
    sCLA.sLoopI.f32Min = 0.0f;
    sCLA.sLoopI.f32Max = 1.0f;
    sCLA.sLoopI.f32Itemp = 0.0f;
    sCLA.sLoopI.f32Out = 0.0f;
    sCLA.sLoopI.u16StopUi = false;

    sCLA.f32IrefLimit = RATED_IOUT_PU;

    sCLA.sPWM.u32PwmBase = EPWM1_BASE;
    sCLA.sPWM.u16CtrlReg = _PWM_SOURCE_SINK_MODE;
    sCLA.sPWM.u16PwmA = 0;
    sCLA.sPWM.u16PwmB = 0;


    //End of initClaCpu
    FG_SETCLA(_CSTAT_INIT_CLA_PARAM);
}
//
// CLA Initialization
//
// Description: This function will
// - copy over code and const from flash to CLA program and data ram
//   respectively
// - Initialize the task vectors (MVECTx)
// - setup each task's trigger
// - enable each individual task
// - map program and data spaces to the CLA
// - run any one-time initialization task
// Please note that the CLA can only run code and access data that is in RAM.
// the user must assign constants (tables) to FLASH, and copy them over to
// RAM at run-time. They must be copied to a RAM that lies in the address space
// of the CLA, and prior to giving the CLA control over that space
//
void initCLA(void)
{
    //
    // Copy the program and constants from FLASH to RAM before configuring
    // the CLA
    //
#if defined(_FLASH)
    memcpy((uint32_t *)&Cla1ProgRunStart, (uint32_t *)&Cla1ProgLoadStart,
           (uint32_t)&Cla1ProgLoadSize);
    memcpy((uint32_t *)&Cla1ConstRunStart, (uint32_t *)&Cla1ConstLoadStart,
        (uint32_t)&Cla1ConstLoadSize );
#endif //defined(_FLASH)

    //
    // CLA Program will reside in RAMLS0/1/2 and data in RAMLS3/4/5
    //
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS0, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS1, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS2, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS3, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS4, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS5, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS0, MEMCFG_CLA_MEM_PROGRAM);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS1, MEMCFG_CLA_MEM_PROGRAM);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS2, MEMCFG_CLA_MEM_PROGRAM);
#if defined(_FLASH)
    //
    // In Flash config, constants are loaded in Flash and then copied to LS3/4/5
    // CLA reads the constants from LS3/4/5
    //
    MemCfg_setCLAMemType(MEMCFG_SECT_LS3, MEMCFG_CLA_MEM_DATA);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS4, MEMCFG_CLA_MEM_DATA);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS5, MEMCFG_CLA_MEM_DATA);

#endif //defined(_FLASH)

//
// Suppressing #770-D conversion from pointer to smaller integer
// The CLA address range is 16 bits so the addresses passed to the MVECT
// registers will be in the lower 64KW address space. Turn the warning
// back on after the MVECTs are assigned addresses
//
#pragma diag_suppress=770

    //
    // Assign the task vectors and set the triggers for task 1
    //
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_1, (uint16_t)&Cla1Task1);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_2, (uint16_t)&Cla1Task2);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_3, (uint16_t)&Cla1Task3);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_4, (uint16_t)&Cla1Task4);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_5, (uint16_t)&Cla1Task5);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_6, (uint16_t)&Cla1Task6);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_7, (uint16_t)&Cla1Task7);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_8, (uint16_t)&Cla1Task8);

    CLA_setTriggerSource(CLA_TASK_1, CLA_TRIGGER_ADCA1);

#pragma diag_warning=770

    //
    // Enable Task 1
    //
    CLA_enableTasks(CLA1_BASE, CLA_TASKFLAG_1);

    initClaCtrlParam();
}
