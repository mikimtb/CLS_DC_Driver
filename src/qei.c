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
static void _encoder_nvic_config(void);

// Private variables definition
static qei_event_handler_t _qei_index_event_hdlr;

// Interrupt handlers
void QEI_IRQHandler()
{
	if (TIM_GetITStatus(QEI_TIM, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(QEI_TIM, TIM_IT_Update);

		// Check if the handler is set and call if not NULL
		if(_qei_index_event_hdlr != NULL)
		{
			_qei_index_event_hdlr();
		}
	}
}

// Public functions implementation
void qei_init(uint16_t enc_pulses_per_revolution, qei_event_handler_t qei_index_hdlr)
{
	// Copy index event handler so proper function is
	// called each time the underflow or overflow is detected
	_qei_index_event_hdlr = qei_index_hdlr;

	_encoder_rcc_config();
	_encoder_gpio_config();
	_encoder_tim_config(enc_pulses_per_revolution);
	_encoder_nvic_config();
}

uint16_t qei_get_current_count(void)
{
	return TIM_GetCounter(QEI_TIM);
}

uint16_t qei_get_QEI_CR1()
{
	return QEI_TIM->CR1;
}

void qei_reset_count(void)
{
	NVIC_DisableIRQ(QEI_IRQn);

	TIM_SetCounter(QEI_TIM, QEI_RESET_VALUE);
	TIM_ClearITPendingBit(QEI_TIM, TIM_IT_Update);

	NVIC_EnableIRQ(QEI_IRQn);
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

static void _encoder_nvic_config()
{
	TIM_ITConfig(QEI_TIM, TIM_IT_Update, ENABLE );
	TIM_ClearITPendingBit(QEI_TIM, TIM_IT_Update);
	NVIC_EnableIRQ(QEI_IRQn);
}

