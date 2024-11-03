;************************************************************************************************
;*                                                                                              *
;*      Initialisieren der internen SPI-Schnittstelle - sie l�uft auf folgenden Pins:         *
;*                                                                                              *
;*      PE2     /SS                                                                             *
;*      PB4     /MISO                                                                           *
;*      PB5     /MOSI                                                                           *
;*      PE3     /SPSCK                                                                          *
;*      Read = 1, Write =0                                                                      *
;*																								*
;*																								*
;************************************************************************************************

; Bits in Register SPI1S1
SPRF    EQU     7       ; SPI Read Buffer Full Flag   0:No data in Receiver Buffer   1:Data in Receiver Buffer available
SPTEF   EQU     5       ; SPI Transmit Empty Flag     0:Transmit Buffer not empty    1:Transmit Buffer empty (ready to send!)
MODF    EQU     4       ; Master Mode Fault Flag

; Bits in Register SPI1C1\
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


;@brief		initialising the SPI, registers are set, SPI pins are
;			set as input or output
SPI_Init
	;doing this in two steps becaseu CPHA can't be changed while SPE=0
	
    mov   #%01010100,SPI1C1   ; SPI Aktiviert als Master
;           ||||||||
;           ||||||||_____  Shifter direction -->        0:MSB shift out first!
;           |||||||______  SS Output enable             1:Pin under SPI control
;           ||||||_______  Clock Phase,                 0:Middle of Cycle
;           |||||________  Clock Polarity               0:Active High
;           ||||_________  MSTR,                        1:Master
;           |||__________  Transmit Interrupt enable    0:OFF
;           ||___________  SPI enable                   1:ON
;           |____________  Interrupt enable             0:OFFSPI_Init

    mov   #%01010000,SPI1C1   ; SPI Aktiviert als Master
;           ||||||||
;           ||||||||_____  Shifter direction -->        0:MSB shift out first!
;           |||||||______  SS Output enable             1:Pin under SPI control
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
 
 

    mov   #%00100000,SPI1BR     ; Stellt den SPI_Clock ein (Baudrate)
;           ||||||||
;           ||||||||_____  )
;           |||||||______  > Timer Scaler 2..256 (2)
;           ||||||_______  )
;           |||||________  nc
;           ||||_________  )
;           |||__________  > Timer Prescaler 1..8
;           ||___________  )
;           |____________  nc

		bset	2,PTEDD		; Pin PE2 --> OUTPUT (SS)
		bset	2,PTED
        bset    3,PTEDD         ; Pin PE3 --> OUTPUT (SPSCK1)
        bset    4,PTEDD         ; Pin PE4 --> OUTPUT (MOSI)
        bclr    5,PTEDD         ; Pin PE5 --> INPUT (MISO)
        
        lda		#1
        sta		PTEDS

		rts


SPI_Test
	lda				#BME680_CHIPID_REGISTER
	sta				SPI_Adress
	jsr				SPI_readReg
	lda				SPI_readByte
	sta				SPI_testByte1
	rts
		

;@brief writes a SINGLE byte to the adress
;param[in] 	BME680_Adress
;param[in]	BME680_sendByte
SPI_writeReg
	lda				SPI1S
	bclr			2, PTED
	
	lda				SPI_Adress
	jsr				SPI_waitByteSend
	sta				SPI1D
	
	lda				#0
	jsr				SPI_Delay
	
	lda				SPI1S
	lda				SPI_sendByte
	jsr				SPI_waitByteSend
	sta				SPI1D
	
	lda				#0
	jsr				SPI_Delay
	
	bset			2, PTED	
	rts
	
	
;@brief sends a SINGLE adress and gets back the byte that the BME680 returns
;@param[in] 	BME680_Adress		the byte that should be sent
;@param[out]	SPIRead			the byte that gets returned
SPI_readReg			
	sei				
	lda				SPI1S
	
	bclr			2, PTED
	
	lda				SPI_Adress
	add				#$80			;1000 0000 to make the register a read command
	jsr				SPI_waitByteSend
	sta				SPI1D
	
	lda				#0
	jsr				SPI_Delay
	
	lda				SPI1S
	jsr				SPI_waitByteSend
	sta				SPI1D
	
	lda				#0
	jsr				SPI_Delay
	
    bset			2, PTED
	
	lda				SPI1D
	sta				SPI_testByte1
	
	lda				SPI1D
	sta				SPI_testByte2
	lda				SPI1D
	sta				SPI_testByte3
	
	lda				SPI1S
	cli
	rts


;@brief 	wait for byte to send
;@detail	wait for the byte to send
SPI_waitByteSend
	brclr			SPTEF, SPI1S, *
	rts
	
;@brief ensures a delay so that the SPI finsihes sending/receiving data
;@param[in] 	accumulator (a)		stores the start value (must be 0)
SPI_Delay
	inca	
	nop
	cmpa		#30T
	bne 		SPI_Delay
	rts