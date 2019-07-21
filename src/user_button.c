/*
 * user_gpio.c
 *
 *  Created on: Aug 13, 2018
 *      Author: Miroslav
 */
#include "user_button.h"

// Public function definitions

void button_init(button_t * b)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(b->RCC_APBxPeriph, ENABLE);

	GPIO_InitStruct.GPIO_Pin = b->GPIO_pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(b->GPIOx, &GPIO_InitStruct);
}

void button_check(button_t* b)
{
	// Read current button status
	uint8_t current_button_value = GPIO_ReadInputDataBit(b->GPIOx, b->GPIO_pin);
	uint32_t elapsed_time = 0;

	switch (b->current_state)
	{
	case WAIT_FOR_PRESS:
		if (current_button_value == b->button_active_value)
		//Button press is detected
		{
			b->press_time = millis();
			if (b->on_button_press_handler != NULL)
			{
				b->on_button_press_handler();
			}
			b->prev_state = b->current_state;
			b->current_state = BUTTON_PRESSED;
		}
		break;
	case BUTTON_PRESSED:
		elapsed_time = millis() - b->press_time;
		if (current_button_value == b->button_active_value)
		{
			if(elapsed_time > b->press_long_debounce)
			{
				// At this point long button press is detected and the callback handler should be called
				if (b->on_button_long_press_handler != NULL)
				{
					b->on_button_long_press_handler();
				}
				b->prev_state = b->current_state;
				b->current_state = WAIT_FOR_BUTTON_RELEASE;
			}
		}
		else
		{
			b->prev_state = b->current_state;
			b->current_state = BUTTON_RELEASED;
		}
		break;
	case WAIT_FOR_BUTTON_RELEASE:
		if (current_button_value != b->button_active_value)
		{
			b->prev_state = b->current_state;
			b->current_state = BUTTON_RELEASED;
		}
		break;
	case BUTTON_RELEASED:
		elapsed_time = millis() - b->press_time;
		if (b->prev_state == BUTTON_PRESSED)
		{
			if(elapsed_time > b->press_click_debounce)
			{
				if(b->on_button_click_handler != NULL)
				{
					b->on_button_click_handler();
				}
			}
		}
		b->prev_state = b->current_state;
		b->current_state = WAIT_FOR_PRESS;
		break;
	}
}

