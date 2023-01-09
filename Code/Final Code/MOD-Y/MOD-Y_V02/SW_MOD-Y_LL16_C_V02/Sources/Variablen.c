#include "Variablen.h"


#pragma DATA_SEG _DATA_ZEROPAGE 
	Byte			RealTimeFlag;               //Flag, das angibt, ob der Realtime um ist 0=Nein 1 = JA!
	Byte			RealTimeCounter;
	Byte			RunLedCounter;
	int				WaterSensor_ADCValue;
	float 			WaterSensor_Pressure;
	int				RelaiseState;
	int 			CurrentSensor1;
	int 			CurrentSensor2;
	int 			CurrentSensor3;
	Byte 			ADCPort;
	Byte			sendPacketArr[20];
