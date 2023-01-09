# import serial  # https://pyserial.readthedocs.io/en/latest/pyserial_api.html
import ast
import time
# import RPi.GPIO as GPIO


class Eric9:
    status = {0b0000: {"time": time.time(), 0b0101: [0, 0, 0], 0b0110: 0},
              0b0001: {"time": time.time(), 0b0010: [0, 0], 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
              0b0010: {"time": time.time(), 0b0001: 0, 0b0010: 5, 0b0100: 0},
              0b0011: {"time": time.time(), 0b0001: 0, 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
              0b0100: [time.time(), 0], 0b0101: [time.time(), 0], 0b0110: [time.time(), 0],
              0b0111: [time.time(), 0], 0b1000: [time.time(), 0], 0b1001: [time.time(), 0], 0b1010: [time.time(), 0],
              0b1011: [time.time(), 0], 0b1100: [time.time(), 0], 0b1101: [time.time(), 0], 0b1110: [time.time(), 0],
              0b1111: [time.time(), 0]}

    send_byte_arr = []
    eric_busy_port = 29
    host_ready_port = 27

    port = "/dev/ttyS0"
    baud_rate = 19200
    # ser = serial.Serial(port, baud_rate)

    # ports should be set here
    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.eric_busy_port, GPIO.input)
        GPIO.setup(self.host_ready_port, GPIO.output)

    # sends the bytes of the array bytes to the ERIC9 over the UART port, returns 1 if transmission initiated
    def eric9_send_byte_arr(self, bytes):
        # here the bytes in the array will be sent over the ERIC9
        if not self.eric9_check_eric_busy():
            # self.ser.write(bytes)
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
    def eric9_set_host_ready(self):
        GPIO.output(self.host_ready_port, GPIO.LOW)

    # set's the pin for the hostReady Port so the ERIC9 knows that the Raspberry is not ready to receive data
    def eric9_set_host_not_ready(self):
        GPIO.output(self.host_ready_port, GPIO.HIGH)

    # returns true if the ERIC9 is busy and can't receive data
    def eric9_check_eric_busy(self):
        return GPIO.input(self.eric_busy_port)

    def send_status_dict(self):
        temp = str(self.status)
        self.eric9_send_byte_arr(temp[0:220])
        self.eric9_send_byte_arr(temp[220:440])
        self.eric9_send_byte_arr(temp[440:625])

    def receive_status_dict(self):
        if self.ser.inWaiting() >= 625:
            temp = self.ser.read(625)
            try:
                self.status: dict = ast.literal_eval(temp)
            except:
                print("Couldn't convert received data!!")

            self.ser.reset_input_buffer()

eric = Eric9()
eric.send_status_dict()
