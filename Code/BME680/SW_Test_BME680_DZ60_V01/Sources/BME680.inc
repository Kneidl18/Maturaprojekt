;@brief		ERROR state. if the BME680 ID isn't correct	
BME680_ChipIdError
	lda			#1
	sta			BME680_error
	rts
	
	
;read the spi reg, set mem_page to 0, check for the right ID (0x61), 
;put mem_page back to 1, read control register, trigger first measurement
BME680_commonInitialization
	;read the BME680_SPI_Register and clear the MEM_PAGE bit
	lda			#BME680_SPI_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	bclr			BME680_SPI_MEM_PAGE_POSITION, BME680_byte
	
	lda			BME680_byte
	sta			SPI_sendByte
	lda			#BME680_SPI_REGISTER
	sta			SPI_Adress
	jsr			SPI_writeReg
	
	;read the BME680 CHIP_ID and check if it's correct
	lda			#BME680_CHIPID_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	cmpa			#BME680_CHIPID
	bne			BME680_ChipIdError

;load Calibration values for arr1	
BME680_getCalibration_arr1
	lda			#BME680_COEFF_START_ADDRESS1
	add			BME680_coeffCounter
	sta			SPI_Adress
	jsr			SPI_readReg
	lda			SPI_readByte
	sta			BME680_coeff_arr1 + BME680_coeffCounter
	
	lda			BME680_coeffCounter
	inca
	sta			BME680_coeffCounter
	cmpa		#24T
	bne			BME680_getCalibration_arr1
	
	lda			#0
	sta			BME680_coeffCounter
	
;load Calibration values for arr2
BME680_getCalibration_arr2	
	lda			#BME680_COEFF_START_ADDRESS2
	add			BME680_coeffCounter
	sta			SPI_Adress
	jsr			SPI_readReg
	lda			SPI_readByte
	sta			BME680_coeff_arr2 + BME680_coeffCounter
	
	lda			BME680_coeffCounter
	inca
	sta			BME680_coeffCounter
	cmpa		#15T
	bne			BME680_getCalibration_arr2
	
	lda			#0
	sta			BME680_coeffCounter

BME680_storeCalibrationValues
;Temperature related coefficients
	lda			BME680_coeff_arr2 + BME680_T1_MSB_REG
	sta			BME680_T1
	lda			BME680_coeff_arr2 + BME680_T1_LSB_REG
	sta			BME680_T1 + 1
	
	lda			BME680_coeff_arr1 + BME680_T2_MSB_REG
	sta			BME680_T2
	lda			BME680_coeff_arr1 + BME680_T2_LSB_REG
	sta			BME680_T1 + 1
	
	lda			BME680_coeff_arr1 + BME680_T3_REG
	sta			BME680_T3

;Pressure related coefficients
	lda			BME680_coeff_arr1 + BME680_P1_MSB_REG
	sta			BME680_P1
	lda			BME680_coeff_arr1 + BME680_P1_LSB_REG
	sta			BME680_P1 + 1
	
	lda			BME680_coeff_arr1 + BME680_P2_MSB_REG
	sta			BME680_P2
	lda			BME680_coeff_arr1 + BME680_P2_LSB_REG
	sta			BME680_P2 + 1
	
	lda			BME680_coeff_arr1 + BME680_P3_REG
	sta			BME680_P3
	
	lda			BME680_coeff_arr1 + BME680_P4_MSB_REG
	sta			BME680_P4
	lda			BME680_coeff_arr1 + BME680_P4_LSB_REG
	sta			BME680_P4 + 1
	
	lda			BME680_coeff_arr1 + BME680_P5_MSB_REG
	sta			BME680_P5
	lda			BME680_coeff_arr1 + BME680_P5_LSB_REG
	sta			BME680_P5 + 1
	
	lda			BME680_coeff_arr1 + BME680_P6_REG
	sta			BME680_P6
	
	lda			BME680_coeff_arr1 + BME680_P7_REG
	sta			BME680_P7
	
	lda			BME680_coeff_arr1 + BME680_P8_MSB_REG
	sta			BME680_P8
	lda			BME680_coeff_arr1 + BME680_P8_LSB_REG
	sta			BME680_P8 + 1
	
	lda			BME680_coeff_arr1 + BME680_P9_MSB_REG
	sta			BME680_P9
	lda			BME680_coeff_arr1 + BME680_P9_LSB_REG
	sta			BME680_P9 + 1
	
	lda			BME680_coeff_arr1 + BME680_P10_REG
	sta			BME680_P10
	
