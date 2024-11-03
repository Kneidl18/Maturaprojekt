// InitMCU.c
void InitMCU(void);

// Realtime.c
void InitRealtime(void);
__interrupt void _Realtime(void);

// Blinklicht.c
void Blinklicht(void);
void InitBlinklicht(void);

// DoorSensor.c
void DoorSensor_Init(void);
byte DoorSensor_GetState(void);
void DoorSensor_CheckState(void);
void DoorSensor_UpdateState(void);
void DoorSensor_PreparePacket(void);
