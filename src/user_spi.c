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
static void _spi_uart_config();
static void _spi_nvic_config();



// Public functions
void spi_init(void);
bool spi_bputc(uint16_t data);
bool spi_bgetc(uint16_t * data);



