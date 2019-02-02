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


typedef enum _gpio_pin_value_e
{
	ACTIVE_LOW = 0,
	ACTIVE_HIGH = 1
} gpio_pin_value_e;

typedef enum _button_states_e
{
	WAIT_FOR_PRESS = 1,
	BUTTON_PRESSED = 2,
	WAIT_FOR_BUTTON_RELEASE = 3,
	BUTTON_RELEASED = 4
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
	uint16_t press_click_debounce;             		/*!< Time in ms for normal press for the button */
	uint16_t press_long_debounce;                  	/*!< Time in ms for long press for the button */
	// Button callback functions
	void (*on_button_pressed_handler)(void);		/*!< Button function handler for button pressed event */
	void (*on_button_clicked_handler)(void);		/*!< Button function handler for button clicked event */
	void (*on_button_long_pressed_handler)(void);	/*!< Button function handler for button clicked event */
} button_t;

// Public function declarations

/**
 * The function create button and return pointer to it.
 *
 * The function returns a pointer to the newly created button and it must be assigned
 * to the pointer to the button_t structure in the function caller side to prevent memory leak!!!
 *
 * @param [in] b_alias - String that define button alias, up to ten characters
 * @param [in] b_clk - Peripheral clock of the port where the button is connected to
 * @param [in] b_port - Port where the button is connected to
 * @param [in] b_pin - Pin where the button is connected to
 * @param [in] b_active_state - Defines button active state, ACTIVE_HIGH or ACTIVE_LOW
 * @param [in] b_click_debounce_time - Debounce delay in milliseconds
 * @param [in] b_long_press_debounce_time - Long press delay in milliseconds
 * @param [in] b_press_handler - Button press callback definition
 * @param [in] b_click_handler - Button click callback definition
 * @param [in] b_long_press_handler - Button long press callback definition
 * @return The pointer to the created button
 */
button_t* button_create(char* b_alias, uint32_t b_clk, GPIO_TypeDef* b_port, uint16_t b_pin,
		gpio_pin_value_e b_active_state, uint16_t b_click_debounce_time, uint16_t b_long_press_debounce_time,
		void (*b_press_handler)(void), void (*b_click_handler)(void), void (*b_long_press_handler)(void));

/**
 * The function check the button status and call subscribed
 * callback function if some button event is generated
 *
 * @param b - the button the status needs to be checked
 */
void button_check(button_t* b);

#endif /* USER_BUTTON_H_ */
