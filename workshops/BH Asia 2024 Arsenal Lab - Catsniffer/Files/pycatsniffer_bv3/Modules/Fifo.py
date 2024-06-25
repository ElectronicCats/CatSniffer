import platform
import threading
import time
import os
import logging
import sys
import typer

from . import Pcap
from .Definitions import LINKTYPE_IEEE802_15_4_NOFCS, DEFAULT_TIMEOUT_JOIN

JOIN_TIMEOUT = 1

if platform.system() == "Windows":
    try:
        import win32pipe, win32file, pywintypes
    except ImportError:
        print(
            "\x1b[33;1mError: win32pipe, win32file, pywintypes modules not found. Please install pywin32 package.\x1b[0m"
        )
        exit(1)

DEFAULT_FILENAME = "fcatsniffer"


class Fifo(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

        self.fifo_worker = None
        self.fifo_recv_cancel = False
        self.fifo_need_header = True
        self.fifo_data = []
        self.fifo_packet = None
        self.last_packet = None
        self.linktype = LINKTYPE_IEEE802_15_4_NOFCS
        self.fifo_data_lock = threading.Lock()
        self.daemon = True

    def set_linktype(self, linktype: int):
        self.linktype = linktype

    def stop_thread(self):
        self.join(DEFAULT_TIMEOUT_JOIN)


class FifoLinux(Fifo):
    def __init__(self, fifo_filname: str = DEFAULT_FILENAME):
        super().__init__()
        self.fifo_filname = fifo_filname
        self.fifo_worker = None
        self.fifo_path = os.path.join("/tmp", self.fifo_filname)
        self.type_worker = "pcap"

    def create(self):
        try:
            os.mkfifo(self.fifo_path)
        except OSError as e:
            print(e)

    def open(self):
        if os.path.exists(self.fifo_path) == False:
            self.create()
        try:
            self.fifo_worker = open(self.fifo_path, "ab")
        except OSError as e:
            print(e)

    def run(self):
        # print("RUNNING FIFO")
        self.fifo_recv_cancel = False
        if self.fifo_worker is None:
            self.open()
        while not self.fifo_recv_cancel:
            try:
                if self.fifo_packet:
                    if self.fifo_packet != self.last_packet:
                        if self.fifo_need_header:
                            self.fifo_worker.write(
                                Pcap.get_global_header(self.linktype)
                            )
                            self.fifo_worker.flush()
                            self.fifo_need_header = False
                        # print("WRITING FIFO")
                        self.fifo_worker.write(self.fifo_packet)
                        self.fifo_worker.flush()
                        self.last_packet = self.fifo_packet
                        self.fifo_packet = None

                else:
                    time.sleep(0.01)
            except BrokenPipeError as e:
                self.fifo_need_header = True
                pass
            except Exception as e:
                typer.secho(f"[FIFOLINUX] - {e}", fg=typer.colors.BRIGHT_RED)
                break

    def stop_worker(self):
        self.fifo_recv_cancel = True
        self.fifo_data = []
        if self.fifo_worker:
            self.fifo_worker.close()
        try:
            os.remove(self.fifo_path)
        except FileNotFoundError as e:
            logging.error(e)
            sys.exit(0)
        finally:
            self.join(JOIN_TIMEOUT)

    def add_data(self, data):
        self.fifo_packet = data

    def set_fifo_filename(self, fifo_filname: str):
        self.fifo_filname = fifo_filname

class FifoWindows(Fifo):
    def __init__(self, fifo_filname: str = DEFAULT_FILENAME):
        super().__init__()
        self.fifo_filname = fifo_filname
        self.fifo_worker = None
        self.fifo_path = f"\\\\.\\pipe\\{self.fifo_filname}"
        self.type_worker = "pcap"

    def create(self):
        try:
            self.fifo_worker = win32pipe.CreateNamedPipe(
                self.fifo_path,
                win32pipe.PIPE_ACCESS_DUPLEX,
                win32pipe.PIPE_TYPE_MESSAGE
                | win32pipe.PIPE_READMODE_MESSAGE
                | win32pipe.PIPE_WAIT,
                1,
                65536,
                65536,
                0,
                None,
            )
        except pywintypes.error as e:
            typer.secho(f"[FIFOWINDOWS] - {e}", fg=typer.colors.BRIGHT_RED)

    def open(self):
        if self.fifo_worker is None:
            self.create()
        try:
            win32pipe.ConnectNamedPipe(self.fifo_worker, None)
            logging.info(f"[FIFOWINDOWS] Open {self.fifo_path}")
        except pywintypes.error as e:
            typer.secho(f"[FIFOWINDOWS] - {e}", fg=typer.colors.BRIGHT_RED)

    def run(self):
        try:
            self.fifo_recv_cancel = False
            if self.fifo_worker is None:
                self.open()
            while not self.fifo_recv_cancel:
                if self.fifo_packet:
                    if self.fifo_packet != self.last_packet:
                        if self.fifo_need_header:
                            win32file.WriteFile(
                                self.fifo_worker, Pcap.get_global_header(self.linktype)
                            )
                            win32file.FlushFileBuffers(self.fifo_worker)
                            self.fifo_need_header = False

                        win32file.WriteFile(self.fifo_worker, self.fifo_packet)
                        win32file.FlushFileBuffers(self.fifo_worker)
                        self.last_packet = self.fifo_packet
                        self.fifo_packet = None

                else:
                    time.sleep(0.01)
        except pywintypes.error as e:
            logging.error(e)
            pass

    def stop_worker(self):
        self.fifo_recv_cancel = True
        self.fifo_packet = None
        self.join(DEFAULT_TIMEOUT_JOIN)

    def add_data(self, data):
        self.fifo_packet = data

    def set_fifo_filename(self, fifo_filname: str):
        self.fifo_filname = fifo_filname
        self.fifo_path = f"\\\\.\\pipe\\{self.fifo_filname}"
