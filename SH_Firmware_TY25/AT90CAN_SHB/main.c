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


uint8_t SPI_trash =	0;
uint8_t wheelspeed_left = 0;
uint8_t wheelspeed_right = 0;
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
			
			//get wheelspeed data from ATMEGA16
			
				//wheelspeed_left
				SS_uC_LOW();								// select uC  for SPI-communication
				SPI_trash = SPI_transfer(0x01);				// for 0x01: slave put ws_left in SPDR
				wheelspeed_left = SPI_transfer(0x00);		// Slave send 8bit of wheelspeed_left
				SS_uC_HIGH();								// deselect uC for SPI-communication
			
				//wheelspeed_right
				SS_uC_LOW();								// select uC  for SPI-communication
				SPI_trash = SPI_transfer(0x03);				// for 0x03: slave put ws_right in SPDR
				wheelspeed_right = SPI_transfer(0x00);		// Slave send 8bit of wheelspeed_left
				SS_uC_HIGH();								// deselect uC for SPI-communication
			
					
				//steering angle
						int8_t steering_percentage = calculate_steering_percent((double) adc_get(2));			
			
				//damper travel
					uint16_t federwegFL =  SPRINGTRAVEL_MAX - DAMP_MAX_FL + damper_poti((double)adc_get(4));
					uint16_t federwegFR =  SPRINGTRAVEL_MAX - DAMP_MAX_FR + damper_poti((double)adc_get(3));
			
				//CAN-communication
					//mob_id = 0x400
					SensorHub0_databytes[0]	=	federwegFL												;	// lsb DTS FL
					SensorHub0_databytes[1]	=	federwegFL >> 8											;	// msb DTS FL
					SensorHub0_databytes[2]	=	federwegFR												;	// lsb DTS FR
					SensorHub0_databytes[3]	=	federwegFR >> 8											;	// msb DTS FR
					SensorHub0_databytes[4]	=	0														;	
					SensorHub0_databytes[5]	=	0														;
					SensorHub0_databytes[6]	=	0														;	// SDC
					SensorHub0_databytes[7]	=	steering_percentage										;	// SA	
			
				//uint16_t testBPS = ADC2Sensor(adc_get(0),0.0,5.0,100,10,5,100);
				//SensorHub1_databytes[0]	=	adc_get(0) & 0xFF		;	// lsb BPS_F
				//SensorHub1_databytes[1]	=	adc_get(0) >> 8			;	// msb BPS_F
				
					//mob_id = 0x401
					SensorHub1_databytes[0]	=	ADC2Sensor(adc_get(0),0.0,5.0,100,10,5,10) & 0xFF		;	// lsb BPS_F
					SensorHub1_databytes[1]	=	ADC2Sensor(adc_get(0),0.0,5.0,100,10,5,10) >> 8			;	// msb BPS_F
					SensorHub1_databytes[2]	=	ADC2Sensor(adc_get(1),0.5,4.5,100,10,5,10) & 0xFF		;	// lsb BPS_R
					SensorHub1_databytes[3]	=	ADC2Sensor(adc_get(1),0.5,4.5,100,10,5,10) >> 8			;	// msb BPS_R
					SensorHub1_databytes[4]	=	0														;	
					SensorHub1_databytes[5]	=	wheelspeed_left									;	// wheelspeed left
					SensorHub1_databytes[6]	=	0														;
					SensorHub1_databytes[7]	=	wheelspeed_right							;	// wheelspeed right
			
				//SensorHub2_databytes[0]	=	0														;	// reserved
				//SensorHub2_databytes[1]	=	0														;	// reserved
				//SensorHub2_databytes[2]	=	0														;	// reserved
				//SensorHub2_databytes[3]	=	0														;	// reserved
				//SensorHub2_databytes[4]	=	0														;	// reserved
				//SensorHub2_databytes[5]	=	0														;	// reserved
				//SensorHub2_databytes[6]	=	0														;	// reserved
				//SensorHub2_databytes[7]	=	0														;	// reserved
			
			
			
				can_tx(&can_SensorHub0_mob, SensorHub0_databytes);			
				can_tx(&can_SensorHub1_mob, SensorHub1_databytes);
				//can_tx(&can_SensorHub2_mob, SensorHub2_databytes);										// reserved
			
				sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			
		} // end of 200ms

    }
}

