/************************************************************************************************/
/*        LCD-Treiber für AW-32-Demoboard        - steuert ein 16x2 LCD an                                    */
/*        (c) Deto 2015                                                                                */
/*                                                                                                */
/*        In Ram (Variablen.c) folgende Variablen einfügen:                                        */
/*                                                                                                */
/*    // (Globale) Variablen für das LCD  [LCD_8Bit.c]                                                */
/*                                                                                                  */
/*  void                (* pLCD_Function)(void);           // Funktionspointer auf akt.LCD-Routine        */
/*  Byte                LCD_Oben[16];                        // LCD-Buffer in der RAM, obere Zeile         */
/*  Byte                LCD_Unten[16];                        // LCD-Buffer in der RAM, untere Zeile        */
/*                                                                                                */
/*        In Variablen.h folgendes einfügen:                                                        */
/*                                                                                                */
/*    //  Variablen für das LCD  [LCD_8Bit.c]                                                        */
/*                                                                                                    */
/*  extern void         (* pLCD_Function)(void);                                                */
/*  extern Byte         LCD_Oben[16];                                                                */
/*  extern Byte         LCD_Unten[16];                                                                */
/*                                                                                                */
/*                                                                                                */
/*        Vom main() aus erfolgt der Aufruf wie folgt:                                                */
/*                                                                                                */
/*  pLCD_Function ();               // Pointer auf den aktuelle LCD-Schritt                        */
/*                                                                                                */
/*        In der Initialisierungsphase (z.B. Start08.c, siehe  "_Startup") einfach Init aufrufen:        */
/*                                                                                                */
/*  Init_LCD();              // [LCD_(Bit.c] Initialisiert das LCD                                */
/*                                                                                                */
/*                                                                                                */
/*        Noch zu beachten:                                                                        */
/*                                                                                                */
/*        Die DEFINES sind richtig zu stellen, auf welchen Ports/Bits das LCD hängt!                */
/*                                                                                                */
/*        Im Beispiel unten hängt das LCD mit dem Datenport auf PORTA, während die zwei                 */
/*        notwendigen Steuerleitungen auf PG0 ("E"-Steuerleitung) bzw. PG1 ("RS"-Steuerleitung)        */
/*        liegen.                                                                                        */
/*                                                                                                */
/*        Zur Funktion:                                                                                */
/*                                                                                                */
/*        Alles was in den Variablen "LCD_Oben" bzw. "LCD-Unten" steht wird 1:1 auf das LCD        */
/*        geschrieben, das macht der Treiber dann selbstständig. Zum Reinkopieren eventuell        */
/*        Routinen "CopyText" usw. verwenden!                                                        */
/*                                                                                                */
/*  Die Texte stehen als Konstanten in Text.c  bzw. Text.h!                                        */
/************************************************************************************************/

#include <MC9S08DZ60.h>
#include "Variablen.h"
#include "FunctionProto.h"
#include "defines.h"
#include "Texte.h"


#define LCD_E_BIT       PTGD_PTGD3                   // Auf welchem PIN/PORT hängt das "E"-Bit? 
#define LCD_E_DIR       PTGDD_PTGDD3            // In diesem Falle auf PORTG, Pin 0  (PG0)

#define LCD_RS_BIT      PTGD_PTGD2                // Auf welchem PIN/PORT hängt das "RS"-Bit? 
#define LCD_RS_DIR      PTGDD_PTGDD2            // In diesem Falle auf PORTG, Pin 1  (PG1)

#define LCD_DATA_PORT   PTAD                        // Welcher (8-Bit)-Port ist der Datenport? 
#define LCD_DATA_DIR    PTADD                        // PORTA

#define LCD_BCKLT_BIT   PTGD_PTGD4
#define LCD_BCKLT_DIR   PTGDD_PTGDD4

#define LCD_DELAY_TIME1  400
#define LCD_DELAY_TIME2  25                        // Delay-Zeit, an Display anzupassen, ev. kürzer machen... probieren!




/************************************************************************************************/
/*                                                                                              */
/*      Init_LCD: Initialisiert Pointer und Variablen für das LCD                               */
/*                                                                                              */
/************************************************************************************************/

