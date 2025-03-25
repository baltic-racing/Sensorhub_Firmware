/*///*
 //* SHR_Firmware.c
 //*
 //* Created: 28.04.2023 17:35:31
 //* Author : nico
 // 
//
///*	INCLUDES	
//
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "system_config.h"
//#include "adc_functions.h"
//#include "canlib.h"
//#include "sensor_functions.h"
//
///*	Init Global Variables	
//unsigned long sys_tick = 0;
//unsigned long time_old = 0;
//unsigned long time_old_10ms = 0;
//unsigned long time_old_100ms = 0;
//
///*	GLobal Variables	
//
//float apps1_percentage;
//float apps2_percentage;
//uint8_t steering_sign = 0;		//indicator for steering percentage
//uint8_t sdc_open = 0;
//extern uint16_t adc_values[3];
//
//
//int main(void)
//{
	///*	Configuration	
	//port_config();
	//sys_timer_config();
	//adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	//can_cfg();
	//
	//struct CAN_MOB can_SHR0_mob;
	//can_SHR0_mob.mob_id = 0x400;
	//can_SHR0_mob.mob_idmask = 0; //send
	//can_SHR0_mob.mob_number = 0;  //IDs might be wrong
	//uint8_t SHR0_databytes[8] = {0};
	//
	//sei();
	//
	//
    //while (1) 
    //{
		//if ((sys_tick - time_old) >= 1)
		//{
			//time_old = sys_tick;
			//adc_start_conversion();
			//time_old_10ms++;
			//time_old_100ms++;
		//}
		//
		//if (time_old_10ms >= 10)
		//{	
			//
			//uint16_t apps1_percentage = update_apps_percentage_alt((double) adc_get(0), 1);
			//uint16_t apps2_percentage = update_apps_percentage_alt((double) adc_get(1), 2);
			//
			//uint16_t steering_percentage = calculate_steering_percent((double) adc_get(2));
			//
				//if(adc_values[0] <= 80|| adc_values[1] <= 80){
					////if APPS shorted to ground or open circuit
					//sdc_open=1;
				//}
				//if (apps1_percentage > apps2_percentage+10.0){
					//sdc_open=1; //deviation between apps +10%
				//}
				//if (apps1_percentage < apps2_percentage-10.0){
					//sdc_open=1;//deviation between apps -10%
				//}
				//
				//if (adc_values[2] <= POT_MID){		//left from middle position
					//steering_sign = 0x80;
				//}
				//if (adc_values[2] > POT_MID){		//right from middle position
					//steering_sign = 0x00;
				//}
			//
			//
			//
			//SHR0_databytes[0] =  (uint16_t)apps1_percentage;			//lsb APPS1
			//SHR0_databytes[1] = ((uint16_t)apps1_percentage >> 8);		//msb APPS1
			//SHR0_databytes[2] =  (uint16_t)apps2_percentage;			//lsb APPS2
			//SHR0_databytes[3] = ((uint16_t)apps2_percentage >> 8);		//msb APPS2
			//SHR0_databytes[4] =  steering_sign | (steering_percentage);	//sign indicator for steering percentage | steering percentage
			//SHR0_databytes[5] = 0;
			//SHR0_databytes[6] = sdc_open;
			//SHR0_databytes[7] = 0;
			//
			//can_tx(&can_SHR0_mob, SHR0_databytes);
			//time_old_10ms = 0;
		//}
		//
		//if (time_old_100ms >= 100)
		//{
			//PORTC ^= (1<<PC2);
			//time_old_100ms = 0;
		//}
    //}
//}*/

/*
 * SHR_Firmware.c
 *
 * Created: 28.04.2023 17:35:31
 * Author : nico
 */ 

/*	INCLUDES	*/

#include "main.h"

/*	Init Global Variables	*/
unsigned long sys_tick = 0;
unsigned long time_old = 0;
unsigned long time_old_10ms = 0;
unsigned long time_old_100ms = 0;
unsigned long time_old_1000ms = 0;

/*	GLobal Variables	*/

extern uint16_t adc_values[3];

float apps1_percentage;
float apps2_percentage;
uint8_t steering_sign = 0;		//indicator for steering percentage
uint8_t sdc_open = 0;

volatile uint16_t wheelspeed[2];

uint8_t i = 1;
uint8_t switchi = 1;

