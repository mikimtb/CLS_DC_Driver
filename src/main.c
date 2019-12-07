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

//	uint16_t data1, data2;
//	device_registers_t wparams;


	delay_init();
#ifdef USE_UART_CONSOLE
	console_init();
#endif

//	wparams.is_initialized = 0x59;
//	wparams.default_position_setpoint = 0;
//	wparams.default_velocity_setpoint = 5;
//	wparams.motor_operating_voltage = 12;
//	wparams.encoder_max_count = 128;
//	wparams.motor_gearbox_ratio = 26;
//	wparams.motor_max_velocity = 200;
//	wparams.motor_status_reg_bits.IS_HAVE_GEARBOX = 1;
//	wparams.p_gain = 1;
//	wparams.i_gain = 0;
//	wparams.d_gain = 0;
//
//	app_params_init();
//	//app_params_read_all();
//	app_params_write_all(&wparams);

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

}
