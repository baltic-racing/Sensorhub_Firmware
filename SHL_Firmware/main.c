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

#include "main.h"

/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;
unsigned long time_old_10ms = 0;
unsigned long time_old_100ms = 0;
unsigned long time_old_1000ms = 0;

/*	GLobal Variables	*/

volatile uint16_t wheelspeed[2];

uint8_t switchi = 1;								// switch between the two Bytes of SPI-communication
uint8_t i = 1;

int main(void)
{
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	can_cfg();
	SPI_MasterInit();
	
	struct CAN_MOB can_SHL0_mob;
	can_SHL0_mob.mob_id = 0x410;
	can_SHL0_mob.mob_idmask = 0xFFFF;//sent
	can_SHL0_mob.mob_number = 0;
	uint8_t SHL0_databytes[8] = {0};
		
	struct CAN_MOB can_SHL1_mob;
	can_SHL1_mob.mob_id = 0x411;
	can_SHL1_mob.mob_idmask = 0xFFFF;//sent
	can_SHL1_mob.mob_number = 1;
	uint8_t SHL1_databytes[8] = {0};
	
	sei();
	
	
     while (1)
     {
	     if(TIME_PASSED_1_MS)
	     {
			time_old_10ms++;
			time_old_100ms++;
			time_old_1000ms++;
			adc_start_conversion();
			
			time_1ms = sys_time;
		} // end of 1ms
		
		if(TIME_PASSED_10_MS)
			{				
			if(switchi == 1){
				PORTE &= ~(1<<SS_uC);
				SPDR = 0x22;										// Write the Register will start the conversation
				//while(!(SPSR & (1<<SPIF)));
				//PORTE |= (1<<SS_uC);
				//wheelspeed [0] = SPI_Data_Reg;
				
				//switchi = 0;
				}else{
				//PORTE &= ~(1<<SS_uC);
				//SPDR = 0x33;										// Write the Register will start the conversation
				//while(!(SPSR & (1<<SPIF)));
				//PORTE |= (1<<SS_uC);
				//wheelspeed [1] = SPI_Data_Reg;
				
				switchi = 1;
			}
			time_10ms = sys_time;
				
		} // end of 10ms

		if (TIME_PASSED_100_MS)
		{
			SHL0_databytes[0] = ADC2Sensor(adc_get(1),0.5,4.5,100,10,5,10) & 0xff;		//lsb BPS1ADC2Sensor(adc_data_1,1,4.5,100,10,5,10) & 0xff; //lsb BPS1
			SHL0_databytes[1] = ADC2Sensor(adc_get(1),0.5,4.5,100,10,5,10) >>8;			//lsb BPS1ADC2Sensor(adc_data_1,1,4.5,100,10,5,10) >>8;//msb BPS1
			SHL0_databytes[2] = ADC2Sensor(adc_get(0),0.5,4.5,100,10,5,10) & 0xff;		//lsb BPS1
			SHL0_databytes[3] = ADC2Sensor(adc_get(0),0.5,4.5,100,10,5,10) >>8;			//msb BPS1
			SHL0_databytes[4] = wheelspeed[0] & 0xff;									//SPI getter Wheel Speed lsb
			SHL0_databytes[5] = (wheelspeed[1]) >> 8;									//SPI getter Wheel Speed msb
			SHL0_databytes[6] = 0;														//adc_get(3)		& 0xff	; //lsb SA
			SHL0_databytes[7] = 0;														//(adc_get(3)>>8)	& 0xff	; //msb SA
			
			uint16_t federwegFL =  damper_poti((float)adc_get(2));
			uint16_t federwegFR =  damper_poti((float)adc_get(3));
			
			SHL1_databytes[0] = (uint16_t) federwegFL;
			SHL1_databytes[1] = ((uint16_t) federwegFL)>>8;								//DPRL
			SHL1_databytes[2] = (uint16_t) federwegFR;
			SHL1_databytes[3] = ((uint16_t) federwegFR)>>8;								//DPRR;
			SHL1_databytes[4] = 0;
			SHL1_databytes[5] = 0;
			SHL1_databytes[6] = 0;
			SHL1_databytes[7] = 0;
			
			can_tx(&can_SHL0_mob, SHL0_databytes);
			can_tx(&can_SHL1_mob, SHL1_databytes);
			
			if(i == 1){
				PORTC ^= (1<<PC2);
				i = 0;
			}else{
				PORTC ^= (1<<PC0);														// foult just for fun
				i = 1;
			}
			
			time_100ms = sys_time;
		} // end of 100ms
	
		if (TIME_PASSED_200_MS)
		{
			
			time_200ms = sys_time;
		} // end of 200ms
    }
}