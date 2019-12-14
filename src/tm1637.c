#include "tm1637.h"
#include "stm32f10x_gpio.h"

// Private functions declaration
static void _on_TM1637_tmr_tick(void);
/**
 * The clock tik generator
 * @param [i] i - the time in clocks that define the clock period
 */
static void _tik_delay(uint32_t i);
/**
 * The function generate START of a message package
 */
static void _generate_start(void);
/**
 * The function generate STOP of a message package
 */
static void _generate_stop(void);
/**
 * The function write data to the display
 * @param [in] data - the data that should be written
 */
static void _write_data(uint8_t data);
/**
 * The function return digit mask of a number num
 * @param [i] num - the number from 0 - 9
 * @return the mask for the desired number
 */
static int8_t _codding(uint8_t num);
/**
 * The function return array of a masks for the passed src array of characters
 * @param [in] src - the pointer to the input array
 * @param [out] dst - the pointer to the output array with the coded data
 * @param [in] colon - The COLON_ON or COLON_OFF depending the needs
 */
static void _codding_all(uint8_t * src, uint8_t * dst, uint8_t colon);
/**
 * The function separate number num to the digits and add it to the array with address dst
 * @param [in] num - The number that should be separated to digits
 * @param [out] dst - The pointer to the destination array with separated digits
 * @param [in] find_zeros - The flag that define whether zeros will be shown or not, TRUE - zeros are not shown, FALSE - zeros are shown
 * @return true if result is successful, otherwise false
 */
static bool _separate_number_to_digits(int16_t num, uint8_t * dst, bool find_zeros);
/**
 * The function convert character to digit that can be shown on a display
 * @param [in] c - the character that should be converted
 * @return true if the character is converted successfully, otherwise false
 */
static uint8_t _convert_char_to_digit(uint8_t c);
/**
 * The function separate string str to the digits and add it to the array with address dst
 * @param [in] str - The pointer to the input array
 * @param [out] dst - The pointer to the destination array with separated digits
 */
static void _separate_string_to_digits(uint8_t * str, uint8_t * dst);

// Private members
static tm1637_t _disp = { .brightness_level = BRIGHTNESS_LEVEL_5,
						  .on_off_status = DISP_OFF,
						  .mode = tm1637_mode_constant_on };		/*!< The display object */

ctimer_t blinking_tmr = {"DISP_TMR", DISABLED, 0, 500, _on_TM1637_tmr_tick};
/**
 * The array with the character masks
 *      A
 *     ---
 *  F |   | B
 *     -G-
 *  E |   | C
 *     ---
 *      D
 */
const uint8_t digit_mask[] =
{				// X    GFEDCBA
		0x3f,   // 0 0b00111111
		0x06,   // 1 0b00000110
		0x5b,   // 2 0b01011011
		0x4f,   // 3 0b01001111
		0x66,   // 4 0b01100110
		0x6d,   // 5 0b01101101
		0x7d,   // 6 0b01111101
		0x07,   // 7 0b00000111
		0x7f,   // 8 0b01111111
		0x6f,   // 9 0b01101111
		0x77,   // A 0b01110111
		0x7c,   // B 0b01111100
		0x39,   // C 0b00111001
		0x5e,   // D 0b01011110
		0x79,   // E 0b01111001
		0x71,   // F 0b01110001
		0x73,	// P 0b01110011
		0x40,   // - 0b01000000
		0x00    // nothing 0b00000000
};

// Public functions implementation
void TM1637_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(TM1637_PORT_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = TM1637_DATA_PIN |TM1637_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TM1637_PORT, &GPIO_InitStructure);

#ifdef USE_UART_CONSOLE
	printf("Display Initialized...\r\n");
#endif
}

void TM1637_display(uint8_t seg_n, uint8_t character)
{
	uint8_t seg_data;

	//_disp.on_off_status = DISP_ON;

	seg_data = _convert_char_to_digit(character);
	seg_data = _codding(seg_data);

	_generate_start();
	_write_data(DATA_CMD | ADDR_AUTO | NORMAL_MODE);
	_generate_stop();
	_generate_start();
	_write_data(ADR_CMD | GRID1_ADR | seg_n);
	_write_data(seg_data);
	_generate_stop();
	_generate_start();
	_write_data(DISP_CTRL_CMD | _disp.on_off_status | _disp.brightness_level);
	_generate_stop();
}

