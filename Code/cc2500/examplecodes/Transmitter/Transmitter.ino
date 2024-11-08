/*
* Transmitter file is designed to transmit a packet of 3 bytes.
* -------------------------------------------------------------
* The Packet which is to be sent is hard-coded.
*
* Pattern for the Packet is: 
* 1st Byte = Length of Packet
* 2nd Byte = Your Data
* 3rd Byte = More Data
* .
* .
* Nth Byte = Last Byte of Data
*
* -------------------------------------------------------------
*
* Hard-coded Packet for this sample program is:
* 1st Byte = 3;    // Length of Packet
* 2nd Byte = 0x09;
* 3rd Byte = 0x01;
*
* Receiver will get the Packet in the above Pattern.
*
* -------------------------------------------------------------
*
* To run this program properly, do the following steps:
*
* 1. Connect the GDO0-Pin of CC2500 With Arduino's Pin-4
* 2. Connect a Push Button(Active-High) with Arduino's Pin-2
* 3. Transmitter will continously trransmit the Hard-coded Packet Until You Release the Push-Button to Low
*
* -------------------------------------------------------------
*
* Precautions:
* - Do appropriate current and voltage conversion between your microcontroller and CC2500 module.
* - High voltage or High current may damage your CC2500 Module.
*/

#define REG_IOCFG2           0x0000
#define REG_IOCFG1           0x0001
#define REG_IOCFG0           0x0002
#define REG_FIFOTHR          0x0003
#define REG_SYNC1            0x0004
#define REG_SYNC0            0x0005
#define REG_PKTLEN           0x0006
#define REG_PKTCTRL1         0x0007
#define REG_PKTCTRL0         0x0008
#define REG_ADDR             0x0009
#define REG_CHANNR           0x000A
#define REG_FSCTRL1          0x000B
#define REG_FSCTRL0          0x000C
#define REG_FREQ2            0x000D
#define REG_FREQ1            0x000E
#define REG_FREQ0            0x000F
#define REG_MDMCFG4          0x0010
#define REG_MDMCFG3          0x0011
#define REG_MDMCFG2          0x0012
#define REG_MDMCFG1          0x0013
#define REG_MDMCFG0          0x0014
#define REG_DEVIATN          0x0015
#define REG_MCSM2            0x0016
#define REG_MCSM1            0x0017
#define REG_MCSM0            0x0018
#define REG_FOCCFG           0x0019
#define REG_BSCFG            0x001A
#define REG_AGCCTRL2         0x001B
#define REG_AGCCTRL1         0x001C
#define REG_AGCCTRL0         0x001D
#define REG_WOREVT1          0x001E
#define REG_WOREVT0          0x001F
#define REG_WORCTRL          0x0020
#define REG_FREND1           0x0021
#define REG_FREND0           0x0022
#define REG_FSCAL3           0x0023
#define REG_FSCAL2           0x0024
#define REG_FSCAL1           0x0025
#define REG_FSCAL0           0x0026
#define REG_RCCTRL1          0x0027
#define REG_RCCTRL0          0x0028
#define REG_FSTEST           0x0029
#define REG_PTEST            0x002A
#define REG_AGCTEST          0x002B
#define REG_TEST2            0x002C
#define REG_TEST1            0x002D
#define REG_TEST0            0x002E
#define REG_PARTNUM          0x0030
#define REG_VERSION          0x0031
#define REG_FREQEST          0x0032
#define REG_LQI              0x0033
#define REG_RSSI             0x0034
#define REG_MARCSTATE        0x0035
#define REG_WORTIME1         0x0036
#define REG_WORTIME0         0x0037
#define REG_PKTSTATUS        0x0038
#define REG_VCO_VC_DAC       0x0039
#define REG_TXBYTES          0x003A
#define REG_RXBYTES          0x003B
#define REG_RCCTRL1_STATUS   0x003C
#define REG_RCCTRL0_STATUS   0x003D
//#include <cc2500_REG.h>

