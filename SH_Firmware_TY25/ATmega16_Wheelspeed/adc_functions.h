/*
 * ads_functions.h
 *
 * Created: 16.10.2024 12:55:14
 *  Author: Egquus
 */ 

#ifndef ADC_FUNCTIONS_H_
#define ADC_FUNCTIONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

//function to config the adc interface
void adc_config();
//function thats restarting the next conversion
void adc_start_conversion();
//getter for adc vars
//reserverd for future use
uint16_t adc_get(uint8_t adc);

#define ADCVALUES 1

ISR(ADC_vect);

#endif /* ADC_FUNCTIONS_H_ */