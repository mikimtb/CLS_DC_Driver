/*
 * motion_controller.c
 *
 *  Created on: Feb 1, 2019
 *      Author: Miroslav
 */

#include "motion_controller.h"

// Private functions declaration
static void _position_cnt(void);
static bool _calculate_angular_velocity(int64_t current_position);
static void _calculate_velocity_coefficients(uint8_t num_of_cycles);
static bool _acceleration_ramp(float *ramp_calc_velocity);
static bool _deacceleration_ramp(float *ramp_calc_velocity);
static void _mc_rcc_config(void);
static void _mc_tim_config(void);
static void _mc_nvic_config(void);

// Private variables
static motion_ctrl_t m_ctrl;
/*
 * The array that keeps constant that should be used in the
 * calculator depending on the number of cycles that has past
 * Vcoeff = 60 / (deltaT x GEAR_RATIO x ENC_PULSE_PER_REVOLUTION)
 */
static float angular_velocity_coeff[5] = { 0,00 };
/*
 * Velocity control PID controller
 */
static mpid_c_t v_pid;
/*
 * Acceleration and Deacceleration Ramp variables
 */
ramp_status_e acc_ramp_status = RAMP_ACTIVE;
ramp_status_e deacc_ramp_status = RAMP_DONE;
float temp_velocity_setpoint = 0;

