/*
 * isr.c
 *
 *  Created on: 2022�~3��15��
 *      Author: cody_chen
 */
#include <math.h>
#include "common.h"
#include "cla_task.h"

#define PWM_SAMPLE_RATE    BUCK_PWM_KHZ
#define ISR_100USEC        (uint32_t)((float)BUCK_PWM_KHZ*1000.0f*0.0001)
#define ISR_200USEC        (uint32_t)((float)BUCK_PWM_KHZ*1000.0f*0.0002)
#define ISR_500USEC        (uint32_t)((float)BUCK_PWM_KHZ*1000.0f*0.0005)
#define ISR_1MSEC          (uint32_t)((float)BUCK_PWM_KHZ*1000.0f*0.0010)

#ifdef _FLASH
#pragma SET_CODE_SECTION(".TI.ramfunc")
#endif //_FLASH

void debugDAC(HAL_DRV s)
{
//    DAC_UPU(sCLA.sVO.sLPF.f32Out, DACA_TP209_BASE);
    DAC_UPU(sDrv.sVref.f32Out, DACB_TP208_BASE);
    DAC_UPU(sDrv.sVref.f32Target, DACC_TP210_BASE);
#if FORCE_OUTPUT_PWM
    DAC_SPU( sCLA.f32TestPU, DACA_TP209_BASE);
#endif
}

void scanBrownInOut(HAL_DRV s)
{
#if FORCE_OUTPUT_PWM

#else
    if(false == FG_GETCLA(_CSTAT_BROWN_IN)) {
        if(VIN_BROWN_IN_PU < sCLA.sVIN.sLPF.f32Out) {
            sCLA.f32OpenGain = -1.0f;
            FG_SETCLA(_CSTAT_BROWN_IN);
        }
    }
    else {

        if(VIN_BROWN_OUT_PU > sCLA.sVIN.sLPF.f32Out) {
            sDrv.u16EnablePwm = false;
            FG_RSTCLA(_CSTAT_OUTPUT_READY);
        }
        else {
            if(s->sVref.f32Target != s->sVref.f32Out) {
                sCLA.f32OpenGain = s->sVref.f32Out / (sCLA.sVIN.sLPF.f32Out * VIN2VOUT_SCALE);
            }
            else {
                sCLA.f32OpenGain = sCLA.sVO.sLPF.f32Out / (sCLA.sVIN.sLPF.f32Out * VIN2VOUT_SCALE);
            }
        }
    }
#endif //#if FORCE_OUTPUT_PWM

    if(true == sDrv.u16EnablePwm) {
#if ENABLE_CYCLE_PROTECTION
        if(FG_ANDERR(_ALL_ERROR)) {
            FG_RSTCLA(_CSTAT_ENABLE_PWM);
        }
        else {
            FG_SETCLA(_CSTAT_ENABLE_PWM);
        }
#else
        FG_SETCLA(_CSTAT_ENABLE_PWM);
#endif //ENABLE_CYCLE_PROTECTION

    }
    else {
        FG_RSTCLA(_CSTAT_ENABLE_PWM);
    }

}

/*E-Load Control*/
void eLoadScanError(HAL_DRV s)
{
    static uint16_t Chk_UnReg , Chk_OTP;
    
    Chk_UnReg = RD_EL_UnReg();
    Chk_OTP = RD_EL_OTP();

    if(Chk_UnReg)
        {FG_SETELERR(_WARNING_EL_UnReg);}
    else
        {FG_RSTELERR(_WARNING_EL_UnReg);}
    
    if(Chk_OTP)
        {FG_SETELERR(_ERR_EL_OTP);}
    else
        {FG_RSTELERR(_ERR_EL_OTP);}

}
void eLoadControl(HAL_DRV s)
{
    eLoadScanError(s);
    if(true == s->u16EnableEL) 
    {
#if ENABLE_CYCLE_PROTECTION
        if(FG_ANDELERR(_ALL_EL_ERROR)) 
        {
            FG_RSTEL(_ELSTAT_ENABLE_EL);
            RST_ELOAD_ON();
        }
        else 
        {
            if(s->u16ELVonEnable)
            {
                if(sCLA.sVO.sCali.f32Out > sCLA.f32ELVon)
                {
                    FG_SETEL(_ELSTAT_ENABLE_EL);
                    SET_ELOAD_ON();
                }
            }
            else
            {
                FG_SETEL(_ELSTAT_ENABLE_EL);
                SET_ELOAD_ON();
            }
        }
#else
        if(s->u16ELVonEnable)
        {
            if(sCLA.sVO.sCali.f32Out > sCLA.f32ELVon)
            {
                FG_SETEL(_ELSTAT_ENABLE_EL);
                SET_ELOAD_ON();
            }
        }
        else
        {
            FG_SETEL(_ELSTAT_ENABLE_EL);
            SET_ELOAD_ON();
        }
#endif //ENABLE_CYCLE_PROTECTION

    }
    else 
    {
        FG_RSTEL(_ELSTAT_ENABLE_EL);
        RST_ELOAD_ON();
    }

    static uint16_t Chk_Eload_ON_Status;
    
    Chk_Eload_ON_Status = RD_EL_LOAD_ON();
    
    if(!Chk_Eload_ON_Status)
        {FG_SETEL(_ELSTAT_EL_LDON);}
    else
        {FG_RSTEL(_ELSTAT_EL_LDON);}

}
/*E-Load Control*/

