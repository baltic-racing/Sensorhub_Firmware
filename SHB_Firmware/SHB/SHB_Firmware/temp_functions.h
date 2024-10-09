/*
 * temp_functions.h
 *
 * Created: 08.10.2024 13:57:05
 *  Author: Egquus
 */ 

#ifdef TEMP_FUNCTIONS_H_
#define TEMP_FUNCTIONS_H_

uint8_t temp_counter = 1;
uint8_t temp;

uint16_t tnp_temp;

void temp_switch(uint8_t temp);

uint16_t temp_rdy;

#endif /* TEMP_FUNCTIONS_H_ */