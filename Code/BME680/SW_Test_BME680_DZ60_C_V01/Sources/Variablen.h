#include <stdtypes.h>

#pragma DATA_SEG _DATA_ZEROPAGE
	extern Byte 		RealTimeFlag;
	extern Byte		RealTimeCounter;
	extern Byte 		RunLedCounter;
	
	extern Byte			BME680_ChipIdCorrect;
	extern uint			BME680_resHeatRange;
	extern uint			BME680_H3;
	extern uint			BME680_H4;
	extern uint			BME680_H5;
	extern uint			BME680_H6;
	extern uint			BME680_H7;
	extern uint			BME680_G1;
	extern uint			BME680_G3;
	extern uint			BME680_T3;
	extern uint			BME680_P3;
	extern uint			BME680_P6;
	extern uint			BME680_P7;
	extern uint			BME680_P10;
	extern uint			BME680_resHeat;
	extern uint			BME680_rngSwErr;
	extern Byte			BME680_H1[2];
	extern Byte      	BME680_H2[2];
	extern Byte      	BME680_T1[2];
	extern Byte      	BME680_P1[2];
	extern Byte      	BME680_G2[2];
	extern Byte      	BME680_T2[2];
	extern Byte      	BME680_P2[2];
	extern Byte      	BME680_P4[2];
	extern Byte      	BME680_P5[2];
	extern Byte      	BME680_P8[2];
	extern Byte      	BME680_P9[2];
	extern int			BME680_tFine;
	extern int					BME680_temperature;
	extern int			BME680_pressure;
	extern int			BME680_humidity;
	extern int			BME680_gas;
	extern uint			BME680_altitudeLvl;
	extern uint			BME680_waitGetData;
	extern char			CC2500_sendPacketArr[50];
 