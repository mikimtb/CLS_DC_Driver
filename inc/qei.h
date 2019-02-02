/*
 * rotary_encoder.h
 *
 *  Created on: Jan 25, 2019
 *      Author: Miroslav
 */

#ifndef QEI_H_
#define QEI_H_

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

#define QEI_TIM				TIM3					/*!< The Timer that is used for QEI interface */
#define QEI_CLK            	RCC_APB1Periph_TIM3		/*!< The Timer peripheral clock definition */
#define QEI_GPIO           	GPIOB					/*!< The GPIO port that is connected to the rotary encoder interface */
#define QEI_GPIO_CLK        RCC_APB2Periph_GPIOB	/*!< The GPIO peripheral clock definition */
#define QEI_CH_A 	        GPIO_Pin_4				/*!< The rotary encoder channel A pin definition */
#define QEI_CH_B	        GPIO_Pin_5				/*!< The rotary encoder channel B pin definition */

#define QEI_RESET_VALUE		0x00					/*!< The rotary encoder Zero position */

void qei_init(uint16_t enc_pulses_per_revolution);
uint16_t qei_get_current_count(void);
void qei_reset_count(void);

#endif /* QEI_H_ */
