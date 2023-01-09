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
DelayCounter            ds      1T              ; Gr�sslich - Delay f�r LCD...		
CopyCounter             ds      1T							
SourcePointer           ds      2T              ; Pointer f�r Copy-Routinen		
DestPointer             ds      2T              ; Ebenfalls f�r die Copy-Routinen	
TextLenght              ds      1T							
DezBCD_Value            ds      2T              ; Anzuzeigende 16 Bit-Zahl              
BCD_Counter             ds      1T   

;ADC
ADC_Pointer				ds		2T
Analog_PB0				ds		2T
Analog_PB1				ds		1T
Analog_PB2				ds		1T
Analog_PB3				ds		1T

;************************************************************************************************
;*                                                                                              *
;*      Und hier die sog. "Extend RAM", also der gro�e Speicher                                 *
;*                                                                                              *
;************************************************************************************************

                ORG     E_RAM_START
                
