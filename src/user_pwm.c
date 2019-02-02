/*
 * user_pwm.c
 *
 *  Created on: Aug 12, 2018
 *      Author: Miroslav
 */
#include "user_pwm.h"

// Private functions
static void _pwm_rcc_config();
static void _pwm_gpio_config();
static void _pwm_config();

// Private variables
static pwm_t pwm = {{0}, {0}, {0}, 0, 0, 0, 0 };	/*!< Global PWM port */

//Public functions implementation
bool pwm_init(uint16_t pwm_frequency)
{
	bool function_completed = false;
	GPIO_InitTypeDef GPIO_InitStruct;

	if((pwm_frequency < FREQ_MAX) && (pwm_frequency > FREQ_MIN))
	{
		pwm.frequency = pwm_frequency;

		_pwm_rcc_config();
		_pwm_gpio_config();
		_pwm_config();

		RCC_APB2PeriphClockCmd(IR2104_DRV_EN_PORT_CLK, ENABLE);

	    GPIO_InitStruct.GPIO_Pin = IR2104_DRV_EN_PIN;
	    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(IR2104_DRV_EN_PORT, &GPIO_InitStruct);

	    GPIO_WriteBit(IR2104_DRV_EN_PORT, IR2104_DRV_EN_PIN, Bit_RESET);

		function_completed = true;

		#ifdef USE_UART_CONSOLE
		printf("TIM1 PWM is initialized...\r\n"
				"Frequency: %d [Hz]\r\n"
				"Period: %d\r\n"
				"CH1 Pulse: %d\r\n"
				"CH2_PUlse: %d\r\n"
				"Alligment: Center Aligned\r\n", pwm.frequency, pwm.period, pwm.ch1_pulse, pwm.ch2_pulse);
		#endif
	}

	return function_completed;
}

void pwm_driver_enable(bool cmd)
{
	GPIO_WriteBit(IR2104_DRV_EN_PORT, IR2104_DRV_EN_PIN, cmd);
}

bool pwm_set_pulse_width(uint32_t ch1_pulse, uint32_t ch2_pulse)
{
	bool function_completed = false;

	if (ch1_pulse < pwm.period && ch2_pulse < pwm.period)
	{
		pwm.ch1_pulse = ch1_pulse;
		pwm.ch2_pulse = ch2_pulse;

		// Update configuration for PWM channel 1
		pwm.TIM_OCInitStruct.TIM_Pulse = pwm.ch1_pulse;
		TIM_OC1Init(TIM1, &pwm.TIM_OCInitStruct);
		// Update configuration for PWM channel 2
		pwm.TIM_OCInitStruct.TIM_Pulse = pwm.ch2_pulse;
		TIM_OC2Init(TIM1, &pwm.TIM_OCInitStruct);

		function_completed = true;
	}

	return function_completed;
}

// Private functions implementation
static void _pwm_rcc_config()
{
	/* TIM1, GPIOA and AFIO clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
}

static void _pwm_gpio_config()
{
	/* GPIOA Configuration: Channel 1 and 2 as alternate function push-pull */
	pwm.GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	pwm.GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	pwm.GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &pwm.GPIO_InitStruct);
}

static void _pwm_config()
{
	/* TIM1 Configuration ---------------------------------------------------
	Generate 2 PWM signals:
	TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
	SystemCoreClock is set to 72 MHz

	The objective is to generate 2 PWM signal at 17.57 KHz:
	 - TIM1_Period_Edge_Aligned = (SystemCoreClock / PWM_freq ) - 1
	 - TIM1_Period_Center_Aligned = (SystemCoreClock / (PWM_freq * 2)) - 1
	The Timer pulse is calculated as follows:
	 - ChannelxPulse = DutyCycle[%] * (TIM1_Period - 1) / 100
	----------------------------------------------------------------------- */
	pwm.period = (SystemCoreClock / (pwm.frequency * 2)) - 1;
	pwm.ch1_pulse = 0;
	pwm.ch2_pulse = 0;

	/* Time Base configuration */
	pwm.TIM_TimeBaseStruct.TIM_Prescaler = 0;
	pwm.TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned3;
	pwm.TIM_TimeBaseStruct.TIM_Period = pwm.period;
	pwm.TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	pwm.TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &pwm.TIM_TimeBaseStruct);

	/* Channel 1 and 2 Configuration in PWM mode */
	pwm.TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	pwm.TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	pwm.TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	pwm.TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	pwm.TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	pwm.TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	// Update configuration for pwm channel 1
	pwm.TIM_OCInitStruct.TIM_Pulse = pwm.ch1_pulse;
	TIM_OC1Init(TIM1, &pwm.TIM_OCInitStruct);
	// Update configuration for pwm channel 2
	pwm.TIM_OCInitStruct.TIM_Pulse = pwm.ch2_pulse;
	TIM_OC2Init(TIM1, &pwm.TIM_OCInitStruct);

	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
