#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h"
#include "inputToCommand.h"

#define SCL_PIN_1		PTDD_PTDD4	  // Clock= PC0		
#define SCL_PORT_1	PTDDD_PTDDD4

#define SDA_PIN_1		PTDD_PTDD3	  // Data= PC1		
#define SDA_PORT_1	PTDDD_PTDDD3


#define READ_ADRESS_AD7415_1 0x93  // war 0x91
#define WRITE_ADRESS_AD7415_1 0x92  // war 0x90

#define READ_ADRESS_AD7415_2 0x95  // war 0x91
#define WRITE_ADRESS_AD7415_2 0x94  // war 0x90


/****************************************************************************/
/*                                                                          */
/* Init_AD7415: Macht nicht viel, sendet nur eine Standard-Konfiguration    */
/* zum AD7415 (die eigentlich die Reset-Kondition sein sollte, aber sicher  */
/* ist sicher) und setzt dann den Adress-Pointer auf das Register "0x00",   */
/* dem Temperatur-Register, um dann später einfach nur die Temperaturwerte  */
/* auslesen zu können.                                                      */
/*                                                                          */
/****************************************************************************/

void Init_AD7415_1 (void)
{

    Byte Reply;

	SDA_PORT_1 = 0;			// SDA und SCL sind EINGÄNGE, also HIGH
	SCL_PORT_1 = 0;
	AirDelay_1 = 0;
	
 	IIC_Start_1();
    IIC_Send_Byte_1 (WRITE_ADRESS_AD7415_1);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack_1();
	IIC_Send_Byte_1 (0x01);		// Configuration Register
	Reply= IIC_Get_Ack_1();
	IIC_Send_Byte_1 (0x40);		// 0100 0000  Filter auf 1, der Rest ist 00, gibt in Hex 0x40
	Reply= IIC_Get_Ack_1();
	IIC_Stop_1 ();

  	IIC_Start_1();                // Einfach nur Adresse "0x00" --> Adresspointer auf Temperatur-Register setzen
    IIC_Send_Byte_1 (WRITE_ADRESS_AD7415_1);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack_1();
	IIC_Send_Byte_1 (0x00);		// Adress-Pointer auf 0x00
	Reply= IIC_Get_Ack_1();
	IIC_Stop_1 ();
	
 	IIC_Start_1();
    IIC_Send_Byte_1 (WRITE_ADRESS_AD7415_2);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack_1();
	IIC_Send_Byte_1 (0x01);		// Configuration Register
	Reply= IIC_Get_Ack_1();
	IIC_Send_Byte_1 (0x40);		// 0100 0000  Filter auf 1, der Rest ist 00, gibt in Hex 0x40
	Reply= IIC_Get_Ack_1();
	IIC_Stop_1 ();

  	IIC_Start_1();                // Einfach nur Adresse "0x00" --> Adresspointer auf Temperatur-Register setzen
    IIC_Send_Byte_1 (WRITE_ADRESS_AD7415_2);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack_1();
	IIC_Send_Byte_1 (0x00);		// Adress-Pointer auf 0x00
	Reply= IIC_Get_Ack_1();	
	IIC_Stop_1 ();
}


/****************************************************************************/
/*                                                                          */
/* 	IIC_Start_1: Startkondition, indem zuerst das Datenpin auf LOW geht,      */
/*  danach das CLOCKPIN.                                                    */
/*  Dadurch, dass der Pin als OUTPUT geschaltet wird und eine logische "0"  */
/*  ausgibt geht die Leitung auf LOW. Für ein HIGH wird die Leitung einfach */
/*  als INPUT geschaltet, somit wird sie hochohmig und der Pull-Up zieht    */
/*  die Leitung auf HIGH.                                                   */
/*                                                                          */
/****************************************************************************/

void IIC_Start_1 (void) {

	SDA_PORT_1 = 1;			// SDA-Pin wird als Ausgang definiert, mit Pegel = LOW
	SDA_PIN_1 = 0;			

	Nop();			     	// Zuerst DATA auf LOW, danach Clock
	Nop();                  // Das ist die Startkondition!!

	SCL_PORT_1 = 1;
	SCL_PIN_1 = 0;
}


