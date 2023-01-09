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
	uchar temp = TPM1C0SC;		// read register to continue timer
	uint compValue = TPM1C0V;	// load the current time Value
	
	__asm bclr		7, TPM1C0SC	// continue counting time
	
	compValue += 0x2F;			// set new time instance to 3.33 ms in the future
	TPM1C0V = compValue;		// save new time instance
				
	RealTimeCounter++;
	if(RealTimeCounter == 3)
	{
		RealTimeFlag = 1;		// set real time flag
		RealTimeCounter = 0;	// one main loop round will be performed
	}
}

void InitRealtime(void)
{
	TPM1SC = 0b00001000;		//}
	TPM1C0SC = 0b01010000;		//> Registerdefinition
	TPM1C0V = 0x2F;				//}
	
	RealTimeFlag = 0;			// set start values to 0
	RealTimeCounter = 0;
}

/*
	Dummy Interrupt-Routine: needed for eventual not wanted interrupts
*/

__interrupt void _Dummy(void)
{
	Nop();
}