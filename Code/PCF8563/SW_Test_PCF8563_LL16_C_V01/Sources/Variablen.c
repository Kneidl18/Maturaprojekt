#include "Variablen.h"


#pragma DATA_SEG _DATA_ZEROPAGE 
	Byte			RealTimeFlag;               //Flag, das angibt, ob der Realtime um ist 0=Nein 1 = JA!
	Byte			RealTimeCounter;
	Byte			RunLedCounter;
	//temp
	unsigned int		AirTemp;
	Byte 			AirDelay;
	Byte 			AirTempCelsius;
	Byte 			AirTempViertelGrad;
