/*
 * main.c
 * for the ATtiny85
 * Created: 18.02.2021 18:20:05
 * Author : Eric Gorkow & Lukas Deeken
 */

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t sys_time_10 = 0;
uint8_t SPIcount = 0;

volatile unsigned long int sys_time = 0;
volatile uint16_t timestepdiff = 0;

volatile uint8_t PB5_state_old = 0;
volatile uint8_t cs_state_old = 0;
volatile uint16_t ticks = 0;



void sys_timer_config();
void sys_tick();
void SPI_USI_config();

int main(void)
{
	sys_timer_config();
	SPI_USI_config();
	sei();
	while (1){}
}


void sys_timer_config(){
	TCCR0A = 0 | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0);
	TCCR0B = 0| (0<<CS02) | (1<<CS01) | (1<<CS00);
	TIMSK |= (1<<OCIE0A) | (1<<TOIE0); //compare interrupt enable
	OCR0A = 250-1; // compare value for 0,1ms
}

ISR(TIM0_COMPA_vect){
	sys_time++; //system time generation
}

ISR(USI_OVF_vect){
	USIDR = timestepdiff >> 8;	//Write LSB into Buffer for send to Master
	USISR = (1 << USIOIF);// Clear Overflow bit
}

ISR(PCINT0_vect){ //Pin Change Interrupt on Chip Select
	if((PINB & (1 << PB0)) != cs_state_old){//If the status of the cs pin has changed either we want to activate or deactivate the spi comm
		cs_state_old = (PINB & (1 << PB0));	
		if((PINB & (1 << PB0))== 0){	// If cs is low & the status of the pin has changed, the SPI Comm should be activated
			USIDR = timestepdiff & 0xff;			//Write MSB into Buffer for send to Master
			DDRB |= (1 << DDB1);		//Configure MISO as Output
			USISR = (1 << USIOIF);      // Clear Overflow bit
			USICR |= (1 << USIOIE);		// 4-bit overflow counter of the USI communication shall be activated
		}else{ // If cs is high & the status of the pin has changed, turn the 4-bit overflow interrupt off:
			USICR &= ~(1 << USIOIE);
			DDRB &= ~(1 << DDB1);		//Configure as Hi Z so MISO does not influence other data traffic
			PORTB &= ~(1 << PB1);		//Configure as Hi Z so MISO does not influence other data traffic
		}
	}else{	//if the value of cs hasnt changed the value of digi in must have changed
		timestepdiff = sys_time; //calculates the time from edge to edge in 0,1ms
		sys_time = 0;
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

/*
unsigned long int sys_time_old = 0;
unsigned long int sys_time = 0;

volatile uint16_t spi_transmissions = 0;//for debugging
volatile uint8_t PB5_state_old = 0;
volatile uint8_t cs_state_old = 0;
volatile uint16_t ticks = 0;

volatile unsigned long int timestepdiff = 0;
volatile float timediff[10];
volatile uint16_t Wheelspeed = 0; //in km/h
volatile float RPS = 0; //Wheel Revolutions per second
volatile float ideal_measurement_angle = 0;
volatile float possible_measurement_angle = 0;
volatile uint8_t interval_width = 0;
volatile float timediffinterval = 0;
volatile uint8_t n = 9 ; //Durchlaufvariable für timestepdiff mit den letzten 10 werten

//Volatile:	 Variable can change within an interrupt
//long:		 32Bit
//Unsigned:	 only positive numbers are allowed

#define trigger_angle 11.25//Trigger Angle in degree both high & low are the same
#define Tcirc 1476.5485 //Tire circumference in mm
#define desired_update_frequency 100//Update frequency for the floating calculation of the Wheelspeed

		timestepdiff = sys_time - sys_time_old;
		sys_time_old = sys_time;
		
		if(n>=1){
			timestepdiff = sys_time - sys_time_old;
			timediff[n] = timestepdiff / 10; //timediff according to 10Khz clock so we divide timesteps by 10 to get timediff in ms
			sys_time_old = sys_time;
		n--;
		}else{
			for (uint8_t i = 9; i <= 1;i--){
				timediff[i]=timediff[i+1];
			}
			timediff[0] = sys_time - sys_time_old; //Timer overflow? Timer Resolution in contrast to angular speed
		}
		
			ideal_measurement_angle = RPS * 360 / desired_update_frequency;
			interval_width = ideal_measurement_angle / trigger_angle;
			possible_measurement_angle = interval_width * trigger_angle;
			if (possible_measurement_angle == 0){
				possible_measurement_angle = trigger_angle;
			}
			for (uint8_t i = interval_width ; i <= 1;i--){
				timediffinterval += timediff[i];
			}
			RPS = 1 / ((timediffinterval / interval_width) * (360 / 1000));
			Wheelspeed = RPS * (Tcirc / 1000*1000) * 60*60;
			SPIdata[0]= Wheelspeed << 8; //MSB
			SPIdata[1] = Wheelspeed & 0xff;	//LSB
			*/