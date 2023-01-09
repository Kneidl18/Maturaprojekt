import ast
import subprocess
import time

import RPi.GPIO as GPIO
import numpy as np
import serial
import spidev

import bme680_VAL as val_BME680
import cc2500_REG as Reg
import cc2500_VAL as val_CC2500

packet_lengths = [3, 4, 5, 6, 8, 17, 46]

status = {0b0000: {"time": time.time(), 0b0101: [0, 0, 0], 0b0110: 0},
          0b0001: {"time": time.time(), 0b0010: [0, 0], 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
          0b0010: {"time": time.time(), 0b0001: 0, 0b0010: 5, 0b0100: 0},
          0b0011: {"time": time.time(), 0b0001: 0, 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
          0b0100: [time.time(), 0], 0b0101: [time.time(), 0], 0b0110: [time.time(), 0],
          0b0111: [time.time(), 0], 0b1000: [time.time(), 0], 0b1001: [time.time(), 0], 0b1010: [time.time(), 0],
          0b1011: [time.time(), 0], 0b1100: [time.time(), 0], 0b1101: [time.time(), 0], 0b1110: [time.time(), 0],
          0b1111: [time.time(), 0]}

file_path = r'/home/pi/Documents/Maturaproject/raspberry-python/FSS_DBMS.txt'


class CC2500:
    CC2500_IDLE = 0x36  # Exit RX / TX, turn
    CC2500_TX = 0x35  # Enable TX. If in RX state, only enable TX if CCA passes
    CC2500_RX = 0x34  # Enable RX. Perform calibration if enabled
    CC2500_FTX = 0x3B  # Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
    CC2500_FRX = 0x3A  # Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
    CC2500_SWOR = 0x38
    CC2500_TXFIFO = 0x3F
    CC2500_RXFIFO = 0x3F

    CC2500_GDO0 = 22  # pin15, GPIO22
    CC2500_SS = 27  # pin13, GPIO27
    # CC2500_MOSI = pin19, GPIO10
    CC2500_MISO = 17  # pin21, GPIO9
    # CC2500_SPSCK = pin23, GPIO11

    spi = spidev.SpiDev()
    spi.open(0, 0)
    spi.max_speed_hz = 300000
    spi.mode = 0

    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.CC2500_SS, GPIO.OUT)
        GPIO.setup(self.CC2500_GDO0, GPIO.IN)
        GPIO.setup(self.CC2500_MISO, GPIO.IN)

        print("Initializing Wireless..")

        self.write_register(Reg.REG_IOCFG2, val_CC2500.VAL_IOCFG2)
        self.write_register(Reg.REG_IOCFG0, val_CC2500.VAL_IOCFG0)
        self.write_register(Reg.REG_IOCFG1, val_CC2500.VAL_IOCFG1)

        self.write_register(Reg.REG_FIFOTHR, val_CC2500.VAL_FIFOTHR)
        self.write_register(Reg.REG_SYNC1, val_CC2500.VAL_SYNC1)
        self.write_register(Reg.REG_SYNC0, val_CC2500.VAL_SYNC0)
        self.write_register(Reg.REG_PKTLEN, val_CC2500.VAL_PKTLEN)
        self.write_register(Reg.REG_PKTCTRL1, val_CC2500.VAL_PKTCTRL1)
        self.write_register(Reg.REG_PKTCTRL0, val_CC2500.VAL_PKTCTRL0)

        self.write_register(Reg.REG_ADDR, val_CC2500.VAL_ADDR)
        self.write_register(Reg.REG_CHANNR, val_CC2500.VAL_CHANNR)
        self.write_register(Reg.REG_FSCTRL1, val_CC2500.VAL_FSCTRL1)
        self.write_register(Reg.REG_FSCTRL0, val_CC2500.VAL_FSCTRL0)
        self.write_register(Reg.REG_FREQ2, val_CC2500.VAL_FREQ2)
        self.write_register(Reg.REG_FREQ1, val_CC2500.VAL_FREQ1)
        self.write_register(Reg.REG_FREQ0, val_CC2500.VAL_FREQ0)
        self.write_register(Reg.REG_MDMCFG4, val_CC2500.VAL_MDMCFG4)
        self.write_register(Reg.REG_MDMCFG3, val_CC2500.VAL_MDMCFG3)
        self.write_register(Reg.REG_MDMCFG2, val_CC2500.VAL_MDMCFG2)
        self.write_register(Reg.REG_MDMCFG1, val_CC2500.VAL_MDMCFG1)
        self.write_register(Reg.REG_MDMCFG0, val_CC2500.VAL_MDMCFG0)
        self.write_register(Reg.REG_DEVIATN, val_CC2500.VAL_DEVIATN)
        self.write_register(Reg.REG_MCSM2, val_CC2500.VAL_MCSM2)
        self.write_register(Reg.REG_MCSM1, val_CC2500.VAL_MCSM1)
        self.write_register(Reg.REG_MCSM0, val_CC2500.VAL_MCSM0)
        self.write_register(Reg.REG_FOCCFG, val_CC2500.VAL_FOCCFG)

        self.write_register(Reg.REG_BSCFG, val_CC2500.VAL_BSCFG)
        self.write_register(Reg.REG_AGCCTRL2, val_CC2500.VAL_AGCCTRL2)
        self.write_register(Reg.REG_AGCCTRL1, val_CC2500.VAL_AGCCTRL1)
        self.write_register(Reg.REG_AGCCTRL0, val_CC2500.VAL_AGCCTRL0)
        self.write_register(Reg.REG_WOREVT1, val_CC2500.VAL_WOREVT1)
        self.write_register(Reg.REG_WOREVT0, val_CC2500.VAL_WOREVT0)
        self.write_register(Reg.REG_WORCTRL, val_CC2500.VAL_WORCTRL)
        self.write_register(Reg.REG_FREND1, val_CC2500.VAL_FREND1)
        self.write_register(Reg.REG_FREND0, val_CC2500.VAL_FREND0)
        self.write_register(Reg.REG_FSCAL3, val_CC2500.VAL_FSCAL3)
        self.write_register(Reg.REG_FSCAL2, val_CC2500.VAL_FSCAL2)
        self.write_register(Reg.REG_FSCAL1, val_CC2500.VAL_FSCAL1)
        self.write_register(Reg.REG_FSCAL0, val_CC2500.VAL_FSCAL0)
        self.write_register(Reg.REG_RCCTRL1, val_CC2500.VAL_RCCTRL1)
        self.write_register(Reg.REG_RCCTRL0, val_CC2500.VAL_RCCTRL0)
        self.write_register(Reg.REG_FSTEST, val_CC2500.VAL_FSTEST)
        self.write_register(Reg.REG_PTEST, val_CC2500.VAL_PTEST)
        self.write_register(Reg.REG_AGCTEST, val_CC2500.VAL_AGCTEST)
        self.write_register(Reg.REG_TEST2, val_CC2500.VAL_TEST2)
        self.write_register(Reg.REG_TEST1, val_CC2500.VAL_TEST1)
        self.write_register(Reg.REG_TEST0, val_CC2500.VAL_TEST0)

        self.write_register(Reg.REG_PARTNUM, val_CC2500.VAL_PARTNUM)
        self.write_register(Reg.REG_VERSION, val_CC2500.VAL_VERSION)
        self.write_register(Reg.REG_FREQEST, val_CC2500.VAL_FREQEST)
        self.write_register(Reg.REG_LQI, val_CC2500.VAL_LQI)
        self.write_register(Reg.REG_RSSI, val_CC2500.VAL_RSSI)
        self.write_register(Reg.REG_MARCSTATE, val_CC2500.VAL_MARCSTATE)
        self.write_register(Reg.REG_WORTIME1, val_CC2500.VAL_WORTIME1)
        self.write_register(Reg.REG_WORTIME0, val_CC2500.VAL_WORTIME0)
        self.write_register(Reg.REG_PKTSTATUS, val_CC2500.VAL_PKTSTATUS)
        self.write_register(Reg.REG_VCO_VC_DAC, val_CC2500.VAL_VCO_VC_DAC)
        self.write_register(Reg.REG_TXBYTES, val_CC2500.VAL_TXBYTES)
        self.write_register(Reg.REG_RXBYTES, val_CC2500.VAL_RXBYTES)
        self.write_register(Reg.REG_RCCTRL1_STATUS, val_CC2500.VAL_RCCTRL1_STATUS)
        self.write_register(Reg.REG_RCCTRL0_STATUS, val_CC2500.VAL_RCCTRL0_STATUS)

        self.read_config_regs()

    def read_config_regs(self):
        print(hex(self.read_register(Reg.REG_IOCFG2)))
        time.sleep(0.1)
        print(hex(self.read_register(Reg.REG_IOCFG1)))
        time.sleep(0.1)
        print(hex(self.read_register(Reg.REG_IOCFG0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FIFOTHR)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_SYNC1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_SYNC0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_PKTLEN)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_PKTCTRL1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_PKTCTRL0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_ADDR)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_CHANNR)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FSCTRL1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FSCTRL0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FREQ2)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FREQ1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FREQ0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_MDMCFG4)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_MDMCFG3)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_MDMCFG2)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_MDMCFG1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_MDMCFG0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_DEVIATN)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_MCSM2)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_MCSM1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_MCSM0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FOCCFG)))
        # time.sleep(0.1)
        #
        # print(hex(self.read_register(Reg.REG_BSCFG)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_AGCCTRL2)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_AGCCTRL1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_AGCCTRL0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_WOREVT1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_WOREVT0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_WORCTRL)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FREND1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FREND0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FSCAL3)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FSCAL2)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FSCAL1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FSCAL0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_RCCTRL1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_RCCTRL0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_FSTEST)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_PTEST)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_AGCTEST)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_TEST2)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_TEST1)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_TEST0)))
        # time.sleep(0.1)
        # print(hex(self.read_register(Reg.REG_PARTNUM)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_VERSION)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_FREQEST)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_LQI)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_RSSI)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_MARCSTATE)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_WORTIME1)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_WORTIME0)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_PKTSTATUS)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_VCO_VC_DAC)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_TXBYTES)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_RXBYTES)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_RCCTRL1_STATUS)))
        # time.sleep(1)
        # print(hex(self.read_register(Reg.REG_RCCTRL0_STATUS)))
        # time.sleep(1)

    def read_register(self, address):
        GPIO.output(self.CC2500_SS, GPIO.LOW)
        address |= (1 << 7)
        read_byte = self.spi.xfer2([address, 00])
        GPIO.output(self.CC2500_SS, GPIO.HIGH)
        return read_byte[1]

    def write_register(self, address, byte):
        GPIO.output(self.CC2500_SS, GPIO.LOW)
        self.spi.xfer2([address, byte])
        GPIO.output(self.CC2500_SS, GPIO.HIGH)

    def send_strobe(self, strobe):
        GPIO.output(self.CC2500_SS, GPIO.LOW)
        self.spi.xfer2([strobe])
        GPIO.output(self.CC2500_SS, GPIO.HIGH)

    def send_packet(self, packet_cc2500: list):
        length = packet_cc2500[0]

        # write_register(Reg.REG_IOCFG1, 0x06)
        # making sure that the radio is in IDLE state before flushing the FIFO
        self.send_strobe(self.CC2500_IDLE)
        # flush TX FIFO
        self.send_strobe(self.CC2500_FTX)

        # SIDLE: exit RX/TX
        self.send_strobe(self.CC2500_IDLE)

        print("Transmitting: ", end='')

        for i in range(length):
            self.write_register(self.CC2500_TXFIFO, packet_cc2500[i])
            print(packet_cc2500[i], end=" ")

        # STX: enable TX
        self.send_strobe(self.CC2500_TX)

        # while GPIO.input(CC2500_GDO0, LOW):
        # print("GDO0 = 0")

        # wait for GDP0 to be cleared -> end of packet
        # while GPIO.input(CC2500_GDO0, HIGH):
        # print("GDO0 = 1")

        print("Finished sending")
        # send_strobe(CC2500_IDLE)

    def listen_for_packet(self):
        self.send_strobe(self.CC2500_RX)
        # switch MISO to output if a pcacket has been received or not
        self.write_register(Reg.REG_IOCFG1, 0x01)
        time.sleep(1)
        # current_millis = time.time_ns()

        if GPIO.input(self.CC2500_MISO) == GPIO.HIGH:
            packet_length = int(self.read_register(self.CC2500_RXFIFO))
            recv_packet = [packet_length]

            print("Packet Received")
            print("Packet Length: ", packet_length)

            for i in range(packet_length - 1):
                recv_packet.append(self.read_register(self.CC2500_RXFIFO))

            # print quality information
            rssi = self.read_register(self.CC2500_RXFIFO)
            lqi = self.read_register(self.CC2500_RXFIFO)

            # make sure that the radio is in IDLE state before flushing the FIFO
            # (unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point
            self.send_strobe(self.CC2500_IDLE)
            # flush RX FIFO
            self.send_strobe(self.CC2500_FRX)
            
            return recv_packet
        else:
            return 0


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

        const_array1 = [1, 1, 1, 1, 1, 0.99, 1, 0.992, 1, 1, 0.998, 0, 995, 1, 0.99, 1, 1]
        const_array2 = [8000000, 4000000, 2000000, 1000000, 499500.4995, 248262.1648, 125000, 63004.03226, 31281.28128,
                        15625, 7812.5, 3906.25, 1953.125, 976.5625, 488.28125, 244.140625]

        # temp
        var1 = ((temp_adc / 16384) - (self._T1 / 1024.0)) * self._T2
        var2 = (((temp_adc / 131072.0) - (self._T1 / 8192.0)) * ((temp_adc / 131072.0) - (self._T1 / 8192.0))) * (
                self._T3 * 16)
        t_fine = var1 + var2
        temp_comp = t_fine / 5120.0

        # pressure
        var1 = (t_fine / 2.0) - 64000.0
        var2 = var1 ** 2 * (self._P6 / 131072.0)
        var2 = var2 + (var1 * self._P5 * 2.0)
        var2 = (var2 / 4.0) + (self._P4 * 65536.0)
        var1 = (((self._P3 * var1 * var1) / 16284.0) + (self._P2 * var1)) / 514188.0
        var1 = (1.0 + (var1 / 32768.0)) * self._P1
        press_comp = 1048576.0 - press_adc
        press_comp = ((press_comp - (var2 / 4096.0)) * 6250.0) / var1
        var1 = (self._P9 * press_comp * press_comp) / 2147483648
        var2 = press_comp * (self._P8 / 32768.0)
        var3 = (press_comp / 256.0) * (press_comp / 256.0) * (press_comp / 256) * (self._P10 / 131072.0)
        press_comp = press_comp + (var1 + var2 + var3 + (self._P7 * 128.0)) / 16.0

        # humidity
        var1 = hum_adc - ((self._H1 * 16.0) + ((self._H3 / 2.0) * temp_comp))
        var2 = var1 * ((self._H2 / 262144.0) * (
                1.0 + ((self._H4 / 16384.0) * temp_comp) + ((self._H5 / 1048576.0) * temp_comp * temp_comp)))
        var3 = self._H6 / 16384.0
        var4 = self._H7 / 2097152.0
        hum_comp = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2)

        # gas
        var1 = (1340.0 + 5.0 * self.range_sw_err) * const_array1[gas_range]
        gas_res = var1 * const_array2[gas_range] / (gas_adc - 512.0 + var1)

        return temp_comp, press_comp, hum_comp, gas_res


