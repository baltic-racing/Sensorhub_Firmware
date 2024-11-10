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

double calculate_steering_percent(double pot_adc) {
	double steering_value = 0;
	
	if(pot_adc <= POT_MID){
		steering_value = 100 - ((100 / (double)(POT_MID - POT_MIN)) * (pot_adc - (double)POT_MIN));
	}

	if(pot_adc > POT_MID){
		steering_value = 100 - ((100 / (double)(POT_MID - POT_MAX)) * (pot_adc - (double)POT_MAX));
	}	
		
	return steering_value;
}

double damper_poti(double dp_adc){
	double travel = 0;
	
	travel = (80*dp_adc)/1024;
	
	return travel; //in mm from 0-80
}