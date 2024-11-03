#include <stdtypes.h>

#pragma DATA_SEG _DATA_ZEROPAGE
	extern Byte 		RealTimeFlag;
	extern Byte			RealTimeCounter;
	extern Byte 		RunLedCounter;
	extern int			WaterSensor_ADCValue;
	extern Byte			sendPacketArr[20];
