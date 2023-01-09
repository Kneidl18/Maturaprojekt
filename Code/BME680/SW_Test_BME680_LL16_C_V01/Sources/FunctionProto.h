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

// BME680
void BME680_Init(void);
void BME680_getCalibration(void);
void BME680_setOversampling(void);
void BME680_setIIRFilter(void);
void BME680_setGas(unsigned int gasTemp, unsigned int gasMillis);
void BME680_waitForReadings(void);
unsigned char BME680_measuring(void);
void BME680_getSensorData(void);
void BME680_preparePacketValues(void);
void BME680_triggerMeasurement(void);
void BME680_PreparePacketGas(unsigned int temp, unsigned int millis);
void BME680_PreparePacketCalibration(unsigned char coeff1[], unsigned char coeff2[]);

//cc2500
void CC2500_Init(void);
void CC2500_listenForPacket(void);
void CC2500_sendPacket(void);
void CC2500_readConfigRegs(void);