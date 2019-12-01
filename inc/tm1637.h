#ifndef TM1637_H
#define TM1637_H
#include "stdint.h"
#include <stdbool.h>
#include "definitions.h"
#include "user_timer.h"

// GPIO definition for the display interface
#define TM1637_PORT_CLK				RCC_APB2Periph_GPIOA		/*!< The GPIO port peripheral clock */
#define TM1637_PORT 				GPIOA						/*!< The GPIO port where the display is connected to */
#define TM1637_DATA_PIN 			GPIO_Pin_1					/*!< The GPIO pin that is connected to the display DATA IO signal */
#define TM1637_CLK_PIN  			GPIO_Pin_2					/*!< The GPIO pin that is connected to the display CLK signal */
#define TM1637_CLK_DELAY  			100							/*!< The delay that defines CLK frequency */
// The type of the 4-Digit display definition
#define DISP_LENGTH					4							/*!< The number of digits the display consist of */
#define DISP_MAX					9999						/*!< The maximal value that can be shown on a display */
#define DISP_MIN					-999						/*!< The minimal value that can be shown on a display */
// Command list
#define DATA_CMD					0x40						/*!< Data command setting mask */
#define DISP_CTRL_CMD				0x80						/*!< Display and control command setting mask */
#define ADR_CMD						0xc0						/*!< Address command setting mask */
// TM1637 Data command settings
#define WRITE_DATA_TO_DISP_REG		0x00						/*!< Write data to display register */
#define READ_KEY_SCAN_DATA			0x02						/*!< Read key scan data */
#define ADDR_AUTO		     		0x00						/*!< The Automatic address adding command */
#define ADDR_FIXED    				0x04						/*!< The Fix address command */
#define NORMAL_MODE					0x00						/*!< Normal mode */
#define TEST_MODE					0x08						/*!< Test mode */
// TM1367 Display control command settings
#define BRIGHTNESS_LEVEL_1			0x00						/*!< Pulse width is set as 1/16 */
#define BRIGHTNESS_LEVEL_2			0x01						/*!< Pulse width is set as 2/16 */
#define BRIGHTNESS_LEVEL_3			0x02						/*!< Pulse width is set as 4/16 */
#define BRIGHTNESS_LEVEL_4			0x03						/*!< Pulse width is set as 10/16 */
#define BRIGHTNESS_LEVEL_5			0x04						/*!< Pulse width is set as 11/16 */
#define BRIGHTNESS_LEVEL_6			0x05						/*!< Pulse width is set as 12/16 */
#define BRIGHTNESS_LEVEL_7			0x06						/*!< Pulse width is set as 13/16 */
#define BRIGHTNESS_LEVEL_8			0x07						/*!< Pulse width is set as 14/16 */
#define DISP_OFF					0x00						/*!< Display OFF command */
#define DISP_ON						0x08						/*!< Display ON command */
// TM1637 Address command settings
#define GRID1_ADR	     			0xc0 						/*!< The address corresponds to the GRID1 */
#define GRID2_ADR	     			0xc1 						/*!< The address corresponds to the GRID2 */
#define GRID3_ADR	     			0xc2 						/*!< The address corresponds to the GRID3 */
#define GRID4_ADR	     			0xc3 						/*!< The address corresponds to the GRID4 */
#define GRID5_ADR	     			0xc4 						/*!< The address corresponds to the GRID5 */
#define GRID6_ADR	     			0xc5 						/*!< The address corresponds to the GRID6 */
// The clock point ON and OFF definitions
#define COLON_ON   					0x80						/*!< The clock points on definition */
#define COLON_OFF					0							/*!< The clock points off definition */
// Special characters index of tube table
#define INDEX_NEGATIVE_SIGN			0x10						/*!< Index of "minus" character mask */
#define INDEX_BLANK			   		0x11						/*!< Index of the blank character mask */
#define DIGIT_MAX_LENGTH			0x12						/*!< The length of the array with digits mask */
#define DIGIT_ERROR					0x00						/*!< Define for error character */
#define ASCII_OFFSET_NUMBER			0x30						/*!< Offset to the ASCII table for numbers */
#define ASCII_OFFSET_STRING			0x37						/*!< Offset to the ASCII table for letters */
#define ZEROS_ON					false
#define ZEROS_OFF					true

// Enum definitions
typedef enum _tm1637_mode_e
{
	tm1637_mode_blinking = 0,
	tm1637_mode_constant_on
} tm1637_mode_e;

//Type definitions
typedef struct _tm1637_t {
	uint8_t brightness_level;									/*!< The number from 0 - 7 that represent brightness level */
	uint8_t on_off_status;										/*!< The display on-off status */
	tm1637_mode_e mode;
} tm1637_t;


// Public functions
/**
 * The function initialize the pins that are used to communicate with the display
 */
void TM1637_init(void);
/**
 * The function write character to the segment seg_n output register
 * @param [in] seg_n - number from 1 to 4 that represent the display that will show the character
 * @param [in] character - ASCII character that should be shown on a segment seg_n
 */
void TM1637_display(uint8_t seg_n, uint8_t character);
/**
 * The function display number on a display
 * @param [in] number - the number that should be shown, in range -999 to 9999
 * @param [in] colon_flag - COLON_ON to show colon between second and third digit or COLON_OFF to not
 */
void TM1637_display_number(int16_t number, uint8_t colon_flag);
/**
 * The function display string that the str pointer point to on a display
 * @param [in] str - the pointer to the array that consist of four character that should be shown
 */
void TM1637_display_string(uint8_t * str);
/**
 * The function set display brightness
 * @param [in] brightness - From RIGHTNESS_LEVEL_1 to RIGHTNESS_LEVEL_8
 * @return true if the brightest level is successfully set, otherwise false
 */
bool TM1637_set_brightness(uint8_t brightness);
/**
 * The function print time on a display
 * @param [in] hours - The number that represents the hours in a range of 0 - 99
 * @param [in] minutes - The number that represents the minutes in a range of 0 - 59
 * @return true if the selected time is in the above range, otherwise false
 */
bool TM1637_display_time(uint8_t hours,uint8_t minutes);
/**
 * The function set BLANK character to each digit and clear the display
 */
void TM1637_clear_display(void);
/**
 * The function turn the display on and off
 * @param [in] status - DISP_OFF to turn the display off, or DISP_ON to turn it on
 */
void TM1637_on_off(uint8_t status);
/**
 * The function toggle display screen status. If
 * the display is on it will be off and vice versa.
 */
void TM1637_toggle(void);

#endif
