// InitMCU.c

void InitMCU(void);

// Realtime.c

void InitRealtime(void);
__interrupt void _Realtime(void);

// Blinklicht.c

void Blinklicht(void);
void InitBlinklicht(void);

// SPI

void SPI_Init(void);
void SPI_Wait(void);
unsigned char SPI_readReg(unsigned char Adress);
void SPI_WaitSend(void);
void SPI_writeReg(unsigned char Adress, unsigned char sendByte);
void SPI_sendStrobe(unsigned char Strobe);

// CC2500

void CC2500_Init(void);
void CC2500_listenForPacket(void);
void CC2500_sendPacket(void);
void CC2500_readConfigRegs(void);