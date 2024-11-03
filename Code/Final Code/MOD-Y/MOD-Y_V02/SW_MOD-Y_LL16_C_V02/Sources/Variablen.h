#include <stdtypes.h>

#pragma DATA_SEG _DATA_ZEROPAGE
	extern Byte 		RealTimeFlag;
	extern Byte			RealTimeCounter;
	extern Byte 		RunLedCounter;
	
	extern Byte			ADCPort;
	extern int 			CurrentSensor1;
	extern int 			CurrentSensor2;
	extern int 			CurrentSensor3;
	extern Byte			sendPacketArr[20];
	
	extern int			RelaiseState;
					
