/*
 * as5040.c
 *
 *  Created on: Jul 27, 2018
 *      Author: Miroslav Bozic
 */
#include "as5040.h"

// Private functions declaration
/**
 * The function initialize the SPI peripheral that is used to establish
 * communication with the AS5040 sensor. After initialization the function
 * read angular position and check all flags. If everything goes fine the
 * function return true and the sensor can be used.
 * @return true if the peripheral and the sensor are initialized successfully, otherwise false
 */
static bool _as5040_serial_init(void);
/**
 * The function make low pulse on the AS5040 CSn pin to initiate incremental output controller
 */
static void _as5040_incremental_init(void);
/**
 * The function initialize gpio pin that is connected to the AS5040 CS pin
 */
static void _cs_init(void);

// Private variables definition
static as5040_t _as5040;

// Public functions implementation
void as5040_init(as5040_read_mode_e r_mode, uint16_t enc_max_count)
{
	_as5040.read_mode = r_mode;
	_as5040.max_counts = enc_max_count;
	_as5040.reg_word.data = 0;

	switch (_as5040.read_mode)
	{
	case READ_INCREMENTAL:
		_as5040_incremental_init();

		#ifdef USE_UART_CONSOLE
		printf("AS5040 is initialized...\r\n"
				"Mode: INCREMENTAL\r\n"
				"MAX_CNT: %u\r\n\r\n", _as5040.max_counts);
		#endif

		break;
	case READ_SPI:
		if(_as5040_serial_init())
		{
			#ifdef USE_UART_CONSOLE
			printf("AS5040 is initialized...\r\n"
					"Mode: SERIAL\r\n"
					"MAX_CNT: %u\r\n\r\n", _as5040.max_counts);
			#endif
		}
		else
		{
			#ifdef USE_UART_CONSOLE
			printf("AS5040 initialization failed...\r\n\r\n");
			#endif
		}
		break;
	}
}

uint16_t as5040_get_angular_position()
{
	uint16_t result = 0;

	switch (_as5040.read_mode)
	{
	case READ_INCREMENTAL:
		result = qei_get_current_count();
		break;
	case READ_SPI:
		_as5040.reg_word.data = spi_transfer(READ_DATA);
		result = _as5040.reg_word.ANGULAR_POSITION;
		break;
	}

	return result;
}

static bool _as5040_serial_init()
{
	bool result = false;
	uint8_t retrie_counter = 100;
	spi_init();

	while((_as5040.reg_word.OCF != true) && (retrie_counter != 0))
	{
		_as5040.reg_word.data = spi_transfer(READ_DATA);
		retrie_counter--;
	}

	if (retrie_counter != 0)
	{
		result = true;
	}

	return result;
}

static void _as5040_incremental_init()
{

	qei_init(_as5040.max_counts);
	qei_reset_count();
	_cs_init();
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
    delay_ms(5);
    GPIO_WriteBit(AS5040_CS_GPIO, AS5040_CS_GPIO_PIN, Bit_SET);
}
