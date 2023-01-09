import cc2500_REG as Reg
import cc2500_VAL as Val

import spidev
import time
import RPi.GPIO as GPIO


CC2500_IDLE = 0x36  # Exit RX / TX, turn
CC2500_TX = 0x35  # Enable TX. If in RX state, only enable TX if CCA passes
CC2500_RX = 0x34  # Enable RX. Perform calibration if enabled
CC2500_FTX = 0x3B  # Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
CC2500_FRX = 0x3A  # Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
CC2500_SWOR = 0x38
CC2500_TXFIFO = 0x3F
CC2500_RXFIFO = 0x3F

TX_TIMEOUT = 10  # Timeouts are added

previous_tx_timeout_millis = 0
previous_millis = 0
send_interval = 400

CC2500_GDO0 = 37
CC2500_SS = 35

packet_lengths = [3, 4, 6, 8, 17, 46]

status = {0b0000: [], 0b0001: [], 0b0010: [], 0b0011: [], 0b0100: [], 0b0101: [],
          0b0110: [], 0b0111: [], 0b1000: [], 0b1001: [], 0b1010: [], 0b1011: [],
          0b1100: [], 0b1101: [], 0b1110: [], 0b1111: []}


def init_cc2500():
    write_register(Reg.REG_IOCFG2, 0x06)
    write_register(Reg.REG_IOCFG0, 0x01)
    write_register(Reg.REG_IOCFG1, 0x06)

    write_register(Reg.REG_FIFOTHR, 0x02)
    write_register(Reg.REG_SYNC1, Val.VAL_SYNC1)
    write_register(Reg.REG_SYNC0, Val.VAL_SYNC0)
    write_register(Reg.REG_PKTLEN, Val.VAL_PKTLEN)
    write_register(Reg.REG_PKTCTRL1, 0x8C)
    write_register(Reg.REG_PKTCTRL0, 0x0D)

    write_register(Reg.REG_ADDR, Val.VAL_ADDR)
    write_register(Reg.REG_CHANNR, Val.VAL_CHANNR)
    write_register(Reg.REG_FSCTRL1, Val.VAL_FSCTRL1)
    write_register(Reg.REG_FSCTRL0, Val.VAL_FSCTRL0)
    write_register(Reg.REG_FREQ2, Val.VAL_FREQ2)
    write_register(Reg.REG_FREQ1, Val.VAL_FREQ1)
    write_register(Reg.REG_FREQ0, Val.VAL_FREQ0)
    write_register(Reg.REG_MDMCFG4, Val.VAL_MDMCFG4)
    write_register(Reg.REG_MDMCFG3, Val.VAL_MDMCFG3)
    write_register(Reg.REG_MDMCFG2, Val.VAL_MDMCFG2)
    write_register(Reg.REG_MDMCFG1, Val.VAL_MDMCFG1)
    write_register(Reg.REG_MDMCFG0, Val.VAL_MDMCFG0)
    write_register(Reg.REG_DEVIATN, Val.VAL_DEVIATN)
    write_register(Reg.REG_MCSM2, Val.VAL_MCSM2)
    write_register(Reg.REG_MCSM1, Val.VAL_MCSM1)
    write_register(Reg.REG_MCSM0, Val.VAL_MCSM0)
    write_register(Reg.REG_FOCCFG, Val.VAL_FOCCFG)

    write_register(Reg.REG_BSCFG, Val.VAL_BSCFG)
    write_register(Reg.REG_AGCCTRL2, 0x00)
    write_register(Reg.REG_AGCCTRL1, 0x40)
    write_register(Reg.REG_AGCCTRL0, Val.VAL_AGCCTRL0)
    write_register(Reg.REG_WOREVT1, Val.VAL_WOREVT1)
    write_register(Reg.REG_WOREVT0, Val.VAL_WOREVT0)
    write_register(Reg.REG_WORCTRL, 0x78)
    write_register(Reg.REG_FREND1, Val.VAL_FREND1)
    write_register(Reg.REG_FREND0, Val.VAL_FREND0)
    write_register(Reg.REG_FSCAL3, Val.VAL_FSCAL3)
    write_register(Reg.REG_FSCAL2, Val.VAL_FSCAL2)
    write_register(Reg.REG_FSCAL1, Val.VAL_FSCAL1)
    write_register(Reg.REG_FSCAL0, Val.VAL_FSCAL0)
    write_register(Reg.REG_RCCTRL1, Val.VAL_RCCTRL1)
    write_register(Reg.REG_RCCTRL0, Val.VAL_RCCTRL0)
    write_register(Reg.REG_FSTEST, Val.VAL_FSTEST)
    write_register(Reg.REG_PTEST, Val.VAL_PTEST)
    write_register(Reg.REG_AGCTEST, Val.VAL_AGCTEST)
    write_register(Reg.REG_TEST2, Val.VAL_TEST2)
    write_register(Reg.REG_TEST1, Val.VAL_TEST1)
    write_register(Reg.REG_TEST0, Val.VAL_TEST0)


