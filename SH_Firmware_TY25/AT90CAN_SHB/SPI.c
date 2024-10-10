/*
 * SPI.c
 *
 * Created: 10.10.2024 16:43:26
 *  Author: Marek
 */ 

#include "SPI."

void SPI_MasterInit()
{
	//Set MOSI, SCK, SS as output
	DDRB |= (1<<MOSI) | (1<<SCK) | (1<<SS) |;
	
	//Set MISO as input
	DDRB &= ~(1<<MISO);
	
	//enables SPI, Master-Mode, configure Clock_Select
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (0<<SPR0);
	
	//Set all SS low
	PORTE |= (1<<PE0) | (1<<PE1) | (1<<PE2)
}