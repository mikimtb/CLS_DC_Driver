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
//#include "stm32f10x_gpio.h"
#include "console_uart.h"
#include "user_spi.h"
#include "user_pwm.h"
#include "control_loop.h"

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

int main(void)
{

//	RCC_ClocksTypeDef RCC_Clocks;
//	RCC_PCLK2Config(RCC_HCLK_Div2);
//	RCC_GetClocksFreq(&RCC_Clocks);
	uint16_t spi_data = 0;
	uint16_t pom_counter = 0;
	delay_init();
	//gpio_led_init();
	console_init();

	//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	spi_init();

	pwm_init(15000);
	pwm_set_pulse_width(1200, 1200);
	delay_ms(10);
	pwm_enable(ENABLE);

	while (1)
	{
		delay_ms(1);
		if(pom_counter < 1199)
		{
			pwm_set_pulse_width(1200 + pom_counter, 1200 - pom_counter);
			pom_counter++;
		}
		else
		{
			pom_counter = 0;
			pwm_enable(DISABLE);
			delay_ms(2000);
			pwm_enable(ENABLE);
		}
		spi_data = spi_transfer(pom_counter);
//		GPIO_SetBits(GPIOC, GPIO_Pin_13);
//		//GPIOC->BSRR |= 1 << 13;
//		//GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
//		//GPIOC->ODR ^= 1 << 13;
//		delay_ms(1000);
//		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//		//GPIOC->BRR |= 1 << 13;
//		//GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
//		//GPIOC->ODR ^= 1 << 13;
//		delay_ms(1000);
	}
}
