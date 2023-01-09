#include "Variablen.h"


#pragma DATA_SEG _DATA_ZEROPAGE 
	Byte			RealTimeFlag;               //Flag, das angibt, ob der Realtime um ist 0=Nein 1 = JA!
	Byte			RealTimeCounter;
	Byte			RunLedCounter;
	Byte			send;
	
	//cc2500
	char 			CC2500_recvPacketArr[1];
	char			CC2500_sendPacketArr[19];
	unsigned int	CC2500_sendInterval;
	uint			i;
	Byte			CC2500_packetReceived;
	
	//door
	Byte			DoorSensorState;
	Byte			DoorSensorArr[5];
	
	//water
	int				WaterSensor_ADCValue;
	
	//temp
	uint 			AirDelay_1;
	Byte 			AirTempCelsius_1;
	Byte 			AirTempViertelGrad_1;
	
	uint 			AirDelay_2;
	Byte 			AirTempCelsius_2;
	Byte 			AirTempViertelGrad_2;
	
	//BME680
	Byte			BME680_ChipIdCorrect;
	uint			BME680_waitGetData;
	
	Byte			BME680_heatrRes;
	Byte			BME680_durval;
	
	Byte			SS_type;
	
	Byte			BME680_values[15];
	
	