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
  	
  	if(BME680_waitGetData++ >= 10 && BME680_ChipIdCorrect == 1)
  	{
  		BME680_getSensorData();  
  	
	  	__asm ldhx	BME680_temperature
	  	__asm ldhx	BME680_humidity
	  	__asm ldhx	BME680_pressure
	  	BME680_altitudeLvl = BME680_altitude(BME680_pressure, 1013.25);
	  	__asm ldhx	BME680_gas
	  	
	  	BME680_waitGetData = 0;
  	}
  	
  
  	
  } /* loop forever */
  /* please make sure that you never leave main */
}
