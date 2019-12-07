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

#define	IS_INITIALIZED_START_ADR					((uint16_t)0x0001)
#define DEFAULT_POSITION_SETPOINT_START_ADR			((uint16_t)0x0002)
#define DEFAULT_VELOCITY_SETPOINT_START_ADR			((uint16_t)0x0006)
#define MOTOR_OPERATING_VOLTAGE_START_ADR			((uint16_t)0x0007)
#define ENCODER_MAX_COUNT_START_ADR					((uint16_t)0x0008)
#define MOTOR_GEARBOX_RATIO_START_ADR				((uint16_t)0x0009)
#define MOTOR_MAX_VELOCITY_START_ADR				((uint16_t)0x000A)
#define MOTOR_STATUS_REG_START_ADR					((uint16_t)0x000B)
#define P_GAIN_START_ADR							((uint16_t)0x000C)
#define I_GAIN_START_ADR							((uint16_t)0x000E)
#define D_GAIN_START_ADR							((uint16_t)0x0010)

typedef enum
{
	VARIABLE_FOUND = 0,
	VARIABLE_NOT_FOUND,
	NO_VALID_PAGE_ERROR = 171
} PARAM_Read_Status_e;

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

FLASH_Status app_params_init(void);
PARAM_Read_Status_e app_params_read_all(void);
FLASH_Status app_params_write_all(device_registers_t * wparams);
uint16_t app_params_get_device_register(uint8_t device_register_index);
uint8_t app_params_get_is_init(void);
int64_t app_params_get_deafult_position_setpoint(void);
int16_t app_params_get_default_velocity_setpoint(void);
uint8_t app_params_get_motor_operating_voltage(void);
uint16_t app_params_get_encoder_max_count(void);
uint16_t app_params_get_motor_gearbox_ration(void);
uint16_t app_params_get_motor_max_velocity(void);
bool app_params_get_is_have_gearbox_flag(void);
float app_params_get_P_gain(void);
float app_params_get_I_gain(void);
float app_params_get_D_gain(void);
FLASH_Status app_params_set_is_init(uint16_t ii);
FLASH_Status app_params_set_deafult_position_setpoint(int64_t dps);
FLASH_Status app_params_set_default_velocity_setpoint(int16_t dvs);
FLASH_Status app_params_set_motor_operating_voltage(uint16_t mov);
FLASH_Status app_params_set_encoder_max_count(uint16_t emc);
FLASH_Status app_params_set_motor_gearbox_ration(uint16_t mgr);
FLASH_Status app_params_set_motor_max_velocity(uint16_t mmv);
FLASH_Status app_params_set_is_have_gearbox_flag(bool ihgf);
FLASH_Status app_params_set_P_gain(float pg);
FLASH_Status app_params_set_I_gain(float ig);
FLASH_Status app_params_set_D_gain(float dg);

#endif /* __APP_PARAMS_H */
