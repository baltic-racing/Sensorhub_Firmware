/*
 * misc_functions.h
 *
 * Created: 08.10.2024 13:57:05
 *  Author: Egquus
 */ 

#ifdef MISC_FUNCTIONS_H_
#define MISC_FUNCTIONS_H_
#include <avr/io.h>
#include <stdio.h>
#include <float.h>
#include <math.h>


uint8_t temp_counter = 1;
uint8_t temp;

uint16_t tnp_temp;
void sys_tick();
void temp_switch(uint8_t temp);

uint8_t damper_poti(travel);

uint16_t temp_rdy;

#endif /* MISC_FUNCTIONS_H_ */