void Init_LCD (void){
      
        LCD_E_DIR=1;                                                  // Die beiden Steuerleitungen als AUSGANG deklarieren
        LCD_RS_DIR=1;

        LCD_BCKLT_DIR=1;          // Backlight einschalten
        LCD_BCKLT_BIT=1;          
       
        LCD_DATA_DIR = 0xFF;                                   // Die Bits des Datenports als AUSGANG deklarieren
       
        pLCD_Function = LCD_Wait_Start;                // Wir starten die Schrittkette mit der Funktion "LCD_Wait_Start"
  
        CopyText (&LCD_Haupttext[0], &LCD_Oben[0],32); // Fülle den LCD-Buffer in der Ram mit dem Start-Text
  
}


/************************************************************************************************/
/*                                                                                              */
/*      LCD_Wait_Start: Wartet am Anfang ein paar msec, damit sich das LCD intern                 */
/*      initialisieren kann. Ein Schritt reicht in der Regel. GGf. hier einen Counter reintun!  */
/*                                                                                              */
/************************************************************************************************/

void LCD_Wait_Start (void){

            pLCD_Function = LCD_Initialisierung;           // Springe gleich weiter zur nächsten Routine!
                                                                                                // Falls Zeit zu wenig, hier einen Counter reintun,
                                                                                                // damit einige RT-Zyklen lang hier gewartet wird!
}


/************************************************************************************************/
/*                                                                                              */
/*      LCD_Initialisierung: Initialisiert hardwaremäßig das LCD laut Datenblatt                */
/*                                                                                              */
/************************************************************************************************/

void LCD_Initialisierung (void){

                Write_LCD_Instruction (0x03);        // Diese Initialisierung ist dem Datenblatt 
                LCD_Delay();                                        // zu entnehmen!!

                Write_LCD_Instruction (0x03);
                LCD_Delay();

                Write_LCD_Instruction (0x03);
                LCD_Delay();

                Write_LCD_Instruction (0x38);
                LCD_Delay();

                Write_LCD_Instruction (0x0C);
                LCD_Delay();

                Write_LCD_Instruction (0x06);
                LCD_Delay();

                pLCD_Function = LCD_Cursor_Home;
}

/************************************************************************************************/
/*                                                                                              */
/*      LCD_Cursor_Home: Setzt den Cursor (nicht sichtbar) in die "Home"-Position               */
/*                                                                                              */
/************************************************************************************************/

void LCD_Cursor_Home (void){

                Write_LCD_Instruction (0x80);                // $80 = Home-Position!!!
                pLCD_Function = LCD_Write_Zeile1;
}

/************************************************************************************************/
/*                                                                                              */
/*      LCD_Write_Zeile1: Schreibt die erste Zeile auf das Display                              */
/*      indem er den RAM-Buffer auf das LCD Zeichen für Zeichen rausschreibt!                        */
/*                                                                                              */
/************************************************************************************************/

void LCD_Write_Zeile1 (void){
  
unsigned char LCD_Counter;
        
        for (LCD_Counter=0; LCD_Counter<16; LCD_Counter ++){
                Write_LCD_Data (LCD_Oben[LCD_Counter]);
                LCD_Delay2();                                                  // Ganz ohne Delay geht es leider nicht!
        }
        
                pLCD_Function = LCD_Goto_Zeile2;
}


/************************************************************************************************/
/*                                                                                              */
/*      LCD_Goto_Zeile2: Setzt den Cursor an den Anfang der zweiten Zeile                       */
/*                                                                                              */
/************************************************************************************************/

void LCD_Goto_Zeile2 (void){

                Write_LCD_Instruction (0xC0);                          // $C0 = Anfang der zweiten Zeile!
                pLCD_Function = LCD_Write_Zeile2;                // (Adresse im LCD)
}

/************************************************************************************************/
/*                                                                                              */
/*      LCD_Write_Zeile2: Schreibt die zweite Zeile auf das Display                             */
/*      indem er den RAM-Buffer auf das LCD Zeichen für Zeichen rausschreibt!                        */
/*                                                                                              */
/************************************************************************************************/

void LCD_Write_Zeile2 (void){
 
Byte LCD_Counter;

        for (LCD_Counter=0; LCD_Counter<16; LCD_Counter ++){
                Write_LCD_Data (LCD_Unten[LCD_Counter]);
                LCD_Delay2();                                                  // Ganz ohne Delay geht es leider nicht!
        }
                pLCD_Function = LCD_Cursor_Home;
}


