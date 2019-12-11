/*
 * fsm.c
 *
 *  Created on: Jul 17, 2019
 *      Author: Miroslav
 */
#include "app.h"

// Extern functions
extern void error_handler(errors_e error_code);

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
static void fsm_menu_item_selected(fsm_events_e e);

// MENU Callback Declaration
static void set_alarm_handler(fsm_events_e e);
static void set_default_position_setpoint_handler(fsm_events_e e);
static void set_default_velocity_setpoint_handler(fsm_events_e e);
static void set_motor_operating_voltage_handler(fsm_events_e e);
static void set_encoder_max_count_handler(fsm_events_e e);
static void set_motor_gearbox_ratio_handler(fsm_events_e e);
static void set_motor_max_velocity_handler(fsm_events_e e);
static void set_motor_is_have_gearbox_handler(fsm_events_e e);
static void set_p_gain_handler(fsm_events_e e);
static void set_i_gain_handler(fsm_events_e e);
static void set_d_gain_handler(fsm_events_e e);

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

my_clock_t temp_alarm = {DISABLED, 0, 0};
bool set_hours_or_minutes = false;


// Finite State Machine Matrix
static state_function_t state_matrix[n_states][n_events] =
{						/* pwr_up_e		/ 	btn_ss_click_e   /		btn_up_click_e 	  	/	btn_down_click_e  /		btn_set_click_e  /			btn_set_long_press_e / 	fsm_timeout_e /	fsm_alarm_e
/* init */				{fsm_run_state, 	NULL, 					NULL, 					NULL, 					NULL, 						NULL, 					NULL, 			NULL			},
/* run */				{NULL, 				fsm_run_state,			fsm_change_vel_state, 	fsm_change_vel_state, 	fsm_show_velocity_state,	fsm_menu_state, 		NULL, 			fsm_alarm_state },
/* change_velocity */	{NULL, 				fsm_run_state, 			fsm_change_vel_state, 	fsm_change_vel_state, 	fsm_show_velocity_state,	NULL, 					fsm_run_state, 	fsm_alarm_state },
/* show_velocity */		{NULL, 				fsm_run_state, 			fsm_change_vel_state, 	fsm_change_vel_state, 	NULL, 						NULL, 					fsm_run_state, 	fsm_alarm_state },
/* alarm */				{NULL, 				fsm_run_state, 			fsm_run_state,		 	fsm_run_state,		 	fsm_run_state,				NULL, 					NULL,		 	NULL		  	},
/* menu */				{NULL, 				NULL, 					fsm_menu_state,			fsm_menu_state, 		fsm_menu_item_selected, 	fsm_run_state, 			NULL, 			NULL 			},
/* menu_item_selected */{NULL, 				fsm_menu_item_selected,	fsm_menu_item_selected, fsm_menu_item_selected, fsm_menu_item_selected, 	fsm_menu_state,			NULL, 			NULL 			}
};

// Configuration Menu Nodes Objects
struct menu_node *config_menu;
// Add as many objects as menu has nodes
struct menu_node set_alarm_node;
struct menu_node set_default_position_setpoint_node;
struct menu_node set_default_velocity_setpoint_node;
struct menu_node set_motor_operating_voltage_node;
struct menu_node set_encoder_max_count_node;
struct menu_node set_motor_gearbox_ratio_node;
struct menu_node set_motor_max_velocity_node;
struct menu_node set_motor_is_have_gearbox_node;
struct menu_node set_p_gain_node;
struct menu_node set_i_gain_node;
struct menu_node set_d_gain_node;

// Finite State Machine Object
fsm_t app_fsm;

