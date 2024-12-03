/*
 * sensor_functions.h
 *
 * Created: 24.06.2023 00:29:08
 *  Author: nico
 */ 
#ifndef SENSOR_FUNCTIONS_H_
#define SENSOR_FUNCTIONS_H_

/*	Inlcudes */
#include <avr/io.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
/*	Defines	*/

#define DAMP_ADC_MIN 68
#define DAMP_MM_MIN 23

#define DAMP_ADC_MAX 793
#define DAMP_MM_MAX 78.45

#define DAMP_MAX_RL 69
#define DAMP_MAX_RR 69
#define SPRINGTRAVEL_MAX 185

/*	Function Prototypes	*/
void ntc_value(double ntc_adc);

double damper_poti(double dp_adc);

#endif /* SENSOR_FUNCTIONS_H_ */