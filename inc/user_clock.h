/*
 * user_clock.h
 *
 *  Created on: Oct 13, 2019
 *      Author: Miroslav
 */

#ifndef USER_CLOCK_H_
#define USER_CLOCK_H_

#include "definitions.h"
#include "user_timer.h"

typedef struct
{
	functional_states_e status;
	uint8_t minutes;
	uint8_t hours;
} my_clock_t;

void clock_init(void);
my_clock_t clock_get_time(void);
uint8_t clock_get_hours(void);
uint8_t clock_get_minutes(void);
void clock_enable(FunctionalState new_state);
my_clock_t clock_update(void);
void alarm_init(void);
void alarm_set(uint8_t h, uint8_t m);
void alarm_enable(FunctionalState new_state);
bool alarm_update(void);

#endif /* USER_CLOCK_H_ */
