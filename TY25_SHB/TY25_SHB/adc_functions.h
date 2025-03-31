/*
 * adc_functions.h
 *
 * Created: 31.03.2025 14:40:33
 *  Author: pasca
 */ 


#ifndef ADC_FUNCTIONS_H_
#define ADC_FUNCTIONS_H_

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>

/*	DEFINES	*/
#define ADCVALUES 7

uint16_t adc_get(uint8_t adc);

/*	FUNCTION PROTOTYPES	*/

void adc_config();
void adc_start_conversion();

/*	Interrupt Service Routines	*/

ISR(ADC_vect);

#endif /* ADC_FUNCTIONS_H_ */