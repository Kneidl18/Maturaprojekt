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
                feed_watchdog:                  ; Watchdog zur�cksetzen
                
                lda     TimerFlag 
                cbeqa	#0T, Main_Loop              ; TimerFlag = 0 --> Realtime noch nicht um
                									; TimerFlag <>0 --> Realtime (10 msec) ist um!
                clr     TimerFlag   
                
               	;jsr		check_BME680
               	jsr		SPI_Test
            
                bra     Main_Loop


;************************************************************************************************
;*                                                                                              *
;*      Es folgen die verschiedensten Includes f�r die                                          *
;*      SubRoutinen                                                                             *
;*                                                                                              *
;************************************************************************************************


                INCLUDE 'Init.inc'              ; Hier startet der �C / Initialisierungen
                INCLUDE 'Realtime.inc'          ; Alles f�r den Realtime
                INCLUDE 'BME680_REG.inc'		;Register for BME680
                INCLUDE	'BME680.inc'			;BME 680 main routine
                INCLUDE 'SPI_V10.inc'
                INCLUDE 'Dummy_Isr.inc'         ; F�r fehlgeschlagene (unerw�nschte) Interrupts
                INCLUDE 'Vectors.inc'           ; Vektoren usw.
