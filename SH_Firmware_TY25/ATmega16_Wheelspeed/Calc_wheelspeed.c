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

extern volatile unsigned long sys_time;
volatile unsigned long int sys_time_old = 0;
volatile unsigned long int timestepdiff[10] = {0};
volatile uint8_t n = 9 ; //Durchlaufvariable für timestepdiff mit den letzten 10 werten

volatile uint8_t spi_data[2];
uint8_t interval_width = 0;
uint16_t Wheelspeed = 0; //[km/h]

unsigned long int timediff[10] = {0};

float ideal_measurement_angle = 0;
float possible_measurement_angle = 0;
volatile float RPS = 0;
float timediffinterval = 0;

void calc_wheelspeed_floating()
{
	//calculate ideal Measurement Angle
	//derive real possible measurement angle from that
	//then integrate the time over the desired interval
	//calculate wheelspeed from that
	ideal_measurement_angle = RPS * 360 / desired_update_frequency;
	interval_width = ideal_measurement_angle / trigger_angle;
	possible_measurement_angle = interval_width * trigger_angle;
	if (possible_measurement_angle == 0)
	{
		possible_measurement_angle = trigger_angle;
	}
	for (uint8_t i = interval_width; i <= 1;)
	{
		timediffinterval += timediff[i];
	}
	RPS = 1 / ((timediffinterval / interval_width) * (360 / 1000));
	Wheelspeed = RPS * (Tcirc / (1000*1000)) * (60*60);
	spi_data[0]= Wheelspeed << 8;		//MSB
	spi_data[1] = Wheelspeed & 0xff;	//LSB
	
}

void PORT_Config(){		//enable Pin change Interrupt on Digital in pin PA0 (PIN 37)
	
	DDRA = (1 << PA0);					// set digital_input as input
	
	// Konfiguriere INT0 für fallende Flanke
	MCUCR |= (1 << ISC01);				// MCUCR = SMCU Control_Reg
	MCUCR &= ~(1 << ISC00);
	
	// Aktiviere INT0
	GICR |= (1 << INT0);
}

ISR(INT0_vect){
	if(n>=1){
		timestepdiff[n] = sys_time - sys_time_old;
		timediff[n] = timestepdiff[n] / 10; //timediff according to 10Khz clock so we divide timesteps by 10 to get timediff in ms
		sys_time_old = sys_time;
		n--;
	}
	else{
		for (uint8_t i= 9; i <= 1;)
		{
			timestepdiff[i]=timestepdiff[i+1];
			i--;
		}
		timestepdiff[0] = sys_time - sys_time_old; //Timer overflow? Timer Resolution in contrast to angular speed
	}
	calc_wheelspeed_floating();
}