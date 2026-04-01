/*
 * sensor_function.h
 *
 * Created: 28.03.2025 12:52:13
 *  Author: pasca
 */ 

#ifndef SENSOR_FUNCTION_H_
#define SENSOR_FUNCTION_H_

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>

//wheelspeed

uint8_t getwheelspeed(uint8_t side);


//brake pressure
uint16_t calculate_BP(uint16_t adc_BP_data);

#define BP_MIN 102			// offset from BP-sensor (0bar=0.5V=102ADC)
#define BP_MAX 921			// maximum range of BP-sensor (150bar=4.5V=921ADC)
#define BP_range 8180		// range from 0bar to 4.5bar in ADC *10 for resolution

//damper travel
double damper_poti(double dp_adc);

#define DAMP_ADC_MIN 369		//adc value 369
#define DAMP_MM_MIN 57.05		// associated value in mm 57.05

#define DAMP_ADC_MAX 844		//adc value
#define DAMP_MM_MAX 20.1		//associated value in mm

#define DAMP_MAX_FL 55.95		//poti value in mm for full spring travel front left
#define DAMP_MAX_FR 56.45		//poti value in mm for full spring travel front right
#define SPRINGTRAVEL_MAX 185	//full spring travel in mm


//steering angle percentage
double calculate_steering_percent(double pot_adc);

#define POT_MIN 150				//adc value for full steering lock right
#define POT_MAX 875				//adc value for full steering lock left
#define POT_MID 537				//adc value for middle position







#endif