void TM1637_display_number(int16_t number, uint8_t colon_flag)
{
	uint8_t seg_data[4];
	uint8_t i;

	//_disp.on_off_status = DISP_ON;

	if(colon_flag == COLON_ON)
	{
		_separate_number_to_digits(number, seg_data, ZEROS_ON);
	}
	else
	{
		_separate_number_to_digits(number, seg_data, ZEROS_OFF);
	}

	_codding_all(seg_data, seg_data, colon_flag);
	_generate_start();
	_write_data(DATA_CMD | ADDR_AUTO | NORMAL_MODE);
	_generate_stop();
	_generate_start();
	_write_data(ADR_CMD | GRID1_ADR);

	for(i=0; i < DISP_LENGTH; i++)
	{
		_write_data(seg_data[i]);
	}

	_generate_stop();
	_generate_start();
	_write_data(DISP_CTRL_CMD | _disp.on_off_status | _disp.brightness_level);
	_generate_stop();
}

void TM1637_display_string(uint8_t * str)
{
	uint8_t seg_data[4];
	uint8_t i;

	//_disp.on_off_status = DISP_ON;

	_separate_string_to_digits(str, seg_data);
	_codding_all(seg_data, seg_data, COLON_OFF);
	_generate_start();
	_write_data(DATA_CMD | ADDR_AUTO | NORMAL_MODE);
	_generate_stop();
	_generate_start();
	_write_data(ADR_CMD | GRID1_ADR);

	for(i=0; i < DISP_LENGTH; i++)
	{
		_write_data(seg_data[i]);
	}

	_generate_stop();
	_generate_start();
	_write_data(DISP_CTRL_CMD | _disp.on_off_status | _disp.brightness_level);
	_generate_stop();
}

bool TM1637_set_brightness(uint8_t brightness)
{
	bool result = false;

	if((BRIGHTNESS_LEVEL_1 <= brightness) && (brightness <= BRIGHTNESS_LEVEL_8))
	{
		_disp.brightness_level = brightness;

		_generate_start();
		_write_data(DISP_CTRL_CMD | _disp.on_off_status | _disp.brightness_level);
		_generate_stop();

		result = true;
	}

	return result;
}

bool TM1637_display_time(uint8_t hours, uint8_t minutes)
{
	int16_t time;
	bool result = false;

	if((0 <= hours) && (hours <= 99) && (0 <= minutes) && (minutes <= 59))
	{
	  time = (hours * 100) + minutes;
	  result = true;
	}

	TM1637_display_number(time, COLON_ON);

	return result;
}

void TM1637_clear_display()
{
	TM1637_display_number(0, COLON_OFF);
}

void TM1637_on_off(uint8_t status)
{
	_disp.on_off_status = status;

	_generate_start();
	_write_data(DISP_CTRL_CMD | _disp.on_off_status | _disp.brightness_level);
	_generate_stop();
}

void TM1637_toggle()
{
	if(_disp.on_off_status == DISP_ON)
	{
		_disp.on_off_status = DISP_OFF;
	}
	else
	{
		_disp.on_off_status = DISP_ON;
	}

	_generate_start();
	_write_data(DISP_CTRL_CMD | _disp.on_off_status | _disp.brightness_level);
	_generate_stop();
}

void TM1637_set_mode(tm1637_mode_e mode)
{
	_disp.mode = mode;
	if(mode == tm1637_mode_blinking)
	{
		if(timer_get_status(&blinking_tmr) == DISABLED)
		{
			timer_enable(&blinking_tmr, ENABLE);
		}
	}
	else
	{
		timer_enable(&blinking_tmr, DISABLE);
		TM1637_on_off(DISP_ON);
	}
}

tm1637_mode_e TM1637_get_mode()
{
	return _disp.mode;
}

// Private functions implementation
static void _on_TM1637_tmr_tick()
{
	if(_disp.mode == tm1637_mode_blinking)
	{
		TM1637_toggle();
	}

#ifdef USE_DEBUG_CONSOLE
	printf("%s Timer Tick Happened \r\n", blinking_tmr.name);
#endif
}

static void _tik_delay(uint32_t i)
{
	while(i)
	{
		i--;
	}
}

static void _generate_start()
{
	GPIO_SetBits(TM1637_PORT, TM1637_CLK_PIN);
	GPIO_SetBits(TM1637_PORT, TM1637_DATA_PIN);
	_tik_delay(TM1637_CLK_DELAY);
	GPIO_ResetBits(TM1637_PORT, TM1637_DATA_PIN);
 }

 static void _generate_stop()
 {
	 GPIO_ResetBits(TM1637_PORT, TM1637_CLK_PIN);
	 _tik_delay(TM1637_CLK_DELAY);
	 GPIO_ResetBits(TM1637_PORT, TM1637_DATA_PIN);
	 _tik_delay(TM1637_CLK_DELAY);
	 GPIO_SetBits(TM1637_PORT, TM1637_CLK_PIN);
	 _tik_delay(TM1637_CLK_DELAY);
	 GPIO_SetBits(TM1637_PORT, TM1637_DATA_PIN);
 }

