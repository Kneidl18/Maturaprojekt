#include <stdtypes.h>



#pragma DATA_SEG _DATA_ZEROPAGE    
    
    // Variablen f�r den RealTime  [Realtime.c]
    
    extern unsigned char       	RealTimeFlag;  
    extern unsigned char        	RealTimeCounter;    

    // Variable f�r das Blinklicht [Blinklicht.c]
    
    extern unsigned char 		RunLedCounter;
    
    //  Variablen f�r das LCD  [LCD_8Bit.c]
    
    extern void         (* pLCD_Function)(void);
    extern unsigned char         LCD_Oben[16];
    extern unsigned char         LCD_Unten[16];
    
    // Variablen f�r die Menuesteuerung
    
    extern void         (* pMenue_Function)(void);
    extern int          StartCounter;
    
    // F�r den ADC [ADC.c]
    
    extern unsigned char         Analog_PB0; 
    extern unsigned char        Analog_PB1;  
    extern void        (* pADC_Function)(void);   	// Funktionspointer auf die aktuelle ADC-Routine
    
     
    // Variablen f�r die Tastenerkennung
    
    extern unsigned char        Taste_Up_Flag; 
    extern unsigned char        Taste_Up_Counter;                           
    extern unsigned char        Taste_Down_Flag;
    extern unsigned char        Taste_Down_Counter;
    
    // Variablen f�r EEPROM
    
    extern unsigned char        Reply;
    extern unsigned char        TestWert; 
    extern unsigned char        Ergebnis;