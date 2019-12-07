/*
 * definitions.h
 *
 *  Created on: Jul 15, 2018
 *      Author: Miroslav Bozic
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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

// Global typedefs
typedef enum _functional_states_e
{
	DISABLED,								/*!< Functional disable enum */
	ENABLED									/*!< Functional enable enum */
} functional_states_e;

typedef enum _errors_e
{
	NO_ERROR = 0,
	EEPROM_INIT_FAILED,
	EEPROM_READ_FAILED,
	EEPROM_WRITE_FAILED
} errors_e;

#endif /* DEFINITIONS_H_ */
