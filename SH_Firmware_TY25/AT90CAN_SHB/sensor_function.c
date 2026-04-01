/*
 * sensor_function.c
 *
 * Created: 28.03.2025 12:51:55
 *  Author: pasca
 */ 


#include "SPI_lib.h"
#include "sensor_function.h"
#include <avr/io.h>
#include <math.h>

uint8_t getwheelspeed(uint8_t side)
{
	uint8_t wheelspeed_function = 0;
	
	if (side == 0x01)											// wheelspeed left
	{
		//wheelspeed_left
		SS_uC_LOW();											// select uC  for SPI-communication
		SPI_transfer(0x01);										// for 0x01: slave put ws_left in SPDR
		wheelspeed_function = SPI_transfer(0x00);				// Slave send 8bit of wheelspeed_left
		SS_uC_HIGH();											// deselect uC for SPI-communication
	}
	
	if (side == 0x03)											// wheelspeed right
	{
		//wheelspeed_right
		SS_uC_LOW();											// select uC  for SPI-communication
		SPI_transfer(0x03);										// for 0x03: slave put ws_right in SPDR
		wheelspeed_function = SPI_transfer(0x00);				// Slave send 8bit of wheelspeed_left
		SS_uC_HIGH();											// deselect uC for SPI-communication
	}
	
	return wheelspeed_function;
}

uint16_t calculate_BP(uint16_t adc_BP_data)						//sensor 0.5V-4.5V		//0.5V = 102.3 ADC		//4.5V = 921 ADC
{
	uint16_t value_BP = 0;
	
	if(adc_BP_data > BP_MIN)
	{
			value_BP = (adc_BP_data-100)/(0.5453);		//in 0.1bar
	}
	else
	{
		value_BP = 0;
	}
	
	return value_BP;											//return BP  in 0.1bar 
}

double damper_poti(double dp_adc){
	double travel = 0;
	
	//travel = (80*dp_adc)/1024;
	
	travel = DAMP_MM_MIN + (dp_adc-DAMP_ADC_MIN)*((DAMP_MM_MAX - DAMP_MM_MIN)/(DAMP_ADC_MAX - DAMP_ADC_MIN)); //interpolate with 2 values
	
	return travel;
}

double calculate_steering_percent(double pot_adc) {
	double steering_value = 0;
	
	if(pot_adc <= POT_MID){
		steering_value = 100 - ((100 / (double)(POT_MID - POT_MIN)) * (pot_adc - (double)POT_MIN));
		if (steering_value >= 100) steering_value = 100;
	}

	if(pot_adc > POT_MID){
		steering_value = -1 * (100 - ((100 / (double)(POT_MID - POT_MAX)) * (pot_adc - (double)POT_MAX)));
		if (steering_value >= 100) steering_value = -100;
	}
	
	return steering_value;
}