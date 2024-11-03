#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h"

#define DOORSENSOR_PORT		PTDD_PTDD0
#define DOORSENSOR_DIR		PTDDD_PTDDD0

#define PCB			0b0100
#define MODULE_DOOR		0b0001


//************************************************************************************************
//*                                                                                              *
//*     DoorSensor_Init: initializing the port for the DoorSensor                                *
//*		DOORSENSOR_DIR (PORTD0) = input															 *
//*                                                                                              *
//************************************************************************************************
void DoorSensor_Init(void)
{
	DOORSENSOR_DIR = 0;
}

//************************************************************************************************
//*                                                                                              *
//*     DoorSensor_PreparePacket: prepares the packet for the cc2500                             *
//*                                                                                              *
//************************************************************************************************
void DoorSensor_PreparePacket(void)
{
	sendPacketArr[0] = 3;
	sendPacketArr[1] = PCB << 4;
	sendPacketArr[1] += MODULE_DOOR;
	sendPacketArr[2] = DoorSensor_GetState();
}

//************************************************************************************************
//*                                                                                              *
//*     DoorSensor_GetState: returns the state of the DoorSensor                                 *
//*		return 1 for door closed and 0 for door open											 *
//*                                                                                              *
//************************************************************************************************
byte DoorSensor_GetState(void)
{
	return DoorSensorState;	
}

//************************************************************************************************
//*                                                                                              *
//*     DoorSensor_UpdateState: checks the past 5 states of the door                             *
//*		if all states were closed then the state is being closed								 *
//*                                                                                              *
//************************************************************************************************
void DoorSensor_UpdateState(void)
{
	uchar i;
	DoorSensorState = 1;
	for(i = 0; i < 5; i++)
	{
		if(DoorSensorArr[i] == 0)
		{
			DoorSensorState = 0;
		}
	}
}

//************************************************************************************************
//*                                                                                              *
//*     DoorSensor_CheckState: reads the port value (0, 1) and writes it into the array          *
//*                                                                                              *
//************************************************************************************************
void DoorSensor_CheckState(void)
{
	uchar i;
	for(i = 4; i > 0; --i)
	{
		DoorSensorArr[i] = DoorSensorArr[i-1];
	}
	
	DoorSensorArr[0] = DOORSENSOR_PORT;
}