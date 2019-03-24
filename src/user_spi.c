/*
 * user_spi.c
 *
 *  Created on: Aug 5, 2018
 *      Author: Miroslav
 */
#include "user_spi.h"

// Private functions
static void _spi_rcc_config();
static void _spi_gpio_config();
static void _spi_config();
static void _spi_nss(BitAction nss_state);

// Public functions implementation
void spi_init(void)
{
	_spi_rcc_config();
	_spi_gpio_config();
	_spi_config();

	_spi_nss(Bit_SET);

	#ifdef USE_UART_CONSOLE
	printf("SPI1 is initialized...\r\n"
			"Mode: SPI Master\r\n"
			"Direction: Direction 2 Lines, Full Duplex\r\n"
			"Data size: 16b\r\n"
			"SPI_CPOL: High\r\n"
			"SPI CPHA: 2Edge\r\n"
			"First Byte: MSB\r\n\r\n");
	#endif
}

uint16_t spi_transfer(uint16_t data)
{
	//_spi_nss(Bit_RESET);

    // Write data to be transmitted to the SPI data register
	SPI_I2S_SendData(SPI_PORT, data);
    // Wait until transmit complete
    while (!(SPI_PORT->SR & (SPI_I2S_FLAG_TXE)));
    // Wait until receive complete
    while (!(SPI_PORT->SR & (SPI_I2S_FLAG_RXNE)));
    // Wait until SPI is not busy anymore
    while (SPI_PORT->SR & (SPI_I2S_FLAG_BSY));

    //_spi_nss(Bit_SET);

    return SPI_I2S_ReceiveData(SPI_PORT);
}

// Private functions implementation
static void _spi_rcc_config()
{
	//RCC_PCLK2Config(RCC_HCLK_Div2);
	// Enable SPI peripheral and GPIO clock
	RCC_APB2PeriphClockCmd(SPI_CLK | SPI_GPIO_CLK, ENABLE);
}

static void _spi_gpio_config()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// MISO, MOSI and CLK configuration
    GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN | SPI_MISO_PIN | SPI_CLK_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
    // GPIO pin for SS
    GPIO_InitStruct.GPIO_Pin = SPI_NSS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
}

static void _spi_config()
{
	SPI_InitTypeDef SPI_InitStruct;

	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		/*!< _256 The SPI CLK is set to the 281.25KHz, Max for AS5040 is 1MHz */
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;

	SPI_Init(SPI_PORT, &SPI_InitStruct);

	/* Enable SPI Receive and Transmit interrupts */
	SPI_I2S_ITConfig(SPI_PORT, SPI_I2S_IT_RXNE, ENABLE);
	SPI_I2S_ITConfig(SPI_PORT, SPI_I2S_IT_TXE, ENABLE);

	SPI_Cmd(SPI_PORT, ENABLE);
}

static void _spi_nss(BitAction nss_state)
{
	GPIO_WriteBit(SPI_GPIO, SPI_NSS_PIN, nss_state);
}

