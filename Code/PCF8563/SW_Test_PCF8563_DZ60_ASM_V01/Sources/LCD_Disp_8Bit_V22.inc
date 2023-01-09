;************************************************************************************************
;*	LCD-Treiber für AW-32-Demoboard	- steuert ein 16x2 LCD an				*
;*	(c) Deto 2005/2017      								*
;*												*
;*	In Ram folgende Variablen einfügen:							*
;*												*
;* 	LCD_Pointer             ds      2T              ; Pointer auf die aktuelle LCD-Routine	*
;*	LCD_Counter             ds      1T							*
;*	LCD_Oben                ds      16T             ; LCD erste Zeile, 16 Zeichen lang	*
;*	LCD_Unten               ds      16T             ; LCD zweite Zeile, 16 Zeichen lang	*
;*	DelayCounter            ds      1T              ; Grässlich - Delay für LCD...		*
;*	CopyCounter             ds      1T							*
;*	SourcePointer           ds      2T              ; Pointer für Copy-Routinen		*
;*	DestPointer             ds      2T              ; Ebenfalls für die Copy-Routinen	*
;*	TextLenght              ds      1T							*
;*      DezBCD_Value            ds      2T              ; Anzuzeigende 16 Bit-Zahl              *
;*      BCD_Counter             ds      1T                                                      *
;*												*
;*	In der Main.inc (Initialisierung) folgende Routine aufrufen:				*
;*												*
;*	jsr		Init_LCD		; [LCD_Disp_8Bit.inc]  Initialisiert das LCD	*
;*												*
;*	Vom Hauptloop (MainLoop) wie folgt aufrufen:						*
;*												*
;*      jsr     	Update_LCD              ; [LCD_Disp_8Bit.inc] Beschreibt das LCD	*
;*												*
;*												*
;*	Noch zu beachten:									*
;*												*
;*	Die Equates sind richtig zu stellen, auf welchen Ports/Bits das LCD hängt!		*
;*												*
;*	Im Beispiel unten hängt das LCD mit dem Datenport auf PORTA, während die zwei 		*
;*	notwendigen Steuerleitungen auf PG0 ("E"-Steuerleitung) bzw. PG1 ("RS"-Steuerleitung)	*
;*	liegen.											*
;*												*
;*	Zur Funktion:										*
;*												*
;*	Alles was in den Variablen "LCD_Oben" bzw. "LCD-Unten" steht wird 1:1 auf das LCD	*
;*	geschrieben, das macht der Treiber dann selbstständig. Zum Reinkopieren eventuell	*
;*	Routinen "CopyText" usw. verwenden!							*
;*												*
;************************************************************************************************





;************************************************************************************************
;*                                                                                              *
;*      Initialisierung des LCD-Displays auf 8 Bit                                              *
;*                                                                                              *
;************************************************************************************************

LCD_E_BIT       EQU             3                       ; Definitionen, auf welchem Port die
LCD_E_PORT      EQU             PTGD                    ; Bits "E" und "RS" des LCD hängen
LCD_E_DDR	EQU		PTGDD			; Direction-Register für Pin "E"

LCD_RS_BIT      EQU             2			
LCD_RS_PORT     EQU             PTGD
LCD_RS_DDR	EQU		PTGDD


LCD_DATA_PORT   EQU             PTAD                   ; Auf welchem Port hängt das LCD (Datanbus)?
LCD_DATA_DIR	EQU		PTADD
DELTIME         EQU             40T                                        

LCD_BCKL_BIT	EQU		4			; Hintergrundbeleuchtung des LCD
LCD_BCKL_PORT	EQU		PTGD                   
LCD_BCKL_DDR	EQU		PTGDD


;************************************************************************************************
;*                                                                                              *
;*      Init_LCD: Initialisiert Pointer und Variablen für das LCD                               *
;*                                                                                              *
;************************************************************************************************

