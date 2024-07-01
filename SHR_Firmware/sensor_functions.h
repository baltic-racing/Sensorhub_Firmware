/*
 * sensor_functions.h
 *
 * Created: 24.06.2023 00:29:08
 *  Author: nico
 */ 


#ifndef SENSOR_FUNCTIONS_H_
#define SENSOR_FUNCTIONS_H_

/*	Inlcudes	*/

/*	Defines	*/

#define APPS1_MIN	429//433
#define APPS1_MAX	183//184
#define APPS2_MIN	823//826
#define APPS2_MAX	304//325

/*	Function Prototypes	*/

float update_apps_percentage(float apps_adc, uint8_t apps_sensor);


#endif /* SENSOR_FUNCTIONS_H_ */