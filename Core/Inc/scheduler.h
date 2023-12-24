/*
 * my_scheduler.h
 *
 *  Created on: Nov 28, 2023
 *      Author: atfox
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "stdint.h"
#include "stdlib.h"
#include "timer.h"

//#define SCH_MAX_TASKS 	40

void SCH_Init(void);
void SCH_Update(void);
void SCH_Dispath_Tasks(void);
struct Task *SCH_Add_Task(void (*pFunction)(void), uint32_t Delay, uint32_t Period);
int SCH_Delete(struct Task *delTask);


#endif /* INC_SCHEDULER_H_ */
