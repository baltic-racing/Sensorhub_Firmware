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
#include "sensor_function.h"

/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;
unsigned long time_old_10ms = 0;
unsigned long time_old_100ms = 0;

/*	GLobal Variables	*/

extern uint16_t adc_values[4];
extern uint16_t pressure;
uint16_t adc_data_1;
uint16_t adc_data_2;
uint16_t adc_data_3;

int main(void)
{
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	can_cfg();
	
	struct CAN_MOB can_SHL0_mob;
	can_SHL0_mob.mob_id = 0x410;
	can_SHL0_mob.mob_idmask = 0;//sent
	can_SHL0_mob.mob_number = 0;
	uint8_t SHL0_databytes[8] = {0};
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
			adc_start_conversion();
			
			
			
			time_old_10ms = 0;
		}
		
		if (time_old_100ms >= 100)
		{
			uint16_t data = 0;
			adc_data_1 = adc_get(1);
			adc_data_2 = adc_get(2);
			adc_data_3 = adc_get(3);
			
			SHL0_databytes[0] = ADC2Sensor(adc_data_3,0.5,4.5,100,10,5,10) & 0xff; //lsb BPS1ADC2Sensor(adc_data_1,1,4.5,100,10,5,10) & 0xff; //lsb BPS1
			SHL0_databytes[1] = ADC2Sensor(adc_data_3,0.5,4.5,100,10,5,10) >>8; //lsb BPS1ADC2Sensor(adc_data_1,1,4.5,100,10,5,10) >>8;//msb BPS1
			SHL0_databytes[2] = ADC2Sensor(adc_data_2,0.5,4.5,100,10,5,10) & 0xff; //lsb BPS1
			SHL0_databytes[3] = ADC2Sensor(adc_data_2,0.5,4.5,100,10,5,10) >>8; //msb BPS1
			SHL0_databytes[4] = 0; //SPI getter Wheel Speed lsb
			SHL0_databytes[5] = 0; //SPI getter Wheel Speed msb
			SHL0_databytes[6] = 0;//adc_get(3)		& 0xff	; //lsb SA
			SHL0_databytes[7] = 0;//(adc_get(3)>>8)	& 0xff	; //msb SA
			
			can_tx(&can_SHL0_mob, SHL0_databytes);
			PORTC ^= (1<<PC2);
			time_old_100ms = 0;
		}
    }
	}