/*
 * Calc_wheelspeed.c
 *
 * Created: 16.10.2024 12:58:43
 *  Author: Egquus
 */ 

#include "calc_wheelspeed.h"

volatile uint8_t wheelspeed_left;
volatile uint8_t wheelspeed_right;
volatile uint16_t time_left = 0;
volatile uint16_t time_right = 0;
volatile uint32_t old_time_left = 0;
volatile uint32_t old_time_right = 0;

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
	time_left = sys_time - old_time_left;	//count time in 0.1ms between two interrupt
	old_time_left = sys_time;
}


//Digi1-in // PB2
ISR(INT2_vect)
{
	time_right = sys_time - old_time_right;	//count time in 0.1ms between two interrupt
	old_time_right = sys_time;
}


void calc_speed(void)
{
	cli();
	
	if((time_left < 1250)&& (time_left >0))				//check if vehicle is moving (t=1250 == 0.5U/s) and t>0 to prevent division by zero
	{
		wheelspeed_left = (100000/(16*time_left));	
	}
		else
		{
			wheelspeed_left = 0;
		}
		
	if((time_right < 1250)&& (time_right >0)){
		wheelspeed_right = (100000/(16*time_right));
	}
		else
		{
			wheelspeed_right = 0;
		}
	sei();
}														// end calc_wheelspeed

	/*
		16 teeth per revolution
		255 bits for data -> unit = U/s -> max = 25.5U/s -> max_v = 25.5U/s * 1.5m/U = 38.25m/s = 137.7km/h
		
		uC runs with 
	*/

/*  _________________________________________________________________________________________________________
	H	ISC11	H	ISC10	H	Descrition																	H
	H___________H___________H_______________________________________________________________________________H
	H	0		H	0		H	The low level of INT1 generates an interrupt request						H
	H	0		H	1		H	Any logical change on INT1 generates an interrupt request.					H
	H	1		H	0		H	The falling edge of INT1 generates an interrupt request.					H
	H	1		H	1		H	The rising edge of INT1 generates an interrupt request.						H
	_________________________________________________________________________________________________________
	Table MCU_Control_Reg_1	  ->   Interrupt 1 Sense Control												*/