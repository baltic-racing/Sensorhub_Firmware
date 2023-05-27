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
unsigned long time_10_ms =0;

int main(void){
	/*	Configuration	*/
	port_config();
	sys_timer_config();
								//CAN lib may be outdated, small differences between the old can lib but i already used it on FB to communicate between the old SWC during initial testing 
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
			time_10_ms++;  //10 ms reference
			//time_x_ms++; 
			//time_x_ms++;
			//time_x_ms++;
			// ...
		}
		if (time_10_ms <= 10){
			//spi_read(); for later
			SHB_databytes[0] = 0;/*get CLTF1 msb*/
			SHB_databytes[1] = 0;/*get CLTF1 lsb*/
			SHB_databytes[2] = 0;/*get CLTF2 msb*/
			SHB_databytes[3] = 0;/*get CLTF2 lsb*/
			SHB_databytes[4] = 0;/*get CLTR1 msb*/
			SHB_databytes[5] = 0;/*get CLTR1 lsb*/
			SHB_databytes[6] = 0;/*get CLTR2 msb*/
			SHB_databytes[7] = 0;/*get CLTR2 lsb*/
			
			can_tx(&can_SHB_mob, SHB_databytes);
		}
			
			
		
	}
}