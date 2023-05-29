/*
 * SHR_Firmware.c
 *
 * Created: 28.04.2023 17:35:31
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

int main(void)
{
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	can_cfg();
	
	struct CAN_MOB can_SHR0_mob;
	can_SHR0_mob.mob_id = 0x400;
	can_SHR0_mob.mob_idmask = 0; //send
	can_SHR0_mob.mob_number = 3;  //IDs might be wrong
	uint8_t SHR0_databytes[8];
	
	
    while (1) 
    {
		if((sys_tick - time_old) >= 1){
			time_old = sys_tick;
			adc_start_conversion();
			time_100_ms++;

			
		}
		if (time_100_ms <= 100){
			PORTC ^= (1<<PC2);
			
			SHR0_databytes[0] = adc_get(1)		& 0xff	; //lsb APPS1
			SHR0_databytes[1] = (adc_get(1)>>8)	& 0xff	; //msb APPS1
			SHR0_databytes[2] = adc_get(2)		& 0xff	; //lsb APPS2
			SHR0_databytes[3] = (adc_get(2)>>8)	& 0xff	; //msb APPS2
			SHR0_databytes[4] = 0; //SPI getter Wheel Speed R lsb
			SHR0_databytes[5] = 0; //SPI getter Wheel Speed R msb
			SHR0_databytes[6] = 0;
			SHR0_databytes[7] = 0;
			
			can_tx(&can_SHR0_mob, SHR0_databytes);
			time_100_ms = 0;
		} //no fault condition
    }
}

