/*
 * cdebug.h
 *
 *  Created on: 2022¦~9¤ë5¤é
 *      Author: cody_chen
 */

#ifndef CDEBUG_H_
#define CDEBUG_H_




#define SET_DEBUG_MAIN_WHILE()          //GPIO_writePin(19, 1);
#define RST_DEBUG_MAIN_WHILE()          //GPIO_writePin(19, 0);

#define SET_DEBUG_TIME_TASK()           GPIO_writePin(2, 1);
#define RST_DEBUG_TIME_TASK()           GPIO_writePin(2, 0);
#define TGE_DEBUG_TIME_TASK()           GPIO_togglePin(2);

#define SET_DEBUG_ADC_EOC()             //GPIO_writePin(21, 1);
#define RST_DEBUG_ADC_EOC()             //GPIO_writePin(21, 0);

#define SET_DEBUG_CLA_TASK1()           //GPIO_writePin(19, 1); //GPIO_writePin(99, 1);
#define RST_DEBUG_CLA_TASK1()           //GPIO_writePin(19, 0); //GPIO_writePin(99, 0);



#endif /* CDEBUG_H_ */