# TODO: update_status() has to safe the read status dict to status (doesn't do taht for some reason rn)
class Database:
    @staticmethod
    def safe_status():
        file = open(file_path, 'a')
        file.write(str(status) + '\n')
        print(str(status))
        file.close()

    @staticmethod
    def update_status():
        file = open(file_path, 'r')
        temp = str(file.read())
        file.close()

        temp = temp.split('\n')
        status: dict = ast.literal_eval(temp[-2])


class Eric9:
    send_byte_arr = []
    eric_busy_port = 21
    host_ready_port = 20

    port = "/dev/ttyS0"
    baud_rate = 19200
    ser = serial.Serial(port, baud_rate)

    # ports should be set here
    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.eric_busy_port, GPIO.IN)
        GPIO.setup(self.host_ready_port, GPIO.OUT)

    # sends the bytes of the array bytes to the ERIC9 over the UART port, returns 1 if transmission initiated
    def eric9_send_byte_arr(self, bytes):
        # here the bytes in the array will be sent over the ERIC9
        if not self.__eric9_check_eric_busy():
            self.ser.write(str.encode(bytes))
            return 1
        return 0

    # checks if there are any bytes to receive. if then it returns the bytes, else return 0
    def eric9_receive_byte_arr(self):
        if self.ser.inWaiting() > 0:
            temp = self.ser.read()
            time.sleep(0.03)
            data_left = self.ser.inWaiting()
            temp += self.ser.read(data_left)
            return temp
        return 0

    # clears the pin where the hostReady Port is connected to the ERIC9 to indicate that the Raspberry is ready to
    # receive data
    def __eric9_set_host_ready(self):
        GPIO.output(self.host_ready_port, GPIO.LOW)

    # set's the pin for the hostReady Port so the ERIC9 knows that the Raspberry is not ready to receive data
    def __eric9_set_host_not_ready(self):
        GPIO.output(self.host_ready_port, GPIO.HIGH)

    # returns true if the ERIC9 is busy and can't receive data
    def __eric9_check_eric_busy(self):
        return GPIO.input(self.eric_busy_port)

    def send_status_dict(self):
        temp = str(status)
        while self.__eric9_check_eric_busy():
            print(self.__eric9_check_eric_busy())
            
        while self.eric9_send_byte_arr(temp[0:220]) != 1:
            pass
        print(temp[0:220])
        time.sleep(1.3)

        while self.__eric9_check_eric_busy():
            pass
        
        while self.eric9_send_byte_arr(temp[220:440]) != 1:
            pass
        print(temp[220:440])
        time.sleep(1.3)


        while self.__eric9_check_eric_busy():
            pass
        while self.eric9_send_byte_arr(temp[440::]) != 1:
            pass
        print(temp[440:625])


