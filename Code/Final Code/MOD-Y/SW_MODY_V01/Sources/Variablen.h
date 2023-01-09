#include <stdtypes.h>

#pragma DATA_SEG _DATA_ZEROPAGE
	extern Byte 		RealTimeFlag;
	extern Byte			RealTimeCounter;
	extern Byte 		RunLedCounter;
	extern Byte			sendInterval;
	extern Byte			send;
	
	//adc and currentSensor
	extern Byte			ADCPort;
	extern int 			CurrentSensor1;
	extern int 			CurrentSensor2;
	extern int 			CurrentSensor3;
	
	//cc2500
	extern Byte			CC2500_sendPacketArr[50];
	extern Byte			CC2500_recvPacketArr[10];
	extern Byte			CC2500_packetReceived;
	extern int 			i;
	
	//relaise
	extern int			RelaiseState;
					
