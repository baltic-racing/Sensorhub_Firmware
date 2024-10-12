/*
 * CFile1.c
 *
 * Created: 28.04.2023 17:41:58
 *  Author: nico
 */ 

/*	INCLUDES	*/

#include "system_config.h"

/*	Extern Global Variables	*/
extern unsigned long sys_tick;

/*	FUNCTIONS	*/

void port_config(void)
{
	//Fault-LED, Heart-LED
	DDRC |= (1<<PC0) | (1<<PC2);
	//CS-TYPK1, CS-TYPK2
	DDRE |= (1<<PE0) | (1<<PE1);
	//BPS1, BPS2, SA (Steering Angle)
	DDRF &= ~(1<<PF0) & ~(1<<PF1) & ~(1<<PF2);
}

void sys_timer_config(void)
{
	//8 bit Timer 0 configuration
	//ctc mode and 64 as prescaler for 16Mhz
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);
	TIMSK0 = 0 | (1<<OCF0A);	//compare interrupt enable
	OCR0A = 250-1;
	
}

/*	Interrupt Service Routines	*/
ISR(TIMER0_COMP_vect)
{
	sys_tick++;
}