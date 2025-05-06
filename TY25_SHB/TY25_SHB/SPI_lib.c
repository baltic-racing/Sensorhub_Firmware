/*
 * SPI_lib.c
 *
 * Created: 31.03.2025 14:45:42
 *  Author: pasca
 */ 


#include <avr/io.h>

#include "SPI_lib.h"

extern volatile uint8_t wheelspeed;

uint8_t SPIdata[2];
uint8_t SPIcount = 0;

void SPI_MasterInit()
{
	//Set MOSI, SCK, all SS as output
	DDRB |= (1<<MOSI) | (1<<SCK);
	DDRE |= (1<<SS_uC) | (1<<SS_TK1) | (1<<SS_TK2) | (1<<SS_TK3) | (1<<SS_TK4);
	
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

uint8_t SPI_transfer(uint8_t data) {
    SPDR = data; // Sende Byte
    while (!(SPSR & (1 << SPIF))); // Warten bis fertig
    return SPDR; // Empfangenes Byte zurückgeben
}

//void TYPK_read(){
	//PORTB &= ~(1<<PB0);//Switch SS on
	//SS_TK1_LOW();//Switch CS_TYPK_1 on
	//
	//SPDR=0x11; //send empty byte to initiate data Transmission
	////Then we need to go into the interrupt when the transmission of the first byte is finished
//}
//
//uint16_t TYPK_getdata(){
	////cli(); //disable Interrupts so that the SPI transfer will not corrupt our data in a way that we get the MSB of n and LSB of n-1
	//uint16_t TYPkDATA =  ((SPIdata[0] << 8) | SPIdata[1]) >> 3;//PUT MSB shifted to left by 8 in place and or together with LSB then shift to right by three to get rid of Status bits
	////sei(); // enable Interrupts again
	//return TYPkDATA;	
//}
//
//ISR(SPI_STC_vect){
	////Store the data that has been pushed into SPDR via the Slave
	//SPIdata[SPIcount] = SPDR;
	//SPIcount++; //incrementing the counter that indicates the number of bytes that has been transmitted
	////If the entire message has been transmitted terminate the SPI data transfer by pulling Chip Select low
	//if (SPIcount>=2){
		//
		//PORTB |= (1<<PB0);//Switch SS off, High=off, Low=on
		//SS_TK1_HIGH();//Switch SS off, High=off, Low=on
		////PORTE |= (1<<PE0);//Switch CS_TYPK_1 off
		//
		//SPIcount=0;//Resetting counter for next SPI transmission
	//}
	//else{
	////Initiate the next SPI transmission to get the remaining Bytes
	//SPDR=0x11; //send empty byte to initiate data Transmission		
	//}
//}

//ISR(SPI_STC_vect)
//{
	//PORTE |= (1<<SS_uC);
	//wheelspeed = SPI_Data_Reg;
//}