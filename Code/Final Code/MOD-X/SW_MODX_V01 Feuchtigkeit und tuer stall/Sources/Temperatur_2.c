#include "Variablen.h"
#include "defines.h"
#include "FunctionProto.h"
#include "inputToCommand.h"

#define SCL_PIN_2		PTBD_PTBD6	  // Clock= PC0		
#define SCL_PORT_2	PTBDD_PTBDD6

#define SDA_PIN_2		PTBD_PTBD5	  // Data= PC1		
#define SDA_PORT_2	PTBDD_PTBDD5


#define READ_ADRESS_AD7415 0x93  // war 0x91
#define WRITE_ADRESS_AD7415 0x92  // war 0x90


/****************************************************************************/
/*                                                                          */
/* Init_AD7415_2: Macht nicht viel, sendet nur eine Standard-Konfiguration    */
/* zum AD7415 (die eigentlich die Reset-Kondition sein sollte, aber sicher  */
/* ist sicher) und setzt dann den Adress-Pointer auf das Register "0x00",   */
/* dem Temperatur-Register, um dann später einfach nur die Temperaturwerte  */
/* auslesen zu können.                                                      */
/*                                                                          */
/****************************************************************************/

void Init_AD7415_2 (void)
{

    Byte Reply;

	SDA_PORT_2 = 0;			// SDA und SCL sind EINGÄNGE, also HIGH
	SCL_PORT_2 = 0;
	AirDelay_2 = 0;
	
 	IIC_Start_2();
    IIC_Send_Byte_2 (WRITE_ADRESS_AD7415);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack_2();
	IIC_Send_Byte_2 (0x01);		// Configuration Register
	Reply= IIC_Get_Ack_2();
	IIC_Send_Byte_2 (0x40);		// 0100 0000  Filter auf 1, der Rest ist 00, gibt in Hex 0x40
	Reply= IIC_Get_Ack_2();
	IIC_Stop_2 ();

  	IIC_Start_2();                // Einfach nur Adresse "0x00" --> Adresspointer auf Temperatur-Register setzen
    IIC_Send_Byte_2 (WRITE_ADRESS_AD7415);       // Adresse (1001 001 0) (Write)
	Reply= IIC_Get_Ack_2();
	IIC_Send_Byte_2 (0x00);		// Adress-Pointer auf 0x00
	Reply= IIC_Get_Ack_2();

	
	IIC_Stop_2 ();
}


/****************************************************************************/
/*                                                                          */
/* 	IIC_Start_2: Startkondition, indem zuerst das Datenpin auf LOW geht,      */
/*  danach das CLOCKPIN.                                                    */
/*  Dadurch, dass der Pin als OUTPUT geschaltet wird und eine logische "0"  */
/*  ausgibt geht die Leitung auf LOW. Für ein HIGH wird die Leitung einfach */
/*  als INPUT geschaltet, somit wird sie hochohmig und der Pull-Up zieht    */
/*  die Leitung auf HIGH.                                                   */
/*                                                                          */
/****************************************************************************/

void IIC_Start_2 (void) {

	SDA_PORT_2 = 1;			// SDA-Pin wird als Ausgang definiert, mit Pegel = LOW
	SDA_PIN_2 = 0;			

	Nop();			     	// Zuerst DATA auf LOW, danach Clock
	Nop();                  // Das ist die Startkondition!!

	SCL_PORT_2 = 1;
	SCL_PIN_2 = 0;
}


/****************************************************************************/
/*                                                                          */
/*  IIC_Stop_2: Stop-Kondition: Zuerst geht Clock auf HIGH, danach DATA       */
/*                                                                          */
/****************************************************************************/


void IIC_Stop_2 (void){

	SDA_PORT_2 = 1;			// Sicherheitshalber mal beide Ports als Ausgang schalten!
	SCL_PORT_2 = 1;


	SCL_PORT_2 = 0;			// zuerst geht der CLOCK auf HIGH, indem er ein INPUT wird...
	Nop();
	Nop();
	SDA_PORT_2 = 0;           // und danach DATA! --> Das ist die STOP-Kondition

}

/****************************************************************************/
/*                                                                          */
/*  IIC_Send_Byte_2: Sendet ein Byte hinaus                                   */
/*                                                                          */
/****************************************************************************/



void IIC_Send_Byte_2 (unsigned char Wert){

Byte Counter,ShiftWert;

	ShiftWert=Wert;
	
	SDA_PORT_2 = 1;

	for (Counter=0; Counter <8; Counter++){
		if ((ShiftWert & 0x80)	== 0x80) {
		        SDA_PORT_2 = 0;                   // Sendet ein HIGH

		}
		else {
		        SDA_PORT_2 = 1;                   // Sendet ein LOW
		        SDA_PIN_2 = 0;
		        

		}
		
		SCL_PORT_2 = 0;		// Einen Clock geben  --> Clock auf HIGH

		SCL_PORT_2 = 1;       // Clock auf LOW
		SCL_PIN_2 = 0;

		ShiftWert = ShiftWert<<1;
	}
	SDA_PORT_2 = 0;           // Am Ende: Port als EINGANG (damit das ACK empfangen werden kann)

}

