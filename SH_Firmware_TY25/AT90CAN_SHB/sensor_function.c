/*
 * sensor_function.c
 *
 * Created: 28.03.2025 12:51:55
 *  Author: pasca
 */ 

#include "sensor_function.h"
#include "SPI_lib.h"
#include <avr/io.h>
#include <math.h>

double APPS_calculation(int apps_adc){
	uint16_t apps_percent = PERC_MIN + (apps_adc - APPS1_ADC_MIN) * ((PERC_MAX - PERC_MIN)/(APPS1_ADC_MAX - APPS1_ADC_MIN));
	if (apps_percent < 0.0){
		apps_percent = 0.0;
	}
	if (apps_percent > 100.0){
		apps_percent = 100.0;
	}
	return apps_percent;
}

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

// Typ K temperature
float read_TK_temperature(TK_Channel channel) {
	uint8_t high_byte = 0;
	uint8_t low_byte = 0;
	uint16_t raw_value = 0;
	float temperature = 0.0;

	switch(channel) {
		case TK1: SS_TK1_LOW(); break;
		case TK2: SS_TK2_LOW(); break;
		case TK3: SS_TK3_LOW(); break;
		case TK4: SS_TK4_LOW(); break;
	}

	high_byte = SPI_transfer(0x00);
	low_byte  = SPI_transfer(0x00);

	switch(channel) {
		case TK1: SS_TK1_HIGH(); break;
		case TK2: SS_TK2_HIGH(); break;
		case TK3: SS_TK3_HIGH(); break;
		case TK4: SS_TK4_HIGH(); break;
	}

	raw_value = (high_byte << 8) | low_byte;
	temperature = ((raw_value >> 3) & 0x7FF) * 0.25;

	return temperature * 100;
}
