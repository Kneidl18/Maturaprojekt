#include "Variablen.h"
#include "MC9S08DZ60.h"
#include "defines.h"

//************************************************************************************************
//*                                                                                              *
//*      Init_Realtime: Initialisiert den RealTime                                               *
//*                                                                                              *
//*      BusClock: Bei 4,9152 MHz-Quarz wäre der Busclock zwar ein Viertel davon, aber der FLL   *
//*      taktet den Bus um das Vierfache auf 19,6608 MHz rauf. Somit ist der Busclock effektiv   *
//*      4,9152 MHz, oder als Kehrwert 203,451 nsec.                                             *
//*      Der Realtime zählt nun diese Busclocks rauf (Einheiten zu 203,451 ns)                   *
//*                                                                                              *
//*      Bei $6000 Zyklen (Dezimal: 49.152 Zyklen) á 203,451 nsec ergibt das EXAKT 10 msec       *
//*      Wenn ich nur 8192 hinaufzähle, dann kommt ein Wert von  1,666667 ms raus!               *
//*                                                                                              *
//************************************************************************************************

#pragma TRAP_PROC
__interrupt void _Realtime(void)
{
	RTCSC = 0xB1;			
	
	RealTimeCounter++;
	if(RealTimeCounter == 6)
	{
		RealTimeFlag = 1;
		RealTimeCounter = 0;
	}
}

void InitRealtime(void)
{
	RTCMOD = 0x2F;
	RTCSC = 0xB1;
	RealTimeFlag = 0;
	RealTimeCounter = 0;
}

/*
	Dummy Interrupt-Routine: needed for eventual not wanted interrupts
*/

__interrupt void _Dummy(void)
{
	Nop();
}