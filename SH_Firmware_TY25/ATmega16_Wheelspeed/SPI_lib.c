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
	
	SPDR = 0x00;
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
	// Read the received byte from the SPI Data Register
    uint8_t received = SPDR;

    // Immediately send the previously prepared response byte
    // (this value will be shifted out during the current SPI transfer)
    SPDR = response;

    // Prepare the next response based on the received command
    if (received == 0x01) {
	    response = wheelspeed_left;								// Command 0x01: send left wheel speed
	    } else if (received == 0x02) {
	    response = wheelspeed_right;							// Command 0x02: send right wheel speed
	    } else {
	    response = 0xFF;										// Unknown command: send error value
    }
	
	
	
	//if(SPI_Data_Reg == 0x33){
		//SPI_Data_Reg = (wheelspeed);
	//}
}