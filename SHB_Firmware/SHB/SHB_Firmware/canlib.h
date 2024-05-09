//*	INCLUDES	*/
#include <avr/io.h>


#ifndef CAN_LIB_H_
#define CAN_LIB_H_

struct CAN_MOB{
	
	uint16_t mob_id;
	uint16_t mob_idmask;
	uint16_t mob_number;
	
};


uint8_t can_check_free(uint8_t);
void can_cfg();
void can_rx(struct CAN_MOB *to_receive , uint8_t *data);
void can_tx(struct CAN_MOB *to_send, uint8_t *data);


#endif