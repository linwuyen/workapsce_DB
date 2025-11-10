#include "g_uart.h"

uint32_t openUart(UART_T *uart, uint32_t u32baudrate)
{
	  uint32_t u32UartClkSrcSel = 0ul, u32UartClkDivNum = 0ul;
    uint32_t u32ClkTbl[6ul] = {__HXT, 0ul, __LXT, __HIRC, 0ul, __LIRC};
    uint32_t u32Baud_Div = 0ul;


    if (uart == (UART_T *)UART0)
    {
        /* Get UART clock source selection */
        u32UartClkSrcSel = ((uint32_t)(CLK->CLKSEL1 & CLK_CLKSEL1_UART0SEL_Msk)) >> CLK_CLKSEL1_UART0SEL_Pos;
        /* Get UART clock divider number */
        u32UartClkDivNum = (CLK->CLKDIV0 & CLK_CLKDIV0_UART0DIV_Msk) >> CLK_CLKDIV0_UART0DIV_Pos;
    }
    else if (uart == (UART_T *)UART1)
    {
        /* Get UART clock source selection */
        u32UartClkSrcSel = (CLK->CLKSEL1 & CLK_CLKSEL1_UART1SEL_Msk) >> CLK_CLKSEL1_UART1SEL_Pos;
        /* Get UART clock divider number */
        u32UartClkDivNum = (CLK->CLKDIV0 & CLK_CLKDIV0_UART1DIV_Msk) >> CLK_CLKDIV0_UART1DIV_Pos;
    }

    /* Select UART function */
    uart->FUNCSEL = UART_FUNCSEL_UART;

    /* Set UART line configuration */
    uart->LINE = UART_WORD_LEN_8 | UART_PARITY_EVEN | UART_STOP_BIT_1;

    /* Set UART Rx and RTS trigger level */
    uart->FIFO &= ~(UART_FIFO_RFITL_Msk | UART_FIFO_RTSTRGLV_Msk);

    /* Get PCLK clock frequency if UART clock source selection is PCLK */
    if (u32UartClkSrcSel == 4ul)
    {
        /* UART Port as UART0 */
        if (uart == (UART_T *)UART0)
        {
            u32ClkTbl[u32UartClkSrcSel] =  CLK_GetPCLK0Freq();
        }
        else     /* UART Port as UART1*/
        {
            u32ClkTbl[u32UartClkSrcSel] =  CLK_GetPCLK1Freq();
        }

    }

    /* Set UART baud rate */
    if (u32baudrate != 0ul)
    {
        u32Baud_Div = UART_BAUD_MODE2_DIVIDER((u32ClkTbl[u32UartClkSrcSel]) / (u32UartClkDivNum + 1ul), u32baudrate);

        if (u32Baud_Div > 0xFFFFul)
        {
            uart->BAUD = (UART_BAUD_MODE0 | UART_BAUD_MODE0_DIVIDER((u32ClkTbl[u32UartClkSrcSel]) / (u32UartClkDivNum + 1ul), u32baudrate));
        }
        else
        {
            uart->BAUD = (UART_BAUD_MODE2 | u32Baud_Div);
        }
    }
		
		return 0;
}

uint32_t isRxEmpty(UART_T *uart)
{	
	return UART_GET_RX_EMPTY(uart);  /* Check RX empty => failed */
}

uint32_t isTxFull(UART_T *uart)
{
	return UART_IS_TX_FULL(uart);
}

uint32_t wrBytes(UART_T * uart, uint8_t *pu8TxBuf, uint32_t u32Count)
{
	int16_t i = 0;
	for(i=0; i<u32Count; i++) uart->DAT = pu8TxBuf[i];    /* Send UART Data from buffer */
	
	return uart->DAT;
}

uint32_t rdByte(UART_T * uart)
{
	return (uint8_t)uart->DAT;	
}

uint32_t DL_UART_transmitData(UART_T *uart,uint8_t u8Data)
{
return UART_WRITE(uart, u8Data);
}
