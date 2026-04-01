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

// define variables
	//wheelspeed_variables
	uint8_t wheelspeed_left = 0;
	uint8_t wheelspeed_right = 0;
	
	//Temperature_variables
	uint16_t TK1_temp = 0;
	uint16_t TK2_temp = 0;
	uint16_t TK3_temp = 0;
	uint16_t TK4_temp = 0;
	
	//Acceleration_Pedal_Position_Sensor-variables
	double APPS1 = 0;
	double APPS2 = 0;

//define CAN
	extern uint8_t SensorHub0_databytes[8];
	extern uint8_t SensorHub1_databytes[8];
	extern uint8_t SensorHub2_databytes[8];

	extern struct CAN_MOB can_SensorHub0_mob;
	extern struct CAN_MOB can_SensorHub1_mob;
	extern struct CAN_MOB can_SensorHub2_mob;


int main(void)
{
	port_config();				//set input and output Pins
    sys_timer_config();			//set timer (Prescaler for Clock from quarz etc.)
	adc_config();
	can_cfg();
	CAN_Init_Messages();
	SPI_MasterInit();
	
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
				
				//get wheelspeed data from ATMEGA16
						
					wheelspeed_left = getwheelspeed(0x01);
					wheelspeed_right = getwheelspeed(0x03);
						
				//CAN-communication	
					//mob_id = 0x402
					SensorHub2_databytes[0]	=	0														;	// reserved
					SensorHub2_databytes[1]	=	0														;	// reserved
					SensorHub2_databytes[2]	=	0														;	// reserved
					SensorHub2_databytes[3]	=	0														;	// reserved
					SensorHub2_databytes[4]	=	0														;	// reserved (wheelspeed 16bit)
					SensorHub2_databytes[5]	=	wheelspeed_left											;	// wheelspeed left
					SensorHub2_databytes[6]	=	0														;	// reserved (wheelspeed 16bit)
					SensorHub2_databytes[7]	=	wheelspeed_right										;	// wheelspeed right
				
					//transmit can_mob2 
					can_tx(&can_SensorHub2_mob, SensorHub2_databytes);
			
		} // end of 10ms


		if (TIME_PASSED_100_MS)
		{
			time_100ms = sys_time;
			
				//get adc-values
					uint16_t adc_BPS_front		=	adc_get(0)	;
					uint16_t adc_BPS_rear		=	adc_get(1)	;
					
					uint16_t adc_SA				=	adc_get(2)	;
					
					uint16_t adc_damperFR		=	adc_get(3)	;
					uint16_t adc_damperFL		=	adc_get(4)	;
					
				//Brake pressure
					uint16_t BP_front			=	calculate_BP(adc_BPS_front);
					uint16_t BP_rear			=	calculate_BP(adc_BPS_rear);
				
				//steering angle
					int8_t steering_percentage	=	calculate_steering_percent((double) adc_SA);			
			
				//damper travel
					uint16_t federwegFR			=	SPRINGTRAVEL_MAX - DAMP_MAX_FR + damper_poti((double)adc_damperFR);
					uint16_t federwegFL			=	SPRINGTRAVEL_MAX - DAMP_MAX_FL + damper_poti((double)adc_damperFL);		
					
				//get wheelspeed data from ATMEGA16	
				//wheelspeed
						//SS_uC_LOW();											// select uC  for SPI-communication
						//SPI_trash = SPI_transfer(0x01);							// for 0x03: slave put ws_right in SPDR
						//uint8_t wheelspeed_left = SPI_transfer(0x00);			// Slave send 8bit of wheelspeed_left
						//SS_uC_HIGH();											// deselect uC for SPI-communication
//
						//SS_uC_LOW();											// select uC  for SPI-communication
						//SPI_trash = SPI_transfer(0x03);							// for 0x03: slave put ws_right in SPDR
						//uint8_t wheelspeed_right = SPI_transfer(0x00);			// Slave send 8bit of wheelspeed_left
						//SS_uC_HIGH();											// deselect uC for SPI-communication				
			
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

					//mob_id = 0x401
					SensorHub1_databytes[0]	=	BP_front	& 0xFF										;	// lsb BPS_F
					SensorHub1_databytes[1]	=	BP_front	>>8											;	// msb BPS_F
					SensorHub1_databytes[2]	=	BP_rear		& 0xFF										;	// lsb BPS_R
					SensorHub1_databytes[3]	=	BP_rear		>>8											;	// msb BPS_R
					SensorHub1_databytes[4]	=	0										;	// reserved (wheelspeed 16bit)
					SensorHub1_databytes[5]	=	0														;	// wheelspeed left
					SensorHub1_databytes[6]	=	0										;	// reserved (wheelspeed 16bit)
					SensorHub1_databytes[7]	=	0														;	// wheelspeed right											
					
					//transmit CAN-messages
					can_tx(&can_SensorHub0_mob, SensorHub0_databytes);			
					can_tx(&can_SensorHub1_mob, SensorHub1_databytes);
					
					//toggle heart LED			
					sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
		} // end of 200ms

    }
}

