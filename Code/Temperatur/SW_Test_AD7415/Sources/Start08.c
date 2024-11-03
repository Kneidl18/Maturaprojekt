#include "MC9S08AW32.h"
#include "FunctionProto.h"


extern void main(void); /* prototype of main function */


void _Startup(void) {

        asm {                          // Initialisiert den Stack-Pointer
                ldhx    #0x0870
                txs
        }
  

        PTADD = 0x00;           // PORTA alles Eing�nge
        PTBDD = 0x00;           // PORTB alles Eing�nge
        PTCDD = 0x00;           // PORTC alles Eing�nge
        PTDDD = 0x00;           // PORTD alles Eing�nge
        PTEDD = 0x00;           // PORTE alles Eing�nge
        PTFDD = 0x00;           // PORTF alles Eing�nge
        PTGDD = 0x00;           // PORTG alles Eing�nge


        PTAD = 0;               // Port-Bits alle auf Null
        PTBD = 0;               // Port-Bits alle auf Null
        PTCD = 0;               // Port-Bits alle auf Null
        PTDD = 0;               // Port-Bits alle auf Null
        PTED = 0;               // Port-Bits alle auf Null
        PTFD = 0;               // Port-Bits alle auf Null
        PTGD = 0;               // Port-Bits alle auf Null

        
        InitMCU();              // Stellt die verschiedenen Register der CPU ein [.]
        InitBlinklicht();       // [Blinklicht.c]
        InitRealtime();         // Initialisiert den Realtime  [Realtime.c]
        Init_AD7415();

        asm cli ;               // Aktiviert die Interrupts ( "cli")
  
        __asm JMP main;         // Springt zum Hauptloop


}

