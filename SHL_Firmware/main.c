/*
 * SHL_Firmware.c
 *
 * Created: 08.05.2023 20:03:09
 * Author : nico
 */ 

/*	INCLUDES	*/

#include <avr/io.h>
#include "system_config.h"
#include "adc_functions.h"
#include "canlib.h"
#include "sensor_function.h"


/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;

uint8_t time_10_ms = 0;
uint8_t time_50_ms = 0;
uint8_t time_100_ms = 0;
uint8_t time_200_ms = 0; 

uint16_t data = 700;
extern uint16_t pressure;

int main(void){
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	can_cfg();
	
	struct CAN_MOB can_SHL0_mob;
	can_SHL0_mob.mob_id = 0x410;
	can_SHL0_mob.mob_idmask = 0;//sent
	can_SHL0_mob.mob_number = 0;
	uint8_t SHL0_databytes[8] = {0};
	
	sei();
	
	while (1){
		if((sys_tick - time_old) >= 1){ 
			time_old = sys_tick;
			adc_start_conversion();
			time_10_ms++;
			time_50_ms++;
			time_100_ms++;  	
		}
		if(time_10_ms >= 10)
		{
			adc_start_conversion();
			
			time_10_ms = 0;
		}
		if (time_50_ms >= 50)
		{
			can_tx(&can_SHL0_mob, SHL0_databytes);
			
			ADC_brake_Sensor_front(data);
			
		time_50_ms = 0;
		}
		if (time_100_ms >= 100)
		{
		PORTC ^= (1<<PC2);
		SHL0_databytes[0] = (pressure		& 0xff)	; //lsb BPS1
		SHL0_databytes[1] = ((pressure>>8)	& 0xff)	; //msb BPS1
		SHL0_databytes[2] = adc_get(2)		& 0xff	; //lsb BPS2
		SHL0_databytes[3] = (adc_get(2)>>8)	& 0xff	; //msb BPS2
		SHL0_databytes[4] = 0; //SPI getter Wheel Speed lsb
		SHL0_databytes[5] = 0; //SPI getter Wheel Speed msb
		SHL0_databytes[6] = adc_get(3)		& 0xff	; //lsb SA
		SHL0_databytes[7] = (adc_get(3)>>8)	& 0xff	; //msb SA
		
		time_100_ms = 0;
		}
		
	} //no fault condition
}