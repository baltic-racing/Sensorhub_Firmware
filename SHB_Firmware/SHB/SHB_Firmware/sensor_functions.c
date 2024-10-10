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




/*	Functions	*/
void temp_calc(double ntc_adc)
{
	double temp =0; 
	
	temp = ((0.128479 * ntc_adc) - 8.137044) * 10; // NTC Kurve ist linearisiert im Bereich 80∞-20∞, auﬂerhalb ungenauer
	
	return temp;
}
double damper_poti(double dp_adc){
	double travel =0;
	
	travel = (75*dp_adc)/1024;
	
	return travel; //in mm from 0-75
}



