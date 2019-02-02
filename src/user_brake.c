/*
 * user_brake.c
 *
 *  Created on: Jan 20, 2019
 *      Author: Miroslav
 */
#include "user_brake.h"

void brake_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(BRAKE_PORT_CLK, ENABLE);

    GPIO_InitStruct.GPIO_Pin = BRAKE_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BRAKE_PORT, &GPIO_InitStruct);
    // Deactivate brake control to prevent high current leakage
    GPIO_WriteBit(BRAKE_PORT, BRAKE_PIN, Bit_SET);
}

void brake_control(brake_control_t cmd)
{
	if(cmd == DEACTIVATE)
	{
		GPIO_WriteBit(BRAKE_PORT, BRAKE_PIN, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(BRAKE_PORT, BRAKE_PIN, Bit_RESET);
	}
}

