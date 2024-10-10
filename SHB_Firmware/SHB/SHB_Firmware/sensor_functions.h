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


/*	Function Prototypes	*/
void ntc_value(double ntc_adc);

double damper_poti(double dp_adc);

#endif /* SENSOR_FUNCTIONS_H_ */