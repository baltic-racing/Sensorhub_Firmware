/*
 * sensor_functions.c
 *
 * Created: 24.06.2023 00:28:43
 *  Author: nico
 */ 

/*	Includes	*/

#include <avr/io.h>
#include "sensor_functions.h"
#include "adc_functions.h"

/* Variables */
float apps1_percentage;
float apps2_percentage;

extern uint16_t adc_values[1];

/*	Functions	*/

double update_apps_percentage_alt(double apps_adc, uint8_t apps_sensor){
	double apps_value = 0;
	if (apps_adc < 75){
		return apps_value = 0;
	}
	switch (apps_sensor){
		case 1: {
			apps_value = (100/(double)(APPS1_MAX - APPS1_MIN)) * (apps_adc - (double)APPS1_MIN);
			break;
		}
		case 2: {
			apps_value = (100/(double)(APPS2_MAX - APPS2_MIN)) * (apps_adc - (double)APPS2_MIN);
			break;
		}
	}
	if(apps_value >99){
		apps_value = 99;
	}
	return apps_value;
}

