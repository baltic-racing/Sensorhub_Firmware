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

#define LEFT 0
#define RIGHT 1

extern volatile unsigned long sys_time;

void PORT_Config();
void calc_speed(void);


extern volatile uint8_t wheelspeed_left;
extern volatile uint8_t wheelspeed_right;

#endif /* CALC_WHEELSPEED_H_ */