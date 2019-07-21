/*
 * tone.h
 *
 *  Created on: Jul 21, 2019
 *      Author: Miroslav
 */

#ifndef TONE_H_
#define TONE_H_

#include <stdint.h>

#define T_100_MS 143
#define T_200_MS 287
#define T_300_MS 431
#define T_400_MS 575
#define T_500_MS 719
#define T_600_MS 863
#define T_700_MS 1007
#define T_800_MS 1151
#define T_900_MS 1295

typedef enum _tone_parts_e
{
	SOUND,
	PAUSE
}tone_parts_e;

typedef struct _tone_t
{
	tone_parts_e step_type;				/*!< Defines the SOUND or PAUSE that makes one tone */
	uint16_t step_duration;				/*!< Defines the duration of the SOUND or PAUSE */
}tone_t;

typedef struct _melody_t
{
	tone_t * const tones;				/*!< The pointer to the array of tones that define the melody */
	uint8_t tone_max_count;				/*!< The number of tones and delays in the melody */
}melody_t;

#endif /* TONE_H_ */