// Public functions
void app_init()
{
	uint8_t i = 0;
	device_registers_t dp;

	//***************************************//
	//*   Push Buttons Inits START Here     *//
	//***************************************//
	for (i=0; i<BTN_NUM; i++)
	{
		button_init(&buttons[i]);
	}
	//**************************************//
	//*    Push Buttons Inits ENDS Here    *//
	//************************************* //

	//****************************************************//
	//*    EEPROM Inits and Params Restore START Here    *//
	//****************************************************//
	if (app_params_init() != FLASH_COMPLETE)
	{
		error_handler(EEPROM_INIT_FAILED);
	}

	if (app_params_read_all() != VARIABLE_FOUND)
	{
		error_handler(EEPROM_READ_FAILED);
	}
	//***************************************************//
	//*    EEPROM Inits and Params Restore ENDS Here    *//
	//***************************************************//

	//********************************************//
	//*    Motion Controller Inits START Here    *//
	//********************************************//
	// Copy all parameters to the temp variable
	for (i=0;i<DEVICE_REG_NUM; i++)
	{
		dp.device_registers[i] = app_params_get_device_register(i);
	}
	// Check if the device is initialized
	if (dp.is_initialized != "Y")
	{
		// If not, go to wizard to initialize the device
	}

	motion_controller_init_motor_params(dp.motor_operating_voltage, dp.encoder_max_count, dp.motor_status_reg_bits.IS_HAVE_GEARBOX, dp.motor_gearbox_ratio, dp.motor_max_velocity);
	motion_controller_init_PID_params(dp.p_gain, dp.i_gain, dp.d_gain);
	motion_controller_init_default_setpoints(dp.default_position_setpoint, dp.default_velocity_setpoint);
	//*******************************************//
	//*		 Motion Controller Inits ENDS Here 	*//
	//*******************************************//

	//*********************************//
	//*	   Beeper Inits START Here    *//
	//*********************************//
#ifdef USE_BEEPER
	beeper_init();
#endif
	//beeper_start(&long_beep);
	//********************************//
	//*    Beeper Inits ENDS Here    *//
	//********************************//

	//******************************//
	//*    FSM Inits START Here    *//
	//******************************//
	fsm_init_state(pwr_up_event);
	// Push power up event to buffer to initiate state machine running
	PUSH_EVENT(fsm_events_buff, pwr_up_event);
	//*****************************//
	//*    FSM Inits ENDS Here    *//
	//*****************************//

	//******************************************//
	//*    Clock and Alarm Inits START Here    *//
	//******************************************//
	clock_init();
	clock_enable(ENABLE);
	alarm_init();
	//alarm_set(0, 15);
	//alarm_enable(ENABLE);
	//*****************************************//
	//*    Clock and Alarm Inits ENDS Here    *//
	//*****************************************//

	//**********************************//
	//*    Display Inits START Here    *//
	//**********************************//
	TM1637_init();
	TM1637_set_mode(tm1637_mode_constant_on);
	//TM1637_on_off(DISP_ON);
	//*********************************//
	//*    Display Inits ENDS Here    *//
	//*********************************//

	//*******************************//
	//*    MENU Inits START Here    *//
	//*******************************//
	MENU_build_node(&set_alarm_node, "A   ", NULL, NULL, &set_d_gain_node, &set_default_position_setpoint_node, set_alarm_handler);
	MENU_build_node(&set_default_position_setpoint_node, "P1  ", NULL, NULL, &set_alarm_node, &set_default_velocity_setpoint_node, set_default_position_setpoint_handler);
	MENU_build_node(&set_default_velocity_setpoint_node, "P2  ", NULL, NULL, &set_default_position_setpoint_node, &set_motor_operating_voltage_node, set_default_velocity_setpoint_handler);
	MENU_build_node(&set_motor_operating_voltage_node, "P3  ", NULL, NULL, &set_default_velocity_setpoint_node, &set_encoder_max_count_node, set_motor_operating_voltage_handler);
	MENU_build_node(&set_encoder_max_count_node, "P4  ", NULL, NULL, &set_motor_operating_voltage_node, &set_motor_gearbox_ratio_node, set_encoder_max_count_handler);
	MENU_build_node(&set_motor_gearbox_ratio_node, "P5  ", NULL, NULL, &set_encoder_max_count_node, &set_motor_max_velocity_node, set_motor_gearbox_ratio_handler);
	MENU_build_node(&set_motor_max_velocity_node, "P6  ", NULL, NULL, &set_motor_gearbox_ratio_node, &set_motor_is_have_gearbox_node, set_motor_max_velocity_handler);
	MENU_build_node(&set_motor_is_have_gearbox_node, "P7  ", NULL, NULL, &set_motor_max_velocity_node, &set_p_gain_node, set_motor_is_have_gearbox_handler);
	MENU_build_node(&set_p_gain_node, "P8  ", NULL, NULL, &set_motor_is_have_gearbox_node, &set_i_gain_node, set_p_gain_handler);
	MENU_build_node(&set_i_gain_node, "P9  ", NULL, NULL, &set_p_gain_node, &set_d_gain_node, set_i_gain_handler);
	MENU_build_node(&set_d_gain_node, "P10 ", NULL, NULL, &set_i_gain_node, &set_alarm_node, set_d_gain_handler);

	config_menu = &set_alarm_node;
	//******************************//
	//*    MENU Inits ENDS Here    *//
	//******************************//


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
	PUSH_EVENT(fsm_events_buff, btn_set_click_event);

#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_SET].alias);
#endif
}

