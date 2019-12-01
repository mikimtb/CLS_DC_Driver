/*
 * control_loop.c
 *
 *  Created on: Jul 15, 2018
 *      Author: Miroslav Bozic
 */
#include <user_delay.h>

extern ctimer_t blinking_tmr;

volatile uint32_t _millis = 0;		/*!< Global variable that keeps system time */

void SysTick_Handler (void)
 {
   _millis++;

   /*
    * Check TM1637 blinking timer starts here
    */
   timer_check(&blinking_tmr);
   /*
    * Check TM1637 blinking timer ends here
    */
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
