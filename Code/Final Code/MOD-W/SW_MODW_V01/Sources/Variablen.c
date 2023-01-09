#include "Variablen.h"


#pragma DATA_SEG _DATA_ZEROPAGE 
	Byte			RealTimeFlag;               //Flag, das angibt, ob der Realtime um ist 0=Nein 1 = JA!
	Byte			RealTimeCounter;
	Byte			RunLedCounter;
	
	//modw
	Byte			DoorOpen;
	Byte			ButtonPressed;
	Byte			timerRanOut;
	Byte			firstRound;
	uint			delayDoorSend;				
	
	//cc2500
	char 			CC2500_recvPacketArr[20];
	char			CC2500_sendPacketArr[20];
	unsigned int	CC2500_sendInterval;
	uint			i;
	Byte			CC2500_packetReceived;
	unsigned char 	crc;
	
	//doorSensor
	Byte			DoorSensorState;
	Byte			DoorSensorArr[5];
	