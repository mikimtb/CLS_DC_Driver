/*
 * fsm.c
 *
 *  Created on: Jul 17, 2019
 *      Author: Miroslav
 */
#include <app.h>

// Callback function prototypes
//static void on_btn_start_stop_press(void);
static void on_btn_start_stop_click(void);
//static void on_btn_start_stop_long_press(void);

//static void on_btn_up_press(void);
static void on_btn_up_click(void);
//static void on_btn_up_long_press(void);

//static void on_btn_down_press(void);
static void on_btn_down_click(void);
//static void on_btn_down_long_press(void);

//static void on_btn_set_press(void);
static void on_btn_set_click(void);
static void on_btn_set_long_press(void);

static void on_fsm_timeout_tmr_tick(void);
static void on_rtc_generator_tmr_tick(void);

// FSM Callback Declaration
static void fsm_init_state(fsm_events_e e);
static void fsm_run_state(fsm_events_e e);
static void fsm_change_vel_state(fsm_events_e e);
static void fsm_show_velocity_state(fsm_events_e e);
static void fsm_alarm_state(fsm_events_e e);
static void fsm_menu_state(fsm_events_e e);

// Macros
CIRCBUFF_DEF(fsm_events_buff, EVENT_BUFFER_SIZE);

/*
 * I/O Peripheral declaration
 */
// Push Buttons

button_t buttons[BTN_NUM] = {{"START/STOP", START_STOP_BTN_CLK, START_STOP_BTN_PORT, START_STOP_BTN_PIN, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 40, 1000, NULL, on_btn_start_stop_click, NULL},
							 {"UP", UP_BTN_CLK, UP_BTN_PORT, UP_BTN_PIN, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 40, 1000, NULL, on_btn_up_click, NULL},
							 {"DOWN", DOWN_BTN_CLK, DOWN_BTN_PORT, DOWN_BTN_PIN, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 40, 1000, NULL, on_btn_down_click, NULL},
							 {"SET", SET_BTN_CLK, SET_BTN_PORT, SET_BTN_PIN, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 40, 2000, NULL, on_btn_set_click, on_btn_set_long_press}};

//Tones and melodies
static tone_t tone1[1] = {{SOUND, T_100_MS}};
static tone_t tone2[1] = {{SOUND, T_400_MS}};
static tone_t tone3[12] = {{SOUND, T_100_MS}, {PAUSE, T_100_MS},
						   {SOUND, T_100_MS}, {PAUSE, T_100_MS},
						   {SOUND, T_100_MS}, {PAUSE, T_100_MS},
						   {SOUND, T_100_MS}, {PAUSE, T_300_MS},
						   {SOUND, T_300_MS}, {PAUSE, T_100_MS},
						   {SOUND, T_300_MS}, {PAUSE, T_500_MS}};

// Beeps and alarm declaration
static beeper_t short_beep = {.m = { .tones = tone1, .tone_max_count = 1}, .beep_type = SINGLE, .tone_count = 0};
static beeper_t long_beep = {.m = { .tones = tone2, .tone_max_count = 1}, .beep_type = SINGLE, .tone_count = 0};
static beeper_t alarm_beep = {.m = { .tones = tone3, .tone_max_count = 12}, .beep_type = LOOP, .tone_count = 0};

// Timers
ctimer_t timers[TMR_NUM] = {{"FSM TMR", DISABLED, 0, 2000, on_fsm_timeout_tmr_tick},
							{"RTC TMR", DISABLED, 0, 1000, on_rtc_generator_tmr_tick}};

// Finite State Machine Matrix
static state_function_t state_matrix[n_states][n_events] =
{						/* pwr_up_e		/ 	btn_ss_click_e   /	btn_up_click_e 	  	/	btn_down_click_e  /		btn_set_click_e  /			btn_set_long_press_e / 	fsm_timeout_e /	fsm_alarm_e
/* init */				{fsm_run_state, 	NULL, 				NULL, 					NULL, 					NULL, 						NULL, 					NULL, 			NULL			},
/* run */				{NULL, 				fsm_run_state,		fsm_change_vel_state, 	fsm_change_vel_state, 	fsm_show_velocity_state,	fsm_menu_state, 		NULL, 			fsm_alarm_state },
/* change_velocity */	{NULL, 				fsm_run_state, 		fsm_change_vel_state, 	fsm_change_vel_state, 	NULL, 						NULL, 					fsm_run_state, 	fsm_alarm_state },
/* show_velocity */		{NULL, 				NULL, 				NULL, 					NULL,				 	NULL, 						NULL, 					fsm_run_state, 	fsm_alarm_state },
/* alarm */				{NULL, 				fsm_run_state, 		fsm_run_state,		 	fsm_run_state,		 	fsm_run_state,				NULL, 					NULL,		 	NULL,		  	},
/* menu */				{NULL, 				NULL, 				fsm_menu_state, 		fsm_menu_state, 		fsm_menu_state, 			fsm_run_state, 			fsm_run_state, 	fsm_alarm_state }
};

