// InitMCU.c
void InitMCU(void);

// Realtime.c
void InitRealtime(void);
__interrupt void _Realtime(void);

// Blinklicht.c
void Blinklicht(void);
void InitBlinklicht(void);

// AD7415 and everything for the I2C
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(unsigned char Wert);
unsigned char IIC_Get_Ack(void);
unsigned char IIC_Get_Byte(void);
void IIC_Put_Ack(void);
void IIC_Put_Nack(void);

void Read_Air_Temp(void);
void Init_AD7415(void);
