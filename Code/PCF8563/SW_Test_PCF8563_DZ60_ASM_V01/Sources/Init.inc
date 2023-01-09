;************************************************************************************************
;*                                                                                              *
;*      Main_Init: Hier beginnt der Controller mit der Arbeit                                   *
;*                                                                                              *
;************************************************************************************************


Main_Init
                ldhx    #E_RAM_END              ; Setze meinen Stackpointer ganz an das Ende der Extended RAM
                txs
                
                jsr     Clear_Zero_Ram          ; [.] Löscht die ZEROPAGE-RAM
                jsr     Clear_Extended_Ram      ; [.] Löscht die EXTENDED-RAM
                

                jsr     Init_MCU                ; [.] Controller Clock usw.       
                jsr     Init_Realtime           ; [Realtime.inc] Realtime
                ;jsr		Init_LCD
                ;jsr		Init_ADC				;init for the ADC
                jsr		Init_I2C
                ;jsr		Init_EEPROM_Text
                ;jsr		Init_Tasten
                jsr		Set_StartTime_PCF8563
                
                cli                             ; Interrupts freigeben

                jmp     Main_Loop

                


;************************************************************************************************
;*                                                                                              *
;*      Initialisierung verschiedener Register der MCU                                          *
;*                                                                                              *
;************************************************************************************************

Init_MCU

                ; Siehe Seite 154 des Datasheet
                
              ;  lda     ;MCGC2 = 0x36 (%00110110)
                ;Loop until OSCINIT (bit 1) in MCGSC is 1,
                ;MCGC1 = 0xB8 (%10111000)
                ;Loop until IREFST (bit 4) in MCGSC is 0,
                ;Loop until IREFST (bit 4) in MCGSC is 0,
                ;first set LP (bit 3) in MCGC2 to 1.
                ;BLPE/PBE: MCGC1 = 0x90 (%10010000)
                ;BLPE/PBE: MCGC3 = 0x44 (%01000100)
                ;clear LP (bit 3) in MCGC2 to 0 here to switch to PBE mode
                ;Loop until PLLST (bit 5) in MCGSC is se
                ;Then loop until LOCK (bit 6) in MCGSC is set
                ;MCGC1 = 0x10 (%00010000)
                ;Loop until CLKST (bits 3 and 2) in MCGSC are %11,

        ; SOPT1: COPT=0,STOPE=0,SCI2PS=0,IICPS=0 
        CLRA
        STA     SOPT1                                               
        ; SOPT2: COPCLKS=0,COPW=0,ADHTS=0,MCSEL=0 
        CLRA
        STA     SOPT2                                               
        ; SPMSC1: LVWF=0,LVWACK=0,LVWIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 
        LDA     #$1C
        STA     SPMSC1                                               
        ; SPMSC2: LVDV=0,LVWV=0,PPDF=0,PPDACK=0,PPDC=0 
        CLRA
        STA     SPMSC2                                               
  ;  System clock initialization 
        ;Test if the device trim value is stored on the specified address
        LDA     $FFAF
        CBEQA   #$FF, SkipTrim
        ; MCGTRM: Initialize internal clock trim from a non volatile memory 
        LDA     $FFAF
        STA     MCGTRM
        ; MCGSC: Initialize internal clock trim from a non volatile memory 
        LDA     $FFAE
        STA     MCGSC
SkipTrim:
        ; MCGC2: BDIV=2,RANGE=1,HGO=1,LP=0,EREFS=1,ERCLKEN=1,EREFSTEN=0 
        MOV     #$B6,MCGC2             ; Set MCGC2 register 
        ; MCGC1: CLKS=0,RDIV=7,IREFS=0,IRCLKEN=0,IREFSTEN=0 
        MOV     #$38,MCGC1             ; Set MCGC1 register 
        ; MCGC3: LOLIE=0,PLLS=0,CME=0,VDIV=1 
        MOV     #$01,MCGC3             ; Set MCGC3 register 
while_Cpu0:                            ; Wait until external reference is stable 
        BRCLR   1,MCGSC,while_Cpu0
whileSet_Cpu0:                         ; Wait until external reference is selected 
        BRSET   4,MCGSC,whileSet_Cpu0
while_Cpu1:                            ; Wait until FLL is locked 
        BRCLR   6,MCGSC,while_Cpu1
whileVal_Cpu0:                         ; Wait until FLL clock is selected as a bus clock reference 
        LDA     MCGSC
        AND     #$0C
        BNE     whileVal_Cpu0
  

; Initialization of the CPU registers in FLASH 

        ; NVPROT: EPS=3,FPS=$3F 
   ;     ORG     NVPROT
   ;     DC.B    $FF

        ; NVOPT: KEYEN=0,FNORED=1,EPGMOD=1,SEC1=1,SEC0=0 
    ;    ORG     NVOPT
    ;    DC.B    $7E




                rts
                
                
;************************************************************************************************
;*                                                                                              *
;*      Routinen zum Löschen der gesamten RAM (Zero+Extended)                                   *
;*                                                                                              *
;************************************************************************************************




Clear_Zero_Ram
                lda     #0T
                ldhx    #Z_RAM_START
Next_Clear_Zero
                sta     0,X
                aix     #1T
                cphx    #Z_RAM_END
                bne     Next_Clear_Zero
                rts
                
Clear_Extended_Ram                
                lda     #0T
                ldhx    #E_RAM_START
Next_Clear_Extended
                sta     0,X
                aix     #1T
                cphx    #E_RAM_END-4T
                bne     Next_Clear_Extended   ; Den Stack lasse ich leben...
                rts
                
                