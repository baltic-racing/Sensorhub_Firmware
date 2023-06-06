/*
 * misc_functions.c
 *
 * Created: 05.04.2019 18:51:39
 *  Author: Ole Hannemann
 * last change: 28.02.2021
 *  Author: Lukas Deeken
 */ 
#include "misc_functions.h"
volatile unsigned long sys_time = 0;
#include <math.h>

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
	float ADC_range = end_ADC - start_ADC;
	float sensor_factor = sensor_max/ADC_range;
	uint16_t Sensor_Data = (data-start_ADC)*precision*sensor_factor;
	if (data<start_ADC){//Set sensor value to zero if below the ADC Threshold
		Sensor_Data = 0;
	}
	return Sensor_Data;
}

uint16_t ADC2Angle(int16_t ADC_Data, uint16_t angle_mid, int16_t angle_min, uint16_t angle_max, uint16_t ADC_mid, uint16_t ADC_min, uint16_t ADC_max){
	
	//returns the Angle in 0,1 Degree steps in Positive and negative range with a mid value
	
	int16_t Angle = 0;
	
	uint16_t ADC_range_up = ADC_max - ADC_mid;
	uint16_t Angle_Range_up = angle_max - angle_mid;
	
	uint16_t ADC_range_down = ADC_mid - ADC_min;
	uint16_t Angle_Range_down = angle_mid + angle_min;	
	
	double Degree_per_ADC_up = (double)Angle_Range_up/(double)ADC_range_up;
	double Degree_per_ADC_Down = (double)Angle_Range_down/(double)ADC_range_down;
	
//	Algorythm for 110 0 -110
	if (ADC_Data > ADC_mid-1){
		Angle = (ADC_Data-ADC_mid) * 10 * Degree_per_ADC_up;
	}else if (ADC_Data < ADC_mid+1){
		Angle = ((ADC_mid-ADC_Data) * 10 * Degree_per_ADC_Down);
	}else{
		Angle = 0;
	}


/* Algoryth for 0 110 220
	if (ADC_Data > ADC_mid-1){
		Angle = ADC_Data * 10 * Degree_per_ADC_up;
	}else if (ADC_Data < ADC_mid+1){
		Angle = ADC_Data * 10 * Degree_per_ADC_Down;
	}else{
		Angle = 0;
	}
	*/
	return Angle;
	
}