/*
 * SHR_Firmware.c
 *
 * Created: 28.04.2023 17:35:31
 * Author : nico
 */ 

/*	INCLUDES	*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "system_config.h"
#include "adc_functions.h"
#include "canlib.h"
#include "sensor_functions.h"

/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;
unsigned long time_old_10ms = 0;
unsigned long time_old_100ms = 0;

/*	GLobal Variables	*/

float apps1_percentage;
float apps2_percentage;

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
	can_SHR0_mob.mob_number = 0;  //IDs might be wrong
	uint8_t SHR0_databytes[8] = {0};
	
	sei();
	
	
    while (1) 
    {
		if ((sys_tick - time_old) >= 1)
		{
			time_old = sys_tick;
			adc_start_conversion();
			time_old_10ms++;
			time_old_100ms++;
		}
		
		if (time_old_10ms >= 10)
		{	
			//float adc_shift_1 = adc_get(0);
			//float adc_shift_2 = adc_get(1);
			
			uint16_t apps1_percentage = update_apps_percentage((float)/*adc_shift_1*/adc_get(0), 1);
			uint16_t apps2_percentage = update_apps_percentage((float)/*adc_shift_2*/adc_get(1), 2);
			
			SHR0_databytes[0] =  (uint16_t)apps1_percentage;			//lsb APPS1
			SHR0_databytes[1] = ((uint16_t)apps1_percentage >> 8);		//msb APPS1
			SHR0_databytes[2] =  (uint16_t)apps2_percentage;			//lsb APPS2
			SHR0_databytes[3] = ((uint16_t)apps2_percentage >> 8);		//msb APPS2
			SHR0_databytes[4] = 0; //SPI getter Wheel Speed R lsb
			SHR0_databytes[5] = 0; //SPI getter Wheel Speed R msb
			SHR0_databytes[6] = 0;
			SHR0_databytes[7] = 0;
			
			can_tx(&can_SHR0_mob, SHR0_databytes);
			time_old_10ms = 0;
		}
		
		if (time_old_100ms >= 100)
		{
			PORTC ^= (1<<PC2);
			time_old_100ms = 0;
		}
    }
}