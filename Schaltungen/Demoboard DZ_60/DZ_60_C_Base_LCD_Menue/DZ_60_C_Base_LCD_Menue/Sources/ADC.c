#include "Variablen.h"
#include <MC9S08DZ60.h>
#include "FunctionProto.h"

void Init_ADC (void){
        ADCCFG = 0x00;
        APCTL1 = 0x03;            // Pin PB0 und PB1 = ADC
        ADCSC1 = 0x00;         // Startet PB0 als Konvertierung!!
        pADC_Function = Read_PB0;
        
}




void Read_PB0 (void){
        Analog_PB0=ADCRL;
        ADCSC1 = 0x01;       
        pADC_Function = Read_PB1;
}


void Read_PB1 (void){
        Analog_PB1=ADCRL;
        ADCSC1 = 0x00;       
        pADC_Function = Read_PB0;
}