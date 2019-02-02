/*
 * user_brake.h
 *
 *  Created on: Jan 20, 2019
 *      Author: Miroslav
 */

#ifndef USER_BRAKE_H_
#define USER_BRAKE_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define BRAKE_PORT			GPIOA						/*!< The GPIO port where the brake control output is connected to */
#define BRAKE_PORT_CLK     	RCC_APB2Periph_GPIOA		/*!< The brake control pin GPIO port clock */
#define BRAKE_PIN          	GPIO_Pin_11					/*!< The GPIO pin where the brake control input is connected to */

typedef enum
{
	DEACTIVATE = 0,
	ACTIVATE = !DEACTIVATE
} brake_control_t;

/**
 * The function initialize output that is connected to the BRAKE_PORT.PRAKE_PIN
 */
void brake_init();

/**
 * The function control regenerative braking circuit and connects
 * brake resistor using power MOSFET when its needed.
 * @param [in] cmd - control action can be ACTIVATE or DEACTIVATE
 */
void brake_control(brake_control_t cmd);


#endif /* USER_BRAKE_H_ */
