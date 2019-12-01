/*
 * user_clock.c
 *
 *  Created on: Oct 13, 2019
 *      Author: Miroslav
 */
#include "user_clock.h"

static my_clock_t app_clock;
static my_clock_t app_alarm;

void clock_init()
{
	app_clock.minutes = 0;
	app_clock.hours = 0;
	app_clock.status = DISABLED;
}

my_clock_t clock_get_time(void)
{
	return app_clock;
}

uint8_t clock_get_hours()
{
	return app_clock.hours;
}

uint8_t clock_get_minutes()
{
	return app_clock.minutes;
}

void clock_enable(FunctionalState new_state)
{
	app_clock.status = new_state;
}

my_clock_t clock_update()
{
	if(app_clock.status == ENABLED)
	{
		if(!(++app_clock.minutes < 60))
		{
			if(!(++app_clock.hours < 100))
			{
				app_clock.hours = 0;
			}
			app_clock.minutes = 0;
		}
	}

	return app_clock;
}

void alarm_init()
{
	app_alarm.minutes = 0;
	app_alarm.hours = 0;
	app_alarm.status = DISABLED;
}

void alarm_set(uint8_t h, uint8_t m)
{
	app_alarm.minutes = m+1;
	app_alarm.hours = h;
}

void alarm_enable(FunctionalState new_state)
{
	app_alarm.status = new_state;
}

bool alarm_update()
{
	bool result = false;

	if(app_alarm.status == ENABLED)
	{
		if(!(--app_alarm.minutes > 1))
		{
			if(app_alarm.hours == 0)
			{
				app_alarm.status = DISABLED;
				result = true;
			}

			app_alarm.hours--;
			app_alarm.minutes = 59;
		}
	}

	return result;
}

