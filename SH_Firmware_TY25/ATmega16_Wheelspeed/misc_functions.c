/*
 * misc_functions.c
 *
 * Created: 10.10.2024 16:22:24
 *  Author: Egquus
 */ 

#include "misc_functions.h"

volatile unsigned long sys_time = 0;
volatile unsigned long time_delta_left = 0;
volatile unsigned long time_delta_right = 0;

void sys_timer_config(){
	//Timer/Counter0 Control Register = Waveform Generation Mode | Compare Match Ouput Mode(0/1)
	
	//WGM01 = 1 && WGM00 = 0 => CTC (Clear Timer on Compare Match) enabled -> Timer counts up, when OCR0 is reached, reset to 0 => double buffering disabled
	
	//COM01 = 0 && COM00 = 0 => Normal port operation, OC0A disconnected
	
	//CS02 = 0 && CS01 = 1 && CS00 = 1 => CLK prescaler 64
		// 16MHz clock /  64 Prescaer -> 250kHz 
		
	TCCR0 = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00); //CTC mode & presclaer 64

	
	//TIMSK = Timer/Counter0 Interrupt Mask Register |= Ouput Compare Match A Interrupt Enable | Timer/Counter0 Overflow Interrupt Enable
	
	TIMSK |= (1<<OCIE0); //interrupt enable	//| (1<<TOIE0); -> this interrupt was previous enabled but nowhere used
	
	
	//OCR0 = output compare match interrupt enable

	OCR0 = 24; // TCCR0 -> 250kHz -> OCR0 counts 25times -> 100us interrupts	
	
	
}

void sys_tick_heart()
{
	PORTC ^= (1<<PC0); //toggle the Heart led on Pin 4, will be used in the super loop in main.c to indicade that the loop is running correctly
}

void fault_not_detected()
{
	PORTC &= ~(1<<PC1); //turn off red led in case of no fault
}

void fault_detected()
{
	PORTC |= (1<<PC1); //turn on red led when called (fault present)
}

ISR(TIMER0_COMP_vect)
{
	//1ms interrupts
	time_delta_left++;
	time_delta_right++;
	sys_time++;
}