/****************************************************************************/
/*                                                                          */
/*  IIC_Stop_1: Stop-Kondition: Zuerst geht Clock auf HIGH, danach DATA       */
/*                                                                          */
/****************************************************************************/


void IIC_Stop_1 (void){

	SDA_PORT_1 = 1;			// Sicherheitshalber mal beide Ports als Ausgang schalten!
	SCL_PORT_1 = 1;


	SCL_PORT_1 = 0;			// zuerst geht der CLOCK auf HIGH, indem er ein INPUT wird...
	Nop();
	Nop();
	SDA_PORT_1 = 0;           // und danach DATA! --> Das ist die STOP-Kondition

}

/****************************************************************************/
/*                                                                          */
/*  IIC_Send_Byte_1: Sendet ein Byte hinaus                                   */
/*                                                                          */
/****************************************************************************/



void IIC_Send_Byte_1 (unsigned char Wert){

Byte Counter,ShiftWert;

	ShiftWert=Wert;
	
	SDA_PORT_1 = 1;

	for (Counter=0; Counter <8; Counter++){
		if ((ShiftWert & 0x80)	== 0x80) {
		        SDA_PORT_1 = 0;                   // Sendet ein HIGH

		}
		else {
		        SDA_PORT_1 = 1;                   // Sendet ein LOW
		        SDA_PIN_1 = 0;
		        

		}
		
		SCL_PORT_1 = 0;		// Einen Clock geben  --> Clock auf HIGH

		SCL_PORT_1 = 1;       // Clock auf LOW
		SCL_PIN_1 = 0;

		ShiftWert = ShiftWert<<1;
	}
	SDA_PORT_1 = 0;           // Am Ende: Port als EINGANG (damit das ACK empfangen werden kann)

}

/****************************************************************************/
/*                                                                          */
/*  IIC_Get_Ack_1: Holt einen Acknowledge herein                              */
/*                                                                          */
/****************************************************************************/



unsigned char IIC_Get_Ack_1 (void){

unsigned char Response;

	SDA_PORT_1 = 0;			// Port wird als Eingang geschaltet (falls er es noch nicht war)

	Nop();

    SCL_PORT_1 = 0;    		// Einen Clock geben  --> SCL = HIGH
	if (SDA_PIN_1 == 1) {
	        Response = 0;

	}
	else {
	        Response = 1;

	}
	SCL_PORT_1 = 1;
	SCL_PIN_1 = 0;        // Clock wieder auf LOW

	
	SDA_PORT_1 = 1;		// Port wieder als AUSGANG schalten
	SDA_PIN_1 = 0;
	return (Response);
	
}


/****************************************************************************/
/*                                                                          */
/*  IIC_Get_Byte_1: Holt ein Byte herein                                      */
/*                                                                          */
/****************************************************************************/


unsigned char IIC_Get_Byte_1 (void){

unsigned char Response, Counter;

	SDA_PORT_1 = 0;			// Port als Eingang!

	for (Counter=0; Counter <8; Counter++){
		Response=Response<<1;
		
		SCL_PORT_1 = 0;			// Einen Clock geben  SCL = HIGH

		if (SDA_PIN_1 == 1) Response |=0x01;
		else Response &= 0xFE;			// Fraglich....!!!
		
		SCL_PORT_1 = 1;           // Clock wieder auf LOW
		SCL_PIN_1 = 0;


	}
	return (Response);
			
}

/****************************************************************************/
/*                                                                          */
/*  IIC_Put_Ack_1: Gibt einen Acknowledge auf die Datenleitung aus            */
/*                                                                          */
/****************************************************************************/

void IIC_Put_Ack_1(void)
{
	SDA_PORT_1 = 1;       // Datenleitung auf LOW
	SDA_PIN_1 = 0;
	
	SCL_PORT_1 = 0;        // Clock auf HIGH

	SCL_PORT_1 = 1;       // Clock wieder auf LOW
	SCL_PIN_1 = 0;        
	
	SDA_PORT_1 = 0;       // Datenleitung wieder freigeben (als Input setzen)
}

/****************************************************************************/
/*                                                                          */
/*  IIC_Put_Nack_1: Gibt einen NOT-Acknowledge auf die Datenleitung aus       */
/*                                                                          */
/****************************************************************************/

