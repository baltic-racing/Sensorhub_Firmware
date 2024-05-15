/*
 * sensor_function.c
 *
 * Created: 13.05.2024
 *  Author: Egquus
 */ 

#include "sensor_function.h"
#include <avr/io.h>
uint16_t pressure = 0;

void ADC_brake_Sensor_front(uint16_t data){

	pressure = (data/10.24);
	
	
}