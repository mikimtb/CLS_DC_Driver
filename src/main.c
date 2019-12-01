/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include <app.h>
#include <user_delay.h>
#include "stm32f10x.h"
#include <string.h>
//#include "stm32f10x_gpio.h"
#include "console_uart.h"
//#include "user_pwm.h"
//#include "user_button.h"
//#include "user_beeper.h"
//#include "user_brake.h"
//#include "tm1637.h"
#include "eeprom.h"
//#include "as5040.h"
#include "motion_controller.h"
//#include "user_timer.h"

//void on_b1_press(void);
//void on_b1_long_press(void);
//void on_b1_click(void);
//
//void on_b2_press(void);
//void on_b2_long_press(void);
//void on_b2_click(void);

//void on_timer_tick(void);
//
//void on_index(void);

//ctimer_t my_timer = { "FSM TIMER", DISABLED, 0, 1000, on_timer_tick};

//button_t *b1;
//button_t *b2;

//button_t b1 = {"START/STOP", RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_12, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 50, 2000, on_b1_press, on_b1_click, on_b1_long_press};
//button_t b2 = {"DOWN", RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_13, ACTIVE_LOW, 0, WAIT_FOR_PRESS, WAIT_FOR_PRESS, 50, 1000, on_b2_press, on_b2_click, on_b2_long_press};

//static tone_t tone1[1] = {{SOUND, T_100_MS}};
//static tone_t tone2[1] = {{SOUND, T_400_MS}};
//static tone_t tone3[12] = {{SOUND, T_100_MS}, {PAUSE, T_100_MS},
//						   {SOUND, T_100_MS}, {PAUSE, T_100_MS},
//						   {SOUND, T_100_MS}, {PAUSE, T_100_MS},
//						   {SOUND, T_100_MS}, {PAUSE, T_300_MS},
//						   {SOUND, T_300_MS}, {PAUSE, T_100_MS},
//						   {SOUND, T_300_MS}, {PAUSE, T_500_MS}};
//
//static beeper_t short_beep = {.m = { .tones = tone1, .tone_max_count = 1}, .beep_type = SINGLE, .tone_count = 0};
//static beeper_t long_beep = {.m = { .tones = tone2, .tone_max_count = 1}, .beep_type = SINGLE, .tone_count = 0};
//static beeper_t alarm = {.m = { .tones = tone3, .tone_max_count = 12}, .beep_type = LOOP, .tone_count = 0};

//void gpio_led_init()
//{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//
//	GPIO_Init(GPIOC, &GPIO_InitStruct);
//}

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NumbOfVar] = {0x0001, 0x0002};

int main(void)
{

	delay_init();
	//gpio_led_init();
	console_init();

	//brake_init();
	//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	//spi_init();
//
//	pwm_init(15000);
//	pwm_set_pulse_width(1200, 1200);
//	delay_ms(10);

	//pwm_set_pulse_width(1200-500, 1200+500);

	//pwm_driver_enable(true);

	/*FLASH_Unlock();
	EE_Init();

	EE_WriteVariable(0x0001, 123);
	EE_WriteVariable(0x0002, 234);

	EE_ReadVariable(0x0001,&spi_data);
	EE_ReadVariable(0x0002, &pom_counter);

	printf("Var1: %u\r\nVar2: %u\r\n", spi_data, pom_counter);*/

//	beeper_init();

//	TM1637_init();
	//TM1637_brightness(0);

//	b1 = button_create("START", RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_12, ACTIVE_LOW, 50, 2000, on_b1_press, on_b1_click, on_b1_long_press);
//	b2 = button_create("STOP", RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_13, ACTIVE_LOW, 50, 1000, on_b2_press, on_b2_click, on_b2_long_press);
//	button_init(&b1);
//	button_init(&b2);
//	TM1637_display_number(1234, COLON_OFF);
//	TM1637_clear_display();
//	TM1637_display_number(-999, COLON_OFF);
//	TM1637_on_off(DISP_OFF);
//	TM1637_on_off(DISP_ON);
//	TM1637_set_brightness(1);
//	TM1637_set_brightness(2);
//	TM1637_set_brightness(3);
//	TM1637_set_brightness(4);
//	TM1637_set_brightness(5);
//	TM1637_set_brightness(6);
//	TM1637_set_brightness(7);
//	TM1637_set_brightness(8);
//	TM1637_display(0, 'A');
//	TM1637_clear_display();
//	TM1637_display(0, '-');
//	TM1637_display(1, '-');
//	TM1637_display(2, '-');
//	TM1637_display(3, '-');
//	TM1637_display_time(12, 54);
	app_init();
	motion_controller_init(12, 128, GEARBOX, 26, 200);
	//brake_control(DEACTIVATE);
	//as5040_init(127, on_index);
//	Ctimer_enable(&my_timer, ENABLE);
	delay_ms(10);
	//TM1637_clearDisplay();



	while (1)
	{
		app_check_for_events();
		app_resolve_events();
//		button_check(&b1);
//		button_check(&b2);
//		timer_check(&my_timer);
//		spi_data = as5040_get_angular_position();
//		printf("%u\r\n", spi_data);
//		spi_data = as5040_get_angular_position();
//		printf("%u\r\n", spi_data);

//		TM1637_clear_display();
//		delay_ms(250);
//		TM1637_display(0, '-');
//		delay_ms(120);
//		TM1637_display(1, '-');
//		delay_ms(140);
//		TM1637_display(2, '-');
//		delay_ms(170);
//		TM1637_display(3, '-');
//		delay_ms(200);

//		GPIO_WriteBit(QEI_GPIO, QEI_CH_A, Bit_SET);
//		GPIO_WriteBit(QEI_GPIO, QEI_CH_B, Bit_SET);
//		delay_ms(10);
//		GPIO_WriteBit(QEI_GPIO, QEI_CH_A, Bit_RESET);
//		GPIO_WriteBit(QEI_GPIO, QEI_CH_B, Bit_RESET);

//		printf("[ %lu ] : %lld : %d \r\n\r\n", millis(), motion_controller_get_current_angular_position(), motion_controller_get_current_angular_velocity());
//		delay_ms(300);
	}
}

//void on_b1_press()
//{
//	printf("%s Press Detected\r\n\r\n",b1.alias);
//}
//void on_b1_long_press()
//{
//	printf("%s Long Press Detected\r\n\r\n", b1.alias);
//	beeper_start(&alarm);
//}
//void on_b1_click()
//{
//	printf("%s Click Detected\r\n\r\n", b1.alias);
//	//beeper_beep(1);
//	beeper_start(&short_beep);
//	motion_controller_start();
//	pwm_set_pulse_width(1200+500, 1200-500);
//}
//
//void on_b2_press()
//{
//	printf("%s Press Detected\r\n\r\n",b2.alias);
//}
//void on_b2_long_press()
//{
//	printf("%s Long Press Detected\r\n\r\n", b2.alias);
//	beeper_stop();
//}
//void on_b2_click()
//{
//	printf("%s Click Detected\r\n\r\n", b2.alias);
//	beeper_start(&long_beep);
//	//motion_controller_stop();
//	pwm_set_pulse_width(1200, 1200);
//}
//
//void on_timer_tick()
//{
//	printf("%s Timer tick\r\n", my_timer.name);
//}
//
//void on_index()
//{
//	printf("Index_Detected\r\n\r\n");
//	pwm_set_pulse_width(1200, 1200);
//}
