#include "Variablen.h" /* Alle meine Variablen */
#include <MC9S08AW32.h>
#include "FunctionProto.h"




void main(void) {


  for(;;) {
          while(RealTimeFlag == 0) 
        {
           __RESET_WATCHDOG();         /* in MC9S08AW32.h definiert! */
        }
        RealTimeFlag = 0;


        // Ab hier starten meine Tasks!
        Blinklicht();
        Read_Air_Temp();
     
  }
}
