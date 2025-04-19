/*
 * Calc_wheelspeed.c
 *
 * Created: 16.10.2024 12:58:43
 *  Author: Egquus
 */ 

#include "calc_wheelspeed.h"

#define desired_update_frequency 100 //Update frequency for the floating calculation of the Wheelspeed
#define PULSES_PER_ROTATION 16
//#define trigger_angle 11.25 //Trigger Angle in degree both high & low are the same
#define TRIGGER_ANGLE_DEG 22.5
#define TIRE_CIRCUMFERENCE_MM 1476.5485 // Tire circumference in mm
#define Tcirc_16 92.2842 // Tire circumference/16 in mm
#define DISTANCE_PER_PULSE_MM (TIRE_CIRCUMFERENCE_MM * (TRIGGER_ANGLE_DEG / 360.0f))
#define SWITCH_THERESHOLD_STEPS 10 // ab ~35km/h

#define TICK_DURATION_US 4.0f
#define TIMER_TICKS_PER_MS 250

extern volatile unsigned long sys_time;
extern volatile unsigned long time_delta_left;

//volatile uint8_t wheelspeed_calc;
volatile uint8_t wheelspeed_left = 0;
volatile uint8_t wheelspeed_right = 0;
volatile uint16_t steps_per_intervall_left = 0;
volatile uint16_t steps_per_intervall_right = 0;
volatile uint16_t timestepdiff_left = 0;
volatile uint16_t timestepdiff_right = 0;

volatile unsigned long time_old_ws = 0;
volatile unsigned long int sys_time_old_ws = 0;
//uint64_t delta_ws = 0;




void PORT_Config(){					
	
	DDRD &= ~(1 << PD2);					// Configure PD2 (INT0 pin) as input
	
	// Configure INT0 for falling edge
	MCUCR |= (1 << ISC01);					// MCUCR = SMCU Control_Reg
	MCUCR &= ~(1 << ISC00);
	
	GICR |= (1 << INT0);					// Enable external interrupt INT0
}


ISR(INT0_vect){
	
	timestepdiff_left = time_delta_left;		// Save the measured time between two impulses
	time_delta_left = 0;					   	// Reset time counter for next measurement
	steps_per_intervall_left++;					// Count number of impulses in current interval// Count number of impulses in current interval
	
}


//different calculation for high speed and low speed, transition point is configurable
void calc_speed(void){
	if (steps_per_intervall_left >= SWITCH_THERESHOLD_STEPS){
		calc_speed_high(LEFT, steps_per_intervall_left);
	}
	else{
		calc_speed_low(LEFT, timestepdiff_left);
	}
	
	if (steps_per_intervall_right >= SWITCH_THERESHOLD_STEPS){
		calc_speed_high(RIGHT, steps_per_intervall_right);
	}
	else{
		calc_speed_low(RIGHT, timestepdiff_right);
	}
	
	steps_per_intervall_left = 0;
	steps_per_intervall_right = 0;
}

// low speed: time per degree -> degree per time -> km/h
void calc_speed_low (uint8_t side, double timestep_diff_ms){
	double ang_vel = timestep_diff_ms / TRIGGER_ANGLE_DEG;
	if (ang_vel == 0){
		if (side == LEFT) wheelspeed_left = 0;
		else wheelspeed_right = 0;
		return;
	}
	
	ang_vel = 1.0 / ang_vel;
	double RPS = (ang_vel / 360.0) * 1000.0;
	double speed_kmh = RPS * (TIRE_CIRCUMFERENCE_MM / 1000.0) * 3.6;
	double RPM = RPS * 60;
	
	if (side == LEFT) wheelspeed_left = (uint8_t) speed_kmh;
	else wheelspeed_right = (uint8_t) speed_kmh;
}

// high speed: impulses per second -> rounds per second -> km/h
void calc_speed_high(uint8_t side, uint8_t steps_per_intervall){
	
	double impulses_per_second = (steps_per_intervall * 1000.0) / update_frequency_simple_ms;
	double RPS = impulses_per_second / number_of_teeth; 
	double RPM = RPS * 60;
	double speed_kmh = RPS * (TIRE_CIRCUMFERENCE_MM / 1000) * 3.6;
	
	
	if (side == LEFT) wheelspeed_left = (uint8_t)speed_kmh;
	else wheelspeed_right = (uint8_t)speed_kmh;
}




//uint16_t speed(){
	//
	////uint8_t wheelspeed = (Tcirc_16/trigger_angle)/(trigger_angle/delta);
	////wheelspeed = ((Tcirc)/(delta_ws))*3,6; //wheelspeed = (Tcirc_16*1000)/(delta/60/60/60);
	//
	////wheelspeed = ((1476.5485)/(delta_ws))*3.6;
	//wheelspeed_calc = ((1476.5485*3600)/(delta_ws*100000));
	//rotaion = Tcirc_16 / delta_ws;
	//return rotaion;					// Cut the double long (wheelspeed_calc) to uint16_t (speed)
//}

/*  _________________________________________________________________________________________________________
	H	ISC11	H	ISC10	H	Descrition																	H
	H___________H___________H_______________________________________________________________________________H
	H	0		H	0		H	The low level of INT1 generates an interrupt request						H
	H	0		H	1		H	Any logical change on INT1 generates an interrupt request.					H
	H	1		H	0		H	The falling edge of INT1 generates an interrupt request.					H
	H	1		H	1		H	The rising edge of INT1 generates an interrupt request.						H
	_________________________________________________________________________________________________________
	Table MCU_Control_Reg_1	  ->   Interrupt 1 Sense Control												*/