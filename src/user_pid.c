/*
 * user_pid.c
 *
 *  Created on: Mar 24, 2019
 *      Author: Miroslav
 */
#include "user_pid.h"

void mpid_init(mpid_c_t * plant_pid, int16_t max_out, int16_t min_out)
{
	// Anti-windup
	plant_pid->i_max = max_out / plant_pid->i_gain;
	plant_pid->i_min = min_out / plant_pid->i_gain;

	plant_pid->d_state = 0.0;
	plant_pid->i_state = 0.0;
}

float mpid_update(mpid_c_t * plant_pid, int16_t plant_error, int16_t plant_current_value)
{
	float p_term;
	float i_term;
	float d_term;

	// Calculate P term
	p_term = plant_pid->p_gain * plant_error;

	// Calculate I term
	plant_pid->i_state += plant_error;
	// Apply anti-windup
	if (plant_pid->i_state > plant_pid->i_max)
	{
		plant_pid->i_state = plant_pid->i_max;
	}
	else if (plant_pid->i_state < plant_pid->i_min)
	{
		plant_pid->i_state = plant_pid->i_min;
	}

	i_term = plant_pid->i_gain * plant_pid->i_state;

	// Calculate D term
	d_term = plant_pid->d_gain * (plant_pid->d_state - plant_current_value);
	plant_pid->d_state += d_term;

	return p_term + i_term + d_term;
}

void mpid_reset(mpid_c_t * plant_pid)
{
	plant_pid->i_state = 0;
	plant_pid->d_state = 0;

}
