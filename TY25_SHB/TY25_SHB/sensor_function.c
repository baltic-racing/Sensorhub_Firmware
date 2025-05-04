/*
 * sensor_function.c
 *
 * Created: 31.03.2025 14:45:12
 *  Author: pasca
 */ 


#include "sensor_function.h"
#include "SPI_lib.h"
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>


float beta = 3450.0f;				// optimized between 0-100∞C
float R_25C = 10000.0f;				// restistance at 25∞C
float T_25C = 298.15f;
float VCC = 5.0f;
float ADC_max = 1023.0f;
float R_i = 3300.0f;

uint16_t temp_calc(double ntc_adc){
	float U_NTC = (VCC / ADC_max) * ntc_adc;
	float R_NTC = R_i * ((VCC / U_NTC) - 1);
	
	float temperature_kelvin = (beta * T_25C) / (beta + (T_25C * log(R_NTC / R_25C)));
	float temperature_celsius = (temperature_kelvin - 273.15f) * 10;
	
	return (uint16_t) temperature_celsius;
}

//uint16_t temp_calc(double ntc_adc){
	//double temp = 0;
	//
	//
	//temp = ((0.128479 * ntc_adc) - 8.137044) * 10; // NTC Kurve ist linearisiert im Bereich 80∞-20∞, auﬂerhalb ungenauer
	//
	//return temp;
//}

double damper_poti(double dp_adc){
	double travel = 0;
	
	//travel = (80*dp_adc)/1024;
	
	travel = DAMP_MM_MIN + (dp_adc-DAMP_ADC_MIN)*((DAMP_MM_MAX - DAMP_MM_MIN)/(DAMP_ADC_MAX - DAMP_ADC_MIN)); //interpolate with 2 values
	
	return travel;
}

//float read_max6675(void) {
	//uint8_t high_byte, low_byte;
	//uint16_t value;
//
	//// Chip Select Pin als Ausgang
	////MAX6675_CS_DDR |= (1 << MAX6675_CS_PIN);
	//
	//// CS auf Low (aktiv)
	//SS_TK1_LOW();
	////_delay_us(10); // kleine Wartezeit
//
	//// Zwei Bytes vom MAX6675 lesen
	//(void) SPI_transfer(0x00);
	//high_byte = SPI_transfer(0x00);
	////(void) SPI_transfer(0x00);
	//low_byte  = SPI_transfer(0x00);
//
	//// CS wieder auf High (inaktiv)
	//SS_TK1_HIGH();
//
	//// Bits zusammenf¸gen
	//value = ((uint16_t)high_byte << 8) | low_byte;
//
	//// Pr¸fen, ob Thermoelement angeschlossen ist (Bit D2 == 1 ? Fehler)
	//if (value & 0x0004) {
		//return -1.0; // Fehlerwert
	//}
//
	//// Bits D[14:3] enthalten Temperatur in 0,25 ∞C Schritten
	//value >>= 3; // nur D[14:3] verwenden
	////value = value & 0x0FFF;
//
	//// In Grad Celsius umrechnen
	//return value * 0.25;
//}