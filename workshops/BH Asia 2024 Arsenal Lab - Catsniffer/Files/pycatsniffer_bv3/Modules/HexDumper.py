import os
import time
import threading

from .Utils import generate_filename
from .Definitions import DEFAUTL_DUMP_PATH, DEFAULT_HEX_PATH


class HexDumper(threading.Thread):
    DEFAULT_FILENAME = "hexdump.hexdump"

    def __init__(self, filename: str = DEFAULT_FILENAME):
        super().__init__()
        self.filename = filename
        self.last_packet = None
        self.frame_packet = None
        self.running = True
        self.type_worker = "raw"

    def get_filename(self):
        return os.path.join(
            os.getcwd(),
            DEFAUTL_DUMP_PATH,
            DEFAULT_HEX_PATH,
            f"{generate_filename()}_{self.filename}",
        )

    def set_filename(self, filename):
        self.filename = filename

    def run(self):
        if not os.path.exists(self.get_filename()):
            os.makedirs(os.path.dirname(self.get_filename()), exist_ok=True)

        dumper_file = open(self.get_filename(), "ab")
        while self.running:
            if self.frame_packet:
                if self.frame_packet != self.last_packet:
                    dumper_file.write(self.frame_packet)
                    dumper_file.write(b"\n")
                    dumper_file.flush()
                    self.last_packet = self.frame_packet
                    self.frame_packet = None
            else:
                time.sleep(0.01)

    def stop_thread(self):
        self.join()

    def stop_worker(self):
        self.running = False
        self.frame_packet = None
        self.join()

    def add_data(self, data):
        self.frame_packet = data
