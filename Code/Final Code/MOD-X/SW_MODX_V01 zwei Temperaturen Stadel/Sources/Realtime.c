#include "Variablen.h"
#include "MC9S08LL16.h"
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
	uchar temp = TPM1C0SC;
	uint compValue = TPM1C0V;
	
	__asm bclr		7, TPM1C0SC
	
	compValue += 0x2F;
	TPM1C0V = compValue;
				
	RealTimeCounter++;
	if(RealTimeCounter == 3)
	{
		RealTimeFlag = 1;
		RealTimeCounter = 0;
	}
}

void InitRealtime(void)
{
	TPM1SC = 0b00001000;
	TPM1C0SC = 0b01010000;
	TPM1C0V = 0x2F;
	
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