#define VAL_IOCFG2           0x29
#define VAL_IOCFG1           0x2E
#define VAL_IOCFG0           0x06
#define VAL_FIFOTHR          0x07
#define VAL_SYNC1            0xD3
#define VAL_SYNC0            0x91
#define VAL_PKTLEN           0xFF
#define VAL_PKTCTRL1         0x04
#define VAL_PKTCTRL0         0x05
#define VAL_ADDR             0x00
#define VAL_CHANNR           0x00
#define VAL_FSCTRL1          0x08
#define VAL_FSCTRL0          0x00
#define VAL_FREQ2            0x5D
#define VAL_FREQ1            0x93
#define VAL_FREQ0            0xB1
#define VAL_MDMCFG4          0x86
#define VAL_MDMCFG3          0x83
#define VAL_MDMCFG2          0x03
#define VAL_MDMCFG1          0x22
#define VAL_MDMCFG0          0xF8
#define VAL_DEVIATN          0x44
#define VAL_MCSM2            0x07
#define VAL_MCSM1            0x30
#define VAL_MCSM0            0x18
#define VAL_FOCCFG           0x16
#define VAL_BSCFG            0x6C
#define VAL_AGCCTRL2         0x03
#define VAL_AGCCTRL1         0x40
#define VAL_AGCCTRL0         0x91
#define VAL_WOREVT1          0x87
#define VAL_WOREVT0          0x6B
#define VAL_WORCTRL          0xF8
#define VAL_FREND1           0x56
#define VAL_FREND0           0x10
#define VAL_FSCAL3           0xA9
#define VAL_FSCAL2           0x0A
#define VAL_FSCAL1           0x00
#define VAL_FSCAL0           0x11
#define VAL_RCCTRL1          0x41
#define VAL_RCCTRL0          0x00
#define VAL_FSTEST           0x59
#define VAL_PTEST            0x7F
#define VAL_AGCTEST          0x3F
#define VAL_TEST2            0x88
#define VAL_TEST1            0x31
#define VAL_TEST0            0x0B
#define VAL_PARTNUM          0x80
#define VAL_VERSION          0x03
#define VAL_FREQEST          0x00
#define VAL_LQI              0x00
#define VAL_RSSI             0x00
#define VAL_MARCSTATE        0x00
#define VAL_WORTIME1         0x00
#define VAL_WORTIME0         0x00
#define VAL_PKTSTATUS        0x00
#define VAL_VCO_VC_DAC       0x00
#define VAL_TXBYTES          0x00
#define VAL_RXBYTES          0x00
#define VAL_RCCTRL1_STATUS   0x00
#define VAL_RCCTRL0_STATUS   0x00
//#include <cc2500_VAL.h>

#include <SPI.h>

#define CC2500_IDLE    0x36      // Exit RX / TX, turn
#define CC2500_TX      0x35      // Enable TX. If in RX state, only enable TX if CCA passes
#define CC2500_RX      0x34      // Enable RX. Perform calibration if enabled
#define CC2500_FTX     0x3B      // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC2500_FRX     0x3A      // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC2500_TXFIFO  0x3F
#define CC2500_RXFIFO  0x3F

#define No_of_Bytes    3

const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

const int GDO0_PIN = 4;     // the number of the GDO0_PIN pin
int GDO0_State = 0;         // variable for reading the pushbutton status

void setup()
{
  Serial.begin(9600);
  pinMode(SS,OUTPUT);
  SPI.begin();
  digitalWrite(SS,HIGH);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);     
  pinMode(GDO0_PIN, INPUT);     

  Serial.println("Starting..");
  init_CC2500();

  /* This function is to make sure that cc2500 is successfully configured.
   * This function read values of some registers from CC2500. 
   * To use this function, you must read the register values from 
   * -> Arduino-CC2500-Library / CC2500_Library / cc2500_VAL.h
   * Then compare the values from result of this function..
   */
  Read_Config_Regs();
}

void loop()
{
    
    
    buttonState = digitalRead(buttonPin);
    //Serial.println(buttonState);
    
//  To start transmission
    while (buttonState)
      {
        // read the state of the pushbutton value:
        buttonState = digitalRead(buttonPin);
        Serial.println("Transmission to start");
        delay(10);
        TxData_RF(No_of_Bytes);    //  Transmit No_of_Bytes-1
        Serial.println("Transmission is over");
        delay(10);
      }
          
     /* 
    while (buttonState)
      {
      // read the state of the pushbutton value:
      buttonState = digitalRead(buttonPin);
      Serial.println("PB = 1");
      }
      */
}

