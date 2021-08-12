/*
 * SPI.c
 *
 * Created: 25.03.2021 17:23:26
 *  Author: lukas
 */  

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t SPI_typK_data[4];
uint8_t SPI_Speed_data[4];
uint8_t SPIcount = 0;
volatile uint8_t TKTF = 0;	//TypK Transmission Flag, indicates that a SPI TypK read Transmission should be initiated
volatile uint8_t TKTFISR = 0;

void SPI_config(){
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2); // define SS, SCK & MOSI as Output
	DDRB &= ~(1<<PB3); // define MISO as input
	DDRE |= (1<<PE0) | (1<<PE1) | (1<<PE2) | (1<<PE3); // define CS as Output
	PORTB |= (1<<PB0); //Switch SS off
	PORTE |= (1<<PE0) | (1<<PE1) | (1<<PE2) | (1<<PE3); //Switch CS off
	SPCR = (1<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
	SPSR &= ~(0<<SPI2X);
	//SPIE activates SPI Interrupt; SPE Enable SPI; DORD to MSB first see. MAX6675 Datasheet; MSTR set AT90CAN as Master;  
	//CPOL/CPHA set to read Data on falling edge & have clock to pulse from low to High; SPI2X/SPR1/SPR0 set clock divider to 64 = 250Khz max freq. for MAX6675 10Mhz (100ns on time)
}

void SPI_read(){			//initiate dummy transmission to get into the ISR
	PORTB &= ~(1<<PB0);		//Switch SS on
	PORTE &= ~(1<<PE2);		//Switch CS_Speed_1 on
	SPDR = 0x11;			//send empty byte to initiate data Transmission
}
	
ISR(SPI_STC_vect){
		if (TKTFISR == 1){						//When the TKTF has been set and a speed read has completed we go into here
			SPI_typK_data[SPIcount] = SPDR;		//Push the data from the SPDR into the TYPK Array
			SPIcount++;				
			if (SPIcount == 2){					//If 2 Bytes have been transmitted, change to the other Chip
				PORTE |= (1<<PE0);				//Switch CS_TYPK_1 off
				PORTE &= ~(1<<PE1);				//Switch CS_TYPK_2 on
			}
			if (SPIcount >= 4){					//If the entire message has been transmitted terminate the SPI transmission
				PORTE |= (1<<PE1);				//Switch CS_TYPK_2 off
				PORTB |= (1<<PB0);				// Switch SS off
				SPIcount=0;						//Resetting counter for next SPI transmission
				TKTF =0;						//Indicate that the TYPK transmission has been successful
				TKTFISR = 0;					//Indicate that the TYPK transmission has been successful
			}					
			else{								//Initiate the next SPI transmission to get the remaining Bytes											
				SPDR=0x11;						//send empty byte to initiate data Transmission	
			}
		}
		else{ //TKTFISR = 0 We will enter here if there is no need to go into the TYPK transmission Routine
			SPI_Speed_data[SPIcount] = SPDR;	//Push the data from the SPDR into the Speed Array
			SPIcount++;							//incrementing the counter that indicates the number of bytes that has been transmitted
			if (SPIcount == 2){					//If 2 Bytes have been transmitted, change to the other Chip
				PORTE |= (1<<PE2);				//Switch CS_Speed_1 off
				PORTE &= ~(1<<PE3);				//Switch CS_Speed_2 on
			}
			if (SPIcount>=4){					//If the entire message has been transmitted check whether we want a TYPK transmission
				PORTE |= (1<<PE3);				//Switch CS_TYPK_2 off
				SPIcount=0;						//Resetting counter for next SPI transmission
				if (TKTF==1){					//check whether the flag for the TYPK transmission has been set 
					PORTE &= ~(1<<PE0);			//Switch CS_TYPK_1 on
					SPDR = 0x11;				//send empty byte to initiate data Transmission
					TKTFISR = 1;				//Indicate that in the next iteration of the ISR we have to switch differently between the CS
				}
				else{							//IF no TYPK Transmission is requested simply terminate the SPI Transmission
					PORTB |= (1<<PB0);			//Switch SS off, High=off, Low=on terminate the SPI data transfer by pulling SS low
				}
			
			}
			else{
												//Initiate the next SPI transmission to get the remaining Bytes
			SPDR=0x11;							//send empty byte to initiate data Transmission
			}
		}
	}
		
	uint16_t TYPK_getdata1(){
	//cli(); //disable Interrupts so that the SPI transfer will not corrupt our data in a way that we get the MSB of n and LSB of n-1
	uint16_t TYPkDATA1 =  ((SPI_typK_data[0] << 8) | SPI_typK_data[1]) >> 3;//PUT MSB shifted to left by 8 in place and or together with LSB then shift to right by three to get rid of Status bits
	//sei(); // enable Interrupts again
	return TYPkDATA1;	
}

uint16_t TYPK_getdata2(){
	//cli(); //disable Interrupts so that the SPI transfer will not corrupt our data in a way that we get the MSB of n and LSB of n-1
	uint16_t TYPkDATA2 =  ((SPI_typK_data[2] << 8) | SPI_typK_data[3]) >> 3;//PUT MSB shifted to left by 8 in place and or together with LSB then shift to right by three to get rid of Status bits
	//sei(); // enable Interrupts again
	return TYPkDATA2;										
}

uint16_t Speed_getdata1(){
	//cli(); //disable Interrupts so that the SPI transfer will not corrupt our data in a way that we get the MSB of n and LSB of n-1
	uint16_t SpeedDATA1 =  (SPI_Speed_data[1] << 8) | SPI_Speed_data[0];//PUT MSB shifted to left by 8 in place and or together with LSB then shift to right by three to get rid of Status bits
	//sei(); // enable Interrupts again
	return SpeedDATA1;	
}

uint16_t Speed_getdata2(){
	//cli(); //disable Interrupts so that the SPI transfer will not corrupt our data in a way that we get the MSB of n and LSB of n-1
	uint16_t SpeedDATA2 =  (SPI_Speed_data[3] << 8) | SPI_Speed_data[2];//PUT MSB shifted to left by 8 in place and or together with LSB then shift to right by three to get rid of Status bits
	return SpeedDATA2;
	//sei(); // enable Interrupts again						
}