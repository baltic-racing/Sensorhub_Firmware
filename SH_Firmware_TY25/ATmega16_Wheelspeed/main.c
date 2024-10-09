/*
 * ATmega16_Wheelspeed.c
 *
 * Created: 09.10.2024 18:21:32
 * Author : racin
 */ 

#include <avr/io.h>

#include "SPI.h"


int main(void)
{
	SPI_SlaveInit();
	uint8_t data = 0;
    /* Replace with your application code */
    while (1) 
    {
		data = SPI_SlaveReceive();
    }
}

