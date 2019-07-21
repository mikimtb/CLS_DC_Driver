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
#include "tone.h"

#define BEEPER_TIM			TIM4						/*!< The beeper timer that is used for beeper delay */
#define BEEPER_TIM_CLK      RCC_APB1Periph_TIM4			/*!< The beeper timer peripheral clock definition */
#define BEEPER_IRQn			TIM4_IRQn					/*!< The beeper timer interface interrupt */
#define BEEPER_IRQHandler	TIM4_IRQHandler				/*!< The beeper timer interface interrupt handler */

#define BEEPER_PORT			GPIOB						/*!< The GPIO port where the beeper is connected to */
#define BEEPER_PORT_CLK     RCC_APB2Periph_GPIOB		/*!< The beeper GPIO port clock */
#define BEEPER_PIN          GPIO_Pin_0					/*!< The GPIO pin where the beeper is connected to */

#define START_TRIGGER		0

typedef enum _beep_type_e
{
	SINGLE,												/*!< The tone is reproduced only once */
	LOOP												/*!< The tone is reproduced until stop function is called */
}beep_type_e;

typedef struct _beeper_t
{
	melody_t m;											/*!< The sequence of tones and delays that define melody */
	beep_type_e beep_type;								/*!< Define a way that the melody is played */
	uint8_t tone_count;									/*!< Operational timer that keep track about the reproduction */
}beeper_t;

/**
 * The function initialize beeper connected to the BEEPER_PORT.BEEPER_PIN
 */
void beeper_init();

/**
 * The function start beeper and reproduce the melody in single or loop mode
 *
 * The melody should be made externally in the caller source file. Once the
 * melody is made its address should be passed to the sys_beeper trough the
 * beeper_start function.
 *
 * The example of the beep melody that should be played only once:
 * static tone_t tone1[1] = {{SOUND, T_100_MS}};
 * static beeper_t short_beep = {.m = { .tones = tone1, .tone_max_count = 1}, .beep_type = SINGLE, .tone_count = 0};
 *
 * The example of the three tones melody that should be played in the loop:
 * static tone_t tone2[6] = {{SOUND, T_100_MS}, {PAUSE, T_100_MS},
 *   					     {SOUND, T_100_MS}, {PAUSE, T_100_MS},
 *   					     {SOUND, T_100_MS}, {PAUSE, T_100_MS}};
 * static beeper_t alarm = {.m = { .tones = tone2, .tone_max_count = 6}, .beep_type = LOOP, .tone_count = 0};
 *
 * @param [in] b - pointer to the melody that should be reproduced
 */
void beeper_start(beeper_t * b);

/**
 * The function disable beep timer and stop reproduction of LOOP melodies
 */
void beeper_stop(void);

#endif /* USER_BEEPER_H_ */
