/*
 * as5040.c
 *
 *  Created on: Jul 27, 2018
 *      Author: Miroslav Bozic
 */
#include "as5040.h"

// Private functions declaration
/**
 * The function initialize gpio pin that is connected to the AS5040 CS pin
 */
static void _cs_init(void);

// Private variables definition
static as5040_t _as5040;

// Public functions implementation
void as5040_init(uint16_t enc_max_count, qei_event_handler_t e)
{
	_as5040.max_counts = enc_max_count;

	qei_init(_as5040.max_counts, e);
	qei_reset_count();
	_cs_init();

	#ifdef USE_UART_CONSOLE
	printf("AS5040 is initialized...\r\n"
			"Mode: INCREMENTAL\r\n"
			"MAX_CNT: %u\r\n\r\n", _as5040.max_counts);
	#endif
}

// Private functions implementation
uint16_t as5040_get_angular_position()
{
	uint16_t result = 0;

	result = qei_get_current_count();

	return result;
}

bool as5040_get_direction()
{
	uint16_t qei_cr1_value;

	qei_cr1_value = qei_get_QEI_CR1();

	return (qei_cr1_value >> DIR_FLAG) & BIT_MASK;
}

static void _cs_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(AS5040_CS_GPIO_CLK, ENABLE);
    // GPIO pin for CS
    GPIO_InitStruct.GPIO_Pin = AS5040_CS_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(AS5040_CS_GPIO, &GPIO_InitStruct);

    // Pulse as5040 CSn to initiate IC operation
    GPIO_WriteBit(AS5040_CS_GPIO, AS5040_CS_GPIO_PIN, Bit_SET);
    delay_ms(1);
    GPIO_WriteBit(AS5040_CS_GPIO, AS5040_CS_GPIO_PIN, Bit_RESET);
//    delay_ms(5);
//    GPIO_WriteBit(AS5040_CS_GPIO, AS5040_CS_GPIO_PIN, Bit_SET);
}
