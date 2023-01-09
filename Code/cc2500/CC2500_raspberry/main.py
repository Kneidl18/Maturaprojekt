import cc2500_REG
import cc2500_VAL

import spidev
import time
import RPi.GPIO as GPIO

REG_IOCFG2              = 0x0000
REG_IOCFG1              = 0x0001
REG_IOCFG0              = 0x0002
REG_FIFOTHR             = 0x0003
REG_SYNC1               = 0x0004
REG_SYNC0               = 0x0005
REG_PKTLEN              = 0x0006
REG_PKTCTRL1            = 0x0007
REG_PKTCTRL0            = 0x0008
REG_ADDR                = 0x0009
REG_CHANNR              = 0x000A
REG_FSCTRL1             = 0x000B
REG_FSCTRL0             = 0x000C
REG_FREQ2               = 0x000D
REG_FREQ1               = 0x000E
REG_FREQ0               = 0x000F
REG_MDMCFG4             = 0x0010
REG_MDMCFG3             = 0x0011
REG_MDMCFG2             = 0x0012
REG_MDMCFG1             = 0x0013
REG_MDMCFG0             = 0x0014
REG_DEVIATN             = 0x0015
REG_MCSM2               = 0x0016
REG_MCSM1               = 0x0017
REG_MCSM0               = 0x0018
REG_FOCCFG              = 0x0019
REG_BSCFG               = 0x001A
REG_AGCCTRL2            = 0x001B
REG_AGCCTRL1            = 0x001C
REG_AGCCTRL0            = 0x001D
REG_WOREVT1             = 0x001E
REG_WOREVT0             = 0x001F
REG_WORCTRL             = 0x0020
REG_FREND1              = 0x0021
REG_FREND0              = 0x0022
REG_FSCAL3              = 0x0023
REG_FSCAL2              = 0x0024
REG_FSCAL1              = 0x0025
REG_FSCAL0              = 0x0026
REG_RCCTRL1             = 0x0027
REG_RCCTRL0             = 0x0028
REG_FSTEST              = 0x0029
REG_PTEST               = 0x002A
REG_AGCTEST             = 0x002B
REG_TEST2               = 0x002C
REG_TEST1               = 0x002D
REG_TEST0               = 0x002E
REG_PARTNUM             = 0x0030
REG_VERSION             = 0x0031
REG_FREQEST             = 0x0032
REG_LQI                 = 0x0033
REG_RSSI                = 0x0034
REG_MARCSTATE           = 0x0035
REG_WORTIME1            = 0x0036
REG_WORTIME0            = 0x0037
REG_PKTSTATUS           = 0x0038
REG_VCO_VC_DAC          = 0x0039
REG_TXBYTES             = 0x003A
REG_RXBYTES             = 0x003B
REG_RCCTRL1_STATUS      = 0x003C
REG_RCCTRL0_STATUS      = 0x003D

VAL_IOCFG2               =          0x29
VAL_IOCFG1               =          0x2E
VAL_IOCFG0               =          0x06
VAL_FIFOTHR              =          0x07
VAL_SYNC1                =          0xD3
VAL_SYNC0                =          0x91
VAL_PKTLEN               =          0xFF
VAL_PKTCTRL1             =          0x04
VAL_PKTCTRL0             =          0x05
VAL_ADDR                 =          0x00
VAL_CHANNR               =          0x00
VAL_FSCTRL1              =          0x08
VAL_FSCTRL0              =          0x00
VAL_FREQ2                =          0x5D
VAL_FREQ1                =          0x93
VAL_FREQ0                =          0xB1
VAL_MDMCFG4              =          0x86
VAL_MDMCFG3              =          0x83
VAL_MDMCFG2              =          0x03
VAL_MDMCFG1              =          0x22
VAL_MDMCFG0              =          0xF8
VAL_DEVIATN              =          0x44
VAL_MCSM2                =          0x07
VAL_MCSM1                =          0x30
VAL_MCSM0                =          0x18
VAL_FOCCFG               =          0x16
VAL_BSCFG                =          0x6C
VAL_AGCCTRL2             =          0x03
VAL_AGCCTRL1             =          0x40
VAL_AGCCTRL0             =          0x91
VAL_WOREVT1              =          0x87
VAL_WOREVT0              =          0x6B
VAL_WORCTRL              =          0xF8
VAL_FREND1               =          0x56
VAL_FREND0               =          0x10
VAL_FSCAL3               =          0xA9
VAL_FSCAL2               =          0x0A
VAL_FSCAL1               =          0x00
VAL_FSCAL0               =          0x11
VAL_RCCTRL1              =          0x41
VAL_RCCTRL0              =          0x00
VAL_FSTEST               =          0x59
VAL_PTEST                =          0x7F
VAL_AGCTEST              =          0x3F
VAL_TEST2                =          0x88
VAL_TEST1                =          0x31
VAL_TEST0                =          0x0B
VAL_PARTNUM              =          0x80
VAL_VERSION              =          0x03
VAL_FREQEST              =          0x00
VAL_LQI                  =          0x00
VAL_RSSI                 =          0x00
VAL_MARCSTATE            =          0x00
VAL_WORTIME1             =          0x00
VAL_WORTIME0             =          0x00
VAL_PKTSTATUS            =          0x00
VAL_VCO_VC_DAC           =          0x00
VAL_TXBYTES              =          0x00
VAL_RXBYTES              =          0x00
VAL_RCCTRL1_STATUS       =          0x00
VAL_RCCTRL0_STATUS       =          0x00

