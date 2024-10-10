/*
 * misc_functions.c
 *
 * Created: 10.10.2024 16:22:24
 *  Author: Egquus
 */ 

#include "misc.functions.h"






ISR(TIMER0_COMP_vect)
{
	sys_time++
}