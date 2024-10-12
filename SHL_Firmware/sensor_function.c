/*
 * sensor_function.c
 *
 * Created: 13.05.2024
 *  Author: Egquus
 */ 

#include "sensor_function.h"
#include <avr/io.h>
#include <math.h>

uint16_t ADC2Sensor(uint16_t data, float start_Volt, float end_Volt, uint8_t sensor_max, uint16_t ADC_bit, uint8_t ADC_Volt, uint16_t precision){
	uint16_t start_ADC = (pow(2,ADC_bit))/ADC_Volt*start_Volt;
	uint16_t end_ADC = (pow(2,ADC_bit))/ADC_Volt*end_Volt ;
	float ADC_range = end_ADC - start_ADC;
	float sensor_factor = sensor_max/ADC_range;
	uint16_t Sensor_Data = (data-start_ADC)*precision*sensor_factor;
	if (data<start_ADC){//Set sensor value to zero if below the ADC Threshold
		Sensor_Data = 0;
	}
	return Sensor_Data;
}
