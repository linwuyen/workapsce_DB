/*
 * ModbusVSCI.h
 *
 *  Created on: Mar 5, 2024
 *      Author: cody_chen
 */

#ifndef MODBUSVSCI_H_
#define MODBUSVSCI_H_

#include "common.h"

static inline int16_t VSCI_base(uint32_t base)
{
    return 0;
}


static inline int16_t VSCI_getRxFIFOStatus(uint32_t base)
{
    return (0 == DL_UART_isRXFIFOEmpty((UART_Regs *)base));
}

static inline uint16_t VSCI_readCharNonBlocking(uint32_t base)
{
//    if(sReadCPU1.pushRcnts != sAccessCPU2.popRcnts) {
//        uint16_t u16Temp = sReadCPU1.u16RAM[sAccessCPU2.popRcnts++];
//        if(MBUS_BUFFER_SIZE == sAccessCPU2.popRcnts) sAccessCPU2.popRcnts = 0;
//        return u16Temp;
//    }
    return DL_UART_receiveDataBlocking((UART_Regs *)base);
}

static inline int16_t VSCI_getTxFIFOStatus(uint32_t base)
{
    return (0 == DL_UART_isTXFIFOFull((UART_Regs *)base));
}

static inline void VSCI_writeCharNonBlocking(uint32_t base, uint16_t data)
{
//    sAccessCPU2.u16RAM[sAccessCPU2.pushTcnts++] = data;
//    if(MBUS_SHARERAM_SIZE == sAccessCPU2.pushTcnts) sAccessCPU2.pushTcnts = 0;
    uint8_t u8temp = data;
    DL_UART_fillTXFIFO((UART_Regs *)base, &u8temp, 1);
}



#endif /* MODBUSVSCI_H_ */
