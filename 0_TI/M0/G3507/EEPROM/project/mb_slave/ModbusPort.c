/*
 * ModbusPort.c
 *
 *  Created on: Mar 4, 2024
 *      Author: cody_chen
 */

#include "ModbusPort.h"

ST_FIFO sPort  = {
    .pushRcnts = 0,
    .popRcnts = 0,
    .pushTcnts = 0,
    .popTcnts = 0
};

uint16_t setMbusSendByte(SCI_MODBUS *mbus, uint16_t data)
{
    if(MBUS_BUFFER_SIZE > sPort.pushTcnts) {
        sPort.u16RAM[sPort.pushTcnts++] = data;
        return 1;
    }
    return 0;
}

uint16_t getMbusReadByte(SCI_MODBUS *mbus)
{
    if(sPort.popRcnts < sPort.pushRcnts) {
        uint16_t u16Temp = sPort.u16RAM[sPort.popRcnts++];
        if(sPort.popRcnts == sPort.pushRcnts) sPort.popRcnts = sPort.pushRcnts = 0;
        return u16Temp;
    }
    return 0;
}

int16_t getMbusRxSize(SCI_MODBUS *mbus)
{
    return (sPort.pushRcnts-sPort.popRcnts);
}

void pushRxByteToFIFO(SCI_MODBUS *mbus)
{
    while((0<mbus->getRsize(mbus->sci))&&(MBUS_BUFFER_SIZE > sPort.pushRcnts)) {
        uint16_t u16Temp = (mbus->rdfunc(mbus->sci) & 0x00FF);
        sPort.u16RAM[sPort.pushRcnts++] = u16Temp;
    }

//    if(0 == sReadCPU1.pushRcnts) sAccessCPU2.pushRcnts = 0;
}

void popFIFOtoTxData(SCI_MODBUS *mbus)
{
    while(sPort.popTcnts<sPort.pushTcnts) {
        if(mbus->getWsize(mbus->sci)) {
            mbus->wrfunc(mbus->sci, sPort.u16RAM[sPort.popTcnts++]);
            if(sPort.popTcnts == sPort.pushTcnts) {
                sPort.popTcnts = sPort.pushTcnts = 0;
                return;
            }
        }
        else {
            return;
        }
    }
}


int16_t pollModbusPort(SCI_MODBUS *mbus)
{
    switch(mbus->evPort) {
    case _INIT_SCI_CONFIG:
        if(0 == mbus->sci) return 0;
        runModbus(mbus);
        mbus->evPort = _RECEIVE_DATA_FROM_SCI;

    case _RECEIVE_DATA_FROM_SCI:
        pushRxByteToFIFO(mbus);
        popFIFOtoTxData(mbus);
        break;

    default:
        break;
    }

    return mbus->state;
}

