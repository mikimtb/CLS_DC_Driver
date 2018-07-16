/*
 * console_uart.c
 *
 *  Created on: Jul 15, 2018
 *      Author: Miroslav Bozic
 */
#include "console_uart.h"

#ifdef USE_UART_CONSOLE

// Private functions
static void _rcc_config();
static void _gpio_config();
static void _uart_config();
static void _nvic_config();
static void console_bputc(uint8_t c);

// Private variables
CIRCBUFF_DEF(console_tx_buff, CONSOLE_BUFFER_SIZE);
CIRCBUFF_DEF(console_rx_buff, CONSOLE_BUFFER_SIZE);

// UART IRQ handler
void CONSOLE_IRQHandler()
{
	/* Check whether CONSOLE IRQ handler is triggered by receive interrupt */
	if (USART_GetITStatus(CONSOLE, USART_IT_RXNE))
	{
		/* Clear receive interrupt flag */
		USART_ClearITPendingBit(CONSOLE, USART_IT_RXNE);
		/* Receive incoming character */
		static uint16_t result_word;

		result_word = USART_ReceiveData(CONSOLE);
		console_bputc((uint8_t)result_word);

		if(!ring_buffer_enQ(&console_rx_buff, (uint8_t)result_word))
		{
			printf("Input buffer overflow exception!\r\n");
		}
	}
	/* Check whether CONSOLE IRQ handler is triggered by transmit interrupt */
	if (USART_GetITStatus(CONSOLE, USART_IT_TXE))
	{
		static uint8_t val;

		if (ring_buffer_deQ(&console_tx_buff, &val))
		{
			USART_SendData(CONSOLE, val);
		}
		else
		{
			/* The buffer is empty so transmit interrupt should be disabled */
			USART_ITConfig(CONSOLE, USART_IT_TXE, DISABLE);
		}
	}
}

// Public function definition
int __io_putchar(int ch)
{
	console_bputc(ch);
	return 0;
}

// Private function definition
static void _rcc_config()
{
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(CONSOLE_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	/* Enable CONSOLE USART Clock */
	RCC_APB2PeriphClockCmd(CONSOLE_CLK, ENABLE);
}

static void _gpio_config()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable the USART1 Pins Software Remapping */
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

	/* Configure USART1 RX as input floating */
	GPIO_InitStruct.GPIO_Pin = CONSOLE_RxPin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CONSOLE_GPIO, &GPIO_InitStruct);

	/* Configure USART1 Tx as alternate function push-pull */
	GPIO_InitStruct.GPIO_Pin = CONSOLE_TxPin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(CONSOLE_GPIO, &GPIO_InitStruct);
}

static void _uart_config()
{
	USART_InitTypeDef USART_InitStruct;

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure Console */
	USART_Init(CONSOLE, &USART_InitStruct);

	/* Enable Console Receive and Transmit interrupts */
	USART_ITConfig(CONSOLE, USART_IT_RXNE, ENABLE);
	USART_ITConfig(CONSOLE, USART_IT_TXE, ENABLE);

	/* Enable the Console */
	USART_Cmd(CONSOLE, ENABLE);
}

static void _nvic_config()
{
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel = CONSOLE_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Enable the USART1 Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel = CONSOLE_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

// Public functions
void console_init()
{
	int a;

	_rcc_config();
	_gpio_config();
	_nvic_config();
	_uart_config();

	setvbuf(stdin, NULL, _IONBF, 0);

	printf("Console initialized...\r\n\r\n");
	scanf("%d", &a);

	printf("\r\na = %d", a);
}

static void console_bputc(uint8_t c)
{
	bool restart = ring_buffer_isEmpty(&console_tx_buff);
	while(!ring_buffer_enQ(&console_tx_buff, c));
	if(restart)
	{
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}

#endif
