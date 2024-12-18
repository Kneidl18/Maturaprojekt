#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "FunctionProto.h"
#include "Variablen.h"



void main(void) {

  EnableInterrupts; /* enable interrupts */
  /* include your code here */



  for(;;) {
    while(RealTimeFlag == 0)
  	{
    		__RESET_WATCHDOG(); /* feeds the dog */	
  	}
  	RealTimeFlag = 0;
  	
  	//here my tasks start
  	Blinklicht();
  	
  	// ADC
  	ADC_LL16_V01_getADCValue();
  	
  	//CurrentSensor
  	CurrentSensor_PreparePacket();	
  	
  	// Relaise
  	RelaiseState = Relaise_CheckState();
  	Relaise_PreparePacket();
    
  } /* loop forever */
  /* please make sure that you never leave main */
}