void IIC_Put_Nack_1(void)
{
	SDA_PORT_1 = 0;       // Datenleitung auf HIGH
		
	SCL_PORT_1 = 0;        // Clock auf HIGH

	SCL_PORT_1 = 1;       // Clock wieder auf LOW
	SCL_PIN_1 = 0;        
	
	SDA_PORT_1 = 1;       // Datenleitung wieder LOW
	SDA_PIN_1=0;
}


/****************************************************************************/
/*                                                                          */
/*  Read_Air_Temp_1: Liest zyklisch jede Sekunde die Temperatur aus.          */
/*  Das Auslesen dauert bei einem 4,9152MHz-Quarz knapp 300µs.              */
/*                                                                          */
/*  In die Variable "AirTempCelsius_1" kommt die Temperatur in Grad Celsius   */
/*  In die Variable "AirTempViertelGrad_1" die "Nachkommastellen" der Grad    */
/*  Celsius, wobei  "00" --> 0,00 °C                                        */
/*                  "01" --> 0,25 °C                                        */
/*                  "02" --> 0,50 °C                                        */
/*                  "03" --> 0,75 °C                                        */
/*                                                                          */
/****************************************************************************/


void Read_Air_Temp_1(void)
{


    unsigned char Reply;



  AirDelay_1++;
  if (AirDelay_1 >= 100)              // 100 Realtime-Zyklen á 10ms = 1 Sekunde
  {                                 // Auslesen einmal pro Sekunde, weil öfter eh kaum Sinn macht...
  
  	AirDelay_1 = 0;
  	
  	
  	// Beim Baustein handelt es sich um einen AD7415ARTZ-0
  	// Adresse des Bausteines mit AS-Pin auf GND: 1001 000 (0x48) ZUZÜGLICH dem R/W\-Bit als LSB.
  	// Da die Adresse nur 7 Bit lang ist und mit dem R/W-Bit 8 Bit rauskommen, ergeben sich (als 8-Bit gesehen)
  	// folgende Adressen: 0x90 bei WRITE und 0x91 bei READ
  	
	IIC_Start_1 ();				// Read!!
	IIC_Send_Byte_1 (READ_ADRESS_AD7415_1);
	Reply= IIC_Get_Ack_1();
	AirTempCelsius_1 = IIC_Get_Byte_1 ();
	IIC_Put_Ack_1();
	AirTempViertelGrad_1 = (IIC_Get_Byte_1()>>6);
	IIC_Put_Nack_1();
	IIC_Stop_1();
	
	
	IIC_Start_1 ();				// Read!!
	IIC_Send_Byte_1 (READ_ADRESS_AD7415_2);
	Reply= IIC_Get_Ack_1();
	AirTempCelsius_2 = IIC_Get_Byte_1 ();
	IIC_Put_Ack_1();
	AirTempViertelGrad_2 = (IIC_Get_Byte_1()>>6);
	IIC_Put_Nack_1();
	IIC_Stop_1();

  }
}

//************************************************************************************************
//*                                                                                              *
//*     DoorSensor_PreparePacket: prepares the packet for the cc2500                             *
//*                                                                                              *
//************************************************************************************************
void AD7415_preparePacket_1(void)
{
	CC2500_sendPacketArr[0] = 5;
	CC2500_sendPacketArr[1] = PCB_MOD_X2 << 4;
	CC2500_sendPacketArr[1] += SENSOR_TEMP;
	CC2500_sendPacketArr[2] = 1;
	CC2500_sendPacketArr[3] = AirTempCelsius_1;
	CC2500_sendPacketArr[4] = AirTempViertelGrad_1;
	
}

//************************************************************************************************
//*                                                                                              *
//*     DoorSensor_PreparePacket: prepares the packet for the cc2500                             *
//*                                                                                              *
//************************************************************************************************
void AD7415_preparePacket_2(void)
{
	CC2500_sendPacketArr[0] = 5;
	CC2500_sendPacketArr[1] = PCB_MOD_X1 << 4;
	CC2500_sendPacketArr[1] += SENSOR_TEMP;
	CC2500_sendPacketArr[2] = 0;
	CC2500_sendPacketArr[3] = AirTempCelsius_2;
	CC2500_sendPacketArr[4] = AirTempViertelGrad_2;
	
}