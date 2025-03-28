/*
 * sensor_function.c
 *
 * Created: 28.03.2025 12:51:55
 *  Author: pasca
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

double damper_poti(double dp_adc){
	double travel = 0;
	
	//travel = (80*dp_adc)/1024;
	
	travel = DAMP_MM_MIN + (dp_adc-DAMP_ADC_MIN)*((DAMP_MM_MAX - DAMP_MM_MIN)/(DAMP_ADC_MAX - DAMP_ADC_MIN)); //interpolate with 2 values
	
	return travel;
}

double calculate_steering_percent(double pot_adc) {
	double steering_value = 0;
	
	if(pot_adc <= POT_MID){
		steering_value = 100 - ((100 / (double)(POT_MID - POT_MIN)) * (pot_adc - (double)POT_MIN));
	}

	if(pot_adc > POT_MID){
		steering_value = 100 - ((100 / (double)(POT_MID - POT_MAX)) * (pot_adc - (double)POT_MAX));
	}
	
	return steering_value;
}