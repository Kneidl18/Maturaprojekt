#include "Variablen.h"
#include "MC9S08AW32.h"
#include "defines.h"


/****************************************************************************************/
/*                                                                                      */
/*  Realtime-Interrupt und dazugehörige Initialisierung                                 */
/*  Ich stelle den Interrupt auf                                                        */
/*  Bei 4,9152 MHz-Quarz und FLL= 4 ist der Busclock 1/4,9152 MHz = 203,451 nsec        */
/*                                                                                      */                         
/*      Bei $6000 Zyklen (Dezimal: 49.152 Zyklen) á 203,451 nsec = 10 msec              */
/*      $6000 ergibt sich aus 8192 vom Prescaler (im SRTISC-Register) und dem           */
/*      RealTimeCounter, der bis 6 zählt. 6 x 8192 = 49.152 Zyklen                      */
/*                                                                                      */
/****************************************************************************************/


__interrupt void _Realtime (void) {
    SRTISC = 0x75;
    
/*          SRTISC       #%01110101             ; Dividiert durch 8192 x 203,451... nsec = 1,6666 msec                                          */
/*                         ||||||||                                                                                                             */
/*                         ||||||||_____________  Bit 0 - Bit 2: Divisionsrate 0:0:0 = Disable  0:0:1 = Div 256                                 */
/*                         |||||||______________  0:1:0 = Div 1024   0:1:1 = Div 2048   1:0:0 = Div 4096                                        */
/*                         ||||||_______________  1:0:1 = Div 8192   1:1:0 = Div 16384  1:1:1 = Div 32768                                       */
/*                         |||||________________  Fix auf 0 (ungebraucht)                                                                       */
/*                         ||||_________________  RTIE : Realtime Interrupt enable 1=Yes   0=No                                                 */
/*                         |||__________________  RTICLKS : Realtime Clock Select: 0=intern 1kHz  1=Ext. Clock (Quarz)                          */
/*                         ||___________________  RTIACK: Acknowledge Flag: Muss vor neuem Int. gelöscht werden! "1" schreiben!                 */
/*                         |____________________  RTIF: Zeigt an, ob es einen RealTime gab - nur zum Auslesen!                                  */
       

    
    
    RealTimeCounter++;
    if (RealTimeCounter == 6) {
    	RealTimeFlag=1;
    	RealTimeCounter=0;
    }
}


void InitRealtime (void) {
        SRTISC = 0x75;
        RealTimeFlag=0;
        RealTimeCounter=0;
}



/****************************************************************************************/
/*                                                                                      */
/*  Dummy Interrupt-Routine: braucht es, um ggf. falsch auftretende (und nicht gewollte)*/
/*  Interrupts abzufangen!                                                              */
/*                                                                                      */
/****************************************************************************************/


__interrupt void _Dummy (void){
        Nop();
}




