#include "FunctionProto.h"
#include "Variablen.h"
#include "defines.h"
#include "MC9S08LL16.h"

#define		SPI_SSDD_CC2500		PTBDD_PTBDD7
#define		SPI_SSD_CC2500		PTBD_PTBD7
#define		SPI_SSDD_BME680		PTBDD_PTBDD3
#define		SPI_SSD_BME680		PTBD_PTBD3
#define		SPI_SPSCK			PTBDD_PTBDD6
#define		SPI_MOSI			PTBDD_PTBDD5
#define		SPI_MISO			PTBDD_PTBDD4


void SPI_Init(void)
{
	SPIC1 = 0b01010100;		// }
	SPIC1 = 0b01010000;		// > setting the SPI to master and other settings 
	SPIC2 = 0x00;			// }
	SPIBR = 0b00100010; 	//   setting the speed of the SPI (here busfrequenzy / 2)
	
	SPI_SSDD_CC2500 = 1;	// setting the pins of the SPI as input and output
	SPI_SSD_CC2500 = 1;
	
	SPI_SSDD_BME680 = 1;
	SPI_SSD_BME680 = 1;
	
	SPI_SPSCK = 1;
	SPI_MOSI = 1;
	SPI_MISO = 0;
	PTBDS = 1;	
}


// reads an Adress from the SPI (Adress is 
// absolute Adress. MSB will be set to 1 automatically)
unsigned char SPI_readReg(unsigned char Adress)
{
	unsigned char SPIByte;
	
	Adress |= 1UL << 7;
	
	__asm lda	SPIS;
	
	if(SS_type == 0)
	{	
		SPI_SSD_CC2500 = 0;
		while(SPI_MISO == 1){};
	}
	else
	{
		SPI_SSD_BME680 = 0;
	}
	
	
	SPI_WaitSend();
	SPID = Adress;
	SPI_Wait();
	
	__asm lda	SPIS;
	SPI_WaitSend();
	SPID = 0x00;
	
	SPI_Wait();
	
	SPI_SSD_CC2500 = 1;
	SPI_SSD_BME680 = 1;
	
	SPIByte = SPID;
	
	__asm lda	SPIS;
	
	return (SPIByte);
}

//sends a single strobe (hex byte)
void SPI_sendStrobe(unsigned char Strobe)
{
	__asm lda	SPIS;
	
	if(SS_type == 0)
	{	
		SPI_SSD_CC2500 = 0;
		while(SPI_MISO == 1){};
	}
	else
	{
		SPI_SSD_BME680 = 0;
	}
	
	
	SPI_WaitSend();
	SPID = Strobe;
	SPI_Wait();
	
	__asm lda	SPIS;
	
	SPI_SSD_CC2500 = 1;
	SPI_SSD_BME680 = 1;
}
 
//writes sendByte to Adress
void SPI_writeReg(unsigned char Adress, unsigned char sendByte)
{
	__asm lda	SPIS;
	
	if(SS_type == 0)
	{	
		SPI_SSD_CC2500 = 0;
		while(SPI_MISO == 1){};
	}
	else
	{
		SPI_SSD_BME680 = 0;
	}
	
	
	SPI_WaitSend();
	SPID = Adress;
	SPI_Wait();
	
	__asm lda	SPIS;
	SPI_WaitSend();
	SPID = sendByte;
	SPI_Wait();
	
	SPI_SSD_CC2500 = 1;
	SPI_SSD_BME680 = 1;
	
	__asm lda	SPIS;
}


//Waiting until the SPI is ready to send a Byte
void SPI_WaitSend(void)
{
	while(SPIS_SPTEF == 0){
	}
}

void SPI_Wait(void)
{
	uchar i;
	
	for(i = 0; i < 255; i++)
	{
	Nop();	
	}
}