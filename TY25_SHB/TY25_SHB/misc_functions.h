/*
 * misc_functions.h
 *
 * Created: 31.03.2025 14:44:49
 *  Author: pasca
 */ 

#ifndef MISC_FUNCTIONS_H_
#define MISC_FUNCTIONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void port_config(void);
void sys_timer_config(void);
void sys_tick_heart(void);

ISR(TIMER0_COMP_vect);

#endif /* MISC_FUNCTIONS_H_ */