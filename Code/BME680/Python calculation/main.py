# ***************************************************************************************************
# ** Declare the constants used for calibration                                                    **
# ***************************************************************************************************
BME680_COEFF_SIZE1 = 25  # < First array with coefficients
BME680_COEFF_SIZE2 = 16  # < Second array with coefficients
BME680_COEFF_START_ADDRESS1 = 0x89  # < start address for array 1
BME680_COEFF_START_ADDRESS2 = 0xE1  # < start address for array 2
BME680_HUM_REG_SHIFT_VAL = 4  # < Ambient humidity shift value
BME680_BIT_H1_DATA_MSK = 0x0F  # < Mask for humidity
BME680_T2_LSB_REG = 1  # < Register for temperature calibration
BME680_T2_MSB_REG = 2  # < Register for temperature calibration
BME680_T3_REG = 3  # < Register for temperature calibration
BME680_P1_LSB_REG = 5  # < Register for pressure calibration
BME680_P1_MSB_REG = 6  # < Register for pressure calibration
BME680_P2_LSB_REG = 7  # < Register for pressure calibration
BME680_P2_MSB_REG = 8  # < Register for pressure calibration
BME680_P3_REG = 9  # < Register for pressure calibration
BME680_P4_LSB_REG = 11  # < Register for pressure calibration
BME680_P4_MSB_REG = 12  # < Register for pressure calibration
BME680_P5_LSB_REG = 13  # < Register for pressure calibration
BME680_P5_MSB_REG = 14  # < Register for pressure calibration
BME680_P7_REG = 15  # < Register for pressure calibration
BME680_P6_REG = 16  # < Register for pressure calibration
BME680_P8_LSB_REG = 19  # < Register for pressure calibration
BME680_P8_MSB_REG = 20  # < Register for pressure calibration
BME680_P9_LSB_REG = 21  # < Register for pressure calibration
BME680_P9_MSB_REG = 22  # < Register for pressure calibration
BME680_P10_REG = 23  # < Register for pressure calibration
BME680_H2_MSB_REG = 0  # < Register for humidity calibration
BME680_H2_LSB_REG = 1  # < Register for humidity calibration
BME680_H1_LSB_REG = 1  # < Register for humidity calibration
BME680_H1_MSB_REG = 2  # < Register for humidity calibration
BME680_H3_REG = 3  # < Register for humidity calibration
BME680_H4_REG = 4  # < Register for humidity calibration
BME680_H5_REG = 5  # < Register for humidity calibration
BME680_H6_REG = 6  # < Register for humidity calibration
BME680_H7_REG = 7  # < Register for humidity calibration
BME680_T1_LSB_REG = 8  # < Register for gas calibration
BME680_T1_MSB_REG = 9  # < Register for gas calibration
BME680_GH2_LSB_REG = 10  # < Register for gas calibration
BME680_GH2_MSB_REG = 11  # < Register for gas calibration
BME680_GH1_REG = 12  # < Register for gas calibration
BME680_GH3_REG = 13  # < Register for gas calibration
BME680_ADDR_RES_HEAT_RANGE_ADDR = 0x02  # < Register for gas calibration
BME680_RHRANGE_MSK = 0x30  # < Register for gas calibration
BME680_ADDR_RES_HEAT_VAL_ADDR = 0x00  # < Register for gas calibration
BME680_ADDR_RANGE_SW_ERR_ADDR = 0x04  # < Register for gas calibration
BME680_RSERROR_MSK = 0xF0  # < Register for gas calibration


def set_gas(gas_temp, gas_millis, H3, G1, G2, temperature, res_heat_range, res_heat):
    if gas_temp < 200:
        gas_temp = 200
    elif gas_temp > 400:
        gas_temp = 400

    var1 = int((((temperature / 100) * H3) / 1000)) << 8
    var2 = ((G1 + 784) * ((((G2 + 154009) * gas_temp * 5) / 100) + 3276800) / 10)
    var3 = var1 + (var2 / 2)
    var4 = (var3 / (res_heat_range + 4))
    var5 = (131 * res_heat) + 65536
    heatr_res_x_100 = (((var4 / var5) - 250) * 34)
    heatr_res = ((heatr_res_x_100 + 50) / 100)

    factor = 0
    if gas_millis >= 0xFC0:
        durval = 0xFF
    else:
        while gas_millis > 0x3F:
            gas_millis = gas_millis >> 2
            factor += 1
        durval = gas_millis + (factor * 64)

    return heatr_res, durval


def altitude(press, sea_level=1013.25):
    return 44330.0 * (1.0 - ((float(press / 100.0) / sea_level) ** 0.190))


