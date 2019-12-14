/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f10x.h"
#include <string.h>
#include "user_delay.h"
#include "console_uart.h"
#include "app.h"

#include "app_params.h"

void error_handler(errors_e error_code);

int main(void)
{
	delay_init();
#ifdef USE_UART_CONSOLE
	console_init();
#endif

	app_init();

	delay_ms(10);

	while (1)
	{
		app_check_for_events();
		app_resolve_events();
	}
}

void error_handler(errors_e error_code)
{
	switch (error_code)
	{
		case NO_ERROR:
			break;
		case EEPROM_INIT_FAILED:
			break;
		case EEPROM_READ_FAILED:
			break;
		case EEPROM_WRITE_FAILED:
			break;
	}

	printf("[ERROR]: %u\r\n", error_code);
}
