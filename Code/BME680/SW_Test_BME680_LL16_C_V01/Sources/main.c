#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "FunctionProto.h"
#include "Variablen.h"



void main(void) {

  for(;;) {
    while(RealTimeFlag == 0)
  	{
    		__RESET_WATCHDOG(); /* feeds the dog */	
  	}
  	RealTimeFlag = 0;
  	
  	//here my tasks start
  	Blinklicht();
    
    if(BME680_waitGetData++ >= 10 && BME680_ChipIdCorrect == 1 && BME680_setGasDone)
  	{
  		BME680_getSensorData();  
	  	BME680_waitGetData = 0;
  	}
  	else if(!BME680_setGasDone)
  	{
		BME680_setGas(320, 150);
		
  	}
  	
  	
  } /* loop forever */
  /* please make sure that you never leave main */
}
