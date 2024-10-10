/*
 * system_config.h
 *
 * Created: 28.04.2023 17:44:19
 *  Author: nico
 */ 
#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>

/*	FUNCTION PROTOTYPES	*/

void port_config(void);
void sys_timer_config(void);

/*	Interrupt Service Routines	*/

ISR(TIMER0_COMP_vect);

/*	MACROS	*/

#define TIME_PASSED_1_MS	((sys_tick - time_old) >= 1)
#define TIME_PASSED_10_MS	(time_old_10ms >= 10)
#define TIME_PASSED_100_MS	(time_old_100ms >= 100)

#endif /* SYSTEM_CONFIG_H_ */