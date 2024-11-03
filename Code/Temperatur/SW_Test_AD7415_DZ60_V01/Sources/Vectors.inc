;************************************************************************************************
;*                                                                                              *
;*                 Interrupt Vectoren                                                           *
;*                                                                                              *
;************************************************************************************************
      
      ORG  $FFC0
      
        dc.w    Dummy_ISR           ; $FFC0     ACMP2
        dc.w    Dummy_ISR           ; $FFC2     ACMP1
        dc.w    Dummy_ISR           ; $FFC4     CANTX
        dc.w    Dummy_ISR           ; $FFC6     CANRX
        dc.w    Dummy_ISR           ; $FFC8     CANERR
        dc.w    Dummy_ISR           ; $FFCA     CANWU
        dc.w    Realtime            ; $FFCC     RTI
        dc.w    Dummy_ISR           ; $FFCE     IIC1
        dc.w    Dummy_ISR           ; $FFD0     ADC
        dc.w    Dummy_ISR           ; $FFD2     KEYBOARD
        dc.w    Dummy_ISR           ; $FFD4     SCI2TX
        dc.w    Dummy_ISR      	    ; $FFD6     SCI2RX
        dc.w    Dummy_ISR           ; $FFD8     SCI2ERR
        dc.w    Dummy_ISR           ; $FFDA     SCI1TX
        dc.w    Dummy_ISR           ; $FFDC     SCI1RX
        dc.w    Dummy_ISR           ; $FFDE     SCI1ERR
        dc.w    Dummy_ISR           ; $FFE0     SPI1
        dc.w    Dummy_ISR           ; $FFE2     TPM2OVF
        dc.w    Dummy_ISR           ; $FFE4     TPM2CH1
        dc.w    Dummy_ISR           ; $FFE6     TPM2CH0
        dc.w    Dummy_ISR           ; $FFE8     TPM1OVF
        dc.w    Dummy_ISR           ; $FFEA     TPM1CH5
        dc.w    Dummy_ISR           ; $FFEC     TPM1CH4
        dc.w    Dummy_ISR           ; $FFEE     TPM1CH3
        dc.w    Dummy_ISR           ; $FFF0     TPM1CH2
        dc.w    Dummy_ISR           ; $FFF2     TPM1CH1
        dc.w    Dummy_ISR           ; $FFF4     TPM1CH0
        dc.w    Dummy_ISR           ; $FFF6     LOL
        dc.w    Dummy_ISR           ; $FFF8     LVD
        dc.w    Dummy_ISR           ; $FFFA     IRQ
        dc.w    Dummy_ISR           ; $FFFC     SWI
      
        dc.w    Main_Init           ; $FFFE     RESET
