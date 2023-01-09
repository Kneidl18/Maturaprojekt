#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h"

#define SCL_PIN		PTCD_PTCD0	  // Clock= PC0		
#define SCL_PORT	PTCDD_PTCDD0



#define SDA_PIN		PTCD_PTCD1	  // Data= PC1		
#define SDA_PORT	PTCDD_PTCDD1

#define READ_ADRESS_PCF8563 0xA3  // war 0x91
#define WRITE_ADRESS_PCF8563 0xA2  // war 0x90


/****************************************************************************/
/*                                                                          */
/* Init_PCF8563: Macht nicht viel, sendet nur eine Standard-Konfiguration    */
/* zum PCF8563 (die eigentlich die Reset-Kondition sein sollte, aber sicher  */
/* ist sicher) und setzt dann den Adress-Pointer auf das Register "0x00",   */
/* dem Temperatur-Register, um dann später einfach nur die Temperaturwerte  */
/* auslesen zu können.                                                      */
/*                                                                          */
/****************************************************************************/

void Init_PCF8563 (void)
{

    Byte Reply;

	SDA_PORT = 0;			// SDA und SCL sind EINGÄNGE, also HIGH
	SCL_PORT = 0;
	AirDelay = 0;
	
 	IIC_Start();
    IIC_Send_Byte (WRITE_ADRESS_PCF8563);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x01);		// Configuration Register
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x40);		// 0100 0000  Filter auf 1, der Rest ist 00, gibt in Hex 0x40
	Reply= IIC_Get_Ack();
	IIC_Stop ();

  	IIC_Start();                // Einfach nur Adresse "0x00" --> Adresspointer auf Temperatur-Register setzen
    IIC_Send_Byte (WRITE_ADRESS_PCF8563);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// Adress-Pointer auf 0x00
	Reply= IIC_Get_Ack();

	
	IIC_Stop ();
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
/*  Read_Air_Temp: Liest zyklisch jede Sekunde die Temperatur aus.          */
/*  Das Auslesen dauert bei einem 4,9152MHz-Quarz knapp 300µs.              */
/*                                                                          */
/*  In die Variable "AirTempCelsius" kommt die Temperatur in Grad Celsius   */
/*  In die Variable "AirTempViertelGrad" die "Nachkommastellen" der Grad    */
/*  Celsius, wobei  "00" --> 0,00 °C                                        */
/*                  "01" --> 0,25 °C                                        */
/*                  "02" --> 0,50 °C                                        */
/*                  "03" --> 0,75 °C                                        */
/*                                                                          */
/****************************************************************************/


void Read_Air_Temp(void)
{


    unsigned char Reply;



  AirDelay++;
  if (AirDelay == 100)              // 100 Realtime-Zyklen á 10ms = 1 Sekunde
  {                                 // Auslesen einmal pro Sekunde, weil öfter eh kaum Sinn macht...
  
  	AirDelay = 0;
  	
  	
  	// Beim Baustein handelt es sich um einen PCF8563ARTZ-0
  	// Adresse des Bausteines mit AS-Pin auf GND: 1001 000 (0x48) ZUZÜGLICH dem R/W\-Bit als LSB.
  	// Da die Adresse nur 7 Bit lang ist und mit dem R/W-Bit 8 Bit rauskommen, ergeben sich (als 8-Bit gesehen)
  	// folgende Adressen: 0x90 bei WRITE und 0x91 bei READ
  	

	IIC_Start ();				// Read!!
	IIC_Send_Byte (READ_ADRESS_PCF8563);
	Reply= IIC_Get_Ack();
	AirTempCelsius = IIC_Get_Byte ();
	IIC_Put_Ack();
	AirTempViertelGrad = (IIC_Get_Byte()>>6);
	IIC_Put_Nack();
	IIC_Stop();

  }
}
