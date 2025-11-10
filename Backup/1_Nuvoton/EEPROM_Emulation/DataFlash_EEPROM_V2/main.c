/******************************************************************************
 * @file     main.c
 * @version  V3.00
 * $Revision: 8 $
 * $Date: 14/02/10 2:43p $
 * @brief    FMC erase/program/read sample program for M051 series MCU
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "M051Series.h"
#include "EEPROM_Emulate.h"

#define PLLCON_SETTING      CLK_PLLCON_50MHz_HXT
#define PLL_CLOCK           50000000

#define Test_data_size			20
#define Test_page_amount		6

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable External XTAL (4~24 MHz) */
    CLK->PWRCON |= CLK_PWRCON_XTL12M_EN_Msk;

    CLK->PLLCON = PLLCON_SETTING;

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_PLL_STB_Msk | CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Switch HCLK clock source to PLL */
    CLK->CLKSEL0 = CLK_CLKSEL0_HCLK_S_PLL;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate();
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()
}

int main()
{
    uint32_t i;
		uint8_t u8Data;

    /* Unlock protected registers */
    SYS_UnlockReg();
	
		SYS_Init();
GPIO_SetMode(P0, BIT3, GPIO_PMD_OUTPUT);
		P03 = 0;
		/* Test Init_EEPROM() */
		Init_EEPROM(Test_data_size, Test_page_amount);
						P03^=1;
							P03^=1;
		/* Test Search_Valid_Page() */
		Search_Valid_Page();
				P03^=1;
							P03^=1;
		/* Test Write_Data() */
		for(i = 0; i < 254; i++)
		{
				Write_Data(i%Test_data_size, i%256);
				P03^=1;
		}		
						P03^=1;
		/* Test Write_Data() contain Manage_Next_Page() */
		Write_Data(i%Test_data_size, 0xFF);
	P03^=1;		
								P03^=1;
		/* Test Read_Data() */
		Read_Data(0x7, &u8Data);
		P03^=1;

		/* Test Write over 20000 times */
		//while(Get_Cycle_Counter() < 20000)
			while(1)
		{
				for(i = 0; i < 247; i++)
				{
						Write_Data(i%Test_data_size, i%256);
				}
		}	
    while(1);
}

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
