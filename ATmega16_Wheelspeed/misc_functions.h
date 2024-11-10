/*
 * misc_functions.h
 *
 * Created: 10.10.2024 16:24:38
 *  Author: Marek
 */ 
#ifndef MISC_FUNCTIONS_H_
#define MISC_FUNCTIONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void sys_timer_config();

ISR(TIMER0_COMP_vect);

#endif /* MISC_FUNCTIONS_H_ */