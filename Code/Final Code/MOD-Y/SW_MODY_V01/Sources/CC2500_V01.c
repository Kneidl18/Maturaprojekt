#include "CC2500_VAL.h"
#include "CC2500_REG.h"
#include "FunctionProto.h"
#include "Variablen.h"
#include "MC9S08LL16.h"
#include "defines.h"


#define CC2500_IDLE    0x36      // Exit RX / TX, turn
#define CC2500_TX      0x35      // Enable TX. If in RX state, only enable TX if CCA passes
#define CC2500_RX      0x34      // Enable RX. Perform calibration if enabled
#define CC2500_FTX     0x3B      // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC2500_FRX     0x3A      // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC2500_SWOR    0x38
#define CC2500_TXFIFO  0x3F
#define CC2500_RXFIFO  0x3F

#define PACKET_LENGHT_MAX 	20

#define MISO_PIN		PTBD_PTBD4
#define CC2500_GDO0		PTCD_PTCD0
#define CC2500_GDO0_DD	PTCDD_PTCDD0
#define CC2500_GDO1		PTCD_PTCD1
#define CC2500_GDO1_DD	PTCDD_PTCDD1


//listens for a package income on the cc2500
void CC2500_listenForPacket(void)
{	
	char PacketLength;
	Byte rssi, lqi;
	uchar i;

	SPI_sendStrobe(CC2500_RX);
	
	SPI_writeReg(REG_IOCFG1, 0x01);
	
	Nop();
	Nop();
	
  	if (MISO_PIN == 1) 
  	{      
	    PacketLength = SPI_readReg(CC2500_RXFIFO);
	    
	   	if(PacketLength <= PACKET_LENGHT_MAX)
	   	{		  
			for(i = 1; i < PacketLength; i++)
			{
				CC2500_recvPacketArr[i] = SPI_readReg(CC2500_RXFIFO);
			}
		    // store quality information
		    rssi = SPI_readReg(CC2500_RXFIFO);
		    lqi = SPI_readReg(CC2500_RXFIFO);
	   	}
	   	else
	   	{
	   		// ERROR packet is longer then expected
	   	}
	   	
	    // Make sure that the radio is in IDLE state before flushing the FIFO
	    // (Unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point) 
	   	SPI_sendStrobe(CC2500_IDLE);
	    // Flush RX FIFO
	   	SPI_sendStrobe(CC2500_FRX);
	   	
	   	CC2500_packetReceived = 1;
	   	
	   	i = 0;
	   	Nop();
	   	Nop();
   	
  	}
  	else { }
}


//sends a Packet stored in sendPacketArr over the cc2500. lenght of the packet is stored in sendPacketArr[0]
void CC2500_sendPacket(void)
{
	byte crc;
	int i;
	
	
	//first byte to be send = length of the package
	
	//SPI_writeReg(REG_IOCFG1, 0x06);
	// Make sure that the radio is in IDLE state before flushing the FIFO
	SPI_sendStrobe(CC2500_IDLE);
	// Flush TX FIFO
	SPI_sendStrobe(CC2500_FTX);
	
	// SIDLE: exit RX/TX
	SPI_sendStrobe(CC2500_IDLE);
	
	crc = createCRC();
	
	CC2500_sendPacketArr[CC2500_sendPacketArr[0]] = crc;
	CC2500_sendPacketArr[0]++;
	
	//for(i = CC2500_sendPacketArr[0]; i > 0; i--)
	//{
	//	CC2500_sendPacketArr[i] = CC2500_sendPacketArr[i - 1];
	//}
	//CC2500_sendPacketArr[0] = 126;
	
	// send packet to CC2500
	for(i = 0; i < CC2500_sendPacketArr[0]; i++)
	{
		SPI_writeReg(CC2500_TXFIFO, CC2500_sendPacketArr[i]);
	}
	
	Nop();
	Nop();
	
	// STX: exable TX
	SPI_sendStrobe(CC2500_TX);
	
	//TODO idk why this doesnt work but it works without it too 
	// Wait for GDO0 to be set -> sync transmitted
	//while(CC2500_GDO0 == 0 && MISO_PIN == 0){ }
	
	// Wait for GDO0 ot be cleared -> end of Packet
	//while(CC2500_GDO0 == 1 && MISO_PIN == 1){ }
	
	//SPI_sendStrobe(CC2500_IDLE);
	
   	Nop();
   	Nop();
}