def get_sensor_data(buff, coeff_arr, rng_sw_err):
    lookup_table1 = [2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2126008810, 2147483647, 2130303777,
                     2147483647, 2147483647, 2143188679, 2136746228, 2147483647, 2126008810, 2147483647, 2147483647]
    lookup_table2 = [4096000000, 2048000000, 1024000000, 512000000, 255744255, 127110228, 64000000, 32258064,
                     16016016, 8000000, 4000000, 2000000, 1000000, 500000, 250000, 125000]

    T1 = coeff_arr.pop()
    T2 = coeff_arr.pop()
    T3 = coeff_arr.pop()

    P1 = coeff_arr.pop()
    P2 = coeff_arr.pop()
    P3 = coeff_arr.pop()
    P4 = coeff_arr.pop()
    P5 = coeff_arr.pop()
    P6 = coeff_arr.pop()
    P7 = coeff_arr.pop()
    P8 = coeff_arr.pop()
    P9 = coeff_arr.pop()
    P10 = coeff_arr.pop()

    H1 = coeff_arr.pop()
    H2 = coeff_arr.pop()
    H3 = coeff_arr.pop()
    H4 = coeff_arr.pop()
    H5 = coeff_arr.pop()
    H6 = coeff_arr.pop()
    H7 = coeff_arr.pop()

    G1 = coeff_arr.pop()
    G2 = coeff_arr.pop()
    G3 = coeff_arr.pop()

    adc_pres = int(int(buff[2] << 12) | int(buff[3] << 4) | int(buff[4] >> 4))
    adc_temp = int(int(buff[5] << 12) | int(buff[6] << 4) | int(buff[7] >> 4))
    adc_hum = int(int(buff[8] << 8) | buff[9])
    adc_gas_res = int(int(buff[13] << 2) | int(buff[14] >> 6))
    gas_range = int(buff[14] & 0x0f)

    # computing the temperature
    var1 = (adc_temp >> 3) - (T1 << 1)
    var2 = (var1 * T2) >> 11
    var3 = ((var1 >> 1) * (var1 >> 1)) >> 12
    var3 = (var3 * (T3 << 4)) >> 14
    tFine = (var2 + var3)
    temperature = ((tFine * 5) + 128) >> 8

    # computing the pressure
    var1 = (tFine >> 1) - 64000
    var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * P6) >> 2
    var2 = var2 + ((var1 * P5) << 1)
    var2 = (var2 >> 2) + (P4 << 16)
    var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * P3) >> 3) + ((P2 * var1) >> 1)
    var1 = var1 >> 18
    var1 = ((32768 + var1) * P1) >> 15
    pressure = 1048576 - adc_pres
    pressure = ((pressure - (var2 >> 12)) * 3125)

    if pressure >= 0x40000000:
        pressure = (int(pressure / var1) << 1)
    else:
        pressure = ((pressure << 1) / var1)

    pressure = int(pressure)
    var1 = (P9 * (((pressure >> 3) * (pressure >> 3)) >> 13)) >> 12
    var2 = ((pressure >> 2) * P8) >> 13
    var3 = ((pressure >> 8) * (pressure >> 8) * (pressure >> 8) * P10) >> 17
    pressure = pressure + ((var1 + var2 + var3 + (P7 << 7)) >> 4)

    # computing the humidity
    temp_scaled = int(((tFine * 5) + 128) >> 8)
    var1 = (adc_hum - (H1 << 4)) - (int((temp_scaled * H3) / 100) >> 1)
    var2 = int(H2 * (int((temp_scaled * H4) / 100) + ((int(temp_scaled * ((temp_scaled * H5) / 100)) >> 6) / 100) + (
            1 << 14))) >> 10
    var3 = var1 * var2
    var4 = H6 << 7
    var4 = int(var4 + ((temp_scaled * H7) / 100)) >> 4
    var5 = ((var3 >> 14) * (var3 >> 14)) >> 10
    var6 = (var4 * var5) >> 1

    humidity = (((var3 + var6) >> 10) * 1000) >> 12

    if humidity > 100000:
        humidity = 100000
    elif humidity < 0:
        humidity = 0

    # computing the gas
    var1 = ((1340 + (5 * rng_sw_err)) * (lookup_table1[gas_range])) >> 16
    var2 = (((adc_gas_res << 15) - 16777216) + var1)
    var3 = ((lookup_table2[gas_range] * var1) >> 9)
    gas = ((var3 + (var2 >> 1)) / var2)

    return temperature, humidity, pressure, gas


