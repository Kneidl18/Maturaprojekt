;************************************************************************************************
;*                                                                                              *
;*      Initialisieren der internen SPI-Schnittstelle - sie läuft auf folgenden Pins:           *
;*                                                                                              *
;*      PB3     /SS                                                                             *
;*      PB4     /MISO                                                                           *
;*      PB5     /MOSI                                                                           *
;*      PB6     /SPSCK                                                                          *
;*                                                                                              *
;*																								*
;*																								*
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


BME680_Init

	mov   #%01010010,SPI1C1   ; SPI Aktiviert als Master
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

        bset    6,PTBDD         ; Pin PE6 --> OUTPUT (SPSCK1)
        bset    5,PTBDD         ; Pin PE6 --> OUTPUT (MOSI)
        bclr    4,PTBDD         ; Pin PE5 --> INPUT (MISO)
	
	
;setting the different registers for the oversampling
;set 	spi_mem_page to 1 (register address range 0x00 to 0x7F)
;		spi_mem_page to 0 (register address range 0x80 to 0xFF) -> default
;1. osrs_h<2:0> 	register 0x72		example: 0b001
;2.	osrs_t<7:5> 	register 0x74		example: 0b010
;3. osrs_p<4:2>	 	register 0x74		example: 0b101
;4. gas_wait_0<7:0> register 0x64 		example: 0x59
;5. res_heat_0<7:0> register 0x54		example: 
;6. nb_conv<3:0>	register 0x71		example: 0x0
;7. run_gas_l<4>	register 0x71		example: 1
	
	
;end init. everything is set i hope


	lda		#$50 ;0x50 is the id register. this is a try to read something from the BME
	sta		sendByteBme
	
	ldhx	#send_Byte
	sthx	BMEPointer
	
	rts
	
;mainroutine, springt zur gespeicherten routine	
check_BME680
	ldhx 	BMEPointer
	jmp 	0,X
	
	rts

;sendet ein Byte ueber die Serielle
send_Byte
	sei 								;interrupts deaktivieren
	
	lda		sendByteBme
	sta		SPI1D

	
check_SPRF		
	brset	SPRF, SPI1S, read_SPI1D
	
	ldhx 	#check_SPRF	
	sthx 	BMEPointer
	
	rts


read_SPI1D
	lda		SPI1D
	sta		SPIRead
	cli									;interrupts wieder erlauben
	
	ldhx	#send_Byte
	ldhx	BMEPointer
	
	rts