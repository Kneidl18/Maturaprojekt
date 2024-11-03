;************************************************************************************************
;*                                                                                              *
;*      Initialisieren der internen SPI-Schnittstelle - sie läuft auf folgenden Pins:           *
;*                                                                                              *
;*      PE4     /SS                                                                             *
;*      PE5     /MISO                                                                           *
;*      PE6     /MOSI                                                                           *
;*      PE7     /SPSCK                                                                          *
;*                                                                                              *
;*	ACHTUNG #1!!! Das Signal aus MOSI muss extern einmal invertiert werden, da es sonst	*
;*	verkehrt rum ist!! Z.B. durch einen HC14! Da zwischen den einzelnen Bytes ein kleiner	*
;*	Gap ist kann es nicht einfach software-mäßig invertiert werden, da sonst der Idle-	*
;*	Pegel High ist und somit kleine Spikes auftreten? Irgend so was war das Problem!	*
;*												*
;*	Achtung #2!!! Der Quarz lief bei diesem Beispiel mit doppelter Frequenz, also mit	*
;*	9,8304 MHz statt mit den üblichen 4,9152 MHz! Muss beachtet werden, da sonst die	*
;*	Zeiten nicht stimmen! Siehe Datenblatt NeoPixel/WS18?? o.Ä.!				*
;*												*
;************************************************************************************************

; Bits in Register SPI1S1

SPRF    EQU     7       ; SPI Read Buffer Full Flag   0:No data in Receiver Buffer   1:Data in Receiver Buffer available
SPTEF   EQU     5       ; SPI Transmit Empty Flag     0:Transmit Buffer not empty    1:Transmit Buffer empty (ready to send!)
MODF    EQU     4       ; Master Mode Fault Flag

; Bits in Register SPI1C1

SPIE    EQU     7       ; SPI Interrupt Enable 
SPE     EQU     6       ; SPI Enable
SPTIE   EQU     5       ; SPI Transmit Interrupt Enable 
MSTR    EQU     4       ; Master/Slave select  0:Slave 1:Master
CPOL    EQU     3       ; Clock Polarity 0:active High   1:active Low
CPHA    EQU     2       ; Clock Phase  0:Middle  1:start
SSOE    EQU     1       ; Slave select Output Enable
LSBFE   EQU     0       ; LSB first  0: SPI data with MSB first  1:LSB first

; Bits in Register SPI1C2

MODFEN  EQU     4       ; Master Mode Fault function enable  0:disable  1:enable
BIDIROE EQU     3       ; Bidirectional Mode Output enable
SPISWAI EQU     1       ; SPI Sop in Wait
SPC0    EQU     0       ; SPI Pin control: 0:Two wires  1:Single wire, bidirectional



        
Init_SPI
    mov   #%01010000,SPI1C1   ; SPI Aktiviert als Master
;           ||||||||
;           ||||||||_____  Shifter direction -->        0:MSB shift out first!
;           |||||||______  SS Output enable             0:Pin under Port control (wenn MODFEN in SPI1C2 =0!)
;           ||||||_______  Clock Phase,                 0:Middle of Cycle
;           |||||________  Clock Polarity               0:Active High
;           ||||_________  MSTR,                        1:Master
;           |||__________  Transmit Interrupt enable    0:OFF
;           ||___________  SPI enable                   1:ON
;           |____________  Interrupt enable             0:OFF

    mov   #%00000000,SPI1C2   
;           ||||||||
;           ||||||||_____  Bidirectional enable
;           |||||||______  SPI Stop in Wait mode
;           ||||||_______  nc
;           |||||________  Bidirectional, Output enable
;           ||||_________  Mode-Fault enable
;           |||__________  nc
;           ||___________  nc
;           |____________  nc
 
 

    mov   #%00000000,SPI1BR     ; Stellt den SPI_Clock ein (Baudrate)
