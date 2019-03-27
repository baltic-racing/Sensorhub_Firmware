/*
 * can_lib.h
 *
 * Created: 27.03.2019 11:45:02
 *  Author: Ole Hannemann
 */ 

#include <avr/io.h>

#ifndef CAN_LIB_H_
#define CAN_LIB_H_

struct CAN_MOB{
	
	uint8_t data_bytes[8];
	uint16_t mob_id;
	uint16_t mob_idmask;
	uint8_t mob_number;
	
};


uint8_t can_check_free(uint8_t mobnum);
void can_cfg();
void can_rx(struct CAN_MOB *to_receive);
void can_tx(struct CAN_MOB *to_send);


#endif /* CAN_LIB_H_ */