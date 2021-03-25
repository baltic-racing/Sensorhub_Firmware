/*
 * misc_functions.c
 *
 * Created: 05.04.2019 18:51:39
 *  Author: Ole Hannemann
 * last change: 28.02.2021
 *  Author: Lukas Deeken
 */ 
#include "misc_functions.h"
volatile unsigned long sys_time = 0;

void port_config(){
	DDRA = 0;												//All not used
	DDRB = 0;												//Only SPI so 0 just like CAN
	DDRC = 0 | (1<<PC0) | (1<<PC2);							//PC0 and PC2 as Output for Heart and Foult LED otherwise unused
	DDRD = 0;												//Either CAN ID or Can so all Input
	DDRE = 0 | (1<<PE0) | (1<<PE1) | (1<<PE2) | (1<<PE3);	//PE0 to PE3 as Chip Select Outputs for SPI
	DDRF = 0;												//mark analog inputs on Port F as input														
}

void sys_tick(){

	PORTC ^= 1<<PC2; //toggling the Status of PC2 to make the Heart LED blink every SYStick

}

void sys_timer_config(){
	
	//8 bit Timer 0 configuration
	//ctc mode and 64 as prescaler for 16Mhz
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);
	TIMSK0 = 0 | (1<<OCF0A);	//compare interrupt enable
	OCR0A = 250-1;				// compare value for 1ms;
	
}

//ISR for Timer 0 compare interrupt
ISR(TIMER0_COMP_vect){
	
	sys_time++; //system time generation
	//for every time the timer equals 249 an interrupt is generated resulting in invreasing the SYStime

}
