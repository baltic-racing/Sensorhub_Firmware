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
uint16_t Ticks = 0;

unsigned long int time_old = 0;
unsigned long int sys_time = 0;
//volatile unsigned long int timestepdiff = 0;
//volatile unsigned long int timediff[10] = 0;
volatile unsigned long int sys_time_old = 0;
volatile uint8_t PB5_state_old = 0;
//volatile uint16_t Wheelspeed  = 0; //in km/h
//volatile float RPS = 0; //Wheel Revolutions per second
//volatile float ideal_measurement_angle = 0;
//volatile float possible_measurement_angle = 0;
//volatile uint8_t interval_width = 0;
//volatile float timediffinterval = 0;
//volatile uint8_t n = 9 ; //Durchlaufvariable für timestepdiff mit den letzten 10 werten
//Volatile:	 Variable can change within an interrupt
//long:		 32Bit
//Unsigned:	 only positive numbers are allowed

//#define trigger_angle = 11.25 ; //Trigger Angle in degree both high & low are the same
//#define Tcirc = 1476.5485 ; //Tire circumference in mm
//#define desired_update_frequency = 100; //Update frequency for the floating calculation of the Wheelspeed

void sys_timer_config();
void SPI_USI_config();
void PORT_Config();
//void calc_wheelspeed_floating();

int main(void)
{
    sys_timer_config();
	SPI_USI_config();
	PORT_Config();
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

void sys_timer_config(){
	TCCR0A = 0 | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0);
	TCCR0B = 0| (0<<CS02) | (1<<CS01) | (0<<CS00);
	TIMSK |= (1<<OCIE0A) | (1<<TOIE0); //compare interrupt enable
	OCR0A = 100-1; // compare value for 10Khz
	//Systime Calc 8Mhz Qaurtz
	//Prescaler = 8 => 1 Mhz
	// Goal 10 Khz set OCR0A Value accordingly to 100
}

ISR(TIM0_COMPA_vect){
	sys_time++; //system time generation
}

ISR(USI_OVF_vect){
	USIDR = 5;	//Write LSB into Buffer for send to Master
	USISR = (1 << USIOIF);;// Clear Overflow bit
}

ISR(PCINT0_vect){ //Pin Change Interrupt on Chip Select
	if((PINB & (1 << PB0)) == 0){	// If edge is falling, the SPI Comm should be activated
		USIDR = 3;			//Write MSB into Buffer for send to Master
		DDRB |= (1 << DDB1);		//Configure MISO as Output
		USISR = (1 << USIOIF);      // Clear Overflow bit
		USICR |= (1 << USIOIE);		// 4-bit overflow counter of the USI communication shall be activated
	}
	else if((PINB & (1 << PB0)) == 1){ // If edge is rising, turn the 4-bit overflow interrupt off:
		USICR &= ~(1 << USIOIE);
		DDRB &= ~(1 << DDB1);		//Configure as Hi Z so MISO does not influence other data traffic
		PORTB &= ~(1 << PB1);		//Configure as Hi Z so MISO does not influence other data traffic		
	}else if ((PINB & (1 << PB5)) != PB5_state_old){
		PB5_state_old = (1 << PB5);
		Ticks++; //Increments this variable for every pin change on PB5 (Wheelspeed trigger)
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
}

void PORT_Config(){
	PCMSK = (1 << PCINT5);					//enable Pin change Interrupt on Digital in pin PB5
	}

ISR(PCINT5){
	if(n>=1){
		timestepdiff = sys_time - sys_time_old;
		timediff[n] = timestepdiff / 10; //timediff according to 10Khz clock so we divide timesteps by 10 to get timediff in ms
		sys_time_old = sys_time;
		n--;
	}
	else{
		for (uint8_t i= 9; i <= 1)
		{
			timestepdiff[i]=timestepdiff[i+1];
			i--
		}
		timestepdiff[0] = sys_time - sys_time_old; //Timer overflow? Timer Resolution in contrast to angular speed
	}
	calc_wheelspeed_floating()
	
}



void calc_wheelspeed_floating(){


	//calculate ideal Measurement Angle
	//derive real possible measurement angle from that
	//then integrate the time over the desired interval
	//calculate wheelspeed from that
	ideal_measurement_angle = RPS * 360 / desired_update_frequency;
	interval_width = ideal_measurement_angle / trigger_angle;
	possible_measurement_angle = interval_width * trigger_angle;
	if (possible_measurement_angle == 0){
		possible_measurement_angle = trigger_angle;
	}
	for (uint8_t i=interval_width;i<=1){
		timediffinterval += timediff[i];
	}
	RPS = 1 / ((timediffinterval / interval_width) * (360 / 1000));
	Wheelspeed = RPS * (Tcirc / 1000^2) * 60^2;
	SPIdata[0]= Wheelspeed << 8; //MSB
	SPIdata[1] = Wheelspeed & 0xff;	//LSB
}