;humidity related coefficitents
	ldx			BME680_coeff_arr2 + BME680_H1_MSB_REG
	lda			BME680_HUM_REG_SHIFT_VAL
	jsr			BME680_leftShiftX
	stx			BME680_H1
	
	ldx			BME680_coeff_arr2 + BME680_H1_LSB_REG
	lda			BME680_HUM_REG_SHIFT_VAL
	jsr			BME680_rightShiftX
	stx			BME680_H1 + 1
	
	ldx			BME680_coeff_arr2 + BME680_H2_MSB_REG
	lda			BME680_HUM_REG_SHIFT_VAL
	jsr			BME680_leftShiftX
	stx			BME680_H2
	
	ldx			BME680_coeff_arr2 + BME680_H2_LSB_REG
	lda			BME680_HUM_REG_SHIFT_VAL
	jsr			BME680_rightShiftX
	stx			BME680_H2 + 1
	
	lda			BME680_coeff_arr1 + BME680_H3_REG
	sta			BME680_H3
	
	lda			BME680_coeff_arr1 + BME680_H4_REG
	sta			BME680_H4
	
	lda			BME680_coeff_arr1 + BME680_H5_REG
	sta			BME680_H5
	
	lda			BME680_coeff_arr1 + BME680_H6_REG
	sta			BME680_H6
	
	lda			BME680_coeff_arr1 + BME680_H7_REG
	sta			BME680_H7

;Gas heater related coefficients
	lda			BME680_coeff_arr2 + BME680_GH1_REG
	sta			BME680_G1
	
	lda			BME680_coeff_arr2 + BME680_GH2_MSB_REG
	sta			BME680_G2
	lda			BME680_coeff_arr2 + BME680_GH2_LSB_REG
	sta			BME680_G2 + 1
	
	lda			BME680_coeff_arr2 + BME680_GH3_REG
	sta			BME680_G3
	
	lda			BME680_ADDR_RES_HEAT_RANGE_ADDR
	sta			SPI_Adress
	jsr			SPI_readReg
	lda			SPI_readByte
	and			BME680_RHRANGE_MSK
	;division by 16 missing (uint8_t)BME680_byte by 16
	;now solved by right shifting for 4 times (2^4 = 16 => division by 16)
	lsra
	lsra
	lsra
	lsra
	sta			BME680_resHeatRange
	
	lda			BME680_ADDR_RES_HEAT_VAL_ADDR
	sta			SPI_Adress
	jsr			SPI_readReg
	lda			SPI_readByte
	sta			BME680_resHeat
	
	lda			BME680_ADDR_RANGE_SW_ERR_ADDR
	sta			SPI_Adress
	jsr			SPI_readReg
	lda			SPI_readByte
	and			BME680_RSERROR_MSK
	;division by 16 missing (uint8_t)BME680_byte by 16
	;now solved by right shifting for 4 times (2^4 = 16 => division by 16)
	lsra
	lsra
	lsra
	lsra
	sta			BME680_rngSwErr
	
	;end of getCalibration


BME680_rightShiftX
	lsrx
	deca
	cmpa		#0
	bgt			BME680_rightShiftX				
	
BME680_leftShiftX
	lslx
	deca
	cmpa		#0
	bgt			BME680_leftShiftX

BME680_commonInitializationContinue
	lda			#BME680_SPI_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	bset		BME680_SPI_MEM_PAGE_POSITION, BME680_byte
	
	lda			BME680_byte
	sta			SPI_sendByte
	lda			#BME680_SPI_REGISTER
	sta			SPI_Adress
	jsr			SPI_writeReg
	
	lda			#BME680_CONTROL_MEASURE_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	bset		0, BME680_byte
	
	lda			BME680_byte
	sta			SPI_sendByte
	lda			#BME680_CONTROL_MEASURE_REGISTER
	sta			SPI_Adress
	jsr			SPI_writeReg
		
