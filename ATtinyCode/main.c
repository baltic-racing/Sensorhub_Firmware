
/*
 * main.c
 * for the ATtiny85
 * Created: 18.02.2021 18:20:05
 * Author : Eric Gorkow & Lukas Deeken
 */

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
volatile unsigned long int sys_time_1000 = 0;
volatile uint16_t timestepdiff = 0;
volatile uint16_t steps_per_intervall = 0;
volatile unsigned long int timediff[10];
uint16_t Wheelspeed_kmh_float = 0;
uint16_t Wheelspeed_kmh = 0;


volatile uint8_t PB5_state_old = 0;
volatile uint8_t cs_state_old = 0;
volatile uint16_t ticks = 0;
uint8_t PinChangeInterruptOccured = 0;


#define trigger_angle 11.25//Trigger Angle in degree both high & low are the same
#define Tcirc 1476.5485 //Tire circumference in mm
#define desired_update_frequency 100//Update frequency for the floating calculation of the Wheelspeed
#define update_frequency_simple_ms 50
#define number_of_teeth 32

void sys_timer_config();
void sys_tick();
void SPI_USI_config();
void ticks2speed();
void timediffarray_shfit();
void Lowspeed_WS();
void Highspeed_WS();
void PinChangeInterruptHandler();

int main(void)
{
	cli();
	sys_timer_config();
	SPI_USI_config();
	sei();
	while (1){
		
		if (sys_time_1000>=(update_frequency_simple_ms*10)){
			sys_time_1000 = 0;
			//timestepdiff = 36; //TEST//
			//ticks2speed();
			//timediffarray_shfit();//TEST//
			if (steps_per_intervall >= 18){
				Highspeed_WS();
			}else{
				Lowspeed_WS();
			}
			steps_per_intervall = 0;
			
		if (PinChangeInterruptOccured)
		{
			PinChangeInterruptOccured = 0;
			PinChangeInterruptHandler();
		}
			
		}
	}
}


void sys_timer_config(){
	TCCR0A = 0 | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0);
	TCCR0B = 0 | (1<<CS01); //Prescaler 8
	TIMSK |= (1<<OCIE0A) | (1<<TOIE0); //compare interrupt enable
	OCR0A = 128-1; //This Plus Prescaler gives a Freq of 15,625Khz
	/*
	TCCR0A = 0 | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0);
	TCCR0B = 0| (0<<CS02) | (1<<CS01) | (1<<CS00);
	TIMSK |= (1<<OCIE0A) | (1<<TOIE0); //compare interrupt enable
	OCR0A = 25-1; // compare value for 0,1ms
	*/
}

void PinChangeInterruptHandler()
{
	if((PINB & (1 << PB0)) != cs_state_old){//If the status of the cs pin has changed either we want to activate or deactivate the spi comm
		cs_state_old = (PINB & (1 << PB0));
		if((PINB & (1 << PB0))== 0){	// If cs is low & the status of the pin has changed, the SPI Comm should be activated
			USIDR = 12300 & 0xff;//Write MSB into Buffer for send to Master
			DDRB |= (1 << DDB1);		//Configure MISO as Output
			USISR = (1 << USIOIF);      // Clear Overflow bit
			USICR |= (1 << USIOIE);		// 4-bit overflow counter of the USI communication shall be activated
			}else{ // If cs is high & the status of the pin has changed, turn the 4-bit overflow interrupt off:
			USICR &= ~(1 << USIOIE);
			DDRB &= ~(1 << DDB1);		//Configure as Hi Z so MISO does not influence other data traffic
			PORTB &= ~(1 << PB1);		//Configure as Hi Z so MISO does not influence other data traffic
		}
		}else{	//if the value of cs hasnt changed the value of digi in must have changed
		timestepdiff = time_delta; //calculates the time from edge to edge in 0,1ms
		time_delta = 0;
		//execute with every generated interrupt
		//timediffarray_shfit();
		steps_per_intervall++;
	}
	PCMSK |= (1 << PCINT0);					//enable Pin change Interrupt on CS pin PB0
	PCMSK |= (1 << PCINT5);
}

