/*
 * rotary_encoder.c
 *
 *  Created on: Jan 25, 2019
 *      Author: Miroslav
 */
#include "qei.h"

// Private functions declaration
static void _encoder_rcc_config(void);
static void _encoder_gpio_config(void);
static void _encoder_tim_config(uint16_t period);

// Public functions implementation
void qei_init(uint16_t enc_pulses_per_revolution)
{
	_encoder_rcc_config();
	_encoder_gpio_config();
	_encoder_tim_config(enc_pulses_per_revolution);
}

uint16_t qei_get_current_count(void)
{
	return TIM_GetCounter(QEI_TIM);
}

void qei_reset_count(void)
{
	TIM_SetCounter(QEI_TIM, QEI_RESET_VALUE);
}

//Private functions implementation
static void _encoder_rcc_config()
{
	// Enable GPIO clock and remap gpio to alternate function
	RCC_APB2PeriphClockCmd(QEI_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	// Enable QEI clock
	RCC_APB1PeriphClockCmd(QEI_CLK, ENABLE);
}

static void _encoder_gpio_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Disable NJTRST that is connected to the PB4
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);
	// Enable the TIM3 Pins Software Remapping
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

	// Config QEI channel A and channel B inputs
	GPIO_InitStructure.GPIO_Pin = QEI_CH_A | QEI_CH_B;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(QEI_GPIO, &GPIO_InitStructure);
}

static void _encoder_tim_config(uint16_t period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(QEI_TIM);

	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(QEI_TIM, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(QEI_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_Cmd(QEI_TIM, ENABLE);
}

