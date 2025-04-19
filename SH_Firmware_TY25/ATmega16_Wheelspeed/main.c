/*
 * ATmega16_Wheelspeed.c
 *
 * Created: 09.10.2024 18:21:32
 * Author : racin
 */ 

#include "main.h"

#define desired_update_frequency 100 //Update frequency for the floating calculation of the Wheelspeed
#define PULSES_PER_ROTATION 16
#define trigger_angle 11.25 //Trigger Angle in degree both high & low are the same
#define TRIGGER_ANGLE_DEG 22.5
#define TIRE_CIRCUMFERENCE_MM 1476.5485 // Tire circumference in mm
#define Tcirc_16 92.2842 // Tire circumference/16 in mm
#define DISTANCE_PER_PULSE_MM (TIRE_CIRCUMFERENCE_MM * (TRIGGER_ANGLE_DEG / 360.0f))


int main(void)
{
	sys_timer_config();
	SPI_SlaveInit();
	PORT_Config();
	
	sei();

    while (1) 
    {
		
		if(TIME_PASSED_1_MS)
		{
			time_1ms = sys_time;

		} // end of 1ms
		
		if(TIME_PASSED_10_MS)
		{
			time_10ms = sys_time;
			
			
			//float impulses_per_second = pulse_counter * 100.0f;
			//float speed_kmh = impulses_per_second * DISTANCE_PER_PULSE_MM * 0.0036f;
			//
			//if (speed_kmh > 255.0f) speed_kmh = 255.0f;
			//wheelspeed = (uint8_t)speed_kmh;
			//
			//pulse_counter = 0;
			
		} // end of 10ms
		
		if(TIME_PASSED_50_MS)
		{
			time_50ms = sys_time;

		} // end of 1ms

		if (TIME_PASSED_100_MS)
		{
			time_100ms = sys_time;
			
			calc_speed();
			sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			//sys_tick_heart();
			
		} // end of 200ms
    }
}