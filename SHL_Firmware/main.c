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


/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;
unsigned long time_100_ms = 0;
int main(void){
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	can_cfg();
	
	struct CAN_MOB can_SHL_mob;
	can_SHL_mob.mob_id = 0x410;
	can_SHL_mob.mob_idmask = 0x0; //send 
	can_SHL_mob.mob_number = 1;
	uint8_t SHL_databytes[8];
	
	
	
	while (1){
		if((sys_tick - time_old) >= 1){ 
			time_old = sys_tick;
			adc_start_conversion();
			time_100_ms++;  

			
		}
		if (time_100_ms <= 100){
		sys_tick_heart();	
			
		SHL_databytes[0] = adc_get(1)		& 0xff	; //lsb BPS1
		SHL_databytes[1] = (adc_get(1)>>8)	& 0xff	; //msb BPS1
		SHL_databytes[2] = adc_get(2)		& 0xff	; //lsb BPS2
		SHL_databytes[3] = (adc_get(2)>>8)	& 0xff	; //msb BPS2
		SHL_databytes[4] = 0; //SPI getter Wheel Speed lsb
		SHL_databytes[5] = 0; //SPI getter Wheel Speed msb
		SHL_databytes[6] = adc_get(3)		& 0xff	; //lsb SA
		SHL_databytes[7] = (adc_get(3)>>8)	& 0xff	; //msb SA
		
		can_tx(&can_SHL_mob, SHL_databytes);
		time_100_ms = 0;
		}
	} //no fault condition
}