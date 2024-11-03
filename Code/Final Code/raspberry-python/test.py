import time
import numpy as np
import bme680_VAL as val_BME680

packet_lengths = [3, 4, 5, 6, 8, 17, 46]

status = {0b0000: {"time": time.time(), 0b0101: [0, 0, 0], 0b0110: 0},
          0b0001: {"time": time.time(), 0b0010: [0, 0], 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
          0b0010: {"time": time.time(), 0b0001: 0, 0b0010: 5, 0b0100: 0},
          0b0011: {"time": time.time(), 0b0001: 0, 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
          0b0100: [time.time(), 0], 0b0101: [time.time(), 0], 0b0110: [time.time(), 0],
          0b0111: [time.time(), 0], 0b1000: [time.time(), 0], 0b1001: [time.time(), 0], 0b1010: [time.time(), 0],
          0b1011: [time.time(), 0], 0b1100: [time.time(), 0], 0b1101: [time.time(), 0], 0b1110: [time.time(), 0],
          0b1111: [time.time(), 0]}

test_dict = {}
for i in range(1, 16):
    test_dict[i] = 0

TX_TIMEOUT = 10  # Timeouts are added

previous_tx_timeout_millis = 0
previous_millis = 0
send_interval = 1000


class BME680:
    _H6, _P10, res_heat_range, _H3, _H4, _H5, _H7, _G1 = 0, 0, 0, 0, 0, 0, 0, 0
    _G3, _T3, _P3, _P6, _P7, res_heat_val_BME680, range_sw_err = 0, 0, 0, 0, 0, 0, 0
    _H1, _H2, _T1, _P1, _G2, _T2, _P2, _P4, _P5, _P8, _P9 = 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

    def set_calibration_values(self, buff):
        coeff_arr1, coeff_arr2 = buff[0:25], buff[25:41]
        self._T1 = (coeff_arr2[val_BME680.BME680_T1_MSB_REG] << 8) + coeff_arr2[val_BME680.BME680_T1_LSB_REG]
        self._T2 = (coeff_arr1[val_BME680.BME680_T2_MSB_REG] << 8) + coeff_arr1[val_BME680.BME680_T2_LSB_REG]
        self._T3 = coeff_arr1[val_BME680.BME680_T3_REG]

        self._P1 = (coeff_arr1[val_BME680.BME680_P1_MSB_REG] << 8) + coeff_arr1[val_BME680.BME680_P1_LSB_REG]
        self._P2 = (coeff_arr1[val_BME680.BME680_P2_MSB_REG] << 8) + coeff_arr1[val_BME680.BME680_P2_LSB_REG]
        self._P3 = coeff_arr1[val_BME680.BME680_P3_REG]
        self._P4 = (coeff_arr1[val_BME680.BME680_P4_MSB_REG] << 8) + coeff_arr1[val_BME680.BME680_P4_LSB_REG]
        self._P5 = (coeff_arr1[val_BME680.BME680_P5_MSB_REG] << 8) + coeff_arr1[val_BME680.BME680_P5_LSB_REG]
        self._P6 = (coeff_arr1[val_BME680.BME680_P6_REG])
        self._P7 = (coeff_arr1[val_BME680.BME680_P7_REG])
        self._P8 = (coeff_arr1[val_BME680.BME680_P8_MSB_REG] << 8) + coeff_arr1[val_BME680.BME680_P8_LSB_REG]
        self._P9 = (coeff_arr1[val_BME680.BME680_P9_MSB_REG] << 8) + coeff_arr1[val_BME680.BME680_P9_LSB_REG]
        self._P10 = (coeff_arr1[val_BME680.BME680_P10_REG])

        self._H1 = (coeff_arr2[val_BME680.BME680_H1_MSB_REG] << val_BME680.BME680_HUM_REG_SHIFT_VAL) | (
                (coeff_arr2[
                     val_BME680.BME680_H1_LSB_REG] >> val_BME680.BME680_HUM_REG_SHIFT_VAL) & val_BME680.BME680_BIT_H1_DATA_MSK)
        self._H2 = (coeff_arr2[val_BME680.BME680_H2_MSB_REG] << val_BME680.BME680_HUM_REG_SHIFT_VAL) | (
                (coeff_arr2[
                     val_BME680.BME680_H2_LSB_REG] >> val_BME680.BME680_HUM_REG_SHIFT_VAL) & val_BME680.BME680_BIT_H1_DATA_MSK)
        self._H3 = coeff_arr2[val_BME680.BME680_H3_REG]
        self._H4 = coeff_arr2[val_BME680.BME680_H4_REG]
        self._H5 = coeff_arr2[val_BME680.BME680_H5_REG]
        self._H6 = coeff_arr2[val_BME680.BME680_H6_REG]
        self._H7 = coeff_arr2[val_BME680.BME680_H7_REG]

        self._G1 = coeff_arr2[val_BME680.BME680_GH1_REG]
        self._G2 = (coeff_arr2[val_BME680.BME680_GH2_MSB_REG] << 8) + coeff_arr2[val_BME680.BME680_GH2_LSB_REG]
        self._G3 = coeff_arr2[val_BME680.BME680_GH3_REG]

        self.res_sw_err = buff[41]
        self._res_heat_range = buff[42]
        self.res_heat_val_BME680 = buff[43]

    # target_temp = temp we want to reach, gas_millis = for how long, amb_temp = temperature in the room
    def calculate_heat_gas_bme680(self, target_temp=320, gas_millis=150, amb_temp=15):
        var1 = (self._G1 / 16.0) + 49.0
        var2 = ((self._G2 / 32768.0) * 0.0005) + 0.00235
        var3 = self._G3 / 1024.0
        var4 = var1 * (1.0 + (var2 * target_temp))
        var5 = var4 + (var3 * amb_temp)
        res_heat_x_ret = (3.4 * ((var5 * (4.0 / (4.0 + self.res_heat_range)) * (
                1.0 / (1.0 + (self.res_heat_val_BME680 * 0.002)))) - 25))

        factor = 0
        while gas_millis > 0x3F:
            gas_millis = gas_millis >> 2
            factor += 1
        dur_val_bme680 = gas_millis + (factor * 64)

        return res_heat_x_ret, dur_val_bme680

    def calculate_values_bme680(self, buff):
        press_adc = ((buff[2] << 12) | (buff[3] << 4) | (buff[4] >> 4))
        temp_adc = ((buff[5] << 12) | (buff[6] << 4) | (buff[7] >> 4))
        hum_adc = ((buff[8] << 8) | buff[9])
        gas_adc = (buff[13] << 2) | (buff[14] >> 6)
        gas_range = buff[14] & 0x0F

        lookupTable1 = [2147483647, 2147483647, 2147483647, 2147483647,
                        2147483647, 2126008810, 2147483647, 2130303777,
                        2147483647, 2147483647, 2143188679, 2136746228,
                        2147483647, 2126008810, 2147483647, 2147483647]

        lookupTable2 = [4096000000, 2048000000, 1024000000, 512000000,
                        255744255, 127110228, 64000000, 32258064,
                        16016016, 8000000, 4000000, 2000000,
                        1000000, 500000, 250000, 125000]

        # temp
        var1 = ((temp_adc / 16384) - (self._T1 / 1024.0)) * self._T2
        var2 = (((temp_adc / 131072.0) - (self._T1 / 8192.0)) * ((temp_adc / 131072.0) - (self._T1 / 8192.0))) * (
                self._T3 * 16)
        t_fine = var1 + var2
        temp_comp = t_fine / 5120.0

        # pressure
        var1 = (int(temp_adc) >> 3) - (int(self._T1) << 1)
        var2 = (var1 * int(self._T2)) >> 11
        var3 = ((((var1 >> 1) * (var1 >> 1)) >> 12) * (int(self._T3) << 4)) >> 14
        t_fine = var2 + var3
        var1 = (int(t_fine) >> 1) - 64000
        var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * int(self._P6)) >> 2
        var2 = var2 + ((var1 * int(self._P5)) << 1)
        var2 = (var2 >> 2) + (int(self._P4) << 16)
        var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * (int(self._P3) << 5)) >> 3) + ((int(self._P2) * var1) >> 1)
        var1 = var1 >> 18
        var1 = ((32768 + var1) * int(self._P1)) >> 15
        press_comp = 1048576 - press_adc
        press_comp = int(abs((press_comp - (var2 >> 12)) * abs(int(3125))))
        if press_comp >= (1 << 30):
            press_comp = (int(press_comp / int(var1)) << 1)
        else:
            press_comp = ((press_comp << 1) / int(var1))

        var1 = (int(self._P9) * int(((press_comp >> 3) * (press_comp >> 3)) >> 13)) >> 12
        var2 = (int(press_comp >> 2) * int(self._P8)) >> 13
        var3 = (int(press_comp >> 8) * int(press_comp >> 8) * int(press_comp >> 8) * int(self._P10)) >> 17
        press_comp = int(press_comp) + ((var1 + var2 + var3 + (int(self._P7) << 7)) >> 4)

        # humidity
        var1 = hum_adc - ((self._H1 * 16.0) + ((self._H3 / 2.0) * temp_comp))
        var2 = var1 * ((self._H2 / 262144.0) * (
                1.0 + ((self._H4 / 16384.0) * temp_comp) + ((self._H5 / 1048576.0) * temp_comp * temp_comp)))
        var3 = self._H6 / 16384.0
        var4 = self._H7 / 2097152.0
        hum_comp = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2)

        # gas
        var1 = ((1340 + (5 * self.range_sw_err)) * (lookupTable1[gas_range])) >> 16
        uvar2 = ((gas_adc << 15) - 16777216) + var1
        var3 = (int(lookupTable2[gas_range] * var1) >> 9)
        gas_res = (var3 + (uvar2 >> 1)) / uvar2

        return temp_comp, press_comp, hum_comp, gas_res

    @staticmethod
    def calculate_altitude_bme680(press):
        altitude = 44330.0 * (1.0 - ((press / 101325) ** 0.1903))

        return altitude


