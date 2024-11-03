
Display_Values
		ldhx	Analog_PB0
		sthx	DezBCD_Value
		ldhx	#LCD_Oben
		sthx	DestPointer
		jsr		DezimalBCD_16Bit
		
		lda		Analog_PB1
		ldhx	#LCD_Oben + 8
		sthx	DestPointer
		jsr		DezimalBCD
		
		lda		Analog_PB2
		ldhx	#LCD_Unten
		sthx	DestPointer
		jsr		DezimalBCD
		
		lda		Analog_PB3
		ldhx	#LCD_Unten + 8
		sthx	DestPointer
		jsr		DezimalBCD
		
		rts
		
		

Check_ADC	
		ldhx	ADC_Pointer
		jsr		0,X
		rts
		
ADC_Channel_PB0
		lda		ADCRH				;analog high register auslesen
		sta		Analog_PB0			;save into first 8 Bit of the variable
		
		lda		ADCRL				;analog register auslesen
		sta		Analog_PB0 + 1		;save into second 8 Bit of the varaiable
		
		ldhx	#ADC_Channel_PB1	;set pointer to next routine
		sthx	ADC_Pointer
		
		lda		#9T					;set ADC to next channel
		sta		ADCSC1
		
		rts
		
ADC_Channel_PB1
		lda		ADCRL				;analog register auslesen
		sta		Analog_PB1			;save into varaiable
		
		ldhx	#ADC_Channel_PB2	;set pointer to next routine
		sthx	ADC_Pointer
		
		lda		#10T				;set ADC to next channel
		sta		ADCSC1
		
		rts
		
		
ADC_Channel_PB2
		lda		ADCRL				;analog register auslesen
		sta		Analog_PB2			;save into varaiable
		
		ldhx	#ADC_Channel_PB3	;set pointer to next routine
		sthx	ADC_Pointer
		
		lda		#11T				;set ADC to next channel
		sta		ADCSC1
		
		rts
		
		
ADC_Channel_PB3
		lda		ADCRL				;analog register auslesen
		sta		Analog_PB3			;save into varaiable
		
		ldhx	#ADC_Channel_PB0	;set pointer to next routine
		sthx	ADC_Pointer
		
		lda		#12T					;set ADC to next channel
		sta		ADCSC1
		
		rts
		
		
	
Init_ADC

		lda		#%01011000		;Wandlergeschwindigkeit, 8 Bit Mode
		sta		ADCCFG
		
		lda		#%00000000		;no compare funktion, Software trigger. .everything set 0
		sta		ADCSC2
		
		lda		#$FF			;PB0-PB8 as input (Analog), else digital input/output
		sta		APCTL2
		
		lda		#0				;port a and port c set as digital not analog
		sta		APCTL1
		sta		APCTL3
		
		lda		#12T				;channel 8 (port B) is input (analog) port
		sta		ADCSC1
		
		ldhx	#ADC_Channel_PB0	;set routine start to check_PB0
		sthx	ADC_Pointer
		
		rts
			
		
		
		
		