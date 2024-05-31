/*
 * sensor_functions.c
 *
 * Created: 24.06.2023 00:28:43
 *  Author: nico
 */ 

/*	Includes	*/

#include <avr/io.h>
#include "sensor_functions.h"

/*	Functions	*/

float update_apps_percentage(float apps_adc, uint8_t apps_sensor)
{
	float apps_value = 0;
	switch (apps_sensor)
	{
		case 1: {apps_value = (100/(float)(APPS1_MAX - APPS1_MIN)) * (apps_adc - (float)APPS1_MIN); break;}
		case 2: {apps_value = (100/(float)(APPS2_MAX - APPS2_MIN)) * (apps_adc - (float)APPS2_MIN); break;}
	}
		
	if(apps_value < 4)
	{
		apps_value = 0;
	}
		
	if(apps_value >99)
	{
		apps_value = 99;
	}
	
	return apps_value;
}