/*
 * SPI.c
 *
 * Created: 09.10.2024 18:22:15
 *  Author: Egquus
 */ 

#include <avr/io.h>
#include "SPI_lib.h"

uint8_t k = 0;
extern volatile uint8_t spi_data[2];

void SPI_MasterInit()
{
	//Set MOSI, SCK, SS as output
	DDRB |= (1<<MOSI) | (1<<SCK) | (1<<SS_uC);
	
	//Set MISO as input
	DDRB &= ~(1<<MISO);
	
	// Set internal pull-up for MISO
	//PORT_ |= (1 << MISO);
	
	//enables SPI, Master-Mode, configure Clock_Select
	SPI_Control_Reg |= (1<<SPI_Enable) | (1<<SPI_Master) | (1<<SPI_Relation1) | (0<<SPI_Relation0);
	
	//Set all SS low
	PORTB |= (1<<SS_uC);
}

void SPI_SlaveInit()
{
	//Set MOSI, SCK, SS as input
	DDRB &= ~((1<<MOSI) | (1<<SCK) | (1<<SS_uC));
	//PORTB |= 1<<SS_uC;
	
	//Set MISO as output
	DDRB |= (1<<MISO);
	
	//enables SPI
	SPI_Control_Reg |= (1<<SPI_Enable) | (1<<SPI_Interrupt_Enable);
	
	DDRA &= ~(1<<PA4);
}

char SPI_SlaveReceive()											// Use with care -> stop the uC
{																//
	/* Wait for reception complete */							//
	while(!(SPI_Status_Reg & (1<<SPI_Interrupft_Flag)));		//
	/* Return data register */									//
	return SPI_Data_Reg;										//
}																//

ISR(SPI_STC_vect)
{
	if (SPDR == 0x22)
	{
		PORTA |= (1<<PA4);
	}else
	{
		PORTA &= ~(1<<PA4);
	}
	if(k==0)
	{
		SPI_Data_Reg = spi_data[0];
		k++;
	}
	else
	{
		SPI_Data_Reg = spi_data[1];
		k=0;
	}
}