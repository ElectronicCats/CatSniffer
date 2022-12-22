import serial
import time
import sys

import binascii
import threading
import logging.handlers
import struct

__version__ = '0.0.1'

defaults = {
    'hex_file': 'ccsniffpiper.hexdump',
    'out_fifo': '/tmp/ccsniffpiper',
    'pcap_file': 'ccsniffpiper.pcap',
    'debug_level': 'WARNING',
    'log_level': 'INFO',
    'log_file': 'ccsniffpiper.log',
    'channel': 11,
}

logger = logging.getLogger(__name__)
stats = {}

class CC1352:

    DEFAULT_CHANNEL = 0x0B  # 11

    DATA_EP = 0x83
    DATA_TIMEOUT = 2500

    DIR_OUT = 0x40
    DIR_IN = 0xc0

    GET_IDENT = 0xc0
    SET_POWER = 0xc5
    GET_POWER = 0xc6

    SET_START = 0xd0  # bulk in starts
    SET_STOP = 0xd1  # bulk in stops
    SET_CHAN = 0xd2  # 0x0d (idx 0) + data)0x00 (idx 1)

    HEARTBEAT_FRAME = 0x01
    COMMAND_FRAME = 0x00

    def __init__(self, callback, port):

        stats['Captured'] = 0
        stats['Non-Frame'] = 0

        self.callback = callback
        self.thread = None
        self.running = False
        self.serial_port = serial.Serial(port, 921600, 8, 'N', 1, timeout=1)
        
    def close(self):
        self.serial_port.close()

    def start(self):
        # start sniffing
        self.running = True
        #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_START)
        self.thread = threading.Thread(target=self.recv)
        self.thread.daemon = True
        self.thread.start()

    def stop(self):
        # end sniffing
        self.running = False
        self.thread.join()
        #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_STOP)

    def isRunning(self):
        return self.running

    def recv(self):

        while self.running:

            if self.serial_port.in_waiting > 0:
	            bytesteam = self.serial_port.read(self.serial_port.in_waiting)
	        #print(bytesteam.hex())
            print ("RECV>> %s" % binascii.hexlify(bytesteam))

            if len(bytesteam) >= 3:
                (cmd, cmdLen) = struct.unpack_from("<BH", bytesteam)
                payload = bytesteam[3:]
                if len(payload) == cmdLen:
                    # buffer contains the correct number of bytes
                    if CC1352.COMMAND_FRAME == cmd:
                        logger.info(f'Read a frame of size {cmdLen}')
                        stats['Captured'] += 1
                        (timestamp,
                         pktLen) = struct.unpack_from("<IB", payload)
                        frame = payload[5:]

                        if len(frame) == pktLen:
                            self.callback(timestamp, frame.tobytes())
                        else:
                            logger.warning(
                                f'Received a frame with incorrect length, pktLen:{pktLen}, len(frame):{len(frame)}'
                            )

                    # elif cmd == CC2531.COMMAND_CHANNEL:
                    #     logger.info('Received a command response: [%02x %02x]' % (cmd, payload[0]))
                    #     # We'll only ever see this if the user asked for it, so we are
                    #     # running interactive.
                    elif CC1352.HEARTBEAT_FRAME == cmd:
                        logger.debug(f'Heartbeat - {payload[0]}')
                    else:
                        logger.warning(
                            'Received a command response with unknown code - CMD:{:02x} byte:{}'
                            .format(cmd, bytesteam))

    def set_channel(self, channel):
        was_running = self.running

        if channel >= 11 and channel <= 26:
            if self.running:
                self.stop()

            self.channel = channel

            # set channel command
            #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_CHAN, 0, 0,
            #                       [channel])
            #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_CHAN, 0, 1,
            #                       [0x00])

            self.get_channel()

            if was_running:
                self.start()

        else:
            raise ValueError("Channel must be between 11 and 26")

    def get_channel(self):
        return self.channel

    def __repr__(self):

        if self.dev:
            return "%s <Channel: %d>" % (self.name, self.channel)
        else:
            return "Not connected"
