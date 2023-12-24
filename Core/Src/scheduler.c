/*
 * my_scheduler.c
 *
 *  Created on: Nov 28, 2023
 *      Author: atfox
 */

#include <scheduler.h>

struct Task{
	// Task pointer
	void (*pTask) (void);
	// Delay until function will be run (unit ms)
	uint32_t Delay;
	// Period of Task (unit ms)
	uint32_t Period;
	// Task next
	struct Task *next;
} ;

struct Task *head_task;

/*
 * @brief:	Reset list (Force)
 * @para:	none
 * @retval:	none
 * */
void SCH_Init(void) {
//	if(head_task != 0) free(head_task);
	head_task = 0;
}

/*
 * @brief:	Add new task to list
 * @para:	pFunction: pointer of function
 * @para:	Delay: delay until the task is executed (unit: ms)
 * @para:	Period: period of the task	(unit: ms)
 * @retval: Pointer of new task (user can use to delete task later)
 * @cplex:  O(n)
 * */
struct Task *SCH_Add_Task(void (*pFunction)(void), uint32_t Delay, uint32_t Period) {
	// The Complexity of adding task function is O(n)
	struct Task *new_task = (struct Task*)malloc(sizeof(struct Task));
	new_task->pTask = pFunction;
	new_task->Delay = Delay;	// Align time automatically
	new_task->Period = Period;
	new_task->next = 0;
	if(head_task == 0) {		// Empty List
		head_task = new_task;
		return (new_task);
	}
	else {						// 1 more tasks in list
		struct Task *pre_task = 0;
		struct Task *cur_task = head_task;
		uint32_t total_delay = cur_task->Delay;// Total delay time
		// Run to longer task
		while(cur_task != 0) {
			if(new_task->Delay < total_delay) {
				break;
			}
			pre_task = cur_task;
			cur_task = cur_task->next;
			total_delay += (cur_task == 0) ? 0 : cur_task->Delay;
		}
		// Move total delay to the previous node
		total_delay -= (cur_task == 0) ? 0 : cur_task->Delay;

		if(pre_task == 0) {		// New task is shortest task
			new_task->next = head_task;
			head_task = new_task;
		}
		else {					// Exist 1 more task whose time is shorter than new task's time
			new_task->Delay -= total_delay;	// Stack
			pre_task->next = new_task;
			new_task->next = cur_task;
		}
		// Modify next task's delay
		if(cur_task != 0) {
			cur_task->Delay -= new_task->Delay;
		}
		return (new_task);
	}
}

/*
 * @brief:	Delete task (Caution: can change head_task)
 * @para:	Pointer of the task, which user want to delete
 * @retval: Successful / Unsuccessful
 * @cplex:  O(1) - O(n) ("Delete one-shot task" case is O(1))
 * */
int SCH_Delete(struct Task *delTask) {
	// Null-task or Empty list
	if(delTask == 0 || head_task == 0) return 0;

	if(delTask == head_task) {		// Deleted by Dispatcher
		if(head_task->Delay > 0) {	// delTask is not deleted by Dispatcher
			struct Task *pCur = head_task->next;
			if(pCur != 0) {
				pCur->Delay += head_task->Delay;
			}
		}
		head_task = head_task->next;
		delTask->next = 0;
	}
	else {							// delTask is not deleted by Dispatcher
		struct Task *pCur = head_task;
		// Run to previous node of delTask
		while(pCur->next != delTask) {
			if(pCur->next == 0) return 0; // delTask is not exist
			else pCur = pCur->next;
		}
		// Delete node
		pCur->next = delTask->next;
		delTask->next = 0;
		// Modify delay
		pCur = pCur->next;
		if(pCur != 0) {
			pCur->Delay += delTask->Delay;
		}
	}
	// De-allocate
	free(delTask);
	return 1;
}

/*
 * @brief:	Dispatch task (Execution Stage in CompArch :v)
 * @para:	none
 * @retval: none
 * @cplex:
 * */
void SCH_Dispath_Tasks(void) {
	if(head_task->Delay <= 0) {
		// Run task
		(*head_task->pTask)();
		// Renew task
		if(head_task->Period != 0) {
			SCH_Add_Task(head_task->pTask, head_task->Period, head_task->Period);
		}
		// Delete old task
		SCH_Delete(head_task);
	}
}

/*
 * @brief:	Update task's delay (Critical Path in CompArch :v)
 * @para:	none
 * @retval: none
 * @cplex:  O(1)
 * */
void SCH_Update(void) {
	if(head_task != 0) head_task->Delay -= (head_task->Delay > 0) ? TIMER_CYCLE : 0;
}

