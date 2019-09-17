/*
 * fsm.c
 *
 *  Created on: Jul 17, 2019
 *      Author: Miroslav
 */
#include "hmi_fsm.h"

// Callback function prototypes
static void on_btn_start_stop_press(void);
static void on_btn_start_stop_click(void);
static void on_btn_start_stop_long_press(void);

static void on_btn_up_press(void);
static void on_btn_up_click(void);
static void on_btn_up_long_press(void);

static void on_btn_down_press(void);
static void on_btn_down_click(void);
static void on_btn_down_long_press(void);

static void on_btn_set_press(void);
static void on_btn_set_click(void);
static void on_btn_set_long_press(void);

static void on_fsm_timeout_tmr_tick(void);
static void on_rtc_generator_tmr_tick(void);

/*
 * I/O Peripheral declaration
 */
// Push Buttons

button_t buttons[BTN_NUM] = {{"START/STOP", START_STOP_BTN_CLK, START_STOP_BTN_PORT, START_STOP_BTN_PIN, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 40, 1000, on_btn_start_stop_press, on_btn_start_stop_click, on_btn_start_stop_long_press},
							 {"UP", UP_BTN_CLK, UP_BTN_PORT, UP_BTN_PIN, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 40, 1000, on_btn_up_press, on_btn_up_click, on_btn_up_long_press},
							 {"DOWN", DOWN_BTN_CLK, DOWN_BTN_PORT, DOWN_BTN_PIN, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 40, 1000, on_btn_down_press, on_btn_down_click, on_btn_down_long_press},
							 {"SET", SET_BTN_CLK, SET_BTN_PORT, SET_BTN_PIN, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 40, 2000, on_btn_set_press, on_btn_set_click, on_btn_set_long_press}};
// Timers
ctimer_t timers[TMR_NUM] = {{"FSM TMR", DISABLED, 0, 2000, on_fsm_timeout_tmr_tick},
							{"RTC TMR", DISABLED, 0, 1000, on_rtc_generator_tmr_tick}};
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
static beeper_t alarm = {.m = { .tones = tone3, .tone_max_count = 12}, .beep_type = LOOP, .tone_count = 0};

uint8_t min = 0;
uint8_t sec = 0;

// Public functions
void hmi_fsm_init()
{
	//bool result = false;
	uint8_t i = 0;

	// Buttons bsp initialization
	for (i=0; i<BTN_NUM; i++)
	{
		button_init(&buttons[i]);
	}
//	button_init(&b1);
//	button_init(&b2);
//	button_init(&b3);
//	button_init(&b4);
	// Display bsp initialization
	TM1637_init();
	TM1637_display_time(0,0);
	// Beeper bsp initialization
	beeper_init();
	beeper_start(&long_beep);

	timer_enable(&timers[RTC_TMR], ENABLE);
}

void hmi_fsm_check_for_events()
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

// Static functions
static void on_btn_start_stop_press()
{
#ifdef USE_UART_CONSOLE
	printf("%s Press \r\n", buttons[BTN_START_STOP].alias);
#endif
}
static void on_btn_start_stop_click()
{
	beeper_start(&short_beep);
#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_START_STOP].alias);
#endif
}
static void on_btn_start_stop_long_press()
{
#ifdef USE_UART_CONSOLE
	printf("%s Long Press \r\n", buttons[BTN_START_STOP].alias);
#endif
}

static void on_btn_up_press()
{
#ifdef USE_UART_CONSOLE
	printf("%s Press \r\n", buttons[BTN_UP].alias);
#endif
}
static void on_btn_up_click()
{
	beeper_start(&short_beep);
#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_UP].alias);
#endif
}
static void on_btn_up_long_press()
{
#ifdef USE_UART_CONSOLE
	printf("%s Long Press \r\n", buttons[BTN_UP].alias);
#endif
}

static void on_btn_down_press()
{
#ifdef USE_UART_CONSOLE
	printf("%s Press \r\n", buttons[BTN_DOWN].alias);
#endif
}
static void on_btn_down_click()
{
	beeper_start(&short_beep);
#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_DOWN].alias);
#endif
}
static void on_btn_down_long_press()
{
#ifdef USE_UART_CONSOLE
	printf("%s Long Press \r\n", buttons[BTN_DOWN].alias);
#endif
}

static void on_btn_set_press()
{
#ifdef USE_UART_CONSOLE
	printf("%s Press \r\n", buttons[BTN_SET].alias);
#endif
}
static void on_btn_set_click()
{
	beeper_start(&short_beep);
#ifdef USE_UART_CONSOLE
	printf("%s Click \r\n", buttons[BTN_SET].alias);
#endif
}
static void on_btn_set_long_press()
{
#ifdef USE_UART_CONSOLE
	printf("%s Long Press \r\n", buttons[BTN_SET].alias);
#endif
}

static void on_fsm_timeout_tmr_tick()
{

}
static void on_rtc_generator_tmr_tick()
{
	if(!(++sec < 60))
	{
		if(!(++min < 100))
		{
			min = 0;
		}
		sec = 0;
	}

	TM1637_display_time(min, sec);
}
