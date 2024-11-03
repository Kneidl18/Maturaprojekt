#include "FunctionProto.h"
#include "defines.h"
#include "Variablen.h"

#define PCB		0b0000
#define MODULE_CURRENT	0b0101

void CurrentSensor_PreparePacket(void)
{
	sendPacketArr[0] = 8;
	sendPacketArr[1] = PCB << 4;
	sendPacketArr[1] += MODULE_CURRENT;
	sendPacketArr[2] = (byte)(CurrentSensor1 >> 8);
	sendPacketArr[3] = CurrentSensor1 & 0x00FF;
	sendPacketArr[4] = (byte)(CurrentSensor2 >> 8);
	sendPacketArr[5] = CurrentSensor2 & 0x00FF;
	sendPacketArr[6] = (byte)(CurrentSensor3 >> 8);
	sendPacketArr[7] = CurrentSensor3 & 0x00FF;
}