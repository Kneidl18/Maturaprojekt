#include "Variablen.h"


#pragma DATA_SEG _DATA_ZEROPAGE 
	Byte			RealTimeFlag;               //Flag, das angibt, ob der Realtime um ist 0=Nein 1 = JA!
	Byte			RealTimeCounter;
	Byte			RunLedCounter;
	Byte			sendInterval;
	Byte			send;
	
	//adc and currentSensor
	int 			CurrentSensor1;
	int 			CurrentSensor2;
	int 			CurrentSensor3;
	Byte 			ADCPort;
	
	//cc2500
	Byte			CC2500_sendPacketArr[50];
	Byte			CC2500_recvPacketArr[10];
	Byte			CC2500_packetReceived;
	int				i;
	
	//relaise
	int				RelaiseState;