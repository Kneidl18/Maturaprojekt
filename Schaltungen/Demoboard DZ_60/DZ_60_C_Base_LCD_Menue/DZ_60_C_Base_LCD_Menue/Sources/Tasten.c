#include <MC9S08DZ60.h>
#include "Variablen.h"

#define TASTE_UP        PTDD_PTDD0
#define TASTE_UP_PORT	  PTDDD_PTDDD0
#define TASTE_DOWN      PTDD_PTDD1
#define TASTE_DOWN_PORT	PTDDD_PTDDD1
#define TASTENHYSTERESE 5


void Init_Tasten (void){
        TASTE_UP_PORT=0;                // Tasten als EINGANG programmieren
        TASTE_DOWN_PORT=0;
        
        Taste_Up_Counter =0;           // Counter und Flags alle auf Null setzen
        Taste_Down_Counter =0; 
        Taste_Up_Flag =0;
        Taste_Down_Flag =0; 
}

void Tastenerkennung (void){
        
        if (TASTE_UP==1){
                Taste_Up_Counter++;
                if (Taste_Up_Counter>=TASTENHYSTERESE){
                        Taste_Up_Counter=TASTENHYSTERESE;
                        Taste_Up_Flag |= 0x01;
                } 
        }else {
                        Taste_Up_Counter =0;
                        Taste_Up_Flag = 0;
        }
        
        
        if (TASTE_DOWN==1){
                Taste_Down_Counter++;
                if (Taste_Down_Counter>=TASTENHYSTERESE){
                        Taste_Down_Counter=TASTENHYSTERESE;
                        Taste_Down_Flag |= 0x01;
                } 
        }else {
                        Taste_Down_Counter =0;
                        Taste_Down_Flag = 0;
        }
        
}


