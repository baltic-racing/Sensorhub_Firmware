/*
 * sensor_function.c
 *
 * Created: 31.03.2025 14:45:12
 *  Author: pasca
 */ 


#include "sensor_function.h"
#include <avr/io.h>
#include <math.h>

uint16_t temp_calc(double ntc_adc){
	double temp = 0;
	
	temp = ((0.128479 * ntc_adc) - 8.137044) * 10; // NTC Kurve ist linearisiert im Bereich 80∞-20∞, auﬂerhalb ungenauer
	
	return temp;
}

double damper_poti(double dp_adc){
	double travel = 0;
	
	//travel = (80*dp_adc)/1024;
	
	travel = DAMP_MM_MIN + (dp_adc-DAMP_ADC_MIN)*((DAMP_MM_MAX - DAMP_MM_MIN)/(DAMP_ADC_MAX - DAMP_ADC_MIN)); //interpolate with 2 values
	
	return travel;
}

