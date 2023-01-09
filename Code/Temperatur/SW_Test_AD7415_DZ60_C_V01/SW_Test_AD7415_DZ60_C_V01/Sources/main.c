#include <MC9S08DZ60.h>
#include "FunctionProto.h"
#include "Variablen.h"  	/* All my variables */


void main(void) {


  for(;;) {
  
  	while(RealTimeFlag == 0)
  	{
    		__RESET_WATCHDOG(); /* feeds the dog */	
  	}
  	RealTimeFlag = 0;
  	
  	// here my tasks start
  	Blinklicht();
  	Read_Air_Temp();
  
  	
  } /* loop forever */
  /* please make sure that you never leave main */
}
