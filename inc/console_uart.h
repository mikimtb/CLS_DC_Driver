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

#define CONSOLE					USART1
#define CONSOLE_GPIO           	GPIOB
#define CONSOLE_CLK            	RCC_APB2Periph_USART1
#define CONSOLE_GPIO_CLK        RCC_APB2Periph_GPIOB
#define CONSOLE_RxPin           GPIO_Pin_7
#define CONSOLE_TxPin           GPIO_Pin_6
#define CONSOLE_IRQn            USART1_IRQn
#define CONSOLE_IRQHandler      USART1_IRQHandler

/**
 * The function initialize UART console
 * The USART1 port is used for the console with
 * PB6 -> TX and PB7 -> RX pins assigned to the
 * peripheral.
 */
void console_init(void);

#endif

#endif /* CONSOLE_UART_H_ */
