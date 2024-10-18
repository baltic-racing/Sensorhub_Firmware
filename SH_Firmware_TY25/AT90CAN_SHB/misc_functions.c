/*
 * misc_functions.c
 *
 * Created: 10.10.2024 16:22:24
 *  Author: Egquus
 */ 

#include "misc_functions.h"

volatile unsigned long sys_time = 0;

void port_config(void)
{
	// Set Fault(PCO) and Heart(PC2) LED as Output
	DDRC |= (1<<PC0) | (1<<PC2);
}

void sys_timer_config(void)
{
	//CTC-Mode, /64
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);
	//Compare value for 1ms (Formula in Datasheet)
	OCR0A = 249; //249
	//Compare Interrupt Enable
	TIMSK0 = 0 | (1<<OCIE0A);
}

void sys_tick_heart()
{
	PORTC ^= (1<<PC2); //toggle the Heart led on Pin 4, will be used in the super loop in main.c to indicade that the loop is running correctly
};

void fault_not_detected()
{
	PORTC &= ~(1<<PC0); //turn off red led in case of no fault
}

void fault_detected()
{
	PORTC |= (1<<PC0); //turn on red led when called (fault present)
}

ISR(TIMER0_COMP_vect)
{
	sys_time++;
}