/************************************************************************************************/
/*                                                                                              */
/*      Write_LCD_Instruction: Schreibt eine "Instruction" (Befehl) auf das LCD                 */
/*                                                                                              */
/************************************************************************************************/

void Write_LCD_Instruction (unsigned char Daten) {
                
                LCD_RS_BIT = 0;                           // RS-Bit auf "0" --> Sende Befehle (Instruktionen)!
                Nop();                                                
                LCD_DATA_PORT = Daten;           // Dann die Daten auf den Port und dem "E"-Pin 
                Nop();                                           // einen Clock geben L->H->L!
                LCD_E_BIT = 1;
                Nop();                                           // Die Nop() machen eine kurze Verzögerung, grad
                LCD_E_BIT = 0;                           // mal ein paar hundert Nanosekunden.
}

/************************************************************************************************/
/*                                                                                              */
/*      Write_LCD_Data: Schreibt ein Datenwort (z.B. Buchstaben) auf das LCD                    */
/*                                                                                              */
/************************************************************************************************/

void Write_LCD_Data (unsigned char Daten){
                
                LCD_RS_BIT = 1;                                // RS-Bit auf "1" --> Sende Daten!
                Nop();
                LCD_DATA_PORT = Daten;                
                Nop();                                                // Dann die Daten auf den Port und dem "E"-Pin
                LCD_E_BIT = 1;                                // einen Clock geben L->H->L!
                Nop();
                LCD_E_BIT = 0;
}

/************************************************************************************************/
/*                                                                                              */
/*      LCD_Delay: Grauslige Methode, um Zeit zu schinden... braucht es zum Glück nur in der    */
/*                Initialisierungsphase!                                                                */
/*                                                                                              */
/************************************************************************************************/

void LCD_Delay (void){

int i;
                for (i=0;i<LCD_DELAY_TIME1;i++) Nop();
}
                
/************************************************************************************************/
/*                                                                                              */
/*      LCD_Delay2: Noch so ein ekliger Delay... ohne geht es leider nicht                      */
/*                                                                                              */
/************************************************************************************************/

void LCD_Delay2 (void){
        
int i;
                for (i=0;i<LCD_DELAY_TIME2;i++) Nop();
}


/************************************************************************************************/
/*                                                                                              */
/*      Utility: Kopiert einen "TextLenght"-langen Text von einer Position (SourcePointer)      */
/*      in eine andere (DestPointer) um.                                                        */
/*                                                                                              */
/************************************************************************************************/

void CopyText (unsigned char *pSource, unsigned char *pDestination, unsigned char Lenght){

Byte Counter;

                for (Counter =0; Counter<Lenght; Counter ++) pDestination[Counter]=pSource[Counter];
        
}



/************************************************************************************************/
/*                                                                                              */
/*      Dezimal-BCD: Wandelt eine Dezimalzahl in eine BCD-Zahl um und schreibt sie              */
/*      in die Position "pDestination", "pDestination+1" und "pDestination+2"                   */
/*      Die zu wandelnde Zahl wird übergeben                                                    */
/*      Aufruf aus einer Routine z.B.   DezimalBCD (&LCD_Oben[3], Analog_PB0);                  */
/*      Schreibt an Stelle "LCD_Oben+3" bis "LCD_Oben+5" die dreistellige Zahl "Analog_PB0"     */
/*                                                                                              */
/************************************************************************************************/


void DezimalBCD (unsigned char *pDestination, unsigned char Wert){

Byte Alt, Zwischen;

        Alt=Wert;

        Zwischen = Wert / 10;                   
        pDestination[2]=(Alt%10)+0x30;          // Rest der Division Alt/10, plus 0x30 (ASCII "0") dazu

        Alt = Zwischen /10;        
        pDestination[1]=(Zwischen % 10)+0x30;
        
        pDestination[0]=(Alt % 10)+0x30;
        
        // Eliminiert die vorangehenden Nullen, schaut zuerst auf die Hunderterstelle, dann auf die Zehner
        
        if (pDestination[0]==0x30){                     // pDestination[0] => Hunderterstelle
                pDestination[0]=0x20;
                if (pDestination[1]==0x30) pDestination[1]=0x20;        // pDestination[1] => Zehnerstelle
                }
        
        
}
        
