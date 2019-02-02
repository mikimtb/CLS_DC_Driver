/*
 * user_beeper.c
 *
 *  Created on: Jan 12, 2019
 *      Author: Miroslav
 */
#include "user_beeper.h"
#include "definitions.h"
#include "user_delay.h"

void beeper_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(BEEPER_PORT_CLK, ENABLE);

    GPIO_InitStruct.GPIO_Pin = BEEPER_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEPER_PORT, &GPIO_InitStruct);
}

void beeper_beep()
{
	GPIO_WriteBit(BEEPER_PORT, BEEPER_PIN, Bit_SET);
	delay_ms(BEEP_DURATION);
	GPIO_WriteBit(BEEPER_PORT, BEEPER_PIN, Bit_RESET);
}
