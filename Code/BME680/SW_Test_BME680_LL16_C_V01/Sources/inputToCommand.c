#include "FunctionProto.h"
#include "Variablen.h"
#include "defines.h"
#include "inputToCommand.h"


void inputToCommand(void)
{
	if(CC2500_recvPacketArr[1] >> 4 == PCB_MOD_X1)
	{
		switch(CC2500_recvPacketArr[1] & 0xF)
		{
			case SENSOR_DOOR: 
				break;
				
			case SENSOR_TEMP: 
				break;
				
			case SENSOR_HUMIDITY: 
				break;
				
			case SENSOR_WATER: 
				break;
				
			case SENSOR_CURRENT: 
				break;
				
			case SENSOR_RELAISE: 
				break;
				
			default:
				break;
		}
	}
}