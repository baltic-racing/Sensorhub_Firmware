/*
 * CFile1.c
 *
 * Created: 09.05.2024 16:06:47
 *  Author: aaron
 */ 
#include "adc_functions.h"

// array to store the adc data in
uint16_t adc_values[ADCVALUES]; //Defined in the Header file (2 right now)
// index var to know which adc will come next
uint8_t adc_next = 0;



void adc_config()
{
	//AREF = AVCC
	ADMUX = (1<<REFS0);
	//ADC Enable, /16, Interrupt Enable
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0) | (1<<ADIE);
	//Start Conversion
	ADCSRA |= (1<<ADSC);
}

void adc_start_conversion()
{
	//Start next conversion with same config
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS0) | (1<<ADPS1);
	//noise reduction mode 
	SMCR = (1<<SM0) | (0<<SM1) | (1<<SM2);
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

	adc_start_conversion();
	
}

// getter for each adc var
uint16_t adc_get(uint8_t adc)
{
	return adc_values[adc];
}
