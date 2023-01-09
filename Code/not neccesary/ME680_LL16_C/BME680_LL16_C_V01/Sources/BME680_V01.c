//**************************************************************
//
//	SPI Status Register (SPIS)
//	contains important information: SPRF (is there
//	a byte safed in SPID that has to be read), SPTEF
//	(can i safe a byte into the SPID that has to be 
//	transferred), MODF (is there a second master 
//	which would cause a fail)
//
//	SPI Data Register SPID
//


#include "Variablen.h"
#include "defines.h"

 
void check_BME680 (void)
{
	
}

//************************************************************************************************
//*                                                                                              *
//*      EEPROM_Write_Byte: Schreibt ein Byte an Speicherzelle $0000 - und zwar mit dem          *
//*      Wert, der in "EE_Wert" gespeichert ist, an Adresse in "EE_Adresse"                      *
//*                                                                                              *
//************************************************************************************************


void BME680_Init (void){
	//setup SPI Control register 1 for SPI data transfer (done in MC9S08LL16.h)
//asm{
	//bclr	7, SPI1C1		//0 = SPI interrupt enable OFF 
	//bset	6, SPI1C1		//1 = spi active
	//bclr	5, SPI1C1		//0 = SPI transmitter interrup enable OFF
	//bset	4, SPI1C1		//1 = is master (0 = is slave)
	//bset	3, SPI1C1		//1 = when SS LOW data transfer start
	//bset	2, SPI1C1		//1 when first edge of sck is start of byte transfer
	//bset	1, SPI1C1		//1 = SS active (MODFEN = 1 then automatic, MODFEN = 0 then input for mode fault)
	//bclr	0, SPI1C1		//1 = LSB first, 0 = MSB first
	
	
	//setup SPI Control register 2 for SPI data transfer
	
	//bset	4, SPI1C2		//1 = MODFEN for active SS
	//bclr	3, SPI1C2		//0 = bidirectional mode OFF (SPC0 = 0 MUST BE)

	//bclr	1, SPI1C2		//0 = continue data transfer if MCU in wait mode
	//bclr	0, SPI1C2		//0 = SPC0 is LOW for MOSI MISO, 1 for MOMI, SISO	
	
	
	//setup SPI baud rate register
	
	//lda	#%00100100		//all in one set
	//sta	SPI1BR
	//bclr	6, SPI1BR		//first 3 are for the divider of the pre scale
	//bset	5, SPI1BR
	//bclr	4, SPI1BR
	
	//bset	2, SPI1BR		//last 3 for the division of the pre scale which is the final output
	//bclr	1, SPI1BR
	//bclr	0, SPI1BR
	
	
	//end init spi. everything is set i hope
	//}
	
	
/*** SPIC1 - SPI Control Register 1; 0x00000028 ***/
SPIC1 = 0x5E;

//typedef union {
//  byte Byte;
//  struct {
//    byte LSBFE       :0;                                       /* LSB First (Shifter Direction)
//    byte SSOE        :1;                                       /* Slave Select Output Enable
//    byte CPHA        :1;                                       /* Clock Phase
//    byte CPOL        :1;                                       /* Clock Polarity
//    byte MSTR        :1;                                       /* Master/Slave Mode Select
//    byte SPTIE       :0;                                       /* SPI Transmit Interrupt Enable
//    byte SPE         :1;                                       /* SPI System Enable
//    byte SPIE        :0;                                       /* SPI Interrupt Enable (for SPRF and MODF)
//  } Bits;
//} SPIC1STR;


/*** SPIC2 - SPI Control Register 2; 0x00000029 ***/
SPIC2 = 0xF4;

/*typedef union {
  byte Byte;
  struct {
    byte SPC0        :0;                                       SPI Pin Control 0
    byte SPISWAI     :0;                                       SPI Stop in Wait Mode
    byte             :1; 
    byte BIDIROE     :0;                                       Bidirectional Mode Output Enable
    byte MODFEN      :1;                                       Master Mode-Fault Function Enable
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} SPIC2STR;
*/


/*** SPIBR - SPI Baud Rate Register; 0x0000002A ***/
SPIBR = 0xCC;

/*
typedef union {
  byte Byte;
  struct {
    byte SPR0        :0;                                       SPI Baud Rate Divisor Bit 0
    byte SPR1        :0;                                       SPI Baud Rate Divisor Bit 1
    byte SPR2        :1;                                       SPI Baud Rate Divisor Bit 2
    byte             :1; 
    byte SPPR0       :0;                                       SPI Baud Rate Prescale Divisor Bit 0
    byte SPPR1       :0;                                       SPI Baud Rate Prescale Divisor Bit 1
    byte SPPR2       :1;                                       SPI Baud Rate Prescale Divisor Bit 2
    byte             :1; 
  } Bits;
  struct {
    byte grpSPR  :3;
    byte         :1;
    byte grpSPPR :3;
    byte         :1;
  } MergedBits;
} SPIBRSTR;
*/


}