// creates a crc out of the send package by linking them via and and xor
unsigned char createCRC(void)
{
	unsigned char lenght;
	unsigned char crc1;
	unsigned char crc2;
	int i;
	crc1 = 0;
	crc2 = 0;
	
	lenght = CC2500_sendPacketArr[0];
	
	for(i = 0; i < lenght; i++)
	{
		crc1 = crc1 + CC2500_sendPacketArr[i];
	}
	
	for(i = 0; i < lenght; i++)
	{
	//	crc2 = crc2 ^ CC2500_sendPacketArr[i];
	}
	
	return (crc1 ^ crc2);
}

//initializes the CC2500 (registers and GD0, GD1)
void CC2500_Init(void)
{
	
	
	CC2500_GDO0_DD = 0;
	CC2500_GDO1_DD = 0;
	
	SPI_writeReg(REG_IOCFG2,0x06);
  	SPI_writeReg(REG_IOCFG0,0x01);
  	SPI_writeReg(REG_IOCFG1,0x06);

  	SPI_writeReg(REG_FIFOTHR, 0x02);
 	SPI_writeReg(REG_SYNC1,VAL_SYNC1);
 	SPI_writeReg(REG_SYNC0,VAL_SYNC0);
 	SPI_writeReg(REG_PKTLEN,VAL_PKTLEN);
  	SPI_writeReg(REG_PKTCTRL1,0x8C);
  	SPI_writeReg(REG_PKTCTRL0, 0x0D);
  
  	SPI_writeReg(REG_ADDR,VAL_ADDR);
  	SPI_writeReg(REG_CHANNR,VAL_CHANNR);
  	SPI_writeReg(REG_FSCTRL1,VAL_FSCTRL1);
  	SPI_writeReg(REG_FSCTRL0,VAL_FSCTRL0);
  	SPI_writeReg(REG_FREQ2,VAL_FREQ2);
  	SPI_writeReg(REG_FREQ1,VAL_FREQ1);
  	SPI_writeReg(REG_FREQ0,VAL_FREQ0);
  	SPI_writeReg(REG_MDMCFG4,VAL_MDMCFG4);
  	SPI_writeReg(REG_MDMCFG3,VAL_MDMCFG3);
  	SPI_writeReg(REG_MDMCFG2,VAL_MDMCFG2);
  	SPI_writeReg(REG_MDMCFG1,VAL_MDMCFG1);
  	SPI_writeReg(REG_MDMCFG0,VAL_MDMCFG0);
  	SPI_writeReg(REG_DEVIATN,VAL_DEVIATN);
  	SPI_writeReg(REG_MCSM2,VAL_MCSM2);
  	SPI_writeReg(REG_MCSM1,VAL_MCSM1);
  	SPI_writeReg(REG_MCSM0,VAL_MCSM0);
  	SPI_writeReg(REG_FOCCFG,VAL_FOCCFG);

  	SPI_writeReg(REG_BSCFG,VAL_BSCFG);
  	SPI_writeReg(REG_AGCCTRL2,0x00);
  	SPI_writeReg(REG_AGCCTRL1,0x40);
  	SPI_writeReg(REG_AGCCTRL0,VAL_AGCCTRL0);
  	SPI_writeReg(REG_WOREVT1,VAL_WOREVT1);
  	SPI_writeReg(REG_WOREVT0,VAL_WOREVT0);
  	SPI_writeReg(REG_WORCTRL,0x78);
  	SPI_writeReg(REG_FREND1,VAL_FREND1);
  	SPI_writeReg(REG_FREND0,VAL_FREND0);
  	SPI_writeReg(REG_FSCAL3,VAL_FSCAL3);
  	SPI_writeReg(REG_FSCAL2,VAL_FSCAL2);
  	SPI_writeReg(REG_FSCAL1,VAL_FSCAL1);
  	SPI_writeReg(REG_FSCAL0,VAL_FSCAL0);
  	SPI_writeReg(REG_RCCTRL1,VAL_RCCTRL1);
  	SPI_writeReg(REG_RCCTRL0,VAL_RCCTRL0);
  	SPI_writeReg(REG_FSTEST,VAL_FSTEST);
  	SPI_writeReg(REG_PTEST,VAL_PTEST);
  	SPI_writeReg(REG_AGCTEST,VAL_AGCTEST);
  	SPI_writeReg(REG_TEST2,VAL_TEST2);
  	SPI_writeReg(REG_TEST1,VAL_TEST1);
  	SPI_writeReg(REG_TEST0,VAL_TEST0);
  	
	SPI_writeReg(REG_PARTNUM,VAL_PARTNUM);
	SPI_writeReg(REG_VERSION,VAL_VERSION);
	SPI_writeReg(REG_FREQEST,VAL_FREQEST);
	SPI_writeReg(REG_LQI,VAL_LQI);
	SPI_writeReg(REG_RSSI,VAL_RSSI);
	SPI_writeReg(REG_MARCSTATE,VAL_MARCSTATE);
	SPI_writeReg(REG_WORTIME1,VAL_WORTIME1);
	SPI_writeReg(REG_WORTIME0,VAL_WORTIME0);
	SPI_writeReg(REG_PKTSTATUS,VAL_PKTSTATUS);
	SPI_writeReg(REG_VCO_VC_DAC,VAL_VCO_VC_DAC);
	SPI_writeReg(REG_TXBYTES,VAL_TXBYTES);
	SPI_writeReg(REG_RXBYTES,VAL_RXBYTES);
	SPI_writeReg(REG_RCCTRL1_STATUS,VAL_RCCTRL1_STATUS);
	SPI_writeReg(REG_RCCTRL0_STATUS,VAL_RCCTRL0_STATUS);
  	
  	CC2500_readConfigRegs();
}

