#include "ModbusMaster.h"
#include "ModbusSci.h"

typedef struct {
	CMDPACK *pb;
	CMDPACK *pi;
	CMDPACK *po;
	CMDPACK *pe;
} CMDFIFO;

#define SIZE_OF_FIFO		16

CMDPACK CmdPack[SIZE_OF_FIFO] = {DEFAULT_CMDPACK};



#define DEFAULT_CMDFIFO (CMDFIFO){ \
			.pb = &CmdPack[0], \
			.pi = &CmdPack[0], \
			.po = &CmdPack[0], \
			.pe = &CmdPack[SIZE_OF_FIFO-1] }

CMDFIFO CmdFiFo = DEFAULT_CMDFIFO;

#pragma CODE_SECTION(pushCmdPack, "ramfuncs");

int16 pushCmdPack(CMDPACK * src)
{
	*CmdFiFo.pi = *src;
	if(CmdFiFo.pi < CmdFiFo.pe) CmdFiFo.pi++;
	else CmdFiFo.pi = CmdFiFo.pb;

	return 1;
}

#pragma CODE_SECTION(popCmdPack, "ramfuncs");

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

extern int16 (*ModbusFunc[25])(SCI_MODBUS *pMbus);

#define PARSER_TIMEOUT	500

#define ERROR_BUFFER 16
MSG_ERROR_ID errlog[ERROR_BUFFER];
Uint16 errcnt = 0;

//#pragma CODE_SECTION(exeModbusMaster, "ramfuncs");

int16 exeModbusMaster(SCI_MODBUS *mbus)
{
	Uint16 i = 0;
	HEADER_ERROR_REG *err = 0;
	switch(mbus->evstep) {
	case _INIT_SCI_GPIO:
		mbus->initgpio();
	case _INIT_SCI_CONFIG:
		initSciX(SERIAL_BAUDRATE, SERIAL_BITS_NUMBER, SERIAL_PARITY, mbus->sci);
		for(i=0; i<SIZE_OF_FIFO;i++) {
			CmdPack[i] = DEFAULT_CMDPACK;
		}
		for(i=0; i<ERROR_BUFFER;i++) {
			errlog[i] = MB_NO_ERROR;
		}
		CmdFiFo = DEFAULT_CMDFIFO;
		errcnt = 0;
		mbus->evstep = _POP_COMMAND_OUT;
	case _POP_COMMAND_OUT:
		mbus->pData = popCmdPack();
		mbus->tick1msec = 0;
		if(0 != mbus->pData)	mbus->evstep = _EXE_RW_COMMAND;
		break;

	case _EXE_RW_COMMAND:
		mbus->state = (MODBUS_STATUS)ModbusFunc[mbus->pData->function](mbus);
		if(MBUS_SUCCESS == mbus->state) mbus->evstep = _RESET_FIFO_DATA;
		if(MBUS_FAIL == mbus->state) mbus->evstep = _SAVE_ERROR_LOG;
		if(PARSER_TIMEOUT < mbus->tick1msec) mbus->evstep = _PROCESS_TIMEOUT;
		break;

	case _PROCESS_TIMEOUT:
		mbus->evHoldReg = _SEND_SLAVEID_AND_FUNCID;
		mbus->state = MBUS_FAIL;
		if(ERROR_BUFFER > errcnt) {
			errlog[errcnt] = MSG_ERROR_TIMEOUT;
			errcnt++;
		}
		mbus->evstep = _INIT_SCI_CONFIG;
		break;

	case _SAVE_ERROR_LOG:
		if(ERROR_BUFFER > errcnt) {
			err = (HEADER_ERROR_REG *)&mbus->RxBuffer[0];
			errlog[errcnt] = (MSG_ERROR_ID)err->ErrorNo;
			errcnt++;
		}

	case _RESET_FIFO_DATA:
		mbus->evstep = _POP_COMMAND_OUT;
		return 1;

	default:
		break;

	}

	return 0;
}
