/*
 * user_gpio.h
 *
 *  Created on: Aug 13, 2018
 *      Author: Miroslav
 */

#ifndef USER_BUTTON_H_
#define USER_BUTTON_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "definitions.h"
#include "user_delay.h"

#define PRESS_DEBOUNCE_DELAY 50

typedef enum _gpio_pin_value_e
{
	ACTIVE_LOW = 0,
	ACTIVE_HIGH = 1
} gpio_pin_value_e;

typedef enum _button_states_e
{
	WAIT_FOR_PRESS = 1,
	PRESS_DEBOUNCE,
	BUTTON_PRESSED,
	WAIT_FOR_BUTTON_RELEASE,
	BUTTON_RELEASED
} button_states_e;

// The button descriptor
typedef struct _button_t {
	char alias[10];									/*!< String that define button alias */
	uint32_t RCC_APBxPeriph;						/*!< GPIOx PORT peripheral clock */
	GPIO_TypeDef* GPIOx;                        	/*!< GPIOx PORT for the button */
	uint16_t GPIO_pin;                          	/*!< GPIO pin for the button */
	gpio_pin_value_e button_active_value;         	/*!< GPIO state for pin when pressed */
	uint32_t press_time;                         	/*!< Time when button was pressed */
	button_states_e current_state;                 	/*!< Current button state */
	button_states_e prev_state;						/*!< Previous button state */
	uint16_t press_click_debounce;             		/*!< Time in ms for click for the button */
	uint16_t press_long_debounce;                  	/*!< Time in ms for long press for the button */
	// Button callback functions
	void (*on_button_press_handler)(void);		/*!< Button function handler for button pressed event */
	void (*on_button_click_handler)(void);		/*!< Button function handler for button clicked event */
	void (*on_button_long_press_handler)(void);	/*!< Button function handler for button clicked event */
} button_t;

// Public function declarations

/**
 * The function initialize gpio where the button b is connected
 * @param b - pointer to the button variable
 */
void button_init(button_t * b);

/**
 * The function check the button status and call subscribed
 * callback function if some button event is generated
 *
 * @param b - the button the status needs to be checked
 */
void button_check(button_t* b);

#endif /* USER_BUTTON_H_ */
