#include <stdtypes.h>

#pragma DATA_SEG _DATA_ZEROPAGE
	extern Byte 		RealTimeFlag;
	extern Byte			RealTimeCounter;
	extern Byte 		RunLedCounter;
	extern char 		CC2500_recvPacketArr[20];
	extern char			CC2500_sendPacketArr[20];
	extern unsigned int	CC2500_sendInterval;
	extern uint			i;		