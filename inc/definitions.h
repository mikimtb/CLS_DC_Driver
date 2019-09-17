/*
 * definitions.h
 *
 *  Created on: Jul 15, 2018
 *      Author: Miroslav Bozic
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <stddef.h>

#define USE_UART_CONSOLE 1 											/*!< Define USE_UART_CONSOLE to use console port for the configurations and debug console */
#define CONSOLE_BUFFER_SIZE	32										/*!< UART console ring buffer size definition, used on console_uart.c */

// Push Buttons definition
#define START_STOP_BTN_CLK			RCC_APB2Periph_GPIOB
#define START_STOP_BTN_PORT			GPIOB
#define START_STOP_BTN_PIN			GPIO_Pin_12

#define DOWN_BTN_CLK				RCC_APB2Periph_GPIOB
#define DOWN_BTN_PORT				GPIOB
#define DOWN_BTN_PIN				GPIO_Pin_13

#define UP_BTN_CLK					RCC_APB2Periph_GPIOB
#define UP_BTN_PORT					GPIOB
#define UP_BTN_PIN					GPIO_Pin_15

#define SET_BTN_CLK					RCC_APB2Periph_GPIOB
#define SET_BTN_PORT				GPIOB
#define SET_BTN_PIN					GPIO_Pin_14

#endif /* DEFINITIONS_H_ */
