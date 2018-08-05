/*
 * user_spi.c
 *
 *  Created on: Aug 5, 2018
 *      Author: Miroslav
 */
#include "user_spi.h"

// Private functions
static void _spi_rcc_config();
static void _spi_gpio_config();
static void _spi_nvic_config();
static void _spi_config();



// Public functions implementation
void spi_init(void)
{

}

bool spi_bputc(uint16_t data)
{
	bool result = false;

	return result;
}

bool spi_bgetc(uint16_t * data)
{
	bool result = false;

	return result;
}

// Private functions implementation
static void _spi_rcc_config()
{
	_spi_rcc_config();
	_spi_gpio_config();
	_spi_nvic_config();
	_spi_config();

	#ifdef USE_UART_CONSOLE
	printf("SPI Port Initialized...\r\n\r\n");
	#endif
}

static void _spi_gpio_config()
{

}

static void _spi_config()
{

}

static void _spi_nvic_config()
{

}



