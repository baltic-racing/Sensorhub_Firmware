/*
 * sensor_functions.h
 *
 * Created: 24.06.2023 00:29:08
 *  Author: nico
 */ 


#ifndef SENSOR_FUNCTIONS_H_
#define SENSOR_FUNCTIONS_H_

/*	Inlcudes */

/*	Defines	*/

#define APPS1_MIN	899//433
#define APPS1_MAX	406//184
#define APPS2_MIN	172//826
#define APPS2_MAX	727//325

/*	Function Prototypes	*/

float update_apps_percentage(float apps_adc, uint8_t apps_sensor);
double update_apps_percentage_alt(double apps_adc, uint8_t apps_sensor);

#endif /* SENSOR_FUNCTIONS_H_ */