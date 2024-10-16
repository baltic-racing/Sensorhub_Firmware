/*
 * SPI.h
 *
 * Created: 09.10.2024 18:21:59
 *  Author: Egquus
 */ 

#include <avr/io.h>

#include "calc_wheelspeed.h"

#ifndef SPI_LIB_H_
#define SPI_LIB_H_

#define MOSI PB5							//Start: individual configuration
#define MISO PB6
#define SCK PB7
#define SS_uC PB4							//End

#define SPI_Control_Reg SPCR
#define SPI_Interrupt_Enable SPIE
#define SPI_Enable SPE
#define SPI_Data_Order DORD
#define SPI_Master MSTR
#define SPI_Clock_Polarity CPOL
#define SPI_Clock_Phase CPHA
#define SPI_Relation0 SPR0				// Attention, this also affects the clock frequency [see Table SPI_1]
#define SPI_Relation1 SPR1				// Attention, this also affects the clock frequency [see Table SPI_1]

#define SPI_Status_Reg SPSR
#define SPI_Interrupft_Flag SPIF
#define SPI_Collision_Flag WCOL
#define SPI_Double_Speed SPI2X			// Attention, this also affects the clock frequency [see Table SPI_1]
	
#define SPI_Data_Reg SPDR

void SPI_MasterInit();
void SPI_SlaveInit(void);
char SPI_SlaveReceive(void);


#endif /* SPI_H_ */

//SPCR -> SPI Control Register | SPIE | SPE | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0
//SPIE ->
/*  _________________________________________________________
	H	SPI2X	H	SPR1	H	SPR0	H	SCK_Frequency	H
	H___________H___________H___________H___________________H
	H	0		H	0		H	0		H	f_clkio/4		H
	H	0		H	0		H	1		H	f_clkio/16		H
	H	0		H	1		H	0		H	f_clkio/64		H
	H	0		H	1		H	1		H	f_clkio/128		H
	H	1		H	0		H	0		H	f_clkio/2		H
	H	1		H	0		H	1		H	f_clkio/8		H
	H	1		H	1		H	0		H	f_clkio/32		H
	H	1		H	1		H	1		H	f_clkio/64		H
	_________________________________________________________
	Table SPI_1												*/