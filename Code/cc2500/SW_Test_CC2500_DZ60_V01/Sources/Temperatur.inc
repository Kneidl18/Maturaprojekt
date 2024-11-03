SCL_PIN				EQU			0				;Clock = PC0
SCL_PORT			EQU			PTCD
SCL_DIR				EQU			PTCDD

SDA_PIN				EQU			1				;Data = PC1
SDA_PORT			EQU			PTCD
SDA_DIR				EQU			PTCDD

SCL_PIN 			EQU			PTCD_PTCD0
SCL_PORT			EQU			PTCDD_PTCDD0

SDA_PIN				EQU			PTCD_PTCD1
SDA_PORT			EQU			PTCDD_PTCDD1

READ_ADRESS_AD7415	EQU			$00000091 		;war 0x91
WRITE_ADRESS_AD7415	EQU			$00000090		;war 0x90


;****************************************************************************
;*                                                                          *
;* Init_AD7415: Macht nicht viel, sendet nur eine Standard-Konfiguration    *
;* zum AD7415 (die eigentlich die Reset-Kondition sein sollte, aber sicher  *
;* ist sicher) und setzt dann den Adress-Pointer auf das Register "0x00",   *
;* dem Temperatur-Register, um dann später einfach nur die Temperaturwerte  *
;* auslesen zu können.                                                      *
;*                                                                          *
;****************************************************************************

Init_AD7415

			bclr	SDA_PIN, SDA_PORT
			bclr	0, SCL_PORT
			lda		#0
			sta		AirDelay
			
			jsr		IIC_Start
			
			lda		WRITE_ADRESS_AD7415
			sta		SendByte
			jsr		IIC_Send_Byte		;write to adress 1001 001 0 (write)
			
			;check ACK missing
			
			lda		#$00000001
			sta		SendByte
			jsr		IIC_Send_Byte		;confuguration register
			
			;check ACK missing
			
			lda		#$00000040			;0100 0000  Filter auf 1, der Rest ist 00, gibt in Hex 0x40
			sta		SendByte
			jsr		IIC_Send_Byte
			
			;ckech ACK missing
			
			jsr		IIC_Stop
			
			
			jsr		IIC_Start
			lda		WRITE_ADRESS_AD7415	;Einfach nur Adresse "0x00" --> Adresspoitner auf Temperatur-Register setzen
			
			;check ACK missing
			
			lda		#0					;Adress-Pointer auf 0x00
			sta		SendByte
			jsr		IIC_Send_Byte
			
			jsr		IIC_Stop
			
			rts
			
			
;****************************************************************************
;*                                                                          *
;* 	IIC_START: Startkondition, indem zuerst das Datenpin auf LOW geht,      *
;*  danach das CLOCKPIN.                                                    *
;*  Dadurch, dass der Pin als OUTPUT geschaltet wird und eine logische "0"  *
;*  ausgibt geht die Leitung auf LOW. Für ein HIGH wird die Leitung einfach *
;*  als INPUT geschaltet, somit wird sie hochohmig und der Pull-Up zieht    *
;*  die Leitung auf HIGH.                                                   *
;*                                                                          *
;****************************************************************************				

IIC_Start
			
			bset	0, SDA_PORT			;SDA-Pin wird als Ausgang definiert mit Pegel = LOW
			bclr	0, SDA_PIN
			
			nop							;Zuerst DATA auf LOW, danach Clock
			nop							;DAs ist die Startkondition!!
			
			bset	0, SCL_PORT
			bclr	0, SCL_PIN
			
			rts
			

;****************************************************************************
;*                                                                          *
;*  IIC_STOP: Stop-Kondition: Zuerst geht Clock auf HIGH, danach DATA       *
;*                                                                          *
;****************************************************************************
			
IIC_Stop

			bset	0, SDA_PORT			;SIcherheitshalber mal beide Ports als Ausgang schalten
			bset	0, SCL_PORT
			
			bclr	0, SCL_PORT			;zuerst geht der CLOCK auf HIGH indem er ein IMPUT wird...
			nop
			nop
			bclr	0, SDA_PORT			;und danach DATA! --> Das ist die STOP-Kondition
			
			rts
			

;****************************************************************************
;*                                                                          *
;*  IIC_Send_Byte: Sendet ein Byte hinaus                                   *
;*                                                                          *
;****************************************************************************

IIC_Send_Byte
			
			lda		SendByte
			sta		ShiftWert
			
			bset	0, SDA_PORT
			
			lda		#0
			sta		Counter
			
			
			