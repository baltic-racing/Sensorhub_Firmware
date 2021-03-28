/*
 * SPI.h
 *
 * Created: 25.03.2021 17:23:42
 *  Author: lukas
 */ 

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <avr/interrupt.h>

//creating Prototypes of the functions used in the SPI.c
void SPI_config();
void TYPK_read();
uint16_t TYPK_getdata1();
uint16_t TYPK_getdata2();
ISR(SPI_STC_vect);

#endif // SPI_H 
