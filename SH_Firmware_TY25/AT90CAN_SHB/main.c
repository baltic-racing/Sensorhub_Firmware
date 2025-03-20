/*
 * AT90CAN_SHB.c
 *
 * Created: 10.10.2024 15:46:56
 * Author : Egquus
 */ 

#include "main.h"

uint8_t switchi = 1;

volatile uint16_t wheelspeed[2];

extern uint8_t SensorHub0_databytes[8];

extern struct CAN_MOB can_SensorHub0_mob;

int main(void)
{
	port_config();
    sys_timer_config();
	SPI_MasterInit();
	can_cfg();
	CAN_Init_Messages();
	
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
			if(switchi == 1){
 			time_10ms = sys_time;
 			PORTE &= ~(1<<SS_uC);
 			SPDR = 0x22;										// Write the Register will start the conversation
 			while(!(SPSR & (1<<SPIF)));
 			//PORTE |= (1<<SS_uC);
 			wheelspeed [0] = SPI_Data_Reg;
			
			switchi = 0;
			}else{
			//PORTE &= ~(1<<SS_uC);
			//SPDR = 0x33;										// Write the Register will start the conversation
			//while(!(SPSR & (1<<SPIF)));
			//PORTE |= (1<<SS_uC);
			//wheelspeed [1] = SPI_Data_Reg;
			
			switchi = 1;
			}
			
		} // end of 10ms

		if (TIME_PASSED_100_MS)
		{
			time_100ms = sys_time;
			
			SensorHub0_databytes[0]	=	wheelspeed[0]&0xff		;
			SensorHub0_databytes[1]	=	(wheelspeed[1]>>8)&0xff	;
			SensorHub0_databytes[2]	=	0						;
			SensorHub0_databytes[3]	=	0						;
			SensorHub0_databytes[4]	=	0						;
			SensorHub0_databytes[5]	=	0						;
			SensorHub0_databytes[6]	=	0						;
			SensorHub0_databytes[7]	=	0						;
			
			
			can_tx(&can_SensorHub0_mob, SensorHub0_databytes);
			
			sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			
		} // end of 200ms

    }
}