Init_LCD
		bset	LCD_E_BIT,LCD_E_DDR	; Das "E"-Bit wird als Ausgang definiert
		bset	LCD_RS_BIT,LCD_RS_DDR	; Das "RS"-Bit wird als Ausgang definiert
		lda	#%11111111
		sta	LCD_DATA_DIR		; Der "Data"-Port (ganze 8-Bit) werden als Output gesetzt
		
                lda     #0T                     ; Counter auf Null setzen
                sta     LCD_Counter
                ldhx    #LCD_Wait_Start         ; LCD-Schrittkette beginnt mit diesem Schritt
                sthx    LCD_Pointer
                
		bset	LCD_BCKL_BIT,LCD_BCKL_DDR	; Hintergrundbeleuchtung: Pin auf OUT
		bset	LCD_BCKL_BIT,LCD_BCKL_PORT	; und einschalten!

		ldhx	#Init_Text_LCD
		sthx	SourcePointer
		ldhx	#LCD_Oben
		sthx	DestPointer
		lda	#32T
		sta	TextLenght
		jsr	CopyText
                 
                rts


Init_Text_LCD
		dc.b	'                '
		dc.b	'                '

;************************************************************************************************
;*                                                                                              *
;*      Update_LCD: Wird vom Main_Loop aufgerufen, kümmert sich um das Beschreiben des LCD      *
;*      Führt praktisch nur nacheinander die verschienden Schritte der Schrittkette aus         *
;*                                                                                              *
;************************************************************************************************
                
Update_LCD                
                ldhx    LCD_Pointer
                jsr     0,X
                rts

;************************************************************************************************
;*                                                                                              *
;*      LCD_Wait_Start: Wartet am Anfang 50 msec, damit sich das LCD intern initialisieren      *
;*      kann.                                                                                   *
;*                                                                                              *
;************************************************************************************************


LCD_Wait_Start
                lda     LCD_Counter
                inca
                sta     LCD_Counter
                cmpa    #10T                     ; Warte 10 x 5msec R.T. = 50 msec
                bne     Exit_LCD_Wait
                ldhx    #LCD_Initializing
                sthx    LCD_Pointer

Exit_LCD_Wait
                rts

;************************************************************************************************
;*                                                                                              *
;*      LCD_Initialisierung: Initialisiert hardwaremäßig das LCD laut Datenblatt                *
;*                                                                                              *
;************************************************************************************************


LCD_Initializing
                lda     #$03
                jsr     Write_LCD_Instruction
                jsr     Delay

                lda     #$03
                jsr     Write_LCD_Instruction
                jsr     Delay


                lda     #$03
                jsr     Write_LCD_Instruction
                jsr     Delay
                                                ;                   x  x  x  DL N  F  x  x

                lda     #%00111000              ; $28 Function Set: 0  0  1  1  1  0  0  0
                jsr     Write_LCD_Instruction   ; DL: 0=4-Bit-Interface  1=8-Bit-Interface
                jsr     Delay


                lda     #%00001100              ; $0C  Display On/Off
                jsr     Write_LCD_Instruction
                jsr     Delay

                                                ;                  x  x  x  x  x  x  ID S
                lda     #%00000110              ; $06  Entry Mode: 0  0  0  0  0  1  1  0
                jsr     Write_LCD_Instruction   ; ID : 0=Adress decrement 1=Adress Increment
                jsr     Delay

                ldhx    #LCD_Sonderzeichen
                sthx    LCD_Pointer

                rts



;************************************************************************************************
;*                                                                                              *
;*      LCD_Sonderzeichen: Definiert die 8 Sonderzeichen in der CG-Ram des Display        	*
;*      Die CG-Ram (Charakter-Graphics Ram) startet bei $40 und geht bis $7F, hat also 64 Bytes *
;*      Jedes selbst definierte Zeichen ist 5x7 Pixel groß, besteht also aus 8 Zeilen           *
;*      (7 Zeilen + 1 Cursorzeile), die jeweils 5 Pixel breit sind. Die ersten drei Bits eines  *
;*      Zeichens (Bit 7,6 und 5) werden also nicht gebraucht und bleiben folglich auf "0"!      *
;*                                                                                              *
;************************************************************************************************


LCD_Sonderzeichen
                lda     #$40                    ; $40 = Anfang CG Ram --> Setze den Schreibepointer
                jsr     Write_LCD_Instruction   ; auf diese Adresse und beginne, die einzelnen Bytes
                jsr     Delay                   ; zu senden

                clra
                sta     LCD_Counter

                ldhx    #Sonderzeichen-1
