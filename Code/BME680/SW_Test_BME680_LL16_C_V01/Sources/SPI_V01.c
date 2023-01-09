#include "FunctionProto.h"
#include "Variablen.h"
#include "defines.h"
#include "MC9S08LL16.h"

#define		SPI_SSDD			PTBDD_PTBDD3
#define		SPI_SSD				PTBD_PTBD3
#define		SPI_SPSCK			PTBDD_PTBDD6
#define		SPI_MOSI			PTBDD_PTBDD5
#define		SPI_MISO			PTBDD_PTBDD4


void SPI_Init(void)
{
	SPIC1 = 0b01010100;
	SPIC1 = 0b01010000;
	SPIC2 = 0x00;
	SPIBR = 0b00100010;
	
	SPI_SSDD = 1;
	SPI_SSD = 1;
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
	
	SPI_SSD = 0;
	
	SPI_WaitSend();
	SPID = Adress;
	SPI_Wait();
	
	__asm lda	SPIS;
	SPI_WaitSend();
	SPID = 0x00;
	
	SPI_Wait();
	SPI_SSD = 1;
	
	SPIByte = SPID;
	
	__asm lda	SPIS;
	
	return (SPIByte);
}

void SPI_writeReg(unsigned char Adress, unsigned char sendByte)
{
	__asm lda	SPIS;
	
	SPI_SSD = 0;
	
	SPI_WaitSend();
	SPID = Adress;
	SPI_Wait();
	
	__asm lda	SPIS;
	SPI_WaitSend();
	SPID = sendByte;
	SPI_Wait();
	
	SPI_SSD = 1;
	
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
	unsigned char i;
	
	for(i = 0; i < 15; i++)
	{
	Nop();	
	}
}