// Finite State Machine Object
fsm_t app_fsm;

// Public functions
void app_init()
{
	uint8_t i = 0;
	// Buttons BSP initialization
	for (i=0; i<BTN_NUM; i++)
	{
		button_init(&buttons[i]);
	}

	// FSM Init
	fsm_init_state(pwr_up_event);
	// Push power up event to buffer to initiate state machine running
	PUSH_EVENT(fsm_events_buff, pwr_up_event);

	clock_init();
	clock_enable(ENABLE);
	alarm_init();
	//alarm_set(0, 15);
	//alarm_enable(ENABLE);

	//Display initialization
	TM1637_init();
	TM1637_on_off(DISP_ON);
	//TM1637_display_time(0,0);

	// Beeper BSP initialization
	beeper_init();
	beeper_start(&long_beep);

	timer_enable(&timers[RTC_TMR], ENABLE);
}

void app_check_for_events()
{
	uint8_t i = 0;
	// Check buttons states
	for(i=0; i<BTN_NUM; i++)
	{
		button_check(&buttons[i]);
	}
	// Check timers states
	for(i=0; i<TMR_NUM; i++)
	{
		timer_check(&timers[i]);
	}
}

void app_resolve_events()
{
	fsm_events_e event;
//	while (ring_buffer_deQ(&fsm_events_buff, &event))
	while (POP_EVENT(fsm_events_buff, event))
	{
		/**
		 * Resolving events starts here
		 */
		app_fsm.prevous_event = app_fsm.current_event;
		app_fsm.current_event = event;
		app_fsm.state_resolve_function.fsm_state_fptr = state_matrix[app_fsm.current_state][app_fsm.current_event].fsm_state_fptr;

		if (app_fsm.state_resolve_function.fsm_state_fptr != NULL)
		{
			app_fsm.state_resolve_function.fsm_state_fptr(event);
		}
		/**
		 * Resolving events ends here
		 */
#ifdef USE_UART_CONSOLE
	printf("Event ID %d has been resolved \r\n", (uint8_t)event);
#endif
	}
}

// Static functions
//static void on_btn_start_stop_press()
//{
//#ifdef USE_UART_CONSOLE
//	printf("%s Press \r\n", buttons[BTN_START_STOP].alias);
//#endif
//}

static void on_btn_start_stop_click()
{
	beeper_start(&short_beep);
	//ring_buffer_enQ(&fsm_events_buff, (uint8_t)btn_start_stop_click_event);
	PUSH_EVENT(fsm_events_buff, btn_start_stop_click_event);

#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_START_STOP].alias);
#endif
}

//static void on_btn_start_stop_long_press()
//{
//#ifdef USE_UART_CONSOLE
//	printf("%s Long Press \r\n", buttons[BTN_START_STOP].alias);
//#endif
//}

//static void on_btn_up_press()
//{
//#ifdef USE_UART_CONSOLE
//	printf("%s Press \r\n", buttons[BTN_UP].alias);
//#endif
//}

static void on_btn_up_click()
{
	beeper_start(&short_beep);
	//ring_buffer_enQ(&fsm_events_buff, (uint8_t)btn_up_click_event);
	PUSH_EVENT(fsm_events_buff, btn_up_click_event);

#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_UP].alias);
#endif
}

//static void on_btn_up_long_press()
//{
//#ifdef USE_UART_CONSOLE
//	printf("%s Long Press \r\n", buttons[BTN_UP].alias);
//#endif
//}

//static void on_btn_down_press()
//{
//#ifdef USE_UART_CONSOLE
//	printf("%s Press \r\n", buttons[BTN_DOWN].alias);
//#endif
//}

static void on_btn_down_click()
{
	beeper_start(&short_beep);
	//ring_buffer_enQ(&fsm_events_buff, (uint8_t)btn_down_click_event);
	PUSH_EVENT(fsm_events_buff, btn_down_click_event);

#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_DOWN].alias);
#endif
}

//static void on_btn_down_long_press()
//{
//#ifdef USE_UART_CONSOLE
//	printf("%s Long Press \r\n", buttons[BTN_DOWN].alias);
//#endif
//}

//static void on_btn_set_press()
//{
//#ifdef USE_UART_CONSOLE
//	printf("%s Press \r\n", buttons[BTN_SET].alias);
//#endif
//}

