#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h" 

#define RUNLEDGREEN			PTDD_PTDD1
#define RUNLEDPORTGREEN		PTDDD_PTDDD1



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
  	
  	LCDSUPPLY = 0b00110111;				// Microcontroller Pins set to full complementary
  	LCDC1 = 0b00000100;
}