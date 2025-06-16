/*
 * CAN_data.c
 *
 * Created: 31.03.2025 14:41:09
 *  Author: pasca
 */ 


#include "CAN_data.h"

uint8_t SensorHubB0_databytes[8];
uint8_t SensorHubB1_databytes[8];
uint8_t SensorHubB2_databytes[8];

struct CAN_MOB can_SensorHubB0_mob;
struct CAN_MOB can_SensorHubB1_mob;
struct CAN_MOB can_SensorHubB2_mob;

CAN_Init_Messages(){
	
	can_SensorHubB0_mob.mob_id = 0x410;
	can_SensorHubB0_mob.mob_idmask = 0xFFFF; //sent
	can_SensorHubB0_mob.mob_number = 0;

	can_SensorHubB1_mob.mob_id = 0x411;
	can_SensorHubB1_mob.mob_idmask = 0xFFFF; //sent
	can_SensorHubB1_mob.mob_number = 1;
	
	can_SensorHubB2_mob.mob_id = 0x412;
	can_SensorHubB2_mob.mob_idmask = 0xFFFF; //sent
	can_SensorHubB2_mob.mob_number = 2;
	
}