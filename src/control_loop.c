/*
 * control_loop.c
 *
 *  Created on: Jul 15, 2018
 *      Author: Miroslav Bozic
 */
#include "control_loop.h"

volatile uint32_t _millis = 0;		/*!< Global variable that keeps system time */

void SysTick_Handler (void)
 {
   _millis++;
 }

void delay_init()
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);
	(void) SysTick_Config (RCC_Clocks.HCLK_Frequency / systick_freq);
}

void delay_ms(uint16_t delay)
{
	uint32_t delay_end = _millis + delay;
	while (_millis < delay_end);
}

uint32_t millis()
{
	return _millis;
}
