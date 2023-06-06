/*
 * canlib.c
 *
 * Created: 27.03.2019 11:47:50
 *  Author: Ole Hannemann
 */ 

#include "canlib.h"
#include <avr/io.h>


uint8_t can_check_free(uint8_t mobnum){
	
	uint8_t mob_status = 0;
	
	if(mobnum >7){
		
		mob_status = !((CANEN1 >> (mobnum-8)) &1);


		} else {
		
		mob_status = !((CANEN2 >> mobnum) &1);
	}

	return mob_status;
	
}
void can_cfg(){
	
	CANGCON = 0; // Disable CAN
	
	for (uint8_t mob = 0; mob < 15 ; mob++){//reset all mobs
		CANPAGE = mob<<MOBNB0 | (1<<AINC);
		CANSTMOB = 0;
		CANCDMOB = 0;
		CANIDT4 = 0;
		CANIDT3 = 0;
		CANIDT2 = 0;
		CANIDT1 = 0;
		CANIDM4 = 0;
		CANIDM3 = 0;
		CANIDM2 = 0;
		CANIDM1 = 0;
		for (uint8_t byte = 0; byte < 8; byte++){
			CANPAGE = mob<<MOBNB0 | 1<<AINC | byte;
			CANMSG = 0;
		}
	}

	CANBT1 = 0x02;// Set Baudrate
	CANBT2 = 0x0C;// 500kBaud according
	CANBT3 = 0x37;// to Datasheet S. 267 with 16Mhz Oszi

	CANGIE = 0;

	CANGCON |= (1<<ENASTB); // Enable CAN
	
	
	
}
void can_rx(struct CAN_MOB *to_receive, uint8_t *data){
	
	CANPAGE = to_receive->mob_number << MOBNB0;
	if (can_check_free(to_receive->mob_number)){
		/* load the id 11 bit */
		CANIDT1 = to_receive->mob_id >>3;
		CANIDT2 = (to_receive->mob_id << 5)&0b11100000;
		CANIDT3 = 0;
		CANIDT4 = 1<<RTRTAG;
		CANIDM1 = to_receive->mob_idmask >>3;
		CANIDM2 = (to_receive->mob_idmask <<5)&0b11100000;
		CANIDM3 = 0;
		CANIDM4 = 0;
		CANCDMOB = (1 << CONMOB1) | (1 << CONMOB0)| (1<<DLC3);
		CANSTMOB = 0;
	}
	
	for(uint8_t byte = 0; byte <8; byte++){
		CANPAGE = (to_receive->mob_number << MOBNB0) | (1 << AINC) | byte;
		data[byte] = CANMSG;
	}
	
	
}
void can_tx(struct CAN_MOB *to_send, uint8_t *data){
	
	for(uint8_t byte = 0; byte <8; byte++){
		CANPAGE = (to_send->mob_number << MOBNB0) | (1 << AINC) | byte;
		CANMSG = data[byte];
	}
	
	CANPAGE = to_send->mob_number << MOBNB0;
	if (can_check_free(to_send->mob_number)){
		CANSTMOB = 0;
		CANIDT1 = to_send->mob_id>>3;
		CANIDT2 = (to_send->mob_id << 5) & 0b11100000;
		CANIDT3 = 0;
		CANIDT4 = 0;
		CANIDM1 = 0; //0b11111111;
		CANIDM2 = 0; //0b11100000;
		CANIDM3 = 0;
		CANIDM4 = 0;
		CANSTMOB = 0;
		CANCDMOB = (1<<CONMOB0) | 1 << DLC3;
		CANSTMOB = 0;
	}
	
}