def check_doors():
    for i in range(4, 16):
        if time.time() - status[i][0] >= 20 and status[i][1] == 0:
            status[i][0] = time.time()
            status[i][1] = 1


def check_data():
    # CC2500_instance.send_packet([3, 0b01000001, 1])
    # CC2500_instance.send_packet([3, 0b01000001, 0])
    check_doors()

    x = CC2500_instance.listen_for_packet()
    if x != 0:
        old_status = status

        if x[0] in packet_lengths:
            pcb = x[1] >> 4
            sensor = x[1] & 0x0F

            if pcb in status and pcb >= 4:
                if sensor == 1:
                    status[pcb] = [time.time(), x[2]]
            elif pcb == 0:
                if sensor == 6:
                    status[0][6] = (x[2] << 8) + x[3]
                elif sensor == 5:
                    status[0][5] = [(x[2] << 8) + x[3], (x[4] << 8) + x[5], (x[6] << 8) + x[7]]
                status[pcb]['time'] = time.time()
            elif pcb in status:
                status[pcb]['time'] = time.time()
                if sensor == 3 and pcb == 1:
                    if x[0] == 46:
                        BME680_1.set_calibration_values(x[2::])
                    elif x[0] == 6:
                        res_heat_x, durval = BME680_1.calculate_heat_gas_bme680()
                        print(res_heat_x, durval)
                        # TODO: send res_heat_x and durval to bme680
                        packet = [np.uint8(4), np.uint8(pcb << 4 + sensor), np.uint8(round(res_heat_x)),
                                  np.uint8(durval)]
                        print(packet)
                        CC2500_instance.send_packet(packet)
                    else:
                        calculated = BME680_1.calculate_values_bme680(x[2::])

                        status[pcb][3]["temp"] = calculated[0]
                        status[pcb][3]["hum"] = calculated[1]
                        status[pcb][3]["press"] = calculated[2]
                        status[pcb][3]["gas"] = calculated[3]

                elif sensor == 3 and pcb == 3:
                    if x[0] == 46:
                        BME680_2.set_calibration_values(x[2::])
                    elif x[0] == 6:
                        res_heat_x, durval = BME680_2.calculate_heat_gas_bme680()
                        print(res_heat_x, durval)
                        # TODO: send res_heat_x and durval to bme680
                        packet = [np.uint8(4), np.uint8(pcb << 4 + sensor), np.uint8(round(res_heat_x)),
                                  np.uint8(durval)]
                        print(packet)
                        CC2500_instance.send_packet(packet)
                    else:
                        calculated = BME680_2.calculate_values_bme680(x[2::])

                        status[pcb][3]["temp"] = calculated[0]
                        status[pcb][3]["hum"] = calculated[1]
                        status[pcb][3]["press"] = calculated[2]
                        status[pcb][3]["gas"] = calculated[3]

                elif x[0] == 5 and pcb == 1 and sensor == 2:
                    if x[2] == 0:
                        status[pcb][sensor][0] = x[3] + 0.25 * x[4]
                    elif x[2] == 1:
                        status[pcb][sensor][1] = x[3] + 0.25 * x[4]
                elif x[0] == 3 and (pcb == 2 or pcb == 3) and sensor == 1:
                    status[pcb + 30][sensor] = x[2]

        if old_status != status:
            DBMS.safe_status()


if __name__ == '__main__':
    CC2500_instance = CC2500()
    eric9_instance = Eric9()
    DBMS = Database()
    BME680_1 = BME680()
    BME680_2 = BME680()

    # DBMS.update_status()
    counter = 0
    while True:
        check_data()

        if counter >= 10:
            counter = 0
            eric9_instance.send_status_dict()

        counter += 1
        time.sleep(0.1)

    # time_now, value = status[0b0100]
    # NOTE: this is how to get the actual date and time from the time.time()
    # time_now = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time_now))
    # print(status[0b0000][0b0101])
