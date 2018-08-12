/*
 * user_pwm.h
 *
 *  Created on: Aug 12, 2018
 *      Author: Miroslav
 */

#ifndef USER_PWM_H_
#define USER_PWM_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include <stdbool.h>
#include "definitions.h"

#define FREQ_MAX  	(SystemCoreClock / 2)			/*!< Maximal frequency definition */
#define FREQ_MIN	(SystemCoreClock / 0xffff)		/*!< Minimal frequency definition */

typedef struct
{
	GPIO_InitTypeDef GPIO_InitStruct;				/*!< GPIO InitStruct that will be assigned to the created PWM port */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;	/*!< Timer time base structure that will be assigned to the desired TIM */
	TIM_OCInitTypeDef  TIM_OCInitStruct;			/*!< Output Compare initialization structure that will be assigned to the PWM port */
	uint16_t frequency;								/*!< The selected frequency */
	uint16_t period;								/*!< Calculated PWM period based on the selected frequency */
	uint16_t ch1_pulse;								/*!< Channel 1 pulse width */
	uint16_t ch2_pulse;								/*!< Channel 2 pulse width */
} pwm_t;

/**
 * The function initialize PWM port
 * Two channels will be configured
 * Timer: TIM1
 * CH1 Port: PA8
 * CH2 Port: PA9
 * PWM Mode: Center Aligned
 * @param [in] pwm_frequency - desired PWM frequency
 * @return true if the peripheral can be configured to produce selected pwm_frequency
 */
bool pwm_init(uint16_t pwm_frequency);

/**
 * The PWM enable/disable peripheral function
 * @param [in] state - ENABLE to enable the output or DISABLE to disable output
 */
void pwm_enable(FunctionalState state);

/**
 * The function sets PWM pulse width if it is not greater then PWM period
 * @param [in] ch1_pulse - Channel one pulse width
 * @param [in] ch2_pulse - Channel two pulse width
 * @return true if pulse width is updated or false if not
 */
bool pwm_set_pulse_width(uint32_t ch1_pulse, uint32_t ch2_pulse);

#endif /* USER_PWM_H_ */
