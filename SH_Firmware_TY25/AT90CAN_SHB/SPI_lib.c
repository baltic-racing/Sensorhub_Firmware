/*
 * SPI.c
 *
 * Created: 09.10.2024 18:22:15
 *  Author: Egquus
 */ 

#include <avr/io.h>
#include "SPI_lib.h"


void SPI_MasterInit()
{
	//Set MOSI, SCK, all SS as output
	DDRB |= (1<<MOSI) | (1<<SCK);
	DDRE |= (1<<SS_uC) | (1<<SS_TK1) | (1<<SS_TK2);
	
	//Set MISO as input
	DDRB &= ~(1<<MISO);
	
	// Set internal pull-up for MISO
	//PORTB |= (1 << MISO);
	
	//enables SPI, Master-Mode, configure Clock_Select
	SPI_Control_Reg |= (1<<SPI_Enable) | (1<<SPI_Master) | (1<<SPI_Relation1) | (0<<SPI_Relation0);
	
	//Set all SS High
	PORTE |= (1<<SS_uC) | (1<<SS_TK1) | (1<<SS_TK2);
	
}

void SPI_SlaveInit()
{
	//Set MOSI, SCK, SS as input
	DDRB &= ~((1<<MOSI) | (1<<SCK) | (1<<SS_uC));
	
	//Set MISO as output
	DDRB |= (1<<MISO);
	
	//enables SPI
	SPI_Control_Reg |= (1<<SPI_Enable);
}

char SPI_SlaveReceive()
{
	/* Wait for reception complete */
	while(!(SPI_Status_Reg & (1<<SPI_Interrupt_Flag)));
	/* Return data register */
	return SPI_Data_Reg;
}

uint8_t SPI_MasterTransmit(uint8_t data) {
	SPDR = data;
	// Auf Übertragung warten
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
//
//uint16_t SPI_Getspeed(uint8_t side){
	//SS_uC_LOW();
	//SPDR = side;
	//while(!(SPSR & (1 << SPIF)));
	//SS_uC_HIGH();
				//
	//SS_uC_LOW();
	//SPDR = 0xFF;
	//while(!(SPSR & (1 << SPIF)));
	//uint8_t lsb = SPDR;
	//SS_uC_HIGH();
				//
	//SS_uC_LOW();
	//SPDR = (side+1);
	//while(!(SPSR & (1 << SPIF)));
	//SS_uC_HIGH();
	//
	//SS_uC_LOW();
	//SPDR = 0xFF;
	//while(!(SPSR & (1 << SPIF)));
	//uint8_t msb = SPDR;
	//SS_uC_HIGH();
	//return ((msb << 8) | lsb);
	//
//}