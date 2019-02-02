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

#define READ_DATA	0x0000

typedef enum _as5040_read_mode_e
{
	READ_INCREMENTAL = 1,
	READ_SPI
} as5040_read_mode_e;


typedef union
{
	uint16_t data;
	struct __attribute__((packed))
	{
		unsigned ANGULAR_POSITION 	:10;
		unsigned OCF				:1;
		unsigned COF				:1;
		unsigned LIN				:1;
		unsigned MAG_INC			:1;
		unsigned MAG_DEC			:1;
		unsigned EVEN_PAR			:1;
	};
} as5040_register_t;

typedef struct _as5040_t
{
	as5040_read_mode_e read_mode;					/*!< Defined by qei_read_mode_e, A/B quadrature input or SPI read */
	uint16_t max_counts;							/*!< The number of encoder counts for one full revolution */
	as5040_register_t reg_word;						/*!< The as5040 read register */
} as5040_t;

// Private functions declaration
/**
 * The function initialize peripheral that is connected to the AS5040 encoder IC
 * @param [in] r_mode - as5040_read_mode_e, incremental or serial read
 * @param [in] enc_max_count - the number of counts per full revolution of the encoder
 */
void as5040_init(as5040_read_mode_e r_mode, uint16_t enc_max_count);

/**
 * The function read and return angular position using SSI interface
 * @return - Angular position in a range of 0 - 1024
 */
uint16_t as5040_get_angular_position(void);

#endif /* AS5040_H_ */
