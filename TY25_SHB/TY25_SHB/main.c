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

//wheelspeed
uint8_t switchi = 0;
uint8_t switchj = 0;
uint8_t wheelspeed_left_lsb = 0;
uint8_t wheelspeed_left_msb = 0;
uint16_t wheelspeed_left = 0;
uint8_t wheelspeed_right_lsb = 0;
uint8_t wheelspeed_right_msb = 0;
uint16_t wheelspeed_right = 0;

//Typ K
uint16_t TK1_temp = 0;
uint16_t TK2_temp = 0;
uint16_t TK3_temp = 0;
uint16_t TK4_temp = 0;



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
	can_SensorHubB0_mob.mob_id = 0x410;
	can_SensorHubB0_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHubB0_mob.mob_number = 0;
	uint8_t SensorHubB0_databytes[8] = {0};
		
	struct CAN_MOB can_SensorHubB1_mob;
	can_SensorHubB1_mob.mob_id = 0x411;
	can_SensorHubB1_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHubB1_mob.mob_number = 1;
	uint8_t SensorHubB1_databytes[8] = {0};

	struct CAN_MOB can_SensorHubB2_mob;
	can_SensorHubB2_mob.mob_id = 0x412;
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
 			time_10ms = sys_time;
			
		} // end of 10ms

		if (TIME_PASSED_100_MS)
		{
			time_100ms = sys_time;
			
			//wheelspeed left
			if (switchi == 0){
				SS_uC_LOW();
				SPDR = 0x03;
				while(!(SPSR & (1 << SPIF)));
				SS_uC_HIGH();
				switchi++;
			}
			if (switchi == 1){
				SS_uC_LOW();
				SPDR = 0x02;
				while(!(SPSR & (1 << SPIF)));
				wheelspeed_left_lsb = SPDR;
				SS_uC_HIGH();
				switchi++;
			}
			if(switchi == 2){
				SS_uC_LOW();
				SPDR = 0x03;
				while(!(SPSR & (1 << SPIF)));
				wheelspeed_left_msb = SPDR;
				SS_uC_HIGH();
				switchi = 1;
			}
			
			//wheelspeed right
			if (switchj == 0){
				SS_uC_LOW();
				SPDR = 0x01;
				while(!(SPSR & (1 << SPIF)));
				SS_uC_HIGH();
				switchj++;
			}
			if (switchj == 1){
				SS_uC_LOW();
				SPDR = 0x04;
				while(!(SPSR & (1 << SPIF)));
				wheelspeed_right_lsb = SPDR;
				SS_uC_HIGH();
				switchj++;
			}
			if (switchj == 2){
				SS_uC_LOW();
				SPDR = 0x01;
				while(!(SPSR & (1 << SPIF)));
				wheelspeed_right_msb = SPDR;
				SS_uC_HIGH();
				switchj = 1;
			}
			
			//wheelspeed
			wheelspeed_left = (wheelspeed_left_msb << 8) | wheelspeed_left_lsb;
			wheelspeed_right = (wheelspeed_right_msb << 8) | wheelspeed_right_lsb;
			
			// damper travel
			uint16_t federwegRL =  SPRINGTRAVEL_MAX - DAMP_MAX_FL + damper_poti((float)adc_get(4));
			uint16_t federwegRR =  SPRINGTRAVEL_MAX - DAMP_MAX_FR + damper_poti((float)adc_get(6));
			
			// CAN bus
			SensorHubB0_databytes[0]	=	0														;	//lsb 
			SensorHubB0_databytes[1]	=	0														;	//msb 
			SensorHubB0_databytes[2]	=	0														;	//lsb 
			SensorHubB0_databytes[3]	=	0														;	//msb 
			SensorHubB0_databytes[4]	=	federwegRL & 0xFF										;	
			SensorHubB0_databytes[5]	=	federwegRL >> 8											;
			SensorHubB0_databytes[6]	=	federwegRR & 0xFF										;	//
			SensorHubB0_databytes[7]	=	federwegRR >> 8											;	
			
			can_tx(&can_SensorHubB0_mob, SensorHubB0_databytes);			
			
			sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			
			// cooling temperature
			uint16_t tempRU =  temp_calc((float)adc_get(0));
			uint16_t tempRD =  temp_calc((float)adc_get(1));
			uint16_t tempLU =  temp_calc((float)adc_get(2));
			uint16_t tempLD =  temp_calc((float)adc_get(7));
			
			// CAN bus
			SensorHubB1_databytes[0]	=	tempRU & 0xFF											;	// lsb temperature right up
			SensorHubB1_databytes[1]	=	tempRU >> 8												;	// msb temperature right up
			SensorHubB1_databytes[2]	=	tempRD & 0xFF											;	// lsb temperature right down
			SensorHubB1_databytes[3]	=	tempRD >> 8												;	// msb temperature right down
			SensorHubB1_databytes[4]	=	tempLU & 0xFF											;	// lsb temperature left up
			SensorHubB1_databytes[5]	=	tempLU >> 8												;	// msb temperature left up
			SensorHubB1_databytes[6]	=	tempLD & 0xFF											;	// lsb temperature left down
			SensorHubB1_databytes[7]	=	tempLD >> 8												;	// msb temperature left down
			
			can_tx(&can_SensorHubB1_mob, SensorHubB1_databytes);
			
			//// Typ K temperature
			//TK1_temp = (uint16_t)read_TK_temperature(TK1);
			//TK2_temp = (uint16_t)read_TK_temperature(TK2);
			//TK3_temp = (uint16_t)read_TK_temperature(TK3);
			//TK4_temp = (uint16_t)read_TK_temperature(TK4);
			//
			//SensorHubB2_databytes[0]	=	0														;	//reserved
			//SensorHubB2_databytes[1]	=	0														;	//reserved
			//SensorHubB2_databytes[2]	=	0														;	//reserved
			//SensorHubB2_databytes[3]	=	0														;	//reserved
			//SensorHubB2_databytes[4]	=	0														;	//reserved
			//SensorHubB2_databytes[5]	=	0														;	//reserved
			//SensorHubB2_databytes[6]	=	0														;	//reserved
			//SensorHubB2_databytes[7]	=	0														;	//reserved
			//
			//can_tx(&can_SensorHubB2_mob, SensorHubB2_databytes);	//reserved

			
		} // end of 200ms

    }
}

