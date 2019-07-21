/*
 * user_timer.h
 *
 *  Created on: Jul 20, 2019
 *      Author: Miroslav
 */

#ifndef USER_TIMER_H_
#define USER_TIMER_H_

#include "stddef.h"
#include "user_delay.h"

typedef enum _timer_states_e
{
	DISABLED,								/*!< Virtual timer disabled */
	ENABLED									/*!< Virtual timer enabled */
} timer_states_e;

typedef struct _ctimer_t
{
	char name[10];							/*!< Virtual timer alias */
	timer_states_e current_state;			/*!< Virtual timer current state, ENABLED or DISABLED */
	uint32_t start_time;					/*!< The time when the timer is enabled */
	uint32_t tick_interval;					/*!< Virtual timer tick interval */
	void (*on_timer_tick_handler)(void);	/*!< Timer thick event handler function */
}ctimer_t;

/**
 * The function enabled or disable tmr timer
 * @param [in] tmr - pointer to the timer that should be enabled
 * @param [in ]new_state - ENABLE or DISABLE
 */
void timer_enable(ctimer_t* tmr, FunctionalState new_state);

/**
 * The function change tick interval of tmr timer
 * @param [in] tmr - pointer to the timer
 * @param [in] new_interval - desired tick interval in milliseconds
 */
void timer_set_tick_interval(ctimer_t* tmr, uint32_t new_interval);

/**
 * The function that check timer status
 *
 * Should be called periodically to keep timer active
 * @param [in] tmr - pointer to the timer that should be checked
 */
void timer_check(ctimer_t* tmr);

#endif /* USER_TIMER_H_ */