int main(void)
{
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	adc_config(); //CHECK WHICH ADC INPUTS CORRESPOND WITH WHAT DATABYTE WE SEND
	can_cfg();
	SPI_MasterInit();
	
	struct CAN_MOB can_SHR0_mob;
	can_SHR0_mob.mob_id = 0x400;
	can_SHR0_mob.mob_idmask = 0xFFFF;//sent
	can_SHR0_mob.mob_number = 0;
	uint8_t SHR0_databytes[8] = {0};
		
	struct CAN_MOB can_SHR1_mob;
	can_SHR1_mob.mob_id = 0x401;
	can_SHR1_mob.mob_idmask = 0xFFFF;//sent
	can_SHR1_mob.mob_number = 1;
	uint8_t SHR1_databytes[8] = {0};
	
	sei();
	
	
    while (1) 
    {
		if ((sys_tick - time_old) >= 1)
		{
			time_old = sys_tick;
			time_old_10ms++;
			time_old_100ms++;
			time_old_1000ms++;
			
			adc_start_conversion();
		}
		
		if (time_old_10ms >= 10)
		{
			if(switchi == 1){
				
				PORTE &= ~(1<<SS_uC);
				SPDR = 0x22;										// Write the Register will start the conversation
				//while(!(SPSR & (1<<SPIF)));
				//PORTE |= (1<<SS_uC);
				//wheelspeed [0] = SPI_Data_Reg;
				
				switchi = 0;
				}else{
				//PORTE &= ~(1<<SS_uC);
				//SPDR = 0x33;										// Write the Register will start the conversation
				//while(!(SPSR & (1<<SPIF)));
				//PORTE |= (1<<SS_uC);
				//wheelspeed [1] = SPI_Data_Reg;
				
				switchi = 1;
			}
			
			time_old_10ms = 0;
		}
		
		if (time_old_100ms >= 100)
		{
			
			uint16_t apps1_percentage = update_apps_percentage_alt((double) adc_get(0), 1);
			uint16_t apps2_percentage = update_apps_percentage_alt((double) adc_get(1), 2);
			
			uint16_t steering_percentage = calculate_steering_percent((double) adc_get(2));
			
			if(adc_values[0] <= 80|| adc_values[1] <= 80){
				//if APPS shorted to ground or open circuit
				sdc_open=1;
			}
			if (apps1_percentage > apps2_percentage+10.0){
				sdc_open=1; //deviation between apps +10%
			}
			if (apps1_percentage < apps2_percentage-10.0){
				sdc_open=1;//deviation between apps -10%
			}
			
			if (adc_values[2] <= POT_MID){		//left from middle position
				steering_sign = 0x80;
			}
			if (adc_values[2] > POT_MID){		//right from middle position
				steering_sign = 0x00;
			}
			
			
			
			SHR0_databytes[0] =  (uint16_t)apps1_percentage;			//lsb APPS1
			SHR0_databytes[1] = 0;		//msb APPS1
			SHR0_databytes[2] =  (uint16_t)apps2_percentage;			//lsb APPS2
			SHR0_databytes[3] = 0;		//msb APPS2
			SHR0_databytes[4] =  steering_sign | (steering_percentage);	//sign indicator for steering percentage | steering percentage
			SHR0_databytes[5] = 0;
			SHR0_databytes[6] = sdc_open;
			SHR0_databytes[7] = 0;
			
			uint16_t federwegFL =  damper_poti((float)adc_get(2));
			uint16_t federwegFR =  damper_poti((float)adc_get(3));
			
			SHR1_databytes[0] = (uint16_t) federwegFL;
			SHR1_databytes[1] = ((uint16_t) federwegFL)>>8; //DPRL
			SHR1_databytes[2] = (uint16_t) federwegFR;
			SHR1_databytes[3] = ((uint16_t) federwegFR)>>8; //DPRR;
			SHR1_databytes[4] = wheelspeed[0] & 0xff;									//SPI getter Wheel Speed lsb
			SHR1_databytes[5] = wheelspeed[1] >> 8;										//SPI getter Wheel Speed msb
			SHR1_databytes[6] = 0;
			SHR1_databytes[7] = 0;
			
			can_tx(&can_SHR0_mob, SHR0_databytes);
			can_tx(&can_SHR1_mob, SHR1_databytes);
			
			
			if(i == 1){
				PORTC ^= (1<<PC2);
				i = 0;
				}else{
				PORTC ^= (1<<PC0); // fault just for fun
				i = 1;
			}
			
			time_old_100ms = 0;
		}
    }
}