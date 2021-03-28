/*
 * uFAR.c
 *
 * Created: 04.04.2019 20:52:02
 * Author : Ole Hannemann
 */ 

#include <avr/io.h>
#include "canlib.h"
#include "misc_functions.h"
#include "adc_functions.h"
#include "SPI.h"
#include <avr/interrupt.h>

extern unsigned long sys_time;
volatile unsigned long time_old = 0;
volatile uint8_t sys_time_10 = 0;
volatile uint8_t sys_time_50 = 0;
volatile uint8_t sys_time_500 = 0;

int main(void)
{
	can_cfg();
	sys_timer_config();
	port_config();
	adc_config();
	SPI_config();
	
	// Struct for the CAN Message that contains the sampled Sensor Data

	struct CAN_MOB can_SH_mob1;
	can_SH_mob1.mob_id = 0x600;
	can_SH_mob1.mob_idmask = 0; //We are sending this CAN Message Object (MOB) therefore we do not need an ID MASK
	can_SH_mob1.mob_number = 1;
	uint8_t SH_databytes1[8];
	
	struct CAN_MOB can_SH_mob2;
	can_SH_mob2.mob_id = 0x601;
	can_SH_mob2.mob_idmask = 0; //We are sending this CAN Message Object (MOB) therefore we do not need an ID MASK
	can_SH_mob2.mob_number = 2;
	uint8_t SH_databytes2[8];
	
	sei();
	
    while (1) {
		
		//1ms loop
		if((sys_time - time_old) >= 1){ 
			time_old = sys_time;
			adc_start_conversion();
			sys_time_10++;
			
		//10ms loop
			
			if (sys_time_10 >= 10){  
			
			SH_databytes1[0] = adc_get_1() & 0xff;
			SH_databytes1[1] = adc_get_1() >> 8;
			SH_databytes1[2] = adc_get_2() & 0xff;
			SH_databytes1[3] = adc_get_2() >> 8;
			SH_databytes1[4] = adc_get_3() & 0xff;
			SH_databytes1[5] = adc_get_3() >> 8;
			SH_databytes1[6] = adc_get_4() & 0xff;
			SH_databytes1[7] = adc_get_4() >> 8;

			can_tx(&can_SH_mob1, SH_databytes1); //send the CAN Message		
						
			SH_databytes2[0] = adc_get_5() & 0xff;
			SH_databytes2[1] = adc_get_5() >> 8;
			SH_databytes2[2] = TYPK_getdata1() & 0xff;
			SH_databytes2[3] = TYPK_getdata1() >> 8;
			SH_databytes2[4] = TYPK_getdata2() & 0xff;;
			SH_databytes2[5] = TYPK_getdata2() >> 8;
			SH_databytes2[6] = 52;
			SH_databytes2[7] = 2;
						
			can_tx(&can_SH_mob2, SH_databytes2); //send the CAN Message	
			
			sys_time_10 = 0;
			sys_time_50++;
			}

		}
		//50ms loop
		
		if(sys_time_50 >= 5){
			sys_tick();
			sys_time_50 = 0;
			sys_time_500++;
			}
		//500ms loop	
		if(sys_time_500 >= 20){
			TYPK_read();
			sys_time_500 = 0;
			}
			
		}
	}

