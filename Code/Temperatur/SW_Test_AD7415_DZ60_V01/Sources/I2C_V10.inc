SDA_PIN						EQU			3							;Pin on which the DATA-Line is based on
SDA_PORT					EQU			PTFD						;Port on which the DATA-Line is based on
SDA_DIR						EQU			PTFDD						;Dir on which the DATA-Line is based on

SCL_PIN						EQU			2							;Pin on which the CLOCK-Line is based on
SCL_PORT					EQU			PTFD						;Port on which the CLOCK-Line is based on
SCL_DIR						EQU			PTFDD						;Dir on which the CLOCK-Line is based on


;************************************************************************************************
;*                                                                                              *
;*      Variablen für die I2C-Schnittstelle  - In Variablen.inc einfügen!                       *
;*                                                                                              *
;*      BitCounter_I2C          ds      1T                                                      *
;*      I2C_Byte                ds      1T                                                      *
;*                                                                                              *
;*      In der Init.inc folgendes aufrufen, um die I2C zu initialisieren:                       *
;*                                                                                              *
;*      jsr     Init_I2C                                                                        *
;*                                                                                              *
;************************************************************************************************

;************************************************************************************************
;*                                                                                              *
;*      Init_I2C: Setzt im Prinzip nur die Data-Directions der beiden Leitungen                 *
;*      "Data" und "Clock" auf INPUT und bereitet die Ausgänge auf LOW vor;                     *
;*                                                                                              *
;*      Zur Funktion: wenn auf einer der beiden Leitungen (DATA oder CLOCK) ein HIGH gesendet   *
;*      werden soll, dann sind die Pins als INPUT konfiguriert (und daher durch den Pull-Up     *
;*      auf HIGH gezogen).                                                                      *
;*      Soll hingegen ein LOW erzeugt werden, dann wird das dementsprechende Pin als OUTPUT     *
;*      geschaltet und es wird der LOW-Pegel hinausgesendet.                                    *
;*                                                                                              *
;************************************************************************************************

Init_I2C
			bset		SDA_PIN, SDA_DIR
			bset		SCL_PIN, SCL_DIR
			
			bclr		SDA_PIN, SDA_PORT
			bclr		SCL_PIN, SCL_PORT
			
			bclr		SDA_PIN, SDA_DIR
			bclr		SCL_PIN, SCL_DIR
			
			rts


;************************************************************************************************
;*                                                                                              *
;*      I2C_Start: is generating the START condition: CLOCK -> HIGH, DATA -> LOW, CLOCK -> LOW  *
;*                                                                                              *
;************************************************************************************************

I2C_Start
			bclr		SDA_PIN, SDA_DIR
			bclr		SCL_PIN, SCL_DIR
			nop
			
			bset		SDA_PIN, SDA_DIR
			nop
			bset		SCL_PIN, SCL_DIR
			
			rts
			
			
;************************************************************************************************
;*                                                                                              *
;*      I2C_Stop: is generating the STOP condition: CLOCK -> HIGH, DATA -> HIGH					*
;*                                                                                              *
;************************************************************************************************

I2C_Stop
			bset		SDA_PIN, SDA_DIR
			bset		SCL_PIN, SCL_DIR
			nop
			
			bclr		SCL_PIN, SCL_DIR
			nop
			bclr		SDA_PIN, SDA_DIR
			
			rts
		
			
;************************************************************************************************
;*                                                                                              *
;*      I2C_Send_Byte: is sending a Byte using the I2C interface. Bit by bit is getting send	*
;* 		with a clock signal between every bit. First bit is bit 7 and laast one is bit 0.		*
;*		Every bit is getting transferred using the DATA-Line and a CLOCK signal.				*
;*                                                                                              *
;************************************************************************************************

I2C_Send_Byte
			lda			#0
			sta			BitCounter_I2C
			

Send_I2C_Again
			brset		7, I2C_Byte, Send_I2C_High
			
			
Send_I2C_Low
			bset		SDA_PIN, SDA_DIR
			bra			Continue_Send_I2C
			

Send_I2C_High
			bclr		SDA_PIN, SDA_DIR
			

Continue_Send_I2C
			bclr		SCL_PIN, SCL_DIR
			nop
			bset		SCL_PIN, SCL_DIR
			
			lsl			I2C_Byte
			
			inc			BitCounter_I2C
			
			lda			BitCounter_I2C
			cmp			#8T
			bne			Send_I2C_Again
			
			bclr		SDA_PIN, SDA_DIR
			
			rts
			

;************************************************************************************************
;*                                                                                              *
;*      I2C_Get_Ack: is waiting for the Ack but not doing anything with it. possible would be	*
;*		a check of the Ack by adding that part of code.											*
;*                                                                                              *
;************************************************************************************************
		
I2C_Get_Ack
			bclr		SCL_PIN, SCL_DIR
			nop
			
			bset		SCL_PIN, SCL_DIR
			
			rts
			
			
;************************************************************************************************
;*                                                                                              *
;*      I2C_Read_Byte: is reading the incoming Byte by listening on the DATA-Line. With every	*
;*		CLOCK-Signal given by the Master (our Controller) the next bit will be on the DATA-Line.*
;*                                                                                              *
;************************************************************************************************

I2C_Read_Byte
			lda			#0
			sta			BitCounter_I2C
			sta			I2C_Byte
			
I2C_Read_Again
			bclr		SCL_PIN, SCL_DIR
			nop
			
			brset		SDA_PIN, SDA_PORT, Read_I2C_High
			
			
Read_I2C_Low
			lsl			I2C_Byte
			bra			Continue_Read_I2C
					

Read_I2C_High
			lda			I2C_Byte
			lsla		
			inca
			sta			I2C_Byte
			

Continue_Read_I2C
			bset		SCL_PIN, SCL_DIR
			
			inc			BitCounter_I2C
			lda			BitCounter_I2C
			cmp			#8T
			bne			I2C_Read_Again
			
			bclr		SDA_PIN, SDA_DIR
			
			rts
			
			
;************************************************************************************************
;*                                                                                              *
;*      I2C_Put_Ack: When reading the Ack has to be set by the master.							*
;*		DATA -> LOW, CLOCK: LOW -> HIGH -> LOW, DATA -> LOW										*
;*                                                                                              *
;************************************************************************************************
		
I2C_Put_Ack
			bset		SDA_PIN, SDA_DIR
			
			bclr		SCL_PIN, SCL_DIR
			bset		SCL_PIN, SCL_DIR
			
			bclr		SDA_PIN, SDA_DIR
			
			rts
			

;************************************************************************************************
;*                                                                                              *
;*      I2C_Put_NAck: as the end command for receiving everything after ending reading a high	*
;*		signal as Ack called NAck has to be send.												*
;*		DATA -> HIGH, CLOCK: LOW -> HIGH -> LOW, DATA -> HIGH									*
;*                                                                                              *
;************************************************************************************************

I2C_Put_NAck
			bclr		SDA_PIN, SDA_DIR
			
			bclr		SCL_PIN, SCL_DIR
			bset		SCL_PIN, SCL_DIR
			
			bclr		SDA_PIN, SDA_DIR
			
			rts