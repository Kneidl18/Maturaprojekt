import ast
import subprocess
import time

import RPi.GPIO as GPIO
import serial

status = {0b0000: {"time": time.time(), 0b0101: [0, 0, 0], 0b0110: 0},
          0b0001: {"time": time.time(), 0b0010: [0, 0], 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
          0b0010: {"time": time.time(), 0b0001: 0, 0b0010: 5, 0b0100: 0},
          0b0011: {"time": time.time(), 0b0001: 0, 0b0011: {"temp": 0, "hum": 0, "press": 0, "gas": 0}},
          0b0100: [time.time(), 0], 0b0101: [time.time(), 0], 0b0110: [time.time(), 0],
          0b0111: [time.time(), 0], 0b1000: [time.time(), 0], 0b1001: [time.time(), 0], 0b1010: [time.time(), 0],
          0b1011: [time.time(), 0], 0b1100: [time.time(), 0], 0b1101: [time.time(), 0], 0b1110: [time.time(), 0],
          0b1111: [time.time(), 0]}


class Database:
    db_path = r'xyz'

    # initializes the DBMS and creates the database at the paths location, if it doesn't exist yet
    def __init__(self):
        pass

    @staticmethod
    def safe_status():
        try:
            file = open(r'FSS_DBMS.txt', 'a')
            file.write(str(status) + '\n')
            print("saving status_dict: ", str(status))
            file.close()
        except:
            print("ERROR writing to 'FSS_DBMS.txt'")


class Eric9:
    eric_busy_port = 29
    host_ready_port = 27

    port = "/dev/ttyS0"
    baud_rate = 19200
    ser = serial.Serial()

    __rssi = 0
    __lqi = 0

    # ports should be set here
    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(self.eric_busy_port, GPIO.IN)
        GPIO.setup(self.host_ready_port, GPIO.OUT)
        self.ser = serial.Serial(self.port, self.baud_rate)

    # sends the bytes of the array bytes to the ERIC9 over the UART port, returns 1 if transmission initiated
    def send_byte_arr(self, bytes):
        # here the bytes in the array will be sent over the ERIC9
        if not self.__check_eric_busy():
            self.ser.write(str.encode(bytes))
            return 1
        return 0

    # checks if there are any bytes to receive. if there are it checks if it's the expected data, converts it and returns it
    def receive_byte_arr(self):
        if self.ser.inWaiting() >= 550:
            print("receiving byte")
            time.sleep(1.5)
            packet_length = self.ser.inWaiting()
            temp = self.ser.read(packet_length)
                    
            self.ser.flushInput()
            return self.__create_dict(temp)

    # returns the dict (created by dict_to_convert) or 0 if an error occured
    @staticmethod
    def __create_dict(dict_to_convert):
        dict_to_convert = dict_to_convert.decode()
        print("received bytes: ", dict_to_convert)
        try:
            temp_dict: dict = ast.literal_eval(dict_to_convert)
            if type(temp_dict) is dict:
                return temp_dict
            else:
                return 0
        except:
            print("ERROR while trying to convert incomming data")

    # clears the pin where the hostReady Port is connected to the ERIC9 to indicate that the Raspberry is ready to
    # receive data
    def __set_host_ready(self):
        GPIO.output(self.host_ready_port, GPIO.LOW)

    # set's the pin for the hostReady Port so the ERIC9 knows that the Raspberry is not ready to receive data
    def __set_host_not_ready(self):
        GPIO.output(self.host_ready_port, GPIO.HIGH)

    # returns true if the ERIC9 is busy and can't receive data
    def __check_eric_busy(self):
        return GPIO.input(self.eric_busy_port)

    def __check_rssi_lqi_data(self):
        self.rssi = 10
        self.lqi = 10
        
    def clear_input(self):
        self.ser.flushInput()


def send_data_to_raspberry():
    pass


# main routine. makes instances of the classes
if __name__ == '__main__':
    # subprocess.call('python3 GUI_FSS.py', shell=True)

    eric9_instance = Eric9()
    DBMS = Database()
    eric9_instance.clear_input()

    while True:
        status_changed = eric9_instance.receive_byte_arr()
        if type(status_changed) is dict:
            old_status = status
            status = status_changed
            if old_status is not status:
                DBMS.safe_status()
                
        time.sleep(1)

    # time_now, value = status[0b0100]
    # NOTE: this is how to get the actual date and time from the time.time()
    # time_now = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time_now))
    # print(status[0b0000][0b0101])