ISR(TIM0_COMPA_vect){
	time_delta++; //system time generation 0.1ms
	sys_time_1000++;
}

ISR(USI_OVF_vect){
	USIDR = Wheelspeed_kmh >> 8;	//Write LSB into Buffer for send to Master
	USISR = (1 << USIOIF);// Clear Overflow bit
}

ISR(PCINT0_vect){ //Pin Change Interrupt on Chip Select
	PCMSK &= ~(1 << PCINT0);					//enable Pin change Interrupt on CS pin PB0
	PCMSK &= ~(1 << PCINT5);
	PinChangeInterruptOccured = 1;
}
/*
void timediffarray_shfit(){
	//cli();
	for (int8_t i = 8; i > -1;i--){
		timediff[i+1]=timediff[i];
	}
	timediff[0] = timestepdiff;
	//sei();
}
*/
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
/*
void ticks2speed(){
	//Execution with 100Hz
	//Wheelspeed Pre determination
	//weighting der letzten 3 werte für die überschlägige berechnung des Wheelspeed
	double average_time_diff = ((double)timediff[0]*0.5+(double)timediff[1]*0.3+(double)timediff[2]*0.2);
	double angular_velocity = ((double)average_time_diff/10)/(double)trigger_angle;
	angular_velocity = pow((double)angular_velocity,-1);
	double RPS = ((double)angular_velocity/360)*1000; //°/ms-> Rev/s
	
	//High accuracy Wheelspeed
	unsigned long int timediffinterval = 0;
	
	double ideal_measurement_angle = (((double)RPS * 360) / desired_update_frequency);
	double interval_width = ((double)ideal_measurement_angle / trigger_angle);
	double possible_measurement_angle = (double)(lrint(interval_width) * trigger_angle); //lrint rounds to nearest Int
	if (possible_measurement_angle == 0){
		possible_measurement_angle = trigger_angle;
	}
	for (uint8_t i = 0  ;i <= (possible_measurement_angle/trigger_angle)-1;i++){
		timediffinterval += (double)timediff[i];
	}
	double RPS_Float = ((pow((((double)timediffinterval/10.0)/(double)possible_measurement_angle),-1.0))/360.0)*1000.0;
	Wheelspeed_kmh_float = (((double)RPS_Float * (Tcirc / 1000.0))*100.0*3.6);
	
	//setting boundaries for max & min speed
	if (Wheelspeed_kmh_float >= 15000){
		Wheelspeed_kmh_float = 15000;
		
	}else if (Wheelspeed_kmh_float<=300){
		Wheelspeed_kmh_float = 0;
	}
}
*/
	//Wheelspeed Low Speed
void Lowspeed_WS(){
	
	double angular_velocity = ((double)timestepdiff/10.0)/(double)trigger_angle;
	angular_velocity = pow((double)angular_velocity,-1.0);
	double RPS = ((double)angular_velocity/360)*1000.0; //°/ms-> Rev/s	
	Wheelspeed_kmh = (((double)RPS * (Tcirc / 1000.0))*100.0*3.6);
	
}


	
	//Wheelspeed High Speed	
void Highspeed_WS(){
	double RPS = ((double)steps_per_intervall/update_frequency_simple_ms)*1000.0;
	RPS = (double)RPS*(1.0/number_of_teeth);
	Wheelspeed_kmh = (((double)RPS * (Tcirc / 1000.0))*100.0*3.6);
	
}














/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

uint8_t SPIcount = 0;

volatile uint16_t sys_tick = 0;
volatile double timestepdiff = 0;
volatile uint16_t steps_per_intervall = 0;
volatile uint16_t Wheelspeed_kmh = 0;

uint8_t Highspeed_active = 0;

#define TRUE 1


volatile uint8_t PB5_state_old = 0;
volatile uint8_t cs_state_old = 1;
volatile uint16_t ticks = 0;

#define trigger_angle 11.25//Trigger Angle in degree both high & low are the same
#define Tcirc 1476.5485 //Tire circumference in mm
#define desired_update_frequency 100//Update frequency for the floating calculation of the Wheelspeed
#define update_frequency_simple_ms 50
#define number_of_teeth 32

