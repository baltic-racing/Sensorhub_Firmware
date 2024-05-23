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
	ADMUX = (1<<REFS0) | (1<<MUX0); // AREF = AVcc and PF0 (ADC0) as input defined
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0) | (1<<ADIE); // ADEN enables ADC, ADPS0,1,2 for ADC prescaler 16, ADIE enables ADC interrupt // see datasheet page 290
	ADCSRA |= (1<<ADSC); //start first conversation
	DIDR0 |= 1; //Disabling the Digital Input Buffer on this Port because we only use the ADC Port for Programming and ADC
	
	
}

void adc_start_conversion()
{
	///start next conversion with same config
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0) | (1<<ADIE) | (1<<ADSC); //prescaler to 16
	//!!! Currently we have to Start the conversion manually with ADSC=1. Normally SMCR should auto trigger the conversion but as of now it does not work. neither do we know if the uC goes into Noise reduction mode
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

uint16_t adc_get_1(){
	return adc_values[0];
}
uint16_t adc_get_2(){
	return adc_values[1];
}
