#include "FunctionProto.h"
#include "defines.h"
#include "Variablen.h"

#define CURRENTSENSOR1_PORT		PTAD_PTAD0
#define CURRENTSENSOR2_PORT		PTAD_PTAD1
#define CURRENTSENSOR3_PORT		PTAD_PTAD2

#define CURRENTSENSOR1_DIR		PTADD_PTADD0
#define CURRENTSENSOR2_DIR		PTADD_PTADD1
#define CURRENTSENSOR3_DIR		PTADD_PTADD2

//************************************************************************************************
//*                                                                                              *
//*      WaterSensor_Init: initializes the ADC for PTA4                                          *
//*																								 *
//************************************************************************************************

//initializing the WaterSensorPort and the ADC
void ADC_LL16_V01_Init(void)
{
	//AD-CHANNEL: 00100 -> Port A4
	// Status and control register, ADCSC1
	// Status and control register, ADCSC2
	// Data result registers, ADCRH and ADCRL
	// Compare value registers, ADCCVH and ADCCVL
	// Configuration register, ADCCFG
	// Pin control registers, APCTL1, APCTL2, APCTL3
	
	ADCSC1 = 0b00000000;       
	//	       ||||||||__ ) 
	//	   	   |||||||___ )
	//	   	   ||||||____ > ADCH: input channel select (PTA4 = 00100) 
	//	   	   |||||_____ )
	//	   	   ||||______ )
	//	   	   |||_______ ADCO: continuous convertion
	//	   	   ||________ AIEN: enable convertion complete interrupt
	//	   	   |_________ COCO: convertion complete (cleared when reading ADCRL)
	
	ADCSC2 = 0b00000000;      
	//	   	   ||||||||__ )
	//	   	   |||||||___ > reserved
	//	   	   ||||||____ )
	//	   	   |||||_____ )
	//	   	   ||||______ ACFGT: 0=compare triggers when input<=value;1=trigger if input>=value
	//	   	   |||_______ ACFE: compare function enable
	//	   	   ||________ ADTRG: trigger select, 1=hw;0=sw
	//	   	   |_________ ADACT: conversion active, if set currently converting
	
	//compare value for the compare function
	ADCCVH = 0b00000000;	//compare high value (only last 4 bits)
	ADCCVL = 0b00000000;	//compare low value (all 8 bits)
	
	ADCCFG = 0b00110100;      
	//	   	   ||||||||__ )
	//	   	   |||||||___ > ADICLK: input clock select 00=Bus clock
	//	   	   ||||||____ > MODE: 00=8Bit, 01=12Bit, 10=10bit
	//	   	   |||||_____ )
	//	   	   ||||______ ADLSMP: sample time speed 0=short; 1=long sample time
	//	   	   |||_______ > ADIV: 11=bus/8 for internal adclk; 01=bus/2
	//	   	   ||________ )
	//	   	   |_________ ADLPC: Low Power config 0=high speed; 1=low power
	
	APCTL1 = 0b00010000;  
	//	   	   ||||||||__ ADPC0: 0=enable; 1=disable
	//	   	   |||||||___
	//	   	   ||||||____
	//	   	   |||||_____
	//	   	   ||||______ ADPC4: 0=enable; 1=disable
	//	  	   |||_______
	//	   	   ||________
	//	   	   |_________ ADPC7: 0=enable; 1=disable
	
	CURRENTSENSOR1_DIR = 0;
	CURRENTSENSOR2_DIR = 0;
	CURRENTSENSOR3_DIR = 0;
	
	ADCPort = 0;
}


//************************************************************************************************
//*                                                                                              *
//*      WaterSensor_ADC: updates the Variable with the current ADC Value                        *
//*																								 *
//************************************************************************************************
void ADC_LL16_V01_getADCValue(void)
{	
	uint temp;
	
	temp = ADCRH << 8;
	temp += ADCRL;
	
	switch(ADCPort)
	{
		case 0:
		{
			CurrentSensor1 = temp;
			ADCSC1 = 0b00000001; 
			break;
		}
			
		case 1:
		{
			CurrentSensor2 = temp;
			ADCSC1 = 0b00000010; 
			break;
		}
		
		case 2:
		{
			CurrentSensor3 = temp;
			ADCSC1 = 0b00000000; 
			break;
		}
		
		default:
		{
			break;
		}
	}
	
	ADCPort = ADCPort++;
	
	if(ADCPort >= 4)
		ADCPort = 0;
}
