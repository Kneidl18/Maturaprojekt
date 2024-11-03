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
