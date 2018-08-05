/*
 * user_spi.h
 *
 *  Created on: Aug 5, 2018
 *      Author: Miroslav
 */

#ifndef USER_SPI_H_
#define USER_SPI_H_

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include <stdbool.h>

#define SPI_PORT			USART1					/*!< The SPI port that is used */
#define SPI_GPIO           	GPIOB					/*!< The GPIO port that is connected to the SPI interface */
#define SPI_CLK            	RCC_APB2Periph_USART1	/*!< The SPI peripheral clock definition */
#define SPI_GPIO_CLK        RCC_APB2Periph_GPIOB	/*!< The GPIO peripheral clock definition */
#define SPI_CLK_PIN         GPIO_Pin_7				/*!< The SPI CLK pin definition */
#define SPI_MISO_PIN        GPIO_Pin_6				/*!< The SPI MISO pin definition */
#define SPI_MOSI_PIN								/*!< The SPI MOSI pin definition */
#define SPI_CS_PIN									/*!< The SPI CS pin definition */
#define SPI_IRQn            USART1_IRQn				/*!< The SPI interrupt vector address */
#define SPI_IRQHandler      USART1_IRQHandler		/*!< The SPI interrupt handler function */

#endif /* USER_SPI_H_ */
