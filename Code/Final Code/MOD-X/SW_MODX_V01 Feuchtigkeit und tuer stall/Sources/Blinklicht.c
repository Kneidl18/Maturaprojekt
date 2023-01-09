#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h" 

#define RUNLEDGREEN			PTDD_PTDD1
#define RUNLEDPORTGREEN		PTDDD_PTDDD1

#define RUNLEDRED			PTDD_PTDD2
#define RUNLEDPORTRED		PTDDD_PTDDD2



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
  	RUNLEDPORTRED=1 ; 
  	RUNLEDPORTGREEN=1 ; 				// Das Pin des RunLed wird als Ausgang deklariert (PD1)
  	RunLedCounter=10;
  	
  	LCDSUPPLY = 0b00010111;
  	LCDC1 = 0b00000100;
}