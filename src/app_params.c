/*
 * app_params.c
 *
 *  Created on: Dec 4, 2019
 *      Author: Miroslav
 */
#include "app_params.h"

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NumbOfVar] = {0x0001, 0x0002, 0x0003, 0x0004,
									 0x0005, 0x0006, 0x0007, 0x0008,
									 0x0009, 0x000A, 0x000B, 0x000C,
									 0x000D, 0x000E, 0x000F, 0x0010,
									 0x0011, 0x0012};

static device_registers_t params;

FLASH_Status app_params_init()
{
	FLASH_Unlock();
	return EE_Init();
}

PARAM_Read_Status_e app_params_read_all()
{
	uint8_t i;
	PARAM_Read_Status_e read_status;

	for (i=0; i<NumbOfVar; i++)
	{
		read_status = EE_ReadVariable(VirtAddVarTab[i], &params.device_registers[i]);
		if(read_status != VARIABLE_FOUND)
		{
			break;
		}
	}

	return read_status;
}

FLASH_Status app_params_write_all(device_registers_t * wparams)
{
	uint8_t i;
	FLASH_Status write_status;

	for (i=0; i<NumbOfVar; i++)
	{
		write_status = EE_WriteVariable(VirtAddVarTab[i], wparams->device_registers[i]);
		if(write_status != FLASH_COMPLETE)
		{
			break;
		}
	}

	return write_status;
}

uint16_t app_params_get_device_register(uint8_t device_register_index)
{
	return params.device_registers[device_register_index];
}
uint8_t app_params_get_is_init()
{
	return (uint8_t)params.is_initialized;
}

int64_t app_params_get_deafult_position_setpoint()
{
	return params.default_position_setpoint;
}

int16_t app_params_get_default_velocity_setpoint()
{
	return params.default_velocity_setpoint;
}

uint8_t app_params_get_motor_operating_voltage()
{
	return (uint8_t)params.motor_operating_voltage;
}

uint16_t app_params_get_encoder_max_count()
{
	return params.encoder_max_count;
}

uint16_t app_params_get_motor_gearbox_ration()
{
	return params.motor_gearbox_ratio;
}

uint16_t app_params_get_motor_max_velocity()
{
	return params.motor_max_velocity;
}

bool app_params_get_is_have_gearbox_flag()
{
	return params.motor_status_reg_bits.IS_HAVE_GEARBOX;
}

float app_params_get_P_gain()
{
	return params.p_gain;
}

float app_params_get_I_gain()
{
	return params.i_gain;
}

float app_params_get_D_gain()
{
	return params.d_gain;
}

uint8_t app_params_get_acceleration_time()
{
	return params.start_stop_ramp_regs.acceleration_time;
}
uint8_t app_params_get_deacceleration_time()
{
	return params.start_stop_ramp_regs.deacceleration_time;
}

FLASH_Status app_params_set_is_init(uint16_t ii)
{
	params.is_initialized = ii;
	return EE_WriteVariable(IS_INITIALIZED_START_ADR, params.is_initialized);
}

FLASH_Status app_params_set_deafult_position_setpoint(int64_t dps)
{
	uint8_t i;
	FLASH_Status write_status;

	params.default_position_setpoint = dps;
	for(i=0; i<sizeof(params.default_position_setpoint)/2; i++)
	{
		write_status = EE_WriteVariable(DEFAULT_POSITION_SETPOINT_START_ADR + i, params.device_registers[DEFAULT_POSITION_SETPOINT_START_ADR + i-1]);
		if(write_status != FLASH_COMPLETE)
		{
			break;
		}
	}
	return write_status;
}

FLASH_Status app_params_set_default_velocity_setpoint(int16_t dvs)
{
	params.default_velocity_setpoint = dvs;
	return EE_WriteVariable(DEFAULT_VELOCITY_SETPOINT_START_ADR, params.default_velocity_setpoint);
}

FLASH_Status app_params_set_motor_operating_voltage(uint16_t mov)
{
	params.motor_operating_voltage = mov;
	return EE_WriteVariable(MOTOR_OPERATING_VOLTAGE_START_ADR, params.motor_operating_voltage);
}

FLASH_Status app_params_set_encoder_max_count(uint16_t emc)
{
	params.encoder_max_count = emc;
	return EE_WriteVariable(ENCODER_MAX_COUNT_START_ADR, params.encoder_max_count);
}

FLASH_Status app_params_set_motor_gearbox_ration(uint16_t mgr)
{
	params.motor_gearbox_ratio = mgr;
	return EE_WriteVariable(MOTOR_GEARBOX_RATIO_START_ADR, params.motor_gearbox_ratio);
}

FLASH_Status app_params_set_motor_max_velocity(uint16_t mmv)
{
	params.motor_max_velocity = mmv;
	return EE_WriteVariable(MOTOR_MAX_VELOCITY_START_ADR, params.motor_max_velocity);
}

FLASH_Status app_params_set_is_have_gearbox_flag(bool ihgf)
{
	params.motor_status_reg_bits.IS_HAVE_GEARBOX = ihgf;
	return EE_WriteVariable(MOTOR_STATUS_REG_START_ADR, params.motor_status_reg);
}

FLASH_Status app_params_set_P_gain(float pg)
{
	uint8_t i;
	FLASH_Status write_status;

	params.p_gain = pg;
	for(i=0; i<sizeof(params.p_gain)/2; i++)
	{
		write_status = EE_WriteVariable(P_GAIN_START_ADR + i, params.device_registers[P_GAIN_START_ADR + i-1]);
		if(write_status != FLASH_COMPLETE)
		{
			break;
		}
	}
	return write_status;
}

FLASH_Status app_params_set_I_gain(float ig)
{
	uint8_t i;
	FLASH_Status write_status;

	params.i_gain = ig;
	for(i=0; i<sizeof(params.i_gain)/2; i++)
	{
		write_status = EE_WriteVariable(I_GAIN_START_ADR + i, params.device_registers[I_GAIN_START_ADR + i-1]);
		if(write_status != FLASH_COMPLETE)
		{
			break;
		}
	}
	return write_status;
}

FLASH_Status app_params_set_D_gain(float dg)
{
	uint8_t i;
	FLASH_Status write_status;

	params.d_gain = dg;
	for(i=0; i<sizeof(params.d_gain)/2; i++)
	{
		write_status = EE_WriteVariable(D_GAIN_START_ADR + i, params.device_registers[D_GAIN_START_ADR + i-1]);
		if(write_status != FLASH_COMPLETE)
		{
			break;
		}
	}
	return write_status;
}

FLASH_Status app_params_set_acceleration_time(uint8_t acct)
{
	params.start_stop_ramp_regs.acceleration_time = acct;
	return EE_WriteVariable(START_STOP_RAMP_START_ADR, params.start_stop_ramp);
}

FLASH_Status app_params_set_deacceleration_time(uint8_t deacct)
{
	params.start_stop_ramp_regs.deacceleration_time = deacct;
	return EE_WriteVariable(START_STOP_RAMP_START_ADR, params.start_stop_ramp);
}
