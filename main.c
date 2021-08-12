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
volatile uint8_t sys_time_200 = 0;
extern volatile TKTF;

uint16_t adc_data_1;
uint16_t adc_data_2;
uint16_t adc_data_3;
uint16_t adc_data_4;

int main(void)
{
	can_cfg();
	sys_timer_config();
	port_config();
	adc_config();
	SPI_config();
	
	// Struct for the CAN Message that contains the sampled Sensor Data

	struct CAN_MOB can_SH_mob1;
	can_SH_mob1.mob_id = 0x503;
	can_SH_mob1.mob_idmask = 0; //We are sending this CAN Message Object (MOB) therefore we do not need an ID MASK
	can_SH_mob1.mob_number = 1;
	uint8_t SH_databytes1[8];
	
	struct CAN_MOB can_SH_mob2;
	can_SH_mob2.mob_id = 0x504;
	can_SH_mob2.mob_idmask = 0; //We are sending this CAN Message Object (MOB) therefore we do not need an ID MASK
	can_SH_mob2.mob_number = 2;
	uint8_t SH_databytes2[8];
		
	struct CAN_MOB can_SH_mob3;
	can_SH_mob3.mob_id = 0x505;
	can_SH_mob3.mob_idmask = 0; //We are sending this CAN Message Object (MOB) therefore we do not need an ID MASK
	can_SH_mob3.mob_number = 3;
	uint8_t SH_databytes3[8];
	
	sei();
	
    while (1) {
		
		//1ms loop 1000Hz
		if((sys_time - time_old) >= 1){
			time_old = sys_time;
			adc_start_conversion();
			sys_time_10++;
			
			//10ms loop 100Hz
			
			if (sys_time_10 >= 10){
				
				SPI_read(); //Starts the SPI Procedure
				
				
				adc_data_1 = adc_get_1();
				adc_data_2 = adc_get_2();
				
				SH_databytes1[0] = ADC2Angle(adc_data_1,110,0,220,358,153,1023) & 0xff; //Steering Angle mid min max
				SH_databytes1[1] = ADC2Angle(adc_data_1,110,0,220,358,153,1023) >> 8;//Steering Angle
				SH_databytes1[2] = ADC2Sensor(adc_data_2,0.5,4.5,100,10,5,10) & 0xff; //Brake Pressure Rear
				SH_databytes1[3] = ADC2Sensor(adc_data_2,0.5,4.5,100,10,5,10) >> 8; //Brake Pressure Rear
				SH_databytes1[4] = Speed_getdata1()	& 0xff;
				SH_databytes1[5] = Speed_getdata1()	>> 8;
				SH_databytes1[6] = Speed_getdata2()	& 0xff;
				SH_databytes1[7] = Speed_getdata2()	>> 8;

				can_tx(&can_SH_mob1, SH_databytes1); //send the CAN Message
				
				sys_time_10 = 0;
				sys_time_50++;
			}
		}
		//50ms loop 20Hz
		
		adc_data_3 = adc_get_3();
		adc_data_4 = adc_get_4();
		
		if(sys_time_50 >= 5){
			sys_time_50 = 0;
			SH_databytes2[0] = ADC2NTCtemp(adc_data_3,3450,10000,5,1024,1500)	& 0xff;	//CLTRR
			SH_databytes2[1] = ADC2NTCtemp(adc_data_3,3450,10000,5,1024,1500)	>> 8;
			SH_databytes2[2] = ADC2NTCtemp(adc_data_4,3450,10000,5,1024,1500)	& 0xff;	//CLTVR
			SH_databytes2[3] = ADC2NTCtemp(adc_data_4,3450,10000,5,1024,1500)	>> 8;
			SH_databytes2[4] = 1;
			SH_databytes2[5] = 2;
			SH_databytes2[6] = 3;
			SH_databytes2[7] = 4;

			can_tx(&can_SH_mob2, SH_databytes2); //send the CAN Message
			
			sys_time_200++;
		}
		//200ms loop 5Hz
		if(sys_time_200 >= 4){
			sys_tick();
			TKTF = 1; //indicate that we want to talk to the TYPK IC
			
			SH_databytes3[0] = TYPK_getdata1()	& 0xff;	//CLTRL
			SH_databytes3[1] = TYPK_getdata1()	>> 8;
			SH_databytes3[2] = TYPK_getdata2()	& 0xff;	//CLTVL
			SH_databytes3[3] = TYPK_getdata2()	>> 8;
			SH_databytes3[4] = 1;
			SH_databytes3[5] = 2;
			SH_databytes3[6] = 3;
			SH_databytes3[7] = 4;
			
			can_tx(&can_SH_mob3, SH_databytes3); //send the CAN Message
			
			sys_time_200 = 0;
		}
			
		}
	}

