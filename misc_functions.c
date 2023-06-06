/*
 * misc_functions.c
 *
 * Created: 05.04.2019 18:51:39
 *  Author: Ole Hannemann
 * last change: 28.02.2021
 *  Author: Lukas Deeken
 */ 
#include "misc_functions.h"
#include <stdio.h>
#include <float.h>
volatile unsigned long sys_time = 0;
#include <math.h>
#include "adc_functions.h"
#include "SPI.h"

//Volatile:	 Variable can change within an interrupt
//long:		 32Bit
//Unsigned:	 only positive numbers are allowed

#define trigger_angle 11.25//Trigger Angle in degree both high & low are the same
#define Tcirc 1476.5485 //Tire circumference in mm
#define desired_update_frequency 100//Update frequency for the floating calculation of the Wheelspeed

uint16_t timediff[10];

void port_config(){
	DDRA = 0;												//All not used
	DDRB = 0;												//Only SPI so 0 just like CAN
	DDRC = 0 | (1<<PC0) | (1<<PC2);							//PC0 and PC2 as Output for Heart and Foult LED otherwise unused
	DDRD = 0;												//Either CAN ID or Can so all Input
	DDRE = 0 | (1<<PE0) | (1<<PE1) | (1<<PE2) | (1<<PE3);	//PE0 to PE3 as Chip Select Outputs for SPI
	DDRF = 0;												//mark analog inputs on Port F as input														
}

void sys_tick(){

	PORTC ^= 1<<PC2; //toggling the Status of PC2 to make the Heart LED blink every SYStick

}

void sys_timer_config(){
	
	//8 bit Timer 0 configuration
	//ctc mode and 64 as prescaler for 16Mhz
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);
	TIMSK0 = 0 | (1<<OCF0A);	//compare interrupt enable
	OCR0A = 250-1;				// compare value for 1ms;
	
}

//ISR for Timer 0 compare interrupt
ISR(TIMER0_COMP_vect){
	
	sys_time++; //system time generation
	//for every time the timer equals 249 an interrupt is generated resulting in increasing the SYStime

}

uint16_t ADC2NTCtemp(uint16_t data, uint16_t bfactor, uint16_t R_NTC_norm, uint16_t ADC_Volt, uint16_t ADC_acc, uint16_t R_Teiler){
		uint16_t temperature = 0;
	data=1024-data;
		double NTC_Volt = ((double)ADC_Volt/(double)ADC_acc)*(double)data; //in 0,1mv
		double R_NTC = ((double)NTC_Volt*(double)R_Teiler)/((double)ADC_Volt-(double)NTC_Volt);
		double ln = logf((double)R_NTC/(double)R_NTC_norm);

		temperature = (1/((1/T_norm)+(1/(double)bfactor)*(double)ln))*10; //in 0,1K
		return temperature;
}

uint16_t ADC2Sensor(uint16_t data, float start_Volt, float end_Volt, uint8_t sensor_max, uint16_t ADC_bit, uint8_t ADC_Volt, uint16_t precision){
	uint16_t start_ADC = (pow(2,ADC_bit))/ADC_Volt*start_Volt;
	uint16_t end_ADC = (pow(2,ADC_bit))/ADC_Volt*end_Volt ;
	float ADC_range = end_ADC - start_ADC
	;
	float sensor_factor = sensor_max/ADC_range;
	uint16_t Sensor_Data = (data-start_ADC)*precision*sensor_factor;
	if (data<start_ADC){
		Sensor_Data = 0;
	}
	return Sensor_Data;
}

uint16_t ticks2speed(uint16_t timesteppdiff){
	//Execution with 100Hz
	//Wheelspeed Pre determination
	double angular_velocity = ((double)timesteppdiff/10)/(double)trigger_angle;
	angular_velocity = pow((double)angular_velocity,-1);
	double RPS = ((double)angular_velocity/360)*1000; //°/ms-> Rev/s
	
	//High accuracy Wheelspeed
	uint16_t timediffinterval = 0;
	
	
	//execute with every generated interrupt
	for (int8_t i = 8; i > -1;i--){
		timediff[i+1]=timediff[i];
	}
	timediff[0] = timesteppdiff;
	
	
	double ideal_measurement_angle = (double)(((double)RPS * 360) / desired_update_frequency);
	double interval_width = (double)(ideal_measurement_angle / trigger_angle);
	double possible_measurement_angle = (double)(lrint(interval_width) * trigger_angle); //lrint rounds to nearest Int
	if (possible_measurement_angle == 0){
		possible_measurement_angle = trigger_angle;
	}
	
	for (uint8_t i = 0  ;i <= (possible_measurement_angle/trigger_angle)-1;i++){
		timediffinterval += timediff[i];
	}
	
	double RPS_Float = (double)((pow(((timediffinterval/10)/possible_measurement_angle),-1))/360)*1000;
	uint16_t Wheelspeed_kmh_float = (double)((RPS_Float * (Tcirc / 1000))*100*3.6);
	
	return Wheelspeed_kmh_float;

}

/*
uint16_t ticks2speed_moar(){
			for (uint8_t i = 9; i <= 1;i--){
				timediff[i]=timediff[i+1];
			}
	timediff[0] = SpeedDATA2;
		
	timediff[0] = SpeedDATA2;
	angular_velocity = timediff[0]/trigger_angle;
	angular_velocity = pow(angular_velocity,-1);
	RPS_simple = (angular_velocity/360)*1000; //°/ms-> Rev/s		
	
	ideal_measurement_angle = RPS_simple * 360 / desired_update_frequency;
	interval_width = ideal_measurement_angle / trigger_angle;
	possible_measurement_angle = interval_width * trigger_angle;
	if (possible_measurement_angle == 0){
		possible_measurement_angle = trigger_angle;
	}
	for (uint8_t i = interval_width ; i <= 1;i--){
		timediffinterval += timediff[i];
	}
	RPS = 1 / ((timediffinterval / interval_width) * (360 / 1000));
	uint16_t Wheelspeed = RPS * (Tcirc / 1000*1000) * 60*60;
	return Wheelspeed;
	}
	
*/