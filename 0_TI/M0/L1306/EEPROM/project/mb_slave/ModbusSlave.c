#include "ModbusSlave.h"

#define SET_SLAVE_RX_ENABLE  //GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;
#define SET_SLAVE_TX_ENABLE  //GpioDataRegs.GPASET.bit.GPIO26 = 1;
#define RST_SLAVE_TX_ENABLE  //GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;

void cntModbusTimeout(SCI_MODBUS *mbus)
{
    if(0<mbus->rw_size) {
        mbus->timetick = SWTIMER_CNT;

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
    mbus->timestamp = mbus->timetick = SWTIMER_CNT;

}

void rstModbusError(SCI_MODBUS *mbus)
{
    for(; 0<mbus->amount_of_error; mbus->amount_of_error--)
                mbus->errorlog[mbus->amount_of_error-1] = MB_NO_ERROR;

}

int16_t initUartDMA(SCI_MODBUS *mbus)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&((UART_Regs *)mbus->sci)->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &mbus->RxBuf[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, SIZE_OF_RXBUF);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    return 1;
}

int16_t initMbusConfig(SCI_MODBUS *mbus)
{
    rstModbusTimeout(mbus);
    mbus->popcnts = mbus->pushcnts = 0;
	mbus->RxBuf[0] = 0xFF; //Reset Slave Address
	mbus->pFiFo.pb = mbus->pFiFo.pi = &mbus->RxBuf[0];
	mbus->pFiFo.pe = &mbus->RxBuf[SIZE_OF_RXBUF-1];
	mbus->crcdata = DEFAULT_CRC;
	mbus->crc = 0x0000;
	mbus->rw_size = 0;
    mbus->info = DEFAULT_REG_INFO;
    mbus->state = MBUS_FREE;
    mbus->initReg(mbus);
    SET_SLAVE_RX_ENABLE;
	return 1;
}




//int16_t exeModbusSlave(SCI_MODBUS *mbus)
//{
//    switch(mbus->evstep) {
//    case _INIT_SCI_CONFIG:
//        if(0 == mbus->sci) return 0;
//        SYSCFG_DL_DEBUG_SCI_init();
//        initUartDMA(mbus);
//        NVIC_EnableIRQ(DEBUG_SCI_INST_INT_IRQN);
//        rstModbusError(mbus);
//        mbus->evstep = _INIT_MODBUS_INFO;
//
//    case _INIT_MODBUS_INFO:
//        initMbusConfig(mbus);
//        break;
//
//    case _RECEIVE_DATA_FROM_SCI:
//        mbus->rw_size = SIZE_OF_RXBUF - DL_DMA_getTransferSize(DMA, DMA_CH0_CHAN_ID);
//
//        break;
//
//    case _RESET_FIFO_DATA:
//        DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
//        initUartDMA(mbus);
//        mbus->evstep = _RECEIVE_DATA_FROM_SCI;
//        break;
//
//    default:
//        break;
//    }
//
//    return mbus->state;
//}



//int16_t isrMbusRxData(SCI_MODBUS *mbus)
//{
//    if(0 == DL_UART_isRXFIFOEmpty((UART_Regs *)mbus->sci)) {
//        if(SIZE_OF_RXBUF > mbus->rw_size) {
//            rstModbusTimeout(mbus);
//            mbus->rw_size++;
//            *mbus->pFiFo.pi = DL_UART_receiveDataBlocking((UART_Regs *)mbus->sci);
//            mbus->crc = ucMBCRC16(*mbus->pFiFo.pi, &mbus->crcdata);
//            if(mbus->pFiFo.pi < mbus->pFiFo.pe) mbus->pFiFo.pi++;
//            else                                mbus->pFiFo.pi = mbus->pFiFo.pb;
//        }
//        else return -1;
//    }
//
//	return mbus->rw_size;
//}

