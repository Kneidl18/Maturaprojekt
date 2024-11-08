BME680_CHIPID = 0x61                     # < Hard-coded value 0x61 for BME680
BME680_RESET_CODE = 0xB6                 # < Reset when this put in reset reg
BME680_MEASURING_BIT_POSITION = 5        # < Bit position for measuring flag
BME680_I2C_MIN_ADDRESS = 0x76            # < Minimum possible address for BME680
BME680_I2C_MAX_ADDRESS = 0x77            # < Minimum possible address for BME680
BME680_SPI_MEM_PAGE_POSITION = 4         # < Bit position for memory page value
BME680_HUMIDITY_MASK = 0xF8              # < Mask is binary B11111000
BME680_PRESSURE_MASK = 0xE3              # < Mask is binary B11100011
BME680_TEMPERATURE_MASK = 0x1F           # < Mask is binary B00011111

BME680_COEFF_SIZE1 = 25                 # < First array with coefficients
BME680_COEFF_SIZE2 = 16                 # < Second array with coefficients
BME680_COEFF_START_ADDRESS1 = 0x89      # < start address for array 1
BME680_COEFF_START_ADDRESS2 = 0xE1      # < start address for array 2
BME680_HUM_REG_SHIFT_VAL = 4            # < Ambient humidity shift value
BME680_BIT_H1_DATA_MSK = 0x0F           # < Mask for humidity
BME680_T2_LSB_REG = 1                   # < Register for temperature calibration
BME680_T2_MSB_REG = 2                   # < Register for temperature calibration
BME680_T3_REG = 3                       # < Register for temperature calibration
BME680_P1_LSB_REG = 5                   # < Register for pressure calibration
BME680_P1_MSB_REG = 6                   # < Register for pressure calibration
BME680_P2_LSB_REG = 7                   # < Register for pressure calibration
BME680_P2_MSB_REG = 8                   # < Register for pressure calibration
BME680_P3_REG = 9                       # < Register for pressure calibration
BME680_P4_LSB_REG = 11                  # < Register for pressure calibration
BME680_P4_MSB_REG = 12                  # < Register for pressure calibration
BME680_P5_LSB_REG = 13                  # < Register for pressure calibration
BME680_P5_MSB_REG = 14                  # < Register for pressure calibration
BME680_P7_REG = 15                      # < Register for pressure calibration
BME680_P6_REG = 16                      # < Register for pressure calibration
BME680_P8_LSB_REG = 19                  # < Register for pressure calibration
BME680_P8_MSB_REG = 20                  # < Register for pressure calibration
BME680_P9_LSB_REG = 21                  # < Register for pressure calibration
BME680_P9_MSB_REG = 22                  # < Register for pressure calibration
BME680_P10_REG = 23                     # < Register for pressure calibration
BME680_H2_MSB_REG = 0                   # < Register for humidity calibration
BME680_H2_LSB_REG = 1                   # < Register for humidity calibration
BME680_H1_LSB_REG = 1                   # < Register for humidity calibration
BME680_H1_MSB_REG = 2                   # < Register for humidity calibration
BME680_H3_REG = 3                       # < Register for humidity calibration
BME680_H4_REG = 4                       # < Register for humidity calibration
BME680_H5_REG = 5                       # < Register for humidity calibration
BME680_H6_REG = 6                       # < Register for humidity calibration
BME680_H7_REG = 7                       # < Register for humidity calibration
BME680_T1_LSB_REG = 8                   # < Register for gas calibration
BME680_T1_MSB_REG = 9                   # < Register for gas calibration
BME680_GH2_LSB_REG = 10                 # < Register for gas calibration
BME680_GH2_MSB_REG = 11                 # < Register for gas calibration
BME680_GH1_REG = 12                     # < Register for gas calibration
BME680_GH3_REG = 13                     # < Register for gas calibration
BME680_ADDR_RES_HEAT_RANGE_ADDR = 0x02  # < Register for gas calibration
BME680_RHRANGE_MSK = 0x30               # < Register for gas calibration
BME680_ADDR_RES_HEAT_VAL_ADDR = 0x00    # < Register for gas calibration
BME680_ADDR_RANGE_SW_ERR_ADDR = 0x04    # < Register for gas calibration
BME680_RSERROR_MSK = 0xF0               # < Register for gas calibration