//  Send slide strobe
void TxData_RF( unsigned char length)
{
      // Make sure that the radio is in IDLE state before flushing the FIFO
      SendStrobe(CC2500_IDLE);
      // Flush TX FIFO
      SendStrobe(CC2500_FTX);

      // prepare Packet
      unsigned char packet[length];
      // First Byte = Length Of Packet
      packet[0] = length;
      packet[1] = 0x09;
      packet[2] = 0x01;
      /*
      for(int i = 1; i < length; i++)
      {	        	
          packet[i] = i;
      }
      
      */
      
      // SIDLE: exit RX/TX
      SendStrobe(CC2500_IDLE);
      
      for(int i = 0; i < length; i++)
      {	  
          Serial.println("Transmitting ");
          Serial.println(packet[i],HEX);
          WriteReg(CC2500_TXFIFO,packet[i]);
      }
      // STX: enable TX
      SendStrobe(CC2500_TX);
      
      // Wait for GDO0 to be set -> sync transmitted
      while (!GDO0_State)
      {
          // read the state of the GDO0_PIN value:
          GDO0_State = digitalRead(GDO0_PIN);
          //Serial.println("GDO0 = 0");
       }
       
       // Wait for GDO0 to be cleared -> end of packet
       while (GDO0_State)
       {
           // read the state of the GDO0_PIN value:
           GDO0_State = digitalRead(GDO0_PIN);
           //Serial.println("GDO0 = 1");
       }
}// Rf TX Packet 
	

void WriteReg(char addr, char value)
{
  digitalWrite(SS,LOW);
  
  while (digitalRead(MISO) == HIGH) {
    };
    
  SPI.transfer(addr);
  delay(10);
  SPI.transfer(value);
  digitalWrite(SS,HIGH);
}

char ReadReg(char addr)
{
  addr = addr + 0x80;
  digitalWrite(SS,LOW);
  while (digitalRead(MISO) == HIGH) {
    };
  char x = SPI.transfer(addr);
  delay(10);
  char y = SPI.transfer(0);
  digitalWrite(SS,HIGH);
  return y;  
}

char SendStrobe(char strobe)
{
  digitalWrite(SS,LOW);
  
  while (digitalRead(MISO) == HIGH) {
    };
    
  char result =  SPI.transfer(strobe);
  digitalWrite(SS,HIGH);
  delay(10);
  return result;
}