;set oversampling16 for temperature, humidity and pressure
BME680_setOversampling
	;set the temperature oversampling to 16
	;first wait for the BME680 to finish measuring
	jsr			BME680_Measuring
	lda			BME680_measuringFlag
	cmpa			#0
	bne			BME680_setOversampling
	
	lda			#BME680_CONTROL_MEASURE_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	
	bset		5, BME680_byte				;)
	bclr		6, BME680_byte				;> 101 is the oversampling 16 code
	bset		7, BME680_byte				;)
	
	lda			BME680_byte
	sta			SPI_sendByte
	lda			#BME680_CONTROL_MEASURE_REGISTER
	sta			SPI_Adress
	jsr			SPI_writeReg
	
	;set the humidity oversampling to 16
	lda			#BME680_CONTROL_HUMIDITY_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	
	bset		0, BME680_byte				;)
	bclr		1, BME680_byte				;> 101 is the oversampling 16 code
	bset		2, BME680_byte				;)	
	
	lda			BME680_byte
	sta			SPI_sendByte
	lda			#BME680_CONTROL_HUMIDITY_REGISTER
	sta			SPI_Adress
	jsr			SPI_writeReg
	
	;set the pressure oversampling to 16
	lda			#BME680_CONTROL_MEASURE_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	
	bset		2, BME680_byte				;)
	bclr		3, BME680_byte				;> 101 is the oversampling 16 code
	bset		4, BME680_byte				;)
	
	lda			BME680_byte
	sta			SPI_sendByte
	lda			#BME680_CONTROL_MEASURE_REGISTER
	sta			SPI_Adress
	jsr			SPI_writeReg

;set iir16 filter
BME680_setIIRFilter
	lda			#BME680_CONFIG_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	
	bset		2, BME680_byte				;)
	bset		3, BME680_byte				;> 101 is the oversampling 16 code
	bset		4, BME680_byte				;)
	
	lda			#BME680_CONFIG_REGISTER
	sta			SPI_Adress
	lda			BME680_byte
	sta			SPI_sendByte
	jsr			SPI_writeReg

;set gasSeonsor temperature to 320 degree C for 150 ms (320, 150)
BME680_setGas
	jsr			BME680_Measuring
	lda			BME680_measuringFlag
	cmpa		#0
	bne			BME680_Measuring
	
	;Turn off heater bit to turn on
	lda			#BME680_CONTROL_GAS_REGISTER1
	sta			SPI_Adress
	lda			#0
	sta			SPI_sendByte
	jsr			SPI_writeReg
	
	
	
	rts


;@brief		checks if the BME680 is currently measuring
;param[out]	BME680_measuringFlag		Flag is set if currently measuring
BME680_Measuring
	lda			BME680_measuringFaultCounter
	inca		
	sta			BME680_measuringFaultCounter
	cmpa			#100T
	beq			BME680_MeasuringError
	
	lda			#0
	sta			BME680_measuringFlag
	
	lda			#BME680_STATUS_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	
	lda			SPI_readByte
	sta			BME680_byte
	
	brset			BME680_MEASURING_BIT_POSITION, BME680_byte, BME680_stillMeasuring
	
	lda			#0
	sta			BME680_measuringFaultCounter
	rts
	
BME680_stillMeasuring				
	lda			#1
	sta			BME680_measuringFlag
	
	rts

BME680_MeasuringError
	lda			#1
	sta			BME680_measuringErrorFlag
	rts


;@brief		triggers a measurement
BME680_triggerMesurement
	lda			#BME680_CONTROL_MEASURE_REGISTER
	sta			SPI_Adress
	jsr			SPI_readReg
	lda			SPI_readByte
	ora			#1						;TODO check if this is correct
	sta			SPI_sendByte
	jsr			SPI_writeReg
	
	rts	
	

BME680_Init
	lda			#0
	sta			BME680_coeffCounter
