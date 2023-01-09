#include <stdtypes.h>

#pragma DATA_SEG _DATA_ZEROPAGE
	extern Byte 		RealTimeFlag;
	extern Byte			RealTimeCounter;
	extern Byte 		RunLedCounter;
	
	//modw
	extern Byte			DoorOpen;
	extern Byte			ButtonPressed;
	extern Byte			timerRanOut;
	extern Byte			firstRound;
	extern uint			delayDoorSend;
	
	//cc2500
	extern char 		CC2500_recvPacketArr[20];
	extern char			CC2500_sendPacketArr[20];
	extern unsigned int	CC2500_sendInterval;
	extern uint			i;		
	extern Byte			CC2500_packetReceived;
	extern unsigned char crc;		
	
	//DoorSensor
	extern Byte			DoorSensorState;
	extern Byte			DoorSensorArr[5];
	