void ctrlVrefRamp(HAL_DRV s)
{

#if FORCE_OUTPUT_PWM
    s->sVref.f32Target = s->f32RemoteVref;
    execRampCtrl((HAL_RAMPCTRL)&s->sVref);
#else
    if(false == FG_GETCLA(_CSTAT_OUTPUT_READY)) {
        s->sVref.f32Target = s->sVref.f32Out = sCLA.sVO.sLPF.f32Out;
    }
    else {
        s->sVref.f32Target = s->f32RemoteVref;
        execRampCtrl((HAL_RAMPCTRL)&s->sVref);
    }
#endif //FORCE_OUTPUT_PWM


    sCLA.f32Vref = s->sVref.f32Out;

}

void execCalibration(HAL_DRV s)
{
    if(true == FG_GETCLA(_CSTAT_CALIBRATION)) {
        sCLA.sVO.sCali.f32Offset = -VO_SENSE;

        sCLA.sVIN.sCali.f32Offset = - VIN_SENSE;

        sCLA.sIO.sCali.f32Offset = -IO_SENSE;

        sCLA.sIL.sCali.f32Offset = -IL_SENSE;

        sCLA.sIEL.sCali.f32Offset = -IEL_SENSE;

        FG_RSTCLA(_CSTAT_CALIBRATION);
    }
    sCLA.f32Vref = s->sVref.f32Out;

}


void isrTask1msec(HAL_DRV s)
{
    scanBrownInOut(s);
    eLoadControl(s);
    execCalibration(s);
}


void isrTask100usec(HAL_DRV s)
{
    ctrlVrefRamp(s);
}

typedef struct {
    void (*fn) (HAL_DRV s);
    uint32_t cnt;
    uint32_t max;
} ST_ISRTASK;

uint16_t id_isr_task = 0;
ST_ISRTASK isr_task[] = {
        {isrTask1msec,         0,   ISR_1MSEC},
        {isrTask100usec,       0,   ISR_100USEC},
        {0, 0, 0}
};


void isrTimeTask(ST_ISRTASK *t, HAL_DRV s)
{
    static uint32_t delta_t;

    s->u32IsrCnt++;
    s->u32IsrCnt &= 0x7FFFFFFF;

    if(t->cnt > s->u32IsrCnt) {
        delta_t = s->u32IsrCnt + 0x7FFFFFFF - t->cnt;
    }
    else {
        delta_t = s->u32IsrCnt - t->cnt;
    }

    if(delta_t >= t->max) {
        t->fn(s);
        t->cnt = s->u32IsrCnt;
    }
}

void C28_Measurement(HAL_DRV s) {
    sCLA.sVO.sLPF.f32Now = sCLA.sVO.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sVO.sLPF);
    s->f32VO = sCLA.sVO.sLPF.f32Out;

    sCLA.sVIN.sLPF.f32Now = sCLA.sVIN.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sVIN.sLPF);
    s->f32VIN = sCLA.sVIN.sLPF.f32Out;

    sCLA.sIO.sLPF.f32Now = sCLA.sIO.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sIO.sLPF);
    s->f32IO = sCLA.sIO.sLPF.f32Out;

    sCLA.sIL.sLPF.f32Now = sCLA.sIL.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sIL.sLPF);
    s->f32IL = sCLA.sIL.sLPF.f32Out;

    sCLA.sIEL.sLPF.f32Now = sCLA.sIEL.sCali.f32Out;
    mLPF((HAL_LPF)&sCLA.sIEL.sLPF);
    s->f32IEL = sCLA.sIEL.sLPF.f32Out;

}

