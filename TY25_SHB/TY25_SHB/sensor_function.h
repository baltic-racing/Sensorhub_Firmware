/*
 * sensor_function.h
 *
 * Created: 31.03.2025 14:45:25
 *  Author: pasca
 */ 


#ifndef SENSOR_FUNCTION_H_
#define SENSOR_FUNCTION_H_

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>

//damper travel
#define DAMP_ADC_MIN 687.0   //adc value
#define DAMP_MM_MIN 16.95 // associated value in mm

//#define DAMP_ADC_MIN 744.0   //adc value
//#define DAMP_MM_MIN 13.0 // associated value in mm

#define DAMP_ADC_MAX 18.0 // adc value
#define DAMP_MM_MAX 74.86 //associated value in mm

//#define DAMP_ADC_MAX 793.0 // adc value
//#define DAMP_MM_MAX 15.0 //associated value in mm

#define DAMP_MAX_FL 59.6 //poti value in mm for full spring travel front left
#define DAMP_MAX_FR 68.0 //poti value in mm for full spring travel front right
#define SPRINGTRAVEL_MAX 185.0 //full spring travel in mm

typedef enum {
	TK1,
	TK2,
	TK3,
	TK4
} TK_Channel;


//uint16_t ADC_brake_Sensor_front(uint16_t data, float start_Volt, float end_Volt, int sensor_max, int ADC_bit, int ADC_Volt, int precision);
//uint16_t ADC_brake_Sensor(uint16_t data);
double damper_poti(double dp_adc);
uint16_t temp_calc(double ntc_adc);
float read_max6675(void);
float read_TK_temperature(TK_Channel channel);

#endif