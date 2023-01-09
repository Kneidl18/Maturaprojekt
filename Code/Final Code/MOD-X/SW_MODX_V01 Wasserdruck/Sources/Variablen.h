#include <stdtypes.h>

#pragma DATA_SEG _DATA_ZEROPAGE
	extern Byte 		RealTimeFlag;
	extern Byte			RealTimeCounter;
	extern Byte 		RunLedCounter;
	extern Byte			send;
	
	//cc2500
	extern char 		CC2500_recvPacketArr[1];
	extern char			CC2500_sendPacketArr[19];
	extern unsigned int	CC2500_sendInterval;
	extern uint			i;		
	extern Byte			CC2500_packetReceived;
	
	//door
	extern Byte			DoorSensorState;
	extern Byte	 		DoorSensorArr[5];
	
	//water
	extern int			WaterSensor_ADCValue;
	
	//temp
	extern uint			AirDelay_1;
	extern Byte			AirTempCelsius_1;
	extern Byte			AirTempViertelGrad_1;
	
	extern uint			AirDelay_2;
	extern Byte			AirTempCelsius_2;
	extern Byte			AirTempViertelGrad_2;
		
	//BME680
	extern Byte			BME680_ChipIdCorrect;
	extern uint			BME680_waitGetData;
	
	extern Byte			BME680_heatrRes;
	extern Byte			BME680_durval;
	
	extern Byte			SS_type;
	
	extern Byte			BME680_values[15];
	