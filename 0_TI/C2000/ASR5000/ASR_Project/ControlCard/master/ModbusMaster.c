/*
 * ModbusMaster.c
 *
 *  Created on: Jul 10, 2025
 *      Author: roger_lin
 */


#include "ModbusSlave.h"
#include "ModbusPort.h"
#include "ModbusMaster.h"

st_Control st_MB ={
               .FSM =_MASTER_IDLE,
};

CMDPACK CmdPack[SIZE_OF_FIFO] = {DEFAULT_CMDPACK};
CMDPACK CmdPack1 = {
                    .slave = 0x03,
                    .function = 0x03,
                    .address = 0x0,
                    .points = 0x0,
                    .bytes = 0x0,
                    .reg = 0x0
};



CMDFIFO CmdFiFo = DEFAULT_CMDFIFO;


uint16_t pushCmdPack(CMDPACK * src)
{
    *CmdFiFo.pi = *src;
    if(CmdFiFo.pi < CmdFiFo.pe) CmdFiFo.pi++;
    else CmdFiFo.pi = CmdFiFo.pb;

    return 1;
}


CMDPACK * popCmdPack(void)
{
    CMDPACK *temp = 0;

    if(CmdFiFo.pi != CmdFiFo.po) {
        temp = CmdFiFo.po;
        if(CmdFiFo.po < CmdFiFo.pe) CmdFiFo.po++;
        else CmdFiFo.po = CmdFiFo.pb;
    }
    return temp;
}


uint16_t crc_result;
ST_CRC crc_calculator = DEFAULT_CRC;
uint16_t frame_for_crc[6];

uint16_t CalcuCRC(const uint8_t *pdata, uint16_t length)
{

    for (uint16_t i = 0; i < length; i++)
    {
        ucMBCRC16(pdata[i], &crc_calculator);
    }
    return crc_calculator .wCRCWord;
}



int16_t exeModbusMaster(SCI_MODBUS *mbus)
{

    pollModbusPort(mbus);

    switch (st_MB.FSM)
    {

    case _MASTER_IDLE:
        break;

    case _TRANSACTION_SPI_M:
        sSPIA.NewFSM = _Mode_Master;
        st_MB.FSM = _MASTER_IDLE;
        break;

    case _MASTER_PREPARE_REQUEST:


                SCI_writeCharNonBlocking(DEBUG_SCI_BASE,0x03);
                SCI_writeCharNonBlocking(DEBUG_SCI_BASE,0x03);





//        frame_for_crc[0] = st_MB.pack.ID;
//        frame_for_crc[1] = st_MB.pack.FUNC;
//        frame_for_crc[2] = st_MB.pack.Address_hi;
//        frame_for_crc[3] = st_MB.pack.Address_lo;
//        frame_for_crc[4] = st_MB.pack.Quanity_hi;
//        frame_for_crc[5] = st_MB.pack.Quanity_lo;

//        crc_result = CalcuCRC(frame_for_crc, 6);
//
//        st_MB.pack.CRC_Lo = (uint16_t)(crc_result & 0x00FF);
//        st_MB.pack.CRC_Hi = (uint16_t)((crc_result >> 8) & 0x00FF);

        st_MB.FSM = _MASTER_START_TRANSMISSION;

        break;

    case _MASTER_START_TRANSMISSION:
//        SCI_writeCharNonBlocking(DEBUG_SCI_BASE, st_MB.pack.ID);
//        SCI_writeCharNonBlocking(DEBUG_SCI_BASE, st_MB.pack.FUNC);
//        SCI_writeCharNonBlocking(DEBUG_SCI_BASE, st_MB.pack.Address_hi);
//        SCI_writeCharNonBlocking(DEBUG_SCI_BASE, st_MB.pack.Address_lo);
//        SCI_writeCharNonBlocking(DEBUG_SCI_BASE, st_MB.pack.Quanity_hi);
//        SCI_writeCharNonBlocking(DEBUG_SCI_BASE, st_MB.pack.Quanity_lo);
//        SCI_writeCharNonBlocking(DEBUG_SCI_BASE, st_MB.pack.CRC_Lo);
//        SCI_writeCharNonBlocking(DEBUG_SCI_BASE, st_MB.pack.CRC_Hi);

        SCI_writeCharNonBlocking(DEBUG_SCI_BASE,0x03);
        SCI_writeCharNonBlocking(DEBUG_SCI_BASE,0xE8);

        st_MB.FSM = _MASTER_WAIT_FOR_RESPONSE;
        break;

    case _MASTER_WAIT_FOR_RESPONSE:

//        if( 0< SCI_getRxStatus(DEBUG_SCI_BASE))
//        {
//        st_con.u16iRAM[st_con.u16index++]  = SCI_readCharNonBlocking(DEBUG_SCI_BASE);
//
//        }


        SCI_writeCharNonBlocking(DEBUG_SCI_BASE,0x00);
        SCI_writeCharNonBlocking(DEBUG_SCI_BASE,0x01);

        st_MB.FSM = _MASTER_PROCESS_RESPONSE;
        break;

    case _MASTER_PROCESS_RESPONSE:



//        mbus->pData = popCmdPack();
//        mbus->tick1msec = 0;
//        if(0 != mbus->pData)    mbus->evstep = _EXE_RW_COMMAND;
        SCI_writeCharNonBlocking(DEBUG_SCI_BASE,0x05);
        SCI_writeCharNonBlocking(DEBUG_SCI_BASE,0x98);
        st_MB.FSM = _MASTER_IDLE;
        break;

    case _MASTER_TRANSACTION_COMPLETE:




//        mbus->state = (MODBUS_STATUS)ModbusFunc[mbus->pData->function](mbus);
//        if(MBUS_SUCCESS == mbus->state) mbus->evstep = _RESET_FIFO_DATA;
//        if(MBUS_FAIL == mbus->state) mbus->evstep = _SAVE_ERROR_LOG;
//        if(PARSER_TIMEOUT < mbus->tick1msec) mbus->evstep = _PROCESS_TIMEOUT;

        st_MB.FSM = _MASTER_IDLE;
        break;
    }
    return 0;
}




//-----------

