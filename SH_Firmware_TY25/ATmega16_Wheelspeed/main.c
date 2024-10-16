/*
 * ATmega16_Wheelspeed.c
 *
 * Created: 09.10.2024 18:21:32
 * Author : racin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"


int main(void)
{
	sys_timer_config();
	SPI_SlaveInit();
	uint8_t data = 0;
	
	sei();
	
    while (1) 
    {
		data = SPI_SlaveReceive();
		
		if(TIME_PASSED_1_MS)
		{
			time_1ms = sys_time;
			calc_wheelspeed_floating();
		} // end of 1ms
		
		if(TIME_PASSED_10_MS)
		{
			time_10ms = sys_time;
			
			
		} // end of 10ms

		if (TIME_PASSED_100_MS)
		{
			time_100ms = sys_time;
			
			//sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			
		} // end of 200ms
    }
}