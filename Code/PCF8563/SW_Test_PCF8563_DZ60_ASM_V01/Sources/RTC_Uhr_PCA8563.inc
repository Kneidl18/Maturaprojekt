PCF_ADRESSE_W		EQU	$A2
PCF_ADRESSE_R		EQU	$A3



Read_PCF8563

	lda	Delay_Counter
	add	#1T
	sta	Delay_Counter
	cmpa	#10T
	bne	Exit_Read_PCF8563
	lda	#0T
	sta	Delay_Counter

	;--------------- Dieser Teil wird einmal pro Sekunde ausgeführt

	jsr	I2C_Start
	lda	#PCF_ADRESSE_W
	sta	I2C_Byte
	jsr	I2C_Send_Byte
	jsr	I2C_Get_Ack
	lda	#02T
	sta	I2C_Byte
	jsr	I2C_Send_Byte
	jsr	I2C_Get_Ack
	jsr	I2C_Stop
	
	jsr	I2C_Start
	lda	#PCF_ADRESSE_R
	sta	I2C_Byte
	jsr	I2C_Send_Byte
	jsr	I2C_Get_Ack

	jsr	I2C_Read_Byte
	lda	I2C_Byte
	sta	Sekunden
	jsr	I2C_Put_Ack

	jsr	I2C_Read_Byte
	lda	I2C_Byte
	sta	Minuten
	jsr	I2C_Put_Ack

	jsr	I2C_Read_Byte
	lda	I2C_Byte
	sta	Stunden
	jsr	I2C_Put_Ack

	jsr	I2C_Read_Byte
	lda	I2C_Byte
	sta	Tag
	jsr	I2C_Put_Ack

	jsr	I2C_Read_Byte
	lda	I2C_Byte
	sta	Wochentag
	jsr	I2C_Put_NAck
	jsr	I2C_Stop
Exit_Read_PCF8563	
	rts
	
	
Set_StartTime_PCF8563

	jsr	I2C_Start
	lda	#PCF_ADRESSE_W
	sta	I2C_Byte
	jsr	I2C_Send_Byte
	jsr	I2C_Get_Ack
	lda	#02T
	sta	I2C_Byte
	jsr	I2C_Send_Byte
	jsr	I2C_Get_Ack

	;---------- Sekunden setzen -----------

	lda	#00T
	sta	I2C_Byte
	jsr	I2C_Send_Byte
	jsr	I2C_Get_Ack


	;---------- Minuten setzen -----------
	
	lda	#00T
	sta	I2C_Byte
	jsr	I2C_Send_Byte
	jsr	I2C_Get_Ack
	

	;---------- Stunden setzen -----------
	lda	#00T
	sta	I2C_Byte
	jsr	I2C_Send_Byte
	jsr	I2C_Get_Ack
	

	
	jsr	I2C_Stop
	rts
	
	
	