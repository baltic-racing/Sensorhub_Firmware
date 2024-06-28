/*
 * adc_functions.h
 *
 * Created: 28.04.2023 18:30:49
 *  Author: nico
 */ 


#ifndef ADC_FUNCTIONS_H_
#define ADC_FUNCTIONS_H_

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>

/*	DEFINES	*/
#define ADCVALUES 3

//uint16_t adc_get(uint8_t adc);
uint16_t adc_get_1();
uint16_t adc_get_2();
uint16_t adc_get_3();

/*	FUNCTION PROTOTYPES	*/

void adc_config();
void adc_start_conversion();

/*	Interrupt Service Routines	*/

ISR(ADC_vect);

#endif /* ADC_FUNCTIONS_H_ */