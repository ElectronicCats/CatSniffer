import platform
import serial
import serial.tools.list_ports
import threading
import time
import sys
from .Definitions import START_OF_FRAME, END_OF_FRAME
from .Utils import LOG_ERROR, LOG_WARNING

if platform.system() == "Windows":
    DEFAULT_COMPORT = "COM1"
else:
    DEFAULT_COMPORT = "/dev/ttyACM0"

DEFAULT_SERIAL_BAUDRATE = 921600

class UART(threading.Thread):
    def __init__(self, serial_port: str = DEFAULT_COMPORT):
        self.serial_worker = serial.Serial()
        self.serial_worker.port = serial_port
        self.serial_worker.baudrate = DEFAULT_SERIAL_BAUDRATE
        self.recv_cancel = False

    def __del__(self):
        self.serial_worker.close()

    def __str__(self):
        return f"Serial port: {self.serial_worker.port}"

    def set_serial_port(self, serial_port: str):
        self.serial_worker.port = serial_port

    def is_valid_connection(self) -> bool:
        try:
            self.open()
            self.close()
            return True
        except serial.SerialException as e:
            LOG_ERROR(e)
            return False

    def reset_buffer(self):
        if self.serial_worker.is_open:
            self.serial_worker.reset_input_buffer()
            self.serial_worker.reset_output_buffer()

    def open(self):
        self.serial_worker.open()
        self.reset_buffer()

    def close(self):
        if self.serial_worker.is_open:
            self.reset_buffer()
            self.serial_worker.close()

    def is_connected(self):
        return self.serial_worker.is_open

    def send(self, data):
        if self.serial_worker.is_open:
            self.serial_worker.write(data)

    def recv(self):
        if not self.is_connected():
            self.open()
        try:
            time.sleep(0.01)
            bytestream = self.serial_worker.read_until((END_OF_FRAME+START_OF_FRAME))
            sof_index = 0

            eof_index = bytestream.find((END_OF_FRAME+START_OF_FRAME), sof_index)
            if eof_index == -1:
                LOG_WARNING(f"[UART] EOF - {eof_index} not found in {bytestream}")
                return None

            bytestream = START_OF_FRAME + bytestream[sof_index : eof_index + 2]
            return bytestream
        except serial.SerialException as e:
            LOG_ERROR("Error reading from serial port")
            sys.exit(1)
