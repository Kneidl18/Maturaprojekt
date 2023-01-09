;*********************************************************************************************
;** Declare class-internal and not publically visible constants                            	**
;*********************************************************************************************
BME680_STATUS_REGISTER 				EQU		$1D	;///< Device status register
BME680_GAS_HEATER_REGISTER0			EQU		$5A	;///< Heater Register 0 address
BME680_GAS_DURATION_REGISTER0		EQU		$64	;///< Heater Register 0 address
BME680_CONTROL_GAS_REGISTER1		EQU		$70 ;///< Gas control register on/off
BME680_CONTROL_GAS_REGISTER2		EQU		$71 ;///< Gas control register settings
BME680_CONTROL_HUMIDITY_REGISTER	EQU		$72 ;///< Humidity control register
BME680_SPI_REGISTER					EQU		$73 ;///< Status register for SPI memory
BME680_CONTROL_MEASURE_REGISTER		EQU		$74 ;///< Temp, Pressure control register
BME680_CONFIG_REGISTER				EQU		$75 ;///< Configuration register
BME680_CHIPID_REGISTER				EQU		$50 ;///< Chip-Id register
BME680_SOFTRESET_REGISTER			EQU		$E0 ;///< Reset when $B6 is written here
BME680_CHIPID						EQU		$61 ;///< Hard-coded value $61 for BME680
BME680_RESET_CODE					EQU		$B6 ;///< Reset when this put in reset reg
BME680_MEASURING_BIT_POSITION		EQU		5   ;///< Bit position for measuring flag
BME680_I2C_MIN_ADDRESS				EQU		$76 ;///< Minimum possible address for BME680
BME680_I2C_MAX_ADDRESS				EQU		$77 ;///< Minimum possible address for BME680
BME680_SPI_MEM_PAGE_POSITION		EQU		4   ;///< Bit position for memory page value
BME680_HUMIDITY_MASK				EQU		$F8 ;///< Mask is binary B11111000
BME680_PRESSURE_MASK				EQU		$E3 ;///< Mask is binary B11100011
BME680_TEMPERATURE_MASK				EQU		$1F ;///< Mask is binary B00011111

;*********************************************************************************************
;** Declare the constants used for calibration                                     			**
;*********************************************************************************************
BME680_COEFF_SIZE1					EQU		25  ;///< First array with coefficients
BME680_COEFF_SIZE2					EQU		16  ;///< Second array with coefficients
BME680_COEFF_START_ADDRESS1			EQU		$89 ;///< start address for array 1
BME680_COEFF_START_ADDRESS2			EQU		$E1 ;///< start address for array 2
BME680_HUM_REG_SHIFT_VAL			EQU		4   ;///< Ambient humidity shift value
BME680_BIT_H1_DATA_MSK				EQU		$0F ;///< Mask for humidity
BME680_T2_LSB_REG					EQU		1   ;///< Register for temperature calibration
BME680_T2_MSB_REG					EQU		2   ;///< Register for temperature calibration
BME680_T3_REG						EQU		3   ;///< Register for temperature calibration
BME680_P1_LSB_REG					EQU		5   ;///< Register for pressure calibration
BME680_P1_MSB_REG					EQU		6   ;///< Register for pressure calibration
BME680_P2_LSB_REG					EQU		7   ;///< Register for pressure calibration
BME680_P2_MSB_REG					EQU		8   ;///< Register for pressure calibration
BME680_P3_REG						EQU		9   ;///< Register for pressure calibration
BME680_P4_LSB_REG					EQU		11  ;///< Register for pressure calibration
BME680_P4_MSB_REG					EQU		12  ;///< Register for pressure calibration
BME680_P5_LSB_REG					EQU		13  ;///< Register for pressure calibration
BME680_P5_MSB_REG					EQU		14  ;///< Register for pressure calibration
BME680_P7_REG						EQU		15  ;///< Register for pressure calibration
BME680_P6_REG						EQU		16  ;///< Register for pressure calibration
BME680_P8_LSB_REG					EQU		19  ;///< Register for pressure calibration
BME680_P8_MSB_REG					EQU		20  ;///< Register for pressure calibration
BME680_P9_LSB_REG					EQU		21  ;///< Register for pressure calibration
BME680_P9_MSB_REG					EQU		22  ;///< Register for pressure calibration
BME680_P10_REG						EQU		23  ;///< Register for pressure calibration
BME680_H2_MSB_REG					EQU		0   ;///< Register for humidity calibration
BME680_H2_LSB_REG					EQU		1   ;///< Register for humidity calibration
BME680_H1_LSB_REG					EQU		1   ;///< Register for humidity calibration
BME680_H1_MSB_REG					EQU		2   ;///< Register for humidity calibration
BME680_H3_REG						EQU		3   ;///< Register for humidity calibration
BME680_H4_REG						EQU		4   ;///< Register for humidity calibration
BME680_H5_REG						EQU		5   ;///< Register for humidity calibration
BME680_H6_REG						EQU		6   ;///< Register for humidity calibration
BME680_H7_REG						EQU		7   ;///< Register for humidity calibration
BME680_T1_LSB_REG					EQU		8   ;///< Register for gas calibration
BME680_T1_MSB_REG					EQU		9   ;///< Register for gas calibration
BME680_GH2_LSB_REG					EQU		10  ;///< Register for gas calibration
BME680_GH2_MSB_REG					EQU		11  ;///< Register for gas calibration
BME680_GH1_REG						EQU		12  ;///< Register for gas calibration
BME680_GH3_REG						EQU		13  ;///< Register for gas calibration
BME680_ADDR_RES_HEAT_RANGE_ADDR		EQU		$02 ;///< Register for gas calibration
BME680_RHRANGE_MSK					EQU		$30 ;///< Register for gas calibration
BME680_ADDR_RES_HEAT_VAL_ADDR		EQU		$00 ;///< Register for gas calibration
BME680_ADDR_RANGE_SW_ERR_ADDR		EQU		$04 ;///< Register for gas calibration
BME680_RSERROR_MSK					EQU		$F0 ;///< Register for gas calibration
