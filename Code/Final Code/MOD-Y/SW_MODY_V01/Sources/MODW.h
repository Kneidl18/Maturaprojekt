#include "FunctionProto.h"
#include "Variablen.h"
#include "defines.h"
#include "MC9S08LL16.h"


#define KEEP_AWAKE_PIN	PTBD_PTBD3
#define KEEP_AWAKE_DIR	PTBDD_PTBDD3

//initializes the KEEP_AWAKE_PORT as output
void MODW_Init(void)
{
	KEEP_AWAKE_DIR = 1;
	firstRound = 1;
}

//pulls the keepAwake output high
void keepAwake(void)
{
	KEEP_AWAKE_PIN = 1;
}

//drops the keepAwake output low
void turnOff(void)
{
	KEEP_AWAKE_PIN = 0;
}

//checks how the uc got turned on
void checkTurnOn(void)
{	
	if(PCF8563_checkAlarmFlag())
	{
		timerRanOut = 1;
		PCF8563_setAlarm();
		PCF8563_clearAlarmFlag();
	}
	else
	{
		DoorOpen = 1;
	}
}
