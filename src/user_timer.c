/*
 * user_timer.c
 *
 *  Created on: Jul 20, 2019
 *      Author: Miroslav
 */
#include "user_timer.h"

void timer_enable(ctimer_t* tmr, FunctionalState new_state)
{
	if (new_state == ENABLE)
	{
		tmr->start_time = millis();
		tmr->current_state = ENABLED;
	}
	else
	{
		tmr->current_state = DISABLED;
	}
}

FunctionalState timer_get_status(ctimer_t *tmr)
{
	return tmr->current_state;
}

void timer_set_tick_interval(ctimer_t* tmr, uint32_t new_interval)
{
	tmr->tick_interval = new_interval;
}

void timer_check(ctimer_t* tmr)
{
	uint32_t elapsed_time = 0;

	if (tmr->current_state == ENABLED)
	{
		elapsed_time = millis() - tmr->start_time;
		if (elapsed_time >= tmr->tick_interval)
		{
			tmr->start_time = millis();
			if (tmr->on_timer_tick_handler != NULL)
			{
				tmr->on_timer_tick_handler();
			}
		}
	}
}

