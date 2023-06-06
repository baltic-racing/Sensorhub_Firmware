/*
 * main.c
 * for the ATtiny85
 * Created: 18.02.2021 18:20:05
 * Author : Eric Gorkow & Lukas Deeken
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

uint8_t SPIcount = 0;

volatile unsigned long int time_delta = 0;
volatile unsigned long int highSpeedTimeEnd = 0;
volatile unsigned long int highSpeedTimeStart = 0;
uint8_t lowSpeedCounter = 0;
volatile unsigned long int sys_time = 0;
volatile unsigned long int sys_time_1000 = 0;
volatile uint16_t timestepdiff = 0;
volatile uint16_t steps_per_intervall = 0;
volatile unsigned long int timediff[10];
uint16_t Wheelspeed_kmh_float = 0;
uint16_t Wheelspeed_kmh = 0;
uint8_t Wheelspeed_kmh_ext = 0;
uint16_t Wheelspeed_filter_values[3];
uint8_t n = 0;
double Wheelspeed_filtered_double = 0;
uint16_t Wheelspeed_filtered_int = 0;

volatile uint8_t PB5_state_old = 0;
volatile uint8_t cs_state_old = 1;
volatile uint16_t ticks = 0;

volatile uint8_t timer_interrupt_occured = 0;
volatile uint8_t Lowspeed_active = 0;

double speedScale = (3.6 * 2 * 1476.5485) / (32.0) * 10.0;

#define trigger_angle 11.25//Trigger Angle in degree both high & low are the same
#define Tcirc 1476.5485 //Tire circumference in mm
#define desired_update_frequency 100//Update frequency for the floating calculation of the Wheelspeed
#define update_frequency_simple_ms 50
#define number_of_teeth 32.0

void sys_timer_config();
void sys_tick();
void SPI_USI_config();
void Timer_interrupt_Handler();
void Lowspeed_WS();
void Highspeed_WS();
void Wheelspeed_filter();

int main(void)
{
	cli();
	sys_timer_config();
	SPI_USI_config();
	sei();
	
	while (1){
		
		if (sys_time_1000>=(update_frequency_simple_ms*10)){

			sys_time_1000 = 0;
/*			if (steps_per_intervall >= 20 || ((steps_per_intervall > 16) & (Lowspeed_active == 0))){
				Highspeed_WS();
				Lowspeed_active = 0;
			}else if(steps_per_intervall <= 16 || ((steps_per_intervall < 20) & (Lowspeed_active == 1))){
				Lowspeed_WS();
				Lowspeed_active = 1;
			}
*/
			Highspeed_WS();	
		//Wheelspeed_filter();

		
		}
		if (timer_interrupt_occured == 1){
			Timer_interrupt_Handler();
		}
		
	}
}
/*
void Wheelspeed_filter(){
	if (n >= 5){
		n = 0;
	}
	Wheelspeed_filter_values[n] =  Wheelspeed_kmh;
	n++;
	
	for (uint8_t i = 0; i < 5; i++)
	{
		Wheelspeed_filtered_double += Wheelspeed_filter_values[n];
		if (n == 0){
			n = 5;
		}else{
			n--;
		}
	}
	Wheelspeed_filtered_int = Wheelspeed_filtered_double;
	}
	
*/
void sys_timer_config(){
	TCCR0A = 0 | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0);
	TCCR0B = 0| (0<<CS02) | (1<<CS01) | (1<<CS00);
	TIMSK |= (1<<OCIE0A) | (1<<TOIE0); //compare interrupt enable
	OCR0A = 25-1; // compare value for 0,1ms
}

void Timer_interrupt_Handler(){
	time_delta++; //system time generation 0.1ms
	sys_time++;
	sys_time_1000++;
	timer_interrupt_occured = 0;
	}

