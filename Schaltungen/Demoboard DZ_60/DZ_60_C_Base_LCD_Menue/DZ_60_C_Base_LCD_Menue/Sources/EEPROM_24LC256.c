#include "Variablen.h"
#include "defines.h"

#define CLK_PIN         PTFD_PTFD2	  // Clock= PF2		
#define CLK_PORT        PTFDD_PTFDD2

#define CLK_SHOW_PIN    PTCD_PTCD0
#define CLK_SHOW_PORT   PTCDD_PTCDD0

#define SDA_PIN         PTFD_PTFD3	  // Data= PF3		
#define SDA_PORT        PTFDD_PTFDD3
#define SDA_SHOW_PIN    PTCD_PTCD2
#define SDA_SHOW_PORT   PTCDD_PTCDD2





void IIC_Start (void) {

	SDA_PORT = 1;			// Sicherheitshalber mal beide Ports als Ausgang schalten!
	SDA_SHOW_PORT = 1;
	
	CLK_PORT = 1;
	CLK_SHOW_PORT = 1;

	SDA_PIN = 0;			// Das Data-Pin geht auf Low bevor das Clock-Pin runter geht
	SDA_SHOW_PIN = 0;
	Nop();				// Das ist die Startkondition!!
	Nop();
	Nop();
	CLK_PIN = 0;
	CLK_SHOW_PIN = 0;

}

void IIC_Stop (void){

	SDA_PORT = 1;			// Sicherheitshalber mal beide Ports als Ausgang schalten!
	SDA_SHOW_PORT = 1;

	CLK_PIN = 1;
	CLK_SHOW_PIN = 1;
	Nop();
	Nop();
	Nop();
	SDA_PIN = 1;
	SDA_SHOW_PIN = 1;
}


void IIC_Send_Byte (unsigned char Wert){

Byte Counter,ShiftWert;

	ShiftWert=Wert;
	SDA_PORT = 1;

	for (Counter=0; Counter <8; Counter++){
		if ((ShiftWert & 0x80)	== 0x80) {
		        SDA_PIN = 1;
		        SDA_SHOW_PIN = 1;
		}
		else {
		        SDA_PIN = 0;
		        SDA_SHOW_PIN = 0;
		}
		Nop ();
		
		CLK_PIN = 1;		// Einen Clock geben
		CLK_SHOW_PIN = 1;
		Nop();
		Nop();
		CLK_PIN = 0;
		CLK_SHOW_PIN = 0;
		ShiftWert = ShiftWert<<1;
	}
	SDA_PORT = 0;

}




unsigned char IIC_Get_Ack (void){

unsigned char Response;

	SDA_PORT = 0;			// Port wird als Eingang geschaltet

	Nop();
	Nop();

	CLK_PIN=1;			// Einen Clock geben
	CLK_SHOW_PIN = 1;
	Nop();
	Nop();
	if (SDA_PIN == 1) {
	        Response = FALSE;
	        SDA_SHOW_PIN = 1;
	}
	else {
	        Response = TRUE;
	        SDA_SHOW_PIN = 0;
	}
	CLK_PIN = 0;
	CLK_SHOW_PIN = 0;
	
	SDA_PORT = 1;		// Port wieder als AUSGANG schalten
	return (Response);
	
}





unsigned char IIC_Get_Byte (void){

unsigned char Response, Counter;

	SDA_PORT = 0;			// Port als Eingang!

	for (Counter=0; Counter <8; Counter++){
		Response=Response>>1;
		CLK_PIN = 1;			// Einen Clock geben
		CLK_SHOW_PIN = 1;
		Nop();
		Nop();
		if (SDA_PIN == 1) Response |=0x80;
		else Response &= 0x7F;			// Fraglich....!!!
		CLK_PIN = 0;
		CLK_SHOW_PIN = 0;

	}
	return (Response);
			
}

void Store_One_Byte (void) {


	IIC_Start();
	IIC_Send_Byte (0xA0);		// Control Code = 1010  Chip Select Bits = 000   R/W\ = 0   1010 000 0 Bin = 0xA0
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// Adresse High Byte
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// Adresse Low Byte
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x01);		// Testweise mal "0x55" abspeichern!
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x02);		// Testweise mal "0x55" abspeichern!
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x03);		// Testweise mal "0x55" abspeichern!
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x04);		// Testweise mal "0x55" abspeichern!
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x05);		// Testweise mal "0x55" abspeichern!
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x06);		// Testweise mal "0x55" abspeichern!
	Reply= IIC_Get_Ack();
	
	IIC_Stop();
	Nop ();
}

void Recover_One_Byte (void){



    IIC_Start();
    IIC_Send_Byte (0xA0);           // Write
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// Adresse High Byte
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// Adresse Low Byte
	Reply= IIC_Get_Ack();
	IIC_Stop ();
	
	Nop ();
	
	IIC_Start ();				// Read!!
	IIC_Send_Byte (0xA1);
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// Adresse High Byte
	Reply= IIC_Get_Ack();
	IIC_Send_Byte (0x00);		// Adresse Low Byte
	Reply= IIC_Get_Ack();
	
    Ergebnis=IIC_Get_Byte();
    Reply= IIC_Get_Ack();
    
	IIC_Stop();
	Nop ();
}

