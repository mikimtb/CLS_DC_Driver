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

///* Virtual address defined by the user: 0xFFFF value is prohibited */
//uint16_t VirtAddVarTab[NumbOfVar] = {0x0001, 0x0002};

int main(void)
{

//	uint16_t data1, data2;
	device_registers_t wparams;
	FLASH_Status write_status;

	delay_init();
#ifdef USE_UART_CONSOLE
	console_init();
#endif

//	FLASH_Unlock();
//	EE_Init();
//
////	EE_WriteVariable(0x0001, 123);
////	EE_WriteVariable(0x0002, 234);
//
//	EE_ReadVariable(0x0001,&data1);
//	EE_ReadVariable(0x0002, &data2);
//
//	printf("Var1: %u\r\nVar2: %u\r\n", data1, data2);


	app_init();

	wparams.is_initialized = 0x59;
	wparams.default_position_setpoint = 0;
	wparams.default_velocity_setpoint = 5;
	wparams.motor_operating_voltage = 12;
	wparams.encoder_max_count = 32;
	wparams.motor_gearbox_ratio = 26;
	wparams.motor_max_velocity = 200;
	wparams.motor_status_reg_bits.IS_HAVE_GEARBOX = 1;
	wparams.p_gain = 1;
	wparams.i_gain = 0.5;
	wparams.d_gain = 12.6;

	app_params_init();
	app_params_read_all();
	app_params_write_all(&wparams);


	delay_ms(10);

	while (1)
	{
		app_check_for_events();
		app_resolve_events();
	}
}
