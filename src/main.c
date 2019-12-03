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
#include "eeprom.h"

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NumbOfVar] = {0x0001, 0x0002};

int main(void)
{

	delay_init();
#ifdef USE_UART_CONSOLE
	console_init();
#endif

	/*FLASH_Unlock();
	EE_Init();

	EE_WriteVariable(0x0001, 123);
	EE_WriteVariable(0x0002, 234);

	EE_ReadVariable(0x0001,&spi_data);
	EE_ReadVariable(0x0002, &pom_counter);

	printf("Var1: %u\r\nVar2: %u\r\n", spi_data, pom_counter);*/


	app_init();
	delay_ms(10);

	while (1)
	{
		app_check_for_events();
		app_resolve_events();
	}
}
