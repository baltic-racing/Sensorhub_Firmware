/*
 * main.c
 * for the ATtiny85
 * Created: 18.02.2021 18:20:05
 * Author : Eric Gorkow & Lukas Deeken
 */

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t sys_time_10 = 0;
uint8_t SPIdata[2];
uint8_t SPIcount = 0;

unsigned long int time_old = 0;
unsigned long int sys_time = 0;

void sys_timer_config();
void sys_tick();
void SPI_USI_config();

int main(void)
{
    sys_timer_config();
	SPI_USI_config();
	sei();
    while (1)
    {
		//1ms loop 1000Hz
		if((sys_time - time_old) > 0){
			time_old = sys_time;		//start conversion every ms
			sys_time_10++;				//increment 10ms timer
		}
		//10ms loop 100Hz
		if(sys_time_10 >= 10){
			//sys_tick();				//blink status LED Turn off for use on Sensorhub otherwise weird stuff happens ;)
			sys_time_10 = 0;		//reset 50 ms timer
		}	
    }
}

void sys_tick(){
	// switch Status LED state
	//PORTB ^= 1<<PB0;
}

void sys_timer_config(){
	TCCR0A = 0 | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0);
	TCCR0B = 0| (0<<CS02) | (1<<CS01) | (1<<CS00);
	TIMSK |= (1<<OCIE0A) | (1<<TOIE0); //compare interrupt enable
	OCR0A = 250-1; // compare value for 1ms;<
}

ISR(TIM0_COMPA_vect){
	sys_time++; //system time generation
}

ISR(USI_OVF_vect){
	USIDR = SPIdata[2];	//Write LSB into Buffer for send to Master
	USISR = (1 << USIOIF);;// Clear Overflow bit
}

ISR(PCINT0_vect){ //Pin Change Interrupt on Chip Select
	if((PINB & (1 << PB0))== 0){	// If edge is falling, the SPI Comm should be activated
		USIDR = SPIdata[1];			//Write MSB into Buffer for send to Master
		DDRB |= (1 << DDB1);		//Configure MISO as Output
		USISR = (1 << USIOIF);      // Clear Overflow bit
		USICR |= (1 << USIOIE);		// 4-bit overflow counter of the USI communication shall be activated
	}
	else{ // If edge is rising, turn the 4-bit overflow interrupt off:
		USICR &= ~(1 << USIOIE);
		DDRB &= ~(1 << DDB1);		//Configure as Hi Z so MISO does not influence other data traffic
		PORTB &= ~(1 << PB1);		//Configure as Hi Z so MISO does not influence other data traffic		
	}
}

void SPI_USI_config(){
	GIMSK  = (1 << PCIE);					//enable PIN Change Interrupt generally
	PCMSK = (1 << PCINT0);					//enable Pin change Interrupt on CS pin PB0
	
	DDRB &= ~(1 << DDB0)  & ~(1 << DDB2);	//Chip select, SCK input
	DDRB &= ~(1 << DDB1);					//Configure as Hi Z so MISO does not influence other data traffic
	PORTB |= (1 << PB0) | (1 << PB2);		//pull ups for Chipselect and SCK 
	PORTB &= ~(1 << PB1);					//Configure as Hi Z so MISO does not influence other data traffic

	USICR=(1<<USIWM0)|(0<<USIWM1)|(0<<USIOIE)|(1<<USICS1)|(1<< USICS0)|(0<<USICLK);			
	//Choosing SPI aka three wire mode & Disable the Overflow Interrupt as it is handled by the PinChange Interrupt on the CS Pin, 
	//selecting external clock and toggling the 4bit timer on both edges = 8bit per Interrupt  (SPI mode 1)
	USIDR = 0; //Initialization of the first message the Master receives from the ATtiny
	USISR = (1 << USIOIF);// Clear Overflow bit
	//some test data
	SPIdata[2] = 0b10000000;
	SPIdata[1] = 0b00000001;
}