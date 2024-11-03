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
unsigned char createCRC(void);

//commands
void inputToCommand(void);


// door sensor
void DoorSensor_Init(void);
void DoorSensor_PreparePacket(void);
unsigned char DoorSensor_GetState(void);
void DoorSensor_UpdateState(void);
void DoorSensor_CheckState(void);

// BME680
void BME680_Init(void);
void BME680_getCalibration(void);
void BME680_setOversampling(void);
void BME680_setIIRFilter(void);
void BME680_setGas();
void BME680_waitForReadings(void);
unsigned char BME680_measuring(void);
void BME680_getSensorData(void);
void BME680_preparePacketValues(void);
void BME680_triggerMeasurement(void);
void BME680_readValues(void);

//AD7415 and everything for the IIC
void IIC_Start_1(void);
void IIC_Stop_1(void);
void IIC_Send_Byte_1(unsigned char Wert);
unsigned char IIC_Get_Ack_1(void);
unsigned char IIC_Get_Byte_1(void);
void IIC_Put_Ack_1(void);
void IIC_Put_Nack_1(void);
void Read_Air_Temp_1(void);
void Init_AD7415_1(void);
void AD7415_preparePacket_1(void);
void AD7415_preparePacket_2(void);

//water
void WaterSensor_Init(void);
void WaterSensor_ADC(void);
void WaterSensor_PreparePacket(void);