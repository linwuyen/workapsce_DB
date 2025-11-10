#include "common.h"
#include "ModbusSlave.h"
#include "ModbusPort.h"

void cntModbusTimeout(SCI_MODBUS *mbus)
{
    if(0<mbus->sFiFo.pushRcnts) {
        mbus->timetick = SW_TIMER - (uint32_t)CPUTimer_getTimerCount(SWTIRMER_BASE);

        if(mbus->timestamp > mbus->timetick) {
            mbus->timeout = mbus->timetick + SW_TIMER - mbus->timestamp;
        }
        else {
            mbus->timeout = mbus->timetick - mbus->timestamp;
        }

        if(mbus->timeout >= MBUS_TIMEOUT) {
            mbus->timestamp = mbus->timetick;
            mbus->evstep = _INIT_MODBUS_INFO;
        }
    }
}

void rstModbusTimeout(SCI_MODBUS *mbus)
{
    mbus->timestamp = mbus->timetick = SW_TIMER - (uint32_t)CPUTimer_getTimerCount(SWTIRMER_BASE);

}

void rstModbusError(SCI_MODBUS *mbus)
{
    for(; 0<mbus->amount_of_error; mbus->amount_of_error--)
                mbus->errorlog[mbus->amount_of_error-1] = MB_NO_ERROR;

}

int16_t initMbusConfig(SCI_MODBUS *mbus)
{
    rstModbusTimeout(mbus);
    mbus->sFiFo = (ST_FIFO){
       .pushRcnts = 0,
       .popRcnts = 0,
       .pushTcnts = 0,
       .popTcnts = 0,
       .u16RAM[0]= 0xFFFF
    };
	mbus->crcdata = DEFAULT_CRC;
	mbus->crc = 0x0000;
    mbus->info = DEFAULT_REG_INFO;
    mbus->state = MBUS_FREE;
    mbus->evstep = _RECEIVE_DATA_FROM_FIFO;
    mbus->initReg(mbus);

	return 1;
}

int16_t getMbusRxFIFO(SCI_MODBUS *mbus)
{
    if(0<getMbusRxSize(mbus)) {
        if(MBUS_BUFFER_SIZE > mbus->sFiFo.pushRcnts) {
            rstModbusTimeout(mbus);

            mbus->sFiFo.u16RAM[mbus->sFiFo.pushRcnts] = getMbusReadByte(mbus);
            mbus->crc = ucMBCRC16(mbus->sFiFo.u16RAM[mbus->sFiFo.pushRcnts], &mbus->crcdata);
            mbus->sFiFo.pushRcnts++;
        }
        else return 0;
    }

    return (mbus->sFiFo.pushRcnts != mbus->sFiFo.popRcnts);
}

int16_t setMbusTxData(SCI_MODBUS *mbus)
{
    if(mbus->sFiFo.popTcnts < mbus->sFiFo.pushTcnts) {
        setMbusSendByte(mbus, mbus->sFiFo.u16RAM[mbus->sFiFo.popTcnts++]);
    }
    else {
        mbus->state = MBUS_FREE;
        mbus->info = DEFAULT_REG_INFO;
        mbus->sFiFo.popTcnts = mbus->sFiFo.pushTcnts = 0;
    }

    return (mbus->sFiFo.pushTcnts != mbus->sFiFo.popTcnts);
}

void stopModbus(SCI_MODBUS *mbus)
{
    mbus->evstep = _WAIT_FOR_START_MODBUS;
}

void runModbus(SCI_MODBUS *mbus)
{
    mbus->evstep = _INIT_MODBUS_INFO;
}

int16_t exeModbusSlave(SCI_MODBUS *mbus)
{
    pollModbusPort(mbus);

    switch(mbus->evstep) {
    case _WAIT_FOR_START_MODBUS:
        return 0;

    case _INIT_MODBUS_INFO:
        initMbusConfig(mbus);
        break;

    case _RECEIVE_DATA_FROM_FIFO:

        if(0 < getMbusRxFIFO(mbus)) {
            cntModbusTimeout(mbus);

            if(mbus->sFiFo.popRcnts != mbus->sFiFo.pushRcnts) {
                if((0 == mbus->crc)&&(3 < mbus->sFiFo.pushRcnts)) //CRC matched?
                {
                    if((BROADCAST_ID == mbus->sFiFo.u16RAM[0])||(mbus->slaveid == mbus->sFiFo.u16RAM[0])) //broadcast? or slave id matched?
                    {
                        //_PROCESS_RECEIVING_DATA
                        mbus->pHeader = (MODBUS_HEADER *)&mbus->sFiFo.u16RAM[0];
                        mbus->state = (MODBUS_STATUS)ModbusFunc[mbus->pHeader->Function](mbus);
                        if(MBUS_WAIT != mbus->state)
                        {
                            mbus->crcdata = DEFAULT_CRC;
                            mbus->sFiFo.popRcnts = mbus->sFiFo.pushRcnts = 0;
                            ModbusCommError(mbus);
                        }
                    }
                    else {
                        mbus->crcdata = DEFAULT_CRC;
                        mbus->sFiFo.popRcnts = mbus->sFiFo.pushRcnts = 0;
                    }
                }
                mbus->timeout++;
            }
        }
        else {
            setMbusTxData(mbus);
        }

        break;

    case _RESET_FIFO_DATA:
        mbus->evstep = _INIT_MODBUS_INFO;
        break;

    default:
        break;
    }

    return mbus->state;
}
