/*
 * app_params.h
 *
 *  Created on: Dec 4, 2019
 *      Author: Miroslav
 */

#ifndef APP_PARAMS_H_
#define APP_PARAMS_H_

#include "stm32f10x.h"
#include "definitions.h"
#include "eeprom.h"

#define DEVICE_REG_NUM	((uint8_t)0x11)

#define	IS_INITIALIZED_REG_OFFSET					((uint8_t)0)
#define DEFAULT_POSITION_SETPOINT_REG_OFFSET		((uint8_t)1)
#define DEFAULT_VELOCITY_SETPOINT_REG_OFFSET		((uint8_t)5)
#define MOTOR_OPERATING_VOLTAGE_REG_OFFSET			((uint8_t)6)
#define ENCODER_MAX_COUNT_REG_OFFSET				((uint8_t)7)
#define MOTOR_GEARBOX_RATIO_REG_OFFSET				((uint8_t)8)
#define MOTOR_MAX_VELOCITY_REG_OFFSET				((uint8_t)9)
#define MOTOR_STATUS_REG_OFFSET						((uint8_t)10)
#define P_GAIN_REG_OFFSET							((uint8_t)11)
#define I_GAIN_REG_OFFSET							((uint8_t)13)
#define D_GAIN_REG_OFFSET							((uint8_t)15)

typedef union
{
	uint16_t device_registers[DEVICE_REG_NUM];
	struct __attribute__((packed))
	{
		// Device parameters
		uint16_t	is_initialized;
		// Motion Controller Parameters
		int64_t default_position_setpoint;
		int16_t default_velocity_setpoint;
		// Motor Parameters
		uint16_t motor_operating_voltage;
		uint16_t encoder_max_count;
		uint16_t motor_gearbox_ratio;
		uint16_t motor_max_velocity;
		union
		{
			uint16_t motor_status_reg;
			struct __attribute__((packed))
			{
				unsigned IS_HAVE_GEARBOX	:1;
				unsigned RESERVED1			:15;
			} motor_status_reg_bits;
		};
		// PID Controller Parameters
		float p_gain;
		float i_gain;
		float d_gain;
	};
} device_registers_t;
