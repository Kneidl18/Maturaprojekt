Init_EEPROM_Text
        ldhx    #LCD_Oben
        sthx    DestPointer
        ldhx    #EE_Text
        sthx    SourcePointer
        lda     #32T
        sta     TextLenght
        jsr     CopyText


        rts



EE_Text
        dc.b    'Adr:            '
        dc.b    'Val:    Read:   '
        



Test_EEPROM

        lda     Analog_PB0
        ldhx    #LCD_Oben+4T
        sthx    DestPointer
        jsr     DezimalBCD
        
        lda     Analog_PB1
        ldhx    #LCD_Unten+4T
        sthx    DestPointer
        jsr     DezimalBCD
        
        lda     ShipDown_Flag
        cmpa    #%00000001
        bne     Read_Only_Test_EEPROM

        lda     Analog_PB0
        sta     EE_Adresse
        
        lda     Analog_PB1
        sta     EE_Wert
        
        jsr     EEPROM_Write_Byte
        
        rts
        
Read_Only_Test_EEPROM

        lda     Analog_PB0
        sta     EE_Adresse
        
        jsr     EEPROM_Read_Byte
        
        lda     I2C_Byte
        ldhx    #LCD_Unten+13T
        sthx    DestPointer
        jsr     DezimalBCD
        
        rts


;************************************************************************************************
;*                                                                                              *
;*      EEPROM_Write_Byte: Schreibt ein Byte an Speicherzelle $0000 - und zwar mit dem          *
;*      Wert, der in "EE_Wert" gespeichert ist, an Adresse in "EE_Adresse"                      *
;*                                                                                              *
;************************************************************************************************



EEPROM_Write_Byte

        jsr     I2C_Start
        
        lda     #%10100000              ; EEPROM Adress = 1010 + Chip Select Bits = 000   R/W\ = 0   1010 000 0 
        sta     I2C_Byte
        jsr     I2C_Send_Byte
        jsr     I2C_Get_Ack
        
        lda     #$00                    ; Adresse HIGH-Byte
        sta     I2C_Byte
        jsr     I2C_Send_Byte
        jsr     I2C_Get_Ack

        lda     EE_Adresse              ; Adresse Low_Byte
        sta     I2C_Byte
        jsr     I2C_Send_Byte
        jsr     I2C_Get_Ack
        
        
        lda     EE_Wert                 ; Das abzuspeichernde Byte
        sta     I2C_Byte
        jsr     I2C_Send_Byte
        jsr     I2C_Get_Ack


        jsr     I2C_Stop

        rts
  
  
;************************************************************************************************
;*                                                                                              *
;*      EEPROM_Load_Byte: Holt das Byte herein, das auf Speicherzelle "EE_Adresse" gespeichert  *
;*      ist und schreibt es in I2C_Byte                                                         *
;*                                                                                              *
;************************************************************************************************
  
        
EEPROM_Read_Byte        

        jsr     I2C_Start
        
        lda     #%10100000            ; EEPROM Adress = 1010 + Chip Select Bits = 000   R/W\ = 0   1010 000 0<-- Write! 
        sta     I2C_Byte
        jsr     I2C_Send_Byte
        jsr     I2C_Get_Ack
        
        lda     #$00                    ; Adresse HIGH-Byte
        sta     I2C_Byte
        jsr     I2C_Send_Byte
        jsr     I2C_Get_Ack

        lda     EE_Adresse              ; Adresse Low_Byte
        sta     I2C_Byte
        jsr     I2C_Send_Byte
        jsr     I2C_Get_Ack

        jsr     I2C_Start               ; Sende eine erneute Startbedingung OHNE vorher ein Stop gesendet zu haben!

        lda     #%10100001              ; EEPROM Adress = 1010 + Chip Select Bits = 000   R/W\ = 0   1010 000 1<-- Read!
        sta     I2C_Byte
        jsr     I2C_Send_Byte
        jsr     I2C_Get_Ack

		jsr     I2C_Read_Byte

        jsr     I2C_Put_NAck

        jsr     I2C_Stop

        rts