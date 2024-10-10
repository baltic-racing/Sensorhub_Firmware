/*
 * adc_functions.h
 *
 * Created: 09.05.2024 16:07:47
 *  Author: aaron
 */ 

#ifndef ADC_FUNCTIONS_H_
#define ADC_FUNCTIONS_H_

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>

/*	DEFINES, adjust for the array size */
#define ADCVALUES 6 //here we've got all 6 ADCs in use (see uC Pinout)

uint16_t adc_get(uint8_t adc);

/*	FUNCTION PROTOTYPES	*/

void adc_config();
void adc_start_conversion();

/*	Interrupt Service Routines	*/

ISR(ADC_vect);

#endif /* ADC_FUNCTIONS_H_ */