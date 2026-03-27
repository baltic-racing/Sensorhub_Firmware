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

//uint16_t ADC_brake_Sensor_front(uint16_t data, float start_Volt, float end_Volt, int sensor_max, int ADC_bit, int ADC_Volt, int precision);
//uint16_t ADC_brake_Sensor(uint16_t data);
uint16_t ADC2Sensor(uint16_t data, float start_Volt, float end_Volt, uint8_t sensor_max, uint16_t ADC_bit, uint8_t ADC_Volt, uint16_t precision);
double damper_poti(double dp_adc);

//damper travel
#define DAMP_ADC_MIN 369   //adc value 369
#define DAMP_MM_MIN 57.05 // associated value in mm 57.05

#define DAMP_ADC_MAX 844 // adc value
#define DAMP_MM_MAX 20.1 //associated value in mm

#define DAMP_MAX_FL 55.95 //poti value in mm for full spring travel front left
#define DAMP_MAX_FR 56.45 //poti value in mm for full spring travel front right
#define SPRINGTRAVEL_MAX 185 //full spring travel in mm

//steering angle percentage
//#define POT_MIN	171	//adc value for full steering lock right
//#define POT_MAX 797	//adc value for full steering lock left
//#define POT_MID 478	//adc value for middle position

//#define POT_MIN 17	//adc value for full steering lock right
//#define POT_MAX 702	//adc value for full steering lock left
//#define POT_MID 355	//adc value for middle position

//#define POT_MIN 17	//adc value for full steering lock right
//#define POT_MAX 702	//adc value for full steering lock left
//#define POT_MID 355	//adc value for middle position

#define POT_MIN 196	//adc value for full steering lock right
#define POT_MAX 883	//adc value for full steering lock left
#define POT_MID 568	//adc value for middle position

double calculate_steering_percent(double pot_adc);

#endif