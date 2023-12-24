/*
 * timer.h
 *
 *  Created on: Dec 24, 2023
 *      Author: atfox
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdlib.h>
#include "stm32f103x6.h"
#include "stm32f1xx_hal.h"
#include "scheduler.h"

#include "main.h"

//#define DEBUG_SIGNAL_COMPARE

#ifdef DEBUG_SIGNAL_COMPARE
#define DEBUG_SIGNAL_TIMER	50	// *10ms
#endif

#define TIMER_CYCLE			10	// *1ms

#endif /* INC_TIMER_H_ */
