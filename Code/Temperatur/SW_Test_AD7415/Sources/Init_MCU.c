#include "MC9S08AW32.h"

void InitMCU(void) {
 
        SMCLK = 0;
        ICGFLTU = 0;
        ICGS2 = 0;
        ICGS1 = 0;
        ICGC2 = 1;          
        ICGFLTL = 0x0C;
        ICGC1 = 0x7C;
        
               /*
                lda     #%01111100
                ;         ||||||||
                ;         ||||||||_____  Fix 0
                ;         |||||||______  Loss of Clock detection enabled (0)
                ;         ||||||_______  Enabel Osc in OFF-Mode (1)
                ;         |||||________  Clock Mode 1:1 = FLL enabled, external Reference
                ;         ||||_________  Clock Mode 
                ;         |||__________  External reference 0=External Clock  1=Ext.Crystal
                ;         ||___________  Freq.Range 0=Low Frequency  1=High Frequency
                ;         |____________  High Gain Osc Select 0=Low Power 1=High Gain
                
                sta     ICGC1      
                 */

        
}


