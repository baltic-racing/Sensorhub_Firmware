/*
 * adc_functions.c
 *
 * Created: 28.04.2023 18:30:34
 *  Author: nico
 */ 

#include "adc_functions.h"
#include <math.h>

// array to store the adc data in
uint16_t adc_values[ADCVALUES]; //Defined in the Header file (2 right now)
// index var to know which adc will come next
uint8_t adc_next = 0;

void adc_config()
{
	//AREF = AVCC
	ADMUX = (1<<REFS0) | (1<<MUX0);
	//ADC Enable, /16, Interrupt Enable
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0) | (1<<ADIE);
	//Start Conversion
	ADCSRA |= (1<<ADSC);
	DIDR0 |= 1;
	
	
}

void adc_start_conversion()
{
	//Start next conversion with same config
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0) | (1<<ADIE) | (1<<ADSC);
	
	SMCR = (1<<SM0)|(0<<SM1)|(0<<SM2); //enable ADC noise reduction mode and starts the conversion
}

ISR(ADC_vect)
{
	adc_values[adc_next] = ADC;// ADCL | (ADCH << 8); //low bit erst ablesen
	adc_next++;
	if(adc_next == ADCVALUES)
	{
		adc_next = 0;
	}
	// select other ADC Input
	ADMUX = (1<<REFS0) | adc_next;
}

// getter for each adc var
/*uint16_t adc_get(uint8_t adc)
{
	return adc_values[adc];
}*/
uint16_t adc_get_1(){
	return adc_values[0];
}
uint16_t adc_get_2(){
	return adc_values[1];
}
