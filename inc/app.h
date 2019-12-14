/*
 * fsm.h
 *
 *  Created on: Jul 17, 2019
 *      Author: Miroslav
 */

#ifndef APP_H_
#define APP_H_

#include "definitions.h"
#include <stdbool.h>
#include "htec_ring_buffer.h"
#include "user_clock.h"
#include "tm1637.h"
#include "user_button.h"
#include "user_timer.h"
#include "user_beeper.h"
#include "app_params.h"
#include "motion_controller.h"
#include "config_menu.h"

#define EVENT_BUFFER_SIZE							128
#define CHANGE_VELOCITY_STATE_TIMEOUT_TIME			1500
#define SHOW_VELOCITY_STATE_TIMEOUT_TIME			3000

#define UP_DOWN_BTN_LONG_PRESS_INC_DEC_STEP			((uint8_t)10)
#define UP_DOWN_BTN_CLICK_FLOAT_INC_DEC_STEP		((float)0.05)
#define UP_DOWN_BTN_LONG_PRESS_FLOAT_INC_DEC_STEP	((float)0.5)
#define PID_GAIN_MAX								((float)30.00)
#define MAX_RAMP_TIME								((uint8_t)10)
/**
 * Macro definitions
 * The macros should be used to push and pop events to and from stack
 */
#define PUSH_EVENT(x, y)	ring_buffer_enQ(&x, (uint8_t)y)
#define POP_EVENT(x, y)		ring_buffer_deQ(&x, &y)

enum buttons_e
{
	BTN_START_STOP = 0,
	BTN_UP,
	BTN_DOWN,
	BTN_SET,
	BTN_NUM
};

enum timers_e
{
	FSM_TMR = 0,
	RTC_TMR,
	TMR_NUM
};

typedef enum _display_show_mode_e
{
	ext_control = 0,
	show_clock,
	show_velocity
} display_show_mode_e;

typedef enum _fsm_states_e
{
	init_state = 0,
	run_state,
	change_velocity_state,
	show_velocity_state,
	alarm_state,
	menu_items_state,
	menu_selected_item_state,
	n_states
} fsm_states_e;

typedef enum _fsm_events_e
{
	pwr_up_event = 0,
	btn_start_stop_click_event,
	btn_up_click_event,
	btn_up_long_press_event,
	btn_down_click_event,
	btn_down_long_press_event,
	btn_set_click_event,
	btn_set_long_press_event,
	fsm_timeout_tmr_event,
	fsm_alarm_event,
	n_events
} fsm_events_e;

typedef struct
{
	void (*fsm_state_fptr)(fsm_events_e e);
} state_function_t;

typedef struct
{
	fsm_states_e prevous_state;
	fsm_states_e current_state;
	fsm_events_e current_event;
	fsm_events_e prevous_event;
	display_show_mode_e disp_mode;
	state_function_t state_resolve_function;
} fsm_t;

void app_init(void);
void app_check_for_events(void);
void app_resolve_events(void);


#endif /* APP_H_ */
