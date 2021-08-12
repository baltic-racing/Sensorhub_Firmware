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
uint16_t ADC2NTCtemp(uint16_t data, uint16_t bfactor, uint16_t R_NTC, uint16_t ADC_Volt, uint16_t ADC_acc, uint16_t R_Teiler);
uint16_t ADC2Sensor(uint16_t data, float start_Volt, float end_Volt, uint8_t sensor_max, uint16_t ADC_bit, uint8_t ADC_Volt, uint16_t precision);
uint16_t ADC2Angle(int16_t ADC_Data, uint16_t angle_mid, int16_t angle_min, uint16_t angle_max, uint16_t ADC_mid, uint16_t ADC_min, uint16_t ADC_max);

//defines for NTC calc with Thermashop MSW-20328
#define T_norm 298.15 //K

#endif /* MISC_FUNCTIONS_H_ */