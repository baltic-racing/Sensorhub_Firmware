/*
 * system_config.c
 *
 * Created: 09.05.2024 16:08:33
 *  Author: aaron
 */ 
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
	//CLTF1, CLTF2, CLTR1, CLTR2 (F --> Flow, R --> Return)
	DDRF &= ~(1<<PF0) & ~(1<<PF1) & ~(1<<PF2) & ~(1<<PF3);
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