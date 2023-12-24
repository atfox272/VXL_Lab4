/*
 * timer.c
 *
 *  Created on: Nov 15, 2023
 *      Author: atfox
 */

#include "timer.h"

extern TIM_HandleTypeDef htim2;

#ifdef DEBUG_SIGNAL_COMPARE
extern TIM_HandleTypeDef htim3;
int counter_debug = 0;
#endif

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == htim2.Instance) {
		SCH_Update();
	}

#ifdef DEBUG_SIGNAL_COMPARE
	else if(htim->Instance == htim3.Instance) {
		counter_debug++;
		if(counter_debug >= DEBUG_SIGNAL_TIMER) {
			counter_debug = 0;
			HAL_GPIO_TogglePin(GRE_2_GPIO_Port, GRE_2_Pin);
		}
	}
#endif
}
