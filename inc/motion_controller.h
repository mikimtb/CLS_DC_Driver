/*
 * motion_controller.h
 *
 *  Created on: Feb 1, 2019
 *      Author: Miroslav
 */

#ifndef MOTION_CONTROLLER_H_
#define MOTION_CONTROLLER_H_

#include <stm32f10x.h>
#include <stdbool.h>
#include "console_uart.h"
#include "user_pwm.h"
#include "user_brake.h"
#include "as5040.h"
#include "user_pid.h"

#define MC_TIM					TIM2					/*!< The Timer that is used for Motion Controller interface */
#define MC_TIM_CLK          	RCC_APB1Periph_TIM2		/*!< The Timer peripheral clock definition */
#define MC_IRQn					TIM2_IRQn				/*!< The encoder interface interrupt */
#define MC_IRQHandler			TIM2_IRQHandler			/*!< The encoder interface interrupt handler */

#define PWM_15KHZ				15000							/*!< Define 15KHz PWM frequency */
#define ZERO_DUTY				1200							/*!< Define PWM duty cycle at which the voltage in the H bridge is 0V*/

#define MOVEMENT_THRESHOLD		30					/*!< Minimal movement of the motor shaft that initiates new angular velocity calculation */
#define MAX_NUMBER_OF_CYCLES    4					/*!< Maximal number of cycles before angular velocity calculation is initiated */
#define TIME_STAMP				0.01				/*!< Control loop time stamp in seconds */

/**
 * The enumeration for the direction of rotation
 */
typedef enum
{
	CW = 0, 										/*!< CW - Clockwise rotation */
	CCW = 1 										/*!< CCW - Counter Clockwise rotation */
} dir_e;

typedef enum
{
	STOPPED = 0,
	STARTED
} ctrl_status_e;

typedef enum
{
	NO_GEARBOX = 0,
	GEARBOX
} gearbox_e;

typedef enum
{
	DEACTIVATED = 0,
	ACTIVATED
} brake_status_e;

typedef struct _motor_t
{
	uint8_t max_voltage;
	uint16_t encoder_max_count;
	uint64_t encoder_index_counter;
	gearbox_e is_have_gearbox;
	uint16_t gearbox_ratio;
	uint16_t max_velocity;
} motor_t;

typedef struct _motion_ctrl_t
{
	int64_t angular_position;
	int16_t angular_velocity;
	dir_e direction;
	ctrl_status_e status;
	int64_t position_setpoint;
	int16_t velocity_setpoint;
	int16_t pwm_duty_setpoint;
	brake_status_e brake_status;
	uint16_t enc_index;
	motor_t motor;
} motion_ctrl_t;

// Private functions definition
void motion_controller_init(uint8_t motor_voltage, uint16_t enc_max_cnt, gearbox_e gb_status, uint16_t gb_ratio, uint16_t max_velocity);
void motion_controller_start(void);
void motion_controller_stop(void);
void motion_controller_reset(void);
int64_t motion_controller_get_current_angular_position(void);
int16_t motion_controller_get_current_angular_velocity(void);
int16_t motion_controller_get_current_pwm_duty(void);
ctrl_status_e motion_controller_get_status(void);
void motion_controller_set_angular_postion_setpoint(int16_t ap_setpoint);
void motion_controller_set_angular_velocity_setpoint(int16_t av_setpoint);
int16_t motion_controller_get_angular_velocity_setpoint(void);
void motion_controller_set_pwm_duty(int16_t pwm_duty);



#endif /* MOTION_CONTROLLER_H_ */
