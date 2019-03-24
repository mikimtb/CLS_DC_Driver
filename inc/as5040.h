/*
 * as5040.h
 *
 *  Created on: Jul 27, 2018
 *      Author: Miroslav Bozic
 */

#ifndef AS5040_H_
#define AS5040_H_

#include <stm32f10x.h>
#include <stdbool.h>
#include "user_delay.h"
#include "user_spi.h"
#include "qei.h"

#define AS5040_CS_GPIO			GPIOA						/*!< The GPIO port that is connected to the CS interface */
#define AS5040_CS_GPIO_CLK		RCC_APB2Periph_GPIOA		/*!< The GPIO peripheral clock definition */
#define AS5040_CS_GPIO_PIN		GPIO_Pin_4					/*!< The SPI CS pin definition */

#define DIR_FLAG				0x04						/*!< The position of a DIR flag in the QEI_CR1 register */
#define BIT_MASK				0x01						/*!< The mask that is used to take LSB bit */

typedef struct _as5040_t
{
	uint16_t max_counts;							/*!< The number of encoder counts for one full revolution */
	qei_event_handler_t on_index_riched_hdlr;		/*!< The callback that will be called once encoder index is detected */
} as5040_t;

// Private functions declaration
/**
 * The function initialize peripheral that is connected to the AS5040 encoder IC
 * @param [in] enc_max_count - the number of counts per full revolution of the encoder
 * @param [in] e - the pointer to the function that should be called once index signal is detected
 */
void as5040_init(uint16_t enc_max_count, qei_event_handler_t e);

/**
 * The function read and return angular position using SSI interface
 * @return - Angular position in a range of 0 - 1024
 */
uint16_t as5040_get_angular_position(void);

/**
 * The function return direction of rotation
 * @return 0 if counter counts up (CW) or 1 if counter counts up (CCW)
 */
bool as5040_get_direction(void);

#endif /* AS5040_H_ */
