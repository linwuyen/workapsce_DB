#include "stdio.h"
#include "NuMicro.h"

extern uint32_t openUart(UART_T *uart, uint32_t u32baudrate);
extern uint32_t isRxEmpty(UART_T *uart);
extern uint32_t isTxFull(UART_T *uart);
extern uint32_t wrBytes(UART_T * uart, uint8_t *pu8TxBuf, uint32_t u32Count);
extern uint32_t rdByte(UART_T * uart);
extern uint32_t DL_UART_transmitData(UART_T *uart,uint8_t u8Data);
