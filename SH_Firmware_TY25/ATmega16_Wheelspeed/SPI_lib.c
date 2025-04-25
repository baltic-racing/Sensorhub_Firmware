/*
 * SPI.c
 *
 * Created: 09.10.2024 18:22:15
 *  Author: Egquus
 */ 

#include <avr/io.h>
#include "SPI_lib.h"

volatile uint8_t spi_cmd = 0;
volatile uint8_t next_response = 0;
volatile uint8_t awaiting_response = 0;
volatile uint8_t response = 0;
volatile uint8_t last_command = 0;
volatile uint8_t spi_state = 0;
volatile uint8_t spi_response_buffer = 0;
uint8_t rx_count = 1;
uint8_t rx_array[2];

uint8_t switcho = 0;
uint8_t fii = 0;


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
	
	DDRA |= (1<<PA4);
	
	//SPDR = 0x00;
}

char SPI_SlaveReceive(void)											// Use with care -> stop the uC
{																//
	/* Wait for reception complete */							//
	while(!(SPI_Status_Reg & (1<<SPI_Interrupft_Flag)));		//
	/* Return data register */									//
	return SPI_Data_Reg;										//
}																//

void SPI_SlaveSend(uint8_t data){
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
}

ISR(SPI_STC_vect)
{
	
	if(SPDR == 0x01){
		SPDR = wheelspeed_left & 0xFF;		//lsb
		//SPDR = 0xAA;
	}

	if(SPDR == 0x02){
		SPDR = wheelspeed_left >> 8;		//msb
		//SPDR = 0xBB;
	}
	
	
	if (SPDR == 0x03){
		SPDR = wheelspeed_right & 0xFF;		//lsb
		//SPDR = 0xCC;
	}
	
	if (SPDR == 0x04){
		SPDR = wheelspeed_right >> 8;		//msb
		//SPDR = 0xDD;
	}
	

}