//###########################################################################
//
// FILE:    Protect_Module.h
//
// TITLE:
//
//###########################################################################
// $Create Date: 20211015
//###########################################################################

//#define DCAC_OTP_LEVEL 737
// MCP3301 0~0.9~5V <=> 0~737~4096 <=>溫度 0~90~500度
#define DCAC_OTP_LEVEL 90.0



#define POWER_UP_FANFAIL1_ERROR_TIME    2000 //(ms)
#define POWER_UP_FANFAIL2_ERROR_TIME    2000 //(ms)
#define POWER_UP_PFC_UVP_ERROR_TIME     2000 //(ms)
#define POWER_UP_PFC_OVP_ERROR_TIME     2000 //(ms)
#define POWER_UP_PFC_OTP_ERROR_TIME     2000 //(ms)
#define POWER_UP_ACFAIL_ERROR_TIME      2000 //(ms)
#define POWER_UP_DCAC_OTP_ERROR_TIME    2000 //(ms)
#define POWER_UP_FANFAIL_ERROR_TIME     2000 //(ms) //for 全寬機種
#define POWER_UP_PFC_UVP1_ERROR_TIME    2000 //(ms) //for 全寬機種
#define POWER_UP_PFC_OVP1_ERROR_TIME    2000 //(ms) //for 全寬機種
#define POWER_UP_PFC_UVP2_ERROR_TIME    2000 //(ms) //for 全寬機種
#define POWER_UP_PFC_OVP2_ERROR_TIME    2000 //(ms) //for 全寬機種
#define POWER_UP_DC_OVP_ERROR_TIME      2000 //(ms)



#define POWER_READY_FANFAIL1_ERROR_TIME    1000 //(ms)
#define POWER_READY_FANFAIL2_ERROR_TIME    1000 //(ms)
#define POWER_READY_PFC_UVP_ERROR_TIME     1000 //(ms)
#define POWER_READY_PFC_OVP_ERROR_TIME     1000 //(ms)
#define POWER_READY_PFC_OTP_ERROR_TIME     1000 //(ms)
#define POWER_READY_ACFAIL_ERROR_TIME      100 //(ms)
#define POWER_READY_DCAC_OTP_ERROR_TIME    1000 //(ms)
#define POWER_READY_FANFAIL_ERROR_TIME     1000 //(ms) //for 全寬機種
#define POWER_READY_PFC_UVP1_ERROR_TIME    1000 //(ms) //for 全寬機種
#define POWER_READY_PFC_OVP1_ERROR_TIME    1000 //(ms) //for 全寬機種
#define POWER_READY_PFC_UVP2_ERROR_TIME    1000 //(ms) //for 全寬機種
#define POWER_READY_PFC_OVP2_ERROR_TIME    1000 //(ms) //for 全寬機種
#define POWER_READY_DC_OVP_ERROR_TIME      100 //(ms)


#define POWER_READY_IPEAK_OCP_ERROR_TIME   3600 //(2.775us)


#define POWER_READY_IRMS_OCP_ERROR_TIME    250 //(ms)

#define POWER_READY_SHORT_ERROR_TIME       8 //(us)
// cpu timer0 2.78us
// 22.2us/2.78us=8

#define POWER_READY_OVP_ERROR_TIME         0 //(ms)
#define POWER_READY_OPP_ERROR_TIME         250 //(ms)

#define POWER_READY_REMOTE_OVP_ERROR_TIME  0 //(ms)



void Power_Up_Error_Scan(void);
void FANFAIL1_start_time_reset(void);
void FANFAIL2_start_time_reset(void);
void PFC_UVP_start_time_reset(void);
void PFC_OVP_start_time_reset(void);
void PFC_OTP_start_time_reset(void);
void ACFAIL_start_time_reset(void);
void DCAC_OTP_start_time_reset(void);
void p_I_peak_ocp_start_time_reset(void);
void n_I_peak_ocp_start_time_reset(void);
//void p_I_short_ocp_start_time_reset(void);
//void n_I_short_ocp_start_time_reset(void);
void I_rms_ocp_start_time_reset(void);
void V_rms_ovp_start_time_reset(void);
void opp_start_time_reset(void);
void remote_OVP_start_time_reset(void);

void FANFAIL_start_time_reset(void); //for 全寬機種
void PFC_UVP1_start_time_reset(void); //for 全寬機種
void PFC_OVP1_start_time_reset(void); //for 全寬機種
void PFC_UVP2_start_time_reset(void); //for 全寬機種
void PFC_OVP2_start_time_reset(void); //for 全寬機種

void DC_OVP_start_time_reset(void);


void error_start_time_reset(void);


void Power_Ready_Error_Scan(void);


//
// End of file
//
