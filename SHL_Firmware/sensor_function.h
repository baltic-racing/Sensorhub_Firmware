/*
 * sensor_functions.h
 *
 * Created: 13.05.2024
 *  Author: Egquus
 */ 


#ifndef SENSOR_FUNCTION_H_
#define SENSOR_FUNCTION_H_

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>

//uint16_t ADC_brake_Sensor_front(uint16_t data, float start_Volt, float end_Volt, int sensor_max, int ADC_bit, int ADC_Volt, int precision);
//uint16_t ADC_brake_Sensor(uint16_t data);
uint16_t ADC2Sensor(uint16_t data, float start_Volt, float end_Volt, uint8_t sensor_max, uint16_t ADC_bit, uint8_t ADC_Volt, uint16_t precision);

#endif