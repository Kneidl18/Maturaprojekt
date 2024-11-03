#include "Variablen.h"


#pragma DATA_SEG _DATA_ZEROPAGE 
	Byte			RealTimeFlag;               //Flag, das angibt, ob der Realtime um ist 0=Nein 1 = JA!
	Byte			RealTimeCounter;
	Byte			RunLedCounter;
	
	Byte			BME680_ChipIdCorrect;
 	uint			BME680_H6;
	uint			BME680_resHeatRange;
	uint			BME680_H3;
	uint			BME680_H4;
	uint			BME680_H5;
	uint			BME680_H7;
	uint			BME680_G1;
	uint			BME680_G3;
	uint			BME680_T3;
	uint			BME680_P3;
	uint			BME680_P6;
	uint			BME680_P7;
	uint			BME680_P10;
	uint			BME680_resHeat;
	uint			BME680_rngSwErr;
	Byte			BME680_H1[2];
	Byte      	BME680_H2[2];
	Byte      	BME680_T1[2];
	Byte      	BME680_P1[2];
	Byte      	BME680_G2[2];
	Byte      	BME680_T2[2];
	Byte      	BME680_P2[2];
	Byte      	BME680_P4[2];
	Byte      	BME680_P5[2];
	Byte      	BME680_P8[2];
	Byte      	BME680_P9[2];
	int			BME680_tFine;
	int			BME680_temperature;
	int			BME680_pressure;
	int			BME680_humidity;
	int			BME680_gas;
	uint			BME680_altitudeLvl;
	uint			BME680_waitGetData;
	
	Byte			sentSetGas;
	Byte			BME680_heatrRes;
	Byte			BME680_durval;
	Byte			gasReturned;
	Byte			BME680_setGasDone;
	
	//cc2500
	char 			CC2500_recvPacketArr[10];
	char			CC2500_sendPacketArr[50];
	unsigned int	CC2500_sendInterval;
	uint			i;
	Byte			CC2500_packetReceived;