/*
 * AT90CAN_SHB.c
 *
 * Created: 10.10.2024 15:46:56
 * Author : Egquus
 */ 

#include "main.h"

uint8_t wheelspeed = 0;

int main(void)
{
    sys_timer_config();
	SPI_MasterInit();
	
	
	sei();
	
	//	SUPERLOOP STARTS HERE
	
    while (1) 
    {
		if(TIME_PASSED_1_MS)
		{
			time_1ms = sys_time;
		} // end of 1ms
		
		if(TIME_PASSED_10_MS)
		{
			time_10ms = sys_time;
			
			SPI_Data_Reg = 0x22;		// Write the Register will start the conversation
			SPI_Data_Reg = wheelspeed;
			
		} // end of 10ms

		if (TIME_PASSED_100_MS)
		{
			time_100ms = sys_time;
			
			sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			
		} // end of 200ms

    }
}

