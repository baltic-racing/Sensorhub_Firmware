/*
 * SPI.c
 *
 * Created: 09.10.2024 18:22:15
 *  Author: racin
 */ 

#include "SPI.h"

void SPI_SlaveInit()
{
	//Set MOSI, SCK, SS as input
	DDRB &= ~((1<<MOSI) | (1<<SCK) | (1<<SS));
	
	//Set MISO as output
	DDRB |= (1<<MISO);
	
	//enables SPI
	SPCR |= (1<<SPE);
}

char SPI_SlaveReceive()
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return data register */
	return SPDR;
}