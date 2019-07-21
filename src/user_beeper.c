/*
 * user_beeper.c
 *
 *  Created on: Jan 12, 2019
 *      Author: Miroslav
 */
#include "user_beeper.h"
#include "definitions.h"

static void _beeper_rcc_config(void);
static void _beeper_tim_config(void);
static void _beeper_tim_reconfig(uint16_t period);
static void _beeper_nvic_config(void);

static beeper_t *sys_beeper = NULL;		/*!< Pointer to the active beeper */

// IT handlers routine
void BEEPER_IRQHandler()
{
	uint16_t temp_period;

	if (TIM_GetITStatus(BEEPER_TIM, TIM_IT_Update))
	{
		if (sys_beeper->tone_count < sys_beeper->m.tone_max_count)
		{
			// Take the tone or delay duration and update the BEEPER_TIM period
			temp_period = sys_beeper->m.tones[sys_beeper->tone_count].step_duration;
			_beeper_tim_reconfig(temp_period);
			// Check whether the sound should be played or it is time for pause
			switch (sys_beeper->m.tones[sys_beeper->tone_count].step_type)
			{
			case SOUND:
				GPIO_WriteBit(BEEPER_PORT, BEEPER_PIN, Bit_SET);
				break;
			case PAUSE:
				GPIO_WriteBit(BEEPER_PORT, BEEPER_PIN, Bit_RESET);
				break;
			}
			sys_beeper->tone_count++;

			// Reset the timer and clear interrupt flag
			TIM_SetCounter(BEEPER_TIM, 0);
			TIM_ClearITPendingBit(BEEPER_TIM, TIM_IT_Update);
		}
		else
		{
			// Complete sequence is played, check if melody should be repeated in loop
			if (sys_beeper->beep_type == SINGLE)
			{
				// It is single type of melody, disable the timer and turn the beeper off
				TIM_Cmd(BEEPER_TIM, DISABLE);
				GPIO_WriteBit(BEEPER_PORT, BEEPER_PIN, Bit_RESET);

				TIM_ClearITPendingBit(BEEPER_TIM, TIM_IT_Update);
			}
			else
			{
				/* The melody should be repeated in the loop, reset the tone counter
				 * and do not clear interrupt flag to start the melody from teh beginning
				 */
				sys_beeper->tone_count = 0;
			}
		}
	}
}

void beeper_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(BEEPER_PORT_CLK, ENABLE);

    GPIO_InitStruct.GPIO_Pin = BEEPER_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEPER_PORT, &GPIO_InitStruct);

    _beeper_rcc_config();
    _beeper_tim_config();
    _beeper_nvic_config();
}

void beeper_start(beeper_t * b)
{
	// Take the address of the beeper melody that should be played
	sys_beeper = b;
	// Set the melody tone counter to zero to start the reproduction from the beginning
	sys_beeper->tone_count = 0;

	// Set the gpio output
	//GPIO_WriteBit(BEEPER_PORT, BEEPER_PIN, Bit_SET);

	_beeper_tim_reconfig(START_TRIGGER);

	// Reset the BEEPER_TIM to initiate interrupt
	TIM_SetCounter(BEEPER_TIM, 0);
	// Enable the BEEPER_TIM
	TIM_Cmd(BEEPER_TIM, ENABLE);
}

void beeper_stop()
{
	GPIO_WriteBit(BEEPER_PORT, BEEPER_PIN, Bit_RESET);
	TIM_Cmd(BEEPER_TIM, DISABLE);
}

static void _beeper_rcc_config(void)
{
	// Enable Motion Controller TIM clock
	RCC_APB1PeriphClockCmd(BEEPER_TIM_CLK, ENABLE);
}
static void _beeper_tim_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(BEEPER_TIM);

	/* TIM4 Configuration ---------------------------------------------------
	Generate overflow on each 100ms:
	TIM2CLK = SystemCoreClock, Prescaler = 0, TIM4 counter clock = SystemCoreClock
	SystemCoreClock is set to 72 MHz

	The objective is to generate overflow on each 10ms, 100Hz:
	 - Freq = SystemCoreClock / (TIM_Prescaler + 1) * (TIM_Period + 1)
	----------------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Prescaler = 44999;
	TIM_TimeBaseStructure.TIM_Period = T_100_MS;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(BEEPER_TIM, &TIM_TimeBaseStructure);

	TIM_SetCounter(BEEPER_TIM, 0);
	TIM_ClearITPendingBit(BEEPER_TIM, TIM_IT_Update);

	TIM_Cmd(BEEPER_TIM, DISABLE);
}

static void _beeper_tim_reconfig(uint16_t period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = 44999;
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(BEEPER_TIM, &TIM_TimeBaseStructure);
}

static void _beeper_nvic_config()
{
	TIM_ITConfig(BEEPER_TIM, TIM_IT_Update, ENABLE );
	TIM_ClearITPendingBit(BEEPER_TIM, TIM_IT_Update);
	NVIC_EnableIRQ(BEEPER_IRQn);
}