/****************************************************************************/
/*                                                                          */
/*  IIC_Get_Ack_2: Holt einen Acknowledge herein                              */
/*                                                                          */
/****************************************************************************/



unsigned char IIC_Get_Ack_2 (void){

unsigned char Response;

	SDA_PORT_2 = 0;			// Port wird als Eingang geschaltet (falls er es noch nicht war)

	Nop();

    SCL_PORT_2 = 0;    		// Einen Clock geben  --> SCL = HIGH
	if (SDA_PIN_2 == 1) {
	        Response = 0;

	}
	else {
	        Response = 1;

	}
	SCL_PORT_2 = 1;
	SCL_PIN_2 = 0;        // Clock wieder auf LOW

	
	SDA_PORT_2 = 1;		// Port wieder als AUSGANG schalten
	SDA_PIN_2 = 0;
	return (Response);
	
}


/****************************************************************************/
/*                                                                          */
/*  IIC_Get_Byte_2: Holt ein Byte herein                                      */
/*                                                                          */
/****************************************************************************/


unsigned char IIC_Get_Byte_2 (void){

unsigned char Response, Counter;

	SDA_PORT_2 = 0;			// Port als Eingang!

	for (Counter=0; Counter <8; Counter++){
		Response=Response<<1;
		
		SCL_PORT_2 = 0;			// Einen Clock geben  SCL = HIGH

		if (SDA_PIN_2 == 1) Response |=0x01;
		else Response &= 0xFE;			// Fraglich....!!!
		
		SCL_PORT_2 = 1;           // Clock wieder auf LOW
		SCL_PIN_2 = 0;


	}
	return (Response);
			
}

/****************************************************************************/
/*                                                                          */
/*  IIC_Put_Ack_2: Gibt einen Acknowledge auf die Datenleitung aus            */
/*                                                                          */
/****************************************************************************/

void IIC_Put_Ack_2(void)
{
	SDA_PORT_2 = 1;       // Datenleitung auf LOW
	SDA_PIN_2 = 0;
	
	SCL_PORT_2 = 0;        // Clock auf HIGH

	SCL_PORT_2 = 1;       // Clock wieder auf LOW
	SCL_PIN_2 = 0;        
	
	SDA_PORT_2 = 0;       // Datenleitung wieder freigeben (als Input setzen)
}

/****************************************************************************/
/*                                                                          */
/*  IIC_Put_Nack_2: Gibt einen NOT-Acknowledge auf die Datenleitung aus       */
/*                                                                          */
/****************************************************************************/

void IIC_Put_Nack_2(void)
{
	SDA_PORT_2 = 0;       // Datenleitung auf HIGH
		
	SCL_PORT_2 = 0;        // Clock auf HIGH

	SCL_PORT_2 = 1;       // Clock wieder auf LOW
	SCL_PIN_2 = 0;        
	
	SDA_PORT_2 = 1;       // Datenleitung wieder LOW
	SDA_PIN_2=0;
}


/****************************************************************************/
/*                                                                          */
/*  Read_Air_Temp_2: Liest zyklisch jede Sekunde die Temperatur aus.          */
/*  Das Auslesen dauert bei einem 4,9152MHz-Quarz knapp 300µs.              */
/*                                                                          */
/*  In die Variable "AirTempCelsius_2" kommt die Temperatur in Grad Celsius   */
/*  In die Variable "AirTempViertelGrad_2" die "Nachkommastellen" der Grad    */
/*  Celsius, wobei  "00" --> 0,00 °C                                        */
/*                  "01" --> 0,25 °C                                        */
/*                  "02" --> 0,50 °C                                        */
/*                  "03" --> 0,75 °C                                        */
/*                                                                          */
/****************************************************************************/


void Read_Air_Temp_2(void)
{


    unsigned char Reply;



  AirDelay_2++;
  if (AirDelay_2 >= 100)              // 100 Realtime-Zyklen á 10ms = 1 Sekunde
  {                                 // Auslesen einmal pro Sekunde, weil öfter eh kaum Sinn macht...
  
  	AirDelay_2 = 0;
  	
  	
  	// Beim Baustein handelt es sich um einen AD7415ARTZ-0
  	// Adresse des Bausteines mit AS-Pin auf GND: 1001 000 (0x48) ZUZÜGLICH dem R/W\-Bit als LSB.
  	// Da die Adresse nur 7 Bit lang ist und mit dem R/W-Bit 8 Bit rauskommen, ergeben sich (als 8-Bit gesehen)
  	// folgende Adressen: 0x90 bei WRITE und 0x91 bei READ
  	

	IIC_Start_2 ();				// Read!!
	IIC_Send_Byte_2 (READ_ADRESS_AD7415);
	Reply= IIC_Get_Ack_2();
	AirTempCelsius_2 = IIC_Get_Byte_2 ();
	IIC_Put_Ack_2();
	AirTempViertelGrad_2 = (IIC_Get_Byte_2()>>6);
	IIC_Put_Nack_2();
	IIC_Stop_2();

  }
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
	CC2500_sendPacketArr[2] = 1;
	CC2500_sendPacketArr[3] = AirTempCelsius_2;
	CC2500_sendPacketArr[4] = AirTempViertelGrad_2;
	
}