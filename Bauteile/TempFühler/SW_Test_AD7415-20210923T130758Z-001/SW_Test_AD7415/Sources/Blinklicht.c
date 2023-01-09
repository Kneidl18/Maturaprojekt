#include "Variablen.h"
#include "defines.h"

#define RUNLED 		PTDD_PTDD0	  		// Das Blinklicht läuft auf PD0!
#define RUNLEDPORT	PTDDD_PTDDD0


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
    PTDDD_PTDDD1=1;
}