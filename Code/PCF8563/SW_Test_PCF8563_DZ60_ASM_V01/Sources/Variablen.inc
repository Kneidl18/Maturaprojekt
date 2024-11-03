;************************************************************************************************
;*                                                                                              *
;*      Hier kommen meine Variablen rein... Zuerst die Zeropage                                 *
;*                                                                                              *
;************************************************************************************************

                ORG     Z_RAM_START            

;Realtime
TimerFlag               ds      1T
RealTimeCounter         ds      1T

;LCD
LCD_Pointer             ds      2T              ; Pointer auf die aktuelle LCD-Routine	
LCD_Counter             ds      1T							
LCD_Oben                ds      16T             ; LCD erste Zeile, 16 Zeichen lang	
LCD_Unten               ds      16T             ; LCD zweite Zeile, 16 Zeichen lang	
DelayCounter            ds      1T              ; Grässlich - Delay für LCD...		
CopyCounter             ds      1T							
SourcePointer           ds      2T              ; Pointer für Copy-Routinen		
DestPointer             ds      2T              ; Ebenfalls für die Copy-Routinen	
TextLenght              ds      1T							
DezBCD_Value            ds      2T              ; Anzuzeigende 16 Bit-Zahl              
BCD_Counter             ds      1T   

;ADC
ADC_Pointer				ds		2T
Analog_PB0				ds		1T
Analog_PB1				ds		1T
Analog_PB2				ds		1T
Analog_PB3				ds		1T

;I2C
I2C_Byte				ds		1T
BitCounter_I2C			ds		1T

;EEPROM_Test
EE_Adresse				ds		1T
EE_Wert					ds		1T

;Tasten
Taste_1_Flag			ds		1T
Taste_2_Flag			ds		1T
Taste_3_Flag			ds		1T
Taste_4_Flag			ds		1T
PrellCounter_1			ds		1T
PrellCounter_2			ds		1T
PrellCounter_3			ds		1T
PrellCounter_4			ds		1T

;PCF8563
Sekunden				ds 		1T
Minuten					ds 		1T
Stunden					ds 		1T
Tag						ds 		1T
Wochentag				ds 		1T
Delay_Counter			ds 		1T


;************************************************************************************************
;*                                                                                              *
;*      Und hier die sog. "Extend RAM", also der große Speicher                                 *
;*                                                                                              *
;************************************************************************************************

                ORG     E_RAM_START
                

