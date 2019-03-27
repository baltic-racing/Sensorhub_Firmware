/*
 * misc_functions.c
 *
 * Created: 27.03.2019 12:13:16
 *  Author: cerbe
 */ 
#include "misc_functions.h"
#include <avr/io.h>


void port_config(){
	
	
}
void timer_config(){
	
	//8 bit Timer 0 config
	//ctc mode and 64 as prescaler
	TCCR0A = 0 | (1<<WGM01) /*| (1<<COM0A1) */| (1<<CS01) | (1<<CS00);
	TIMSK0 = 0 | (1<<OCF0A); //compare interrupt enable
	OCR0A = 250-1; // compare value for 1ms;

}
void adc_config(){
	
		// AREF = AVcc
		ADMUX = (1<<REFS0);
		// ADEN enabes ADC
		// ADIE eneables interrupts
		// ADC prescaler 128
		// 16000000/128 = 125000
		ADCSRA = (1<<ADEN) | (1<<ADPS2)/* | (1<<ADPS1) | (1<<ADPS0)*/ | (1<<ADIE);
		//start first conversation
		ADCSRA |= (1<<ADSC);

}
void spi_config(){
	
	DDRB = 0;
	//configure needed Ports as output
	DDRB |= (1<<PB2) | (1<<PB1) | (1<<PB0);
	//configure SPI
	SPCR |= (1<<SPIE) | (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1) ;
	
}

void systick(){

	DDRC ^= (1<<PC2);

}