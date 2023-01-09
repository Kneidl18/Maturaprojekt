// InitMCU.c
void InitMCU(void);

// Realtime.c
void InitRealtime(void);
__interrupt void _Realtime(void);

// Blinklicht.c
void Blinklicht(void);
void InitBlinklicht(void);

// WaterSensor.c
void WaterSensor_Init(void);
void WaterSensor_ADC(void);
void WaterSensor_PreparePacket(void);
