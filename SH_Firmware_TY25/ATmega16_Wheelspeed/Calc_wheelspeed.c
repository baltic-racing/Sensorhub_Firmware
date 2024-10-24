/*
 * Calc_wheelspeed.c
 *
 * Created: 16.10.2024 12:58:43
 *  Author: Egquus
 */ 

#include "calc_wheelspeed.h"

uint8_t spi_data[2];

#define desired_update_frequency 100 //Update frequency for the floating calculation of the Wheelspeed
#define trigger_angle 11.25 //Trigger Angle in degree both high & low are the same
#define Tcirc 1476.5485 // Tire circumference in mm
#define Tcirc_16 92.2842 // Tire circumference/16 in mm

extern volatile unsigned long sys_time;

extern volatile uint8_t wheelspeed;

volatile unsigned long time_old = 0;
volatile unsigned long int sys_time_old = 0;
uint16_t delta = 0;


void PORT_Config(){		//enable Pin change Interrupt on Digital_in pin PD3 (PIN 12)
	
	DDRD &= ~(1 << PD3);					// set digital_input as Input
	
	// Konfiguriere INT0 für fallende Flanke
	MCUCR |= (1 << ISC11);				// MCUCR = SMCU Control_Reg
	MCUCR |= (1 << ISC10);
	
	// Aktiviere INT0
	GICR |= (1 << INT1);
}

ISR(INT1_vect){
	
	//delta berechnen
	//quasie die zeit um 360° zu rotieren
	delta = (sys_time - sys_time_old) * 16;
	
	sys_time_old = sys_time; 

}

void speed(){
	
	//uint8_t wheelspeed = (Tcirc_16/trigger_angle)/(trigger_angle/delta);
	

	spi_data[0]= wheelspeed << 8;		//MSB
	spi_data[1] = wheelspeed & 0xff;	//LSB
	
	wheelspeed = ((Tcirc)/(delta))*3,6; //wheelspeed = (Tcirc_16*1000)/(delta/60/60/60);
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