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
  	
  	if(sendInterval++ >= 100)		//every minute send packets with current and relaise status
  	{
  		sendInterval = 0;
  		
	  	//CurrentSensor
	  	CurrentSensor_PreparePacket();
	  	if(send == 1)
	  	CC2500_sendPacket();
	  		
	  	// Relaise
	  	RelaiseState = Relaise_CheckState();
	  	Relaise_PreparePacket();
	  	if(send == 2)
	  	CC2500_sendPacket();
	  	
	  	send++;
	  	if(send > 2)
	  		send = 0;
  	}
    
  } /* loop forever */
  /* please make sure that you never leave main */
}
