/*
 * Calc_wheelspeed.c
 *
 * Created: 16.10.2024 12:58:43
 *  Author: Egquus
 */ 

#include "calc_wheelspeed.h"

volatile uint16_t wheelspeed_left = 0;
volatile uint16_t wheelspeed_right = 0;
volatile uint16_t steps_per_intervall_left = 0;
volatile uint16_t steps_per_intervall_right = 0;
volatile uint16_t timestepdiff_left = 0;
volatile uint16_t timestepdiff_right = 0;

volatile unsigned long time_old_ws = 0;
volatile unsigned long int sys_time_old_ws = 0;
//uint64_t delta_ws = 0;


void PORT_Config(){							//enable Pin change Interrupt on Digital_in pin PD3 (PIN 12)
	
	DDRD &= ~(1 << PD2);					// set digital_input as Input
	DDRB &= ~(1 << PB2);
	
	// Konfiguriere INT0 für fallende Flanke
	MCUCR |= (1 << ISC01);					// MCUCR = SMCU Control_Reg
	MCUCR &= ~(1 << ISC00);
	
	MCUCSR &= ~(1 << ISC2);
	
	// Aktiviere INT0
	GICR |= (1 << INT0);
	GICR |= (1 << INT2);
}

ISR(INT0_vect){
	timestepdiff_left = time_delta_left;
	time_delta_left = 0;
	steps_per_intervall_left++;
}

ISR(INT2_vect){
	timestepdiff_right = time_delta_right;
	time_delta_right = 0;
	steps_per_intervall_right++;
}

void calc_speed(void){
	if (steps_per_intervall_left >= SWITCH_THERESHOLD_STEPS){
		calc_speed_high(LEFT, steps_per_intervall_left);
	}
	else{
		calc_speed_low(LEFT, timestepdiff_left);
	}
	
	if (steps_per_intervall_right >= SWITCH_THERESHOLD_STEPS){
		calc_speed_high(RIGHT, steps_per_intervall_right);
	}else{
		calc_speed_low(RIGHT, timestepdiff_right);
	}
	
	if (steps_per_intervall_left == 0) wheelspeed_left = 0;
	if (steps_per_intervall_right == 0) wheelspeed_right = 0;
	
	steps_per_intervall_left = 0;
	steps_per_intervall_right = 0;
}

void calc_speed_low(uint8_t side, double timestep_diff_ms){
	double ang_vel = timestep_diff_ms / TRIGGER_ANGLE_DEG;
	if (ang_vel == 0){
		if (side == LEFT) wheelspeed_left = 0;
		else wheelspeed_right = 0;
	}
	
	ang_vel = 1.0 / ang_vel;
	double RPS = (ang_vel / 360.0) * 1000.0;
	double speed_kmh = RPS * (TIRE_CIRCUMFERENCE_MM / 1000.0) * 3.6;
	double RPM = RPS * 60;
	
	if (side == LEFT) wheelspeed_left = (uint16_t) RPM;
	else wheelspeed_right = (uint16_t) RPM;
}

void calc_speed_high(uint8_t side, uint8_t steps_per_intervall){
	if (steps_per_intervall == 0){
		if (side == LEFT) wheelspeed_left = 0;
		else wheelspeed_right = 0;
	}
	
	double impulses_per_second = (steps_per_intervall * 1000.0) / update_frequency_simple_ms;
	double RPS = impulses_per_second / number_of_teeth;
	double RPM = RPS * 60;
	double speed_kmh = RPS * (TIRE_CIRCUMFERENCE_MM / 1000) * 3.6;
	
	if (side == LEFT) wheelspeed_left = (uint16_t) RPM;
	else wheelspeed_right = (uint16_t) RPM;
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