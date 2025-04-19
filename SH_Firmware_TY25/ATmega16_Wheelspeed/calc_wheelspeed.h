/*
 * calc_functions.h
 *
 * Created: 16.10.2024 13:00:27
 *  Author: Egquus
 */ 

#ifndef CALC_WHEELSPEED_H_
#define CALC_WHEELSPEED_H_

#define LEFT 0
#define RIGHT 1
#define update_frequency_simple_ms 100
#define number_of_teeth 16

#include <avr/io.h>
#include <avr/interrupt.h>

void PORT_Config();
void calc_speed(void);
void calc_speed_low(uint8_t side, double timestep_diff_ms);
void calc_speed_high(uint8_t side, uint8_t steps_per_intervall);
ISR(INT1_vect);


uint16_t speed();

extern volatile uint8_t wheelspeed_left;
extern volatile uint8_t wheelspeed_right;
extern volatile uint16_t steps_per_intervall_left;
extern volatile uint16_t steps_per_intervall_right;
extern volatile uint16_t timestepdiff_left;
extern volatile uint16_t timestepdiff_right;


#endif /* CALC_WHEELSPEED_H_ */