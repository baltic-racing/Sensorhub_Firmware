/*
 * SPI.h
 *
 * Created: 09.10.2024 18:21:59
 *  Author: racin
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7

void SPI_SlaveInit(void);
char SPI_SlaveReceive(void);




#endif /* SPI_H_ */