#include "FunctionProto.h"
#include "defines.h"
#include "Variablen.h"
#include "inputToCommand.h"

#define PCB				PCB_MOD_Y
#define MODULE_CURRENT	SENSOR_CURRENT

//************************************************************************************************
//*                                                                                              *
//*     CurrentSensor_PreparePacket: Prepares an 8 Byte array	                                 *
//*		Contains the ADC Value of the Current Sensor											 *
//*                                                                                              *
//************************************************************************************************
void CurrentSensor_PreparePacket(void)
{
	CC2500_sendPacketArr[0] = 8;
	CC2500_sendPacketArr[1] = PCB << 4;
	CC2500_sendPacketArr[1] += MODULE_CURRENT;
	CC2500_sendPacketArr[2] = (byte)(CurrentSensor1 >> 8);
	CC2500_sendPacketArr[3] = CurrentSensor1 & 0x00FF;
	CC2500_sendPacketArr[4] = (byte)(CurrentSensor2 >> 8);
	CC2500_sendPacketArr[5] = CurrentSensor2 & 0x00FF;
	CC2500_sendPacketArr[6] = (byte)(CurrentSensor3 >> 8);
	CC2500_sendPacketArr[7] = CurrentSensor3 & 0x00FF;
}