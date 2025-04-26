/*
 * main.c
 *
 * Created: 31.03.2025 14:33:49
 *  Author: pasca
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "misc_functions.h"
#include "adc_functions.h"
#include "CAN_lib.h"
#include "CAN_data.h"
#include "sensor_function.h"
#include "misc_functions.h"
#include "main.h"

uint8_t switchi = 1;
uint8_t steering_sign = 0;		//indicator for steering percentage

volatile uint16_t wheelspeed[2];

extern uint8_t SensorHubB0_databytes[8];
extern uint8_t SensorHubB1_databytes[8];
extern uint8_t SensorHubB2_databytes[8];

extern struct CAN_MOB can_SensorHubB0_mob;
extern struct CAN_MOB can_SensorHubB1_mob;
extern struct CAN_MOB can_SensorHubB2_mob;


int main(void)
{
	port_config();
    sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	SPI_MasterInit();
	can_cfg();
	CAN_Init_Messages();
	
	struct CAN_MOB can_SensorHubB0_mob;
	can_SensorHubB0_mob.mob_id = 0x400;
	can_SensorHubB0_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHubB0_mob.mob_number = 0;
	uint8_t SensorHubB0_databytes[8] = {0};
		
	struct CAN_MOB can_SensorHubB1_mob;
	can_SensorHubB1_mob.mob_id = 0x410;
	can_SensorHubB1_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHubB1_mob.mob_number = 1;
	uint8_t SensorHubB1_databytes[8] = {0};

	struct CAN_MOB can_SensorHubB2_mob;
	can_SensorHubB2_mob.mob_id = 0x420;
	can_SensorHubB2_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHubB2_mob.mob_number = 2;
	uint8_t SensorHubB2_databytes[8] = {0};
	
	sei();	
	
	//	SUPERLOOP STARTS HERE
	
    while (1) 
    {
		if(TIME_PASSED_1_MS)
		{
			time_1ms = sys_time;
			adc_start_conversion();
			
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
			
			//cooling temperature
			uint16_t tempRU =  temp_calc((float)adc_get(0));
			uint16_t tempRD =  temp_calc((float)adc_get(1));
			uint16_t tempLU =  temp_calc((float)adc_get(2));
			uint16_t tempLD =  temp_calc((float)adc_get(7));
			
			//damper travel
			uint16_t federwegRL =  SPRINGTRAVEL_MAX - DAMP_MAX_FL + damper_poti((float)adc_get(4));
			uint16_t federwegRR =  SPRINGTRAVEL_MAX - DAMP_MAX_FR + damper_poti((float)adc_get(6));
			
			
			SensorHubB0_databytes[0]	=	wheelspeed[0]&0xff										;	//lsb 
			SensorHubB0_databytes[1]	=	(wheelspeed[1]>>8)&0xff									;	//msb 
			SensorHubB0_databytes[2]	=	0														;	//lsb 
			SensorHubB0_databytes[3]	=	0														;	//msb 
			SensorHubB0_databytes[4]	=	0														;	
			SensorHubB0_databytes[5]	=	0														;
			SensorHubB0_databytes[6]	=	0														;	//SDC
			SensorHubB0_databytes[7]	=	0														;	
			
			//uint16_t testBPS = ADC2Sensor(adc_get(0),0.0,5.0,100,10,5,100);
			SensorHubB1_databytes[0]	=	tempRU & 0xFF											;	// lsb temperature right up
			SensorHubB1_databytes[1]	=	tempRU >> 8												;	// msb temperature right up
			SensorHubB1_databytes[2]	=	tempRD & 0xFF											;	// lsb temperature right down
			SensorHubB1_databytes[3]	=	tempRD >> 8												;	// msb temperature right down
			SensorHubB1_databytes[4]	=	tempLU & 0xFF											;	// lsb temperature left up	
			SensorHubB1_databytes[5]	=	tempLU >> 8												;	// msb temperature left up
			SensorHubB1_databytes[6]	=	tempLD & 0xFF											;	// lsb temperature left down
			SensorHubB1_databytes[7]	=	tempLD >> 8												;	// msb temperature left down
			
			SensorHubB2_databytes[0]	=	federwegRL & 0xFF										;	//lsb damper travel rear left
			SensorHubB2_databytes[1]	=	federwegRL >> 8											;	//msb damper travel rear left
			SensorHubB2_databytes[2]	=	federwegRR & 0xFF										;	//lsb damper travel rear right
			SensorHubB2_databytes[3]	=	federwegRR >> 8											;	//msb damper travel rear right
			SensorHubB2_databytes[4]	=	0														;	//
			SensorHubB2_databytes[5]	=	0														;	//
			SensorHubB2_databytes[6]	=	0														;
			SensorHubB2_databytes[7]	=	0														;
			
			can_tx(&can_SensorHubB0_mob, SensorHubB0_databytes);			
			can_tx(&can_SensorHubB1_mob, SensorHubB1_databytes);
			can_tx(&can_SensorHubB2_mob, SensorHubB2_databytes);
			
			sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			
		} // end of 200ms

    }
}

