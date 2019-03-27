/*
 * SH_100.c
 *
 * Created: 27.03.2019 11:36:06
 * Author : Ole Hannemann
 */ 

#include <avr/io.h>
#include "misc_functions.h"
#include "canlib.h"

int main(void)
{
    port_config();
	adc_config();
	spi_config();
	timer_config();
    while (1) 
    {
    }
}

