/*
 * my_scheduler.c
 *
 *  Created on: Nov 28, 2023
 *      Author: atfox
 */

#include "my_scheduler.h"

typedef struct {
	// Task pointer
	void (*pTask) (void);
	// Delay until function will be run (unit ms)
	uint32_t Delay;
	// Period of Task (unit ms)
	uint32_t Period;
	// Execution Flag
	uint8_t RunMe;
	// Task's ID
	uint8_t TaskID;
} sTask;

sTask SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_tasks_amount = 0;

void SCH_Init(void) {
	current_tasks_amount = 0;
	for(uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
		SCH_tasks_G[i].pTask  = 0;
		SCH_tasks_G[i].Delay  = 0;
		SCH_tasks_G[i].Period = 0;
		SCH_tasks_G[i].RunMe  = 0;
	}
}

uint8_t SCH_Add_Task (void (*pFunction)(void), uint32_t Delay, uint32_t Period) {
	if(current_tasks_amount < SCH_MAX_TASKS) {
		SCH_tasks_G[current_tasks_amount].pTask  = pFunction;
		SCH_tasks_G[current_tasks_amount].Delay  = Delay / TIMER_CYCLE;
		SCH_tasks_G[current_tasks_amount].Period = Period / TIMER_CYCLE;
		SCH_tasks_G[current_tasks_amount].RunMe  = 0;
		SCH_tasks_G[current_tasks_amount].TaskID = current_tasks_amount;
		current_tasks_amount++;
		return 1;
	}
	else return 0;
}

void SCH_Update(void) {
	for(uint8_t index = 0; index < current_tasks_amount; index++) {
		if(SCH_tasks_G[index].Delay > 0) {
			SCH_tasks_G[index].Delay--;
		}
		else {
			SCH_tasks_G[index].Delay = SCH_tasks_G[index].Period;
			SCH_tasks_G[index].RunMe += 1;
		}
	}
}

void SCH_Dispath_Tasks(void) {
	for(uint8_t index = 0; index < current_tasks_amount; index++) {
		if(SCH_tasks_G[index].RunMe > 0) {
			SCH_tasks_G[index].RunMe -= 1;
			(*SCH_tasks_G[index].pTask)();
			if(SCH_tasks_G[index].Period == 0) {
				SCH_Delete(SCH_tasks_G[index].TaskID);
			}
		}
	}
}

uint8_t SCH_Delete(uint8_t TaskID) {
	if(TaskID == current_tasks_amount) {
		SCH_tasks_G[TaskID].pTask = 0;
		SCH_tasks_G[TaskID].Delay = 0;
		SCH_tasks_G[TaskID].Period = 0;
		SCH_tasks_G[TaskID].RunMe = 0;
		current_tasks_amount--;
		return 1;
	}
	else if (TaskID > current_tasks_amount) {
		return 0;
	}
	else {
		for(uint8_t i = TaskID; i < current_tasks_amount - 1; i++) {
			SCH_tasks_G[i].pTask  = SCH_tasks_G[i + 1].pTask;
			SCH_tasks_G[i].Delay  = SCH_tasks_G[i + 1].Delay;
			SCH_tasks_G[i].Period = SCH_tasks_G[i + 1].Period;
			SCH_tasks_G[i].RunMe  = SCH_tasks_G[i + 1].RunMe;
		}
		SCH_tasks_G[current_tasks_amount - 1].pTask  = 0;
		SCH_tasks_G[current_tasks_amount - 1].Delay  = 0;
		SCH_tasks_G[current_tasks_amount - 1].Period = 0;
		SCH_tasks_G[current_tasks_amount - 1].RunMe  = 0;
		current_tasks_amount--;
		return 1;
	}
}
