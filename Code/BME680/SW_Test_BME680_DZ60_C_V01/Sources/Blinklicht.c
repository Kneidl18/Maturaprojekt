#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h" 

#define RUNLED		PTDD_PTDD0
#define RUNLEDPORT	PTDDD_PTDDD0


#define SCL_PIN		PTCD_PTCD0	  // Clock= PC0		
#define SCL_PORT	PTCDD_PTCDD0

#define SDA_PIN		PTCD_PTCD1	  // Data= PC1		
#define SDA_PORT	PTCDD_PTCDD1


void Blinklicht(void)
{
	if(--RunLedCounter == 0)
	{
		if(RUNLED == 1) RUNLED = 0;
		else
		RUNLED = 1;
		
		RunLedCounter = 10;
	}
}


void InitBlinklicht(void){
  	RUNLEDPORT=1; 				// Das Pin des RunLed wird als Ausgang deklariert (PD0)
  	RunLedCounter=10;
    PTDDD_PTDDD1=1;
}