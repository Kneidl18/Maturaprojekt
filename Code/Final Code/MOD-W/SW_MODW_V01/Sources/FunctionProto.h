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

//MODW
void MODW_Init(void);
void keepAwake(void);
void turnOff(void);
void checkTurnOn(void);

//PCF8563 and everything for the IIC
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(unsigned char Wert);
unsigned char IIC_Get_Ack(void);
unsigned char IIC_Get_Byte(void);
void IIC_Put_Ack(void);
void IIC_Put_Nack(void);

void Init_PCF8563(void);
char PCF8563_checkAlarmFlag(void);
void PCF8563_clearAlarmFlag(void);
void PCF8563_setAlarm(void);