static void on_btn_set_long_press()
{
	beeper_start(&long_beep);
	PUSH_EVENT(fsm_events_buff, btn_set_long_press_event);

#ifdef USE_UART_CONSOLE
	printf("%s Long Press \r\n", buttons[BTN_SET].alias);
#endif
}

static void on_fsm_timeout_tmr_tick()
{
	PUSH_EVENT(fsm_events_buff, fsm_timeout_tmr_event);

	timer_enable(&timers[FSM_TMR], DISABLE);

#ifdef USE_UART_CONSOLE
	printf("%s Timer Tick Happened \r\n", timers[FSM_TMR].name);
#endif
}

static void on_rtc_generator_tmr_tick()
{
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
	case ext_control:
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
	case init_state:
		break;
	case run_state:
		break;
	case change_velocity_state:
		app_fsm.disp_mode = show_clock;
		break;
	case show_velocity_state:
		app_fsm.disp_mode = show_clock;
		break;
	case alarm_state:
		beeper_stop();
		break;
	case menu_items_state:
		app_fsm.disp_mode = show_clock;
		break;
	}

	if(e == btn_start_stop_click_event)
	{
		if (motion_controller_get_status() == STOPPED)
		{
			motion_controller_start();
		}
		else
		{
			motion_controller_stop();
		}
	}

#ifdef USE_UART_CONSOLE
	printf("<RUN STATE> \r\n");
#endif
}

static void fsm_change_vel_state(fsm_events_e e)
{
	int16_t vel_set_point;

	app_fsm.prevous_state = app_fsm.current_state;
	app_fsm.current_state = change_velocity_state;

	if(app_fsm.prevous_state == alarm_state)
	{
		beeper_stop();
	}
	else
	{
		app_fsm.disp_mode = ext_control;
		vel_set_point = motion_controller_get_angular_velocity_setpoint();

		if(e == btn_up_click_event)
		{
			vel_set_point++;
		}
		else if (e == btn_down_click_event)
		{
			vel_set_point--;
		}

		motion_controller_set_angular_velocity_setpoint(vel_set_point);
		TM1637_display_number(motion_controller_get_angular_velocity_setpoint(), COLON_OFF);
	}

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

	// Take control against the display
	app_fsm.disp_mode = ext_control;

	switch (e)
	{
	case btn_up_click_event:
		MENU_up(&config_menu);
		TM1637_display_string(config_menu->name);
		break;
	case btn_down_click_event:
		MENU_down(&config_menu);
		TM1637_display_string(config_menu->name);
		break;
	case btn_set_long_press_event:
		if (app_fsm.prevous_state == menu_selected_item_state)
		{
			// Returned from param config, reset temp variables
			// Temp variables for alarm
			temp_alarm.minutes = 0;
			temp_alarm.hours = 0;
			set_hours_or_minutes = false;
		}

		TM1637_set_mode(tm1637_mode_constant_on);
		TM1637_display_string(config_menu->name);
		break;
	}

#ifdef USE_UART_CONSOLE
	printf("<MENU ITEMS STATE> \r\n");
#endif
}