static void _write_data(uint8_t data)
{
	uint8_t i;
    for(i=0;i<8;i++)        
    {
    	GPIO_ResetBits(TM1637_PORT, TM1637_CLK_PIN);

    	if(data & 0x01)
    	{
    		GPIO_SetBits(TM1637_PORT, TM1637_DATA_PIN);
    	}
    	else
    	{
    		GPIO_ResetBits(TM1637_PORT, TM1637_DATA_PIN);
    	}

    	_tik_delay(TM1637_CLK_DELAY);
    	data = data >> 1;
    	GPIO_SetBits(TM1637_PORT, TM1637_CLK_PIN);
    	_tik_delay(TM1637_CLK_DELAY);
	}

	GPIO_ResetBits(TM1637_PORT, TM1637_CLK_PIN);
	_tik_delay(TM1637_CLK_DELAY);
	GPIO_ResetBits(TM1637_PORT, TM1637_DATA_PIN);

	while(GPIO_ReadInputDataBit(TM1637_PORT, TM1637_DATA_PIN));

	GPIO_SetBits(TM1637_PORT, TM1637_DATA_PIN);
	GPIO_SetBits(TM1637_PORT, TM1637_CLK_PIN);
	_tik_delay(TM1637_CLK_DELAY);
	GPIO_ResetBits(TM1637_PORT, TM1637_CLK_PIN);
}

static int8_t _codding(uint8_t num)
{
	uint8_t codded_data;

	if(num < DIGIT_MAX_LENGTH)
	{
		codded_data = digit_mask[num];
	}
	else
	{
		codded_data = DIGIT_ERROR;
	}

	return codded_data;
}		

static void _codding_all(uint8_t * src, uint8_t * dst, uint8_t colon)
{
	uint8_t i;

	for(i = 0; i < DISP_LENGTH; i++)
	{
		dst[i] = _codding(src[i]) + colon;
	}
}

static bool _separate_number_to_digits(int16_t num, uint8_t * dst, bool find_zeros)
{
	bool result = true;
	bool find_zeros_flag = find_zeros;
	uint8_t i;


	if ((DISP_MIN <= num) && (num <= DISP_MAX))
	{
		sprintf(dst, "%04d", num);

		for (i=0; i < DISP_LENGTH; i++)
		{
			if(num < 0)
			{
				if((dst[i] == '-') && find_zeros_flag)
				{
					dst[i] = INDEX_NEGATIVE_SIGN;
				}
				else if ((dst[i] == '0') && find_zeros_flag)
				{
					dst[i-1] = INDEX_BLANK;
					dst[i] = INDEX_NEGATIVE_SIGN;
				}
				else
				{
					dst[i] -= ASCII_OFFSET_NUMBER;
					find_zeros_flag = false;
				}
			}
			else
			{
				if((dst[i] == '0') && find_zeros_flag)
				{
					if(i < DISP_LENGTH - 1)
					{
						// If the i doesn't point on the last display character remove zeros
						dst[i] = INDEX_BLANK;
					}
					else
					{
						// i points on the latest character, so zero should be shown so display is not blank
						dst[i] -= ASCII_OFFSET_NUMBER;
					}
				}
				else
				{
					dst[i] -= ASCII_OFFSET_NUMBER;
					find_zeros_flag = false;
				}
			}
		}
	}
	else
	{
		result = false;
	}

	return result;
}

static uint8_t _convert_char_to_digit(uint8_t c)
{
	uint8_t result;

	if(('0' <= c) && (c <= '9'))
	{
		result = c - ASCII_OFFSET_NUMBER;
	}
	else if (('A' <= c) && (c <= 'F'))
	{
		result = c - ASCII_OFFSET_STRING;
	}
	else if(c == 'P')
	{
		result = INDEX_P_CHARACTER;
	}
	else if(c == '-')
	{
		result = INDEX_NEGATIVE_SIGN;
	}
	else
	{
		result = INDEX_BLANK;
	}

	return result;
}

static void _separate_string_to_digits(uint8_t * str, uint8_t * dst)
{
	uint8_t i;

	for (i=0; i < DISP_LENGTH; i++)
	{
		if(('0' <= str[i]) && (str[i] <= '9'))
		{
			dst[i] = str[i] - ASCII_OFFSET_NUMBER;
		}
		else if (('A' <= str[i]) && (str[i] <= 'F'))
		{
			dst[i] = str[i] - ASCII_OFFSET_STRING;
		}
		else if(str[i] == 'P')
		{
			dst[i] = INDEX_P_CHARACTER;
		}
		else if(str[i] == '-')
		{
			dst[i] = INDEX_NEGATIVE_SIGN;
		}
		else
		{
			dst[i] = INDEX_BLANK;
		}
	}
}
