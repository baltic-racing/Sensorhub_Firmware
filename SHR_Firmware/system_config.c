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
	//APPS1, APPS2
	DDRF &= ~(1<<PF0) & ~(1<<PF1);
}

void sys_timer_config(void)
{
	//CTC-Mode, /64
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);
	//Compare value for 1ms (Formula in Datasheet)
	OCR0A = 249;
	//Compare Interrupt Enable
	TIMSK0 = 0 | (1<<OCIE0A);
}

/*	Interrupt Service Routines	*/
ISR(TIMER0_COMP_vect)
{
	sys_tick++;
}