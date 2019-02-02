/*
 * user_beeper.h
 *
 *  Created on: Jan 12, 2019
 *      Author: Miroslav
 */

#ifndef USER_BEEPER_H_
#define USER_BEEPER_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define BEEPER_PORT			GPIOB					/*!< The GPIO port where the beeper is connected to */
#define BEEPER_PORT_CLK     RCC_APB2Periph_GPIOB	/*!< The beeper GPIO port clock */
#define BEEPER_PIN          GPIO_Pin_0				/*!< The GPIO pin where the beeper is connected to */
#define BEEP_DURATION       100						/*!< The beep signal duration in milliseconds */

/**
 * The function initialize beeper connected to the BEEPER_PORT.BEEPER_PIN
 */
void beeper_init();

/**
 * The function produce beep which lasts BEEP_DURATION milliseconds
 */
void beeper_beep();

#endif /* USER_BEEPER_H_ */
