/*
 * SHR_Firmware.c
 *
 * Created: 28.04.2023 17:35:31
 * Author : nico
 */ 

/*	INCLUDES	*/

#include <avr/io.h>
#include "system_config.h"

/*	Init Global Variables	*/
unsigned long sys_tick = 0;

int main(void)
{
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	
    while (1) 
    {
		
    }
}

