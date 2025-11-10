/*
 * timetask.c
 *
 *  Created on: May 17, 2024
 *      Author: User
 */

#include "common.h"
#include "./cla/initCLA.h"
#include "shareram.h"
#include "./cb_slave/cbSlave.h"
#include "./cb_slave/cbMaster.h"
#include "./Ib_slave/IbSlavePar.h"

void recalParameters(void)
{
    sIbSlave.sMirror.sConfigMaster.u16UnlockOpFunc.all = 65535;

    sDrv.f32TargetHwOvp = sHwDerive.f32VoutOvpPu;
    VO_OVP_CMPDAC(sDrv.f32DacHwOvp, sDrv.f32TargetHwOvp);
    sDrv.f32RemoteHwOvp = sDrv.f32TargetHwOvp;

    sDrv.f32RemoteHwSourceOcp = sDrv.f32TargetHwSourceOcp = sHwDerive.f32IoutOcpPu;
    IO_OCP_SOURCE_CMPDAC(sDrv.f32DacHwSourceOcp, sDrv.f32RemoteHwSourceOcp);
    sDrv.f32RemoteHwSinkOcp = sDrv.f32TargetHwSinkOcp = sHwDerive.f32IoutOcpPu;
    IO_OCP_SINK_CMPDAC(sDrv.f32DacHwSinkOcp, sDrv.f32RemoteHwSinkOcp);

    sDrv.f32TargetHwVinOvp = sHwDerive.f32VinOvpPu;
    VIN_OVP_CMPDAC(sDrv.f32DacHwVinOvp, sDrv.f32TargetHwVinOvp);
    sDrv.f32RemoteHwVinOvp = sDrv.f32TargetHwVinOvp;
}

void pollBleederParam(void);
void pollOcpParam(void);
void pollOvpParam(void);
void pollVinOvpParam(void);
void pollSlowError(void);
void pollUpdateParamCLA(void);
void (*pollParams)(void) = pollOvpParam;


void pollBleederParam(void)
{
    if(FG_GET(_CSTAT_BLEEDER_OFF, sDrv.fgStatus)) {
        if(0 == sDrv.u32BleederOff) {
            FG_RST(_CSTAT_BLEEDER_OFF, sDrv.fgStatus);
        }
    }
    else {
        if(1 == sDrv.u32BleederOff) {
            FG_SET(_CSTAT_BLEEDER_OFF, sDrv.fgStatus);
        }
    }

    pollParams = pollOcpParam;
}

void pollOcpParam(void)
{
    if(sDrv.f32TargetHwSourceOcp != sDrv.f32RemoteHwSourceOcp) {
        sDrv.f32TargetHwSourceOcp = sDrv.f32RemoteHwSourceOcp;
        IO_OCP_SOURCE_CMPDAC(sDrv.f32DacHwSourceOcp, sDrv.f32TargetHwSourceOcp);
    }

    if(sDrv.f32TargetHwSinkOcp != sDrv.f32RemoteHwSinkOcp) {
        sDrv.f32TargetHwSinkOcp = sDrv.f32RemoteHwSinkOcp;
        IO_OCP_SINK_CMPDAC(sDrv.f32DacHwSinkOcp, sDrv.f32TargetHwSinkOcp);
    }
    pollParams = pollOvpParam;
}

void pollOvpParam(void)
{
    if(sDrv.f32TargetHwOvp != sDrv.f32RemoteHwOvp) {
        sDrv.f32TargetHwOvp = sDrv.f32RemoteHwOvp;
        VO_OVP_CMPDAC(sDrv.f32DacHwOvp, sDrv.f32TargetHwOvp);
    }

    pollParams = pollVinOvpParam;
}


void pollVinOvpParam(void)
{
    if(sDrv.f32TargetHwVinOvp != sDrv.f32RemoteHwVinOvp) {
        sDrv.f32TargetHwVinOvp = sDrv.f32RemoteHwVinOvp;
        VIN_OVP_CMPDAC(sDrv.f32DacHwVinOvp, sDrv.f32TargetHwVinOvp);
    }
    pollParams = pollSlowError;
}


void pollSlowError(void)
{
    if(1 != sIbSlave.sMirror.u16GDstat.b01_Output) {
        SET_ERR(_ERR_M0_STAT, sDrv);
    }
    else {
        RST_ERR(_ERR_M0_STAT, sDrv);
    }

    if(T_ISR_MARGIN_USEC < sDrv.tpIsrCost.f32MaxElapsedUsec) {
        SET_ERR(_ERR_ISR_MARGIN, sDrv);
    }

    pollParams = pollUpdateParamCLA;
}

