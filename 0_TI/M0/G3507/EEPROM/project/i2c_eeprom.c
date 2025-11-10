/*
 * i2c_eeprom.c
 *
 *  Created on: 2023¦~6¤ë5¤é
 *      Author: cody_chen
 */

#include "emu_eeprom.h"


typedef enum {
    _IS_RX_EMPTY           = 0x0000,
    _GET_MEMORY_ADDRESS    = (0x0001<<0),
    _ERROR_MARK_RCVFSM     = 0x8000
} FSM_RXSTEP;


typedef enum {
    _INIT_I2C_CONFIG        = 0x0000,
    _CHECK_IS_FLAHS_READY   = (0x0001<<0),
    _RECEIVE_MEMORY_ADDRESS = (0x0001<<1),
    _RECEIVE_DATA_BLOCK     = (0x0001<<2),
    _TRANSMIT_DATA_BLOCK    = (0x0001<<3),
    _ERROR_MASK_I2C_SLAVE   = 0x8000
} FSM_I2CS;

typedef struct {
    I2C_Regs *i2c;
    FSM_I2CS fsm;

    FSM_RXSTEP  rxstep;
    uint8_t * pRAM;
    uint8_t* (*rdbyte)(uint16_t u16addr);
    int16_t (*wrbyte)(uint16_t u16addr, uint8_t u8Data);
    int16_t (*program)(void);
    int16_t (*isFree)(void);


    /* Counters for TX length and bytes sent */
    uint32_t u32Tlen, u32Tcnt;

    /* Counters for TX length and bytes sent */
    uint32_t u32Rlen, u32Rcnt;

    union {
        uint16_t u16data;
        uint8_t  u8data[2];
        struct {
            uint8_t u8low;
            uint8_t u8high;
        }byte;
    } memaddr;
} ST_I2CS;

typedef ST_I2CS * HAL_I2CS;

ST_I2CS stI2cS;


#define SET_I2CS(bit)  FG_SET(bit, stI2cS.fsm);
#define RST_I2CS(bit)  FG_RST(bit, stI2cS.fsm);
#define SWTO_I2CS(bit) FG_SWTO(bit, stI2cS.fsm);
#define GET_I2CS(bit)  FG_GET(bit, stI2cS.fsm)
#define AND_I2CS(bit)  FG_AND(bit, stI2cS.fsm)

extern SYSCONFIG_WEAK void SYSCFG_DL_I2C_EEPROM_init(void);


void rcvBlockI2C(HAL_I2CS v)
{
    switch(v->rxstep) {
    case _IS_RX_EMPTY:
        if(DL_I2C_isTargetRXFIFOEmpty(v->i2c)) {

        }
        break;
    case _GET_MEMORY_ADDRESS:

        break;

    default:
        break;
    }
}






void execI2cSlave(void)
{
    HAL_I2CS p = (HAL_I2CS)&stI2cS;

    switch(p->fsm) {
    case _INIT_I2C_CONFIG:
        p->i2c = I2C_EEPROM_INST;
        SYSCFG_DL_I2C_EEPROM_init();

        p->u32Tcnt = 0;
        p->u32Tlen = 0;
        DL_I2C_enableInterrupt(p->i2c, DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER);

        /* Initialize variables to receive data inside RX ISR */
        p->u32Rcnt = 0;
        p->u32Rlen = 0;

        NVIC_EnableIRQ(I2C_EEPROM_INST_INT_IRQN);

        SWTO_I2CS(_CHECK_IS_FLAHS_READY);
        break;
    case _CHECK_IS_FLAHS_READY:
        if(isEmuEepromFree()) {
            p->isFree = isEmuEepromFree;
            p->rdbyte = readEmuEeprom;
            p->wrbyte = writeEmuEeprom;
            p->program = setProgramEmuEeprom;
            SWTO_I2CS(_RECEIVE_MEMORY_ADDRESS);
        }
        break;

    case _RECEIVE_MEMORY_ADDRESS:
        rcvBlockI2C(p);
        break;

    default:

        break;
    }
}


void I2C_EEPROM_INST_IRQHandler(void)
{
    HAL_I2CS p = (HAL_I2CS)&stI2cS;


    switch (DL_I2C_getPendingInterrupt(p->i2c)) {
        case DL_I2C_IIDX_TARGET_START:
            /* Initialize RX or TX after Start condition is received */
            p->u32Tcnt = 0;
            p->u32Rcnt = 0;
            /* Flush TX FIFO to refill it */
            DL_I2C_flushTargetTXFIFO(p->i2c);
            break;

        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in buffer */
            while (DL_I2C_isTargetRXFIFOEmpty(p->i2c) != true) {
                uint8_t u8char = DL_I2C_receiveTargetData(p->i2c);
                if(p->u32Rcnt < 2) {
                    uint32_t swapbyte = 1 - p->u32Rcnt;
                    p->memaddr.u8data[swapbyte] = u8char;
                    p->pRAM = readEmuEeprom(p->memaddr.u16data);
                    p->u32Tlen = EMU_KBYTES - p->memaddr.u16data;
                    p->u32Tcnt = 0;
                }
                else {
                    uint32_t memaddr = p->memaddr.u16data + p->u32Rcnt - 2;
                    writeEEPROM();
                    writeEmuEeprom(memaddr, u8char);
                }
                p->u32Rcnt++;
            }
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER:

            /* Fill TX FIFO if there are more bytes to send */
            if (p->u32Tcnt < p->u32Tlen) {
                uint8_t u8bytes = p->u32Tlen - p->u32Tcnt;
                p->u32Tcnt += DL_I2C_fillTargetTXFIFO(
                    p->i2c, &p->pRAM[p->u32Tcnt], 8);
            } else {
                /*
                 * Fill FIFO with 0x00 if more data is requested than
                 * expected u32Tlen
                 */
                while (DL_I2C_transmitTargetDataCheck(p->i2c, 0x00) != false)
                    ;
            }
            break;

//        case DL_I2C_IIDX_NO_INT:
//            __BKPT() ;
//
//
//        case DL_I2C_IIDX_CONTROLLER_PEC_RX_ERROR :
//            __BKPT() ;
        case DL_I2C_IIDX_TIMEOUT_A:
            __BKPT() ;
//        case DL_I2C_IIDX_TIMEOUT_B:
//            __BKPT() ;
//
//        case DL_I2C_IIDX_TARGET_RX_DONE:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_TX_DONE:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_RXFIFO_FULL:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_TXFIFO_EMPTY:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_STOP:
//             __BKPT() ;
//        case DL_I2C_IIDX_TARGET_GENERAL_CALL:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_EVENT1_DMA_DONE:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_EVENT2_DMA_DONE:
//            __BKPT() ;
//
//
//        case DL_I2C_IIDX_TARGET_PEC_RX_ERROR:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_TXFIFO_UNDERFLOW:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_RXFIFO_OVERFLOW:
//            __BKPT() ;
//        case DL_I2C_IIDX_TARGET_ARBITRATION_LOST:
//            __BKPT() ;
//        case DL_I2C_IIDX_INTERRUPT_OVERFLOW:
//            __BKPT() ;


        default:
            break;
    }
}


