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
void BME680_triggerMeasurement(void);
unsigned int BME680_altitude(int press, float seaLevel);