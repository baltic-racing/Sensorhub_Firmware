/*
 * temp_functions.c
 *
 * Created: 08.10.2024 13:56:43
 *  Author: Egquus
 */ 
#include <avr/io.h>
#include "temp_functions.h"

volatile uint8_t temp_counter;
uint16_t temp_rdy[5]; // Annahme: Größe des Arrays ist 5
float tnp_temp;

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