void DAC_WriteData(HAL_DRV s) {

    s->sDAC.u16Dac_PutoCnt = DAC_MAX_CNT * csatPosPU(s->f32RemoteIELref);
    
    if(SPI_FIFO_TXEMPTY == SPI_getTxFIFOStatus(DAC8812_BASE) && s->sDAC.u16Dac_FgnLDAC == true) 
    {
        RST_DAC_nLDAC();
        __asm(" RPT #5 || NOP");
        SET_DAC_nLDAC();
        s->sDAC.u16Dac_FgnLDAC = false;
    }
    
    if(s->u16DacCh == DAC_CH_NONE)  {return;}
    else if(s->u16DacCh != DAC_CH_NONE && s->sDAC.u16Dac_PutoCnt <= DAC_MAX_CNT) 
    {
        s->sDAC.SPI_FORMAT.Bits.Address =  s->u16DacCh;
        s->sDAC.SPI_FORMAT.Bits.Data = s->sDAC.u16Dac_PutoCnt;
        
        switch (s->sDAC.SPI_FORMAT.Bits.Address)
        {
            case DAC_A_CH:
                if(s->sDAC.u16Dac_Data_ACh_Chk != s->sDAC.SPI_FORMAT.Bits.Data && s->sDAC.u16Dac_FgnLDAC == false) 
                {
                    SPI_writeDataBlockingFIFO(DAC8812_BASE, (((uint16_t)s->sDAC.SPI_FORMAT.FiFo.HIData) << 7));
                    SPI_writeDataBlockingFIFO(DAC8812_BASE, (((uint16_t)s->sDAC.SPI_FORMAT.FiFo.LOData) << 7));
                    s->sDAC.u16Dac_FgnLDAC = true;
                    s->sDAC.u16Dac_Chl_Chk = (uint16_t)s->sDAC.SPI_FORMAT.Bits.Address;
                    s->sDAC.u16Dac_Data_ACh_Chk = (uint16_t)s->sDAC.SPI_FORMAT.Bits.Data;
                }
                break;
            case DAC_B_CH:
                if (s->sDAC.u16Dac_Data_BCh_Chk != s->sDAC.SPI_FORMAT.Bits.Data && s->sDAC.u16Dac_FgnLDAC == false)
                {
                    SPI_writeDataBlockingFIFO(DAC8812_BASE, (((uint16_t)s->sDAC.SPI_FORMAT.FiFo.HIData) << 7));
                    SPI_writeDataBlockingFIFO(DAC8812_BASE, (((uint16_t)s->sDAC.SPI_FORMAT.FiFo.LOData) << 7));
                    s->sDAC.u16Dac_FgnLDAC = true;
                    s->sDAC.u16Dac_Chl_Chk = (uint16_t)s->sDAC.SPI_FORMAT.Bits.Address;
                    s->sDAC.u16Dac_Data_BCh_Chk = (uint16_t)s->sDAC.SPI_FORMAT.Bits.Data;
                }
                break;
            case DAC_ALL_CH:
 #if    DAC8812_CYCLE_WRITE
                if (s->sDAC.u16Dac_FgnLDAC == false)
 #else 
                if (s->sDAC.u16Dac_Data_ALLCh_Chk != s->sDAC.SPI_FORMAT.Bits.Data && s->sDAC.u16Dac_FgnLDAC == false)
 #endif // endif DAC8812_CYCLE_WRITE
                {
                    SPI_writeDataBlockingFIFO(DAC8812_BASE, (((uint16_t)s->sDAC.SPI_FORMAT.FiFo.HIData) << 7));
                    SPI_writeDataBlockingFIFO(DAC8812_BASE, (((uint16_t)s->sDAC.SPI_FORMAT.FiFo.LOData) << 7));
                    s->sDAC.u16Dac_FgnLDAC = true;
                    s->sDAC.u16Dac_Chl_Chk = (uint16_t)s->sDAC.SPI_FORMAT.Bits.Address;
                    s->sDAC.u16Dac_Data_ACh_Chk = s->sDAC.u16Dac_Data_BCh_Chk = (uint16_t)s->sDAC.SPI_FORMAT.Bits.Data;
                    s->sDAC.u16Dac_Data_ALLCh_Chk = (uint16_t)s->sDAC.SPI_FORMAT.Bits.Data;
                }
                break;
            default:
                break;
        }
    }
}


__interrupt void INT_ADC_EOC_ISR (void)
{
    if(FG_GETCLA(_CSTAT_TASK1_OK)) {
        SET_DEBUG_ADC_EOC();

        isrTimeTask(&isr_task[id_isr_task++], (HAL_DRV)&sDrv);
        if(0 == isr_task[id_isr_task].fn) id_isr_task = 0;

#if FORCE_OUTPUT_PWM
    #if (TEST_MODE == TEST_HRPWM_RANGE)
        execRampUpDwon((HAL_RAMPUPDN)&sDrv.sTest);
        sCLA.f32TestPU = sDrv.sTest.f32Out;

    #elif (TEST_MODE == TEST_HRPWM_MINSTEP)
        sCLA.f32TestPU = sDrv.sTest.f32Out;
    #else

    #endif
#endif //FORCE_OUTPUT_PWM

        C28_Measurement((HAL_DRV)&sDrv);

        DAC_WriteData((HAL_DRV)&sDrv);

        debugDAC((HAL_DRV)&sDrv);

        sDrv.u32HeartBeat++;
        sDrv.u32HeartBeat &= 0x7FFFFFFF;

        FG_SETCLA(_CSTAT_PWMADC_OK)
    }

    // Clear the interrupt flag
    ADC_clearInterruptStatus(AD_A_BASE, ADC_INT_NUMBER1);

    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INT_AD_A_1_INTERRUPT_ACK_GROUP);

    RST_DEBUG_ADC_EOC();
}

#ifdef _FLASH
#pragma SET_CODE_SECTION()
#endif //_FLASH
