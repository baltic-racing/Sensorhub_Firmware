/*
 * misc_functions.c
 *
 * Created: 10.10.2024 16:22:24
 *  Author: Egquus
 */ 

#include "misc_functions.h"

volatile unsigned long sys_time = 0;

void sys_timer_config(){
	//Timer/Counter0 Control Register = Waveform Generation Mode | Compare Match Ouput Mode(0/1)
	//COM01 = 0 && COM00 => Normal port operation, OC0A disconnected
	//WGM01 = 1 && WGM00 = 0 => Clear Timer on Compare (CTC) enabled => double buffering disabled
	//CS02 = 0 && CS01 = 1 && CS00 = 1 => CLK prescaler 64
	TCCR0 = 0 | (1<<WGM01) | (0<<COM01) | (0<<COM00) | (0<<CS02) | (1<<CS01) | (1<<CS00);
	//Systime Calc 16Mhz external Qaurtz via AT90CAN
	//Prescaler = 64 =>  250 kHz
	
	//Timer/Counter0 Interrupt Mask Register |= Ouput Compare Match A Interrupt Enable | Timer/Counter0 Overflow Interrupt Enable
	TIMSK |= (1<<OCIE0) | (1<<TOIE0); //compare interrupt enable
	
	OCR0 = 250-1;
	//Prescaler = 64 =>  250 kHz => 1kHz
	
	// Goal 10 Khz set OCR0A Value accordingly to 100
}

void sys_tick_heart()
{
	//PORT_ ^= (1<<P__); //toggle the Heart led on Pin 4, will be used in the super loop in main.c to indicade that the loop is running correctly
};

void fault_not_detected()
{
	//PORT_ &= ~(1<<P__); //turn off red led in case of no fault
}

void fault_detected()
{
	//PORT_ |= (1<<P__); //turn on red led when called (fault present)
}

ISR(TIMER0_COMP_vect)
{
	sys_time++;
}