CC2500_IDLE         = 0x36      # Exit RX / TX, turn
CC2500_TX           = 0x35      # Enable TX. If in RX state, only enable TX if CCA passes
CC2500_RX           = 0x34      # Enable RX. Perform calibration if enabled
CC2500_FTX          = 0x3B      # Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
CC2500_FRX          = 0x3A      # Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
CC2500_SWOR         = 0x38
CC2500_TXFIFO       = 0x3F
CC2500_RXFIFO       = 0x3F

TX_TIMEOUT          = 10        # Timeouts are added

previous_tx_timeout_millis = 0
previous_millis = 0
send_interval = 400

CC2500_GDO0 = 37
CC2500_SS = 35


def init_cc2500():
    write_register(REG_IOCFG2, 0x06)
    write_register(REG_IOCFG0, 0x01)
    write_register(REG_IOCFG1, 0x06)

    write_register(REG_FIFOTHR,  0x02)
    write_register(REG_SYNC1, VAL_SYNC1)
    write_register(REG_SYNC0, VAL_SYNC0)
    write_register(REG_PKTLEN, VAL_PKTLEN)
    write_register(REG_PKTCTRL1, 0x8C)
    write_register(REG_PKTCTRL0,  0x0D)
  
    write_register(REG_ADDR, VAL_ADDR)
    write_register(REG_CHANNR, VAL_CHANNR)
    write_register(REG_FSCTRL1, VAL_FSCTRL1)
    write_register(REG_FSCTRL0, VAL_FSCTRL0)
    write_register(REG_FREQ2, VAL_FREQ2)
    write_register(REG_FREQ1, VAL_FREQ1)
    write_register(REG_FREQ0, VAL_FREQ0)
    write_register(REG_MDMCFG4, VAL_MDMCFG4)
    write_register(REG_MDMCFG3, VAL_MDMCFG3)
    write_register(REG_MDMCFG2, VAL_MDMCFG2)
    write_register(REG_MDMCFG1, VAL_MDMCFG1)
    write_register(REG_MDMCFG0, VAL_MDMCFG0)
    write_register(REG_DEVIATN, VAL_DEVIATN)
    write_register(REG_MCSM2, VAL_MCSM2)
    write_register(REG_MCSM1, VAL_MCSM1)
    write_register(REG_MCSM0, VAL_MCSM0)
    write_register(REG_FOCCFG, VAL_FOCCFG)

    write_register(REG_BSCFG, VAL_BSCFG)
    write_register(REG_AGCCTRL2, 0x00)
    write_register(REG_AGCCTRL1, 0x40)
    write_register(REG_AGCCTRL0, VAL_AGCCTRL0)
    write_register(REG_WOREVT1, VAL_WOREVT1)
    write_register(REG_WOREVT0, VAL_WOREVT0)
    write_register(REG_WORCTRL, 0x78)
    write_register(REG_FREND1, VAL_FREND1)
    write_register(REG_FREND0, VAL_FREND0)
    write_register(REG_FSCAL3, VAL_FSCAL3)
    write_register(REG_FSCAL2, VAL_FSCAL2)
    write_register(REG_FSCAL1, VAL_FSCAL1)
    write_register(REG_FSCAL0, VAL_FSCAL0)
    write_register(REG_RCCTRL1, VAL_RCCTRL1)
    write_register(REG_RCCTRL0, VAL_RCCTRL0)
    write_register(REG_FSTEST, VAL_FSTEST)
    write_register(REG_PTEST, VAL_PTEST)
    write_register(REG_AGCTEST, VAL_AGCTEST)
    write_register(REG_TEST2, VAL_TEST2)
    write_register(REG_TEST1, VAL_TEST1)
    write_register(REG_TEST0, VAL_TEST0)


