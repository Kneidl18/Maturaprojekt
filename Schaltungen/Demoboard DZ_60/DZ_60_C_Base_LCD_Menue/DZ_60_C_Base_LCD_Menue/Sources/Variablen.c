#include "Variablen.h"


#pragma DATA_SEG _DATA_ZEROPAGE 
    
    // Variablen f�r den RealTime  [Realtime.c]
    
    unsigned char        RealTimeFlag;               //Flag, das angibt, ob der Realtime um ist 0=Nein 1 = JA!
    unsigned char        RealTimeCounter;
       
    // Variable f�r das Blinklicht [Blinklicht.c]
    
    unsigned char	RunLedCounter;
    
    //  Variablen f�r das LCD  [LCD_8Bit.c]
    
    void        (* pLCD_Function)(void);   	// Funktionspointer auf die aktuelle LCD-Routine
    unsigned char        LCD_Oben[16];				// LCD-Buffer in der RAM, obere Zeile � 16 Zeichen
    unsigned char        LCD_Unten[16];				// LCD-Buffer in der RAM, untere Zeile � 16 Zeichen
    
    // F�r die Menuesteuerung [Menue.c]
    
    void                (* pMenue_Function)(void);
    int         StartCounter;
    
    // F�r den ADC [ADC.c]
    
    unsigned char        Analog_PB0;
    unsigned char        Analog_PB1;
    void        (* pADC_Function)(void);   	// Funktionspointer auf die aktuelle ADC-Routine
    
    // F�r die Tastenerkennung
    
    unsigned char        Taste_Up_Flag; 
    unsigned char        Taste_Up_Counter;                           
    unsigned char        Taste_Down_Flag;
    unsigned char        Taste_Down_Counter;

    // Variablen f�r EEPROM
    
    unsigned char        Reply;   
    unsigned char        TestWert; 
    unsigned char	Ergebnis;
    
    