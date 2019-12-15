/*
 * user_pid.h
 *
 *  Created on: Mar 24, 2019
 *      Author: Miroslav
 */

#ifndef USER_PID_H_
#define USER_PID_H_

#include "user_pwm.h"

//#define PWM_MAX_OUT

typedef struct _mpid
{
	float p_gain;
	float i_gain;
	float i_max;
	float i_min;
	float i_state;
	float d_gain;
	float d_state;
}mpid_c_t;

void mpid_init(mpid_c_t * plant_pid, int16_t max_out, int16_t min_out);
float mpid_update(mpid_c_t * plant_pid, float plant_error, int16_t plant_current_value);
void mpid_reset(mpid_c_t * plant_pid);

#endif /* USER_PID_H_ */
