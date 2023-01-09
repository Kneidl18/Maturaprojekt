#include "Variablen.h"
#include "defines.h"

#define RUNLED 		PTCD_PTCD0	  		// Das Blinklicht läuft auf PC0!
#define RUNLEDPORT	PTCDD_PTCDD0


void Blinklicht (void) {
 if(--RunLedCounter == 0)
        {
          
           if (RUNLED==1) RUNLED =0;
           else
           RUNLED = 1;
           
           RunLedCounter = 100;
        }
}



void InitBlinklicht(void){
  	RUNLEDPORT=1; 				// Das Pin des RunLed wird als Ausgang deklariert (PD0)
  	RunLedCounter=100;
}