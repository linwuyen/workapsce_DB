/*
 * initCLA.h
 *
 *  Created on: 2022¦~3¤ë29¤é
 *      Author: cody_chen
 */

#ifndef INITCLA_H
#define INITCLA_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memcfg.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "cla.h"
#include "memcfg.h"


#include "HwConfig.h"
#include "cla_task.h"


extern uint32_t Cla1ProgRunStart, Cla1ProgLoadStart, Cla1ProgLoadSize;
extern uint32_t Cla1ConstRunStart, Cla1ConstLoadStart, Cla1ConstLoadSize;

extern void initCLA(void);


#endif /* INITCLA_H */