// IT handlers routine
void MC_IRQHandler()
{
	int16_t calculated_pwm_output;
	float plant_error;

	if (TIM_GetITStatus(MC_TIM, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(MC_TIM, TIM_IT_Update);

		/* Motion Controller loop is starting here */

		// Update current position
		m_ctrl.angular_position = m_ctrl.motor.encoder_index_counter + as5040_get_angular_position();
		// Calculate and update velocity
		_calculate_angular_velocity(m_ctrl.angular_position);
		// Calculate and update motion parameters
		if (m_ctrl.status == STARTED)
		{
			if (_acceleration_ramp(&temp_velocity_setpoint))
			{
				plant_error = temp_velocity_setpoint - m_ctrl.angular_velocity;
			}
			else
			{
				plant_error = (float)(m_ctrl.velocity_setpoint) - m_ctrl.angular_velocity;
			}
			calculated_pwm_output = (int16_t)(mpid_update(&v_pid, plant_error, m_ctrl.angular_velocity));
			motion_controller_set_pwm_duty(m_ctrl.current_pwm_duty + calculated_pwm_output);
		}
		else if (m_ctrl.status == STOPPED)
		{
			if (_deacceleration_ramp(&temp_velocity_setpoint))
			{
				plant_error = temp_velocity_setpoint - m_ctrl.angular_velocity;
				calculated_pwm_output = (int16_t)(mpid_update(&v_pid, plant_error, m_ctrl.angular_velocity));
				motion_controller_set_pwm_duty(m_ctrl.current_pwm_duty + calculated_pwm_output);
			}
			else
			{
				motion_controller_set_pwm_duty(0);
				mpid_reset(&v_pid);
			}
		}
		// Update

		//printf(" %f \r\n", m_ctrl.angular_velocity);
		/* Motion Controller loop is ending here */
	}
}

// Public functions definition
void motion_controller_init_motor_params(uint8_t motor_voltage, uint16_t enc_max_cnt, gearbox_e gb_status, uint16_t gb_ratio, uint16_t max_velocity)
{
	// Initialize regenerative dynamic brake and deactivate it
	brake_init();
	m_ctrl.brake_status = DEACTIVATED;

	// Initialize PWM generator
	pwm_init(PWM_15KHZ);
	m_ctrl.current_pwm_duty = 0;
	pwm_set_pulse_width(ZERO_DUTY, ZERO_DUTY);
	pwm_driver_enable(true);

	// Initialize DC motor settings
	m_ctrl.motor.encoder_max_count = enc_max_cnt;
	m_ctrl.motor.encoder_index_counter = 0;
	m_ctrl.motor.gearbox_ratio = gb_ratio;
	m_ctrl.motor.is_have_gearbox = gb_status;
	m_ctrl.motor.max_voltage = motor_voltage;
	m_ctrl.motor.max_velocity = max_velocity;

	// Initialize incremental encoder
	if(m_ctrl.motor.is_have_gearbox == GEARBOX)
	{
		// If motor is coming with gearbox attached, index event is generated per 1 output shaft revolution
		m_ctrl.enc_index = (m_ctrl.motor.gearbox_ratio * m_ctrl.motor.encoder_max_count) - 1;
	}
	else
	{
		// If gearbox don't exist the index event is generated per 1 motor shaft revolution
		m_ctrl.enc_index = m_ctrl.motor.encoder_max_count - 1;
	}
	as5040_init(m_ctrl.enc_index, _position_cnt);

	// Initialize motion controller
	m_ctrl.angular_position = 0;
	m_ctrl.angular_velocity = 0;
	m_ctrl.direction = as5040_get_direction();
	m_ctrl.status = STOPPED;

	// Initialize coefficients
	_calculate_velocity_coefficients(MAX_NUMBER_OF_CYCLES);

	// Initialize Motion Controller control loop timer
	_mc_rcc_config();
	_mc_tim_config();
	_mc_nvic_config();

	TIM_Cmd(MC_TIM, ENABLE);

	#ifdef USE_UART_CONSOLE
	printf("Motion Controller is initialized...\r\n");
	#endif
}

void motion_controller_init_PID_params(float p, float i, float d)
{
	// Initialize PID controller
	v_pid.p_gain = p;
	v_pid.i_gain = i;
	v_pid.d_gain = d;

	mpid_init( &v_pid, PWM_MAX_OUT, -PWM_MAX_OUT );

	#ifdef USE_UART_CONSOLE
	printf("PID Controller is initialized...\r\n");
	#endif
}

void motion_controller_init_default_setpoints(int64_t dps, int16_t dvs)
{
	m_ctrl.position_setpoint = dps;
	motion_controller_set_angular_velocity_setpoint(dvs);
	//m_ctrl.velocity_setpoint = dvs;

#ifdef USE_UART_CONSOLE
printf("Default set-points initialized...\r\n");
#endif
}

void motion_controller_start(void)
{
	m_ctrl.status = STARTED;
	// Enable MC control loop
	//TIM_Cmd(MC_TIM, ENABLE);
}
void motion_controller_stop(void)
{
	m_ctrl.status = STOPPED;
	// Disable MC control loop
	//TIM_Cmd(MC_TIM, DISABLE);
}
void motion_controller_reset(void)
{
	m_ctrl.status = STOPPED;
	// Disable MC control loop
	TIM_Cmd(MC_TIM, DISABLE);
	// Reset MC loop timer
	NVIC_DisableIRQ(MC_IRQn);
	TIM_SetCounter(MC_TIM, POS_CNT_RST_VALUE);
	TIM_ClearITPendingBit(MC_TIM, TIM_IT_Update);
	NVIC_EnableIRQ(MC_IRQn);
}
int64_t motion_controller_get_current_angular_position(void)
{
	return m_ctrl.angular_position;
}
int16_t motion_controller_get_current_angular_velocity(void)
{
	return (int16_t) m_ctrl.angular_velocity;
}
int16_t motion_controller_get_current_pwm_duty(void)
{
	return m_ctrl.current_pwm_duty;
}
ctrl_status_e motion_controller_get_status()
{
	return m_ctrl.status;
}
void motion_controller_set_angular_postion_setpoint(int16_t ap_setpoint)
{
	m_ctrl.position_setpoint = ap_setpoint;
}
void motion_controller_set_angular_velocity_setpoint(int16_t av_setpoint)
{
	float acc_deacc_step_count;

	int16_t av_setpoint_temp = av_setpoint;

	if (av_setpoint_temp < 0)
	{
		av_setpoint_temp *= -1;
	}
	if (av_setpoint_temp <= m_ctrl.motor.max_velocity)
	{
		m_ctrl.velocity_setpoint = av_setpoint;

		// Calculate acceleration ramp parameters
		acc_deacc_step_count = (float)(app_params_get_acceleration_time()) / TIME_STAMP;
		m_ctrl.acceleration_velocity_stamp = (float)m_ctrl.velocity_setpoint / acc_deacc_step_count;

		// Calculate deacceleration ramp parameters
		acc_deacc_step_count = (float)(app_params_get_deacceleration_time()) / TIME_STAMP;
		m_ctrl.deacceleration_velocity_stamp = (float)m_ctrl.velocity_setpoint / acc_deacc_step_count;
	}
}
int16_t motion_controller_get_angular_velocity_setpoint()
{
	return m_ctrl.velocity_setpoint;
}
void motion_controller_set_pwm_duty(int16_t pwm_duty)
{
	m_ctrl.current_pwm_duty = pwm_duty;
	pwm_set_pulse_width(ZERO_DUTY - pwm_duty, ZERO_DUTY + pwm_duty);
}

// Private functions definition
static void _position_cnt()
{
	m_ctrl.direction = as5040_get_direction();
	if (m_ctrl.direction == CW)
	{
		m_ctrl.motor.encoder_index_counter += m_ctrl.enc_index;
	}
	else
	{
		m_ctrl.motor.encoder_index_counter -= m_ctrl.enc_index;
	}
}

static bool _calculate_angular_velocity(int64_t current_position)
{
	bool result = false;
	static int64_t prev_position = 0;					/*!< The variable keeps previous position, it is needed for delta velocity calculation */
	static int8_t estimation_cycle = 0;				/*!< The variable keeps number of cycles that has past until threshold movement is reached */
	int64_t delta_position;

	delta_position = current_position - prev_position;
	if((delta_position >= MOVEMENT_THRESHOLD) | (estimation_cycle >= MAX_NUMBER_OF_CYCLES))
	{
		// Calculate new angular velocity
		m_ctrl.angular_velocity = delta_position * angular_velocity_coeff[estimation_cycle];
		// Update static variables
		prev_position = current_position;
		estimation_cycle = 0;

		result = true;
	}
	else
	{
		estimation_cycle++;
	}


	return result;
}

static void _calculate_velocity_coefficients(uint8_t num_of_cycles)
{
	uint8_t i = 0;

	for(i = 1; i <= (num_of_cycles + 1); i++)
	{
		// Vcoeff = 60 / (deltaT x GEAR_RATIO x ENC_PULSE_PER_REVOLUTION)
		angular_velocity_coeff[i-1] = 60 / ((TIME_STAMP * i) * m_ctrl.motor.gearbox_ratio * m_ctrl.motor.encoder_max_count);
	}
}

static bool _acceleration_ramp(float *ramp_calc_velocity)
{
	bool result = true;
	bool positive_ramp_done_flag;
	bool negative_ramp_done_flag;

	if (acc_ramp_status == RAMP_ACTIVE)
	{
		*ramp_calc_velocity += m_ctrl.acceleration_velocity_stamp;

		negative_ramp_done_flag = (*ramp_calc_velocity < (float)(m_ctrl.velocity_setpoint)) && (m_ctrl.velocity_setpoint < 0);
		positive_ramp_done_flag = (*ramp_calc_velocity > (float)(m_ctrl.velocity_setpoint)) && (m_ctrl.velocity_setpoint > 0);

		if (negative_ramp_done_flag || positive_ramp_done_flag)
		{
			acc_ramp_status = RAMP_DONE;
			deacc_ramp_status = RAMP_ACTIVE;
			*ramp_calc_velocity = (float)(m_ctrl.velocity_setpoint);
			result = false;
		}
	}
	else
	{
		result = false;
	}

	return result;
}

static bool _deacceleration_ramp(float *ramp_calc_velocity)
{
	bool result = true;
	bool positive_ramp_done_flag;
	bool negative_ramp_done_flag;

	if (deacc_ramp_status == RAMP_ACTIVE)
	{
		*ramp_calc_velocity -= m_ctrl.deacceleration_velocity_stamp;

		negative_ramp_done_flag = (*ramp_calc_velocity > 0) && (m_ctrl.velocity_setpoint < 0);
		positive_ramp_done_flag = (*ramp_calc_velocity < 0) && (m_ctrl.velocity_setpoint > 0);

		if (negative_ramp_done_flag || positive_ramp_done_flag)
		{
			acc_ramp_status = RAMP_ACTIVE;
			deacc_ramp_status = RAMP_DONE;
			*ramp_calc_velocity = 0;
			result = false;
		}
	}
	else
	{
		result = false;
	}

	return result;

//	acc_ramp_status = RAMP_ACTIVE;
//	deacc_ramp_status = RAMP_DONE;
//	*ramp_calc_velocity = 0;
//
//	return false;
}

static void _mc_rcc_config(void)
{
	// Enable Motion Controller TIM clock
	RCC_APB1PeriphClockCmd(MC_TIM_CLK, ENABLE);
}
static void _mc_tim_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_DeInit(MC_TIM);

	/* TIM2 Configuration ---------------------------------------------------
	Generate overflow on each 10ms:
	TIM2CLK = SystemCoreClock, Prescaler = 0, TIM2 counter clock = SystemCoreClock
	SystemCoreClock is set to 72 MHz

	The objective is to generate overflow on each 10ms, 100Hz:
	 - Freq = SystemCoreClock / (TIM_Prescaler + 1) * (TIM_Period + 1)
	 - Freq = 72MHz / (44999 + 1) * (15 + 1) = 100Hz
	----------------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Prescaler = 44999;
	TIM_TimeBaseStructure.TIM_Period = 15;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(MC_TIM, &TIM_TimeBaseStructure);

	TIM_SetCounter(MC_TIM, POS_CNT_RST_VALUE);
	TIM_ClearITPendingBit(MC_TIM, TIM_IT_Update);

	TIM_Cmd(MC_TIM, DISABLE);
}

static void _mc_nvic_config()
{
	TIM_ITConfig(MC_TIM, TIM_IT_Update, ENABLE );
	TIM_ClearITPendingBit(MC_TIM, TIM_IT_Update);
	NVIC_EnableIRQ(MC_IRQn);
}
