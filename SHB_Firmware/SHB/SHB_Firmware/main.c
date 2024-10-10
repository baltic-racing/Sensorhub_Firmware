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

extern uint16_t adc_values[1];

int main(void)
{
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	can_cfg();
	
	struct CAN_MOB can_SHB0_mob;
	can_SHB0_mob.mob_id = 0x420;
	can_SHB0_mob.mob_idmask = 0; //receive with no filter?
	can_SHB0_mob.mob_number = 1;
	uint8_t SHB0_databytes[8];
	
	struct CAN_MOB can_SHB1_mob;
	can_SHB1_mob.mob_id = 0x421;
	can_SHB1_mob.mob_idmask = 0; //receive with no filter?
	can_SHB1_mob.mob_number = 2;
	uint8_t SHB1_databytes[8];
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
			
			PORTC ^= (1<<PC0); // fault just for fun

			uint16_t temp1 =  temp_calc((float)adc_get(1));
			uint16_t temp2 =  temp_calc((float)adc_get(2));
			uint16_t temp3 =  temp_calc((float)adc_get(3));
			uint16_t temp4 =  temp_calc((float)adc_get(4));
			
			SHB0_databytes[0] = temp1;
			SHB0_databytes[1] = 0;
			SHB0_databytes[2] = temp2; 
			SHB0_databytes[3] = 0;
			SHB0_databytes[4] = temp3;
			SHB0_databytes[5] = 0;
			SHB0_databytes[6] = temp4;
			SHB0_databytes[7] = 0;
			
			
			uint16_t federwegRL =  damper_poti((float)adc_get(5));
			uint16_t federwegRR =  damper_poti((float)adc_get(6));
			
			SHB1_databytes[0] = (uint16_t) federwegRL; 
			SHB1_databytes[1] = ((uint16_t) federwegRL)>>8; //DPRL
			SHB1_databytes[2] = (uint16_t) federwegRR;
			SHB1_databytes[3] = ((uint16_t) federwegRR)>>8; //DPRR;
			SHB1_databytes[4] = 0;
			SHB1_databytes[5] = 0;
			SHB1_databytes[6] = 0;
			SHB1_databytes[7] = 0;
			
			can_tx(&can_SHB0_mob, SHB0_databytes); // CAN_10Hz
			can_tx(&can_SHB1_mob, SHB1_databytes);
			
			time_old_10ms = 0;
		}
		
		if (time_old_100ms >= 100)
		{
			PORTC ^= (1<<PC2);
			time_old_100ms = 0;
		}
    }
	}