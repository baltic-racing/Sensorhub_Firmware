/*
 * ATmega16_Wheelspeed.c
 *
 * Created: 09.10.2024 18:21:32
 * Author : racin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "SPI.h"

#define desired_update_frequency 100 //Update frequency for the floating calculation of the Wheelspeed
#define trigger_angle 11.25 //Trigger Angle in degree both high & low are the same
#define Tcirc 1476.5485 // Tire circumference in mm

unsigned long int time_old = 0;
unsigned long int sys_time = 0;
uint8_t sys_time_10 = 0;

uint8_t spi_data[2];
uint8_t interval_width = 0;
uint16_t Wheelspeed = 0; //[km/h]

unsigned long int timediff[10] = {0};

float ideal_measurement_angle = 0;
float possible_measurement_angle = 0;
volatile float RPS = 0;
float timediffinterval = 0;


int main(void)
{
	SPI_SlaveInit();
	uint8_t data = 0;
    /* Replace with your application code */
    while (1) 
    {
		data = SPI_SlaveReceive();
		
		if((sys_time-time_old) >= 1)
		{
			time_old = sys_time;
			sys_time_10++;
		}
		
		if(sys_time_10 >= 10)
		{
			sys_time_10 = 0;
			
			
			
		}
		
    }
}

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

void calc_wheelspeed_floating()
{
		//calculate ideal Measurement Angle
		//derive real possible measurement angle from that
		//then integrate the time over the desired interval
		//calculate wheelspeed from that
		ideal_measurement_angle = RPS * 360 / desired_update_frequency;
		interval_width = ideal_measurement_angle / trigger_angle;
		possible_measurement_angle = interval_width * trigger_angle;
		if (possible_measurement_angle == 0){
			possible_measurement_angle = trigger_angle;
		}
		for (uint8_t i = interval_width; i <= 1;)
		{
			timediffinterval += timediff[i];
		}
		RPS = 1 / ((timediffinterval / interval_width) * (360 / 1000));
		Wheelspeed = RPS * (Tcirc / (1000*1000)) * (60*60);
		spi_data[0]= Wheelspeed << 8; //MSB
		spi_data[1] = Wheelspeed & 0xff;	//LSB
	
}

