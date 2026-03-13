/*
 * Calc_wheelspeed.c
 *
 * Created: 16.10.2024 12:58:43
 *  Author: Egquus
 */ 

#include "calc_wheelspeed.h"

volatile uint8_t wheelspeed_left = 0;
volatile uint8_t wheelspeed_right = 0;
volatile uint16_t time_left = 0;
volatile uint16_t time_right = 0;

void PORT_Config(){	
	
	// set PD2 and PB2 as input
	DDRD &= ~(1 << PD2);					
	DDRB &= ~(1 << PB2);
	
		// configure INT0: ISC01 = 1 && ISC02 = 0 --> INT0 = falling edge
	MCUCR |= (1 << ISC01);					//MCUCR = SMCU Control_Reg
	MCUCR &= ~(1 << ISC00);
	
		// configure INT2: ISC2 = 0 --> falling edge
	MCUCSR &= ~(1 << ISC2);
	
	//General Interrupt Flag Register --> delete Flags (before activation)
	GIFR |= (1<<INTF0) | (1<<INTF2);
	// activate interrupts INT0 & INT2
	GICR |= (1 << INT0);
	GICR |= (1 << INT2);
}


//Digi2-in // PD2
ISR(INT0_vect)
{
	time_left = time_delta_left;	//count time in 0.1ms between two interrupt
	time_delta_left = 0;
}


//Digi1-in // PB2
ISR(INT2_vect)
{
	time_right = time_delta_right;	//count time in 0.1ms between two interrupt
	time_delta_right = 0;
}


void calc_speed(void)
{
	wheelspeed_left = (100000/(16*time_left));			//*16 because of 16 teeths // *100000 for seconds
	wheelspeed_right = (100000/(16*time_right));
		
	time_left = 0;
	time_right = 0;
	
	
}

/*  _________________________________________________________________________________________________________
	H	ISC11	H	ISC10	H	Descrition																	H
	H___________H___________H_______________________________________________________________________________H
	H	0		H	0		H	The low level of INT1 generates an interrupt request						H
	H	0		H	1		H	Any logical change on INT1 generates an interrupt request.					H
	H	1		H	0		H	The falling edge of INT1 generates an interrupt request.					H
	H	1		H	1		H	The rising edge of INT1 generates an interrupt request.						H
	_________________________________________________________________________________________________________
	Table MCU_Control_Reg_1	  ->   Interrupt 1 Sense Control												*/