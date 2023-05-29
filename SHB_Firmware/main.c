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
	struct CAN_MOB can_SHB_mob;
	can_SHB_mob.mob_id = 0x420;
	can_SHB_mob.mob_idmask = 0; //send
	can_SHB_mob.mob_number = 0; //only 1 object here, do i need to adjust numbers across all software connected with CAN?
	uint8_t SHB_databytes[8];
	
	sei();
	
	while (1){
		if((sys_tick - time_old) >= 1){ //1 ms loop, hopefully makes implementing more than 2 frequencies easier => probably no need, different main for each SHx
			time_old = sys_tick;
			adc_start_conversion();   
			time_20_ms++;  //20 ms reference
			//time_x_ms++; 
			//time_x_ms++;
			//time_x_ms++;
			// ...
			
		}
		if (time_20_ms <= 10){  //correct to 10 ms later in naming
			//spi_read(); no need for SPI communication on this one! (no chips to communicate with)
			SHB_databytes[0] = adc_get(1)		& 0xff	; //lsb 
			SHB_databytes[1] = (adc_get(1)>>8)	& 0xff	; //msb
			SHB_databytes[2] = adc_get(2)		& 0xff	; //lsb 
			SHB_databytes[3] = (adc_get(2)>>8)	& 0xff	; //msb
			SHB_databytes[4] = adc_get(2)		& 0xff	; //lsb 
			SHB_databytes[5] = (adc_get(2)>>8)	& 0xff	; //msb
			SHB_databytes[6] = adc_get(3)		& 0xff	; //lsb 
			SHB_databytes[7] = (adc_get(3)>>8)	& 0xff	; //msb
			
			can_tx(&can_SHB_mob, SHB_databytes);
			time_20_ms = 0;
		}
			
	//5hz loop for the thermoelement uC	(no spi here though so no 5hz loop)
		
	}
}