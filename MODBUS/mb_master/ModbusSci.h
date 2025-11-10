/*
 * ModbusSci.h
 *
 *  Created on: 2018¦~3¤ë28¤é
 *      Author: user
 */

#ifndef MODBUSSCI_H_
#define MODBUSSCI_H_


#include "DSP2803x_Sci.h"                // SCI Registers

#define LOW_SPEED_CLOCK 	15000000

#define SERIAL_BAUDRATE 	38400
#define SERIAL_PARITY 		SERIAL_PARITY_EVEN
#define SERIAL_BITS_NUMBER 	8

#define SERIAL_START_STOP_NUMBER_BITS	2
#if SERIAL_PARITY == SERIAL_PARITY_NONE
#define SERIAL_PARITY_NUMBER_BITS		0
#else
#define SERIAL_PARITY_NUMBER_BITS		1
#endif

typedef enum {
	SERIAL_PARITY_NONE,
	SERIAL_PARITY_EVEN,
	SERIAL_PARITY_ODD
} SerialParity;

extern void cleanSciX(volatile struct SCI_REGS *psci);
extern void resetSciX(volatile struct SCI_REGS *psci);
extern int16 initSciX(Uint32 baudrate, Uint16 databits, Uint16 parity, volatile struct SCI_REGS *psci);


#endif /* MODBUSSCI_H_ */