static void on_btn_set_click()
{
	beeper_start(&short_beep);
	//ring_buffer_enQ(&fsm_events_buff, (uint8_t)btn_set_click_event);
	PUSH_EVENT(fsm_events_buff, btn_set_click_event);

#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_SET].alias);
#endif
}

static void on_btn_set_long_press()
{
	beeper_start(&long_beep);
	//ring_buffer_enQ(&fsm_events_buff, (uint8_t)btn_set_long_press_event);
	PUSH_EVENT(fsm_events_buff, btn_set_long_press_event);

#ifdef USE_UART_CONSOLE
	printf("%s Long Press \r\n", buttons[BTN_SET].alias);
#endif
}

static void on_fsm_timeout_tmr_tick()
{
	//ring_buffer_enQ(&fsm_events_buff, (uint8_t)fsm_timeout_tmr_event);
	PUSH_EVENT(fsm_events_buff, fsm_timeout_tmr_event);

	timer_enable(&timers[FSM_TMR], DISABLE);

#ifdef USE_UART_CONSOLE
	printf("%s Timer Tick Happened \r\n", timers[FSM_TMR].name);
#endif
}

static void on_rtc_generator_tmr_tick()
{
	my_clock_t time;

	clock_update();

	if(alarm_update())
	{
		PUSH_EVENT(fsm_events_buff, fsm_alarm_event);
	}

	switch (app_fsm.disp_mode)
	{
	case show_clock:
		TM1637_display_time(clock_get_hours(), clock_get_minutes());
		break;
	case show_velocity:
		TM1637_display_number(motion_controller_get_current_angular_velocity(), COLON_OFF);
		break;
	}

#ifdef USE_UART_CONSOLE
	printf("%s Timer Tick Happened \r\n", timers[FSM_TMR].name);
#endif
}

// FSM Callback Implementation
static void fsm_init_state(fsm_events_e e)
{
	app_fsm.current_event = e;
	app_fsm.prevous_event = e;
	app_fsm.current_state = init_state;
	app_fsm.prevous_state = init_state;
	app_fsm.disp_mode = show_clock;
	app_fsm.state_resolve_function.fsm_state_fptr = NULL;
}

static void fsm_run_state(fsm_events_e e)
{
	app_fsm.prevous_state = app_fsm.current_state;
	app_fsm.current_state = run_state;

	switch(app_fsm.prevous_state)
	{
	case alarm_state:
		beeper_stop();
		break;
	case show_velocity_state:
		app_fsm.disp_mode = show_clock;
		break;
	case run_state:
		if (motion_controller_get_status() == STOPPED)
		{
			motion_controller_start();
			pwm_set_pulse_width(1200-800, 1200+800);
		}
		else
		{
			motion_controller_stop();
			pwm_set_pulse_width(1200, 1200);
		}
		break;
	}

#ifdef USE_UART_CONSOLE
	printf("<RUN STATE> \r\n");
#endif
}

static void fsm_change_vel_state(fsm_events_e e)
{
	app_fsm.prevous_state = app_fsm.current_state;
	app_fsm.current_state = change_velocity_state;

	timer_set_tick_interval(&timers[FSM_TMR], CHANGE_VELOCITY_STATE_TIMEOUT_TIME);
	timer_enable(&timers[FSM_TMR], ENABLE);

#ifdef USE_UART_CONSOLE
	printf("<CHANGE VELOCITY STATE> \r\n");
#endif
}

static void fsm_show_velocity_state(fsm_events_e e)
{
	app_fsm.prevous_state = app_fsm.current_state;
	app_fsm.current_state = show_velocity_state;

	app_fsm.disp_mode = show_velocity;

	timer_set_tick_interval(&timers[FSM_TMR], SHOW_VELOCITY_STATE_TIMEOUT_TIME);
	timer_enable(&timers[FSM_TMR], ENABLE);

#ifdef USE_UART_CONSOLE
	printf("<SHOW VELOCITY STATE> \r\n");
#endif
}

static void fsm_alarm_state(fsm_events_e e)
{
	app_fsm.prevous_state = app_fsm.current_state;
	app_fsm.current_state = alarm_state;
	// Enable alarm
	beeper_start(&alarm_beep);

#ifdef USE_UART_CONSOLE
	printf("<ALARM STATE> \r\n");
#endif
}

static void fsm_menu_state(fsm_events_e e)
{
	app_fsm.prevous_state = app_fsm.current_state;
	app_fsm.current_state = menu_items_state;

#ifdef USE_UART_CONSOLE
	printf("<MENU STATE> \r\n");
#endif
}
