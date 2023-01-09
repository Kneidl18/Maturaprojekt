#include "Variablen.h" /* Alle meine Variablen */
#include <MC9S08DZ60.h>
#include "FunctionProto.h"
#include "defines.h"




void main(void) {


  for(;;) {
          while(RealTimeFlag == 0) 
        {
           __RESET_WATCHDOG();         /* in MC9S08AW32.h definiert! */
        }
        RealTimeFlag = 0;


        // Ab hier starten meine Tasks!
        pADC_Function ();               // Liese den ADC aus
        Blinklicht();			// Bringt ein Led auf PD0 zum blinken [Blinklicht.c]
        pLCD_Function ();               // Pointer auf den aktuelle LCD-Schritt
        pMenue_Function ();             // Pointer auf mein aktuelles Menue
        Tastenerkennung ();             // Einlesen der verschiedenen Tasten
        
        
       // Store_One_Byte ();
       // Recover_One_Byte ();
	
    	Nop ();
  }
}
