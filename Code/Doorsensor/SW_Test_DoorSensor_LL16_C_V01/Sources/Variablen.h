#include <stdtypes.h>

#pragma DATA_SEG _DATA_ZEROPAGE
	extern Byte 		RealTimeFlag;
	extern Byte		RealTimeCounter;
	extern Byte 		RunLedCounter;
	extern Byte			DoorSensorState;
	extern Byte			DoorSensorArr[5];
	extern Byte			sendPacketArr[20];
