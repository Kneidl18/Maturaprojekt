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




SPI_Init

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
	
	rts
	
;end init. everything is set i hope