void sys_timer_config();
void SPI_USI_config();
void ticks2speed();
void Lowspeed_WS();
void Highspeed_WS();

int main(void)
{
	cli();
	sys_timer_config();
	SPI_USI_config();
	sei();
	while (1){
		
	}
}

void sys_timer_config(){
	//For every Compare Match of timer 0 we want to increment Timer 1 by 1 without generating a software interrupt
	//Timer 1 needs to be set up so that we can measure more than 10ms with it
	//Timer 0 and 1 are used as a 16bit timer which is reset evbbery 10ms by an incoming SPI transmit
	
	//set Timer 1 to 15,625khz
	TCCR1 = 0 | (1<<CTC1)| (1<<CS10) | (1<<CS11) | (1<<CS13); //Prescaler 1024
	TIMSK |= (1<<OCIE1A);
	OCR1C = 255 - 1;
	
	//Set Timer 0 to 2Mhz
	TCCR0A = 0 | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0);
	TCCR0B = 0 | (1<<CS01); //Prescaler 8
	TIMSK |= (1<<OCIE0A) | (1<<TOIE0); //compare interrupt enable
	OCR0A = 128-1; //This Plus Prescaler gives a Freq of 15,625Khz
}

ISR(TIM0_COMPA_vect){
	TCNT0 = 0;
}

ISR(TIM1_COMPA_vect){
	TCNT1 = 0;
}

ISR(USI_OVF_vect){
	USIDR = 10000 >> 8;
	//USIDR = Wheelspeed_kmh >> 8;	//Write LSB into Buffer for send to Master
	USISR = (1 << USIOIF);// Clear Overflow bit
	//Reset Timers to 0 for next sensor rythm
	TCNT0 = 0;
	TCNT1 = 0;
}

ISR(PCINT0_vect){ //Pin Change Interrupt on Chip Select
	if((PINB & (1 << PB0)) != cs_state_old){//If the status of the cs pin has changed either we want to activate or deactivate the spi comm
		cs_state_old = (PINB & (1 << PB0));	
		if((PINB & (1 << PB0))== 0){	// If cs is low & the status of the pin has changed, the SPI Comm should be activated
			//USIDR = Wheelspeed_kmh & 0xff;//Write MSB into Buffer for send to Master
			USIDR = 10000 & 0xff;
			DDRB |= (1 << DDB1);		//Configure MISO as Output
			USISR = (1 << USIOIF);      // Clear Overflow bit
			USICR |= (1 << USIOIE);		// 4-bit overflow counter of the USI communication shall be activated
		}else{ // If cs is high & the status of the pin has changed, turn the 4-bit overflow interrupt off:
			USICR &= ~(1 << USIOIE);
			DDRB &= ~(1 << DDB1);		//Configure as Hi Z so MISO does not influence other data traffic
			PORTB &= ~(1 << PB1);		//Configure as Hi Z so MISO does not influence other data traffic
		}
	}else{	//if the value of cs hasnt changed the value of digi in must have changed
		//systime Build
		sys_tick = TCNT0+TCNT1*128;
		timestepdiff = 1.0/(2.0*1000)*sys_tick; //calculates the time from edge to edge in 0,1ms
		steps_per_intervall++;
	}
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
	
	double angular_velocity = ((double)timestepdiff)/(double)trigger_angle;
	angular_velocity = pow((double)angular_velocity,-1.0);
	double RPS = ((double)angular_velocity/360)*1000.0; //°/ms-> Rev/s	
	Wheelspeed_kmh = (((double)RPS * (Tcirc / 1000.0))*100.0*3.6);
	if (Wheelspeed_kmh >= 13000){
		Wheelspeed_kmh = 13000;
	}
	
}
	//Wheelspeed High Speed	
void Highspeed_WS(){
	double RPS = ((double)steps_per_intervall/update_frequency_simple_ms)*1000.0;
	RPS = (double)RPS*(1.0/number_of_teeth);
	Wheelspeed_kmh = (((double)RPS * (Tcirc / 1000.0))*100.0*3.6);
	if (Wheelspeed_kmh >= 13000){
		Wheelspeed_kmh = 13000;
	}	
}
*/