void pollUpdateParamCLA(void)
{
    sCLA.f32VoutOffset = sDrv.sVO.sCali.f32Offset;
    sCLA.f32VoutGain =  sDrv.sVO.sCali.f32Gain;

    sCLA.f32VinOffset = sDrv.sVIN.sCali.f32Offset;
    sCLA.f32VinGain =  sDrv.sVIN.sCali.f32Gain;

    sCLA.f32IoutOffset = sDrv.sIO.sCali.f32Offset;
    sCLA.f32IoutGain =  sDrv.sIO.sCali.f32Gain;

    pollParams = pollBleederParam;
}

void ctrlByCANbus(void)
{
    switch(sCbSlave.u32CtrlMode) {
    case _CONTROL_BY_STANDALONE:
        sDrv.f32InputVref = sDrv.f32RemoteVref;
        sDrv.regPwm.u16Enable.all = sDrv.regPwm.u16Remote.all;
        FG_RST(_CSTAT_CANBUS_READY, sDrv.fgStatus);

        if(getCbSlaveSN()) {
            FG_SWTO(_WAIT_FOR_ENABLE_SYNCOUT, sCbSlave.u32CtrlMode);
        }
        break;

    case _WAIT_FOR_ENABLE_SYNCOUT:
        if(sDrv.u32SN.all != getCbSlaveSN()) {
            ENABLE_SYNCOUT();
            FG_SET(_CSTAT_CANBUS_MASTER, sDrv.fgStatus);
        }
        else {
            DISABLE_SYNCOUT();
            FG_RST(_CSTAT_CANBUS_MASTER, sDrv.fgStatus);
        }

         FG_SWTO(_WAIT_FOR_ENABLE_ECAP, sCbSlave.u32CtrlMode);
        break;

    case _WAIT_FOR_ENABLE_ECAP:
        if(initEcapForVIshare()) {
            if(FG_GET(_CSTAT_CANBUS_MASTER, sDrv.fgStatus)) {
                sCLA.u32Remote.b00_Master = 1;
                FG_SWTO(_CONTROL_BY_CAN_MASTER, sCbSlave.u32CtrlMode);
            }
            else {
                sCLA.u32Remote.b16_Slave = 1;
                FG_SWTO(_CONTROL_BY_CAN_SLAVE, sCbSlave.u32CtrlMode);
            }

            FG_SET(_CSTAT_CANBUS_READY, sDrv.fgStatus);
        }
        break;

    case _CONTROL_BY_CAN_MASTER:

        break;

    case _CONTROL_BY_CAN_SLAVE:

        break;

    default:
        FG_SET(_MARK_ERROR_OF_CAN_MODE, sCbSlave.u32CtrlMode);
        break;

    }
}



void task25msec(void * s)
{
    pollParams();

    if(isCallbackReady()) {
        if(GETn_STAT(_CSTAT_INIT_PARAMS, sDrv)) {
            initHwConfigAndDrvParams();
            recalParameters();

            sAccessCPU1.f32Cpu1VinScale = sHwConfig.f32VoutScale;
            sAccessCPU1.f32Cpu1IoutScale = sHwConfig.f32IoutScale;

            SET_STAT(_CSTAT_INIT_PARAMS, sDrv);
        }
    }

    if(0 != sDrv.u32RemoteReadWriteCnt) {
        readDataFromToFlash(sDrv.u32RemoteReadWriteCnt);
        sDrv.u32RemoteReadWriteCnt = 0;
    }

    if((0 != sDrv.u32HeartBeat)&&(0 != sCLA.u32HeartBeat)) {
        SET_STAT(_CSTAT_THREAD_READY, sDrv);
    }

    if(_VISHARE_CALI_READY == sCLA.fsm_vishare) {
        SET_STAT(_CSTAT_CLA_READY, sDrv);
    }



    runRemoteCali();
}

void task2D5msec(void * s)
{
#if (TEST_TIMETASK_STABLE == TEST_MODE)
        GPIO_togglePin(CPU1_LED852_GPIO84);
#endif //TEST_TIMETASK_STABLE

    if(GET_STAT(_CSTAT_INIT_SUCCESS, sDrv)) {
        scanWarning();
    }

    measTimerLength(&sDrv.tpTaskLength);
}

void asapTask(void *s)
{
    runDebug();

    runManualFlashApi();

    if(FG_GETn(_CSTAT_OUTPUT_ON, sDrv.fgStatus)) {
        // Operate any flash action need to wrok under No PWM.
        runFlashStorage();
    }



    exeCANbusSlave();
    exeCANbusMaster();

    runIbSlaveMain();

    ctrlByCANbus();
}

ST_TIMETASK time_task[] = {
        {task2D5msec,         0,   T_2D5MS},
        {task25msec,          0,   T_25MS},
        {asapTask,            0,        0},
        {0, 0, 0}
};


void pollTimeTask(void)
{
    scanTimeTask(time_task, (void *)0);
}
