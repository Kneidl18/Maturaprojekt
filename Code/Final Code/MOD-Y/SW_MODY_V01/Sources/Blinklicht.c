#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h" 

#define RUNLEDGREEN			PTDD_PTDD4
#define RUNLEDPORTGREEN		PTDDD_PTDDD4



void Blinklicht(void)
{
	if(--RunLedCounter == 0)
	{
		if(RUNLEDGREEN == 1)
		{
			RUNLEDGREEN = 0;
		}
		else{
			RUNLEDGREEN = 1;
		}
		
		
		
		RunLedCounter = 10;
	}
}


void InitBlinklicht(void){
  	RUNLEDPORTGREEN=1 ; 				// Das Pin des RunLed wird als Ausgang deklariert (PD1)
  	RunLedCounter=10;
}