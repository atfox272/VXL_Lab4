/*
 * my_scheduler.h
 *
 *  Created on: Nov 28, 2023
 *      Author: atfox
 */

#ifndef INC_MY_SCHEDULER_H_
#define INC_MY_SCHEDULER_H_

#include "stdint.h"

#define TIMER_CYCLE		10	// ms

#define SCH_MAX_TASKS 	40
#define NO_TASK_ID 		0

void SCH_Init(void);
uint8_t SCH_Add_Task (void (*pFunction)(void), uint32_t Delay, uint32_t Period);
void SCH_Update(void);
void SCH_Dispath_Tasks(void);
uint8_t SCH_Delete(uint8_t TaskID);


#endif /* INC_MY_SCHEDULER_H_ */