Next_Sonderzeichen
                aix     #1T
                lda     0,X
                jsr     Write_Lcd_Data
                jsr     Delay2
                lda     LCD_Counter
                inca
                sta     LCD_Counter
                cmpa    #64T                    ; 8 Zeichen á 8 Byte = 64 Bytes
                bne     Next_Sonderzeichen      ; Theoretisch können max. 8 Zeichen definiert werden!!

                ldhx    #LCD_Cursor_Home
                sthx    LCD_Pointer
                
                clra
                sta     LCD_Counter

		rts





Sonderzeichen
                ; Zeichen mit Adresse $00 --> in der CG-Ram von $40 bis $47 gespeichert
                
                dc.b    %00000100       ; Pfeil nach unten       
                dc.b    %00000100       ; Die drei hochwertigsten Bits (Bit 7, 6 und 5) werden
                dc.b    %00000100       ; nicht gebraucht und sind immer auf 0!
                dc.b    %00000100       ; Ein Zeichen ist 5 Bit breit und 8 Bit hoch, wobei die
                dc.b    %00011111       ; letzte Zeile die Cursorzeile ist
                dc.b    %00001110
                dc.b    %00000100
                dc.b    %00000000       ; Letzte Zeile: Cursorzeile!

                ; Zeichen mit Adresse $01 --> in der CG-Ram von $48 bis $4F gespeichert
                
                dc.b    %00000100       ; Pfeil nach oben
                dc.b    %00001110
                dc.b    %00011111
                dc.b    %00000100
                dc.b    %00000100
                dc.b    %00000100
                dc.b    %00000100
                dc.b    %00000000

                ; Zeichen mit Adresse $02 --> in der CG-Ram von $50 bis $57 gespeichert
                
                dc.b    %00000001       ; Balken Breite 1
                dc.b    %00000001
                dc.b    %00000001
                dc.b    %00000001
                dc.b    %00000001
                dc.b    %00000001
                dc.b    %00000001
                dc.b    %00000000

                ; Zeichen mit Adresse $03 --> in der CG-Ram von $58 bis $5F gespeichert
                
                dc.b    %00000011       ; Balken Breite 2
                dc.b    %00000011
                dc.b    %00000011
                dc.b    %00000011
                dc.b    %00000011
                dc.b    %00000011
                dc.b    %00000011
                dc.b    %00000000

                ; Zeichen mit Adresse $04 --> in der CG-Ram von $60 bis $67 gespeichert
                
                dc.b    %00000111       ; Balken Breite 3
                dc.b    %00000111
                dc.b    %00000111
                dc.b    %00000111
                dc.b    %00000111
                dc.b    %00000111
                dc.b    %00000111
                dc.b    %00000000

                ; Zeichen mit Adresse $05 --> in der CG-Ram von $68 bis $6F gespeichert
                
                dc.b    %00001111       ; Balken Breite 4
                dc.b    %00001111
                dc.b    %00001111
                dc.b    %00001111
                dc.b    %00001111
                dc.b    %00001111
                dc.b    %00001111
                dc.b    %00000000

                ; Zeichen mit Adresse $06 --> in der CG-Ram von $70 bis $77 gespeichert
                
                dc.b    %00011111       ; Balken Breite 5
                dc.b    %00011111
                dc.b    %00011111
                dc.b    %00011111
                dc.b    %00011111
                dc.b    %00011111
                dc.b    %00011111
                dc.b    %00000000

                ; Zeichen mit Adresse $07 --> in der CG-Ram von $78 bis $7F gespeichert
                
                dc.b    %00000100       ; "Raumschiff"... Na ja...
                dc.b    %00001110
                dc.b    %00001110
                dc.b    %00010001
                dc.b    %00010001
                dc.b    %00011111
                dc.b    %00000000
                dc.b    %00000000



;************************************************************************************************
;*                                                                                              *
;*      LCD_Cursor_Home: Setzt den Cursor (nicht sichtbar) in die "Home"-Position               *
;*                                                                                              *
;************************************************************************************************

LCD_Cursor_Home
                lda     #$80                    ; Cursor Home
                jsr     Write_LCD_Instruction   ; ID : 0=Adress decrement 1=Adress Increment

                ldhx    #LCD_Write_Zeile1
                sthx    LCD_Pointer
                
                clra
                sta     LCD_Counter
                rts

