/*
 * SPI.c
 *
 * Created: 09.10.2024 18:22:15
 *  Author: Egquus
 */ 

#include <avr/io.h>

#include "SPI_lib.h"

extern volatile uint8_t wheelspeed;

void SPI_MasterInit()
{
	//Set MOSI, SCK 
	DDRB |= (1<<MOSI) | (1<<SCK);
	
	//Set MISO as input
	DDRB &= ~(1<<MISO);
	
	//sez Hardware-SS-Pin as Output. otherwise AT90CAN is selected as Slave by an other uC
	DDRB |= (1<<PB0);
	
	//all SS as output
	DDRE |= (1<<SS_uC) | (1<<SS_TK1) | (1<<SS_TK2) | (1<<SS_TK3) | (1<<SS_TK4);
	
	//all SS high = inactive
	PORTE |= (1<<SS_uC) | (1<<SS_TK1) | (1<<SS_TK2) | (1<<SS_TK3) | (1<<SS_TK4);
	
	// Set internal pull-up for MISO
	//PORTB |= (1 << MISO);
	
	//enables SP, Master-Mode, set SPI-frequency --> prescaler 64 --> 16MHz:64=250kHz
	SPI_Control_Reg = (1<<SPI_Enable) | (1<<SPI_Master) | (1<<SPI_Relation1);
}

uint8_t SPI_transfer(uint8_t data)
{
	SPDR = data;						// write in SPDR, which information to send next
	
	while(!(SPSR & (1 << SPIF)))		// wait for SPI_Interrupt_Flag (SPIF) --> if send = transfer complete
	{
	}
	return SPDR;
}