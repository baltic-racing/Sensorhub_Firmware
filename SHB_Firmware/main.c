/*
 * SHB_Firmware.c
 *
 * Created: 08.05.2023 20:00:36
 * Author : nico
 */ 

/*	INCLUDES	*/

#include <avr/io.h>
#include "system_config.h"
#include "canlib.h"
#include "adc_functions.h"

/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;
unsigned long time_20_ms =0;

int main(void){
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	can_cfg();
	adc_config();							//CAN lib may be outdated, small differences between the old can lib but i already used it on FB to communicate between the old SWC during initial testing 
								//for message objects, the sensor only SENDS data!
	struct CAN_MOB can_SHB0_mob;
	can_SHB0_mob.mob_id = 0x420;
	can_SHB0_mob.mob_idmask = 0; //receive with no filter?
	can_SHB0_mob.mob_number = 5;
	uint8_t SHB0_databytes[8];
	
	
	sei();
	
	while (1){
		if((sys_tick - time_old) >= 1){
			time_old = sys_tick;
			adc_start_conversion();   
			time_20_ms++;  //20 ms reference

			
		}
		if (time_20_ms >= 20){ 
			
			SHB0_databytes[0] = adc_get(1)		& 0xff	; //lsb 
			SHB0_databytes[1] = (adc_get(1)>>8)	& 0xff	; //msb
			SHB0_databytes[2] = adc_get(2)		& 0xff	; //lsb 
			SHB0_databytes[3] = (adc_get(2)>>8)	& 0xff	; //msb
			SHB0_databytes[4] = adc_get(2)		& 0xff	; //lsb 
			SHB0_databytes[5] = (adc_get(2)>>8)	& 0xff	; //msb
			SHB0_databytes[6] = adc_get(3)		& 0xff	; //lsb 
			SHB0_databytes[7] = (adc_get(3)>>8)	& 0xff	; //msb
			
			can_tx(&can_SHB0_mob, SHB0_databytes);
			time_20_ms = 0;
		}
			
	
		
	}
}//no fault condition!