void CC2500_readConfigRegs(void)
{
	Byte temp;
	
	
	
	temp = SPI_readReg(REG_IOCFG2);
	__asm lda temp;
   
  	temp = 	SPI_readReg(REG_IOCFG1);
	__asm lda temp;
   
  	temp = SPI_readReg(REG_IOCFG0);
	__asm lda temp;
   
/* 	SPI_readReg(REG_FIFOTHR);
   
  	SPI_readReg(REG_SYNC1);
   
  	SPI_readReg(REG_SYNC0);
   
  	SPI_readReg(REG_PKTLEN);
   
  	SPI_readReg(REG_PKTCTRL1);
   
  	SPI_readReg(REG_PKTCTRL0);
   
  	SPI_readReg(REG_ADDR);
   
  	SPI_readReg(REG_CHANNR);
   
  	SPI_readReg(REG_FSCTRL1);
   
  	SPI_readReg(REG_FSCTRL0);
   
  	SPI_readReg(REG_FREQ2);
   
  	SPI_readReg(REG_FREQ1);
   
  	SPI_readReg(REG_FREQ0);
   
  	SPI_readReg(REG_MDMCFG4);
   
  	SPI_readReg(REG_MDMCFG3);
   
  	SPI_readReg(REG_MDMCFG2);
   
  	SPI_readReg(REG_MDMCFG1);
   
  	SPI_readReg(REG_MDMCFG0);
   
  	SPI_readReg(REG_DEVIATN);
   
  	SPI_readReg(REG_MCSM2);
   
  	SPI_readReg(REG_MCSM1);
   
  	SPI_readReg(REG_MCSM0);
   
  	SPI_readReg(REG_FOCCFG);
   

  	SPI_readReg(REG_BSCFG);
   
  	SPI_readReg(REG_AGCCTRL2);
   
  	SPI_readReg(REG_AGCCTRL1);
   
  	SPI_readReg(REG_AGCCTRL0);
   
  	SPI_readReg(REG_WOREVT1);
   
  	SPI_readReg(REG_WOREVT0);
   
  	SPI_readReg(REG_WORCTRL);
   
  	SPI_readReg(REG_FREND1);
   
  	SPI_readReg(REG_FREND0);
   
  	SPI_readReg(REG_FSCAL3);
   
  	SPI_readReg(REG_FSCAL2);
   
  	SPI_readReg(REG_FSCAL1);
   
  	SPI_readReg(REG_FSCAL0);
   
  	SPI_readReg(REG_RCCTRL1);
   
  	SPI_readReg(REG_RCCTRL0);
   
  	SPI_readReg(REG_FSTEST);
   
  	SPI_readReg(REG_PTEST);
   
  	SPI_readReg(REG_AGCTEST);
   
  	SPI_readReg(REG_TEST2);
   
  	SPI_readReg(REG_TEST1);
   
  	SPI_readReg(REG_TEST0);
   
 
  	SPI_readReg(REG_PARTNUM);
   
  	SPI_readReg(REG_VERSION);
   
  	SPI_readReg(REG_FREQEST);
   
  	SPI_readReg(REG_LQI);
   
  	SPI_readReg(REG_RSSI);
   
  	SPI_readReg(REG_MARCSTATE);
   
  	SPI_readReg(REG_WORTIME1);
   
  	SPI_readReg(REG_WORTIME0);
   
  	SPI_readReg(REG_PKTSTATUS);
   
  	SPI_readReg(REG_VCO_VC_DAC);
   
  	SPI_readReg(REG_TXBYTES);
   
  	SPI_readReg(REG_RXBYTES);
   
  	SPI_readReg(REG_RCCTRL1_STATUS);
   
  	SPI_readReg(REG_RCCTRL0_STATUS);
  */	
}