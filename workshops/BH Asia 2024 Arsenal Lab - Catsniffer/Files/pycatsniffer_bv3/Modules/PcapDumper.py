import os
import time
import threading

from .Utils import generate_filename
from .Definitions import (
    DEFAUTL_DUMP_PATH,
    DEFAULT_PCAP_PATH,
    LINKTYPE_BLUETOOTH_LE_LL_WITH_PHDR,
)
from .Pcap import get_global_header


class PcapDumper(threading.Thread):
    DEFAULT_FILENAME = "pcapdump.pcap"

    def __init__(self, filename: str = DEFAULT_FILENAME):
        super().__init__()
        self.filename = filename
        self.data_queue = []
        self.data_length = 0
        self.last_packet = None
        self.frame_packet = None
        self.data_queue_lock = threading.Lock()
        self.running = True
        self.needs_header = True
        self.type_worker = "pcap"
        self.linktype = LINKTYPE_BLUETOOTH_LE_LL_WITH_PHDR

    def get_filename(self):
        return os.path.join(
            os.getcwd(),
            DEFAUTL_DUMP_PATH,
            DEFAULT_PCAP_PATH,
            f"{generate_filename()}_{self.filename}",
        )

    def set_filename(self, filename):
        self.filename = filename

    def set_linktype(self, linktype: int):
        self.linktype = linktype

    def run(self):
        if not os.path.exists(self.get_filename()):
            os.makedirs(os.path.dirname(self.get_filename()), exist_ok=True)

        dumper_file = open(self.get_filename(), "ab")
        while self.running:
            if self.frame_packet:
                if self.frame_packet != self.last_packet:
                    if self.needs_header:
                        dumper_file.write(get_global_header(self.linktype))
                        dumper_file.flush()
                        self.needs_header = False

                    dumper_file.write(self.frame_packet)
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
