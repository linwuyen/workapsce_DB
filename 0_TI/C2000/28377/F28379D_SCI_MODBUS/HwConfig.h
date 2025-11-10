/*
 * HwConfig.h
 *
 *  Created on: 2022¦~4¤ë20¤é
 *      Author: cody_chen
 */

#ifndef HWCONFIG_H_
#define HWCONFIG_H_

//! For 28377S
//! The equation for the output clock is:
//!   TBCLK = EPWMCLK/(highSpeedPrescaler * pre-scaler)
//! \b Note: EPWMCLK is a scaled version of SYSCLK. At reset EPWMCLK is half
//!          SYSCLK.
//! UP/DOWM MODE:= TPWM= (TBPRD + 1) x TBCLK
//! UPDOWM MODE := TPWM= 2 x TBPRD x TBCLK

#define OSCSRC_FREQ                      20000000   //DEVICE_OSCSRC_FREQ

#define EPWMCLK_FREQ                     100000000  //((OSCSRC_FREQ * 20 * 1) / 2 / 2)

#define BUCK_PWM_KHZ                     100UL    // (kHz)
#define CNT_BUCK_PWM                    (uint16_t)(((uint32_t)EPWMCLK_FREQ/1000/BUCK_PWM_KHZ)>>1)
#define CNT_BUCK_HRPWM                  (uint32_t)((((uint32_t)EPWMCLK_FREQ/1000/BUCK_PWM_KHZ)>>1)<<8)

#define T_BUCK_PWM_DEADBAND_NSEC         100UL    // (nsec)
#define CNT_BUCK_PWM_DEADBAND           (uint16_t)(EPWMCLK_FREQ/1000*T_BUCK_PWM_DEADBAND_NSEC/1000000)
#define CNT_BUCK_HRPWM_DEADBAND         (uint32_t)((EPWMCLK_FREQ/1000*T_BUCK_PWM_DEADBAND_NSEC/1000000)<<8)

#define VIN_SCALE                        13.3f    // [Vpeak]
#define IO_SCALE                         7.517f   // [Apeak]
#define IL_SCALE                         7.517f   // [Apeak]
#define IEL_SCALE                        7.517f   // [Apeak]
#define VOUT_SCALE                       6.7f     // [Vpeak]

#define V2I_SCALE                        (VOUT_SCALE/IL_SCALE)
#define VIN2VOUT_SCALE                   (VIN_SCALE/VOUT_SCALE)

#define VREF_TARGET                      5.0f     // [V]
#define VREF_PU                          (VREF_TARGET/VOUT_SCALE)
#define VREF_RAMP_T_SEC                  0.01f    // [sec]
#define VREF_RAMP_SAMPLE_HZ              10000.0f // [Hz]
#define VREF_RAMP_STEP                   (1.0/(VREF_RAMP_T_SEC*VREF_RAMP_SAMPLE_HZ))

#define RATED_POWER                      5.0f     // [W]
#define MAX_POWER_TIMES                  1.2      // [Ratio]
#define RATED_IOUT_PU                    (RATED_POWER*MAX_POWER_TIMES/VREF_TARGET/IO_SCALE)

#define VIN_BROWN_IN                     8.0f     // [Vrms]
#define VIN_BROWN_OUT                    4.0f     // [Vrms]
#define VIN_BROWN_IN_PU                 (VIN_BROWN_IN/VIN_SCALE)
#define VIN_BROWN_OUT_PU                (VIN_BROWN_OUT/VIN_SCALE)

//f32k1 = 1.0F / (1.0F + ((2PI() * fc) / sample_rate_hz));
#define LPF_FC(fc_khz, sample_rate_khz)  1.0f/(1.0f+(_2PI() * fc_khz / sample_rate_khz))
#define VIN_LPF_K1                       LPF_FC(25.0f, 100.0f)
#define VO_LPF_K1                        LPF_FC(25.0f, 100.0f)
#define IO_LPF_K1                        LPF_FC(25.0f, 100.0f)
#define IL_LPF_K1                        LPF_FC(25.0f, 100.0f)
#define IEL_LPF_K1                       LPF_FC(25.0f, 100.0f)


// @ ADC_RESULT_Xy(X, y)
// X:= ADCA/B/C, y:= SOC0-15
#define ADC_12BITS_RESULT_Xy(X, y)            (((float)HWREGH(ADC ## X ##RESULT_BASE + y))*0.0002442f)
#define ADC_16BITS_RESULT_Xy(X, y)            (((float)HWREGH(ADC ## X ##RESULT_BASE + y))*0.0000305f-1.0f)
#define ADC_RESULT_Xy(X,y)                    ADC_12BITS_RESULT_Xy(X, y)

#define A0_IL_SENSE                    ADC_RESULT_Xy(A, 0)
#define A1_IL_SENSE                    ADC_RESULT_Xy(A, 1)
#define A2_IL_SENSE                    ADC_RESULT_Xy(A, 2)
#define A3_IL_SENSE                    ADC_RESULT_Xy(A, 3)

#define B0_VIN_SENSE                   ADC_RESULT_Xy(B, 0)
#define B1_VIN_SENSE                   ADC_RESULT_Xy(B, 1)
#define B2_VIN_SENSE                   ADC_RESULT_Xy(B, 2)
#define B3_VIN_SENSE                   ADC_RESULT_Xy(B, 3)

#define C0_VO_SENSE                    ADC_RESULT_Xy(C, 0)
#define C1_VO_SENSE                    ADC_RESULT_Xy(C, 1)
#define C2_IO_SENSE                    ADC_RESULT_Xy(C, 2)
#define C3_IO_SENSE                    ADC_RESULT_Xy(C, 3)

#define IL_SENSE                       ((A0_IL_SENSE+A1_IL_SENSE+A2_IL_SENSE+A3_IL_SENSE)*0.25f)
#define IO_SENSE                       ((C2_IO_SENSE+C3_IO_SENSE)*0.5f)
#define VO_SENSE                       ((C0_VO_SENSE+C1_VO_SENSE)*0.5f)
#define VIN_SENSE                      ((B0_VIN_SENSE+B1_VIN_SENSE+B2_VIN_SENSE+B3_VIN_SENSE)*0.25f)
#define IEL_SENSE                      0.0f //((D0_IN2P3N_IEL_SENSE+D2_IN2P3N_IEL_SENSE)*0.5f)


#define _V_LOOP_KP                     (0.8f)
#define _V_LOOP_KI                     (0.00001f)

#define _I_LOOP_KP                     (0.8f)
#define _I_LOOP_KI                     (0.00001f)

#define ENABLE_DEBUG_DAC               1
#define FORCE_OUTPUT_PWM               0

#define TEST_HRPWM_MINSTEP             1
#define TEST_HRPWM_RANGE               2
#define TEST_MODE                      TEST_HRPWM_RANGE

#define V_LOOP                         1
#define I_LOOP                         2
#define I_FWD                          3
#define OPEN_LOOP                      4
#define IEL_LOOP                       5
#define LOOP_MODE                      (OPEN_LOOP+I_FWD+I_LOOP+V_LOOP)


#include "cdebug.h"

#endif /* HWCONFIG_H_ */
