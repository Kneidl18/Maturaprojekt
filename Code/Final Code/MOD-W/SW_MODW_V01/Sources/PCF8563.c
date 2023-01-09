#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h"


#define SCL_PIN		PTAD_PTAD4	  // Clock= PA3	
#define SCL_PORT	PTADD_PTADD4

#define SDA_PIN		PTAD_PTAD5	  // Data= PA2		
#define SDA_PORT	PTADD_PTADD5

#define READ_ADRESS_PCF8563 0xA3
#define WRITE_ADRESS_PCF8563 0xA2

#define ALARM_REGISTER		0x09


/****************************************************************************/
/*                                                                          */
/* 	Init_PCF8563: sets control_status_register 1 and 2	(0x00, 0x01)		*/
/*	1: 0x00, 2: 0x02														*/
/*  AF and AIE are set active in control_status_2(0x01): 0b0000 0010		*/
/*	INT will have the same status as AF					*/
/*	For safety clear the AlarmFlag -> call PCF8563_clearAlarmFlag()			*/
/*	sets the AlarmCounter to 1 day -> call PCF8563_setAlarm()      			*/
/*  If the AlarmFlag is set it has to be cleared before a new alarm         */
/*                                                                          */
/****************************************************************************/
void Init_PCF8563(void)
{

    Byte Reply;

	SDA_PORT = 0;			// SDA und SCL sind EINGÄNGE, also HIGH
	SCL_PORT = 0;
	
 	IIC_Start();
    	IIC_Send_Byte (WRITE_ADRESS_PCF8563);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// control_status_1 Register
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// 0000 0000
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x01);		// control_status_2 Register
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x12);		// 0b0001 0010: set AF and AIE -> active Alarm
	Reply= IIC_Get_Ack();
	IIC_Stop ();
	
	PCF8563_clearAlarmFlag();
	PCF8563_setAlarm();
}


/****************************************************************************/
/*                                                                          */
/* 	IIC_START: Startkondition, indem zuerst das Datenpin auf LOW geht,      */
/*  danach das CLOCKPIN.                                                    */
/*  Dadurch, dass der Pin als OUTPUT geschaltet wird und eine logische "0"  */
/*  ausgibt geht die Leitung auf LOW. Für ein HIGH wird die Leitung einfach */
/*  als INPUT geschaltet, somit wird sie hochohmig und der Pull-Up zieht    */
/*  die Leitung auf HIGH.                                                   */
/*                                                                          */
/****************************************************************************/

void IIC_Start (void) {

	SDA_PORT = 1;			// SDA-Pin wird als Ausgang definiert, mit Pegel = LOW
	SDA_PIN = 0;			

	Nop();			     	// Zuerst DATA auf LOW, danach Clock
	Nop();                  // Das ist die Startkondition!!

	SCL_PORT = 1;
	SCL_PIN = 0;
}


/****************************************************************************/
/*                                                                          */
/*  IIC_STOP: Stop-Kondition: Zuerst geht Clock auf HIGH, danach DATA       */
/*                                                                          */
/****************************************************************************/


void IIC_Stop (void){

	SDA_PORT = 1;			// Sicherheitshalber mal beide Ports als Ausgang schalten!
	SCL_PORT = 1;


	SCL_PORT = 0;			// zuerst geht der CLOCK auf HIGH, indem er ein INPUT wird...
	Nop();
	Nop();
	SDA_PORT = 0;           // und danach DATA! --> Das ist die STOP-Kondition

}

/****************************************************************************/
/*                                                                          */
/*  IIC_Send_Byte: Sendet ein Byte hinaus                                   */
/*                                                                          */
/****************************************************************************/



void IIC_Send_Byte (unsigned char Wert){

Byte Counter,ShiftWert;

	ShiftWert=Wert;
	
	SDA_PORT = 1;

	for (Counter=0; Counter <8; Counter++){
		if ((ShiftWert & 0x80)	== 0x80) {
		        SDA_PORT = 0;                   // Sendet ein HIGH

		}
		else {
		        SDA_PORT = 1;                   // Sendet ein LOW
		        SDA_PIN = 0;
		        

		}
		
		SCL_PORT = 0;		// Einen Clock geben  --> Clock auf HIGH

		SCL_PORT = 1;       // Clock auf LOW
		SCL_PIN = 0;

		ShiftWert = ShiftWert<<1;
	}
	SDA_PORT = 0;           // Am Ende: Port als EINGANG (damit das ACK empfangen werden kann)

}

/****************************************************************************/
/*                                                                          */
/*  IIC_Get_Ack: Holt einen Acknowledge herein                              */
/*                                                                          */
/****************************************************************************/



