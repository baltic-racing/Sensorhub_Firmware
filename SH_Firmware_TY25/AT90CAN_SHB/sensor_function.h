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
#define DAMP_ADC_MIN 68   //adc value
#define DAMP_MM_MIN 23 // associated value in mm

#define DAMP_ADC_MAX 793 // adc value
#define DAMP_MM_MAX 78.45 //associated value in mm

#define DAMP_MAX_FL 76 //poti value in mm for full spring travel front left
#define DAMP_MAX_FR 68 //poti value in mm for full spring travel front right
#define SPRINGTRAVEL_MAX 185 //full spring travel in mm

//steering angle percentage
#define POT_MIN	0	//adc value for full steering lock left
#define POT_MAX 1023	//adc value for full steering lock right
#define POT_MID 512	//adc value for middle position

typedef enum {
	TK1,
	TK2,
	TK3,
	TK4
} TK_Channel;

double calculate_steering_percent(double pot_adc);
float read_TK_temperature(TK_Channel channel);

#endif