;************************************************************************************************
;*                                                                                              *
;*      LCD_Write_Zeile1: Schreibt die erste Zeile auf das Display                              *
;*                                                                                              *
;************************************************************************************************

LCD_Write_Zeile1

                ldhx    #LCD_Oben-1
Next_Zeile1
                aix     #1T
                lda     0,X
                jsr     Write_Lcd_Data
                jsr     Delay2
                lda     LCD_Counter
                inca
                sta     LCD_Counter
                cmpa    #16T
                bne     Next_Zeile1

                ldhx    #LCD_Goto_Zeile2
                sthx    LCD_Pointer
                
                clra
                sta     LCD_Counter

                rts


;************************************************************************************************
;*                                                                                              *
;*      LCD_Goto_Zeile2: Setzt den Cursor an den Anfang der zweiten Zeile                       *
;*                                                                                              *
;************************************************************************************************

LCD_Goto_Zeile2
                lda     #$C0                    ; Cursor zweite Zeile
                jsr     Write_LCD_Instruction   ; ID : 0=Adress decrement 1=Adress Increment

                ldhx    #LCD_Write_Zeile2
                sthx    LCD_Pointer
                clra
                sta     LCD_Counter
                rts

;************************************************************************************************
;*                                                                                              *
;*      LCD_Write_Zeile2: Schreibt die zweite Zeile auf das Display                             *
;*                                                                                              *
;************************************************************************************************

LCD_Write_Zeile2
                ldhx    #LCD_Unten-1
Next_Zeile2
                aix     #1T
                lda     0,X
                jsr     Write_Lcd_Data  
                jsr     Delay2
                lda     LCD_Counter
                inca
                sta     LCD_Counter
                cmpa    #16T
                bne     Next_Zeile2

                ldhx    #LCD_Cursor_Home
                sthx    LCD_Pointer
                clra
                sta     LCD_Counter

                rts



;************************************************************************************************
;*                                                                                              *
;*      LCD_Instruction: Schreibt eine "Instruction" (Befehl) auf das LCD                       *
;*                                                                                              *
;************************************************************************************************


Write_LCD_Instruction
                bclr    LCD_RS_BIT,LCD_RS_PORT            ; RS = 0
                nop
                nop
                sta     LCD_DATA_PORT
                nop
                nop
                nop
                nop
                bset    LCD_E_BIT,LCD_E_PORT             ; Clock auf "E"
                nop
                nop
                nop
                nop
                nop
                nop
                bclr    LCD_E_BIT,LCD_E_PORT
                rts

Write_Lcd_Data
                bset    LCD_RS_BIT,LCD_RS_PORT            ; RS = 1
                nop
                nop
                nop
                nop
                sta     LCD_DATA_PORT
                nop
                nop
                nop
                nop
                nop
                nop
                bset    LCD_E_BIT,LCD_E_PORT             ; Clock auf "E"
                nop
                nop
                bclr    LCD_E_BIT,LCD_E_PORT
                rts




Delay           ldhx    #$500
next_x          aix     #-1
                cphx    #0
                bne     next_x
                rts


Delay2
                lda     DelayCounter
                inca
                sta     DelayCounter
                cmpa    #DELTIME
                bne     Delay2
                clra
                sta     DelayCounter
                rts

;************************************************************************************************
;*                                                                                              *
;*      Utility: Kopiert einen "TextLenght"-langen Text von einer Position (SourcePointer)      *
;*      in eine andere (DestPointer) um.                                                        *
;*                                                                                              *
;************************************************************************************************

CopyText
        clra
        sta     CopyCounter
NextCopyText
        ldhx    SourcePointer
        lda     0,X
        aix     #1
        sthx    SourcePointer
        ldhx    DestPointer
        sta     0,X
        aix     #1T
        sthx    DestPointer
        lda     CopyCounter
        inca
        sta     CopyCounter
        cmpa    TextLenght                  ; Lenght = Anzahl der Zeichen, die zu kopieren sind
        bne     NextCopyText
 rts