int16_t isrMbusRxData(SCI_MODBUS *mbus)
{
    uint16_t u16Size = SIZE_OF_RXBUF - DL_DMA_getTransferSize(DMA, DMA_CH0_CHAN_ID);

    if(mbus->pFiFo.pi != &mbus->RxBuf[u16Size]) {
        mbus->rw_size++;
        rstModbusTimeout(mbus);
        mbus->crc = ucMBCRC16(*mbus->pFiFo.pi, &mbus->crcdata);
        if(mbus->pFiFo.pi < mbus->pFiFo.pe) mbus->pFiFo.pi++;
        else                                mbus->pFiFo.pi = mbus->pFiFo.pb;
    }

    return mbus->rw_size;
}

void DEBUG_SCI_INST_IRQHandler(void)
{
  //  DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}


int16_t exeModbusSlave(SCI_MODBUS *mbus)
{
    switch(mbus->evstep) {
    case _INIT_SCI_CONFIG:
        if(0 == mbus->sci) return 0;
        SYSCFG_DL_DEBUG_SCI_init();
        rstModbusError(mbus);
        mbus->evstep = _INIT_MODBUS_INFO;

    case _INIT_MODBUS_INFO:
        initMbusConfig(mbus);
        mbus->evstep = _RESET_RX_DMA;
        break;

    case _RESET_RX_DMA:
        DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
        if(1 == DL_UART_isRXFIFOEmpty((UART_Regs *)mbus->sci)) {
            initUartDMA(mbus);
            NVIC_EnableIRQ(DEBUG_SCI_INST_INT_IRQN);
            mbus->evstep = _RECEIVE_DATA_FROM_SCI;
        }
        break;

    case _RECEIVE_DATA_FROM_SCI:

        if(0 < isrMbusRxData(mbus)) {

            cntModbusTimeout(mbus);

            if(mbus->pFiFo.pi != mbus->pFiFo.pb)
            {
                if((0 == mbus->crc)&&(3 < mbus->rw_size)) //CRC matched?
                {
                    if((0 == *mbus->pFiFo.pb)||(mbus->slaveid == *mbus->pFiFo.pb)) //broadcast? or slave id matched?
                    {
                        //response
                        if(mbus->slaveid == *mbus->pFiFo.pb) SET_SLAVE_TX_ENABLE;
                        else                                 RST_SLAVE_TX_ENABLE;

                        //_PROCESS_RECEIVING_DATA
                        mbus->pHeader = (MODBUS_HEADER *)mbus->pFiFo.pb;
                        mbus->state = (MODBUS_STATUS)ModbusFunc[mbus->pHeader->Function](mbus);
                        if(MBUS_WAIT != mbus->state)
                        {
                            mbus->crcdata = DEFAULT_CRC;
                            mbus->rw_size = 0;
                            mbus->pFiFo.pi = mbus->pFiFo.pb;
                            ModbusCommError(mbus);
                            mbus->evstep = _RESET_RX_DMA;
                        }
                    }
                    else
                    {
                        mbus->crcdata = DEFAULT_CRC;
                        mbus->rw_size = 0;
                        mbus->pFiFo.pi = mbus->pFiFo.pb;
                        mbus->evstep = _RESET_RX_DMA;
                    }
                }
                mbus->timeout++;
            }
        }
        else {
            if(mbus->popcnts<mbus->pushcnts) {
                if(0 == DL_UART_isTXFIFOFull((UART_Regs *)mbus->sci)) {
                    uint8_t u8temp = (uint8_t)mbus->TxBuf[mbus->popcnts++];
                    DL_UART_fillTXFIFO((UART_Regs *)mbus->sci, &u8temp, 1);
                    if(mbus->popcnts==mbus->pushcnts) break;
                    else                              rstModbusTimeout(mbus);
                }
            }
            else {
                if(false == DL_UART_isBusy((UART_Regs *)mbus->sci)) {
                    SET_SLAVE_RX_ENABLE;  //set RS485 IC RE enable
                    mbus->state = MBUS_FREE;
                    mbus->info = DEFAULT_REG_INFO;
                    mbus->evstep = _RECEIVE_DATA_FROM_SCI;
                    mbus->popcnts = mbus->pushcnts = 0;

                }
            }
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
