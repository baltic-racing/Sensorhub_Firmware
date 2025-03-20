/*
 * Calc_wheelspeed.c
 *
 * Created: 16.10.2024 12:58:43
 *  Author: Egquus
 */ 

#include "calc_wheelspeed.h"

#define desired_update_frequency 100 //Update frequency for the floating calculation of the Wheelspeed
#define trigger_angle 11.25 //Trigger Angle in degree both high & low are the same
#define Tcirc 1476.5485 // Tire circumference in mm
#define Tcirc_16 92.2842 // Tire circumference/16 in mm

extern volatile unsigned long sys_time;

volatile uint8_t wheelspeed_calc;
volatile uint8_t wheelspeed;
volatile uint8_t rotaion;

volatile unsigned long time_old_ws = 0;
volatile unsigned long int sys_time_old_ws = 0;
uint64_t delta_ws = 0;


void PORT_Config(){							//enable Pin change Interrupt on Digital_in pin PD3 (PIN 12)
	
	DDRD &= ~(1 << PD2);					// set digital_input as Input
	
	// Konfiguriere INT0 für fallende Flanke
	MCUCR |= (1 << ISC11);					// MCUCR = SMCU Control_Reg
	MCUCR |= (1 << ISC10);
	
	// Aktiviere INT0
	GICR |= (1 << INT0);
}

ISR(INT0_vect){
	
	//delta berechnen
	//quasie die zeit um 360° zu rotieren
	delta_ws = ((sys_time - sys_time_old_ws) * 16);
	
	wheelspeed = speed();
	
	sys_time_old_ws = sys_time; 
}

uint16_t speed(){
	
	//uint8_t wheelspeed = (Tcirc_16/trigger_angle)/(trigger_angle/delta);
	//wheelspeed = ((Tcirc)/(delta_ws))*3,6; //wheelspeed = (Tcirc_16*1000)/(delta/60/60/60);
	
	//wheelspeed = ((1476.5485)/(delta_ws))*3.6;
	wheelspeed_calc = ((1476.5485*3600)/(delta_ws*100000));
	rotaion = Tcirc_16 / delta_ws;
	return rotaion;					// Cut the double long (wheelspeed_calc) to uint16_t (speed)
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