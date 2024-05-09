/*
 * main.c
 *
 * Created: 5/9/2024 3:58:38 PM
 *  Author: aaron
 */ 

#include <avr/io.h>
#include "system_config.h"
#include "canlib.h"
#include "adc_functions.h"

/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;
unsigned long time_10_ms =0;
unsigned long time_100_ms =0;

int main(void){
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	can_cfg();
	adc_config();
	
	struct CAN_MOB can_SHB0_mob;
	can_SHB0_mob.mob_id = 0x420;
	can_SHB0_mob.mob_idmask = 0; //receive with no filter?
	can_SHB0_mob.mob_number = 0;
	uint8_t SHB0_databytes[8];
	
	sei();
	
	while (1){
		if((sys_tick - time_old) >= 1){
			time_old = sys_tick;
			adc_start_conversion();
			time_10_ms++;  //10 ms reference
			time_100_ms++; //100ms refence
			
		}
		if (time_10_ms >= 10){
			
			SHB0_databytes[0] = adc_get(1)		; //lsb
			SHB0_databytes[1] = (adc_get(1)>>8)	; //msb
			SHB0_databytes[2] = adc_get(2)		; //lsb
			SHB0_databytes[3] = (adc_get(2)>>8)	; //msb
			SHB0_databytes[4] = adc_get(2)		; //lsb
			SHB0_databytes[5] = (adc_get(2)>>8)	; //msb
			SHB0_databytes[6] = adc_get(3)		; //lsb
			SHB0_databytes[7] = (adc_get(3)>>8)	; //msb
			
			can_tx(&can_SHB0_mob, SHB0_databytes);
			time_10_ms = 0;
		}
	 if(time_100_ms >= 100){
		 PORTC ^= (1<<PC2); // heart LED
		 time_100_ms=0;
	 }
	 
	 
		
		
	}
}//no fault condition!