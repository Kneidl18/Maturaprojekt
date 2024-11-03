#include "FunctionProto.h"
#include "Variablen.h"
#include "Texte.h"

void Init_Menue (void){
 
 pMenue_Function = StartMenue;       
 StartCounter = 0;    
        
}


void StartMenue (void){
        
    CopyText (&LCD_Haupttext[0], &LCD_Oben[0],32);    
    StartCounter++;
    if (StartCounter == 200)  {
        pMenue_Function = Menue1;           // 200 x 10ms = 2 Sekunden 
        StartCounter=0;
    }
    
    
}

void Menue1 (void){
        
    CopyText (&LCD_MenueText1[0], &LCD_Oben[0],32);    
        
    if (Taste_Up_Flag == 0x01) {
        pMenue_Function = Menue2;   
        Taste_Up_Flag |=0x02;
    }
    if (Taste_Down_Flag == 0x01) {
        pMenue_Function = Menue3;   
        Taste_Down_Flag |= 0x02;
    }
}

void Menue2 (void){
        
    CopyText (&LCD_MenueText2[0], &LCD_Oben[0],32);    
    
    DezimalBCD (&LCD_Unten[9], Analog_PB0);
        
    if (Taste_Up_Flag == 0x01)  {
        pMenue_Function = Menue3;   
        Taste_Up_Flag |=0x02;
    }
    if (Taste_Down_Flag == 0x01) {
        pMenue_Function = Menue1;   
        Taste_Down_Flag |=0x02;
    }
}


void Menue3 (void){
        
    CopyText (&LCD_MenueText3[0], &LCD_Oben[0],32);    
        
    DezimalBCD (&LCD_Unten[9], Analog_PB1);
        
        
    if (Taste_Up_Flag == 0x01) {
        pMenue_Function = Menue1;   
        Taste_Up_Flag |=0x02;
    }
    if (Taste_Down_Flag == 0x01) {
        pMenue_Function = Menue2;   
        Taste_Down_Flag |=0x02;
    }
}