;************************************************************************************************
;*                                                                                              *
;*      Dezimal-BCD: Wandelt eine Dezimalzahl in eine BCD-Zahl um und schreibt sie              *
;*      in die Position "DestPointer", "DestPointer+1" und "DestPointer+2"                      *
;*      Die zu wandelnde Zahl steht im Register A                                               *
;*                                                                                              *
;************************************************************************************************


DezimalBCD
        clrh                            ; da 8 Bit-Division: H = 0, A hat ja schon den Wert, X
        ldx     #10T                    ; noch mit 10 laden
        div
        psha                           ; Ergebnis in A, Rest in H
        pshh                           ; Rette das Ergebnis auf den Stack
        pula
        add     #$30                   ; "0" dazuzählen
        ldhx    DestPointer
        sta     2,X
        pula
        clrh
        ldx     #10T
        div
        psha
        pshh
        pula
        add     #$30
        ldhx    DestPointer
        sta     1,X
        pula
        clrh
        ldx     #10T
        div
        pshh
        pula
        cmpa    #0T
        beq     BCD_NoZero              ; Kill leading Zero...
        add     #$30
        ldhx    DestPointer
        sta     0,X
        rts
BCD_NoZero
        lda     #' '
        ldhx    DestPointer
        sta     0,X
        lda     1,X
        cmpa    #'0'
        bne     Exit_DBCD               ; Kill second Zero if first even zero
        lda     #' '
        sta     1,X
Exit_DBCD        
        rts


;************************************************************************************************
;*                                                                                              *
;*      Dezimal-BCD_16Bit: Wandelt eine Dezimalzahl in eine BCD-Zahl um und schreibt sie        *
;*      in die Position "DestPointer", "DestPointer+1" ... "DestPointer+4"                      *
;*      Die zu wandelnde Zahl steht in der Variable "DezimalBCD_Value" (16 Bit Variable)        *
;*                                                                                              *
;************************************************************************************************


DezimalBCD_16Bit
        lda     #0T
        sta     BCD_Counter

CheckTenTousand        
        ldhx    DezBCD_Value
        cphx    #10000T
        blo     TenTousandsOk                   ; 10.000T = $2710
        lda     DezBCD_Value+1
        sub     #$10
        sta     DezBCD_Value+1
        lda     DezBCD_Value
        sbc     #$27
        sta     DezBCD_Value
        lda     BCD_Counter
        inca
        sta     BCD_Counter
        bra     CheckTenTousand
TenTousandsOk
        sthx    DezBCD_Value
        add     #$30
        ldhx    DestPointer
        sta     0,X
        
        lda     #0T
        sta     BCD_Counter
CheckTousand        
        ldhx    DezBCD_Value
        cphx    #1000T
        blo     TousandsOk                   ; 1.000T = $03E8
        lda     DezBCD_Value+1
        sub     #$E8
        sta     DezBCD_Value+1
        lda     DezBCD_Value
        sbc     #$03
        sta     DezBCD_Value
        lda     BCD_Counter
        inca
        sta     BCD_Counter
        bra     CheckTousand
TousandsOk
        sthx    DezBCD_Value
        add     #$30
        ldhx    DestPointer
        sta     1,X

        ; Ab hier: mit einfacher Division machbar!!
        
        
        ldhx    DezBCD_Value
        txa                            ; da 8 Bit-Division: H = 0, A hat ja schon den Wert, X
        ldx     #10T                   ; noch mit 10 laden
        div
        psha                           ; Ergebnis in A, Rest in H
        pshh                           ; Rette das Ergebnis auf den Stack
        pula
        add     #$30                   ; "0" dazuzählen
        ldhx    DestPointer
        sta     4,X
        pula
        clrh
        ldx     #10T
        div
        psha
        pshh
        pula
        add     #$30
        ldhx    DestPointer
        sta     3,X
        pula
        clrh
        ldx     #10T
        div
        pshh
        pula
        add     #$30
        ldhx    DestPointer
        sta     2,X

        ; Fehlt: Kill leading Zeros...
        
        ldhx    DestPointer
        
KillZeroAgain        
        lda     0,X
        cmpa    #'0'
        bne     Exit_BCD16Bit
        lda     #' '
        sta     0,X

        aix     #1T
        beq     KillZeroAgain
        
        
Exit_BCD16Bit
        rts



