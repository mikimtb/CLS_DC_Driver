/*
 * control_loop.h
 *
 *  Created on: Jul 15, 2018
 *      Author: Miroslav Bozic
 */

#ifndef USER_DELAY_H_
#define USER_DELAY_H_

#include "stm32f10x.h"

#define systick_freq 1000 /*!< The systick timer frequency definition, 1ms */

// Private functions
/**
 * The function initialize systick timer
 * to be used as 1ms tick source
 */
void delay_init();
/**
 * The function produce delay in milliseconds
 * @param [in] delay - the delay
 */
void delay_ms(uint16_t delay);
/**
 * The function return number of milliseconds,
 * passed from MCU start
 * @return time in milliseconds
 */
uint32_t millis(void);

#endif /* USER_DELAY_H_ */