void init_CC2500()
{
  WriteReg(REG_IOCFG2,VAL_IOCFG2);
  WriteReg(REG_IOCFG1,VAL_IOCFG1);
  WriteReg(REG_IOCFG0,VAL_IOCFG0);

  WriteReg(REG_FIFOTHR,VAL_FIFOTHR);
  WriteReg(REG_SYNC1,VAL_SYNC1);
  WriteReg(REG_SYNC0,VAL_SYNC0);
  WriteReg(REG_PKTLEN,VAL_PKTLEN);
  WriteReg(REG_PKTCTRL1,VAL_PKTCTRL1);
  WriteReg(REG_PKTCTRL0,VAL_PKTCTRL0);
  WriteReg(REG_ADDR,VAL_ADDR);
  WriteReg(REG_CHANNR,VAL_CHANNR);
  WriteReg(REG_FSCTRL1,VAL_FSCTRL1);
  WriteReg(REG_FSCTRL0,VAL_FSCTRL0);
  WriteReg(REG_FREQ2,VAL_FREQ2);
  WriteReg(REG_FREQ1,VAL_FREQ1);
  WriteReg(REG_FREQ0,VAL_FREQ0);
  WriteReg(REG_MDMCFG4,VAL_MDMCFG4);
  WriteReg(REG_MDMCFG3,VAL_MDMCFG3);
  WriteReg(REG_MDMCFG2,VAL_MDMCFG2);
  WriteReg(REG_MDMCFG1,VAL_MDMCFG1);
  WriteReg(REG_MDMCFG0,VAL_MDMCFG0);
  WriteReg(REG_DEVIATN,VAL_DEVIATN);
  WriteReg(REG_MCSM2,VAL_MCSM2);
  WriteReg(REG_MCSM1,VAL_MCSM1);
  WriteReg(REG_MCSM0,VAL_MCSM0);
  WriteReg(REG_FOCCFG,VAL_FOCCFG);

  WriteReg(REG_BSCFG,VAL_BSCFG);
  WriteReg(REG_AGCCTRL2,VAL_AGCCTRL2);
  WriteReg(REG_AGCCTRL1,VAL_AGCCTRL1);
  WriteReg(REG_AGCCTRL0,VAL_AGCCTRL0);
  WriteReg(REG_WOREVT1,VAL_WOREVT1);
  WriteReg(REG_WOREVT0,VAL_WOREVT0);
  WriteReg(REG_WORCTRL,VAL_WORCTRL);
  WriteReg(REG_FREND1,VAL_FREND1);
  WriteReg(REG_FREND0,VAL_FREND0);
  WriteReg(REG_FSCAL3,VAL_FSCAL3);
  WriteReg(REG_FSCAL2,VAL_FSCAL2);
  WriteReg(REG_FSCAL1,VAL_FSCAL1);
  WriteReg(REG_FSCAL0,VAL_FSCAL0);
  WriteReg(REG_RCCTRL1,VAL_RCCTRL1);
  WriteReg(REG_RCCTRL0,VAL_RCCTRL0);
  WriteReg(REG_FSTEST,VAL_FSTEST);
  WriteReg(REG_PTEST,VAL_PTEST);
  WriteReg(REG_AGCTEST,VAL_AGCTEST);
  WriteReg(REG_TEST2,VAL_TEST2);
  WriteReg(REG_TEST1,VAL_TEST1);
  WriteReg(REG_TEST0,VAL_TEST0);
 
  WriteReg(REG_PARTNUM,VAL_PARTNUM);
  WriteReg(REG_VERSION,VAL_VERSION);
  WriteReg(REG_FREQEST,VAL_FREQEST);
  WriteReg(REG_LQI,VAL_LQI);
  WriteReg(REG_RSSI,VAL_RSSI);
  WriteReg(REG_MARCSTATE,VAL_MARCSTATE);
  WriteReg(REG_WORTIME1,VAL_WORTIME1);
  WriteReg(REG_WORTIME0,VAL_WORTIME0);
  WriteReg(REG_PKTSTATUS,VAL_PKTSTATUS);
  WriteReg(REG_VCO_VC_DAC,VAL_VCO_VC_DAC);
  WriteReg(REG_TXBYTES,VAL_TXBYTES);
  WriteReg(REG_RXBYTES,VAL_RXBYTES);
  WriteReg(REG_RCCTRL1_STATUS,VAL_RCCTRL1_STATUS);
  WriteReg(REG_RCCTRL0_STATUS,VAL_RCCTRL0_STATUS);
  
}

void Read_Config_Regs(void)
{ 
  Serial.println(ReadReg(REG_IOCFG2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_IOCFG1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_IOCFG0),HEX);
   delay(10);
/* Serial.println(ReadReg(REG_FIFOTHR),HEX);
   delay(10);
  Serial.println(ReadReg(REG_SYNC1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_SYNC0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_PKTLEN),HEX);
   delay(10);
  Serial.println(ReadReg(REG_PKTCTRL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_PKTCTRL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_ADDR),HEX);
   delay(10);
  Serial.println(ReadReg(REG_CHANNR),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCTRL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCTRL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREQ2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREQ1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREQ0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG4),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG3),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_DEVIATN),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MCSM2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MCSM1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MCSM0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FOCCFG),HEX);
   delay(10);

  Serial.println(ReadReg(REG_BSCFG),HEX);
   delay(10);
  Serial.println(ReadReg(REG_AGCCTRL2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_AGCCTRL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_AGCCTRL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_WOREVT1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_WOREVT0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_WORCTRL),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREND1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREND0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCAL3),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCAL2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCAL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCAL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_RCCTRL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_RCCTRL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSTEST),HEX);
   delay(10);
  Serial.println(ReadReg(REG_PTEST),HEX);
   delay(10);
  Serial.println(ReadReg(REG_AGCTEST),HEX);
   delay(10);
  Serial.println(ReadReg(REG_TEST2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_TEST1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_TEST0),HEX);
   delay(10);
 /*
  Serial.println(ReadReg(REG_PARTNUM),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_VERSION),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_FREQEST),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_LQI),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_RSSI),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_MARCSTATE),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_WORTIME1),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_WORTIME0),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_PKTSTATUS),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_VCO_VC_DAC),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_TXBYTES),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_RXBYTES),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_RCCTRL1_STATUS),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_RCCTRL0_STATUS),HEX);
   delay(1000);
*/  
}
