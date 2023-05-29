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
void sys_tick_heart(void);
/*	Interrupt Service Routines	*/

ISR(TIMER0_COMP_vect);

#endif /* SYSTEM_CONFIG_H_ */