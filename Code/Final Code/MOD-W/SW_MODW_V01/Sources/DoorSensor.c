#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h"
#include "inputToCommand.h"

#define DOORSENSOR_PORT		PTDD_PTDD0
#define DOORSENSOR_DIR		PTDDD_PTDDD0

#define PCB				PCB_MOD_W10


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
	CC2500_sendPacketArr[0] = 3;
	CC2500_sendPacketArr[1] = PCB << 4;
	CC2500_sendPacketArr[1] += SENSOR_DOOR;
	CC2500_sendPacketArr[2] = DoorSensor_GetState();
}

//************************************************************************************************
//*                                                                                              *
//*     DoorSensor_GetState: returns the state of the DoorSensor                                 *
//*		return 1 for door closed and 0 for door open											 *
//*                                                                                              *
//************************************************************************************************
unsigned char DoorSensor_GetState(void)
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