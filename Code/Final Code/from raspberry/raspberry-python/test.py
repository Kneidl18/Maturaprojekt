import time
import numpy as np
import bme680_VAL as val

packet_lengths = [3, 4, 6, 8, 17, 46]

status = {0b0000: {"time": time.time(), 0b0101: [0, 0, 0], 0b0110: 0},
          0b0001: {"time": time.time(), 0b0010: [0, 0], 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
          0b0010: {"time": time.time(), 0b0001: 0, 0b0010: 5, 0b0100: 0},
          0b0011: {"time": time.time(), 0b0001: 0, 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
          0b0100: [time.time(), 0], 0b0101: [time.time(), 0], 0b0110: [time.time(), 0],
          0b0111: [time.time(), 0], 0b1000: [time.time(), 0], 0b1001: [time.time(), 0], 0b1010: [time.time(), 0],
          0b1011: [time.time(), 0], 0b1100: [time.time(), 0], 0b1101: [time.time(), 0], 0b1110: [time.time(), 0],
          0b1111: [time.time(), 0]}

_H6, _P10, res_heat_range, _H3, _H4, _H5, _H7, _G1 = 0, 0, 0, 0, 0, 0, 0, 0
_G3, _T3, _P3, _P6, _P7, res_heat_val, range_sw_err = 0, 0, 0, 0, 0, 0, 0
_H1, _H2, _T1, _P1, _G2, _T2, _P2, _P4, _P5, _P8, _P9 = 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

test_dict = {}
for i in range(1, 16):
    test_dict[i] = 0

TX_TIMEOUT = 10  # Timeouts are added

previous_tx_timeout_millis = 0
previous_millis = 0
send_interval = 1000


def listen_for_packet():
    return [3, 0b00000101, 0, 10, 0, 10, 0, 10]


def get_calibration_bme680(buff):
    coeff_arr1, coeff_arr2 = buff[0:25], buff[25:41]
    _T1 = (coeff_arr2[val.BME680_T1_MSB_REG] << 8) + coeff_arr2[val.BME680_T1_LSB_REG]
    _T2 = (coeff_arr1[val.BME680_T2_MSB_REG] << 8) + coeff_arr1[val.BME680_T2_LSB_REG]
    _T3 = coeff_arr1[val.BME680_T3_REG]

    _P1 = (coeff_arr1[val.BME680_P1_MSB_REG] << 8) + coeff_arr1[val.BME680_P1_LSB_REG]
    _P2 = (coeff_arr1[val.BME680_P2_MSB_REG] << 8) + coeff_arr1[val.BME680_P2_LSB_REG]
    _P3 = coeff_arr1[val.BME680_P3_REG]
    _P4 = (coeff_arr1[val.BME680_P4_MSB_REG] << 8) + coeff_arr1[val.BME680_P4_LSB_REG]
    _P5 = (coeff_arr1[val.BME680_P5_MSB_REG] << 8) + coeff_arr1[val.BME680_P5_LSB_REG]
    _P6 = (coeff_arr1[val.BME680_P6_REG])
    _P7 = (coeff_arr1[val.BME680_P7_REG])
    _P8 = (coeff_arr1[val.BME680_P8_MSB_REG] << 8) + coeff_arr1[val.BME680_P8_LSB_REG]
    _P9 = (coeff_arr1[val.BME680_P9_MSB_REG] << 8) + coeff_arr1[val.BME680_P9_LSB_REG]
    _P10 = (coeff_arr1[val.BME680_P10_REG])

    _H1 = (coeff_arr2[val.BME680_H1_MSB_REG] << val.BME680_HUM_REG_SHIFT_VAL) | (
            (coeff_arr2[val.BME680_H1_LSB_REG] >> val.BME680_HUM_REG_SHIFT_VAL) & val.BME680_BIT_H1_DATA_MSK)
    _H2 = (coeff_arr2[val.BME680_H2_MSB_REG] << val.BME680_HUM_REG_SHIFT_VAL) | (
            (coeff_arr2[val.BME680_H2_LSB_REG] >> val.BME680_HUM_REG_SHIFT_VAL) & val.BME680_BIT_H1_DATA_MSK)
    _H3 = coeff_arr2[val.BME680_H3_REG]
    _H4 = coeff_arr2[val.BME680_H4_REG]
    _H5 = coeff_arr2[val.BME680_H5_REG]
    _H6 = coeff_arr2[val.BME680_H6_REG]
    _H7 = coeff_arr2[val.BME680_H7_REG]

    _G1 = coeff_arr2[val.BME680_GH1_REG]
    _G2 = (coeff_arr2[val.BME680_GH2_MSB_REG] << 8) + coeff_arr2[val.BME680_GH2_LSB_REG]
    _G3 = coeff_arr2[val.BME680_GH3_REG]

    res_sw_err = buff[41]
    _res_heat_range = buff[42]
    res_heat_val = buff[43]


# target_temp = temp we want to reach, gas_millis = for how long, amb_temp = temperature in the room
def calculate_heat_gas_bme680(target_temp=320, gas_millis=150, amb_temp=15):
    var1 = (_G1 / 16.0) + 49.0
    var2 = ((_G2 / 32768.0) * 0.0005) + 0.00235
    var3 = _G3 / 1024.0
    var4 = var1 * (1.0 + (var2 * target_temp))
    var5 = var4 + (var3 * amb_temp)
    res_heat_x = (3.4 * ((var5 * (4.0 / (4.0 + res_heat_range)) * (1.0 / (1.0 + (res_heat_val * 0.002)))) - 25))

    factor = 0
    while gas_millis > 0x3F:
        gas_millis = gas_millis >> 2
        factor += 1
    dur_val = gas_millis + (factor * 64)

    return res_heat_x, dur_val


def calculate_values_bme680(buff):
    press_adc = ((buff[2] << 12) | (buff[3] << 4) | (buff[4] >> 4))
    temp_adc = ((buff[5] << 12) | (buff[6] << 4) | (buff[7] >> 4))
    hum_adc = ((buff[8] << 8) | buff[9])
    gas_adc = (buff[13] << 2) | (buff[14] >> 6)
    gas_range = buff[14] & 0x0F

    const_array1 = [1, 1, 1, 1, 1, 0.99, 1, 0.992, 1, 1, 0.998, 0, 995, 1, 0.99, 1, 1]
    const_array2 = [8000000, 4000000, 2000000, 1000000, 499500.4995, 248262.1648, 125000, 63004.03226, 31281.28128,
                    15625, 7812.5, 3906.25, 1953.125, 976.5625, 488.28125, 244.140625]

    # temp
    var1 = ((temp_adc / 16384) - (_T1 / 1024.0)) * _T2
    var2 = (((temp_adc / 131072.0) - (_T1 / 8192.0)) * ((temp_adc / 131072.0) - (_T1 / 8192.0))) * (
            _T3 * 16)
    t_fine = var1 + var2
    temp_comp = t_fine / 5120.0

    # pressure
    var1 = (t_fine / 2.0) - 64000.0
    var2 = var1 ** 2 * (_P6 / 131072.0)
    var2 = var2 + (var1 * _P5 * 2.0)
    var2 = (var2 / 4.0) + (_P4 * 65536.0)
    var1 = (((_P3 * var1 * var1) / 16284.0) + (_P2 * var1)) / 514188.0
    var1 = (1.0 + (var1 / 32768.0)) * _P1
    press_comp = 1048576.0 - press_adc
    press_comp = ((press_comp - (var2 / 4096.0)) * 6250.0) / var1
    var1 = (_P9 * press_comp * press_comp) / 2147483648
    var2 = press_comp * (_P8 / 32768.0)
    var3 = (press_comp / 256.0) * (press_comp / 256.0) * (press_comp / 256) * (_P10 / 131072.0)
    press_comp = press_comp + (var1 + var2 + var3 + (_P7 * 128.0)) / 16.0

    # humidity
    var1 = hum_adc - ((_H1 * 16.0) + ((_H3 / 2.0) * temp_comp))
    var2 = var1 * ((_H2 / 262144.0) * (
            1.0 + ((_H4 / 16384.0) * temp_comp) + ((_H5 / 1048576.0) * temp_comp * temp_comp)))
    var3 = _H6 / 16384.0
    var4 = _H7 / 2097152.0
    hum_comp = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2)

    # gas
    var1 = (1340.0 + 5.0 * range_sw_err) * const_array1[gas_range]
    gas_res = var1 * const_array2[gas_range] / (gas_adc - 512.0 + var1)

    return temp_comp, press_comp, hum_comp, gas_res


def safe_status():
    file = open(r'FSS_DBMS.txt', 'a')
    file.write(str(status) + '\n')
    print(str(status))
    file.close()


if __name__ == '__main__':
    while True:
        current_millis = time.time_ns()
        if current_millis - previous_millis > send_interval:
            previous_millis = current_millis

        # x = listen_for_packet()
        x = [6, 0b00010011, 320 >> 8, 320 & 0xff, 150 >> 8, 150 & 0xff]
        if x != 0:
            old_status = status

            if x[0] in packet_lengths:
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
