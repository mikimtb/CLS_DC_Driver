/*
 * fsm.h
 *
 *  Created on: Jul 17, 2019
 *      Author: Miroslav
 */

#ifndef HMI_FSM_H_
#define HMI_FSM_H_

#include "definitions.h"
#include "tm1637.h"
#include "user_button.h"
#include "user_timer.h"
#include "user_beeper.h"

enum buttons_e
{
	BTN_START_STOP = 0,
	BTN_UP,
	BTN_DOWN,
	BTN_SET,
	BTN_NUM
};

enum timers_e
{
	FSM_TMR = 0,
	RTC_TMR,
	TMR_NUM
};

void hmi_fsm_init(void);
void hmi_fsm_check_for_events(void);


#endif /* HMI_FSM_H_ */
