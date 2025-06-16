/*
 * SPI_lib.h
 *
 * Created: 31.03.2025 14:45:53
 *  Author: pasca
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef SPI_LIB_H_
#define SPI_LIB_H_

#define MOSI PB2							//Start: individual configuration
#define MISO PB3
#define SCK PB1
#define SS_uC PE4
#define SS_TK1 PE0
#define SS_TK2 PE1							
#define SS_TK3 PE2
#define SS_TK4 PE3							//End

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
	#define SPI_Interrupt_Flag SPIF
	#define SPI_Collision_Flag WCOL
	#define SPI_Double_Speed
	
#define SPI_Data_Reg SPDR
	
#define SS_uC_LOW() (PORTE &= ~(1 << SS_uC))
#define SS_uC_HIGH() (PORTE |= (1 << SS_uC))	
#define SS_TK1_LOW() (PORTE &= ~(1 << SS_TK1))
#define SS_TK1_HIGH() (PORTE |= (1 << SS_TK1))
#define SS_TK2_LOW() (PORTE &= ~(1 << SS_TK2))
#define SS_TK2_HIGH() (PORTE |= (1 << SS_TK2))
#define SS_TK3_LOW() (PORTE &= ~(1 << SS_TK3))
#define SS_TK3_HIGH() (PORTE |= (1 << SS_TK3))
#define SS_TK4_LOW() (PORTE &= ~(1 << SS_TK4))
#define SS_TK4_HIGH() (PORTE |= (1 << SS_TK4))
#define LEFT 0x01
#define RIGHT 0x03

void SPI_MasterInit();
void SPI_SlaveInit(void);
char SPI_SlaveReceive(void);
uint8_t SPI_transfer(uint8_t data);
uint16_t TYPK_getdata();


#endif /* SPI_H_ */

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