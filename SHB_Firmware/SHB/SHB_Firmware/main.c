/*
 * main.c
 *
 * Created: 5/9/2024 3:58:38 PM
 *  Author: aaron
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "system_config.h"
#include "canlib.h"
#include "adc_functions.h"
#include "misc_functions.h"

/*	Init Global Variables	*/
extern volatile unsigned long sys_time;
unsigned long time_old = 0;
volatile unsigned long time_1ms = 0;
volatile unsigned long time_10ms = 0;
volatile unsigned long time_100ms = 0;
volatile unsigned long time_200ms = 0;
volatile unsigned long time_1000ms = 0;

#define TIME_PASSED_1_MS	(sys_time - time_1ms) >= 1
#define TIME_PASSED_10_MS	(sys_time - time_10ms) >= 10
#define TIME_PASSED_100_MS	(sys_time - time_100ms) >= 100
#define TIME_PASSED_200_MS	(sys_time - time_200ms) >= 200
#define TIME_PASSED_1000_MS	(sys_time - time_1000ms) >= 1000

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
	if(TIME_PASSED_1_MS)
	{
		time_1ms = sys_time;
	}
	
	if(TIME_PASSED_10_MS)
	{
		time_10ms = sys_time;
		adc_start_conversion();
	}	//end of 10 ms cycle

	if (TIME_PASSED_100_MS)
	{
		time_100ms = sys_time;
		PORTC ^= (1<<PC2);
		
		temp_switch(temp_counter);
		uint16_t adc4_data =adc_get(4);
		
		SHB0_databytes[0] = temp_rdy(1)		; //lsb
		SHB0_databytes[1] = (temp_rdy(1)>>8)		; //lsb
		SHB0_databytes[3] = temp_rdy(2)	; //msb
		SHB0_databytes[4] = (temp_rdy(2)>>8)		; //lsb
		SHB0_databytes[5] = temp_rdy(3)	; //msb
		SHB0_databytes[6] = (temp_rdy(3)>>8)		; //lsb
		
		SHB1_databytes[0] =damper_poti(adc_get(5)); //DPRL
		SHB1_databytes[1] = 1; //DPRR
		SHB1_databytes[2] = damper_poti(adc_get(6));
		SHB1_databytes[3] = 1;
		
		can_tx(&can_SHB0_mob, SHB0_databytes); // CAN_10Hz
		can_tx(&can_SHB1_mob, SHB1_databytes);
		
	}  //end of 100ms
	
	if (TIME_PASSED_200_MS)
	{
		time_200ms = sys_time;
		
	} //end of 200ms

}  //end of while
}
			

			