ISR(TIM0_COMPA_vect){
	timer_interrupt_occured = 1;
}
/*
ISR(USI_OVF_vect){
	cli();
	USIDR = debugStatic >> 8;//Wheelspeed_kmh >> 8;	//Write LSB into Buffer for send to Master
	USISR = (1 << USIOIF);// Clear Overflow bit
	sei();
}
*/
ISR(PCINT0_vect){ //Pin Change Interrupt on Chip Select
	
	cli();
	
	if((PINB & (1 << PB0)) != cs_state_old){//If the status of the cs pin has changed either we want to activate or deactivate the spi comm
		cs_state_old = (PINB & (1 << PB0));	
		if((PINB & (1 << PB0))== 0){	// If cs is low & the status of the pin has changed, the SPI Comm should be activated
			USIDR = Wheelspeed_kmh & 0xff;//Write MSB into Buffer for send to Master
			DDRB |= (1 << DDB1);		//Configure MISO as Output
			USISR = (1 << USIOIF);      // Clear Overflow bit
			//USICR |= (1 << USIOIE);		// 4-bit overflow counter of the USI communication shall be activated
		}else{ // If cs is high & the status of the pin has changed, turn the 4-bit overflow interrupt off:
			USICR &= ~(1 << USIOIE);
			DDRB &= ~(1 << DDB1);		//Configure as Hi Z so MISO does not influence other data traffic
			PORTB &= ~(1 << PB1);		//Configure as Hi Z so MISO does not influence other data traffic
		}
	}else{	//if the value of cs hasnt changed the value of digi in must have changed
		timestepdiff = time_delta; //calculates the time from edge to edge in 0,1ms
		highSpeedTimeEnd = sys_time;
		time_delta = 0;
		steps_per_intervall++;
	}
	
	sei();
	
}

void SPI_USI_config(){
	GIMSK  = (1 << PCIE);					//enable PIN Change Interrupt generally
	PCMSK = (1 << PCINT0);					//enable Pin change Interrupt on CS pin PB0
	PCMSK |= (1 << PCINT5);
	DDRB &= ~(1 << DDB0)  & ~(1 << DDB2);	//Chip select, SCK input
	DDRB &= ~(1 << DDB1);					//Configure as Hi Z so MISO does not influence other data traffic
	PORTB |= (1 << PB0) | (1 << PB2);		//pull ups for Chipselect and SCK
	PORTB &= ~(1 << PB1);					//Configure as Hi Z so MISO does not influence other data traffic

	USICR=(1<<USIWM0)|(0<<USIWM1)|(0<<USIOIE)|(1<<USICS1)|(1<< USICS0)|(0<<USICLK);
	//Choosing SPI aka three wire mode & Disable the Overflow Interrupt as it is handled by the PinChange Interrupt on the CS Pin,
	//selecting external clock and toggling the 4bit timer on both edges = 8bit per Interrupt  (SPI mode 1)
	USIDR = 0; //Initialization of the first message the Master receives from the ATtiny
	USISR = (1 << USIOIF);// Clear Overflow bit
}
	//Wheelspeed Low Speed
void Lowspeed_WS(){
	
	double angular_velocity = ((double)timestepdiff/10.0)/(double)trigger_angle;
	angular_velocity = pow((double)angular_velocity,-1.0);
	double RPS = ((double)angular_velocity/360)*1000.0; //°/ms-> Rev/s	
	Wheelspeed_kmh = (((double)RPS * (Tcirc / 1000.0))*100.0*3.6);
	
}
	//Wheelspeed High Speed	
void Highspeed_WS(){
	//double RPS = ((double)steps_per_intervall/update_frequency_simple_ms)*1000.0;
	//RPS = (double)RPS*(1.0/number_of_teeth);
	//Wheelspeed_kmh = (((double)RPS * (Tcirc / 1000.0))*100.0*3.6);
	
	if (highSpeedTimeStart != highSpeedTimeEnd) {
		Wheelspeed_kmh = steps_per_intervall * speedScale / (highSpeedTimeEnd - highSpeedTimeStart);	
		highSpeedTimeStart = highSpeedTimeEnd;
		steps_per_intervall = 0;
	} else if (lowSpeedCounter < 3){
		lowSpeedCounter++;
	} else {
		lowSpeedCounter = 0;
		Wheelspeed_kmh = 0;
	}
}


