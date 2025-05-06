/*
 * sensor_function.c
 *
 * Created: 31.03.2025 14:45:12
 *  Author: pasca
 */ 


#include "sensor_function.h"
#include "SPI_lib.h"
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>


float beta = 3450.0f;				// optimized between 0-100°C
float R_25C = 10000.0f;				// restistance at 25°C
float T_25C = 298.15f;
float VCC = 5.0f;
float ADC_max = 1023.0f;
float R_i = 3300.0f;


// NTC temperature
uint16_t temp_calc(double ntc_adc){
	float U_NTC = (VCC / ADC_max) * ntc_adc;
	float R_NTC = R_i * ((VCC / U_NTC) - 1);
	
	float temperature_kelvin = (beta * T_25C) / (beta + (T_25C * log(R_NTC / R_25C)));
	float temperature_celsius = (temperature_kelvin - 273.15f) * 10;
	
	return (uint16_t) temperature_celsius;
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


// damper travel
double damper_poti(double dp_adc){
	double travel = 0;
	
	//travel = (80*dp_adc)/1024;
	
	travel = DAMP_MM_MIN + (dp_adc-DAMP_ADC_MIN)*((DAMP_MM_MAX - DAMP_MM_MIN)/(DAMP_ADC_MAX - DAMP_ADC_MIN)); //interpolate with 2 values
	
	return travel;
}

