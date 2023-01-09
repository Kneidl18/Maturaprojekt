; *
; * CC2500 Sample transceiver code
; * This will send a packet containing the length and "Hello"
; * Every 400ms, then go into receive mode.
; * Adapted from https://github.com/yasiralijaved/Arduino-CC2500-Library
; * Written by Zohan SonZohan@gmail.com
; *
; * Hardware SPI:
; * MISO -> PB4
; * MOSI -> PB5
; * SCLK/SCK -> PB6
; * CSN/SS - > PB3
; *


CC2500_IDLE   		EQU			 $36      ;// Exit RX / TX, turn
CC2500_TX     		EQU			 $35      ;// Enable TX. If in RX state, only enable TX if CCA passes
CC2500_RX      		EQU			 $34      ;// Enable RX. Perform calibration if enabled
CC2500_FTX     		EQU			 $3B      ;// Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
CC2500_FRX     		EQU			 $3A      ;// Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
CC2500_SWOR    		EQU			 $38
CC2500_TXFIFO  		EQU			 $3F
CC2500_RXFIFO  		EQU			 $3F

TX_TIMEOUT 	   		EQU			 10 		;// Timeouts are added

SENT_INTERVAL		EQU			 40			;send every 400 milliseconds




CC2500_Init
	lda		#$50 				;0x50 is the id register. this is a try to read something from the BME
	sta		sendByteBme
	
	ldhx	#send_Byte
	sthx	CC2500Pointer
	
	rts
	

write_Reg
	rti
	lda		CC2500Addr + 1
	sta		SPI1D
	
	brclr	5, SPI1S, ERROR	

write_Reg_Val	
	lda		CC2500Val
	sta		SPI1D
	cli
	
	rts
	
	
ERROR
	jmp		write_Reg
	rts
	
	
;mainroutine, springt zur gespeicherten routine	
check_BME680
	ldhx 	CC2500Pointer
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
	sthx 	CC2500Pointer
	
	rts


read_SPI1D
	lda		SPI1D
	sta		SPIRead
	cli									;interrupts wieder erlauben
	
	ldhx	#send_Byte
	ldhx	CC2500Pointer
	
	rts