/*
 * main.h
 *
 * Created: 13.10.2024 12:09:30
 *  Author: Egquus
 */ 

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "calc_wheelspeed.h"
#include "misc_Functions.h"
#include "SPI_lib.h"


//	Variables
extern volatile unsigned long sys_time;
extern volatile unsigned long time_delta_left;
extern volatile unsigned long time_delta_right;

extern volatile uint8_t wheelspeed;

volatile unsigned long time_1ms = 0;
volatile unsigned long time_10ms = 0;
volatile unsigned long time_100ms = 0;
volatile unsigned long time_200ms = 0;
volatile unsigned long time_1000ms = 0;

//	Macros
#define TIME_PASSED_1_MS	(sys_time - time_1ms) >= 10
#define TIME_PASSED_10_MS	(sys_time - time_10ms) >= 100
#define TIME_PASSED_100_MS	(sys_time - time_100ms) >= 1000
#define TIME_PASSED_200_MS	(sys_time - time_200ms) >= 2000
#define TIME_PASSED_1000_MS	(sys_time - time_1000ms) >= 10000

#endif /* MAIN_H_ */