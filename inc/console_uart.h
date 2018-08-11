/*
 * console_uart.h
 *
 *  Created on: Jul 15, 2018
 *      Author: Miroslav Bozic
 */

#ifndef CONSOLE_UART_H_
#define CONSOLE_UART_H_

#include "definitions.h"

#ifdef USE_UART_CONSOLE
#include "stm32f10x.h"
#include "htec_ring_buffer.h"
#include "stdio.h"

#define CONSOLE					USART1					/*!< The UART port that is used as a console one */
#define CONSOLE_GPIO           	GPIOB					/*!< The port the CONSOLE is connected to */
#define CONSOLE_CLK            	RCC_APB2Periph_USART1	/*!< The CONSOLE peripheral clock definition */
#define CONSOLE_GPIO_CLK        RCC_APB2Periph_GPIOB	/*!< The gpio peripheral clock definition */
#define CONSOLE_RxPin           GPIO_Pin_7				/*!< The Console receive pin definition */
#define CONSOLE_TxPin           GPIO_Pin_6				/*!< The Console transmit pin definition */
#define CONSOLE_IRQn            USART1_IRQn				/*!< The Console interrupt vector address */
#define CONSOLE_IRQHandler      USART1_IRQHandler		/*!< The Console interrupt handler function */

/**
 * The function initialize UART console
 * The USART1 port is used for the console with
 * PB6 -> TX and PB7 -> RX pins assigned to the
 * peripheral.
 */
void console_init(void);

#endif

#endif /* CONSOLE_UART_H_ */