def splitt_coeff_arr(coeff_arr1, coeff_arr2):
    temp_arr = []

    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
    # **        Temperature related coefficients            **
    # **        T1 - T3                                     **
    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
    temp_arr.append(int(coeff_arr2[BME680_T1_MSB_REG] + coeff_arr2[BME680_T1_LSB_REG]))
    temp_arr.append(int(coeff_arr1[BME680_T2_MSB_REG] + coeff_arr1[BME680_T2_LSB_REG]))
    temp_arr.append(int(coeff_arr1[BME680_T3_REG]))

    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** 
    # **        Pressure related coefficients               **
    # **        P1 - P10                                    **
    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
    temp_arr.append(int(coeff_arr1[BME680_P1_MSB_REG] + coeff_arr1[BME680_P1_LSB_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P2_MSB_REG] + coeff_arr1[BME680_P2_LSB_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P3_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P4_MSB_REG] + coeff_arr1[BME680_P4_LSB_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P5_MSB_REG] + coeff_arr1[BME680_P5_LSB_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P6_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P7_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P8_MSB_REG] + coeff_arr1[BME680_P8_LSB_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P9_MSB_REG] + coeff_arr1[BME680_P9_LSB_REG]))
    temp_arr.append(int(coeff_arr1[BME680_P10_REG]))

    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
    # ** Humidity related coefficients                **
    # ** H1 - H7                                      **
    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
    temp_arr.append(int(int(coeff_arr2[BME680_H1_MSB_REG] << BME680_HUM_REG_SHIFT_VAL) | (
            (coeff_arr2[BME680_H1_LSB_REG] >> BME680_HUM_REG_SHIFT_VAL) & BME680_BIT_H1_DATA_MSK)))
    temp_arr.append(int(int(coeff_arr2[BME680_H2_MSB_REG] << BME680_HUM_REG_SHIFT_VAL) | (
            (coeff_arr2[BME680_H2_LSB_REG] >> BME680_HUM_REG_SHIFT_VAL) & BME680_BIT_H1_DATA_MSK)))
    temp_arr.append(int(coeff_arr2[BME680_H3_REG]))
    temp_arr.append(int(coeff_arr2[BME680_H4_REG]))
    temp_arr.append(int(coeff_arr2[BME680_H5_REG]))
    temp_arr.append(int(coeff_arr2[BME680_H6_REG]))
    temp_arr.append(int(coeff_arr2[BME680_H7_REG]))

    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
    # ** Gas heater related coefficients                 **
    # ** G1 - G3                                         **
    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
    temp_arr.append(int(coeff_arr2[BME680_GH1_REG]))
    temp_arr.append(int(coeff_arr2[BME680_GH2_MSB_REG] + coeff_arr2[BME680_GH2_LSB_REG]))
    temp_arr.append(int(coeff_arr2[BME680_GH3_REG]))

    return temp_arr


def print_results(temperature, humidity, pressure, gas):
    print("temp: ", int(temperature / 100), ".", int(temperature % 100))

    print("humidity: ", int(humidity / 1000), ".", int(humidity % 1000))

    print("pressure: ", int(pressure / 100), ".", int(pressure % 100))

    alt = altitude(pressure)  # temp altitude
    print("alt: ", int(alt), ".", int((alt * 100) % 100))

    print("gas: ", int(gas / 100), ".", int(gas % 100))


coeff_arr1_test = [0, 171, 104, 3, 16, 41, 139, 57, 216, 88, 0, 60, 29, 168, 255, 18, 30, 0, 0, 253, 0, 56, 241, 30, 127]
coeff_arr2_test = [62, 112, 52, 0, 45, 20, 120, 156, 64, 102, 166, 212, 226, 18, 74, 0]
coeff_arr_split = splitt_coeff_arr(coeff_arr1_test, coeff_arr2_test)


_gas_millis = 150
_gas_temp = 320
_H3 = coeff_arr_split[-8]
_G1 = coeff_arr_split[-3]
_G2 = coeff_arr_split[-2]
_temperature = 1915
_res_heat_range = 0
_res_heat = 118

heatr_res, durval = set_gas(_gas_temp, _gas_millis, _H3, _G1, _G2, _temperature, _res_heat_range, _res_heat)
print(hex(int(heatr_res)), hex(int(durval)))


_rng_sw_err = 0
temp_buff = [0, 0, 108, 222, 144, 117, 140, 64, 87, 237, 128, 0, 0, 91, 248]
temperature, humidity, pressure, gas = get_sensor_data(temp_buff, coeff_arr_split, _rng_sw_err)

print_results(temperature, humidity, pressure, gas)


