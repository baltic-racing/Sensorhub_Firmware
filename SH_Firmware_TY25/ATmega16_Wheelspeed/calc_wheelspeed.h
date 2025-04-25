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
#define update_frequency_simple_ms 100														// Update frequency for the floating calculation of the Wheelspeed
#define number_of_teeth 16																	// Equals pulses per rotation
#define TRIGGER_ANGLE_DEG 22.5
#define TIRE_CIRCUMFERENCE_MM 1476.5485														// Tire circumference in mm
#define DISTANCE_PER_PULSE_MM (TIRE_CIRCUMFERENCE_MM * (TRIGGER_ANGLE_DEG / 360.0f))
#define SWITCH_THERESHOLD_STEPS 5															// From ~16km/h

extern volatile unsigned long sys_time;
extern volatile unsigned long time_delta_left;
extern volatile unsigned long time_delta_right;

#include <avr/io.h>
#include <avr/interrupt.h>

void PORT_Config();
void calc_speed(void);
void calc_speed_low(uint8_t side, double timestep_diff_ms);
void calc_speed_high(uint8_t side, uint8_t steps_per_intervall);
ISR(INT1_vect);


uint16_t speed();

extern volatile uint16_t wheelspeed_left;
extern volatile uint16_t wheelspeed_right;
extern volatile uint16_t steps_per_intervall_left;
extern volatile uint16_t steps_per_intervall_right;
extern volatile uint16_t timestepdiff_left;
extern volatile uint16_t timestepdiff_right;


#endif /* CALC_WHEELSPEED_H_ */