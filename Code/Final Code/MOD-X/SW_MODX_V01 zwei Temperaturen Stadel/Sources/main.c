#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "FunctionProto.h"
#include "Variablen.h"

#define SENDINTERVAL		100
#define RUNLEDRED			PTDD_PTDD2

// info: door and water are done
// TODO: BME680 combine everything, setGas, sendCalibration, check which variables neccessary
// TODO: temp fix IIC
void main(void) {

  EnableInterrupts; /* enable interrupts */
  /* include your code here */

  for(;;) {
    while(RealTimeFlag == 0)
  	{
    		__RESET_WATCHDOG(); /* feeds the dog */	
  	}
  	RealTimeFlag = 0;
  	
  	Blinklicht();
  	//CC2500_listenForPacket();
  	
  	//water
  	//WaterSensor_ADC();
  	
  	//temp
   	Read_Air_Temp_1();
  	
  	
  	//BME680
  	if(BME680_waitGetData++ >= SENDINTERVAL && BME680_ChipIdCorrect == 1)
  	{
  		BME680_getSensorData();  
	  	BME680_waitGetData = 0;
  	}
  	
  	
  	//DoorSensor_CheckState();
  //	DoorSensor_UpdateState();
  	
  	if(CC2500_sendInterval++ >= SENDINTERVAL)
  	{
  		CC2500_sendInterval = 0;
  		
  		if(RUNLEDRED == 1)
		{
			RUNLEDRED = 0;
		}
		else{
			RUNLEDRED = 1;
		}
  		
  		//Door
  		DoorSensor_PreparePacket();
  		if(send == 1)	
  			//CC2500_sendPacket(); 
  		
  		//BME680
  		if(BME680_ChipIdCorrect && send == 3)
  		{
  			BME680_preparePacketValues();
  			//CC2500_sendPacket();	
  		}
  		
  		//temp
  		AD7415_preparePacket_1();
  		if(send == 2)
  			CC2500_sendPacket();
  		
  		AD7415_preparePacket_2();
  		if(send == 1)
  			//CC2500_sendPacket();
  		
  		//water
  		WaterSensor_PreparePacket();
  		if(send == 1)
  			CC2500_sendPacket();
  		send++;
  		if(send > 3)
  			send = 1;
  		
  	}
  } /* loop forever */
  /* please make sure that you never leave main */
}
