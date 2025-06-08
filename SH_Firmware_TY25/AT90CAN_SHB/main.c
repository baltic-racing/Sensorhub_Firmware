/*
 * AT90CAN_SHB.c
 *
 * Created: 10.10.2024 15:46:56
 * Author : Egquus
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "misc_functions.h"
#include "adc_functions.h"
#include "CAN_lib.h"
#include "sensor_function.h"
#include "SPI_lib.h"
#include "main.h"

uint8_t switchi = 0;
uint8_t switchj = 0;
uint8_t wheelspeed_left_lsb = 0;
uint8_t wheelspeed_left_msb = 0;
uint16_t wheelspeed_left = 0;
uint8_t wheelspeed_right_lsb = 0;
uint8_t wheelspeed_right_msb = 0;
uint16_t wheelspeed_right = 0;
uint16_t TK1_temp = 0;
uint16_t TK2_temp = 0;
uint16_t TK3_temp = 0;
uint16_t TK4_temp = 0;
double APPS1 = 0;
double APPS2 = 0;



extern uint8_t SensorHub0_databytes[8];
extern uint8_t SensorHub1_databytes[8];
extern uint8_t SensorHub2_databytes[8];

extern struct CAN_MOB can_SensorHub0_mob;
extern struct CAN_MOB can_SensorHub1_mob;
extern struct CAN_MOB can_SensorHub2_mob;


int main(void)
{
	port_config();
    sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	SPI_MasterInit();
	can_cfg();
	CAN_Init_Messages();
	
	struct CAN_MOB can_SensorHub0_mob;
	can_SensorHub0_mob.mob_id = 0x400;
	can_SensorHub0_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHub0_mob.mob_number = 0;
	uint8_t SensorHub0_databytes[8] = {0};
		
	struct CAN_MOB can_SensorHub1_mob;
	can_SensorHub1_mob.mob_id = 0x401;
	can_SensorHub1_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHub1_mob.mob_number = 1;
	uint8_t SensorHub1_databytes[8] = {0};

	struct CAN_MOB can_SensorHub2_mob;
	can_SensorHub2_mob.mob_id = 0x402;
	can_SensorHub2_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHub2_mob.mob_number = 2;
	uint8_t SensorHub2_databytes[8] = {0};
	
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
			if(switchi == 1){
				SS_uC_LOW();
				SPDR = 0x02;
				while(!(SPSR & (1 << SPIF)));
				wheelspeed_left_lsb = SPDR;
				SS_uC_HIGH();
				switchi++;
			}
			if (switchi == 2){
				SS_uC_LOW();
				SPDR = 0x03;
				while (!(SPSR & (1 << SPIF)));
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
				while(!(SPSR & (1 <<SPIF)));
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
			
			//steering angle
			int8_t steering_percentage = calculate_steering_percent((double) adc_get(2));			
			
			//damper travel
			uint16_t federwegFL =  SPRINGTRAVEL_MAX - DAMP_MAX_FL + damper_poti((float)adc_get(4));
			uint16_t federwegFR =  SPRINGTRAVEL_MAX - DAMP_MAX_FR + damper_poti((float)adc_get(5));
			
			
			SensorHub0_databytes[0]	=	0														;	//lsb APPS1
			SensorHub0_databytes[1]	=	0														;	//msb APPS1
			SensorHub0_databytes[2]	=	0														;	//lsb APPS2
			SensorHub0_databytes[3]	=	0														;	//msb APPS2
			SensorHub0_databytes[4]	=	0														;	
			SensorHub0_databytes[5]	=	0														;
			SensorHub0_databytes[6]	=	0														;	//SDC
			SensorHub0_databytes[7]	=	steering_percentage										;	//SA	
			
			//uint16_t testBPS = ADC2Sensor(adc_get(0),0.0,5.0,100,10,5,100);
			SensorHub1_databytes[0]	=	ADC2Sensor(adc_get(0),0.0,5.0,100,10,5,10) & 0xFF		;	//lsb BPS_F
			SensorHub1_databytes[1]	=	ADC2Sensor(adc_get(0),0.0,5.0,100,10,5,10) >> 8			;	//msb BPS_F
			SensorHub1_databytes[2]	=	ADC2Sensor(adc_get(1),0.5,4.5,100,10,5,10) & 0xFF		;	//lsb BPS_R
			SensorHub1_databytes[3]	=	ADC2Sensor(adc_get(1),0.5,4.5,100,10,5,10) >> 8			;	//msb BPS_R
			SensorHub1_databytes[4]	=	0														;	//lsb wheelspeed left	
			SensorHub1_databytes[5]	=	0														;	//msb wheelspeed left
			SensorHub1_databytes[6]	=	0														;	//lsb wheelspeed right
			SensorHub1_databytes[7]	=	0														;	//msb wheelspeed right
			
			SensorHub2_databytes[0]	=	(uint16_t) federwegFL;									;	//lsb DTS_FL
			SensorHub2_databytes[1]	=	((uint16_t) federwegFL)>>8;								;	//msb DTS_FL
			SensorHub2_databytes[2]	=	(uint16_t) federwegFR;									;	//lsb DTS_FR
			SensorHub2_databytes[3]	=	((uint16_t) federwegFR)>>8;								;	//msb DTS_FR
			SensorHub2_databytes[4]	=	0														;	//
			SensorHub2_databytes[5]	=	0														;	//
			SensorHub2_databytes[6]	=	0														;
			SensorHub2_databytes[7]	=	0														;
			
			can_tx(&can_SensorHub0_mob, SensorHub0_databytes);			
			can_tx(&can_SensorHub1_mob, SensorHub1_databytes);
			//can_tx(&can_SensorHub2_mob, SensorHub2_databytes);
			
			sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			
		} // end of 200ms

    }
}