static void fsm_menu_item_selected(fsm_events_e e)
{
	app_fsm.prevous_state = app_fsm.current_state;
	app_fsm.current_state = menu_selected_item_state;

	config_menu->menu_item_handler(e);

#ifdef USE_UART_CONSOLE
	printf("<MENU_ITEM SELECTED STATE> \r\n");
#endif
}

// MENU Callback Declaration
static void set_alarm_handler(fsm_events_e e)
{
	switch (e)
	{
	case btn_up_click_event:
		if (set_hours_or_minutes == false)
		{
			temp_alarm.minutes++;
			if (temp_alarm.minutes >= MINUTES_MAX)
			{
				temp_alarm.minutes = 0;
			}
		}
		else
		{
			temp_alarm.hours++;
			if (temp_alarm.hours >= HOURS_MAX)
			{
				temp_alarm.hours = 0;
			}
		}

		TM1637_display_time(temp_alarm.hours, temp_alarm.minutes);

		break;
	case btn_down_click_event:
		if (set_hours_or_minutes == false)
		{
			if (temp_alarm.minutes > 0)
			{
				temp_alarm.minutes--;
			}
			else
			{
				temp_alarm.minutes = MINUTES_MAX-1;
			}
		}
		else
		{
			if (temp_alarm.hours > 0)
			{
				temp_alarm.hours--;
			}
			else
			{
				temp_alarm.hours = HOURS_MAX - 1;
			}
		}

		TM1637_display_time(temp_alarm.hours, temp_alarm.minutes);

		break;
	case btn_start_stop_click_event:
		if (set_hours_or_minutes == true)
		{
			set_hours_or_minutes = false;
		}
		else
		{
			set_hours_or_minutes = true;
		}
		break;
	case btn_set_click_event:
		if (app_fsm.prevous_state == menu_items_state)
		{
			/*
			 * New settings session is initiated read current
			 * alarm value and show on the display. Start
			 * display blinking if alarm is disabled
			 */
			temp_alarm.minutes = alarm_get_minutes();
			temp_alarm.hours = alarm_get_hours();
			if (alarm_get_enable_status() == DISABLED)
			{
				TM1637_set_mode(tm1637_mode_blinking);
			}
			else
			{
				TM1637_set_mode(tm1637_mode_constant_on);
			}
			TM1637_display_time(temp_alarm.hours, temp_alarm.minutes);
		}
		else
		{
			if (alarm_get_enable_status() == DISABLED)
			{
				alarm_set(temp_alarm.hours, temp_alarm.minutes);
				alarm_enable(ENABLE);
				TM1637_set_mode(tm1637_mode_constant_on);
			}
			else
			{
				alarm_enable(DISABLE);
				TM1637_set_mode(tm1637_mode_blinking);
			}
		}
		break;
	}
#ifdef USE_UART_CONSOLE
	printf("Set_alarm_handler called... \r\n");
#endif
}

static void set_default_position_setpoint_handler(fsm_events_e e)
{
#ifdef USE_UART_CONSOLE
	printf("Set_default_position_setpoint_handler... \r\n");
#endif
}

static void set_default_velocity_setpoint_handler(fsm_events_e e)
{

}

static void set_motor_operating_voltage_handler(fsm_events_e e)
{

}

static void set_encoder_max_count_handler(fsm_events_e e)
{

}

static void set_motor_gearbox_ratio_handler(fsm_events_e e)
{

}

static void set_motor_max_velocity_handler(fsm_events_e e)
{

}

static void set_motor_is_have_gearbox_handler(fsm_events_e e)
{

}

static void set_p_gain_handler(fsm_events_e e)
{

}

static void set_i_gain_handler(fsm_events_e e)
{

}

static void set_d_gain_handler(fsm_events_e e)
{

}

