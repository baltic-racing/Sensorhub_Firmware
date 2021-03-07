/*
 * misc_functions.h
 *
 * Created: 05.04.2019 18:51:20
 *  Author: Ole Hannemann
 */ 


#ifndef MISC_FUNCTIONS_H_
#define MISC_FUNCTIONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

//creating Prototypes of the functions used in the Misc_functions.c
void port_config();
void sys_timer_config();
ISR(TIMER0_COMP_vect);
void sys_tick();

#endif /* MISC_FUNCTIONS_H_ */