def listen_for_packet():
    return [3, 0b00000101, 0, 10, 0, 10, 0, 10]


def safe_status():
    file = open(r'FSS_DBMS.txt', 'a')
    file.write(str(status) + '\n')
    print(str(status))
    file.close()


def checkCRC(byte_arr):
    byte_arr[0] -= 1
    lenght = byte_arr[0]

    crc1 = 0
    for i in range(lenght):
        crc1 += byte_arr[i]

    crc2 = 0
    for i in range(lenght):
        crc2 ^= byte_arr[i]

    crc = crc1 ^ crc2

    return crc == byte_arr[lenght]


if __name__ == '__main__':
    BME680_1 = BME680()
    BME680_1.set_calibration_values(
        [0, 171, 104, 3, 16, 41, 139, 57, 216, 88, 0, 60, 29, 168, 255, 18, 30, 0, 0, 253, 0, 56, 241, 30, 127, 62, 112,
         52, 0, 45, 20, 120, 156, 64, 102, 166, 212, 226, 18, 74, 0, 0, 118, 0])
    # temp, press, hum, gas = BME680_1.calculate_values_bme680(
    # [0, 0, 108, 158, 224, 116, 112, 112, 95, 33, 128, 0, 0, 205, 55])

    BME680_2 = BME680()
    BME680_2.set_calibration_values(
        [192, 248, 104, 3, 240, 203, 140, 237, 215, 88, 0, 24, 26, 173, 255, 19, 30, 0, 0, 118, 0, 111, 241, 30, 134,
         63,
         138, 46, 0, 45, 20, 120, 156, 98, 101, 118, 201, 233, 18, 199, 0, 0, 119, 0])
    temp, press, hum, gas = BME680_2.calculate_values_bme680(
        [0, 0, 128, 0, 0, 120, 72, 32, 77, 179, 128, 0, 0, 202, 121])

    print(round(temp, 2))
    print(round(hum, 2))
    # print(round(press / 100, 2))
    print(round(gas / 100, 2))
    # print(BME680_1.calculate_altitude_bme680(press))

    while True:
        # x = listen_for_packet()
        x = [6, 18, 1, 17, 1, 44]
        if x != 0 and x[0] - 1 in packet_lengths:
            old_status = status
            if checkCRC(x):
                pcb = x[1] >> 4
                sensor = x[1] & 0x0F

                if pcb in status and pcb >= 0b0100:
                    status[pcb] = [time.time(), x[2]]
                elif pcb in status and pcb == 0b0000:
                    if sensor == 0b0110:
                        status[0b0000][0b0110] = (x[2] << 8) + x[3]
                    elif sensor == 0b0101:
                        status[0b0000][0b0101] = [(x[2] << 8) + x[3], (x[4] << 8) + x[5], (x[6] << 8) + x[7]]
                    status[pcb]['time'] = time.time()
                elif pcb in status:
                    status[pcb]['time'] = time.time()
                    if sensor == 0b0011:
                        if x[0] == 46:
                            get_calibration_bme680(x[2::])
                        elif x[0] == 6:
                            res_heat_x, durval = calculate_heat_gas_bme680()
                            print(res_heat_x, durval)
                            # TODO: send res_heat_x and durval to bme680
                            packet = [np.uint8(4), np.uint8(pcb << 4 + sensor), np.uint8(round(res_heat_x)),
                                      np.uint8(durval)]
                            print(packet)
                            # send_packet(packet)
                        else:
                            calculated = calculate_values_bme680(x[2::])

                            status[pcb][0b0011]["temp"] = calculated[0]
                            status[pcb][0b0011]["hum"] = calculated[1]
                            status[pcb][0b0011]["press"] = calculated[2]
                            status[pcb][0b0011]["gas"] = calculated[3]
                    elif pcb == 0b0001:
                        status[pcb][sensor] = x[2]
                    elif pcb == 0b0001:
                        pass
                    elif pcb == 0b0001:
                        pass

            if old_status != status:
                safe_status()

        # time_now, value = status[0b0100]
        # NOTE: this is how to get the actual date and time from the time.time()
        # time_now = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time_now))
        # print(status[0b0000][0b0101])

        time.sleep(10)
