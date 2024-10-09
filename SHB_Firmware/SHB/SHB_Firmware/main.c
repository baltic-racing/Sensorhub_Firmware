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
#include "temp_functions.h"

/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;

uint8_t time_10_ms =0;
uint8_t time_100_ms =0;
uint8_t time_200_ms =0;

extern uint8_t temp_counter;

int main(void){
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	can_cfg();
	adc_config();
	temp_switch();
	
	struct CAN_MOB can_SHB0_mob;
	can_SHB0_mob.mob_id = 0x420;
	can_SHB0_mob.mob_idmask = 0; //receive with no filter?
	can_SHB0_mob.mob_number = 0;
	uint8_t SHB0_databytes[8];
	
	sei();
	
	while (1){
		if((sys_tick - time_old) >= 1){
			time_old = sys_tick;
			time_10_ms++;  //10 ms reference
			time_100_ms++;
			time_200_ms++;
			
		}
		if (time_10_ms >= 10){
			adc_start_conversion();
			
			time_10_ms = 0;
		}
		if(time_100_ms >= 100){
			
			temp_switch(temp_counter);
			
			SHB0_databytes[0] = temp_rdy(1)		; //lsb
			SHB0_databytes[1] = (temp_rdy(1))		; //lsb
			SHB0_databytes[3] = (temp_rdy(2)>>8)	; //msb
			SHB0_databytes[4] = temp_rdy(2)		; //lsb
			SHB0_databytes[5] = (temp_rdy(2)>>8)	; //msb
			SHB0_databytes[6] = temp_rdy(3)		; //lsb
			SHB0_databytes[7] = (temp_rdy(3)>>8)	; //msb
			
			can_tx(&can_SHB0_mob, SHB0_databytes); // CAN_10Hz
			
			PORTC ^= (1<<PC2); // heart LED
			
			time_100_ms=0;
		}
		
		if(time_200_ms >= 200){
			
			time_200_ms=0;
		}
	}
}//no fault condition!