;           ||||||||
;           ||||||||_____  )
;           |||||||______  > Timer Scaler 2..256 (2)
;           ||||||_______  )
;           |||||________  nc
;           ||||_________  )
;           |||__________  > Timer Prescaler 1..8
;           ||___________  )
;           |____________  nc

        bset    7,PTEDD         ; Pin PE6 --> OUTPUT (SPSCK1)
        bset    6,PTEDD         ; Pin PE6 --> OUTPUT (MOSI)
        bclr    5,PTEDD         ; Pin PE5 --> INPUT (MISO)
        
        
        lda     #%00000001
        sta     PTEDS         ;Define Strength,  Datasheet  6.5.3  S.87
                                ;Pin6=MOSI
                                
                          
        lda #0T
        sta SPI_Delay             
        sta SPI_Data_Len
        sta Farbe
       

    rts


;************************************************************************************************
;*                                                                                              *
;*      SPI_Test: alle Sekunde wird ein Datenstream (einfach ein paar Zahlen)  rausgesendet     *
;*                                                                                              *
;************************************************************************************************



;Connections WS2812    Rot = 5V, Weiss = GND, Grün = Signal
    
    
SPI_Ausgabe
        
        
        lda     SPI_Delay            ; 0,5 Sekunden Delay Counter
        add     #1T
        sta     SPI_Delay
        cmpa    #100T
        bne     Exit_SPI_Test
        lda     #0T
        sta     SPI_Delay
                
        lda     #196T
        sta     LoopCounter
        
        ldhx    #Muster
        sthx    MusterPointer
         sei
Great_Loop  
      
        lda     #24T
        sta     SPI_Data_Len
        
        ldhx    MusterPointer
        lda     0,X
        aix     #1T
        sthx    MusterPointer        
        
        ldx     #24T
        mul
        sta     Temp
       
        
        ldhx    #Farbentabelle
        ldx     Temp

        sei    
                                             ;Interrupts deaktivieren
Repeat_Send 
        lda     SPI1S 
        lda     0,X
        
        sta     SPI1D
        aix     #1T
        dbnz    SPI_Data_Len,Repeat_Send
        cli                                        ;Interrupts wieder erlauben
        
        dbnz    LoopCounter,Great_Loop
        
Exit_SPI_Test        
        rts



        ORG       $A000
        
Farbentabelle

;_________________________________________________________________________________________________

; Farbe 0 = Aus/Schwarz
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111   
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111

; Farbe 1 = Weiss,Max
	 dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011 
	 dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011 
	 dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011

;_________________________________________________________________________________________________

; Farbe 2 = Rot,Max
   dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011       
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111                                            
	 
; Farbe 3 = Grün,Max
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111   
   dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011                    
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
   
; Farbe 4 = Blau,Max
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
	 dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011 
	 
; Farbe 5 = Gelb,Max
	 dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011                    
   dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111 
	 
; Farbe 6 = Violett,Max
   dc.b            %01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111,%00000011                    
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
   dc.b            %01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111,%00000011
   
; Farbe 7 = Türkis,Max
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
	 dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011       
	 dc.b            %00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011,%00000011   

;_________________________________________________________________________________________________

; Farbe 8 = Weiss,Gedimmt,190
	 dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111 
	 dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111 
	 dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111
	 
; Farbe 9 = Rot,Gedimmt,190	 
	 dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111

; Farbe 10 = Grün,Gedimmt,190
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
   dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111	 
	 
; Farbe 11 = Blau,Gedimmt,190	 
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
   dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111

; Farbe 12 = Gelb,Gedimmt,190
	 dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111                    
   dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111
	 dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111 
	 
; Farbe 13 = Violett,Gedimmt,190
   dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111                    
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
   dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111
   
; Farbe 14 = Türkis,Gedimmt,190
   dc.b            %01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111,%01111111
	 dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111       
	 dc.b            %00000011,%01111111,%00000011,%00000011,%00000011,%00000011,%00000011,%01111111   

;_________________________________________________________________________________________________	 	 	 
	 
	 
	 
	 
	        	  