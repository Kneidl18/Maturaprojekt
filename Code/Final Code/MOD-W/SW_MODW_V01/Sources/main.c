#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "FunctionProto.h"
#include "Variablen.h"
#include "inputToCommand.h"
#include "MODW.h"

#define PCB 	PCB_MOD_W10


void main(void) {

  EnableInterrupts; /* enable interrupts */
  /* include your code here */


  for(;;) {
    while(RealTimeFlag == 0)
  	{
    		__RESET_WATCHDOG(); /* feeds the dog */	
  	}
  	RealTimeFlag = 0;
  	
  	keepAwake();
  	Blinklicht();
  	
  	checkTurnOn();

  	if(firstRound)
  	{
  		firstRound = 0;	
	  	if(DoorOpen)
	  	{	
			DoorSensor_PreparePacket();
			CC2500_sendPacketArr[2] = 0xF;
			CC2500_sendPacket();	
	  	} 
		else if(timerRanOut)
		{
			CC2500_sendPacketArr[0] = 3;
			CC2500_sendPacketArr[1] = PCB << 4;
			CC2500_sendPacketArr[1] += SENSOR_DOOR;
			CC2500_sendPacketArr[2] = 0xFF;
			
			CC2500_sendPacket();
		}
		
		//PCF8563_setAlarm();
  	}
  	
  	if(delayDoorSend++ >= 10 && DoorOpen) //if the door is opened it sends the current door status every minute
  	{
		for(delayDoorSend = 0; delayDoorSend < 3; delayDoorSend++)
		{	
			DoorSensor_PreparePacket();
			CC2500_sendPacket();
		}
  		delayDoorSend = 0;
		//PCF8563_setAlarm();
  	}
  	
  	turnOff();  	
  } /* loop forever */
  /* please make sure that you never leave main */
}
