/*
 * SPI.c
 *
 * Created: 09.10.2024 18:22:15
 *  Author: Egquus
 */ 

#include <avr/io.h>
#include "SPI_lib.h"

volatile uint8_t spi_cmd = 0;

void SPI_SlaveInit()
{
	//Set MOSI, SCK, SS as input
	DDRB &= ~((1<<MOSI) | (1<<SCK) | (1<<SS_uC));
	
	//PORTB |= 1<<SS_uC;
	
	//Set MISO as output
	DDRB |= (1<<MISO);
	
	//enables SPI and SPI interrrupt
	SPI_Control_Reg |= (1<<SPI_Enable) | (1<<SPI_Interrupt_Enable);
}

ISR(SPI_STC_vect)
{	
	spi_cmd = SPDR;
    
    if (spi_cmd == 0x01){
		SPDR = wheelspeed_left;
    }
    else if (spi_cmd == 0x03){
		SPDR = wheelspeed_right;
    }
    else{
	    SPDR = 0x00;
	}
}