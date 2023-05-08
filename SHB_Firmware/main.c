/*
 * SHB_Firmware.c
 *
 * Created: 08.05.2023 20:00:36
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