#include "MC9S08DZ60.h"
#include "FunctionProto.h"


extern void main(void); /* prototype of main function */


void _Startup(void) {

        asm    ldhx    #0x0870                       // Initialisiert den Stack-Pointer am Ende der RAM
		    asm	   txs
        
  

        PTADD = 0x00;           // PORTA alles Eingänge
        PTBDD = 0x00;           // PORTB alles Eingänge
        PTCDD = 0x00;           // PORTC alles Eingänge
        PTDDD = 0x00;           // PORTD alles Eingänge
        PTEDD = 0x00;           // PORTE alles Eingänge
        PTFDD = 0x00;           // PORTF alles Eingänge
        PTGDD = 0x00;           // PORTG alles Eingänge


        PTAD = 0;               // Port-Bits alle auf Null
        PTBD = 0;               // Port-Bits alle auf Null
        PTCD = 0;               // Port-Bits alle auf Null
        PTDD = 0;               // Port-Bits alle auf Null
        PTED = 0;               // Port-Bits alle auf Null
        PTFD = 0;               // Port-Bits alle auf Null
        PTGD = 0;               // Port-Bits alle auf Null

        
        InitMCU();              // Stellt die verschiedenen Register der CPU ein [Init_MCU.c]
        InitBlinklicht();       // Initialisiert das Blinklicht [Blinklicht.c]
        InitRealtime();         // Initialisiert den Realtime  [Realtime.c]
        Init_LCD();             // Initialisiert das LCD  [LCD_(Bit.c]
        Init_Menue ();          // Initialisiert die Menuestruktur [Menue.c]
        Init_Tasten ();         // Initialisiert die Tastenreoutine [Tasten.c]
        Init_ADC ();            // Initialisier den ADC [ADC.c]
        asm cli ;               // Aktiviert die Interrupts ( "cli")
  
        asm JMP main;         // Springt zum Hauptloop


}

