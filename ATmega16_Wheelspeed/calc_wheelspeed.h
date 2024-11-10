/*
 * calc_functions.h
 *
 * Created: 16.10.2024 13:00:27
 *  Author: Egquus
 */ 

#ifndef CALC_WHEELSPEED_H_
#define CALC_WHEELSPEED_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void PORT_Config();
ISR(INT1_vect);
uint16_t speed();

#endif /* CALC_WHEELSPEED_H_ */