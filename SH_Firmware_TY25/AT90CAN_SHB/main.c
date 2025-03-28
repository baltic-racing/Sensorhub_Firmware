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

#include "main.h"

uint8_t switchi = 1;

volatile uint16_t wheelspeed[2];

extern uint8_t SensorHub0_databytes[8];
extern uint8_t SensorHub1_databytes[8];

extern struct CAN_MOB can_SensorHub0_mob;
extern struct CAN_MOB can_SensorHub1_mob;

int main(void)
{
	port_config();
    sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	SPI_MasterInit();
	can_cfg();
	CAN_Init_Messages();
	
	struct CAN_MOB can_SensorHub0_mob;
	can_SensorHub0_mob.mob_id = 0x410;
	can_SensorHub0_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHub0_mob.mob_number = 0;
	uint8_t SensorHub0_databytes[8] = {0};
		
	struct CAN_MOB can_SensorHub1_mob;
	can_SensorHub1_mob.mob_id = 0x411;
	can_SensorHub1_mob.mob_idmask = 0xFFFF;//sent
	can_SensorHub1_mob.mob_number = 1;
	uint8_t SensorHub1_databytes[8] = {0};
	
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
			
			uint16_t federwegFL =  SPRINGTRAVEL_MAX - DAMP_MAX_FL + damper_poti((float)adc_get(2));
			uint16_t federwegFR =  SPRINGTRAVEL_MAX - DAMP_MAX_FR + damper_poti((float)adc_get(3));
			
			SensorHub0_databytes[0]	=	0														;
			SensorHub0_databytes[1]	=	0														;
			SensorHub0_databytes[2]	=	0														;
			SensorHub0_databytes[3]	=	0														;
			SensorHub0_databytes[4]	=	0														;
			SensorHub0_databytes[5]	=	0														;
			SensorHub0_databytes[6]	=	0														;
			SensorHub0_databytes[7]	=	0														;
			
			
			
			uint16_t testBPS = ADC2Sensor(adc_get(0),0.0,5.0,100,10,5,100);
			SensorHub1_databytes[0]	=	ADC2Sensor(adc_get(0),0.5,4.5,100,10,5,10) & 0xff		;
			SensorHub1_databytes[1]	=	ADC2Sensor(adc_get(0),0.5,4.5,100,10,5,10) >>8			;
			SensorHub1_databytes[2]	=	ADC2Sensor(adc_get(1),0.5,4.5,100,10,5,10) & 0xff		;
			SensorHub1_databytes[3]	=	ADC2Sensor(adc_get(1),0.5,4.5,100,10,5,10) >>8			;
			SensorHub1_databytes[4]	=	wheelspeed[0]&0xff										;	//wheelspeed[0]&0xff	
			SensorHub1_databytes[5]	=	(wheelspeed[1]>>8)&0xff									;	//(wheelspeed[1]>>8)&0xff
			SensorHub1_databytes[6]	=	0														;
			SensorHub1_databytes[7]	=	0														;
			
			can_tx(&can_SensorHub0_mob, SensorHub0_databytes);			
			can_tx(&can_SensorHub1_mob, SensorHub1_databytes);
			
			sys_tick_heart();
			
		} // end of 100ms
		
		if (TIME_PASSED_200_MS)
		{
			time_200ms = sys_time;
			
		} // end of 200ms

    }
}

