/*
 * SPI.c
 *
 * Created: 27.05.2023 14:18:30
 *  Author: adaam
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

void SPI_config(){
//select the master mode?
 //NO SPI NEEDED ALL DATA (CLTF1, CLTF2 CLTR1, CLTR2) IS READ BY THE ADC0..3 AND SENT THROUGH CAN
}
//read function
//what does the isr vector cause
//fetch data from the array(s) that isr fills