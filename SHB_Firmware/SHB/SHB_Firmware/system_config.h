/*
 * system_config.h
 *
 * Created: 09.05.2024 16:09:30
 *  Author: aaron
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

#endif /* SYSTEM_CONFIG_H_ */