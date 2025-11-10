/*
 * initCLK.h
 *
 *  Created on: May 27, 2024
 *      Author: roger_lin
 */

#ifndef SPWM_CTRL_H_
#define SPWM_CTRL_H_
typedef union {
    float32_t f32Data;
    uint32_t u32Data;
    uint16_t u16Data[2];

    struct {
        uint32_t b0_pfc_ovp_n:1;
        uint32_t b1_pfc_uvp_n:1;
        uint32_t b2_pfc_ovp_2_n:1;
        uint32_t b3_pfc_uvp_2_n:1;
        uint32_t b4_pfc_otp_n:1;
        uint32_t b5_dc_ovp_n:1;
        uint32_t b6_ac_fail_5_n:1;
        uint32_t b7_fan_fail_1:1;
        uint32_t b8_fan_fail_2:1;
        uint32_t b9_fan_fail_3:1;
        uint32_t b10_fan_fail_4:1;
        uint32_t b11_15_reserved:5;
        uint32_t b16_off_pwm_n:1;
        uint32_t b17_master:1;
        uint32_t b18_31_reserved:14;
    } i_stat;

    struct {
        uint32_t b0_15_reserved:16;
        uint32_t b16_off_pwm_n:1;
        uint32_t b17_master:1;
        uint32_t b18_31_reserved:14;
    } o_stat;
} REG_PARAM;



// Define a struct containing three register parameters
typedef struct {
    REG_PARAM sDin;
    REG_PARAM sDout;
    REG_PARAM sFanCtrl;
    REG_PARAM sIref_P;
    REG_PARAM sIref_N;
    REG_PARAM sTemp_C;
    REG_PARAM sHeartbeats;
} ST_PARAMS;

// Macro to calculate the offset of a member in the struct
#define INIT_PARAM(name) (uintptr_t)(REG_PARAM*)(&((ST_PARAMS*)(0))->name)/sizeof(REG_PARAM)

// Define enum with calculated offsets
typedef enum {
    _ID_DIN      = INIT_PARAM(sDin),
    _ID_DOUT     = INIT_PARAM(sDout),
    _ID_FAN_CTRL = INIT_PARAM(sFanCtrl),
    _ID_IREF_P   = INIT_PARAM(sIref_P),
    _ID_IREF_N   = INIT_PARAM(sIref_N),
    _ID_TEMP_C   = INIT_PARAM(sTemp_C),
    _END_OF_IDCMD
} ID_CMD;

extern  ST_PARAMS sParams;

//timetask.c- task10msec
extern void Update_IO(void);

extern void init_EPWM(void);

extern void updateFANCTRL5Duty( void);

#endif /* SPWM_CTRL_H_ */
