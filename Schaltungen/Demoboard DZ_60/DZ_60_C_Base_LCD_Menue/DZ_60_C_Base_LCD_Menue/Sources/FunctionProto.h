// InitMCU.c

void InitMCU(void);

// Realtime.c

void InitRealtime(void);
__interrupt void _Realtime (void);

// Blinklicht.c

void Blinklicht(void);
void InitBlinklicht(void);

// LCD_8Bit.c

void Init_LCD(void);
void LCD_Wait_Start (void);
void LCD_Initialisierung (void);
void LCD_Cursor_Home (void);
void LCD_Write_Zeile1 (void);
void LCD_Goto_Zeile2 (void);
void LCD_Write_Zeile2 (void);

void Write_LCD_Instruction (unsigned char Daten);
void Write_LCD_Data(unsigned char Daten);
void LCD_Delay (void);
void LCD_Delay2 (void);
void CopyText (unsigned char *pSource, unsigned char *pDestination, unsigned char Lenght);
void DezimalBCD (unsigned char *pDestination, unsigned char Wert);

//  Menue.c

void Init_Menue (void);
void StartMenue (void);
void Menue1 (void);
void Menue2 (void);
void Menue3 (void);

// Tastenerkennung.c

void Tastenerkennung (void);
void Init_Tasten (void);

// ADC.c

void Init_ADC (void);
void Read_PB0 (void);
void Read_PB1 (void);

// EEPROM_24LC256.c

void IIC_Start (void);
void IIC_Stop (void);
void IIC_Send_Byte (unsigned char Wert);
unsigned char IIC_Get_Ack (void);
unsigned char IIC_Get_Byte (void);
void Store_One_Byte (void);
void Recover_One_Byte (void);
unsigned char GetHighByte (unsigned int Value);
unsigned char GetLowByte (unsigned int Value);
void SaveString (unsigned int EEPromStartAdress, unsigned char *MyString, unsigned char Laenge);
void LoadString (unsigned int EEPromStartAdress, unsigned char *MyString, unsigned char Laenge);