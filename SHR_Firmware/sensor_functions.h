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

#define APPS1_MIN	883//433
#define APPS1_MAX	398//184
#define APPS2_MIN	191//826
#define APPS2_MAX	737//325

//steering angle percentage
#define POT_MIN	97	//value for full steering lock left
#define POT_MAX 840	//value for full steering lock right
#define POT_MID 480	//value for middle position

/*	Function Prototypes	*/

float update_apps_percentage(float apps_adc, uint8_t apps_sensor);
double update_apps_percentage_alt(double apps_adc, uint8_t apps_sensor);
double calculate_steering_percent(double pot_adc);

#endif /* SENSOR_FUNCTIONS_H_ */