unsigned char IIC_Get_Ack (void){

unsigned char Response;

	SDA_PORT = 0;			// Port wird als Eingang geschaltet (falls er es noch nicht war)

	Nop();

    SCL_PORT = 0;    		// Einen Clock geben  --> SCL = HIGH
	if (SDA_PIN == 1) {
	        Response = 0;

	}
	else {
	        Response = 1;

	}
	SCL_PORT = 1;
	SCL_PIN = 0;        // Clock wieder auf LOW

	
	SDA_PORT = 1;		// Port wieder als AUSGANG schalten
	SDA_PIN = 0;
	return (Response);
	
}


/****************************************************************************/
/*                                                                          */
/*  IIC_Get_Byte: Holt ein Byte herein                                      */
/*                                                                          */
/****************************************************************************/


unsigned char IIC_Get_Byte (void){

unsigned char Response, Counter;

	SDA_PORT = 0;			// Port als Eingang!

	for (Counter=0; Counter <8; Counter++){
		Response=Response<<1;
		
		SCL_PORT = 0;			// Einen Clock geben  SCL = HIGH

		if (SDA_PIN == 1) Response |=0x01;
		else Response &= 0xFE;			// Fraglich....!!!
		
		SCL_PORT = 1;           // Clock wieder auf LOW
		SCL_PIN = 0;


	}
	return (Response);
			
}

/****************************************************************************/
/*                                                                          */
/*  IIC_Put_Ack: Gibt einen Acknowledge auf die Datenleitung aus            */
/*                                                                          */
/****************************************************************************/

void IIC_Put_Ack(void)
{
	SDA_PORT = 1;       // Datenleitung auf LOW
	SDA_PIN = 0;
	
	SCL_PORT = 0;        // Clock auf HIGH

	SCL_PORT = 1;       // Clock wieder auf LOW
	SCL_PIN = 0;        
	
	SDA_PORT = 0;       // Datenleitung wieder freigeben (als Input setzen)
}

/****************************************************************************/
/*                                                                          */
/*  IIC_Put_Nack: Gibt einen NOT-Acknowledge auf die Datenleitung aus       */
/*                                                                          */
/****************************************************************************/

void IIC_Put_Nack(void)
{
	SDA_PORT = 0;       // Datenleitung auf HIGH
		
	SCL_PORT = 0;        // Clock auf HIGH

	SCL_PORT = 1;       // Clock wieder auf LOW
	SCL_PIN = 0;        
	
	SDA_PORT = 1;       // Datenleitung wieder LOW
	SDA_PIN=0;
}

/****************************************************************************/
/*                                                                          */
/*  PCF8563_checkTimer: checks if the alarm ran out				            */
/* 	Returns true if so														*/
/*	The alarm bit is in control_status_2(0x01) bit 3 (0b0000 x000)			*/
/*	If the bit is set it will return 1, else 0								*/
/*	To clear the AlarmFlagBit call PCF8563_clearAlarmFlag()					*/
/*                                                                          */
/****************************************************************************/
char PCF8563_checkAlarmFlag(void)
{
	unsigned char Reply, FlagActive;
	//write: set pointer to control_register_2
	IIC_Start();
    IIC_Send_Byte (WRITE_ADRESS_PCF8563);
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x01);		
	Reply= IIC_Get_Ack();
	IIC_Stop ();
	
	//read 0x0B
	IIC_Start ();				
	IIC_Send_Byte (READ_ADRESS_PCF8563);
	Reply= IIC_Get_Ack();
	FlagActive = IIC_Get_Byte();
	IIC_Put_Nack();
	IIC_Stop();
	
	if(FlagActive == 0b00001010)
		FlagActive = 1;
	else
		FlagActive = 0;
	
	return FlagActive;
}

/****************************************************************************/
/*                                                                          */
/*  PCF8563_clearAlarmFlag: clears and eventuel alarm flag	         		*/
/* 	It writes 0x02 to the control_status_2 Register(0x01)					*/
/*	If the Alarm bit was set it will be cleared and will clear INT too		*/
/*                                                                          */
/****************************************************************************/
void PCF8563_clearAlarmFlag(void)
{
	//write
	Byte Reply;
	
 	IIC_Start();
    IIC_Send_Byte (WRITE_ADRESS_PCF8563);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x01);		// control_status_2 Register
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x12);		// 0001 0010 clear the AlarmFlag
	Reply= IIC_Get_Ack();
	IIC_Stop ();
}

/****************************************************************************/
/*                                                                          */
/*  PCF8563_setTimer: sets the Alarm to 1 Day				         		*/
/* 	It writes 1(0x01) to the register Day_Alarm(0x0B)						*/
/*                                                                          */
/****************************************************************************/
void PCF8563_setAlarm(void)
{
	//write
	Byte Reply;
	
 	IIC_Start();
    IIC_Send_Byte (WRITE_ADRESS_PCF8563);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (ALARM_REGISTER);		// 0x0B Day_Register Register
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x01);		// 0000 0001 set the Alarm to 1 Day
	Reply= IIC_Get_Ack();
	IIC_Stop ();
}
