/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * $Revision: 2 $
 * $Date: 20/06/30 2:29p $
 * @brief
 *           Transmit and receive data from PC terminal through RS232 interface.
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "stdio.h"
#include "NuMicro.h"

#include "common.h"
#include "g_timer.h"
#include "g_uart.h"
#include "i2c_eeprom.h"
#include "emu_eeprom.h"
#include "tst.h"
#include "g_uart.h"
#include "mb_slave/ModbusSlave.h"
#include "usci_i2c.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t startcnt;
uint32_t writesuccess = 0;
void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int intToStr(uint32_t num, char str[]) {
    int i = 0;
    do {
        str[i++] = (num % 10) + '0';
        num = num / 10;
    } while (num);

    str[i] = '\0';  // Null-terminate string
    reverse(str, i);
    return i;
}

typedef enum {
    _EV_INIT_SHOW_VALUE          =  0x0000,
    _EV_PUSH_SHOW_VALUE_OUT      = (0x0001<<0),
    _EV_PUSH_RETURN_CHAR_OUT     = (0x0001<<1),
    _EV_PUSH_PROGRAM_EEPROM_FLAG = (0x0001<<2),
    _EV_VERIFY_EEPROM            = (0x0001<<3),
    _MASK_EERPROM_ERROR          =  0x8000
} FSM_EEPROM;

typedef struct {
    FSM_EEPROM fsm;

    uint32_t   u32SuccessCnt ;

    uint16_t   len;
    uint16_t   iTx;
    char       buffer[20]; // Make sure the buffer is large enough
} UART_CALBACK;
/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
typedef UART_CALBACK *HAL_UART_CALBACK;
UART_CALBACK UARTEmuEE;

void UARTEEPROM(HAL_UART_CALBACK p){
    switch(p->fsm){
            case _EV_INIT_SHOW_VALUE:
                p->len = intToStr(writesuccess, p->buffer);
                p->iTx = 0;
                p->fsm = _EV_PUSH_SHOW_VALUE_OUT;
            break;

            case _EV_PUSH_SHOW_VALUE_OUT:
                if(p->iTx < p->len){
                    while(0 == isTxFull(UART0)) {
											 DL_UART_transmitData(UART0, p->buffer[p->iTx++]);
                            if(p->iTx == p->len)break;
                            }
                           }else
                           {
                               p->fsm = _EV_PUSH_RETURN_CHAR_OUT;
                            }
            break;

            case _EV_PUSH_RETURN_CHAR_OUT:
                if(0 == isTxFull(UART0)) {
									DL_UART_transmitData(UART0,0xD);
                if(getEepromErrorStatus()) {
                    p->fsm = _EV_PUSH_PROGRAM_EEPROM_FLAG;
                }
                else {
                   p->fsm |= _MASK_EERPROM_ERROR;
                }
            }
            break;

            case _EV_PUSH_PROGRAM_EEPROM_FLAG:
								if(0<startcnt){
                    startcnt--;
                  if(1 == startcnt){
                    setProgramEmuEeprom();
//                          if(setProgramEmuEeprom()){
//                              p->fsm = _EV_VERIFY_EEPROM;
//                          }
//                              p->fsm |= _MASK_EERPROM_ERROR;
                      }
										}

            break;

            case _EV_VERIFY_EEPROM:
                setVerifyEmuEeprom();
                p->fsm =  _EV_INIT_SHOW_VALUE;
            break;

            case _MASK_EERPROM_ERROR:
               while(1){
               }
            
           
}
}	






void task25msec(void * s)
{


}

void task2D5msec(void * s)
{
		GPIO_TOGGLE(PC7);
}

typedef struct _ST_TIMETASK{
    void (*fn) (void * s);
    uint32_t cnt;
    uint32_t max;
} ST_TIMETASK;

uint16_t id_ttask = 0;
ST_TIMETASK time_task[] = {
        {task2D5msec,         0,   T_2D5MS},
        {task25msec,          0,   T_25MS},
        {0, 0, 0}
};

bool_t scanTimeTask(ST_TIMETASK *t, void *s)
{
    uint32_t delta_t;
    uint32_t u32Cnt = GET_TIMERCNT;

    if(t->cnt > u32Cnt) {
        delta_t = u32Cnt + SW_TIMER - t->cnt;
    }
    else {
        delta_t = u32Cnt - t->cnt;
    }

    if(delta_t >= t->max) {
        t->fn(s);
        t->cnt = u32Cnt;
        return true;
    }
    else {
        return false;
    }
}


void SYS_Init(void)
{

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
	
	  /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set both PCLK0 and PCLK1 as HCLK */
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1;
	
	  /* Enable ISP module clock */
    CLK_EnableModuleClock(ISP_MODULE);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART module clock source as HIRC and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
		
		/* Enable IP clock */
    CLK_EnableModuleClock(TMR0_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HIRC, 0);
		
		/* Enable UI2C0 clock */
    CLK_EnableModuleClock(USCI0_MODULE);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set PB multi-function pins for UART0 RXD=PB.6 and TXD=PB.4 */
    SYS->GPB_MFP1 = (SYS->GPB_MFP1 & ~(SYS_GPB_MFP1_PB4MFP_Msk | SYS_GPB_MFP1_PB6MFP_Msk)) |        \
                    (SYS_GPB_MFP1_PB4MFP_UART0_TXD | SYS_GPB_MFP1_PB6MFP_UART0_RXD);
										
	  /* Set UI2C0 multi-function pins */
    SYS->GPB_MFP1 = (SYS->GPB_MFP1 & ~(SYS_GPB_MFP1_PB5MFP_Msk | SYS_GPB_MFP1_PB7MFP_Msk)) |
                    (SYS_GPB_MFP1_PB5MFP_USCI0_DAT0 | SYS_GPB_MFP1_PB7MFP_USCI0_CLK);
										
		/* Lock protected registers */
    SYS_LockReg();
}

void UART0_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init UART                                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Reset UART0 */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    openUart(UART0, 115200);
}




/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();	
		
	  /* Lock protected registers */
    SYS_LockReg();

    /* Init UART0 for printf and test */
    UART0_Init();
	
	  /* Set timer frequency to 1HZ */
    openTimer(TIMER0, TIMER_PERIODIC_MODE, 1);
	
		GPIO_SetMode(PC, BIT7, GPIO_MODE_OUTPUT);
	


    /*---------------------------------------------------------------------------------------------------------*/
    /* SAMPLE CODE                                                                                             */
    /*---------------------------------------------------------------------------------------------------------*/

    while(1){

				scanTimeTask(&time_task[id_ttask++], (void *)0);
				if(0 == time_task[id_ttask].fn) id_ttask = 0;
			
				exeModbusSlave((SCI_MODBUS *)&mbcomm);
			
			  execI2cSlave();
			
			  execEmuEeprom();

        tstEmuEeprom();
			
				tstFlashWrite();
			
			  HAL_UART_CALBACK p = &UARTEmuEE;
        UARTEEPROM(p);
		
		}

}
