/*
 * main.c
 * for the ATtiny85
 * Created: 18.02.2021 18:20:05
 * Author : Eric Gorkow & Lukas Deeken
 */

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t sys_time_10 = 0;
	
unsigned long int time_old = 0;
unsigned long int sys_time = 0;

void sys_timer_config();
void sys_tick();

int main(void)
{
    sys_timer_config();
	DDRB |= (1 << PB0);
	sys_tick();
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
			sys_tick();				//blink status LED
			sys_time_10 = 0;		//reset 50 ms timer
		}	
    }
}

void sys_tick(){
	// switch Status LED state
	PORTB ^= 1<<PB0;
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