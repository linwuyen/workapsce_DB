/*
 * i2c_eeprom.c
 *
 *  Created on: 2023¦~6¤ë5¤é
 *      Author: cody_chen
 */

#include "i2c_eeprom.h"
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
    UI2C_T *i2c;
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

volatile enum UI2C_SLAVE_EVENT s_Event = SLAVE_ADDRESS_ACK;

#define SET_I2CS(bit)  FG_SET(bit, stI2cS.fsm);
#define RST_I2CS(bit)  FG_RST(bit, stI2cS.fsm);
#define SWTO_I2CS(bit) FG_SWTO(bit, stI2cS.fsm);
#define GET_I2CS(bit)  FG_GET(bit, stI2cS.fsm)
#define AND_I2CS(bit)  FG_AND(bit, stI2cS.fsm)


void UI2C0_Init(UI2C_T *ui2c, uint32_t u32ClkSpeed)
{
    /* Open USCI_I2C0 and set clock to 100k */
    UI2C_Open(ui2c, u32ClkSpeed);

    /* Get USCI_I2C0 Bus Clock */
    printf("ui2c clock %d Hz\n", UI2C_GetBusClockFreq(ui2c));

    /* Set USCI_I2C0 Slave Addresses */
    UI2C_SetSlaveAddr(ui2c, 0, I2C_EEPROM_TARGET_ID, UI2C_GCMODE_ENABLE); 
    UI2C_SetSlaveAddr(ui2c, 1, I2C_GLOBAL_ID, UI2C_GCMODE_DISABLE); 

    /* Set USCI_I2C0 Slave Addresses Mask */
    UI2C_SetSlaveAddrMask(ui2c, 0, 0x00);     
    UI2C_SetSlaveAddrMask(ui2c, 1, 0x00); 

    /* Enable ui2c protocol interrupt */
    UI2C_ENABLE_PROT_INT(ui2c, (UI2C_PROTIEN_ACKIEN_Msk | UI2C_PROTIEN_NACKIEN_Msk | UI2C_PROTIEN_STORIEN_Msk | UI2C_PROTIEN_STARIEN_Msk));
    NVIC_EnableIRQ(USCI0_IRQn);

}

void execI2cSlave(void)
{
    HAL_I2CS p = (HAL_I2CS)&stI2cS;

    switch(p->fsm) {
    case _INIT_I2C_CONFIG:
        p->i2c = UI2C0;
        UI2C0_Init(p->i2c, 100000);

        p->u32Tcnt = 0;
        p->u32Tlen = 0;

        /* Initialize variables to receive data inside RX ISR */
        p->u32Rcnt = 0;
        p->u32Rlen = 0;
		
				s_Event = SLAVE_ADDRESS_ACK;

				UI2C_SET_CONTROL_REG(UI2C0, (UI2C_CTL_PTRG | UI2C_CTL_AA));

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

    default:

        break;
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/*  USCI_I2C IRQ Handler                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void USCI0_IRQHandler(void)
{
    uint32_t u32Status;
		HAL_I2CS p = (HAL_I2CS)&stI2cS;

    //UI2C0 Interrupt
    u32Status = UI2C_GET_PROT_STATUS(UI2C0);	


    if ((u32Status & UI2C_PROTSTS_STARIF_Msk) == UI2C_PROTSTS_STARIF_Msk)                   /* Re-Start been received */
    {
        /* Clear START INT Flag */
        UI2C_CLR_PROT_INT_FLAG(UI2C0, UI2C_PROTSTS_STARIF_Msk);

        /* Event process */
        s_Event = SLAVE_ADDRESS_ACK;
			
				/* Initialize RX or TX after Start condition is received */
				p->u32Tcnt = 0;
				p->u32Rcnt = 0;

        /* Trigger USCI I2C */
        UI2C_SET_CONTROL_REG(UI2C0, (UI2C_CTL_PTRG | UI2C_CTL_AA));
    }
    else if ((u32Status & UI2C_PROTSTS_ACKIF_Msk) == UI2C_PROTSTS_ACKIF_Msk)                /* USCI I2C Bus have been received ACK */
    {				
        /* Clear ACK INT Flag */
        UI2C_CLR_PROT_INT_FLAG(UI2C0, UI2C_PROTSTS_ACKIF_Msk);

        /* Event process */
        if (s_Event == SLAVE_ADDRESS_ACK)                                                   /* Address Data has been received */
        {
					  startcnt = 4;
						uint8_t u8SlaveID = (uint8_t)UI2C_GET_DATA(UI2C0);
						
						if ((u32Status & UI2C_PROTSTS_SLAREAD_Msk) == UI2C_PROTSTS_SLAREAD_Msk)
						{
								s_Event = SLAVE_SEND_DATA;   
							  
							/* Slave address read has been received */
							  UI2C_SET_DATA(UI2C0, p->pRAM[p->u32Tcnt++]);
						}
						else
            {
                s_Event = SLAVE_GET_DATA;                                                   /* Slave address write has been received */             
            }            
        }
				else if (s_Event == SLAVE_GET_DATA)
        {
            uint8_t u8data = (uint8_t)UI2C_GET_DATA(UI2C0);
					
						if(p->u32Rcnt < 2) {
								uint32_t swapbyte = 1 - p->u32Rcnt;
								p->memaddr.u8data[swapbyte] = u8data;
								p->pRAM = readEmuEeprom(p->memaddr.u16data);
								p->u32Tlen = EMU_KBYTES - p->memaddr.u16data;
								p->u32Tcnt = 0;

						}
						else {
								uint32_t memaddr = p->memaddr.u16data + p->u32Rcnt - 2;
								writeEmuEeprom(memaddr, u8data);
						}
						p->u32Rcnt++;

        }				
        else if (s_Event == SLAVE_SEND_DATA)
        {						
						/* Fill TX FIFO if there are more bytes to send */
            if (p->u32Tcnt < p->u32Tlen) {
							  UI2C_SET_DATA(UI2C0, p->pRAM[p->u32Tcnt++]);
            }
        }

        /* Trigger USCI I2C */
        UI2C_SET_CONTROL_REG(UI2C0, (UI2C_CTL_PTRG | UI2C_CTL_AA));
    }
    else if ((u32Status & UI2C_PROTSTS_NACKIF_Msk) == UI2C_PROTSTS_NACKIF_Msk)
    {
        /* Clear NACK INT Flag */
        UI2C_CLR_PROT_INT_FLAG(UI2C0, UI2C_PROTSTS_NACKIF_Msk);

        /* Event process */
        s_Event = SLAVE_ADDRESS_ACK;
				p->u32Tcnt = 0;
				p->u32Rcnt = 0;

        /* Trigger USCI I2C */
        UI2C_SET_CONTROL_REG(UI2C0, (UI2C_CTL_PTRG | UI2C_CTL_AA));
    }
    else if ((u32Status & UI2C_PROTSTS_STORIF_Msk) == UI2C_PROTSTS_STORIF_Msk)
    {
        /* Clear STOP INT Flag */
        UI2C_CLR_PROT_INT_FLAG(UI2C0, UI2C_PROTSTS_STORIF_Msk);

        s_Event = SLAVE_ADDRESS_ACK;
				p->u32Tcnt = 0;
				p->u32Rcnt = 0;

        /* Trigger USCI I2C */
        UI2C_SET_CONTROL_REG(UI2C0, (UI2C_CTL_PTRG | UI2C_CTL_AA));
    }
}


