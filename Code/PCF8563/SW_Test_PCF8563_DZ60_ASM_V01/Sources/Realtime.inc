;************************************************************************************************
;*                                                                                              *
;*      Init_Realtime: Initialisiert den RealTime                                               *
;*                                                                                              *
;*      BusClock: Bei 4,9152 MHz-Quarz wäre der Busclock zwar ein Viertel davon, aber der FLL   *
;*      taktet den Bus um das Vierfache auf 19,6608 MHz rauf. Somit ist der Busclock effektiv   *
;*      4,9152 MHz, oder als Kehrwert 203,451 nsec.                                             *
;*      Der Realtime zählt nun diese Busclocks rauf (Einheiten zu 203,451 ns)                   *
;*                                                                                              *
;*      Bei $6000 Zyklen (Dezimal: 49.152 Zyklen) á 203,451 nsec ergibt das EXAKT 10 msec       *
;*      Wenn ich nur 8192 hinaufzähle, dann kommt ein Wert von  1,666667 ms raus!               *
;*                                                                                              *
;************************************************************************************************



Init_Realtime
        lda		#47T
        sta		RTCMOD            ; Set modulo register 


		lda		#%10110001
		sta		RTCSC

        rts
       

;************************************************************************************************
;*                                                                                              *
;*      Realtime: Diese Interrupt-Routine wird alle 1,666 msec aufgerufen                       *
;*      8192 Takte á 203,451... nsec = 1,66666 msec (Ein Sechstel von 10 msec)                  *
;*                                                                                              *
;************************************************************************************************


Realtime
       pshh
       lda		#%10110001		; Löscht das RTCSC-Flag
       sta		RTCSC	
       
       lda      #1T
       sta      TimerFlag               


Exit_Realtime       
       pulh

       rti