/*
 * user_spi.h
 *
 *  Created on: Aug 5, 2018
 *      Author: Miroslav
 */

#ifndef USER_SPI_H_
#define USER_SPI_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include <stdbool.h>
#include "definitions.h"
#include "console_uart.h"

#define SPI_PORT			SPI1					/*!< The SPI port that is used */
#define SPI_GPIO           	GPIOA					/*!< The GPIO port that is connected to the SPI interface */
#define SPI_CLK            	RCC_APB2Periph_SPI1		/*!< The SPI peripheral clock definition */
#define SPI_GPIO_CLK        RCC_APB2Periph_GPIOA	/*!< The GPIO peripheral clock definition */
#define SPI_CLK_PIN         GPIO_Pin_5				/*!< The SPI CLK pin definition */
#define SPI_MISO_PIN        GPIO_Pin_6				/*!< The SPI MISO pin definition */
#define SPI_MOSI_PIN		GPIO_Pin_7				/*!< The SPI MOSI pin definition */
#define SPI_NSS_PIN			GPIO_Pin_4				/*!< The SPI CS pin definition */
#define SPI_IRQn            SPI1_IRQn				/*!< The SPI interrupt vector address */
#define SPI_IRQHandler      SPI1_IRQHandler		/*!< The SPI interrupt handler function */

// Public functions definition
/**
 * The function initialize SPI port the is defined above
 */
void spi_init(void);

/**
 * The function initiate SPI_transfer
 * Each time the function is called
 * uint16_t data is send to the slave
 * and at the same time the slave response
 * is read
 * @param [in] data - the data that should be sent to the slave device
 * @return the 16bit long data that is received from the slave device
 */
uint16_t spi_transfer(uint16_t data);

#endif /* USER_SPI_H_ */