def read_config_regs():
    print(hex(read_register(REG_IOCFG2)))
    time.sleep(0.1)
    print(hex(read_register(REG_IOCFG1)))
    time.sleep(0.1)
    print(hex(read_register(REG_IOCFG0)))
    time.sleep(0.1)
    print(hex(read_register(REG_FIFOTHR)))
    time.sleep(0.1)
    print(hex(read_register(REG_SYNC1)))
    time.sleep(0.1)
    print(hex(read_register(REG_SYNC0)))
    time.sleep(0.1)
    print(hex(read_register(REG_PKTLEN)))
    time.sleep(0.1)
    print(hex(read_register(REG_PKTCTRL1)))
    time.sleep(0.1)
    print(hex(read_register(REG_PKTCTRL0)))
    time.sleep(0.1)
    print(hex(read_register(REG_ADDR)))
    time.sleep(0.1)
    print(hex(read_register(REG_CHANNR)))
    time.sleep(0.1)
    print(hex(read_register(REG_FSCTRL1)))
    time.sleep(0.1)
    print(hex(read_register(REG_FSCTRL0)))
    time.sleep(0.1)
    print(hex(read_register(REG_FREQ2)))
    time.sleep(0.1)
    print(hex(read_register(REG_FREQ1)))
    time.sleep(0.1)
    print(hex(read_register(REG_FREQ0)))
    time.sleep(0.1)
    print(hex(read_register(REG_MDMCFG4)))
    time.sleep(0.1)
    print(hex(read_register(REG_MDMCFG3)))
    time.sleep(0.1)
    print(hex(read_register(REG_MDMCFG2)))
    time.sleep(0.1)
    print(hex(read_register(REG_MDMCFG1)))
    time.sleep(0.1)
    print(hex(read_register(REG_MDMCFG0)))
    time.sleep(0.1)
    print(hex(read_register(REG_DEVIATN)))
    time.sleep(0.1)
    print(hex(read_register(REG_MCSM2)))
    time.sleep(0.1)
    print(hex(read_register(REG_MCSM1)))
    time.sleep(0.1)
    print(hex(read_register(REG_MCSM0)))
    time.sleep(0.1)
    print(hex(read_register(REG_FOCCFG)))
    time.sleep(0.1)

    print(hex(read_register(REG_BSCFG)))
    time.sleep(0.1)
    print(hex(read_register(REG_AGCCTRL2)))
    time.sleep(0.1)
    print(hex(read_register(REG_AGCCTRL1)))
    time.sleep(0.1)
    print(hex(read_register(REG_AGCCTRL0)))
    time.sleep(0.1)
    print(hex(read_register(REG_WOREVT1)))
    time.sleep(0.1)
    print(hex(read_register(REG_WOREVT0)))
    time.sleep(0.1)
    print(hex(read_register(REG_WORCTRL)))
    time.sleep(0.1)
    print(hex(read_register(REG_FREND1)))
    time.sleep(0.1)
    print(hex(read_register(REG_FREND0)))
    time.sleep(0.1)
    print(hex(read_register(REG_FSCAL3)))
    time.sleep(0.1)
    print(hex(read_register(REG_FSCAL2)))
    time.sleep(0.1)
    print(hex(read_register(REG_FSCAL1)))
    time.sleep(0.1)
    print(hex(read_register(REG_FSCAL0)))
    time.sleep(0.1)
    print(hex(read_register(REG_RCCTRL1)))
    time.sleep(0.1)
    print(hex(read_register(REG_RCCTRL0)))
    time.sleep(0.1)
    print(hex(read_register(REG_FSTEST)))
    time.sleep(0.1)
    print(hex(read_register(REG_PTEST)))
    time.sleep(0.1)
    print(hex(read_register(REG_AGCTEST)))
    time.sleep(0.1)
    print(hex(read_register(REG_TEST2)))
    time.sleep(0.1)
    print(hex(read_register(REG_TEST1)))
    time.sleep(0.1)
    print(hex(read_register(REG_TEST0)))
    time.sleep(0.1)
    print(hex(read_register(REG_PARTNUM)))
    time.sleep(1)
    print(hex(read_register(REG_VERSION)))
    time.sleep(1)
    print(hex(read_register(REG_FREQEST)))
    time.sleep(1)
    print(hex(read_register(REG_LQI)))
    time.sleep(1)
    print(hex(read_register(REG_RSSI)))
    time.sleep(1)
    print(hex(read_register(REG_MARCSTATE)))
    time.sleep(1)
    print(hex(read_register(REG_WORTIME1)))
    time.sleep(1)
    print(hex(read_register(REG_WORTIME0)))
    time.sleep(1)
    print(hex(read_register(REG_PKTSTATUS)))
    time.sleep(1)
    print(hex(read_register(REG_VCO_VC_DAC)))
    time.sleep(1)
    print(hex(read_register(REG_TXBYTES)))
    time.sleep(1)
    print(hex(read_register(REG_RXBYTES)))
    time.sleep(1)
    print(hex(read_register(REG_RCCTRL1_STATUS)))
    time.sleep(1)
    print(hex(read_register(REG_RCCTRL0_STATUS)))
    time.sleep(1)


