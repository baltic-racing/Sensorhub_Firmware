/*
 * CAN_data.c
 *
 * Created: 28.10.2024 18:01:14
 *  Author: Egquus
 */ 

#include "CAN_data.h"

uint8_t SensorHub0_databytes[8];

struct CAN_MOB can_SensorHub0_mob;

CAN_Init_Messages(){
	
	can_SensorHub0_mob.mob_id = 0x490;
	can_SensorHub0_mob.mob_idmask = 0xFFFF; //sent
	can_SensorHub0_mob.mob_number = 0;

}