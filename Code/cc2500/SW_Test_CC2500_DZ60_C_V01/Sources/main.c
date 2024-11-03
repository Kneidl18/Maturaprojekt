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
  	
  	if(CC2500_sendInterval++ >= 10)
  	{
  		CC2500_sendPacketArr[0] = 5;		  //Packet length
  		CC2500_sendPacketArr[1] = 0b01001000; //H
  		CC2500_sendPacketArr[2] = 0b01100101; //e
  		CC2500_sendPacketArr[3] = 0b01101100; //l
  		CC2500_sendPacketArr[4] = 0b01101100; //l
  		CC2500_sendPacketArr[5] = 0b01101111; //o
  		
  		CC2500_sendPacket(); 
  		
  		CC2500_sendInterval = 0;
  	}
  	CC2500_listenForPacket();
  	
  
  	
  } /* loop forever */
  /* please make sure that you never leave main */
}
