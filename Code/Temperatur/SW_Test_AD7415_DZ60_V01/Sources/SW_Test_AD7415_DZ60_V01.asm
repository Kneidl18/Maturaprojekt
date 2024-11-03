;************************************************************************************************
;*      DZ-Base-Programm  Version 1.0                                                         	*
;*                                                                                              *
;*      Revision vom 10. September 2020                                                         *
;*                                                                                              *
;*      Name:                                                                                   *
;*      Klasse:                                                                                 *
;*      Version:                                                                                *
;*                                                                                              *
;*      Programmiert auf einem Target mit 4,9152 MHz                                            *
;*      Interner Busclock mit FLL auf 19,6608 MHz hinaufgetaktet                                *
;*      Realtime zu exakt 10 msec                                                               *
;*                                                                                              *
;************************************************************************************************

            
				XDEF Main_Init
            	ABSENTRY Main_Init

;************************************************************************************************
;*      Speicherdefinitionen                                                                    *
;*                                                                                              *
;************************************************************************************************




                INCLUDE 'Memory_Mapping.inc'    ; Wo beginnt der Speicher, die RAM, das Flash, EEPROM...
                INCLUDE 'DZ60_Regs.inc'         ; Register des Controllers
                INCLUDE 'Variablen.inc'         ; Hier sind die Variablen drinnen!
                INCLUDE 'Equates.inc'           ; Allgemeine Equates, global




;************************************************************************************************
;*                                                                                              *
;*      Main_Loop: Die Hauptschleife                                                            *
;*      Sie wird exakt alle 10 msec einmal durchlaufen!                                         *
;*                                                                                              *
;************************************************************************************************


                ORG    ROM_DZ60_START


Main_Loop
                feed_watchdog:                  ; Watchdog zurücksetzen
                
                lda     TimerFlag 
                cbeqa	#0T, Main_Loop              ; TimerFlag = 0 --> Realtime noch nicht um
                									; TimerFlag <>0 --> Realtime (10 msec) ist um!
                clr     TimerFlag   
                
               
                ;jsr		Check_ADC
                              
                jsr    	Update_LCD              ; [LCD_Disp_8Bit.inc] Beschreibt das LCD
            
                bra     Main_Loop


;************************************************************************************************
;*                                                                                              *
;*      Es folgen die verschiedensten Includes für die                                          *
;*      SubRoutinen                                                                             *
;*                                                                                              *
;************************************************************************************************


                INCLUDE 'Init.inc'              ; Hier startet der µC / Initialisierungen
                INCLUDE 'Realtime.inc'          ; Alles für den Realtime
                INCLUDE 'LCD_Disp_8Bit_V22.inc'
                INCLUDE	'ADC_DZ60_V10.inc'		;Driver for the ADC
                ;INCLUDE	'I2C_V10.inc'			;I2C driver
                ;INCLUDE 'EEPROM_Test.inc'		;Test for the EEPROM
                INCLUDE 'Temperatur.inc'		;Test for the temperature sensor (I2C)
                INCLUDE 'Dummy_Isr.inc'         ; Für fehlgeschlagene (unerwünschte) Interrupts
                INCLUDE 'Vectors.inc'           ; Vektoren usw.
