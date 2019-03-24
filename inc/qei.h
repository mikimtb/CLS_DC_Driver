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
#include <stddef.h>
#include <stdbool.h>

#define QEI_TIM				TIM3					/*!< The Timer that is used for QEI interface */
#define QEI_CLK            	RCC_APB1Periph_TIM3		/*!< The Timer peripheral clock definition */
#define QEI_GPIO           	GPIOB					/*!< The GPIO port that is connected to the rotary encoder interface */
#define QEI_GPIO_CLK        RCC_APB2Periph_GPIOB	/*!< The GPIO peripheral clock definition */
#define QEI_CH_A 	        GPIO_Pin_4				/*!< The rotary encoder channel A pin definition */
#define QEI_CH_B	        GPIO_Pin_5				/*!< The rotary encoder channel B pin definition */
#define QEI_IRQn			TIM3_IRQn				/*!< The encoder interface interrupt */
#define QEI_IRQHandler		TIM3_IRQHandler			/*!< The encoder interface interrupt handler */

#define QEI_RESET_VALUE		0x00					/*!< The rotary encoder Zero position */

/**
 * The callback type that is used for all
 * events that need to be handled externally
 */
typedef void (*qei_event_handler_t)(void);

/**
 * The function initialize GEI_TIM timer as a incremental encoder
 * input mode and set max number of counts per full revolution
 * @param [in] enc_pulses_per_revolution - number of pulses per revolution in x4 mode, max 65535
 * @param [in] qei_index_hdlr - pointer to the function that should be called each time underflow or overflow is detected
 */
void qei_init(uint16_t enc_pulses_per_revolution, qei_event_handler_t qei_index_hdlr);
/**
 * The function return current value of the QEI_TIM CNT register
 * @return the number of pulses that represent current angular position
 */
uint16_t qei_get_current_count(void);
/**
 * Function return QEI_TIM register that is connected to the incremental encoder
 * @return QEI_TIM->CR1 register
 */
uint16_t qei_get_QEI_CR1(void);
/**
 * The function reset QEI_TIM CNT register to zero
 */
void qei_reset_count(void);

#endif /* QEI_H_ */
