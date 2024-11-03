// InitMCU.c
void InitMCU(void);

// Realtime.c
void InitRealtime(void);
__interrupt void _Realtime(void);

// Blinklicht.c
void Blinklicht(void);
void InitBlinklicht(void);

// WaterSensor.c
void ADC_LL16_V01_Init(void);
void ADC_LL16_V01_getADCValue(void);

//Relaise
void Relaise_Init(void);
int Relaise_CheckState(void);
void Relaise_PreparePacket(void);

//CurrentSensor
void CurrentSensor_PreparePacket(void);

//SPI
void SPI_Init(void);
unsigned char SPI_readReg(unsigned char Adress);
void SPI_sendStrobe(unsigned char Strobe);
void SPI_writeReg(unsigned char Adress, unsigned char sendByte);\
void SPI_WaitSend(void);
void SPI_Wait(void);

//cc2500
void CC2500_readConfigRegs(void);
void CC2500_Init(void);
void CC2500_sendPacket(void);
void CC2500_listenForPacket(void);
unsigned char createCRC(void);