def read_register(address):
    GPIO.output(CC2500_SS, GPIO.LOW)
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


#TODO change method to send a specific package instead of 'Hello'
def send_packet():
    #write_register(REG_IOCFG1, 0x06)
    #making sure that the radio is in IDLE state before flushing the FIFO
    send_strobe(CC2500_IDLE)
    #flush TX FIFO
    send_strobe(CC2500_FTX)

    #prepare packet
    length = 3
    packet = []
    #First byte = length of packet
    packet.append(length)
    packet.append(0x09)
    packet.append(0x01)

    #SIDLE: exit RX/TX
    send_strobe(CC2500_IDLE)

    print("Transmitting ")

    for i in range(length):
        write_register(CC2500_TXFIFO, packet[i])
        print(packet[i], end=" ")

    #STX: enable TX
    send_strobe(CC2500_TX)



    while GPIO.input(CC2500_GDO0, LOW):        #TODO check if this is correct, maybe check actual GDO0 pin instead
        print("GDO0 = 0")

    #wait for GDP0 to be cleared -> end of packet
    while GPIO.input(CC2500_GDO0, HIGH):
        print("GDO0 = 1")

    print("Finished sending")
    #send_strobe(CC2500_IDLE)


#TODO make packet length variable
def listen_for_packet():
    send_strobe(CC2500_RX)
    #switch MISO to output if a pcacket has been received or not
    write_register(REG_IOCFG1, 0x01)
    time.sleep(0.02)
    current_millis = time.time_ns()

    if GPIO.input(23, HIGH):
        packet_length = read_register(CC2500_RXFIFO)
        recv_packet = []

        if packet_length == 6:
            print("Packet Received")
            print("Packet Length: {}", packet_length)   #TODO check if this works

            for i in range(packet_length):
                recv_packet.append(read_register(CC2500_RXFIFO))
                print(recv_packet, end=" ")

            print(" ", end="\n")
            #print quality information
            rssi = read_register(CC2500_RXFIFO)
            lqi = read_register(CC2500_RXFIFO)

            print("RSSI: {}", rssi)
            print("LQI: {}", lqi)

        # make sure that the radio is in IDLE state before flushing the FIFO
        # (unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point
        send_strobe(CC2500_IDLE)
        #flush RX FIFO
        send_strobe(CC2500_FRX)
    else:
        pass


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
            send_packet()
        #listen_for_packet()
