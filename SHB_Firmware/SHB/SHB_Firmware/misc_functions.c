/*
 * misc_functions.c
 *
 * Created: 08.10.2024 13:56:43
 *  Author: Egquus
 */
#include <avr/io.h>
#include "misc_functions.h"

volatile uint8_t temp_counter;
uint16_t temp_rdy[5]; // Annahme: Größe des Arrays ist 5
float tnp_temp;
extern volatile 

void temp_switch(uint8_t temp)
{
	tnp_temp = ((0.128479 * adc_get(temp_counter)) - 8.137044) * 10; // NTC Kurve ist linearisiert im Bereich 80°-20°, außerhalb ungenauer


	if(temp_counter < 5)
	{
		temp_rdy[temp_counter] = (uint16_t)tnp_temp; // Konvertierung zu uint16_t
		temp_counter++;
	}
	else
	{
		temp_counter = 0;
	}
}
damper_poti(uint16_t data){
	uint16_t travel =0;
	travel=(75*data)/1024;
	return travel; //in mm from 0-75
}

//alte Funktion von Lukas
//uint16_t ADC2Sensor(uint16_t data, float start_Volt, float end_Volt, uint8_t sensor_max, uint16_t ADC_bit, uint8_t ADC_Volt, uint16_t precision){
	//uint16_t start_ADC = (pow(2,ADC_bit))/ADC_Volt*start_Volt;
	//uint16_t end_ADC = (pow(2,ADC_bit))/ADC_Volt*end_Volt ;
	//float ADC_range = end_ADC - start_ADC;
	//float sensor_factor = sensor_max/ADC_range;
	//uint16_t Sensor_Data = (data-start_ADC)*precision*sensor_factor;
	//if (data<start_ADC){//Set sensor value to zero if below the ADC Threshold
		//Sensor_Data = 0;
	//}
	//return Sensor_Data;
//}