def read_config_regs():
    print(hex(read_register(Reg.REG_IOCFG2)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_IOCFG1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_IOCFG0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FIFOTHR)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_SYNC1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_SYNC0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_PKTLEN)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_PKTCTRL1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_PKTCTRL0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_ADDR)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_CHANNR)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FSCTRL1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FSCTRL0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FREQ2)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FREQ1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FREQ0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_MDMCFG4)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_MDMCFG3)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_MDMCFG2)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_MDMCFG1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_MDMCFG0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_DEVIATN)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_MCSM2)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_MCSM1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_MCSM0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FOCCFG)))
    time.sleep(0.1)

    print(hex(read_register(Reg.REG_BSCFG)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_AGCCTRL2)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_AGCCTRL1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_AGCCTRL0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_WOREVT1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_WOREVT0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_WORCTRL)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FREND1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FREND0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FSCAL3)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FSCAL2)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FSCAL1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FSCAL0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_RCCTRL1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_RCCTRL0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_FSTEST)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_PTEST)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_AGCTEST)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_TEST2)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_TEST1)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_TEST0)))
    time.sleep(0.1)
    print(hex(read_register(Reg.REG_PARTNUM)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_VERSION)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_FREQEST)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_LQI)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_RSSI)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_MARCSTATE)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_WORTIME1)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_WORTIME0)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_PKTSTATUS)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_VCO_VC_DAC)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_TXBYTES)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_RXBYTES)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_RCCTRL1_STATUS)))
    time.sleep(1)
    print(hex(read_register(Reg.REG_RCCTRL0_STATUS)))
    time.sleep(1)


def read_register(address):
    GPIO.output(CC2500_SS, GPIO.LOW)
    address |= (1 << 7)
    read_byte = spi.xfer2([address, 00])
    GPIO.output(CC2500_SS, GPIO.HIGH)
    return read_byte[1]


def write_register(address, byte):
    GPIO.output(CC2500_SS, GPIO.LOW)
    for i, j in address, byte:
        spi.xfer2([i, j])
    GPIO.output(CC2500_SS, GPIO.HIGH)


def send_strobe(strobe):
    GPIO.output(CC2500_SS, GPIO.LOW)
    result = spi.xfer2([strobe])
    GPIO.output(CC2500_SS, GPIO.HIGH)
    return result


def send_packet(packet: list):
    length = packet[0]

    # write_register(Reg.REG_IOCFG1, 0x06)
    # making sure that the radio is in IDLE state before flushing the FIFO
    send_strobe(CC2500_IDLE)
    # flush TX FIFO
    send_strobe(CC2500_FTX)

    # SIDLE: exit RX/TX
    send_strobe(CC2500_IDLE)

    print("Transmitting: ", end='')

    for i in range(length):
        write_register(CC2500_TXFIFO, packet[i])
        print(packet[i], end=" ")

    # STX: enable TX
    send_strobe(CC2500_TX)

    # while GPIO.input(CC2500_GDO0, LOW):
    # print("GDO0 = 0")

    # wait for GDP0 to be cleared -> end of packet
    # while GPIO.input(CC2500_GDO0, HIGH):
    # print("GDO0 = 1")

    print("Finished sending")
    # send_strobe(CC2500_IDLE)


def listen_for_packet():
    send_strobe(CC2500_RX)
    # switch MISO to output if a pcacket has been received or not
    write_register(Reg.REG_IOCFG1, 0x01)
    time.sleep(0.02)
    # current_millis = time.time_ns()

    if GPIO.input(23, GPIO.HIGH):
        packet_length = read_register(CC2500_RXFIFO)
        recv_packet = [packet_length]

        if packet_length == 6:
            print("Packet Received")
            print("Packet Length: {}", packet_length)  # TODO check if this works

            for i in range(packet_length):
                recv_packet.append(read_register(CC2500_RXFIFO))
                print(recv_packet, end="")

            print()
            # print quality information
            rssi = read_register(CC2500_RXFIFO)
            lqi = read_register(CC2500_RXFIFO)

            print("RSSI: {}", rssi)
            print("LQI: {}", lqi)

        # make sure that the radio is in IDLE state before flushing the FIFO
        # (unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point
        send_strobe(CC2500_IDLE)
        # flush RX FIFO
        send_strobe(CC2500_FRX)

        return recv_packet
    else:
        return 0


if __name__ == '__main__':
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(22, GPIO.OUT)
    GPIO.output(22, GPIO.HIGH)

    spi = spidev.SpiDev()
    spi.open(0, 0)
    spi.max_speed_hz = 200000
    spi.mode = 0

    print("Initializing Wireless..")
    init_cc2500()
    read_config_regs()

    while True:
        current_millis = time.time_ns()
        if current_millis - previous_millis > send_interval:
            previous_millis = current_millis
            send_packet([3, 0x09, 0x03])

        x = listen_for_packet()
        if x != 0:
            # TODO: he will be the action when something arrives through the CC2500
            if x in packet_lengths:
                pcb = x[1] >> 4
                sensor = x[1] & 0x0F
